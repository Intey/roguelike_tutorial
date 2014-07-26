class Actor {
	public:
		int x,y,ch; //ch - ascii char. int - for handle more than 255 chars.
		const char *name;
		TCODColor color;
		Actor(int x, int y, int ch, const char *name, const TCODColor &color);
		void update();
		bool moveOrAttack(int x, int y);
		void render() const;
};
