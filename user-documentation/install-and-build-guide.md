# install and build guide

#### install dependencies:

```bash
yay -S pkg-config cmake make
```

#### Install Conan:

```bash
$ pip install conan
$ conan profile new default --detect
$ conan profile update conf.tools.system.package_manager:mode=install default;
$ conan profile update conf.tools.system.package_manager:sudo=True default;
```

### How to Build

### Automatic build for Linux and Windows

For this to work on Windows you need to have access to a Unix shell, such as Git Bash or WSL as well as access to the make command.

In order to build both the client and the server, you can make the "all" rule:

```bash
$ make
```

In order to build the tests, make the "test" rule:

```bash
$ make test
```

Binaries can be found at:

```bash
./bin/Server
./bin/Client
./bin/Tests
```

### Manual Build

#### For Linux

Delete the build folder if present:

```bash
$ rm -rf build
```

Then build the binaries:

```bash
$ mkdir build
$ cd build
$ conan install . --build=missing
$ cmake .. -G "Unix Makefiles"
$ cmake --build . --config Release
```

#### For Windows

Delete the build folder if present:

```bash
$ Remove-Item 'build' -Recurse
```

Then build the binaries:

```bash
$ New-item -itemtype directory build
$ Set-Location build
$ conan install . --build=missing
$ cmake .. -G "Visual Studio 17"
$ cmake --build . --config Release
```
