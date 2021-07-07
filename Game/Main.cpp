#include "core.h"
#include "Math/Vector2.h"
#include "Math/Color.h"
#include "Math/Random.h"
#include "Graphics/Shape.h"
#include "Graphics/ParticleSystem.h"
#include <iostream>
#include <vector>
#include <string>

nc::Vector2 position{400,300};
std::vector<nc::Vector2> points = { {-5,-5},{5,-5},{0,5},{-5,-5} };
nc::Shape shape{ points, nc::Color{ 0,1,0 } };
const float speed = 250;
float timer = 0;
nc::ParticleSystem particleSystem;

float deltaTime;
// Delta time is time inbetween the frames
float gameTime = 0;

bool Update(float dt) {
	deltaTime = dt;
	gameTime += deltaTime;
	bool quit = Core::Input::IsPressed(Core::Input::KEY_ESCAPE);

	timer += dt * 5;

	if (Core::Input::IsPressed(Core::Input::BUTTON_LEFT)) {
	int x, y;
	Core::Input::GetMousePos(x, y);
	nc::Vector2 psPosition;
	psPosition.x = static_cast<float>(x);
	psPosition.y = static_cast<float>(y);

	std::vector<nc::Color> colors = { nc::Color::white, nc::Color::red, nc::Color::green, nc::Color::blue, nc::Color::purple, nc::Color::orange, nc::Color::yellow, nc::Color::cyan};
	particleSystem.Create(psPosition, 100, 2,colors[nc::RandomRangeInt(0,colors.size())], 150);
	}
	particleSystem.Update(dt);

	nc::Vector2 input;
	if (Core::Input::IsPressed('W')) input.y = -1;
	if (Core::Input::IsPressed('A')) input.x = -1;
	if (Core::Input::IsPressed('S')) input.y = +1;
	if (Core::Input::IsPressed('D')) input.x = +1;
	position += input * speed * dt;

	return quit;
}

void Draw(Core::Graphics& graphics) {

	float scale = (std::sin(timer)+1) * 2;
	shape.Draw(graphics, position);
	particleSystem.Draw(graphics);

	//graphics.SetColor(nc::Color)
	graphics.DrawString(10, 10, std::to_string(deltaTime).c_str());
	graphics.DrawString(10, 20, std::to_string(gameTime).c_str());
	graphics.DrawString(10, 30, std::to_string(1 / deltaTime).c_str());
}

int main() {
	srand(123);
	std::cout << RAND_MAX << std::endl;
	for (size_t i = 0; i < 5; i++) {
		int r = rand();
		std::cout << r << " " << r / static_cast<float>(RAND_MAX) << std::endl;

	}
	nc::Random();

	char name[] = "GAME";
	Core::Init(name, 800, 600, 60);
	Core::RegisterUpdateFn(Update);
	Core::RegisterDrawFn(Draw);

	particleSystem.StartUp();

	Core::GameLoop();
	Core::Shutdown();

	particleSystem.ShutDown();
}