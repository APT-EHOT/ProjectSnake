#include <iostream>
#include <conio.h>
#include <Windows.h>

using namespace std;

bool processIsOver, gameIsOver, classicVariant;
int snakeX, snakeY, fruitX, fruitY;

int tailX[100], tailY[100]; int tailLength;

int KEY_DELTA_SCORE, KEY_DELTA_TIME;

const int GAMESPACE_WIDTH = 40;
const int GAMESPACE_HEIGHT = 20;

int score;

enum snakeDirections { STOP = 0, LEFT, RIGHT, UP, DOWN };
snakeDirections currentDirection;

// pre-initilization, setting difficulty and walls passibility
void setDifficulty() {

	int inputDifficulty = -1;
	int inputVariant = -1;

	//setting difficulty
	while ((inputDifficulty != 0) && (inputDifficulty != 1) && (inputDifficulty != 2) && (inputDifficulty != 3)) {

		cout << "\n\nДобро пожаловать в игру ProjectSnake v1.0!\n"
			<< "Выберите уровень сложности (1 - простая, 2 - средняя, 3 - сложная, 0 - выход): ";

		cin >> inputDifficulty;

		switch (inputDifficulty) {

		case 1:
			KEY_DELTA_TIME = 300;
			KEY_DELTA_SCORE = 10;
			break;
		case 2:
			KEY_DELTA_TIME = 150;
			KEY_DELTA_SCORE = 15;
			break;
		case 3:
			KEY_DELTA_TIME = 0;
			KEY_DELTA_SCORE = 20;
			break;
		case 0:
			processIsOver = true;
			exit(0);
			break;
		default:
			cout << "\n\nНеправильно задана сложность!\n\n";
			break;

		}

	}

	//setting passability
	while ((inputVariant != 0) && (inputVariant != 1)) {

		cout << "\n\nВыберите проходимость стенок (0 - через стены нельзя проходить, 1 - можно): ";
		cin >> inputVariant;

		switch (inputVariant) {
		case 0:
			classicVariant = true;
			break;
		case 1:
			classicVariant = false;
			break;
		default:
			cout << "\n\nНеправильный формат ответа!\n\n";
			break;
		}
	}

}

// game initialization
void init() {

	gameIsOver = false;
	currentDirection = STOP;
	score = 0; tailLength = 0;

	for (int i = 0; i < 100; i++) {
		tailX[i] = 0;
		tailY[i] = 0;
	}

	snakeX = GAMESPACE_WIDTH / 2 - 1;
	snakeY = GAMESPACE_HEIGHT / 2 - 1;

	fruitX = rand() % GAMESPACE_WIDTH;
	fruitY = rand() % GAMESPACE_HEIGHT;

}

// drawing game space, snake and fruit
void drawGamespace() {

	system("cls");

	// top border
	for (int i = 0; i <= GAMESPACE_WIDTH; i++)
		cout << "#";
	cout << "\n";

	// left and right borders, snake and fruit
	for (int i = 0; i < GAMESPACE_HEIGHT; i++) {

		for (int j = 0; j < GAMESPACE_WIDTH; j++) {
			if ((j == 0) || (j == GAMESPACE_WIDTH - 1))
				cout << "#";

			if ((i == snakeY) && (j == snakeX))
				cout << "0";
			else if ((i == fruitY) && (j == fruitX))
				cout << "F";
			else {

				bool printSpace = true;

				for (int k = 0; k < tailLength; k++) {
					if ((tailX[k] == j) && (tailY[k] == i)) {
						cout << "o";
						printSpace = false;
					}
				}

				if (printSpace)
					cout << " ";

			}

		}

		cout << "\n";
	}

	// bottom border
	for (int i = 0; i <= GAMESPACE_WIDTH; i++)
		cout << "#";

	cout << "\n";
	cout << "Score: " << score;

}

// converting input to direction
void handleInput() {

	if (_kbhit()) {

		switch (_getch())
		{

		case 'a':
			currentDirection = LEFT;
			break;
		case 'd':
			currentDirection = RIGHT;
			break;
		case 'w':
			currentDirection = UP;
			break;
		case 's':
			currentDirection = DOWN;
			break;
		case 'x':
			gameIsOver = true;
			break;

		}

	}

}

void borderCheck() {

}

// moving snake, ending the game and handle fruit eating event
void processLogic() {

	// tail logic
	int prevSnakeX = tailX[0];
	int prevSnakeY = tailY[0];
	int prevNextX, prevNextY;

	tailX[0] = snakeX;
	tailY[0] = snakeY;

	//moving tail
	for (int i = 1; i < tailLength; i++) {
		prevNextX = tailX[i];
		prevNextY = tailY[i];

		tailX[i] = prevSnakeX;
		tailY[i] = prevSnakeY;

		prevSnakeX = prevNextX;
		prevSnakeY = prevNextY;

	}

	// changing coords of snake
	switch (currentDirection)
	{
	case LEFT:
		snakeX--;
		break;
	case RIGHT:
		snakeX++;
		break;
	case UP:
		snakeY--;
		break;
	case DOWN:
		snakeY++;
		break;
	}

	// wall passing for classic variant
	if (classicVariant) {
		if ((snakeX > GAMESPACE_WIDTH - 2) || (snakeX < 0) ||
			(snakeY > GAMESPACE_HEIGHT - 1) || (snakeY < 0)) {
			gameIsOver = true;
		}
	}
	// wall passing for non-classic variant
	else {
		if (snakeX >= GAMESPACE_WIDTH - 1)
			snakeX = 0;
		else if (snakeX < 0)
			snakeX = GAMESPACE_WIDTH - 2;

		if (snakeY >= GAMESPACE_HEIGHT)
			snakeY = 0;
		else if (snakeY < 0)
			snakeY = GAMESPACE_HEIGHT - 1;
	}

	// main gameover condition
	for (int i = 0; i < tailLength; i++) {
		if ((tailX[i] == snakeX) && (tailY[i] == snakeY)) {
			gameIsOver = true;
		}
	}

	// fruit eating event
	if ((snakeX == fruitX) && (snakeY == fruitY)) {

		score += KEY_DELTA_SCORE;
		fruitX = rand() % GAMESPACE_WIDTH;
		fruitY = rand() % GAMESPACE_HEIGHT;
		tailLength++;

	}

	Sleep(KEY_DELTA_TIME);

}

int main()
{
	setlocale(0, "");
	processIsOver = false;

	// game looping
	while (!processIsOver) {

		// pre-init
		setDifficulty();
		// init
		init();

		// move looping
		while (!gameIsOver) {

			drawGamespace();
			handleInput();
			processLogic();

		}
		// game over event
		system("cls");
		cout << " ####   ####  ##   # #####\n##     ##  ## ### ## ##\n## ### ###### ## # # ####\n##  ## ##  ## ##   # ##\n ####  ##  ## ##   # #####\n\n ####  ##  ## #####  #####    ###\n##  ## ##  ## ##     ##  ##   ###\n##  ## ##  ## ####   #####    ###\n##  ##  ####  ##     ##  ##\n ####    ##   #####  ##  ##   ###\n\n \t\tЗмейка мертва! x_x";
	}
}

