#include "Weapon.h"
#include "Character.h"
#include "ICS_ConsoleHelper.h"	// for colors, sleeping, pausing...
#include <conio.h>              // for getting characters
#include "2DAdventureHeader.h"  // for other functions I stole from my gladiator project
#include "Enemy.h"
#include "Player.h"
#include "Constants.h"
#include <fstream>
#include <sstream>
#include <cctype>               // for tolower

using namespace std;			// for cout and endl

// constants for text and background colors (using RGB colors)
const string BROWN_TEXT = ICS_textColor(135, 84, 56);

//constants for map
const int GRID_HEIGHT = 10;
const int GRID_WIDTH = 20;
const char WATER = 254;
const char BRIDGE = 178;
const char ROCK = 'R';
const char LAND = '_';
const char ENEMY = 'E';
const char PLAYER = 'P';
const char TOWN = 223;
const int TOWN_Y = 3;
const int TOWN_X = 19;

//constants for town
const int BOB_DONATE_CAP = 1000;
const int POTION_PRICE = 30;
const int DEALER_MIN_NUM = 17;   //the dealer's minimum number in blackjack


/**
* function to initialize a 2d array with land
*
* @param grid                      the 2d array to initialize
*/
void initializeArray(char grid[GRID_HEIGHT][GRID_WIDTH])
{
	//loop through every row
	for (int row = 0; row < GRID_HEIGHT; row++)
	{
		//loop through every column
		for (int column = 0; column < GRID_WIDTH; column++)
		{
			//make it land
			grid[row][column] = LAND;
		}
	}
}

/**
*function to replace a piece on the grid
*
* @param grid                      the grid to put the character in
* @param row                       which row to put it in
* @param column                    which column to put it in
* @param newCharacter              the character to insert
*/
void insertArray(char grid[GRID_HEIGHT][GRID_WIDTH], int row, int column, char newCharacter)
{
	grid[row][column] = newCharacter;
}

/**
* function to build the map (mountains, river, bridge and town)
*
* @param map                       the map to build
*/
void makeMap(char map[GRID_HEIGHT][GRID_WIDTH])
{
	//initialize the map
	initializeArray(map);

	//mountain range
	insertArray(map, 7, 3, ROCK);
	insertArray(map, 7, 4, ROCK);
	insertArray(map, 7, 5, ROCK);
	insertArray(map, 8, 5, ROCK);
	insertArray(map, 8, 6, ROCK);

	insertArray(map, 6, 11, ROCK);
	insertArray(map, 7, 11, ROCK);
	insertArray(map, 7, 12, ROCK);

	//river
	insertArray(map, 0, 12, WATER);
	insertArray(map, 1, 12, WATER);
	insertArray(map, 2, 12, WATER);
	insertArray(map, 3, 12, WATER);
	insertArray(map, 3, 13, WATER);
	insertArray(map, 4, 13, WATER);
	insertArray(map, 5, 13, WATER);
	insertArray(map, 5, 14, WATER);
	insertArray(map, 5, 15, BRIDGE);
	insertArray(map, 5, 16, WATER);
	insertArray(map, 5, 17, WATER);
	insertArray(map, 5, 18, WATER);
	insertArray(map, 5, 19, WATER);

	//town
	insertArray(map, TOWN_Y, TOWN_X, TOWN);
}

/**
* checks if a spot on the map can be walked on (inside the map, not water or rock)
*
* @param map                       the map
* @param row                       the row to check
* @param column                    the column to check
*
* @returns                         true if someone can move there
*/
bool canMoveTo(char map[GRID_HEIGHT][GRID_WIDTH], int row, int column)
{
	//can't walk off the map
	if (row < 0 or row >= GRID_HEIGHT or column < 0 or column >= GRID_WIDTH)
	{
		return false;
	}

	//can't walk on water or rocks
	return map[row][column] != WATER and map[row][column] != ROCK;
}

