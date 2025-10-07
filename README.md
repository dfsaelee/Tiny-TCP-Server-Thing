## Tiny Server Thingy

A tiny C TCP server used for demos and learning. It listens on port 8080 by default, accepts one connection at a time, sends a greeting, prints any data received from the client, then closes the connection. :D 

### Quick start with cmake and gcc:
```bash
mkdir -p build && cd build
cmake ..
make
./simpleServer
```