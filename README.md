# install and build guide

[![drawing](https://camo.githubusercontent.com/9cc8cfc78a665ccad1cb61a8fb2203eb5e599f1ac6b8c292831941f35fa79728/68747470733a2f2f63646e2e6472696262626c652e636f6d2f75736572732f3636343639372f73637265656e73686f74732f363435383736332f722d6c6f676f2e706e67)](https://camo.githubusercontent.com/9cc8cfc78a665ccad1cb61a8fb2203eb5e599f1ac6b8c292831941f35fa79728/68747470733a2f2f63646e2e6472696262626c652e636f6d2f75736572732f3636343639372f73637265656e73686f74732f363435383736332f722d6c6f676f2e706e67)

[![](https://camo.githubusercontent.com/4f4c5ec676ef2eca49e35611629357ed5beb2b1644e69eec4ddc0bd3b0355c6e/68747470733a2f2f62616467656e2e6e65742f6769746875622f73746172732f457069746563682d522d547970652f522d547970653f636f6c6f723d707572706c65)](https://camo.githubusercontent.com/4f4c5ec676ef2eca49e35611629357ed5beb2b1644e69eec4ddc0bd3b0355c6e/68747470733a2f2f62616467656e2e6e65742f6769746875622f73746172732f457069746563682d522d547970652f522d547970653f636f6c6f723d707572706c65) [![](https://camo.githubusercontent.com/828bb94d0c36e4bc410f656f7ae9753ad00c96db6acba5b82a8a9f440514f8f7/68747470733a2f2f62616467656e2e6e65742f6769746875622f636f6e7472696275746f72732f457069746563682d522d547970652f522d547970653f636f6c6f723d677265656e)](https://camo.githubusercontent.com/828bb94d0c36e4bc410f656f7ae9753ad00c96db6acba5b82a8a9f440514f8f7/68747470733a2f2f62616467656e2e6e65742f6769746875622f636f6e7472696275746f72732f457069746563682d522d547970652f522d547970653f636f6c6f723d677265656e) [![](https://camo.githubusercontent.com/165b1c6620f7ced61a38bd637d3e19562bc4be68daa116e0bc8fcb857c186da2/68747470733a2f2f62616467656e2e6e65742f6769746875622f6272616e636865732f457069746563682d522d547970652f522d547970653f636f6c6f723d70696e6b)](https://camo.githubusercontent.com/165b1c6620f7ced61a38bd637d3e19562bc4be68daa116e0bc8fcb857c186da2/68747470733a2f2f62616467656e2e6e65742f6769746875622f6272616e636865732f457069746563682d522d547970652f522d547970653f636f6c6f723d70696e6b) [![](https://camo.githubusercontent.com/c7dcb072f71033004b1812d5a576cee55b489807c55701c92a48fbcb7c8426a9/68747470733a2f2f62616467656e2e6e65742f6769746875622f636f6d6d6974732f457069746563682d522d547970652f522d547970652f6d61696e3f636f6c6f723d6f72616e6765)](https://camo.githubusercontent.com/c7dcb072f71033004b1812d5a576cee55b489807c55701c92a48fbcb7c8426a9/68747470733a2f2f62616467656e2e6e65742f6769746875622f636f6d6d6974732f457069746563682d522d547970652f522d547970652f6d61696e3f636f6c6f723d6f72616e6765)

[![](https://github.com/Epitech-R-Type/R-Type/actions/workflows/build\_linux.yml/badge.svg?branch=main)](https://github.com/Epitech-R-Type/R-Type/actions/workflows/build\_linux.yml/badge.svg?branch=main) [![](https://github.com/Epitech-R-Type/R-Type/actions/workflows/build\_windows.yml/badge.svg?branch=main)](https://github.com/Epitech-R-Type/R-Type/actions/workflows/build\_windows.yml/badge.svg?branch=main) [![](https://github.com/Epitech-R-Type/R-Type/actions/workflows/linter.yml/badge.svg?branch=main)](https://github.com/Epitech-R-Type/R-Type/actions/workflows/linter.yml/badge.svg?branch=main)

### Table of contents

Installation

Contributing Guide

### R-Type

This project aims to implement the classic game [R-Type](https://wikiless.sethforprivacy.com/wiki/R-Type?lang=en).\
It will include network based multiplayer.

#### install dependencies:

```
yay -S pkg-config cmake make
```

#### Install Conan:

```
$ pip install conan
$ conan profile new default --detect
$ conan profile update conf.tools.system.package_manager:mode=install default;
$ conan profile update conf.tools.system.package_manager:sudo=True default;
```

### How to Build

### Automatic build for Linux and Windows

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

### Manual Build

#### For Linux

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

#### For Windows

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