/**
* fuction to print out the map with the player and enemies on it
*
* @param grid                      the 2d array to print out
* @param player                    the player
* @param enemies                   all the enemies
*/
void printGrid(char grid[GRID_HEIGHT][GRID_WIDTH], Character* player, SuperArray<Character*>& enemies)
{
	bool isEnemy = false;

	//print top border of the map
	cout << endl << "\t\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

	//loop through all rows
	for (int row = 0; row < GRID_HEIGHT; row++)
	{
		cout << "\t\t\t| ";

		//loop through all columns
		for (int column = 0; column < GRID_WIDTH; column++)
		{
			//loop through enemies
			for (int x = 0; x < enemies.getSize(); x++)
			{
				if (row == enemies[x]->getYPos() and column == enemies[x]->getXPos())
				{
					isEnemy = true;
				}
			}
			//check if there's an enemy there
			if (isEnemy)
			{
				//print them
				cout << ICS_RED_TEXT << ENEMY << ICS_WHITE_TEXT << ' ';
			}
			//check if the player is there
			else if (row == player->getYPos() and player->getXPos() == column)
			{
				//print them
				cout << ICS_YELLOW_TEXT << PLAYER << ICS_WHITE_TEXT << ' ';
			}
			//check if there's a bridge there
			else if (grid[row][column] == BRIDGE)
			{
				//print it brown
				cout << BROWN_TEXT << grid[row][column] << ICS_WHITE_TEXT << ' ';
			}
			//check if there's water there
			else if (grid[row][column] == WATER)
			{
				//print it blue
				cout << ICS_BLUE_TEXT << grid[row][column] << ICS_WHITE_TEXT << ' ';
			}
			//for any other spot
			else
			{
				//print what's in that box
				cout << grid[row][column] << ' ';
			}
			isEnemy = false;
		}
		//close off the map at the end of each row
		cout << "|";

		//go to next line
		cout << endl;
	}
	// print bottom border of the map
	cout << "\t\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
}

/**
*function for opening the legend
*/
void openLegend()
{
	//clear the screen
	system("cls");

	//print title
	cout << "\t\t\tLegend\n";
	cout << "\t\t\t-------\n";

	//list info
	cout << "\t\t\t" << LAND << " - land\n";
	cout << "\t\t\t" << ROCK << " - rock\n";
	cout << ICS_BLUE_TEXT << "\t\t\t" << WATER << ICS_WHITE_TEXT << " - water\n";
	cout << BROWN_TEXT <<"\t\t\t" << BRIDGE << ICS_WHITE_TEXT << " - bridge\n";
	cout << "\t\t\t" << TOWN << " - town\n";
	cout << ICS_YELLOW_TEXT <<"\t\t\t" << PLAYER << ICS_WHITE_TEXT << " - player\n";
	cout << "\t\t\t" << ENEMY << " - enemy\n";

	//wait for user to quit legend
	system("pause");

	//clear screen
	system("cls");
}

/**
* moves the console cursor to the top so the map redraws over itself
*/
void moveCursorToTop()
{
	COORD pos = { 0, 0 };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
}

/**
* reads the enemy names from the file into the names array and shuffles them
*
* @param enemyNames                the array to fill
*/
void loadEnemyNames(SuperArray<string>& enemyNames)
{
	//declare a variable to store input
	string fileInput = "";

	//ifstream object to read data from this file
	ifstream inFile("Enemy Names.txt");

	//check if the file is open
	if (inFile.is_open())
	{
		//retrieve all lines and store them in input until none remain
		while (getline(inFile, fileInput))
		{
			stringstream ss;
			ss << fileInput;

			string token = "";
			while(getline(ss, token, ','))
			{
				//add the names to the names array
				enemyNames.pushBack(token);
			}
		}
		//shuffle the names
		enemyNames.shuffle();
	}
	//if the file doesnt open
	else
	{
		//tell the user the file didn't open
		cout << "File not opening lil bro pack it up\n";
	}
}

