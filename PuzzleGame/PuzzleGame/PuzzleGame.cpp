#include <iostream>
#include <fstream>

enum class FieldState {
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

bool checkCleared(FieldState fields[]) {
	bool ret = true;
	for (int i = 0; i < width * height; i++) {
		if (fields[i] == FieldState::Object) {
			ret = false;
			break;
		}
	}
	return ret;
}

void initialize(FieldState fields[], int w, int h, const char stageData[]) {
	int si = 0;
	int dx = 0;
	while (true) {
		char ch = stageData[si++];
		if (ch == '\0') break;
		switch (ch) {
		case '#':
			fields[dx++] = FieldState::Block;
			break;
		case ' ':
			fields[dx++] = FieldState::Empty;
			break;
		case '.':
			fields[dx++] = FieldState::ForPlace;
			break;
		case 'p':
			fields[dx++] = FieldState::Person;
			break;
		case 'P':
			fields[dx++] = FieldState::PersonOnForPlace;
			break;
		case 'o':
			fields[dx++] = FieldState::Object;
			break;
		case 'O':
			fields[dx++] = FieldState::ObjectOnForPlace;
			break;
		}
	}
}

int xyToIndex(int x, int y) {
	return x + width * y;
}

FieldState getFieldChar(FieldState fields[], int x, int y) {
	return fields[xyToIndex(x, y)];
}

void setFieldChar(FieldState fields[], int x, int y, FieldState s) {
	fields[xyToIndex(x, y)] = s;
}

void draw(FieldState fields[]) {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			std::cout << static_cast<char>(getFieldChar(fields, x,y));
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

void updateGame(FieldState fields[], char input) {
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

	FieldState nextNextPosChar;
	int nextNextPosX = playerPosX + dX * 2;
	int nextNextPosY = playerPosY + dY * 2;

	FieldState curChar = getFieldChar(fields, playerPosX, playerPosY);
	FieldState nextPosChar = getFieldChar(fields, nextPosX, nextPosY);
	switch (nextPosChar) {
	case FieldState::Empty:
	case FieldState::ForPlace:
		setFieldChar(fields, playerPosX, playerPosY, curChar == FieldState::PersonOnForPlace ? FieldState::ForPlace : FieldState::Empty);
		setFieldChar(fields, nextPosX, nextPosY, nextPosChar == FieldState::Empty ? FieldState::Person : FieldState::PersonOnForPlace);
		playerPosX = nextPosX;
		playerPosY = nextPosY;
		break;
	case FieldState::Object:
	case FieldState::ObjectOnForPlace:
		nextNextPosChar = getFieldChar(fields, nextNextPosX, nextNextPosY);
		if (nextNextPosChar != FieldState::Object && nextNextPosChar != FieldState::Block && nextNextPosChar != FieldState::ObjectOnForPlace) {
			setFieldChar(fields, nextNextPosX, nextNextPosY, nextNextPosChar == FieldState::Empty ? FieldState::Object : FieldState::ObjectOnForPlace);
			setFieldChar(fields, nextPosX, nextPosY, nextPosChar == FieldState::Object ? FieldState::Person : FieldState::PersonOnForPlace);
			setFieldChar(fields, playerPosX, playerPosY, curChar == FieldState::PersonOnForPlace ? FieldState::ForPlace : FieldState::Empty);
			playerPosX = nextPosX;
			playerPosY = nextPosY;
		}
		break;
	}

	isCleared = checkCleared(fields);
}

int main()
{ 
	std::ifstream inputFile("stage.txt", std::ifstream::binary);
	inputFile.seekg(0, std::ifstream::end);
	int fileSize = static_cast<int>(inputFile.tellg());
	inputFile.seekg(0, std::ifstream::beg);
	char* stageData = new char[fileSize];
	inputFile.read(stageData, fileSize);

	FieldState *fields = new FieldState[width * height];
	initialize(fields, width, height, stageData);

	delete[] stageData;
	stageData = 0;

	while (true) {
		draw(fields);
		if (isCleared) break;
		char ch = getInput();
		updateGame(fields, ch);
	}

	delete[] fields;
	fields = 0;

	return 0;
}
