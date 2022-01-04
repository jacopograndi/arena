# Arena

Autochess game in which the player constructs and places it's army of robot.
Each robot is modular and can be composed to the player's liking.

Early support for multiplayer.

In order to compile in windows
* have cmake
* have mingw32 (from the 64 bit version)
* have the sdl2 includes at C:/MinGW_libs/include/SDL2
* have the plibsys includes at C:/MinGW_libs/include/plibsys
* have lib files at C:/MinGW_libs/libs

run cmake -G "MinGW Makefiles"
run mingw32-make