/**
* the weapon shop, lets the player buy or sell weapons
*
* @param player                    the player
* @param weapons                   every weapon in the game
*/
void visitShop(Character* player, SuperArray<Weapon*>& weapons)
{
	//boolean to tell whether the user is in the shop
	bool shop = true;

	//while in shop
	while (shop)
	{
		//clear screen
		system("cls");

		//variable to store input
		string input;

		//variable to store chosen weapon index
		int x = 0;

		cout << "\t\tWeapon Market!\n";
		cout << "\t\t--------------\n\n";
		cout << "\t\tYour gold: " << player->getGold() << endl;

		//loop through weapons (skip free ones like fists, teeth and claws)
		for (int i = 0; i < weapons.getSize(); i++)
		{
			if (weapons[i]->getPrice() > 0)
			{
				cout << "\t\t" << weapons[i]->getName() << endl;
			}
		}

		//approach user and ask them to do something
		cout << "\nHey! Are you looking to buy or sell? (you could just leave, but don't! I need the sale)\n";
		getline(cin, input);

		//check if they want to buy
		if (input == "buy")
		{
			bool validName = false;

			//have the user select a weapon
			cout << "\nName a weapon, I'll bring it out for you!\n";
			getline(cin, input);

			//loop through weapons (skip free ones like fists, teeth and claws)
			for (int i = 0; i < weapons.getSize(); i++)
			{
				//check if this weapon was chosen
				if (input == weapons[i]->getName() and weapons[i]->getPrice() > 0)
				{
					//print stats
					weapons[i]->print();

					//save weapon index for purchases
					x = i;

					//acknowledge it's a valid name
					validName = true;
				}
			}
			//check if chosen weapon was invalid
			if (not validName)
			{
				//print invalid message
				cout << "Invalid! If you aren't gonna buy something, leave!\n";
			}
			//othwerise, if it's valid
			else
			{
				//ask for purchase
				cout << "\nWould you like to buy this weapon?\n";
				getline(cin, input);

				//denied purchase
				if (input == "no")
				{
					//tell user to leave
					cout << "Ok! Then leave! No browsing here\n";
				}
				//approved purchase
				else if (input == "yes")
				{
					//buy it
					player->buyWeapon(weapons, x);
				}
				else
				{
					//print invalid message
					cout << "Invalid, what're you talking about?\n";
				}
			}
		}
		//check if they want to sell
		else if (input == "sell")
		{
			//if they have a weapon
			if (player->getCurrentWeapon()->getName() != weapons[0]->getName())
			{
				//give the player half their original purchase
				int salePrice = player->getCurrentWeapon()->getPrice() / 2;
				player->getGold() += salePrice;

				//assign fists to current weapon
				player->setWeapon(weapons[0]);

				//print dialogue
				cout << "\nAlright, gimme that...";
				ICS_sleep(SLEEP_TIME);
				cout << "\nHere's your " << salePrice << " gold.\n";
			}
			//if they don't have a weapon
			else
			{
				//print dialogue
				cout << "You have nothing to sell!\n";
			}
		}
		//check if they want to leave
		else if (input == "leave")
		{
			//quit loop
			shop = false;
		}
		//invalid inputs
		else
		{
			//print invalid message
			cout << "Invalid input, valid inputs are (buy, sell, leave)\n";
		}
		//pause before loop
		system("pause");
	}
}

/**
* prints the casino title and the player's money and bet
*
* @param player                    the player
* @param bet                       the current bet
*/
void printCasinoHeader(Character* player, int bet)
{
	//title
	cout << "------------pompeii Casino--------------\n";
	cout << "-------------Welcome!---------------\n";

	//print player stats
	cout << "Name: " << player->getName() << endl;
	cout << "Money: " << player->getGold() << endl;
	cout << "Bet: " << bet << endl;
}

