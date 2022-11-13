# New boss

{% hint style="info" %}
bosses are just enemies without **`Veclocity::Compotent`**
{% endhint %}

## Reworking boss stats

To create a boss with custom health and damage stats you can add new instances \
of the **`BossStats`** struct to **`std::vector<BossStats> _bosses`** \
defined in src/server/Game.hpp.\
afterwards you can refer to them in the level fevel files wither their index in the vector (0,1,...)

```cpp
std::vector<BossStats> _bosses = {
    {300, 20, Animation::AnimationID::Cluster, Armament::Type::Buckshot},
    {400, 50, Animation::AnimationID::Cluster, Armament::Type::Laser},
};
```

* if you want to create your own boss with new animation or attacks look into [New enemies](new-enemies.md)

{% hint style="info" %}
Look into the [New levels](../new-levels.md#content-of-a-level-file) Tutorial to see more on how to define bosses in a level
{% endhint %}

