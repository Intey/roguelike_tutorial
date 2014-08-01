class Destructible {
	public:
		float maxHp;
		float hp;
		float defence;
		const char *corpseName;

		Destructible(float maxHp, float defense, const char *corpseName);
		virtual void die(Actor *owner);
		inline bool isDead() { return hp <= 0; }
		float takeDamage(Actor *owner, float damage);
		float heal(float amount);

};

class MonsterDestructible : public Destructible {
	public:
		MonsterDestructible(float maxHp, float defense, const char *corpseName);
		void die(Actor *owner);
};

class PlayerDestructible : public Destructible {
	public:
		PlayerDestructible(float maxHp, float defense, const char *corpseName);
		void die(Actor *owner);
};
