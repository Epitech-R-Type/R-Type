---
description: create new backgrounds, enemies, and bosses.
---

# New entities

Everything visual is considered an entitiy by our ECS.\
Similar steps are requred to add entities of any kind, backgounds, enemies, bosses or projectiles.

each type of entity is only distict by the set of components it uses

| Boss components                          | Enemy components     | background components                    |
| ---------------------------------------- | -------------------- | ---------------------------------------- |
| Health::Component                        | Health::Component    | <mark style="color:red;">NOT USED</mark> |
| Damage::Component                        | Damage::Component    | <mark style="color:red;">NOT USED</mark> |
| <mark style="color:red;">NOT USED</mark> | Veclocity::Compotent | Veclocity::Compotent                     |
| Position::Component                      | Position::Component  | Position::Component                      |
| Animation::Component                     | Animation::Component | Animation::Component                     |
| Hitbox::Component                        | Hitbox::Component    | <mark style="color:red;">NOT USED</mark> |
| Team::Component                          | Team::Component      | <mark style="color:red;">NOT USED</mark> |
| Armament::Component                      | Armament::Component  | <mark style="color:red;">NOT USED</mark> |
| Collision::Component                     | Collision::Component | <mark style="color:red;">NOT USED</mark> |
