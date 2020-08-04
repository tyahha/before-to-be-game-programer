#include <iostream>

/**
 * ########
 * # .. p #
 * # oo   #
 * #      #
 * ########
 */

enum FieldState {
	Block = '#',
	Empty = ' ',
	ForPlace = '.',
	Object = 'o',
	Person = 'p',
	PersonOnForPlace = 'P',
	ObjectOnForPlace = 'O',
};

static const int width = 8;
static const int height = 5;

static int playerPosX = 5;
static int playerPosY = 1;

static bool isCleared = false;

static FieldState fields[] = {
	Block, Block, Block, Block, Block, Block, Block, Block,
	Block, Empty, ForPlace, ForPlace, Empty, Person, Empty, Block,
	Block, Empty, Object, Object, Empty, Empty, Empty, Block,
	Block, Empty, Empty, Empty, Empty, Empty, Empty, Block,
	Block, Block, Block, Block, Block, Block, Block, Block,
};

bool checkCleared() {
	bool ret = true;
	for (int i = 0; i < width * height; i++) {
		if (fields[i] == Object) {
			ret = false;
			break;
		}
	}
	return ret;
}

int xyToIndex(int x, int y) {
	return x + width * y;
}

char getFieldChar(int x, int y) {
	return fields[xyToIndex(x, y)];
}

void setFieldChar(int x, int y, FieldState s) {
	fields[xyToIndex(x, y)] = s;
}

void draw() {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			std::cout << getFieldChar(x,y);
		}
		std::cout << std::endl;
	}
	if (!isCleared) {
		std::cout << "Please input key" << std::endl;
		std::cout << "w: up, a: left, d: right, s: down :" << std::endl;
	}
	else {
		std::cout << "Cleared!!!!" << std::endl;
		while (true);
	}
}

char getInput() {
	char ch;
	std::cin >> ch;
	return ch;
}

void updateGame(char input) {
	int dX = 0, dY = 0;
	switch (input) {
	case 'w':
		dY = -1;
		break;
	case 's':
		dY = 1;
		break;
	case 'a':
		dX = -1;
		break;
	case 'd':
		dX = 1;
		break;
	}
	int nextPosX = playerPosX + dX;
	int nextPosY = playerPosY + dY;

	char nextNextPosChar;
	int nextNextPosX = playerPosX + dX * 2;
	int nextNextPosY = playerPosY + dY * 2;

	char curChar = getFieldChar(playerPosX, playerPosY);
	char nextPosChar = getFieldChar(nextPosX, nextPosY);
	switch (nextPosChar) {
	case Empty:
	case ForPlace:
		setFieldChar(playerPosX, playerPosY, curChar == PersonOnForPlace ? ForPlace : Empty);
		setFieldChar(nextPosX, nextPosY, nextPosChar == Empty ? Person : PersonOnForPlace);
		playerPosX = nextPosX;
		playerPosY = nextPosY;
		break;
	case Object:
	case ObjectOnForPlace:
		nextNextPosChar = getFieldChar(nextNextPosX, nextNextPosY);
		if (nextNextPosChar != Object && nextNextPosChar != Block && nextNextPosChar != ObjectOnForPlace) {
			setFieldChar(nextNextPosX, nextNextPosY, nextNextPosChar == Empty ? Object : ObjectOnForPlace);
			setFieldChar(nextPosX, nextPosY, nextPosChar == Object ? Person : PersonOnForPlace);
			setFieldChar(playerPosX, playerPosY, curChar == PersonOnForPlace ? ForPlace : Empty);
			playerPosX = nextPosX;
			playerPosY = nextPosY;
		}
		break;
	}

	isCleared = checkCleared();
}

int main()
{
	while (true) {
		draw();
		char ch = getInput();
		updateGame(ch);
	}
	return 0;
}
