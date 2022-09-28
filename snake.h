#ifndef SNAKE_H
#define SNAKE_H

#include <vector>

enum Direction {UP, DOWN, RIGHT, LEFT};

struct Coordinates {
	int row;
	int col;
};

class Snake {
	const int max_snake_length = 100;
	Direction direction;
	std::vector < Coordinates > snake_position;
	std::vector < Coordinates > snake_position_one_move_behind;
public:
	Snake(int start_row, int start_col);
	//zwraca rozmiar weza
	int getSize() const;
	//sprawdza czy waz nie udezyl samego siebie
	bool collisionWithOtherPartOfSnake() const;
	//dodaje kolejny element weza
	void grow();
	// zmienia kierunek
	void changeDirection(Direction d);
	//przsuwa glowe we wskazane miejsce
	void moveHead(int row, int col);
	//aktualizuje model
	void update();
	//resetuje weza
	void reset(int start_row, int start_col);

	bool isPartOfSnake(int row, int col) const;

	void printSnakeCoords();

	Coordinates getHeadCoords() const; 
};

#endif