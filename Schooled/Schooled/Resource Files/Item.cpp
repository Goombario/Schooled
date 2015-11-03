#include "../Header Files/Item.h"

Item::Item(Tile t, Stats s) : tile(t), stats(s) {}

Item::Item() {}

Tile Item::getTile() { return tile; }

Stats Item::getStats() { return stats; }

void Item::setTile(Tile t) { tile = t; }