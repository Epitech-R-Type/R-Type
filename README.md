# R-Type

<p align="center">
    <img src="https://cdn.dribbble.com/users/664697/screenshots/6458763/r-logo.png" alt="drawing" width="200">
    <p align="center">
        <img src="https://badgen.net/github/stars/Epitech-R-Type/R-Type?color=purple">
        <img src="https://badgen.net/github/contributors/Epitech-R-Type/R-Type?color=green">
        <img src="https://badgen.net/github/commits/Epitech-R-Type/R-Type/main?color=orange">
    </p>
    <p align="center">
        <img src="https://github.com/Epitech-R-Type/R-Type/actions/workflows/build_linux.yml/badge.svg?branch=main">
        <img src="https://github.com/Epitech-R-Type/R-Type/actions/workflows/build_windows.yml/badge.svg?branch=main">
    </p>
</p>

This project aims to implement the classic game [R-Type](https://wikiless.sethforprivacy.com/wiki/R-Type?lang=en).\
This implementation is **fully networked.**

## Building

Please follow the [install and build guide](user-documentation/install-and-build-guide.md) included in the documentation.

## Features

### Gameplay Features

- Cross platform **Windows** and **Linux**
- **Multiplayer** game support
- Lobby **Creation** / **Joining**
- Basic **side scroller shoot'em'up** controls

### Implementation Features

- Game logic contained **only in server**
- Multithreaded Server
    - **Multithreaded** lobby handling
    - **Asynchronous** & **Multithreaded** network abstraction
- Lightweight **Entity Component System** implementation
- **Reusable** networking encapsulation
- **ECS serialization** based **binary** network protocol
- **Strong** suite of unit tests

### Implementation Details

This R-Type implementation was done using [Raylib](https://github.com/raysan5/raylib) for the graphics side of things and [Asio](https://github.com/chriskohlhoff/asio) for the networking library.

Most of the organization was done using **github issues and projects** along with **github actions** for the CI. Finally, we used [GTest](https://github.com/google/googletest) as testing framework.

## Documentation

In this section you will find all relevant documentation to this project.

### Gitbook Documentation

All of the documentation can be found on [gitbooks.](https://r-type-1.gitbook.io/r-type-documentation/)

### User Documentation

[Table of Contents](user-documentation/)
- [Install and Build Guide](user-documentation/install-and-build-guide.md)
- [Tutorials](user-documentation/tutorials/)
- [System Overview](user-documentation/systems-overview.md)
- [License](user-documentation/license.md)

### Developer Documentation

[Table of Contents](dev-documentation/)
- [Client Documentation](dev-documentation/client-documentation.md)
- [communication protocol](dev-documentation/comunication-protocol.md)
- [ECS documentation](dev-documentation/ecs-documentation.md)
- [team/coding conventions](dev-documentation/team-coding-conventions.md)
