# 2D Adventure Game

A console adventure game in C++. Walk around the map, fight Brutes, Marksmen and Wolves, and spend your gold in town at the weapon shop, the casino (slots and blackjack) or the brewery.

## Controls

- `W` `A` `S` `D` to move
- `L` to open the legend
- In fights and in town, type commands like `attack`, `drink`, `shop` or `leave`

## Building

Open `copycat.sln` in Visual Studio and run it. The game uses Windows console functions (`windows.h`, `conio.h`), so it builds on Windows only.

Enemy names are read from `Enemy Names.txt` (comma-separated). If the file is empty, enemies are called "Nameless".
