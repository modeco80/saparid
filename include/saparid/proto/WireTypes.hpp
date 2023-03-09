//
// Saparid
//
// (C) 2023 modeco80 <lily.modeco80@protonmail.ch>
//

// This file provides the wire types 

#pragma once

#include <cstring>
#include <saparid/common/CommonTypes.hpp>
#include <saparid/common/SpanBuffer.hpp>
#include <saparid/meta/meta.hpp>

namespace saparid::proto {

	/** VSCP version helper. As an aside: this type is wire compatible with the client Hello */
	struct [[gnu::packed]] VscpVersion {
		u8 major;
		u8 minor;

		// TODO: When clang gets decent C++23 deducing this support, replace this
		friend constexpr bool operator==(const VscpVersion version, const VscpVersion other) { return version.major == other.major && version.minor == other.minor; }
	};

	/** Supported VSCP protocol version. */
	constexpr static auto SUPPORTED_VSCP_VERSION = VscpVersion { .major = 1, .minor = 1 };

	namespace shared {

		// i really should just make a type string thing that excludes the \0 from length
		// but still stores it, this is painful to read and alter
		constexpr static char HELLO_MAGIC[5] = { 'h', 'e', 'l', 'l', 'o' };
		constexpr static char REJECT_MAGIC[6] = { 'r', 'e', 'j', 'e', 'c', 't' };

		// This seems to be shared between VSAP and VSCP.
		enum class MethodType : u8 {
			Type0 = 0,

			Type1, // also known as SYS1
			Type2, // also known as SYS2

			Hello, // ?
			Hella = Hello,
			Reject,

			Unk = 6
		};

		enum class Strategy : u32 {
		
		
		};

		template<class TPayloadHeader>
		struct BasicMessageHeader {
			MethodType methodType;
			u32 unk;	 // this could be source?
			u32 srcUaid; // i think..?
			TPayloadHeader payloadHeader;
		};

	} // namespace shared

	/**
	 * Client -> server protocol messages
	 */
	namespace client {

		struct [[gnu::packed]] Hello {
			char magic[5]; ///< Must be "hello"
			VscpVersion clientVscpVersion;

			[[nodiscard]] bool Ok() const { 
				return !std::memcmp(&shared::HELLO_MAGIC[0], &magic[0], sizeof(shared::HELLO_MAGIC)) &&
					   clientVscpVersion == SUPPORTED_VSCP_VERSION; 
			}
		};

		static_assert(sizeof(Hello) == 0x7, "invalid client Hello size");

		// TYPE 0/ Sys0

		struct Type0PayloadHeader {
			enum class Op : u32 {
				Register,
				Deregister,
				AddObject,
				RemoveObject,
				Myself, //?
				UseObject,
				AppMsg,
				Quit
			};

			Op op;
			u32 dataSize;

			/** Get the unparsed data buffer. */
			[[nodiscard]] common::SpanBuffer Data() {
				return { std::bit_cast<u8*>(this + 1), dataSize };
			}

			/** Parse the packet data buffer into a data structure of type \ref Payload */
			template<class Payload>
			Payload DataAs() {
				Payload p;
				auto buffer = Data();
				meta::ReadObject(p, buffer);
				return p;
			}
		};
		
		struct Type0RegisterPayload {
			std::string username;
			std::string avatarWRLPath;
		};

		using Type0MessageHeader = shared::BasicMessageHeader<Type0PayloadHeader>;


	} // namespace client

	/**
	 * Server -> client protocol messages
	 */
	namespace server {

		struct [[gnu::packed]] Hello {
			char magic[6];	///< can be "hello" or "reject".

			u32 unkzero;	///< idk. this is explicitly set to zero by the code
			u32 selfUaid;	///< the UAID of the client we will be sending this to (generated)

			// factories to create the right things
			// this is pain
			constexpr static auto CreateHello(u32 uaid) { return Hello { .magic = { 'h', 'e', 'l', 'l', 'o', '\0'}, .unkzero = 0, .selfUaid = uaid }; }
			constexpr static auto CreateReject(u32 uaid) { return Hello { .magic = { 'r', 'e', 'j', 'e', 'c', 't' }, .unkzero = 0, .selfUaid = uaid }; }
		};


		static_assert(sizeof(server::Hello) == 0xe, "invalid server Hello size");
	} // namespace server
} // namespace saparid::proto


namespace saparid::meta::detail {


	template<>
	auto Schema<::saparid::proto::client::Type0RegisterPayload>() {
		using ::saparid::proto::client::Type0RegisterPayload;
		return kumi::make_tuple(
			zstring_<"username", &Type0RegisterPayload::username>{}, 
			zstring_<"avatarWRLPath", &Type0RegisterPayload::avatarWRLPath>{} 
		);
	}

}
