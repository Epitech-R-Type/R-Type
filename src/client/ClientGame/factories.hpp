#pragma once

#include "../../shared/ECS/ECS.hpp"

class SpriteSystem;

EntityID makePlayer(ECSManager* ECS, SpriteSystem* spriteSystem);

void makeEndboss(ECSManager* ECS, SpriteSystem* spriteSystem);

void makeEnemy(ECSManager* ECS, SpriteSystem* spriteSystem);

void makeBullet(ECSManager* ECS, SpriteSystem* spriteSystem, EntityID source);

EntityID getPlayerID(ECSManager* ECS);