/**
* asks the player for a new bet and changes it if it's valid
*
* @param player                    the player
* @param bet                       the bet to change
*/
void changeBet(Character* player, int& bet)
{
	string input;

	//ask user to change bet
	cout << "What do you want to change your bet to? (Enter a real number that isn't more than what you have and isn't less than one)\n";
	getline(cin, input);

	//make change bet variable and assign it user input
	int newBet = atoi(input.c_str());

	//check if bet is valid
	if (newBet >= 1 and newBet <= player->getGold())
	{
		//change bet
		bet = newBet;
		cout << "\nYou changed your bet!\n";
	}
	//if bet is invalid
	else
	{
		//print invalid message
		cout << "\nInvalid response! Must be a number >= 1 and <= your money\n";
	}
}

/**
* drops the bet in the slot machine and spins
*
* @param player                    the player
* @param bet                       the bet
* @param autowin                   true if the player used the secret autowin
*/
void spinSlots(Character* player, int bet, bool autowin)
{
	//if their bet isn't valid, don't play
	if (bet > player->getGold())
	{
		//print invalid message
		cout << "I don't think you can bet that!\n";
		return;
	}

	//declare variables for random slot numbers
	int rand1 = rand() % 10;
	int rand2 = rand() % 10;
	int rand3 = rand() % 10;

	//autowin makes all three the same
	if (autowin)
	{
		rand2 = rand1;
		rand3 = rand1;
	}

	//print message
	cout << endl << endl << "You drop $" << bet << " into the machine.\n\n";

	//lose bet money
	player->getGold() -= bet;

	//print slot numbers
	ICS_sleep(1000);
	cout << rand1 << " ";
	ICS_sleep(1000);
	cout << rand2 << " ";
	ICS_sleep(1000);
	cout << rand3 << endl;

	//if all three numbers are equal
	if (rand1 == rand2 and rand1 == rand3)
	{
		//print message and add gold
		cout << "Congratulations! You won 10x your bet!\n";
		player->getGold() += bet * 10;
	}
	//if 2 numbers in a row are equal
	else if (rand1 == rand2 or rand2 == rand3)
	{
		//print message and add gold
		cout << "Congratulations! You won 3x your bet!\n";
		player->getGold() += bet * 3;
	}
	//none are equal
	else
	{
		//print message
		cout << "You lost! Come back soon!\n";
	}
}

/**
* the slots machine loop
*
* @param player                    the player
* @param bet                       the current bet
*/
void playSlots(Character* player, int& bet)
{
	string input;

	//stay in this loop until they leave
	while (input != "leave")
	{
		//set colors
		cout << ICS_INTENSE_WHITE_BACKGROUND;
		cout << ICS_BLACK_TEXT;

		printCasinoHeader(player, bet);

		//Ask player what they want to do and set colors
		cout << ICS_BLACK_TEXT << "What would you like to do? (" << ICS_INTENSE_GREEN_TEXT << " play," << ICS_INTENSE_RED_TEXT << " leave" << ICS_BLACK_TEXT << " or " << ICS_MAGENTA_TEXT << "change bet" << ICS_BLACK_TEXT << ")" << endl;
		getline(cin, input);

		//starts slots
		if (input == "play")
		{
			spinSlots(player, bet, false);
		}
		//change bet value
		else if (input == "change bet")
		{
			changeBet(player, bet);
		}
		//free win
		else if (input == "autowin")
		{
			spinSlots(player, bet, true);
		}
		//invalid command
		else if (input != "leave")
		{
			cout << "That's an invalid command. Valid commands are (" << ICS_INTENSE_GREEN_TEXT << " play," << ICS_INTENSE_RED_TEXT << " leave" << ICS_BLACK_TEXT << " or " << ICS_MAGENTA_TEXT << "change bet" << ICS_BLACK_TEXT << ")" << endl;
		}

		//wait for user input, then clear screen
		system("pause");
		system("cls");
	}
}

