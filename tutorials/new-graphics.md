# New Graphics

## Create new animation

you can add your own models to projectiles, players, enemies, bosses and backgrounds

using png format

&#x20;   1\. Add your png spritesheets to the **resources** folder.\
&#x20;      Add their path from directory root underneath <mark style="color:blue;">**`cmrc_add_resource_library(`**</mark> \
&#x20;      only the client needs those resources, so only add it inside <mark style="color:blue;">**`function(build_client)`**</mark>

{% hint style="warning" %}
Always recompile after changing the level files since\
cmrc loads them into the executable
{% endhint %}
