#pragma once

#include <saparid/proto/WireTypes.hpp>
#include <variant>

namespace saparid::proto {

	namespace detail {


		template<class T, class Variant, class Buffer>
		void ReadIntoVariant(Variant& variant, Buffer& buffer) {
			T obj{};
			meta::ReadObject(obj, buffer);
			variant = obj;
		}

		template<class Buffer>
		struct ClientMessage {

			explicit ClientMessage(Buffer& buffer)
				: buffer(buffer) {
			}

			void Read() {
				using namespace shared;
				using enum MethodType;
				meta::ReadObject(messageHeader, buffer);

				// clang-format off

				// Read the payload data
				switch(messageHeader.methodType) {
					case Type0: ReadType0(); break;
					default: break;
				}
				// clang-format on
			}

			void ReadType0() {
				using namespace client;
				using enum Type0Message::Op;
				Type0Message readPayloadHeader{};
				meta::ReadObject(readPayloadHeader, buffer);

				// ?
				//if(header_size > buffer.size() )
				// return;

				auto dataBuffer = Buffer{ buffer.data(), buffer.size() };

				// clang-format off
				switch(readPayloadHeader.op) {
					case Register: ReadIntoVariant<Type0Message::RegisterPayload>(type0Payloads, dataBuffer); break;
					case Deregister: ReadIntoVariant<Type0Message::DeregisterPayload>(type0Payloads, dataBuffer); break;
					default: break;
				}
				// clang-format on

				payloadHeader = readPayloadHeader;
			}

			// accessors

			shared::VscpMessageHeader& TopLevelHeader() {
				return messageHeader;
			}

			template<class T>
			auto& HeaderAs() {
				return std::get<T>(payloadHeader);
			}

			template<class T>
			auto& DataAs() {
				using enum shared::MethodType;
				switch(messageHeader.methodType) {
					case Type0: return std::get<T>(type0Payloads);
					default: break;
				}
			}

		   private:
			Buffer buffer;

			shared::VscpMessageHeader messageHeader{};
			client::HeaderTypes payloadHeader{};

			client::Type0Message::Payloads type0Payloads{};
		};
	}

	using detail::ClientMessage;
}