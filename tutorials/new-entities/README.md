---
description: create new backgrounds, enemies, and bosses.
---

# New entities

Everything visual is considered an entitiy by our ECS.\
Similar steps are requred to add entities of any kind, backgounds, enemies, bosses or projectiles.

each type of entity is only distict by the set of components it uses

| Boss components   | Enemy components     | background components |
| ----------------- | -------------------- | --------------------- |
| Health::Component | Health::Component    |                       |
| Damage::Component | Damage::Component    |                       |
|                   | Veclocity::Compotent | Veclocity::Compotent  |
