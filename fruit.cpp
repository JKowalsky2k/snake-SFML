#include <iostream>
#include "fruit.h"

Fruit::Fruit() {
	fruit.row = 0;
	fruit.col = 0;
}

void Fruit::setFruitCoords(int row, int col) {
	fruit.row = row;
	fruit.col = col;
}

Coordinates Fruit::getFruitCoords() const {
	return fruit;
} 

bool Fruit::isFruit(int row, int col) {
	if (fruit.row == row and fruit.col == col)
		return true;

	return false;
}