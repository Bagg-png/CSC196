#include "Projectile.h"
#include "Player.h"
#include "Engine.h"
#include "Upgrade.h"

void Upgrade::Update(float dt)
{
	Actor::Update(dt);

	lifetime -= dt;
	//if (lifetime <= 0) destroy = true;   OR |
	destroy = (lifetime <= 0);			//	  V

	std::vector<nc::Color> colors = { nc::Color::white, nc::Color::red, nc::Color::green, nc::Color::blue, nc::Color::purple, nc::Color::orange, nc::Color::yellow, nc::Color::cyan };
	scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 10, 1, colors[nc::RandomRangeInt(0, colors.size())], 10);

	transform.Update();
}

void Upgrade::OnCollision(Actor* actor)
{
	if (dynamic_cast<Player*>(actor)) {
		destroy = true;

		nc::Event event;
		event.name = "AddPoints";
		event.data = 100;
		scene->engine->Get<nc::EventSystem>()->Notify(event);
	}
}
