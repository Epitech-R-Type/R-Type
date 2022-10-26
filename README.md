<img src="https://cdn.dribbble.com/users/664697/screenshots/6458763/r-logo.png" alt="drawing" width="200" align="center"/>

![example workflow](https://github.com/Epitech-R-Type/R-Type/actions/workflows/build_linux.yml/badge.svg?branch=main) ![example workflow](https://github.com/Epitech-R-Type/R-Type/actions/workflows/build_windows.yml/badge.svg?branch=main) ![example workflow](https://github.com/Epitech-R-Type/R-Type/actions/workflows/linter.yml/badge.svg?branch=main)

[Contributing Guide](CONTRIBUTING.md)

# R-Type

This project aims to implement the classic game [R-Type](https://wikiless.sethforprivacy.com/wiki/R-Type?lang=en).\
It will include network based multiplayer.

# Install Conan

```
$ pip install conan
$ conan profile new default --detect
$ conan profile update conf.tools.system.package_manager:mode=install default;
$ conan profile update conf.tools.system.package_manager:sudo=True default;
```

# How to Build

## Automatic build for Linux and Windows

For this to work on Windows you need to have access to a Unix shell, such as Git Bash or WSL as well as access to the make command.

In order to build both the client and the server, you can make the "all" rule:

```
$ make
```

In order to build the tests, make the "test" rule:

```
$ make test
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
$ rm -rf build
```

Then build the binaries:

```
$ mkdir build
$ cd build
$ conan install . --build=missing
$ cmake .. -G "Unix Makefiles"
$ cmake --build . --config Release
```

### For Windows

Delete the build folder if present:

```
$ Remove-Item 'build' -Recurse
```

Then build the binaries:

```
$ New-item -itemtype directory build
$ Set-Location build
$ conan install . --build=missing
$ cmake .. -G "Visual Studio 17"
$ cmake --build . --config Release
```
