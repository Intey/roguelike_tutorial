struct Tile {
	bool canWalk;
	Tile() : canWalk(false) {}
};

class Map {
	public: 
		int width, height;

		Map(int width, int height);
		~Map();
		bool isWalkable(int x, int y) const;
		void render() const;
	protected :
		Tile *tiles;
		friend class BspListener;
		void dig (int x, int y, int x1, int y1);
		void createRoom (bool first, int x, int y, int x1, int y1);
};

