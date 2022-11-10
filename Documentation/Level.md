# Level Documentation

## Introduction
This documentation is about how a level file works. 

## Adding a new level file
Level files are stored in src/server/levels and have to be compiled with cmrc, to add them to the compilation process, edit the CMakeLists.txt file. In function(build_server) is the cmrc_add_resource_library part, there you see all the files that are compiled with the server. Add the file path the same way the others are listed.

## Content of a level file
First there are all the generel variables that always apply, then there is a free line followed by the wave variables, each wave is seperated by a free line. There is no \n after the last variable.
Every variable is seperated by a \n.
The space and the parentheses and content are not in the file.
BackgroundId (AnimationID)
BaseMusic (SongID)

MinSpawn (int)
MaxSpawn (int)
SpawnInterval (float)
EnemyTypes (EnemyID, ...)
BossType (BossID)
BossSpawnTimer (int)


The AnimationID and SongID enum are defined in the Animation component (Documentation/Client.md).
The EnemyID and BossID enum are defined.

General variables:
BackgroundId describes which sprite should be used for the background, it will only use the numbers corresponding to a background.
BaseMusic is the music played when there is no boss.

Wave variables:
MinSpawn is the number of minumum spawned enemies per wave.
MaxSpawn is the number of maximum spawned enemies per wave.
SpawnInterval is the amount of seconds (float) between two enemies spawned.
EnemyTypes is a list of integers seperated by a space.
BossType is the enum integer describing the boss, this includes the music.
BossSpawnTimer is the amount seconds until the boss spawns.

<!-- ## Implemented Enemys and Bosses -->

## Tool for creating a level file
There is an executable called CreateFile. With this programm you can create your own level file more easily, it will ask you questions about the level you want to create and then create the coresponding level file in the right place. The name is "level" + the next viable level number.
It will ask you questions and you have to answer with a viable input.
For BackgroundId, BaseMusic, EnemyTypes, BossType you have to put the enum value.A variable will be checked to confirm it exists.