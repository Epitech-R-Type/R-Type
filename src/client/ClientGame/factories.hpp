#pragma once

#include "../../shared/ECS/ECS.hpp"

class SpriteSystem;

EntityID makePlayer(Manager* ECS, SpriteSystem* spriteSystem);

void makeEndboss(Manager* ECS, SpriteSystem* spriteSystem);

void makeEnemy(Manager* ECS, SpriteSystem* spriteSystem);

void makeBullet(Manager* ECS, SpriteSystem* spriteSystem);

EntityID getPlayerID(Manager* ECS);