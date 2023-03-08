## Saparid project structure

- common
	- core library. provides asio defines and implementation (we compile asio seperately here), and other stuff we might want to use elsewhere
		- consumes Boost.Asio & others

- meta
	- Metastructure library. Provides simple static data reflection capabilities, and allows for (de)seralizing data
	  structures from a user-provided buffer.

- proto
	- library for (de)serializing VSCP messages from a buffer, using the metastructure library
	- provides `proto::Message` which is a message backed by a user-provided buffer, which provides the payloads
		- payloads which have strings/such use `std::string_view` pointing to this buffer,
			which avoids making copies when deserializing. the buffer itself has to last
			longer than the message however
	- consumes `common` and `meta`

- world
	- library for running the server world(s)

- saparid
	- the main project
		- consumes `common`, `vscp`, and `world`


## Client states and what they mean

When a client connects we go into the Accept state, then transition into the Hello state.

The Hello state expects the client to send a client hello packet, and nothing else.
If the client sends any other data other than a client hello when in this state, we transition to the Close state.

When the server gets a client hello, it will send back a hello response or reject (depending on if the client is banned).
If the server sends a reject, then after sending it we transition to the Close state.

If the server accepts the client, the next and final state is Vscp, where VSCP client data is expected.
Any invalid data will transition to the Close state.

The Close state marks us closing the connection (and removing the player object from the world).

A "simple" text format of these states would look something like:

```
init state Accept
		-> Hello

state Hello
		|> Close			# (initated by client or protocol error)
		-> Reject			# (initated by server if connection rejected)
		-> Vscp

state Reject
		|> Close

state Vscp 
		|> Close			# (initated by client or protocol error)
		-> Vscp				# (essentially a infinite loop)

terminal state Close
```

...where:

```
state <ident> defines a state
init state <ident> defines a initial state, which is the first state the machine will enter
terminal state <ident> defines a terminal state (which has no transition map)

-> defines a non-terminal transition (which can be self-refencing, for state loops)
|> defines a terminal transition (no more transitions can be made)
```

## Code Sketching

- Server
	- Manages server I/O for clients
	- `SocketId` -> a unique ID for a specific client socket. 
		The server class stores no actual client data besides the session class, which is internal to the Server,
		and not exposed to the rest of the codebase, and not the world

	- Functions:
		- `Close(SocketId)` -> Close a socket forcefully. Will fire OnDisconnect() to allow cleanup.
		- `SendBytes(SocketId, Buffer&)` ?
		- `SendMessage(SocketId, shared_ptr<vscp::Message>)` -> Send a VSCP message to a socket.

	- Signals:
		- `HelloResponse OnHello(SocketId, vscp::proto::client::Hello helloMessage)` -> hello
		- `void OnMessage(SocketId, unique_ptr<vscp::Message>)` -> Event to handle VSCP messages
		- `void OnConnect(SocketId)`	-> Event to handle connections.
		- `void OnDisconnect(SocketId)` -> Event to handle disconnects, this will be used by the world to clean up any player objects

The world does not explicitly do I/O. The Server class takes care of handling Boost.Asio on a lower level, parsing the messages,
and provides the higher level abstraction via signals to the world. A mock implementation of Server could in theory be implemented.

All worlds run on the main thread in round-robin ordering (just a range-for), and are polled after the Asio event loop is polled.

The world's signal handlers can enqueue data for further processing on the next server poll, to avoid concurrency issues.

This could be changed, but for now should be scalable enough.

- world::World

- world::WorldObject (base class for objects in the world)
	- world::WorldCollidibleObject (base class for collidible obects)
		- world::WorldPlayer
			- world::WorldMockPlayer (a "mock" player simulated by the server)
				could be used to implement some fun things?
			- world::WorldRemotePlayer (this is what typically will get instanted)
