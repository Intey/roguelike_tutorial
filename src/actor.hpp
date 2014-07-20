class Actor {
	public:
		int x,y,ch; //ch - ascii char. int - for handle more than 255 chars.
		TCODColor color;
		Actor(int x, int y, int ch, const TCODColor &color);
		void render() const;
};
