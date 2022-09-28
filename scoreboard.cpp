#include "scoreboard.h"

#include <iostream>
#include <algorithm>

bool Scoreboard::compareScore(const Record &sbr1, const Record &sbr2) {
	return (sbr1.score > sbr2.score);
}

// -- public

Scoreboard::Scoreboard() {
	scoreboard_array.resize(5);
	reset();
	loadScoreboardFromFile();
}

void Scoreboard::reset() {
	if (scoreboard_array.size() == 5) {
		Record empty;
		empty.name = "-----";
		empty.score = 0;
		for (auto it = scoreboard_array.begin(); it != scoreboard_array.end(); ++it) {
			*it = empty;
		}
	}
}

void Scoreboard::add(const std::string & name, int score) {
	Record r;
	r.name = name;
	r.score = score;
	scoreboard_array.back() = r;
	std::sort(scoreboard_array.begin(), scoreboard_array.end(), [](const Record &sbr1, const Record &sbr2) {return (sbr1.score > sbr2.score);});
}

void Scoreboard::loadScoreboardFromFile() {
	std::ifstream file;
	file.open("scores.txt");
	if (!file.good()) {
		std::cout << "Nie udalo sie wczytac pliku\n";
		abort();
	}

	scoreboard_array.clear();
	Record sbr;
	std::string line_str = "";
	int line_counter = 0;
	while (std::getline(file, line_str)) {
		switch (line_counter) {
			case 0: {
				sbr.name = line_str;
			}
			break;
			case 1: {
				sbr.score = std::stoi(line_str);
			}
			break;
		}

		line_counter++;
		if (line_counter == 2) {
			line_counter = 0;
			scoreboard_array.push_back(sbr);
		}
	}

	file.close();
}

void Scoreboard::saveScoreboardToFile() {
	std::ofstream file;
	file.open("scores.txt");
	if (!file.good()) {
		std::cout << "Nie udalo sie wczytac pliku\n";
		abort();
	}

	for (auto it = scoreboard_array.begin(); it != scoreboard_array.end(); ++it) {
		file << it->name << "\n";
		file << it->score << "\n";
	}

	file.close();
}

int Scoreboard::getLastScore() const {
	return scoreboard_array[scoreboard_array.size()-1].score;
}

std::string Scoreboard::getName(int i) const {
	return scoreboard_array[i].name;
}

int Scoreboard::getScore(int i) const {
	return scoreboard_array[i].score;
}

int Scoreboard::getSize() const {
	return scoreboard_array.size();
}