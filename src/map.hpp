struct Tile {
	bool canWalk;
	bool explored;
	Tile() : explored(false) {}
};

class Map {
	public: 
		int width, height;

		Map(int width, int height);
		~Map();
		bool isWalkable(int x, int y) const;
		bool canWalk(int x, int y) const;
		bool isInFov(int x, int y) const;
		bool isExplored(int x, int y) const;
		void computeFov();
		void render() const;
		void addMonster(int x, int y);
	protected :
		Tile *tiles;
		TCODMap *map;
		friend class BspListener;
		void dig (int x, int y, int x1, int y1);
		void createRoom (bool first, int x, int y, int x1, int y1);
		void addItem(int x, int y);
};

