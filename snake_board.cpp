#include <iostream>

#include "snake_board.h"
#include "snake.h"

SnakeBoard::SnakeBoard(Snake & snake) : snake(snake) {
	board_width = board_height = 20; 
	reset();
}

bool SnakeBoard::isInBoard(int row, int col) const {
	if (row < 0 or row >= getBoardWidth() or col < 0 or col >= getBoardHeight())
		return false;
	
	return true;
}

void SnakeBoard::reset() {
	// wymiary planszy zawsze beda 40x40
	for (int row = 0; row < 20; ++row) {
		for (int col = 0; col < 20; ++col) {
			board[row][col] = '_';
		}
	}

	game_status = GameStatus::RUNNING;
}

void SnakeBoard::debugDisplay() const {
	for (int row = 0; row < 20; ++row) {
		for (int col = 0; col < 20; ++col) {
			if (snake.isPartOfSnake(row, col))
				std::cout << "O";
			else
				std::cout << board[row][col];
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

char SnakeBoard::getFieldInfo(int row, int col) const {
	return board[row][col];
}

int SnakeBoard::getBoardWidth() const {
	return board_width;
}

int SnakeBoard::getBoardHeight() const {
	return board_height;
}

GameStatus SnakeBoard::getGameStatus() const {
	
	if (!isInBoard(snake.getHeadCoords().row, snake.getHeadCoords().col))
		return END;

	if (snake.collisionWithOtherPartOfSnake())
		return END;

	return RUNNING;
}