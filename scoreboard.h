#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <vector>
#include <fstream>

struct Record {
	std::string name;
	int score;
};

class Scoreboard {

	std::vector < Record > scoreboard_array;
	// nie wiem czy to jest dokonce dobrze, z tego co patrzylem w insternecie to jest opcja
	// wyrzucenia compareScore() poza klase albo dopisanie tutaj static.
	static bool compareScore(const Record &sbr1, const Record &sbr2);
public:
	Scoreboard();
	void loadScoreboardFromFile();
	void saveScoreboardToFile();
	void reset();
	void add(const std::string & name, int score);
	int getLastScore() const;
	std::vector < Record > getVector() {return scoreboard_array;};

	std::vector < Record >::iterator BEGIN() {return scoreboard_array.begin();};
	std::vector < Record >::iterator END() {return scoreboard_array.end();};

	std::string getName(int i) const;
	int getScore(int i) const;
	int getSize() const;
};

#endif
