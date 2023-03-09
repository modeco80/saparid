#pragma once

#include <saparid/proto/WireTypes.hpp>
#include <variant>

namespace saparid::proto {

	namespace detail {
		using ClientHeaderTypes = std::variant<
			client::Type0Message
		>;

		template<std::size_t index, class Variant, class Buffer>
		void InitAndReadIndex(Variant& variant, Buffer& buffer) {
			variant.template emplace<index>();
			meta::ReadObject(std::get<index>(variant), buffer);
		}

		template<class Buffer>
		struct ClientMessage {

			ClientMessage(Buffer& buffer)
				: buffer(buffer) {
			}

			void Read() {
				meta::ReadObject(messageHeader, buffer);
				ReadPayload();
			}

			void ReadPayload() {
				using enum shared::MethodType;
				switch(messageHeader.methodType) {
					case Type0:
						InitAndReadIndex<0>(payloadHeader, buffer);
						ReadType0Payload();
						break;


					default:
						break;
				}
			}

			void ReadType0Payload() {
				using enum client::Type0Message::Op;
				auto& header = std::get<0>(payloadHeader);

				switch(header.op) {
					case Register:
						InitAndReadIndex<0>(type0Payloads, buffer);
						break;
					case Deregister:
						InitAndReadIndex<1>(type0Payloads, buffer);
						break;
				}
			}

		   private:
			Buffer buffer;
			shared::VscpMessageHeader messageHeader;
			ClientHeaderTypes payloadHeader;

			// Eghhgghghhghg maybe fix this
			client::Type0Message::Payloads type0Payloads;
		};
	}

	using detail::ClientMessage;
}