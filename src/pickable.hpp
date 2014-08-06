class Pickable {
	public: 
		bool pick(Actor *item, Actor *owner);
		virtual bool use(Actor *item, Actor *target);
		void drop(Actor *item, Actor *owner);
};

class Healer: public Pickable {
	public: 
		float amount; 
		Healer(float amount);
		bool use(Actor *item, Actor *target);
};

class LightingBolt: public Pickable {
	public:
		float range, damage;
		LightingBolt(float range, float damage);
		bool use(Actor *item, Actor *owner);
};

class Fireball : public LightingBolt {
	public: 
		Fireball(float range, float damage);
		bool use(Actor *item, Actor *owner);
};

class Confuser : public Pickable {
	public:
		int nbTurns;
		float range;
		Confuser(int nbTurns, float range);
		bool use(Actor *item, Actor *owner);
};
