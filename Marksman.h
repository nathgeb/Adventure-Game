#pragma once
#include "Enemy.h"
class Marksman : public Enemy
{
public:

/**
* Constructor of a Marksman
**/
	Marksman(std::string name, int xPos, int yPos);

	//no destructor needed, Character's virtual destructor deletes the weapon
};

