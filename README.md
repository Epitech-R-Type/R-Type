[Contributing Guide](CONTRIBUTING.md)

# R-Type

This project aims to implement the classic game [R-Type](https://wikiless.sethforprivacy.com/wiki/R-Type?lang=en).\
It will include network based multiplayer.

# Install dependencies

```
yay -S pkg-config cmake make
```

# Install Conan

```
pip install conan
conan profile new default --detect
conan profile update conf.tools.system.package_manager:mode=install default;
conan profile update conf.tools.system.package_manager:sudo=True default;
```

# How to Build

## Automatic build for Linux and Windows

For this to work on Windows you need to have access to a Unix shell, such as Git Bash or WSL as well as access to the make command.

In order to build both the client and the server, you can make the "all" rule:

```
make
```

In order to build the tests, make the "test" rule:

```
make test
```

Binaries can be found at:
```
./bin/Server
./bin/Client
./bin/Tests
```

## Manual Build
### For Linux

Delete the build folder if present:

```
rm -rf build
```

Then build the binaries:

```
conan install conanfile.txt -if build --build=missing
cmake -S . -B build
cmake --build build
```

### For Windows

Delete the build folder if present:

```
rmdir /s build
```

Then build the binaries:

```
conan install conanfile.txt -if build --build=missing
cmake -S . -B build
cmake --build build
```
