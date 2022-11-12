# Level Documentation

## Introduction
This documentation is about how a level file works.

## Adding a new level file
Level files are stored in src/server/levels and have to be compiled with cmrc, to add them to the compilation process, edit the CMakeLists.txt file. In function(build_server) is the cmrc_add_resource_library part, there you see all the files that are compiled with the server. Add the file path the same way the others are listed.

## Content of a level file
First there are all the general variables that always apply, then there are the wave variables. Every variable is seperated by a newline but there is no newline after the last variable.

    General variables:
|Variable|Type|
| :-: | :-: |
|BackgroundId|AnimationID|
|BaseMusic|SongID|

    Wave variables:
|Variable|Type|
| :-: | :-: |
|MinSpawn|int|
|MaxSpawn|int|
|SpawnInterval|float
|EnemyTypes|int, ...|
|BossType|int|

    The AnimationID, SongID:
```c++
enum SongID {
    BOSS,
    NORMAL,
    PIANO,
};

enum AnimationID {
    Orb,
    Vortex,
    Cluster,
    Laser,
    Lost,
    SpaceshipLightblue,
    SpaceshipPink,
    SpaceshipGreen,
    SpaceshipRed,
    SpaceshipDarkblue,
    SpaceshipRGB,
    Background,
    Death,
};
```

    General variables:

**BackgroundId** describes which sprite should be used for the background. Keep in mind that there are other AnimationID's. Which means the first background has the value 11.

**BaseMusic** is the music played when there is no boss.

    Wave variables:

**MinSpawn** is the number of minumum spawned enemies per wave.

**MaxSpawn** is the number of maximum spawned enemies per wave.

**SpawnInterval** is the amount of seconds (float) between two enemies spawned.

**EnemyTypes** is a list of integers seperated by a space. The integers represent an index of the following vector:

```c++
std::vector<EnemyStats> _enemys = {
    {10, 5, Animation::AnimationID::Orb, 8.0, Armament::Type::Laser},
    {20, 25, Animation::AnimationID::Orb, 10.0, Armament::Type::Laser},
    {30, 15, Animation::AnimationID::Orb, 3.0, Armament::Type::Laser},
    {50, 50, Animation::AnimationID::Orb, 1.0, Armament::Type::Laser},
};

struct EnemyStats {
    int health;
    int damage;
    Animation::AnimationID sprite;
    float speed;
    Armament::Type armament;
};
```

**BossType** is the integer describing the boss, this includes the music. It is an index of the following vector:

```c++
std::vector<BossStats> _bosses = {
        {300, 20, Animation::AnimationID::Cluster, Armament::Type::Buckshot},
        {400, 50, Animation::AnimationID::Cluster, Armament::Type::Laser},
};

struct BossStats {
    int health;
    int damage;
    Animation::AnimationID sprite;
    Armament::Type armament;
};
```

<!-- ## Implemented Enemys and Bosses -->

## Tool for creating a level file
There is an executable called CreateFile. With this programm you can create your own level file more easily, it will ask you questions about the level you want to create and then create the coresponding level file in the right place. The name of the created file is "level" + the next viable number.

It will ask you questions and you have to answer with a viable input.

As answer you have to put a viable value. If needed a variable will be checked to confirm your answer exists.