## Tiny TCP Server

A tiny C TCP server used for demos and learning. It listens on port 8080 by default, accepts one connection at a time, sends a greeting, prints any data received from the client, then closes the connection. :D 

A big shoutout to [beej's](https://beej.us/guide/bgnet/html/#system-calls-or-bust) Guide for reference!

### Quick start with cmake and gcc:
```bash
mkdir -p build && cd build
cmake ..
make
./simpleServer
```