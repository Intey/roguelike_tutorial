#include "libtcod.hpp"
#include "map.hpp"
#include "actor.hpp"
#include "engine.hpp"

static const int ROOM_MAX_SIZE = 12;
static const int ROOM_MIN_SIZE = 6;

class BspListener : public ITCODBspCallback {
	private:
		Map &map;
		int roomNum;
		int lastx, lasty;
	public:
		BspListener(Map &map) : map(map), roomNum(0) {}
		bool visitNode(TCODBsp *node, void *userData) {
			if ( node->isLeaf() ) {
				int x, y, h, w;
				TCODRandom *rng = TCODRandom::getInstance();
				w=rng->getInt(ROOM_MIN_SIZE, node->w-2);
				h=rng->getInt(ROOM_MIN_SIZE, node->h-2);
				x=rng->getInt(node->x+1, node->x+node->w-w-1);
				y=rng->getInt(node->y+1, node->y+node->h-h-1);
				map.createRoom(roomNum == 0, x, y, x+w-1, y+h-1);
				if ( roomNum != 0 ) {
					map.dig(lastx, lasty, x+w/2, lasty);
					map.dig(x+w/2, lasty, x+w/2, y+h/2);
				}
				lastx = x+w/2;
				lasty = y+ h/2;
				roomNum++;
			}
			
			return true;
		}

};

Map::Map(int width, int height): width(width), height(height){
	tiles = new Tile[width*height];
	TCODBsp bsp(0, 0, width, height);
	bsp.splitRecursive(NULL, 8, ROOM_MAX_SIZE, ROOM_MAX_SIZE, 1.5f, 1.5f);
	BspListener listener(*this);
	bsp.traverseInvertedLevelOrder(&listener, NULL);
}

Map::~Map() {
	delete [] tiles;
}

void Map::dig(int x, int y, int x1, int y1) {
	if ( x1 < x) {
		int tmp = x1;
		x1 = x; 
		x = tmp; 
	}
	if  (y1 < y) {
		int tmp = y1;
		y1 = y; 
		y = tmp; 
	}
	for (int tilex=x; tilex <= x1; tilex++ ){
		for (int tiley = y; tiley <= y1; tiley++){
			tiles[tilex+tiley*width].canWalk=true;
		}
	}
}

void Map::createRoom(bool first, int x, int y, int x1, int y1) {
	dig(x,y,x1,y1);
	//place player in center of first room
	if (first) {
		engine.player->x=(x+x1)/2;
		engine.player->y=(y+y1)/2;
	}else{
		TCODRandom *rng=TCODRandom::getInstance();
		if (rng->getInt(0,3)==0) {
			engine.actors.push(new Actor((x+x1)/2, (y+y1)/2, 'D', 
						TCODColor::yellow));
		}
	}
}

bool Map::isWalkable(int x, int y) const {
	return tiles[x+y*(width)].canWalk;
}

void Map::render() const {
	static const TCODColor darkWall( 0, 50, 50 );
	static const TCODColor darkGround( 0, 0, 50 );

	for ( int x=0; x < width; x++ ) {
		for ( int y=0; y < height; y++ ) 
			TCODConsole::root->setCharBackground( x, y, 
				isWalkable( x, y ) ? darkGround : darkWall );
	}
}

