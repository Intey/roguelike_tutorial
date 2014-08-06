class Actor {
	public:
		int x,y,ch; //ch - ascii char. int - for handle more than 255 chars.
		const char *name;
		TCODColor color;
		bool blocks;
		Attacker *attacker;
		Destructible *destructible;
		Ai *ai;
		Pickable *pickable;
		Container *container;

		Actor(int x, int y, int ch, const char *name, const TCODColor &color);
		~Actor();

		void update();
		void render() const;
		float getDistance(int cx, int cy) const;
};
