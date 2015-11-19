#include "../Header Files/Room.h"
#include "../Header Files/Actor.h"
#include "../Header Files/Buffer.h"
#include "../Header Files/Item.h"
#include "../Header Files/Schooled.h"
#include "../Header Files/Console_color.h"
#include <fstream>
#include <iostream>
#include <cassert>

namespace con = JadedHoboConsole;

using std::endl;

vector<const Tile> Room::tileIndex;
vector<const Item> Room::itemIndex;
vector<const Actor> Room::actorIndex;

void Room::loadTileIndex(string filename)
{
	string line;
	std::ifstream stream(filename);
	if (stream.fail())
	{
		std::cout << "File open failed.\n";
		exit(1);
	}
	while (!stream.eof())
	{
		// Get the information from the file
		getline(stream, line);
		getline(stream, line);
		int index = stoi(line.substr(line.find(':') + 1));
		getline(stream, line);
		char symbol = (line.substr(line.find('\'') + 1))[0];
		getline(stream, line);
		WORD colour = stoi(line.substr(line.find(':') + 1, '/'));
		getline(stream, line);
		bool passable = ((line.substr(line.find(':') + 1)) == "true");

		// Put the tile in the vector
		Tile temp = { symbol, colour, passable, index };
		tileIndex.push_back(temp);
		// If there are more lines, get the empty line
		if (!stream.eof())
		{
			getline(stream, line);
		}
	}
	stream.close();
}

void Room::loadItemIndex(string filename)
{
	string line;
	std::ifstream stream(filename);
	if (stream.fail())
	{
		std::cout << "File open failed.\n";
		exit(1);
	}
	while (!stream.eof())
	{
		// Get the information from the file
		getline(stream, line);
		string name = line.substr(line.find(':') + 1);
		getline(stream, line);
		int index = stoi(line.substr(line.find(':') + 1));
		getline(stream, line);
		char symbol = (line.substr(line.find('\'') + 1))[0];
		getline(stream, line);
		WORD colour = stoi(line.substr(line.find(':') + 1, '/'));
		getline(stream, line);
		bool passable = ((line.substr(line.find(':') + 1)) == "true");
		getline(stream, line);
		int HP = stoi(line.substr(line.find(':') + 1));
		getline(stream, line);
		int EN = stoi(line.substr(line.find(':') + 1));
		getline(stream, line);
		int STR = stoi(line.substr(line.find(':') + 1));
		getline(stream, line);
		string mPickup = (line.substr(line.find(':') + 1));

		// Put the item in the vector
		Item temp;
		temp.setTile({ symbol, colour, passable, index });
		temp.setStats({ HP, EN, STR });
		temp.setMPickup(mPickup);
		temp.setName(name);
		itemIndex.push_back(temp);

		// If there are more lines, get the empty line
		if (!stream.eof())
		{
			getline(stream, line);
		}
	}
	stream.close();
}

void Room::loadActorIndex(string filename)
{
	string line;
	std::ifstream stream(filename);
	if (stream.fail())
	{
		std::cout << "File open failed.\n";
		exit(1);
	}
	actorIndex.push_back(Actor());
	while (!stream.eof())
	{
		// Get the information from the file
		getline(stream, line);
		string name = line.substr(line.find(':') + 1);
		getline(stream, line);
		int index = stoi(line.substr(line.find(':') + 1));
		getline(stream, line);
		char symbol = (line.substr(line.find('\'') + 1))[0];
		getline(stream, line);
		WORD colour = stoi(line.substr(line.find(':') + 1, '/'));
		getline(stream, line);
		bool passable = ((line.substr(line.find(':') + 1)) == "true");
		getline(stream, line);
		int HP = stoi(line.substr(line.find(':') + 1));
		getline(stream, line);
		int EN = stoi(line.substr(line.find(':') + 1));
		getline(stream, line);
		int STR = stoi(line.substr(line.find(':') + 1));
		getline(stream, line);
		ItemPtr heldItem = getItemStats((line.substr(line.find(':') + 1)));
		getline(stream, line);
		string m_attack = (line.substr(line.find(':') + 1));
		getline(stream, line);
		string m_defend = (line.substr(line.find(':') + 1));


		// Put the actor in the vector
		Actor temp;
		temp.setTile({ symbol, colour, passable, index });
		temp.setStats({ HP, EN, STR });
		temp.setHeldItem(heldItem);
		temp.setName(name);
		temp.setMAttack(m_attack);
		temp.setMDefend(m_defend);
		actorIndex.push_back(temp);

		// If there are more lines, get the empty line
		if (!stream.eof())
		{
			getline(stream, line);
		}
	}
	stream.close();
}

