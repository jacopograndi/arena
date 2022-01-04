# Age
Autochess game in which the player constructs and places it's army of robots.
The game is developed in C.

Early support for multiplayer.

![](https://github.com/jacopograndi/arena/blob/main/arena_gameplay.png?raw=true)

Each robot is modular and can be composed to the player's liking.
The component can be found in build/content as json files.

![](https://github.com/jacopograndi/arena/blob/main/arena_unit.png?raw=true)
![](https://github.com/jacopograndi/arena/blob/main/arena_unit_detail.png?raw=true)

The game features:
* A deterministic battle solver
* Multiplayer
* A system to manage unit statistics and apply component bonuses
* Unit evaluation and army evaluation to weight their strenght
* Hundreds of components, each upgradable twice
* A random unit generator controlled by the desired strenght
* A custom bitmap font (with kerning support)
* CMake build support
