#include "Player.h"
#include "Projectile.h"
#include "engine.h"
#include "Enemy.h"
#include "Upgrade.h"
#include <memory>

Player::Player(const nc::Transform& transform, std::shared_ptr<nc::Shape> shape, float speed) : nc::Actor{ transform, shape }, speed{ speed } {
	
}

void Player::Initialize() {
	std::unique_ptr locator = std::make_unique<Actor>();
	locator->transform.localPosition = nc::Vector2{ 10,0 };
	AddChild(std::move(locator));

	locator = std::make_unique<Actor>();
	locator->transform.localPosition = nc::Vector2{ 0,5 };
	AddChild(std::move(locator));

	locator = std::make_unique<Actor>();
	locator->transform.localPosition = nc::Vector2{ 0,-5 };
	AddChild(std::move(locator));

	//Create 3 cores for all the colors
	std::unique_ptr powerCore1 = std::make_unique<Actor>(nc::Transform{ nc::Vector2{400.0f, 300.0f}, 0.0f, 20.0f }, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("powerCoreB.txt"));
	std::unique_ptr powerCore2 = std::make_unique<Actor>(nc::Transform{ nc::Vector2{400.0f, 300.0f}, 90.0f, 20.0f }, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("powerCoreB.txt"));

	powerCore1->transform.localPosition = nc::Vector2{ -5,0 };
	AddChild(std::move(powerCore1));

	powerCore2->transform.localPosition = nc::Vector2{ -5,0 };
	AddChild(std::move(powerCore2));
}