/**
* plays one round of blackjack
*
* @param player                    the player
* @param bet                       the bet
*/
void blackjackRound(Character* player, int bet)
{
	string input;

	//if they can't pay
	if (bet > player->getGold())
	{
		//tell user they're broke
		cout << "You don't have that much money!\n";
		system("pause");
		return;
	}

	//boolean to leave the loop when they say stand
	bool leave = false;

	//make computer card
	int computerCard1 = rand() % 10 + 1;

	//make player cards
	int card1 = rand() % 10 + 1;
	int card2 = rand() % 10 + 1;
	int sum = card1 + card2;

	// deal cards
	cout << "You were dealt a " << card1 << " and a " << card2 << ". Your sum is " << sum << endl;
	cout << "The dealer is showing a " << computerCard1 << endl;

	//while no one's won and loop hasn't been left
	while (sum < 21 and not leave)
	{
		// hit or stand?
		cout << "Do you want to hit or stand?\n";
		getline(cin, input);

		//check if they hit
		if (input == "hit")
		{
			//assign random new card from 1-10
			int newCard = rand() % 10 + 1;
			cout << "You were dealt a " << newCard << endl;

			//add to sum
			sum = sum + newCard;
			cout << "Your total is " << sum << endl;
		}
		//check if they stand
		else if (input == "stand")
		{
			//tell user their total
			cout << "Your total is " << sum << endl;
			leave = true;
		}
		//invalid commmands
		else
		{
			//print invalid statement
			cout << "That's an invalid command. Valid commands are (hit, stand)" << endl;
		}
	}
	//pause
	system("pause");

	////dealer plays out his turn/////

	//dealer's other card
	int computerCard2 = rand() % 10 + 1;
	int computerSum = computerCard1 + computerCard2;

	//tell user dealer's card
	cout << "The dealer's second card is " << computerCard2 << ", his total is " << computerSum << endl;

	//always hit if under 17
	while (computerSum < DEALER_MIN_NUM)
	{
		//make new card and total
		int newComputerCard = rand() % 10 + 1;
		computerSum = computerSum + newComputerCard;

		//tell user dealer's card
		cout << "The dealer hits, he gets a " << newComputerCard << ", his total is " << computerSum << endl;
	}

	//stand if 17 or more and didn't bust
	if (computerSum <= 21)
	{
		cout << "The dealer stands, his total is " << computerSum << endl;
	}

	//win/loss conditions
	//loss, player went over
	if (sum > 21)
	{
		cout << "You bust!" << endl;
		player->getGold() -= bet;
	}
	//win, dealer went over or player is higher
	else if (computerSum > 21 or sum > computerSum)
	{
		cout << "You won $" << bet << endl;
		player->getGold() += bet;
	}
	//loss, dealer is higher
	else if (computerSum > sum)
	{
		cout << "The dealer wins!" << endl;
		player->getGold() -= bet;
	}
	//tie
	else
	{
		cout << "push!" << endl;
	}
	system("pause");
}

/**
* the blackjack table loop
*
* @param player                    the player
* @param bet                       the current bet
*/
void playBlackjack(Character* player, int& bet)
{
	string input;

	while (input != "leave")
	{
		//set colors
		cout << ICS_INTENSE_YELLOW_BACKGROUND;
		cout << ICS_BLACK_TEXT;

		printCasinoHeader(player, bet);

		//Ask player what they want to do and set colors
		cout << "What would you like to do? (" << ICS_INTENSE_GREEN_TEXT << " play," << ICS_INTENSE_RED_TEXT << " leave" << ICS_BLACK_TEXT << " or " << ICS_MAGENTA_TEXT << "change bet" << ICS_BLACK_TEXT << ")" << endl;
		getline(cin, input);

		//playing blackjack
		if (input == "play")
		{
			blackjackRound(player, bet);
		}
		//change bet value
		else if (input == "change bet")
		{
			changeBet(player, bet);
			system("pause");
		}
		//leave
		else if (input == "leave")
		{
			system("pause");
		}
		//invalid commmands
		else
		{
			cout << "That's an invalid command. Valid commands are (" << ICS_INTENSE_GREEN_TEXT << " play," << ICS_INTENSE_RED_TEXT << " leave" << ICS_BLACK_TEXT << " or " << ICS_MAGENTA_TEXT << "change bet" << ICS_BLACK_TEXT << ")" << endl;
			system("pause");
		}
	}
}

