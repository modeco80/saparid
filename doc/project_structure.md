## Saparid project structure

- libvscp
	- library for (de)serializing VSCP/vsap data
- libvscp-io
	- library for writing serialized VSCP/VSAP data to Boost.Asio async streams
		- consumes `libvscp`
- saparid
	- the main project
		- consumes `libvscp-io`
