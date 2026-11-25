#pragma once
#include "Enemy.h"
class Wolf : public Enemy
{
public:

	/**
	* Constructor of a Wolf
	**/
	Wolf(std::string name, int xPos, int yPos);

	//no destructor needed, Character's virtual destructor deletes the weapon
};

