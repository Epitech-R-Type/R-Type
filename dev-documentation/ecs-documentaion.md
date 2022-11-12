# ECS documentaion

### **ECS Documentation**

### **Introduction**

This Documentation is meant to explain how the ECS works, the existing components and the Systems that work on them.

### **Systems**

Every system exept the factory has an apply function. This function is called every iteration of the mainloop and will update everything that has to be updated. If a system changes an entity on the server it will push them to the modifiedEntity vector, after all systems are applied all modified entitys will be send to the clients.

#### **Shared Systems**

The apply function will first check for entitys with health below or equal zero and delete them. If it is a player, that died, the function will also push their UUID to a designated vector. Also after that every entity outside of a box that is a bit bigger than the screen will get deleted. This is necessary to save memory.

#### **Server-only Systems**

The factory does not have an apply function, as it is only there to create entitys. This includes makePlayer, makeEndboss, makeEnemy, makeLaser, makeBuckshot and makeBackground. The player, endboss and enemy functions give the entity the following components: Position, Animation, Health, Armament, Velocity, Hitbox, Team, ImmunityFrame, Damage and CollisionEffect. The boss doesn't have a Velocity component. The player also has the player component. The boss and enemy function take a struct to modify their components.This struct is definied in. The makeLaser and makeBuckshot create one or more bullets using the bullet function. This function creates an entity with the following components: SoundCreation/SoundDestruction, Velocity, Health, Damage, Animation, Position, Team, Hitbox and CollisionEffect. The team component gets created depending which entity has been given to the function as the creator. The makeBackground function will give the entity the following components: Position, Animation and Velocity.

The apply function will first update the hitBox component by getting the position and animation component and will calculating the hitbox. Afterwards it will check the overlap with every other hitbox and if two hitbox with opposing team components overlap it will get the collisionEffect component and the execute them. All of them are effects that deal dmg to the other entity. Bullets have one health, therefore if they hit someone, they "die".

The apply function will go through every entity with an weapon and that is not a player. With the help of a timer it will spawn bullets of the type defined in the armament component.

The apply function has a timer of 0.02 seconds, which means every 0.02 seconds will the position of every entity execpt the player be updated by the value given in the velocity component. When an entity is set to follow another entity their position will be instead updated to the followed entity.

#### **CLient-only Systems**

The apply function will get all entitys with a health, animatian and position component and if the health->visible bool is set to true will draw a health box over the entity.

This system does the music and SFX. The apply function updates the buffer for the music. Also it will call the playSFX function on all the elements of the static queue. When the gamelogic determines it needs to play a SFX, it will push the ID to the queue. Also it checks for Player keyboard input to change the volume and pause the music

| key | effect                 |
| --- | ---------------------- |
| p   | pause/resume the music |
| z   | increase volume        |
| u   | decrease volume        |

The changeSong function changes the music. It takes an SongID enum and will use the static map of songs to change it.

```
static std::map<SongID, Song> Songs{
    {SongID::BOSS, {"resources/song0.mp3"}},
    {SongID::NORMAL, {"resources/song1.mp3"}},
    {SongID::PIANO, {"resources/song2.mp3"}},
};
```

The playSFX function will play a SFX definied in a map given the key.

```
static std::map<SFXID, SFX> SoundEffects{
    {SFXID::KNOCK, {"resources/gunShot1.wav"}},
    {SFXID::LIGHT_GUNSHOT, {"resources/gunShot2.wav"}},
    {SFXID::HEAVY_GUNSHOT, {"resources/gunShot3.wav"}},
};
```

The apply function will go through all entitys with animation and gather all layers. Then it will go through the layers and draw the animation of the entitys. The system uses a map of all animations, but they will be only loaded once there are needed. Every Animation component has a timer after which the index for the frames will be increased.
