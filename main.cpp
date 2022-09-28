#include <iostream>
#include <SFML/Graphics.hpp>

#include "engine.h"
//#include "snake_board.h"
#include "snake.h"
#include "fruit.h"
#include "scoreboard.h"

int main() {
	srand(time(0));
	
	sf::RenderWindow win;
	Snake snake (10, 10);
	Fruit fruit;
	Scoreboard scoreboard;

	Engine game (win, snake, fruit, scoreboard);

	game.run();
	
	return 0;
}