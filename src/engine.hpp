class Engine {
	public :
		TCOD_key_t lastKey;
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

		//Engine();
		Engine(int screenWidth, int screenHeight);
		~Engine();
		void update();
		void render();
		void sendToBackground(Actor *actor);
	private:
		bool computeFov;
};

extern Engine engine;
