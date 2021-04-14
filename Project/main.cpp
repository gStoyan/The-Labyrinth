#include"Game.h"


int main() {
	bool moving;
	Game game("Project",
		640, 480,
		4, 5,
		false);

	while (!game.getWindowShouldClose())
	{
		//UPDATE INPUT ---
		
		game.update();
		game.render();
	}
}