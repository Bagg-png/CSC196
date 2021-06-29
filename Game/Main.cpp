#include "core.h"
#include <iostream>

bool Update(float dt) {
	return false;
}

void Draw(Core::Graphics& graphics) {
	for (int i = 0; i < 100; i++) {
		graphics.SetColor(RGB(rand() % 256, rand() % 256, rand() % 256));
		graphics.DrawLine(static_cast<float>(rand() % 801), static_cast<float>(rand() % 601), static_cast<float>(rand() % 801), static_cast<float>(rand() % 601));
	}
	}

int main() {
	char name[] = "GAME";
	Core::Init(name, 800, 600);
	Core::RegisterUpdateFn(Update);
	Core::RegisterDrawFn(Draw);

	Core::GameLoop();
	Core::Shutdown();
}