/**
* the player talks to someone random in the casino
*
* @param player                    the player
*/
void talkInCasino(Character* player)
{
	//background colour
	cout << ICS_backgroundColor(205, 152, 240);

	//different results
	int chance = rand() % 4;

	//event 1
	if (chance == 0)
	{
		//placeholder
		string nothing;

		//rich uncle gives you betting money
		int freeCash = rand() % BOB_DONATE_CAP + 1;
		cout << ICS_INTENSE_RED_TEXT;
		cout << "Hey newbie how ya doin?\n";
		getline(cin, nothing);
		cout << "Ahh whatever, take some cash, win big!\n";
		system("pause");
		cout << "Uncle Bob gives you $" << freeCash << " \n";
		system("pause");
		player->getGold() += freeCash;
	}
	//event 2
	else if (chance == 1)
	{
		//mysterious man gives player a tip
		cout << ICS_BLUE_TEXT;
		cout << "Psst, hey.";
		ICS_sleep(1000);
		cout << " \nOn the slots machine if you type 'autowin', ";
		ICS_sleep(1500);
		cout << "you'll get a free win! Careful though, it only works once a day!\n";
		system("pause");
	}
	//event 3
	else if (chance == 2)
	{
		//user hears the mayor
		cout << "You hear a big booming laugh, a responding voice stutters something about paying a man back later.\n You turn and see the mayor in the Casino! He seems to be in a tight situation...\n";
		system("pause");
	}
	//event 4
	else
	{
		//user encounters a rude Character
		cout << ICS_GREEN_TEXT << "What are you looking at?\n";
		system("pause");
	}
}

/**
* the casino, where the player can play slots, blackjack or talk
*
* @param player                    the player
*/
void visitCasino(Character* player)
{
	//clear screen
	system("cls");

	string input;

	//variable to store bet value
	int bet = 10;

	//boolean to stay in casino loop
	bool casino = true;

	while (casino)
	{
		//set colors
		cout << ICS_backgroundColor(120, 30, 50);
		cout << ICS_BLACK_TEXT;

		printCasinoHeader(player, bet);

		//Ask player what they want to do
		cout << "What would you like to do? (Valid commands are: slots, blackjack, talk, leave)\n";
		getline(cin, input);

		//Deciding which loop to run
		if (input == "slots")
		{
			playSlots(player, bet);
		}
		else if (input == "blackjack")
		{
			playBlackjack(player, bet);
		}
		else if (input == "talk")
		{
			talkInCasino(player);
		}
		else if (input == "leave")
		{
			//leave loop
			casino = false;
		}
		//invalid commmand
		else
		{
			cout << "That's an invalid command. Valid commands are (slots, blackjack, talk, leave)\n";
			system("pause");
		}
	}
}

/**
* the brewery, where the player can buy a potion
*
* @param player                    the player
*/
void visitBrewery(Character* player)
{
	//bool to stay in brewery
	bool brewery = true;

	while (brewery)
	{
		//clear screen
		system("cls");

		//variable to store user input
		string input;

		//print message for user
		cout << "Welcome to my brewery adventurer!\n Just " << POTION_PRICE << " gold for a healing potion, would you like to buy one?\n";
		getline(cin, input);

		//check if user says yes
		if (input == "yes")
		{
			//check if they can afford it
			if (player->getGold() >= POTION_PRICE)
			{
				//subtract gold and add potion
				player->getGold() -= POTION_PRICE;
				player->getPotions()++;

				//print message to tell player thay got it
				cout << "Here's your potion!\n";
			}
			else
			{
				//tell user they're broke
				cout << "You can't afford one!\n";
			}

			//quit loop
			brewery = false;
		}
		//check if they say no
		else if (input == "no")
		{
			//tell user bye
			cout << "OK, Goodbye then!\n";

			//quit loop
			brewery = false;
		}
		//invalid responses
		else
		{
			//invalid response message
			cout << "Valid responses are (yes, no)\n";
		}

		//pause
		system("pause");
	}
}

