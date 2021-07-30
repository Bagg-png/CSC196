#pragma once
#include "Base/Actor.h"

class Player : public nc::Actor {
public:
	Player(const nc::Transform& transform, std::shared_ptr<nc::Shape> shape, float speed);

	void Initialize() override;
	
	void Update(float dt) override;
	void OnCollision(Actor* actor) override;

	void SwapColor ();

public:
	int currentColor = 0;
private:
	int lives = 3;
	
	int weaponLevel = 1;

	float colorTimer = 0;
	float colorRate = 1.0f;

	float fireTimer = 0;
	float fireRate = 0.5f;
	
	float hurtTimer = 0;
	float hurtRate = 20.5f;

	float speed = 300;
	nc::Vector2 velocity;
};
