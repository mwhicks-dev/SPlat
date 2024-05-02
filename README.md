# SPlat

2D platforming game engine based in C++ and built on top of the SFML multimedia suite.

## Testing

Tests (via Docker):

```bash
docker build -t SPlat/test . && docker run -it --rm SPlat/test
```

## Build

Build (via Docker):

1. Go to the `build/` directory.
2. Execute `make -j$(nproc) client server` to build the client-server.
3. Execute `./server` to run the server demo.
4. Execute `./client` to run the client demo.

## Libraries

I use the following libraries in this implementation:
* [SFML](https://www.sfml-dev.org/) for core engine and multimedia
* [cereal](https://uscilab.github.io/cereal/index.html) for serialization
* [CMake](https://cmake.org) and [Docker](https://www.docker.com) for testing

Thanks to all 3rd parties for their contributions!