/**
* the town menu, lets the player pick where to go
*
* @param player                    the player
* @param weapons                   every weapon in the game (for the shop)
*/
void visitTown(Character* player, SuperArray<Weapon*>& weapons)
{
	//create boolean to represent if the player is in town or not
	bool town = true;

	//while in town
	while (town)
	{
		moveCursorToTop();

		//varible to store input
		string input;

		//set background color
		cout << ICS_WHITE_BACKGROUND;
		cout << ICS_BLACK_TEXT << "You're in the town, do you want to go to the " << ICS_RED_TEXT << "shop, casino, brewery" << ICS_BLACK_TEXT << " or" << ICS_RED_TEXT << " leave" << ICS_RED_TEXT << "?" << endl;
		getline(cin, input);

		if (input == "shop")
		{
			visitShop(player, weapons);
		}
		else if (input == "casino")
		{
			visitCasino(player);
		}
		else if (input == "brewery")
		{
			visitBrewery(player);
		}
		else if (input == "leave")
		{
			//leave town loop
			town = false;
		}
		//invalid entries
		else
		{
			//print error message
			cout << ICS_BLACK_TEXT << "Invalid input, valid inputs are " << ICS_RED_TEXT << "shop, casino, brewery, " << ICS_BLACK_TEXT << "or" << ICS_RED_TEXT << " leave\n" << ICS_BLACK_TEXT;

			//pause
			system("pause");
		}
	}

	//reset colors to deafault
	system("cls");
	cout << ICS_BLACK_BACKGROUND;
	cout << ICS_WHITE_TEXT;
}

/**
* moves a character one square if the spot is walkable
*
* @param map                       the map
* @param character                 who's moving
* @param rowChange                 how much to move up/down (-1, 0 or 1)
* @param columnChange              how much to move left/right (-1, 0 or 1)
*/
void moveCharacter(char map[GRID_HEIGHT][GRID_WIDTH], Character* character, int rowChange, int columnChange)
{
	int newRow = character->getYPos() + rowChange;
	int newColumn = character->getXPos() + columnChange;

	//only move if the spot is valid
	if (canMoveTo(map, newRow, newColumn))
	{
		character->getYPos() = newRow;
		character->getXPos() = newColumn;
	}
}

/**
* randomly moves every enemy that isn't on the player's square
*
* @param map                       the map
* @param player                    the player
* @param enemies                   all the enemies
*/
void moveEnemies(char map[GRID_HEIGHT][GRID_WIDTH], Character* player, SuperArray<Character*>& enemies)
{
	//loop through enemies movement choices
	for (int i = 0; i < enemies.getSize(); i++)
	{
		//don't move if they're on the player's square
		if (player->getYPos() == enemies[i]->getYPos() and player->getXPos() == enemies[i]->getXPos())
		{
			continue;
		}

		//generate a random num from 0-4, 0 means don't move
		int moveChance = randNumGen(0, 4);

		if (moveChance == 1)
		{
			//go north
			moveCharacter(map, enemies[i], -1, 0);
		}
		else if (moveChance == 2)
		{
			//go south
			moveCharacter(map, enemies[i], 1, 0);
		}
		else if (moveChance == 3)
		{
			//go west
			moveCharacter(map, enemies[i], 0, -1);
		}
		else if (moveChance == 4)
		{
			//go east
			moveCharacter(map, enemies[i], 0, 1);
		}
	}
}

/**
* starts a fight with any enemy on the player's square
*
* @param player                    the player
* @param enemies                   all the enemies
* @param enemyNames                the pool of unused names
*/
void checkCollisions(Character* player, SuperArray<Character*>& enemies, SuperArray<string>& enemyNames)
{
	//loop through enemies (stop if the player dies)
	for (int i = 0; i < enemies.getSize() and player->getHealth() > 0; i++)
	{
		//if the player meets an enemy
		if (player->getYPos() == enemies[i]->getYPos() and player->getXPos() == enemies[i]->getXPos())
		{
			//fight and assign true or false to win
			bool win = fight(player, enemies[i]);

			//check if the player won
			if (win)
			{
				//pause
				ICS_sleep(SLEEP_TIME);

				//add the enemy's name back to the array of names (without the " the Brute" part)
				string name = enemies[i]->getName();
				name = name.substr(0, name.find(" the "));
				if (name != "Nameless")
				{
					enemyNames.pushBack(name);
				}

				//remove the enemy from the game and free its memory
				delete enemies.remove(i);

				//everything shifted left, so check this spot again
				i--;
			}
		}
	}
}

