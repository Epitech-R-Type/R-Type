[Contributing Guide](CONTRIBUTING.md)

# R-Type

This project aims to implement the classic game [R-Type](https://wikiless.sethforprivacy.com/wiki/R-Type?lang=en).\
It will include network based multiplayer.

# Install Conan

```pip install conan```

```conan profile new default --detect```

Add the following to ~/.conan/profiles/default if not present.

```
[conf]
tools.system.package_manager:sudo=True
tools.system.package_manager:mode=install
```

# How to Build

### For Linux
```chmod u+x ./setup.sh```\
```./setup.sh```

### For Windows
N/A