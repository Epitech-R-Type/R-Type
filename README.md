[Contributing Guide](CONTRIBUTING.md)

# R-Type

This project aims to implement the classic game [R-Type](https://wikiless.sethforprivacy.com/wiki/R-Type?lang=en).\
It will include network based multiplayer.

# Install Conan

```
$ pip install conan
$ conan profile new default --detect
```

Under Linux add the following to ~/.conan/profiles/default if not present.

```
[conf]
tools.system.package_manager:sudo=True
tools.system.package_manager:mode=install
```

# How to Build

## Automatic build for Linux and Windows

For this to work on Windows you need to have access to a Unix shell, such as Git Bash or WSL.

Make `build.sh` executable if needed, then execute it.

```
$ ./build.sh
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
