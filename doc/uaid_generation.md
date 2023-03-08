## saparid UAID generation

Because UAIDs need to be generated after a client hello, I cannot hash for instance a username to generate one. 
(For simplicity, I'm assuming IDs have to be unique, and enforcing this by generating them using something simpler than a counter)


### So, what does the official server do?

just increments a counter, pretty much. This is "unique" and probably works fine, and I'll probably even use it for a bit, lol


### A general idea/"algorithm" for a more unique UAID generation system

I can hash, using a non-cryptographic (the data itself will be scrambled, anyways) hash:
- client IP
- client port (this is actually very good to add, since the client port is epheremal and will only change on RST)
- some garbage random data (just to reduce chances of collisions)

maybe later on:
- client rDNS hostname (what if a valid IP doesn't have an rDNS?)
	(this would really make it way harder to brute force the data though, as it WOULD add quite a few bytes of additional data into the hash)

For safety's sake, before hashing, the data will be scrambled.

In C++ this would look something like (for a quick and dirty implementation, using MurmurHash3):

```cpp
struct UaidGenerator {

	struct Data {
		net::ip::address clientIp;
		u16 clientPort;
		std::array<u32, 4> garbage;
	};
	

	u32 Hash(const Data& data) const {
		std::vector<u8> dataBytes(32);
		u32 hashOutput{};
		Data copy = data;

		// add random garbage
		for(auto& val : copy.garbage)
			val = static_cast<u32>(xoshiro());
		
		// Add data to the buffer
		AddMember(dataBytes, copy.clientIp);
		AddMember(dataBytes, copy.clientPort);
		AddMember(dataBytes, copy.garbage);

		// Shuffle the bytes around randomly
		std::random_shuffle(dataBytes.begin(), dataBytes.end(), xoshiro);

		// hash that out
		MurmurHash3_x86_32(static_cast<const void*>(dataBytes.data()), dataBytes.size(), static_cast<u32>(xoshiro()), &hashOutput);
		return hashOutput;
	}

private:

	template<class T>
	static void AddMember(std::vector<u8>& buffer, const T& data) {
		auto* bytes = std::bit_cast<u8*>(data);
		for(std::size_t i = 0; i < sizeof(T); ++i)
			buffer.push_back(data[i]);
	}

	template<class T, std::size_t N>
	static void AddMember<std::array<T,N>>(std::vector<u8>& buffer, const std::array<T, N>& data) {
		for(auto& member : data)
			AddMember<T>(member);
	}
	
	template<class CharT>
	static void AddMember<std::basic_string<CharT>>(std::vector<u8>& buffer, const std::basic_string<CharT>& str) {
		for(auto c : str)
			AddMember(c);
	}

	template<class CharT>
	static void AddMember<std::basic_string_view<CharT>>(std::vector<u8>& buffer, std::basic_string_view<CharT> view) {
		for(auto c : view)
			AddMember(c);
	}

	template<>
	static void AddMember<net::ip::address>(std::vector<u8>& buffer, const net::ip::address& address) {
		if(address.is_v4()) {
			AddMember(address.to_v4().to_bytes());
		} else {
			AddMember(address.to_v6().to_bytes());
		}
	}

	// state
	std::random_device rd{};
	Xoshiro256ss xoshiro(rd);
};
```

The result of Hash() would then be a unique UAID for a client's connection.

### Concerns

- If certain data were known (client IP address) it may be possible to speed up cracking the hash
	- Note that the hash would have to be extracted from a packet capture/dump first..

- Murmur3 is a bit insecure
	- As a stopgap we randomly shuffle the data, so it is effectively meaningless unless you know other data.
	- We could use SipHash with a randomly-generated key, which is unique to every server startup
	  (the data does not need to be authenticated/decoded again, so this is good enough)