void Player::Update(float dt)
{
	Actor::Update(dt);

	//children[3]->transform.localRotation += 5 * dt;

	//movement
	float thrust = 0;
	colorTimer -= dt;
	if (Core::Input::IsPressed('D')) transform.rotation -= -4 * dt;
	if (Core::Input::IsPressed('A')) transform.rotation += -4 * dt;
	if (Core::Input::IsPressed('W')) thrust = speed;
	if (Core::Input::IsPressed('S') && colorTimer <= 0) { 
		colorTimer = colorRate;  
		SwapColor(); 
		std::unique_ptr powerCore1 = std::make_unique<Actor>();
		std::unique_ptr powerCore2 = std::make_unique<Actor>();
		switch (currentColor)
		{
		case 0:
			powerCore1 = std::make_unique<Actor>(nc::Transform{ nc::Vector2{400.0f, 300.0f}, 0.0f, 20.0f }, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("powerCoreB.txt"));
			powerCore2 = std::make_unique<Actor>(nc::Transform{ nc::Vector2{400.0f, 300.0f}, 90.0f, 20.0f }, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("powerCoreB.txt"));
			break;
		case 1:
			powerCore1 = std::make_unique<Actor>(nc::Transform{ nc::Vector2{400.0f, 300.0f}, 0.0f, 20.0f }, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("powerCoreY.txt"));
			powerCore2 = std::make_unique<Actor>(nc::Transform{ nc::Vector2{400.0f, 300.0f}, 90.0f, 20.0f }, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("powerCoreY.txt"));
			break;
		case 2:
			powerCore1 = std::make_unique<Actor>(nc::Transform{ nc::Vector2{400.0f, 300.0f}, 0.0f, 20.0f }, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("powerCoreG.txt"));
			powerCore2 = std::make_unique<Actor>(nc::Transform{ nc::Vector2{400.0f, 300.0f}, 90.0f, 20.0f }, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("powerCoreG.txt"));
			break;
		default:
			break;
		}
		powerCore1->transform.localPosition = nc::Vector2{ -5,0 };
		AddChild(std::move(powerCore1));

		powerCore2->transform.localPosition = nc::Vector2{ -5,0 };
		AddChild(std::move(powerCore2));
	}
	

	nc::Vector2 acceleration;
	acceleration = nc::Vector2::Rotate(nc::Vector2::right, transform.rotation) * thrust * dt;
	//nc::Vector2 gravity = (nc::Vector2{ 400,300 } - transform.position).Normalized() * 50;
	//nc::Vector2 gravity = nc::Vector2::down * 50;
	//acceleration += gravity;

	velocity += acceleration * dt;
	//transform.position += velocity * dt;
	transform.position += nc::Vector2::Rotate(nc::Vector2::right, transform.rotation) * thrust * dt;

	//velocity *= 0.975f;

	transform.position.x = nc::Wrap(transform.position.x, 0.0f, 800.0f);
	transform.position.y = nc::Wrap(transform.position.y, 0.0f, 600.0f);

	hurtTimer -= dt;
	//fire
	fireTimer -= dt;
	if (fireTimer <= 0 && Core::Input::IsPressed(VK_SPACE)) {
		fireTimer = fireRate;
		std::vector<nc::Vector2> points = { {-5,-5},{5,-5},{0,10},{-5,-5} };
		std::shared_ptr<nc::Shape> shape = std::make_shared<nc::Shape>(points, nc::Color{ 1,0,0 });
		{
			nc::Transform t = children[0]->transform;
			switch (weaponLevel)
			{
			case 1:
				t.scale = 1.0f;
				break;
			case 2:
				t.scale = 2.0f;
				break;
			case 3:
				t.scale = 3.0f;
				break;
			case 4:
				t.scale = 4.0f;
				break;
			case 5:
				t.scale = 5.0f;
				break;
			case 6:
				t.scale = 6.0f;
				break;
			default:
				t.scale = 7.0f;
			}

			std::unique_ptr<Projectile> projectile;
			switch (currentColor)
			{
			case 0:
				//Blue
				projectile = std::make_unique<Projectile>(t, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("projectileB.txt"), 100.0f);
				projectile->tag = "PlayerB";
				break;
			case 1:
				//Yellow
				projectile = std::make_unique<Projectile>(t, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("projectileY.txt"), 100.0f);
				projectile->tag = "PlayerY";
				break;
			case 2:
				//Green
				projectile = std::make_unique<Projectile>(t, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("projectileG.txt"), 100.0f);
				projectile->tag = "PlayerG";
				break;
			}
			scene->AddActor(std::move(projectile));
		}
		//{
		//	nc::Transform t = children[2]->transform;
		//	t.scale = 2.0f;

		//	std::unique_ptr<Projectile> projectile;
		//	switch (currentColor)
		//	{
		//	case 0:
		//		//Blue
		//		projectile = std::make_unique<Projectile>(t, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("projectileB.txt"), 100.0f);
		//		projectile->tag = "PlayerB";
		//		break;
		//	case 1:
		//		//Yellow
		//		projectile = std::make_unique<Projectile>(t, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("projectileY.txt"), 100.0f);
		//		projectile->tag = "PlayerY";
		//		break;
		//	case 2:
		//		//Green
		//		projectile = std::make_unique<Projectile>(t, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("projectileG.txt"), 100.0f);
		//		projectile->tag = "PlayerG";
		//		break;
		//	}
		//	scene->AddActor(std::move(projectile));
		//}
		scene->engine->Get<nc::AudioSystem>()->PlayAudio("playerFire");
	}

	std::vector<nc::Color> colors = { nc::Color::white, nc::Color::blue, nc::Color::red };
	scene->engine->Get<nc::ParticleSystem>()->Create(children[0]->transform.position, 3, 2, colors, 10, children[0]->transform.rotation, nc::DegToRad(30));
}

void Player::OnCollision(Actor* actor)
{
	if (dynamic_cast<Upgrade*>(actor)) {
		weaponLevel += 1;
	}
	if (dynamic_cast<Enemy*>(actor)) {
		destroy = true;
		scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 200, 1, nc::Color::white, 50);
		scene->engine->Get<nc::AudioSystem>()->PlayAudio("explosion");

		nc::Event event;
		event.name = "PlayerDead";
		event.data = std::string("yes I'm dead!");
		scene->engine->Get<nc::EventSystem>()->Notify(event);
	}
}

void Player::SwapColor() {
	currentColor += 1;
	if (currentColor > 2) {
		currentColor = 0;
	}
}
