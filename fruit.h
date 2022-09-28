#ifndef FRUIT_H
#define FRUIT_H

//tylko zeby uzyc: struct Coordinates {};
#include "snake.h"

class Fruit {
	Coordinates fruit;
public:
	Fruit();
	void setFruitCoords(int row, int col);
	Coordinates getFruitCoords() const;
	bool isFruit(int row, int col);
};

#endif