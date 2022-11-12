---
description: rework existing enemies or create new ones
---

# New enemies

## Reworking enemy stats

to create enmies with custom speed, health and damage stats you can add new instances \
of the **`EnemyStats`** struct to **`std::vector<EnemyStats> _enemies`**\
**``**defined in src/server/Game.hpp.\
afterwards you can refer to them in the level fevel files wither their index in the vector (0,1,2,...)

```cpp
struct EnemyStats {
    int health;
    int damage;
    Animation::AnimationID sprite;
    float speed;
    Armament::Type armament;
};


// inside Game class

std::vector<EnemyStats> _enemies = {
    {10, 5, Animation::AnimationID::Orb, 8.0, Armament::Type::Laser},
    {20, 25, Animation::AnimationID::Orb, 10.0, Armament::Type::Laser},
    {30, 15, Animation::AnimationID::Orb, 3.0, Armament::Type::Laser},
    {50, 50, Animation::AnimationID::Orb, 1.0, Armament::Type::Laser},
    // add new instance here
};
```

{% hint style="info" %}
Look into the [New levels](../new-levels.md#content-of-a-level-file) Tutorial to see more on how to define enemies in a level
{% endhint %}

{% hint style="warning" %}
Always recompile after changing the level files since\
cmrc loads them into the executable
{% endhint %}



## Create new Weapon

The weapons are defined as **`Armament::Type`**'s.\


#### &#x20;   1. Create new **`Armament::Type`** enum in **Components.hpp**

```cpp
namespace Armament {
    enum Type {
        Laser,
        Buckshot,
    };
```

use this enum in your **`EnemyType`** struct as descibed above

#### &#x20;   2. Add enum to switch in  src/server/ArmamentSystems.cpp

```cpp
switch (armament->type) {
    case Armament::Type::Laser:
        Factory::Weapon::makeLaser(ECS, entityID);
        break;
    case Armament::Type::Buckshot:
        Factory::Weapon::makeBuckshot(ECS, entityID);
        break;
    // Add new case for your new Armament::Type enum here
}

```

#### &#x20;   3. Call your attack function in the **`Factory::Weapon`** namespace

```cpp
namespace Factory {
    namespace Ally {
        EntityID makePlayer(std::shared_ptr<ECSManager> ECS, int uniqueID);
    }
    namespace Enemy {
        EntityID makeEndboss(std::shared_ptr<ECSManager> ECS, BossStats stats);

        EntityID makeEnemy(std::shared_ptr<ECSManager> ECS, EnemyStats stats);
    } // namespace Enemy
    
    namespace Weapon {
        void makeLaser(std::shared_ptr<ECSManager> ECS, EntityID source);

        void makeBuckshot(std::shared_ptr<ECSManager> ECS, EntityID source);
        // call your attack function here, with the same arguments as the ones above
    } // namespace Weapon

    namespace Misc {
        void makeBackground(std::shared_ptr<ECSManager> ECS, Animation::AnimationID bgID);
    }
} // namespace FactoryCall your attack function in the Factory::Weapon namespace
```

you can also use othe premade functions above.

#### &#x20;   4. Define new attack function in the **`Factory::Weapon`** namespace

This is the current default attack function in src/server/Systems/Factory.cpp

```cpp
void Factory::Weapon::makeLaser(std::shared_ptr<ECSManager> ECS, EntityID source) {
    bullet(ECS, source, 40, 0, 0);
}
```

you can use the bullet function to create projectiles

the three integer arguments are <mark style="color:blue;">**velocity on x-achsis**</mark>, <mark style="color:blue;">**velocity on y-achsis**</mark> and <mark style="color:blue;">**rotation**</mark>.

Since bullets are entities like enimies and players they are created very silarly.\
Have a look at the **`bullet`** fucntion src/server/Systems/Factory.cpp\


|      bullets consit of:     |
| :-------------------------: |
|     Position::Component     |
|     Veclocity::Compotent    |
|       Team::Component       |
|      Health::Compotent      |
|      Damage::Component      |
|     Animation::Component    |
|      Hitbox::Component      |
|  CollisionEffect::Component |
|   SoundCreation::Component  |
| SoundDestruction::Component |

&#x20;use these components to creat customize projectiles.\
check [ECS documentation](../../dev-documentation/ecs-documentaion.md) for more info on the components.
