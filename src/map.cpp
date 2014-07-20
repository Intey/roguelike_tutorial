
#include "libtcod.hpp"
#include "map.hpp"

Map::Map(int width, int height): width(width), height(height){
	tiles = new Tile[width*height];
}

Map::~Map() {
	delete [] tiles;
}

bool Map::isWalkable(int x, int y) const {
	return tiles[x+y*(width)].canWalk;
}

void Map::render() const {
	static const TCODColor darkWall( 0, 0, 100 );
	static const TCODColor darkGround( 0, 50, 150 );
	for ( int x=0; x < width; x++ ) {
		for ( int y=0; y < height; y++ ) {
		TCODConsole::root->setCharBackground( x, y, 
			isWalkable( x, y ) ? darkGround : darkWall );
		}
	}
}

void Map::setWall( int x, int y ) {
	tiles[ x+y*(width) ].canWalk = false;
}