void Room::display(Buffer& buffer){
	int tile;
	ActorPtr tempA;
	ItemPtr tempI;
	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			if (actorArray[a][b] > 0)
			{
				tile = actorArray[a][b];
				tempA = &actorIndex[tile];
				buffer.draw(tempA->getTile().character, tempA->getTile().colorCode, a, b);
			}
			else if (itemArray[a][b] > 0)
			{
				tile = itemArray[a][b];
				tempI = &itemIndex[tile];
				buffer.draw(tempI->getTile().character, tempI->getTile().colorCode, a, b);

			}
			else
			{
				tile = tileArray[a][b];
				buffer.draw(tileIndex[tile].character, tileIndex[tile].colorCode, a, b);
			}
		}
	}
}

ItemPtr Room::getItemStats(int a)
{
	return &itemIndex[a];
}

int Room::randomItem()
{
	int random = rand() % (itemIndex.size() - 4) + 4;
	return random;
}

ItemPtr Room::getItemStats(string s)
{
	for (unsigned int i = 0; i < itemIndex.size(); i++)
	{
		if (itemIndex[i].getName() == s)
		{
			return &itemIndex[i];
		}
	}
	return nullptr;
}

int Room::findActor(COORD c)
{
	for (unsigned int i = 0; i < actorList.size(); i++)
	{
		if (actorList[i].getLocation() == c)
			return i;
	}
	return -1;
}

Actor& Room::getActor(COORD c)
{
	int i = findActor(c);
	assert(i >= 0);
	return actorList[i];

}

vector<Actor>& Room::getActorList()
{
	return actorList;
}

void Room::removeActor(COORD c)
{
	int i = findActor(c);
	assert(i >= 0);
	actorList.erase(actorList.begin() + i);
}

void Room::moveActors(COORD p)
{
	for (Actor& a : actorList)
	{
		moveEnemy(p, a);
	}
}

bool Room::isPassable(COORD tile){
	int mapX = tile.X;
	int mapY = tile.Y;
	if (mapX < 0 || mapX >= schooled::MAP_WIDTH || mapY < 0 || mapY >= schooled::MAP_HEIGHT)
		return false;

	int tileValue = tileArray[mapY][mapX];
	int itemValue = itemArray[mapY][mapX];
	int actorValue = actorArray[mapY][mapX];
	ItemPtr tempI = &itemIndex[itemValue];
	if (tileIndex[tileValue].isPassable && actorValue == 0 && tempI->getTile().isPassable)
		return true;
	return false;
}

Room::Room() {}

Room::Room(string fileName)
{
	std::ifstream stream;
	stream.open(fileName);

	if (stream.fail())
	{
		std::cout << "File open failed.\n";
		exit(1);
	}

	string m;
	getline(stream, m);
	message = m;
	getline(stream, m); // blank

	// Get the tile array
	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			stream >> tileArray[a][b];
		}
	}
	getline(stream, m); // blank

	// Get the entrance coordinates
	for (COORD &c : entrances)
	{
		stream >> c.X >> c.Y;
	}
	getline(stream, m); // blank

	// Get the item array
	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			stream >> itemArray[a][b];
		}
	}
	getline(stream, m); // blank

	// Get the actor array
	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			stream >> actorArray[a][b];
			if (actorArray[a][b] > 0)
			{
				actorList.push_back(actorIndex[actorArray[a][b]]);
				actorList[actorList.size() - 1].setLocation({ b, a });
			}
		}
	}
	stream.close();

	COORD temp = { 0, 0 };
	for (COORD c : entrances)
	{
		if (c != temp)
		{	
			itemArray[c.Y][c.X] = 2;
		}
	}
}

string Room::getMessage() { return message; }

