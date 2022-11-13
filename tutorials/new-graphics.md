# New Graphics

## Create new animation

you can add your own models to projectiles, players, enemies, bosses and backgrounds.\
Using png format

#### &#x20;   1. Add your png spritesheets to the **resources** folder

&#x20;      Add their path from directory root underneath <mark style="color:blue;">**`cmrc_add_resource_library(`**</mark> \
&#x20;      only the client needs those resources, so only add it inside <mark style="color:blue;">**`function(build_client)`**</mark>

{% code title="CMakeLists.txt" %}
```cpp
    cmrc_add_resource_library(
        client-resources

        ALIAS client::rc
        NAMESPACE client

        // Add the path to your new file here
        resources/r-typesheet1.png
        resources/r-typesheet2.png
        resources/r-typesheet3.png
        resources/r-typesheet5.png
        resources/r-typesheet7.png
        resources/r-typesheet8.png
        resources/r-typesheet9.png
        ...
```
{% endcode %}

{% hint style="warning" %}
Always recompile after changing the resource files since\
cmrc loads them into the executable
{% endhint %}

#### &#x20;   2. Create new Animation::AnimationID enum

```cpp
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
    // Only add new enums down here
};
```

{% hint style="danger" %}
Always add your new enum at the end of the list to not change values
{% endhint %}

#### &#x20;   3. Add a new instance of Animation::Sheet&#x20;

* Go to the **`std::map<> sheets`** in src/shared/ECS/components.hpp
* Add your new **`Animation::AnimationID`** enum and an instance of the **`Sheet`** struct

{% code title="src/shared/ECS/components.hpp" %}
```cpp
static std::map<Animation::AnimationID, Animation::Sheet> Sheets = {
    {Animation::AnimationID::Orb, {"resources/r-typesheet3.png", 1, 1, 16, 16, 12, 1, 1, 0, 0}},
    {Animation::AnimationID::Vortex, {"resources/r-typesheet30a.png", 1, 3, 32, 32, 3, 1, 2, 0, 0}},
    {Animation::AnimationID::Cluster, {"resources/r-typesheet32.png", 0, 0, 259, 142, 2, 3, 1, 1, 1}},
    {Animation::AnimationID::Laser, {"resources/r-typesheet43.png", 1, 41, 48, 4, 8, 1, 2, 0, 0}},
    {Animation::AnimationID::Lost, {"resources/lost.png", 0, 0, 639, 513, 8, 1, 1, 0, 0}},

    {Animation::AnimationID::SpaceshipLightblue, {"resources/r-typesheet42.png", 1, 3, 32, 16, 5, 1, 0, 0, 1, 0.2}},
    {Animation::AnimationID::SpaceshipPink, {"resources/r-typesheet42.png", 1, 20, 32, 16, 5, 1, 0, 0, 1, 0.2}},
    {Animation::AnimationID::SpaceshipGreen, {"resources/r-typesheet42.png", 1, 37, 32, 16, 5, 1, 0, 0, 1, 0.2}},
    {Animation::AnimationID::SpaceshipRed, {"resources/r-typesheet42.png", 1, 54, 32, 16, 5, 1, 0, 0, 1, 0.2}},
    {Animation::AnimationID::SpaceshipDarkblue, {"resources/r-typesheet42.png", 1, 71, 32, 16, 5, 1, 0, 0, 1, 0.2}},
    {Animation::AnimationID::SpaceshipRGB, {"resources/r-typesheet42.png", 1, 3, 32, 16, 1, 5, 0, 1, 0, 0.2}},
    
    {Animation::AnimationID::Background, {"resources/background.png", 0, 0, 256, 64, 1, 1, 0, 0, 0, 0, 1}},
    {Animation::AnimationID::Death, {"resources/background.png", 0, 0, 256, 64, 1, 1, 0, 0, 0, 0, 1}},
};
```
{% endcode %}

the Sheet is defined as:

{% code title="src/shared/ECS/components.hpp" %}
```cpp
struct Sheet {
    std::string path;
    float startX;
    float startY;
    float frameWidth;
    float frameHeight;
    int animWidth;
    int animHeight;
    int separationX;
    int separationY;
    // adds the same frames in reverse to the animation
    bool reverse;
    double interval = 0.1;
    bool tile = false;
};
```
{% endcode %}

| struct Sheet variables:                                                                                                     |
| --------------------------------------------------------------------------------------------------------------------------- |
| <mark style="color:blue;">**`std::string path`**</mark> = path to .png file in resources folder                             |
| <mark style="color:blue;">**`float startX, startY`**</mark> = x and y position of the upper left pixel from first frame     |
| <mark style="color:blue;">**`float frameWidth, frameHeight`**</mark> = width and height of single frame in sprite sheet     |
| <mark style="color:blue;">**`int animWidth, animHeight`**</mark> =  number of horizonal and vertical frames in sprite sheet |
| <mark style="color:blue;">**`int separationX, separationY`**</mark> = pixels between two frames in sprite sheet             |
| <mark style="color:blue;">**`bool reverse`**</mark> = if true doesnt repeat the animation at the end but reverses it        |
| <mark style="color:blue;">**`double interval`**</mark> = seconds between two frames of animation                            |
| <mark style="color:blue;">**`bool tile`**</mark> =  only for backgrounds to infinitely repeat                               |

{% hint style="info" %}
the **`path`** will only be found if you have [loaded it into the client executable](new-graphics.md#1.-add-your-png-spritesheets-to-the-resources-folder)
{% endhint %}
