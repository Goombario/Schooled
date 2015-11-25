#include "../Header Files/Item.h"

Item::Item(Tile t, Stats s) : tile(t), stats(s) {}

Item::Item() {}

// Function definitions
Tile Item::getTile() { return tile; }

Stats Item::getStats() { return stats; }

string Item::getMPickup() { return mPickup; }

string Item::getName() { return name; }

void Item::setTile(Tile t) { tile = t; }

void Item::setStats(Stats s) { stats = s; }

void Item::setMPickup(string s) { mPickup = s; }

void Item::setName(string s) { name = s; }