class Engine {
	public :
		TCOD_key_t lastKey;
		TCOD_mouse_t mouse;
		TCODList<Actor *> actors;
		Actor *player;
		Map *map;

		enum GameStatus {
			STARTUP,
			IDLE,
			NEW_TURN,
			VICTORY,
			DEFEAT
		} gameStatus;

		int fovRadius;
		int screenWidth;
		int screenHeight;
		Gui *gui;

		Engine(int screenWidth, int screenHeight);
		~Engine();

		void update();
		void render();

		void sendToBackground(Actor *actor);

		Actor *getClosestMonster(int x, int y, float range) const;
		bool pickATile(int *x, int *y, float maxRange = 0.0f);
		Actor *getActor(int x, int y) const;

	private:
		bool computeFov;
};

extern Engine engine;
