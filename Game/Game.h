#pragma once
#include "Engine.h"

class Game {
	public:
		enum class eState {
			Title,
			StartGame,
			StartLevel,
			Game,
			Damage,
			GameOver
		};
	public:
		void Initialize();
		void ShutDown();

		void Update(float dt);
		void Draw(Core::Graphics& graphics);

private:
	void UpdateTitle(float dt);
	void StartGame(float dt);
	void UpdateLevel(float dt);
	void OnAddPoints(const nc::Event& event);
	void OnPlayerDead(const nc::Event& event);

	public:
		std::unique_ptr<nc::Engine> engine;
		std::unique_ptr<nc::Scene> scene;

	private:
		eState state = eState::Title;
		float stateTimer = 0.0f;

		void (Game::* stateFunction)(float) = nullptr;

		size_t score = 0;
		size_t lives = 0;
};
