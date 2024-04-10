# SPlat

2D platforming game engine based in C++ and built on top of the SFML multimedia suite.

## Testing

Tests (via Docker):

```bash
docker build -t SPlat/test . && docker run -it --rm SPlat/test
```

## Libraries

I use the following libraries in this implementation:
* [SFML](https://www.sfml-dev.org/) for core engine and multimedia
* [cereal](https://uscilab.github.io/cereal/index.html) for serialization
* [CMake](https://cmake.org) for testing

Thanks to all 3rd parties for their contributions!
