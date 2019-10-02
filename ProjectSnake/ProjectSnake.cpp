#include <iostream>
#include <conio.h>
#include <Windows.h>

using namespace std;

HANDLE hConsole;
COORD gamespacePosition;
COORD scorePosition;

bool processIsOver, gameIsOver, classicVariant;
int snakeX, snakeY, fruitX, fruitY;

int tailX[100], tailY[100]; int tailLength;

int KEY_DELTA_SCORE, KEY_DELTA_TIME;

const int GAMESPACE_WIDTH = 40;
const int GAMESPACE_HEIGHT = 20;

int score;

enum snakeDirections { STOP = 0, LEFT, RIGHT, UP, DOWN };
snakeDirections currentDirection;

// pre-initialization, setting difficulty and walls passibility
void setDifficulty() {

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 15);

	int inputDifficulty = -1;
	int inputVariant = -1;

	//setting difficulty
	while ((inputDifficulty != 0) && (inputDifficulty != 1) && (inputDifficulty != 2) && (inputDifficulty != 3)) {

		cout << "\n\nДобро пожаловать в игру ProjectSnake v1.1!\n"
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

	system("cls");

	gamespacePosition.X = 1; // coords for cursor to draw gamespace
	gamespacePosition.Y = 1;

	scorePosition.X = 7; // coords for cursor to rewrite score
	scorePosition.Y = 22;

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

	SetConsoleTextAttribute(hConsole, 14);

	// top border
	for (int i = 0; i <= GAMESPACE_WIDTH; i++)
		cout << "#";
	cout << "\n";

	// left and right borders
	for (int i = 0; i < GAMESPACE_HEIGHT; i++) {

		for (int j = 0; j < GAMESPACE_WIDTH + 1; j++) {
			if ((j == 0) || (j == GAMESPACE_WIDTH))
				cout << "#";
			else
				cout << " ";

		}

		cout << "\n";
	}

	// bottom border
	for (int i = 0; i <= GAMESPACE_WIDTH; i++)
		cout << "#";
	cout << "\n";
	cout << "Score: ";

	SetConsoleCursorPosition(hConsole, scorePosition);
	cout << score;

}

// drawing game space, snake and fruit
void drawGamespace() {

	SetConsoleTextAttribute(hConsole, 10);

	gamespacePosition.Y = 1;

	// snake and fruit
	for (int i = 0; i < GAMESPACE_HEIGHT; i++) {
		SetConsoleCursorPosition(hConsole, gamespacePosition);

		for (int j = 1; j < GAMESPACE_WIDTH; j++) {

			if ((i == snakeY) && (j == snakeX)) {
				cout << "0";
			}

			else if ((i == fruitY) && (j == fruitX)) {
				SetConsoleTextAttribute(hConsole, 12);
				cout << "X";
				SetConsoleTextAttribute(hConsole, 10);
			}

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

		gamespacePosition.Y++;

	}
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
		if ((snakeX > GAMESPACE_WIDTH - 1) || (snakeX < 1) ||
			(snakeY > GAMESPACE_HEIGHT - 1) || (snakeY < 0)) {
			gameIsOver = true;
		}
	}
	// wall passing for non-classic variant
	else {
		if (snakeX > GAMESPACE_WIDTH - 1)
			snakeX = 1;
		else if (snakeX < 1)
			snakeX = GAMESPACE_WIDTH - 1;

		if (snakeY >= GAMESPACE_HEIGHT)
			snakeY = 0;
		else if (snakeY < 0)
			snakeY = GAMESPACE_HEIGHT - 1;
	}

	// main gameover condition (if snake eats itself)
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

		SetConsoleCursorPosition(hConsole, scorePosition);
		cout << score;
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
		SetConsoleTextAttribute(hConsole, 12);
		cout << " ####   ####  ##   # #####\n##     ##  ## ### ## ##\n## ### ###### ## # # ####\n##  ## ##  ## ##   # ##\n ####  ##  ## ##   # #####\n\n ####  ##  ## #####  #####    ###\n##  ## ##  ## ##     ##  ##   ###\n##  ## ##  ## ####   #####    ###\n##  ##  ####  ##     ##  ##\n ####    ##   #####  ##  ##   ###\n\n\tЗмейка мертва! x_x\tВаш счёт: " << score;
	}
}

