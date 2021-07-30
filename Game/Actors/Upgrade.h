#pragma once
#include "Base/Actor.h"

class Upgrade : public nc::Actor {
public:
	Upgrade(const nc::Transform& transform, std::shared_ptr<nc::Shape> shape) : nc::Actor{ transform, shape } {}
	Upgrade(const nc::Transform& transform, std::shared_ptr<nc::Shape> shape, float lifetime) : nc::Actor{ transform, shape }, lifetime{ lifetime } {}

	void OnCollision(Actor* actor) override;

	void Update(float dt) override;
private:
	float lifetime{ 20 };
};
