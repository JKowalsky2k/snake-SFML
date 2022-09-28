#ifndef SNAKE_BOARD_H
#define SNAKE_BOARD_H

enum GameStatus {RUNNING, END};

class Snake;

class SnakeBoard {
	// board size
	char board[20][20];
	int board_width;
	int board_height;

	GameStatus game_status;

	Snake & snake;

	bool isInBoard(int row, int col) const;
public:
	explicit SnakeBoard(Snake & snake);
	void reset();
	void debugDisplay() const;
	char getFieldInfo(int row, int col) const;
	int getBoardWidth() const;
	int getBoardHeight() const;
	GameStatus getGameStatus() const;
};

#endif