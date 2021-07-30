#include "Game.h"

#include "Actors/Player.h"
#include "Actors/Enemy.h"
#include "Actors/Projectile.h"
#include "Actors/Upgrade.h"

void Game::Initialize()
{
	engine = std::make_unique<nc::Engine>(); // new Engine() gets called
	engine->StartUp();
	scene = std::make_unique<nc::Scene>(); // new Scene() gets called
	scene->engine = engine.get();

	engine->Get<nc::EventSystem>()->Subscribe("AddPoints", std::bind(&Game::OnAddPoints, this, std::placeholders::_1));
	engine->Get<nc::EventSystem>()->Subscribe("PlayerDead", std::bind(&Game::OnPlayerDead, this, std::placeholders::_1));
}

void Game::ShutDown()
{
	scene->RemoveAllActors();
	engine->ShutDown();
}

void Game::Update(float dt)
{
	stateTimer += dt;
	//switch tab tab *variable name* enter

	switch (state)
	{
	case Game::eState::Title:
		if (Core::Input::IsPressed(VK_SPACE)) {
			state = eState::StartGame;
		}
		break;
	case Game::eState::StartGame:
		engine->Get<nc::AudioSystem>()->ShutDown();
		engine->Get<nc::AudioSystem>()->StartUp();
		engine->Get<nc::AudioSystem>()->AddAudio("explosion", "explosion.wav");
		engine->Get<nc::AudioSystem>()->AddAudio("playerFire", "projectileFire.wav");
		engine->Get<nc::AudioSystem>()->AddAudio("song", "song.mp3");
		score = 0;
		lives = 3;
		state = eState::StartLevel;
		break;
	case Game::eState::StartLevel:
		StartGame(dt);
		state = eState::Game;
		break;
	case Game::eState::Game:
		engine->Get<nc::AudioSystem>()->PlayAudio("song");
		if (scene->GetAllActors<Enemy>().size() == 0) {
			UpdateLevel(dt);
			scene->AddActor(std::make_unique<Upgrade>(nc::Transform{ nc::Vector2{400.0f, 300.0f}, 0.0f, 3.0f }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("BulletUpgrade.txt"),100.0f));
		}
		break;
	case Game::eState::Damage:
		lives -= 1;
		break;
	case Game::eState::GameOver:
		if (Core::Input::IsPressed(VK_SPACE)) {
			scene->RemoveAllActors();
			state = eState::StartGame;
		}
		break;
	default:
		break;
	}

	engine->Update(dt);
	scene->Update(dt);
}

void Game::Draw(Core::Graphics& graphics)
{
	switch (state)
	{
	case Game::eState::Title:
		graphics.SetColor(nc::Color::yellow);
		graphics.DrawString(345, 260 + static_cast<int>(std::sin(stateTimer * 6) * 7), "DaGame");
		graphics.SetColor(nc::Color::orange);
		graphics.DrawString(280, 300, "Press 'S' Key to cycle colors");
		graphics.SetColor(nc::Color::orange);
		graphics.DrawString(300, 360, "Press Space to Start");
		break;
	case Game::eState::StartGame:
		break;
	case Game::eState::StartLevel:
		break;
	case Game::eState::Game:
		break;
	case Game::eState::GameOver:
		graphics.SetColor(nc::Color::red);
		graphics.DrawString(345, 300, "Game Over");
		graphics.SetColor(nc::Color::red);
		graphics.DrawString(325, 360, "Press the spacebar to retry");
		break;
	default:
		break;
	}
	graphics.SetColor(nc::Color::white);
	graphics.DrawString(30, 20, std::to_string(score).c_str());
	graphics.DrawString(750, 20, std::to_string(lives).c_str());

	scene->Draw(graphics);
	engine->Draw(graphics);
}

void Game::UpdateTitle(float dt)
{
	if (Core::Input::IsPressed(VK_SPACE)) {
		stateFunction = &Game::StartGame;
	}
}

void Game::StartGame(float dt)
{
	int enemyCount = nc::RandomRangeInt(2, 7);
		scene->AddActor(std::make_unique<Player>(nc::Transform{ nc::Vector2{400.0f, 300.0f}, 0.0f, 2.5f }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("shape.txt"), 80.0f));
		for (size_t i = 0; i < enemyCount; i++) {
			int r = nc::RandomRangeInt(1, 4);
			switch (r) {
			case 1:
				scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f,800.0f), nc::RandomRange(0.0f,600.0f)},
					nc::RandomRange(0, nc::TwoPi), nc::RandomRange(2.0f,4.0f) }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("EnemyB.txt"), "PlayerB", nc::RandomRangeInt(30,60)));
				break;
			case 2:
				scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f,800.0f), nc::RandomRange(0.0f,600.0f)},
					nc::RandomRange(0, nc::TwoPi), nc::RandomRange(2.0f,4.0f) }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("EnemyY.txt"), "PlayerY", nc::RandomRangeInt(30, 60)));
				break;
			case 3:
				scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f,800.0f), nc::RandomRange(0.0f,600.0f)},
					nc::RandomRange(0, nc::TwoPi), nc::RandomRange(2.0f,4.0f) }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("EnemyG.txt"), "PlayerG", nc::RandomRangeInt(30, 60)));
				break;
			}
		}
}

void Game::UpdateLevel(float dt)
{
	for (size_t i = 0; i < 5; i++) {
		int r = nc::RandomRangeInt(1, 4);
		switch (r) {
		case 1:
			scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f,800.0f), nc::RandomRange(0.0f,600.0f)},
				nc::RandomRange(0, nc::TwoPi), nc::RandomRange(2.0f,4.0f) }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("EnemyB.txt"), "PlayerB", nc::RandomRangeInt(30, 60)));
			break;
		case 2:
			scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f,800.0f), nc::RandomRange(0.0f,600.0f)},
				nc::RandomRange(0, nc::TwoPi), nc::RandomRange(2.0f,4.0f) }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("EnemyY.txt"), "PlayerY", nc::RandomRangeInt(30, 60)));
			break;
		case 3:
			scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f,800.0f), nc::RandomRange(0.0f,600.0f)},
				nc::RandomRange(0, nc::TwoPi), nc::RandomRange(2.0f,4.0f) }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("EnemyG.txt"), "PlayerG", nc::RandomRangeInt(30, 60)));
			break;
		}
	}
}

void Game::OnAddPoints(const nc::Event& event)
{
	score += std::get<int>(event.data);
}

void Game::OnPlayerDead(const nc::Event& event)
{
	lives--;
	std::cout << std::get<std::string>(event.data) << std::endl;
	state = eState::GameOver;
}
