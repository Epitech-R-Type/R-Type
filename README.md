[Contributing Guide](CONTRIBUTING.md)

# R-Type

This project aims to implement the classic game [R-Type](https://wikiless.sethforprivacy.com/wiki/R-Type?lang=en).

It will include network based multiplayer.


# How to Build

mkdir build && cd build
conan install ..

### For Linux
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build .

### For Windows
cmake .. -G "Visual Studio 16"
cmake --build . --config Release