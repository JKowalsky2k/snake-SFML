#ifndef ENGINE_H
#define ENGINE_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "snake.h"
#include "fruit.h"
#include "scoreboard.h"

struct ScoreBoardRecord {
	std::string name;
	int score;
};

enum GameStatus {RUNNING, END};

class Engine {

	sf::RenderWindow & win;
	Snake & snake;
	Fruit & fruit;
	Scoreboard & scoreboard;

	char board[20][20];
	//std::vector < ScoreBoardRecord > scoreboard;

	const int board_width 	= 20;
	const int board_height 	= 20;
	const int field_size 	= 40; 
	float one_update_time 	= 0.1f;

	int score;
	sf::Font font;
	sf::Font snake_font;

	enum States {MENU, PLAY, END, SCOREBOARD, PERMEND};
	States state;
	GameStatus game_status;
	
	void stateMachine();
	void statePLAY();
	void stateMENU();
	void stateEND();
	void stateSCOREBOARD();

	void drawGame(); // drawing board in SFML window
	void drawText(); // drawing board using console
	void debugDisplay();

	void reset(std::string file_name="map1.txt");
	bool isInBoard(int row, int col) const;
	GameStatus getGameStatus() const;
	void setSnake();
	void setFruit();
	bool isWall(int row, int col) const;
	bool hitWall() const;

	void loadMapFromFile(std::string file_name);
	//void loadScoreBoardFromFile();
	//void saveScoreBoardToFile();
	// nie wiem czy to jest dokonce dobrze, z tego co patrzylem w insternecie to jest opcja
	// wyrzucenia compareScore() poza klase albo dopisanie tutaj static.
	//static bool compareScore(const ScoreBoardRecord &sbr1, const ScoreBoardRecord &sbr2);
	
	//scisle powiazana tylko z debugDisplay
	std::string debugMsg(int row, int col);
public:
	Engine(sf::RenderWindow & win, Snake & snake, Fruit & fruit, Scoreboard & scoreboard);
	void run();
};

#endif