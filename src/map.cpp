#include "main.hpp"

static const int ROOM_MAX_SIZE		= 12;
static const int ROOM_MIN_SIZE		= 6;
static const int MAX_ROOM_MONSTERS	= 3;
static const int MAX_ROOM_ITEMS		= 2;

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
	map = new TCODMap(width, height);
	TCODBsp bsp(0, 0, width, height);
	bsp.splitRecursive(NULL, 8, ROOM_MAX_SIZE, ROOM_MAX_SIZE, 1.5f, 1.5f);
	BspListener listener(*this);
	bsp.traverseInvertedLevelOrder(&listener, NULL);
}

Map::~Map() {
	delete [] tiles;
	delete map;
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
			map->setProperties(tilex, tiley, true, true);
		}
	}
}

void Map::createRoom(bool first, int x1, int y1, int x2, int y2) {
	dig(x1,y1,x2,y2);
	//place player in center of first room
	if (first) {
		engine.player->x=(x1+x2)/2;
		engine.player->y=(y1+y2)/2;
	}else{
		TCODRandom *rng=TCODRandom::getInstance();
		int nbMosters = rng->getInt(0, MAX_ROOM_MONSTERS);
		while ( nbMosters > 0 ) {
			int x = rng->getInt(x1,x2);
			int y = rng->getInt(y1,y2);
			if ( isWalkable(x,y) ) {
				addMonster(x,y);
			}
			nbMosters--;
		}
		int nbItems=rng->getInt(0, MAX_ROOM_ITEMS);
		while( nbItems > 0 ) {
			int x = rng->getInt(x1,x2);
			int y = rng->getInt(y1,y2);
			if ( isWalkable(x,y) ) {
				addItem(x,y);
			}
			nbItems--;
		}
	}
}

bool Map::isWalkable(int x, int y) const {
	return map->isWalkable(x,y);
}

void Map::render() const {
	static const TCODColor darkWall( 0, 50, 50 );
	static const TCODColor darkGround( 0, 0, 50 );
	static const TCODColor lightWall( 50, 150, 150 );
	static const TCODColor lightGround( 80, 200, 150 );

	for ( int x=0; x < width; x++ ) {
		for ( int y=0; y < height; y++ ) {
			if ( isInFov(x,y) ) {
				TCODConsole::root->setCharBackground( x, y, 
						isWalkable( x, y ) ? lightGround : lightWall );

			}else if ( isExplored(x,y) ){
				TCODConsole::root->setCharBackground( x, y, 
						isWalkable( x, y ) ? darkGround : darkWall );
			}
		}
	}
}

bool Map::isExplored(int x, int y) const {
	return tiles[x+y*width].explored;
}

bool Map::isInFov(int x, int y) const {
	if ( x < 0 || x >= width || y < 0 || y >= height )  {
		return false;
	}
	if ( map->isInFov(x, y)) {
		tiles[x+y*width].explored=true;
		return true;
	}
	return false;
}

void Map::computeFov() {
	map->computeFov(engine.player->x,engine.player->y,
			engine.fovRadius);
}

bool Map::canWalk(int x, int y) const {
	if ( !isWalkable(x,y) ) return false;
	for (Actor **iterator = engine.actors.begin(); iterator != engine.actors.end(); iterator++) {
		Actor *actor = *iterator;
		if ( actor->blocks && actor->x == x && actor->y == y ) {
			return false;
		}
	}
	return true;
}

void Map::addMonster(int x, int y) {
	TCODRandom *rng = TCODRandom::getInstance();
	if ( rng->getInt(0,100) < 80 ) {
		Actor *orc = new Actor(x, y, 'o', "orc", TCODColor::desaturatedGreen);
		orc->destructible = new MonsterDestructible(10,0,"dead orc");
		orc->attacker = new Attacker(3);
		orc->ai = new MonsterAi();
		engine.actors.push(orc);
	} else {
		Actor *troll =new Actor(x,y, 'T', "troll", TCODColor::darkerGreen);
		troll->destructible = new MonsterDestructible(16,1,"troll carcass");
		troll->attacker = new Attacker(4);
		troll->ai = new MonsterAi();
		engine.actors.push(troll);
	}
}

void Map::addItem(int x, int y) {
	TCODRandom *rng = TCODRandom::getInstance();
	int dice = rng->getInt(0, 100);
	if ( dice < 50 ) {
		Actor *healthPotion =  new Actor(x,y,'*',"health potion", TCODColor::violet);
		healthPotion->blocks = false;
		healthPotion->pickable = new Healer(5);
		engine.actors.push(healthPotion);
	} else if ( dice < 80 ) {
		Actor *scrollOfLightingBolt = new Actor(x,y,'#',"scroll of lighting bolt",
			TCODColor::lightYellow);
		scrollOfLightingBolt->blocks = false;
		scrollOfLightingBolt->pickable=new LightingBolt(5,20);
		engine.actors.push(scrollOfLightingBolt);
	} else if ( dice < 90 ) {
		Actor *scrollOfFireball = new Actor(x, y, '^', "scroll of fireball", TCODColor::lightYellow);
		scrollOfFireball->blocks = false;
		scrollOfFireball->pickable = new Fireball(3,12);
		engine.actors.push(scrollOfFireball);
	} else if ( dice < 95 ) {
		Actor *scrollOfConfusion = new Actor(x, y, '%', "scroll of confuse", TCODColor::lightYellow);
		scrollOfConfusion->blocks = false;
		scrollOfConfusion->pickable = new Confuser(10,8);
		engine.actors.push(scrollOfConfusion);
	} else if ( dice < 100 ) {
	Actor *DmgAura = new Actor(x,y,'+', "Damage totem", TCODColor::lightRed);
	DmgAura->blocks = false;
	DmgAura->pickable = new DamageAura(4);
	engine.actors.push(DmgAura);
}

}
