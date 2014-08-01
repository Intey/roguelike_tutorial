class Pickable {
	public: 
		bool pick(Actor *item, Actor *owner);
		virtual bool use(Actor *item, Actor *target);
};

class Healer: public Pickable {
	public: 
		float amount; 
		Healer(float amount);
		bool use(Actor *item, Actor *target);
};
