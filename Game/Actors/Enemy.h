#pragma once
#include "Base/Actor.h"
#include <string>

class Enemy : public nc::Actor {
public:
	Enemy(const nc::Transform& transform, std::shared_ptr<nc::Shape> shape, std::string color, float speed) : nc::Actor{ transform, shape }, color{color}, speed{ speed } {}
	void Update(float dt) override;

	void OnCollision(Actor* actor) override;
private:
	std::string color;
	float speed = 70;
	float fireTimer = 0;
	float fireRate = 3.0f;
};