int main()
{
	//seed random number
	srand((unsigned int)time(NULL));

	//make weapons
	SuperArray<Weapon*> weapons;

	//initialize the weapons array (fists has to stay first, the shop uses it as "no weapon")
	weapons.pushBack(new Weapon("fists", 100, 20 , 5, 0, "punches"));
	weapons.pushBack(new Weapon("teeth", 70, 70, 30, 0, "bites"));
	weapons.pushBack(new Weapon("desert eagle", 30, 60, 50, 120, "shoots"));
	weapons.pushBack(new Weapon("pugio", 100, 30, 15, 55, "stabs"));
	weapons.pushBack(new Weapon("mjolnir", 60, 70, 10, 60, "bludgeons"));
	weapons.pushBack(new Weapon("bow", 80, 30, 20, 24, "snipes"));
	weapons.pushBack(new Weapon("pilum", 30, 80, 60, 40, "impales"));
	weapons.pushBack(new Weapon("gladius", 65, 60, 50, 30, "slashes"));
	weapons.pushBack(new Weapon("claws", 80, 15, 10, 0, "scratches"));

	//make the player (with their own copy of fists)
	Character* player = new Player("Nathan", 0, 0, 100, 500, 3, 100, new Weapon(*weapons[0]));

	//load enemy names
	SuperArray<string> enemyNames;
	loadEnemyNames(enemyNames);

	//make starting enemies
	SuperArray<Character*> enemies;
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		addEnemy(enemies, enemyNames);
	}

	//make the map
	char map[GRID_HEIGHT][GRID_WIDTH];
	makeMap(map);

	//variable to store user input
	char input;

	//ask user for input until user dies
	while (player->getHealth() > 0)
	{
		//respawn an enemy if one died
		if (enemies.getSize() < MAX_ENEMIES)
		{
			addEnemy(enemies, enemyNames);
		}

		//redraw the map
		moveCursorToTop();
		printGrid(map, player, enemies);

		//print info under map
		cout << "Enter " << ICS_BLUE_TEXT << "L" << ICS_WHITE_TEXT << " to open legend, and " << ICS_BLUE_TEXT << "W S A D, " << ICS_WHITE_TEXT << "to move\n";
		input = tolower(_getch());

		if (input == 'l')
		{
			openLegend();
		}
		else if (input == 'w')
		{
			//move north
			moveCharacter(map, player, -1, 0);
		}
		else if (input == 's')
		{
			//move south
			moveCharacter(map, player, 1, 0);
		}
		else if (input == 'a')
		{
			//move west
			moveCharacter(map, player, 0, -1);
		}
		else if (input == 'd')
		{
			//move east
			moveCharacter(map, player, 0, 1);
		}
		else
		{
			//print invalid statement
			cout << "Invalid input, valid inputs are L, W, S, A, D\n";

			//pause
			system("pause");

			//when screen is overwritten, it doesn't reach invalid statement, so clear
			system("cls");
		}

		//check if player hit town
		if (map[player->getYPos()][player->getXPos()] == TOWN)
		{
			visitTown(player, weapons);
		}

		moveEnemies(map, player, enemies);
		checkCollisions(player, enemies, enemyNames);
	}

	//free all the memory before quitting
	delete player;
	while (enemies.getSize() > 0)
	{
		delete enemies.popBack();
	}
	while (weapons.getSize() > 0)
	{
		delete weapons.popBack();
	}

	return 0;
}
