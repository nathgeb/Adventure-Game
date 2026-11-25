#pragma once
#include "Enemy.h"
#include <string>
class Brute : public Enemy
{
	public:
	/**
	* Constructor of a Brute
	**/
	Brute(std::string name, int xPos, int yPos);

	//no destructor needed, Character's virtual destructor deletes the weapon
};

