#include <iostream>

#include "snake.h"

Snake::Snake(int start_row, int start_col) {
	reset(start_row, start_col);
}

void Snake::reset(int start_row, int start_col) {
	Coordinates head_coords;
	head_coords.row = start_row;
	head_coords.col = start_col;

	snake_position.clear();
	snake_position_one_move_behind.clear();

	snake_position.push_back(head_coords);
	snake_position_one_move_behind.push_back(head_coords);

	direction = RIGHT;
}

void Snake::changeDirection(Direction d) {
	if (d == direction)
		return;

	switch (d) {
		case UP:
			if (direction != DOWN) {
				direction = d;
			}
		break;
		case DOWN:
			if (direction != UP) {
				direction = d;
			}
		break;
		case RIGHT:
			if (direction != LEFT) {
				direction = d;
			}
		break;
		case LEFT:
			if (direction != RIGHT) {
				direction = d;
			}
		break;
	}
}

void Snake::moveHead(int row, int col) {
	snake_position[0].row = row;
	snake_position[0].col = col;
}

void Snake::update() {

	snake_position_one_move_behind = snake_position;
	
	switch (direction) {
		case UP:
			snake_position[0].row-=1;
		break;
		case DOWN:
			snake_position[0].row+=1;
		break;
		case RIGHT:
			snake_position[0].col+=1;
		break;
		case LEFT:
			snake_position[0].col-=1;
		break;
	}

	if (getSize() > 1) {
		for (int i = 1; i < getSize(); ++i) {
			snake_position[i].row = snake_position_one_move_behind[i-1].row;
			snake_position[i].col = snake_position_one_move_behind[i-1].col;
		}
	}
}

void Snake::grow() {

	if (getSize() < max_snake_length) {
		Coordinates next_segment;
		
		next_segment.row = snake_position_one_move_behind[getSize()-1].row;
		next_segment.col = snake_position_one_move_behind[getSize()-1].col;
		snake_position.push_back(next_segment);
		snake_position_one_move_behind.push_back(next_segment);
	}
}

bool Snake::collisionWithOtherPartOfSnake() const {
	//sprawdzamy czy waz ma wiecej niz jeden element
	if (getSize() == 1)
		return false;

	//zaczynamy sprawdzac od 2 segmentu
	for (int i = 1; i < getSize(); ++i) {
		if (snake_position[0].row == snake_position[i].row 
			and snake_position[0].col == snake_position[i].col)
		return true;
	}
	return false;
}

int Snake::getSize() const {
	return snake_position.size();
}

bool Snake::isPartOfSnake(int row, int col) const {
	for (int i = 0; i < getSize(); ++i) {
		if (snake_position[i].row == row
			and snake_position[i].col == col)
		return true;
	}
	return false;
}

void Snake::printSnakeCoords() {
	for (int i = 0; i < getSize(); ++i) {
		std::cout << "[ " << snake_position[i].row << ", " << snake_position[i].col << " ]\n";
		std::cout << "{ " << snake_position_one_move_behind[i].row << ", " << snake_position_one_move_behind[i].col << " }\n";
	}
}

Coordinates Snake::getHeadCoords() const {
	return snake_position[0];	
}