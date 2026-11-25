#pragma once
#include <string>                //for strings
#include <iostream>              //for cin and cout
#include <windows.h>             // for Sleep_()
#include <fstream>               // for save file
#include "ICS_ConsoleHelper.h"
#include "ICS_Helpers.h"
#include "SuperArray.h"
#include "Weapon.h"
#include "Character.h"
#include "Constants.h"
#include "Utils.h"
#include "Marksman.h"
#include "Brute.h"
#include "Wolf.h"

using namespace std;       //include standard namespace


/**
* adds a random enemy (brute, marksman or wolf) near one of the map's corners
*
* @param enemies            The array of enemies to add to
* @param enemyNames         The pool of unused names
*/
void addEnemy(SuperArray<Character*>& enemies, SuperArray<string>& enemyNames)
{
	//use a placeholder if we run out of names (or the names file is empty)
	string name = "Nameless";
	if (enemyNames.getSize() > 0)
	{
		name = enemyNames.remove(0);
	}

	int spawnDiff = randNumGen(0, 2);
	int enemyType = randNumGen(0, 2);
	if (enemyType == 0)
	{
		enemies.pushBack(new Brute(name + " the Brute", 19 - spawnDiff, 10 - spawnDiff));
	}
	else if (enemyType == 1)
	{
		enemies.pushBack(new Marksman(name + " the Marksman", 0 + spawnDiff, 10 - spawnDiff));
	}
	else
	{
		enemies.pushBack(new Wolf(name + " the Wolf", 19 - spawnDiff, 0 + spawnDiff));
	}
}

/**
* makes two people fight
*
* @param player             The player
* @param enemy              The enemy
*
* @returns                  True if the player won, false otherwise
*/
bool fight(Character* player, Character* enemy)
{
	//clear screen when function starts
	system("cls");

	//stay in loop until someone dies
	while (player->getHealth() > 0 and enemy->getHealth() > 0)
	{
		//clear screen every round
		system("cls");

		//print both stats

		player->print();
		enemy->print();

		//pause the program 
		Sleep(SLEEP_TIME);

		//user takes turn
		player->takeTurn(enemy);

		//pause the program
		Sleep(SLEEP_TIME);

		//print both stats
		player->print();
		enemy->print();

		//pause the program 
		Sleep(SLEEP_TIME);

		//computer takes turn
		enemy->takeTurn(player);

		//pause
		system("pause");
	}

	//if you lose
	if (player->getHealth() == 0)
	{
		cout << "you died to " << enemy->getName() << ".\n";

		//pause
		system("pause");

		//clear screen
		system("cls");

		//return lost
		return false;
	}
	//if you win
	else
	{
		//pause the program 
		Sleep(SLEEP_TIME);

		//tell player they took gold and potions off their opponent
		cout << "\nYou take " << ICS_YELLOW_TEXT << enemy->getGold() << ICS_WHITE_TEXT << " gold and " << enemy->getPotions() << " potions off of " << enemy->getName() << "'s body!\n\n";

		//transfer gold
		player->getGold() += enemy->getGold();
		enemy->getGold() = 0;

		//transfer potions
		player->getPotions() += enemy->getPotions();
		enemy->getPotions() = 0;

		//pause
		system("pause");

		//clear screen
		system("cls");

		//return won
		return true;
	}
}