void Room::save(string fileName)
{
	std::ofstream stream;
	stream.open(fileName);

	if (stream.fail())
	{
		std::cout << "File open failed.\n";
		exit(1);
	}

	stream << message << endl << endl;

	// Save the tile array
	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			stream << tileArray[a][b] << " ";
		}
		stream << endl;
	}
	stream << endl;

	// Save the entrance coordinates
	for (COORD &c : entrances)
	{
		stream << c.X << " " << c.Y << endl;
	}
	stream << endl;

	// Save the item array
	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			stream << itemArray[a][b] << " ";
		}
		stream << endl;
	}
	stream << endl;

	// Save the actor array
	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			stream << actorArray[a][b] << " ";
		}
		stream << endl;
	}
	stream << std::flush;
	stream.close();
}

void Room::moveEnemy(COORD playerPos, Actor& enemy)
{
	int differenceX, differenceY, deltaX, deltaY;
	int enemyX = enemy.getX();
	int enemyY = enemy.getY();
	int playerX = playerPos.X;
	int playerY = playerPos.Y;
	deltaX = 0;
	deltaY = 0;
	differenceX = enemyX - playerX;
	differenceY = enemyY - playerY;
	bool inRange = differenceX > -4 && differenceX < 4 && differenceY > -4 && differenceY < 4;

	if (lineOfSight(playerPos, enemy) == true || inRange && lineOfSight(playerPos, enemy) == false)
{
		if (differenceX > -2 && differenceX < 2 && differenceY == 0 ||
			differenceY > -2 && differenceY < 2 && differenceX == 0)
		{
			deltaX = 0;
			deltaY = 0;
		}
		else
		{
			if (abs(differenceX) > abs(differenceY))
			{
				deltaX = (enemyX > playerX) ? -1 : 1;
				if (!isPassable({ enemyX + deltaX, enemyY + deltaY }))
				{
					deltaX = 0;
					deltaY = (enemyY > playerY) ? -1 : 1;
				}
			}
			else
			{
				deltaY = (enemyY > playerY) ? -1 : 1;
				if (!isPassable({ enemyX + deltaX, enemyY + deltaY }))
				{
					deltaY = 0;
					deltaX = (enemyX > playerX) ? -1 : 1;
				}
			}
		}
	}
	else{
		deltaX = 0;
		deltaY = 0;
	}

	if (isPassable({ enemyX + deltaX, enemyY + deltaY }))
	{
		setActorInt(enemy.getLocation(), 0);
		enemy.setLocation({ enemyX + deltaX, enemyY + deltaY });
		setActorInt(enemy.getLocation(), enemy.getTile().tileInt);
		Sleep(200);
	}
}

bool Room::lineOfSight(COORD playerPos, Actor& enemy)
{
	bool isFound = false;

	for (int a = 1; isFound == false; a++)
	{
		if (!isPassable({ enemy.getX() - a, enemy.getY() }))
		{
			enemy.setMinX(enemy.getX() - a);
			isFound = true;
		}
	}
	isFound = false;
	for (int a = 1; isFound == false; a++)
	{
		if (!isPassable({ enemy.getX() + a, enemy.getY() }))
		{
			enemy.setMaxX(enemy.getX() + a);
			isFound = true;
		}
	}
	isFound = false;
	for (int a = 1; isFound == false; a++)
	{
		if (!isPassable({ enemy.getX(), enemy.getY() - a }))
		{
			enemy.setMinY(enemy.getY() - a);
			isFound = true;
		}

	}
	isFound = false;
	for (int a = 1; isFound == false; a++)
	{
		if (!isPassable({ enemy.getX(), enemy.getY() + a }))
		{
			enemy.setMaxY(enemy.getY() + a);
			isFound = true;
		}

	}
	if (playerPos.X >= enemy.getMinX() && playerPos.Y >= enemy.getMinY() && 
		playerPos.X <= enemy.getMaxX() && playerPos.Y <= enemy.getMaxY())
		return true;
	else
		return false;
}

bool Room::isAdjacent(COORD playerPos, Actor& enemy){
	if (enemy.getX() == playerPos.X && enemy.getY() == playerPos.Y - 1 || 
		enemy.getX() == playerPos.X && enemy.getY() == playerPos.Y + 1 || 
		enemy.getX() == playerPos.X - 1 && enemy.getY() == playerPos.Y || 
		enemy.getX() == playerPos.X + 1 && enemy.getY() == playerPos.Y)
	{
		return true;
	}
	return false;
}