/*
 * Author: Christian Cutts
 * Date: January 13, 2025
 * Purpose: Snake Game
*/

#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <windows.h>

using namespace std;

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

#define PLAYER_CHAR '@'
#define BACKGROUND '+'
#define BOARDER '#'
#define FOOD 'F'
#define SPEED 100 //lower is faster

bool running = true;
const int boardX = 20;
const int boardY = 20;
const int MAX_LENGTH = 128;
const short unsigned int GAME_SPEED = SPEED;

int playerLocationsX[MAX_LENGTH];
int playerLocationsY[MAX_LENGTH];
int playerDirection; //0=up, 1=down, 2=left, 3=right
int foodLocationX;
int foodLocationY;
int score;


void generateFoodLocation() {
	
	foodLocationX = rand() % (boardX - 3) + 2; //generate random numbers inside board
	foodLocationY = rand() % (boardY - 3) + 2;

	for (int i = 0; i < MAX_LENGTH; i++) {

		// if food generated on top of snake generate again
		if (playerLocationsX[i] == foodLocationX && playerLocationsY[i] == foodLocationY) {

			generateFoodLocation();

		}

	}
}

void growSnake() {

	for (int i = 0; i < MAX_LENGTH; i++) {

		if (playerLocationsX[i] == 0) {

			//set next value in array to previous value.
			playerLocationsX[i] = playerLocationsX[i - 1];
			playerLocationsY[i] = playerLocationsY[i - 1];

			return;
		}
	}
}

void drawBoard() {
	system("cls");
	
	bool itemFound = false;


	for (int y = 0; y < boardY; y++) {
		
		for (int x = 0; x < boardX; x++) {

			itemFound = false;

			for (int i = 0; i < MAX_LENGTH; i++) {

				//check if our x, y is where player is and if so print player char
				if (x == playerLocationsX[i] && y == playerLocationsY[i] && playerLocationsX[i] != 0) {

						cout << "\033[37;47m" << PLAYER_CHAR << "\033[0m";

						itemFound = true;
						break; //break out of MAX_LENGTH loop to not waist cpu cycles
				}
				
			}

			//check if food location is on our x,y and print
			if (x == foodLocationX && y == foodLocationY) {

				cout << "\033[32;41m" << FOOD << "\033[0m";
				
				itemFound = true;
				continue;
			}

			//backgrond || border logic
			if (!itemFound) {

				//check if we are on boarder space
				if (y == 0 || y == boardY - 1 || x == 0 || x == boardX - 1) {

					cout << "\033[44;44m" << BOARDER << "\033[0m";

				}
				//if not print background
				else {

					cout << "\033[90;40m" << BACKGROUND << "\033[0m";

				}
			}
		}

		//when y increments add new line
		cout << endl;
		
	}

	//after board is drawn output our score underneith.
	cout << "score: " << score;

}

void updatePlayer() {

	//Shift previous snake values back 1
	for (int i = MAX_LENGTH - 1; i > 0; i--) {
		if (playerLocationsX[i] != 0) {
			playerLocationsX[i] = playerLocationsX[i - 1];
			playerLocationsY[i] = playerLocationsY[i - 1];
		}
	}

	//increment or decrement head value based on playerInput
	switch (playerDirection) {

	case 0: //up
		playerLocationsY[0]--;
		break;

	case 1: //down
		playerLocationsY[0]++;
		break;

	case 2: //left
		playerLocationsX[0]--;
		break;

	case 3: //right
		playerLocationsX[0]++;
		break;

	}
}

void checkCollision() {

	//check for food collision
	if (playerLocationsX[0] == foodLocationX && playerLocationsY[0] == foodLocationY) {

		score++;

		generateFoodLocation();

		growSnake();

	}

	//check for wall collision
	if (playerLocationsX[0] == 0 || playerLocationsX[0] == boardX - 1 || playerLocationsY[0] == 0 || playerLocationsY[0] == boardY - 1) {
		running = false;
		return;
	}

	//check if snake shorter than 4
	if (playerLocationsX[4] == 0) {
		return;
	}

	//check if player collided with self
	for (int i = 1; i < MAX_LENGTH; i++) {

		if (playerLocationsX[0] == playerLocationsX[i] && playerLocationsY[0] == playerLocationsY[i]) {

			//player collided with self
			running = false;
			return;


		
		}
	}
}

void playerInput() {

	//interate over get key 128 times for smooth input
	for (int i = 0; i < 128; i++) {

		if (_kbhit()) {

			switch ((_getch())) {

			case KEY_UP:
				playerDirection = 0;
				break;

			case KEY_DOWN:
				playerDirection = 1;
				break;

			case KEY_LEFT:
				playerDirection = 2;
				break;

			case KEY_RIGHT:
				playerDirection = 3;
				break;

			}
		}
	}
}



void initialize() {
	srand(time(NULL)); //random seed

	fill_n(playerLocationsY, 128, 0); //clear both snake arrays
	fill_n(playerLocationsX, 128, 0);

	playerLocationsX[0] = { 10 }; //set first initial snake location
	playerLocationsY[0] = { 5 };

	playerDirection = 1; //initial direction of down

	generateFoodLocation();

	score = 0; //set score to 0
}

int main() {
	initialize();

	while (running) {

		playerInput();

		updatePlayer();

		checkCollision();

		drawBoard();

		Sleep(GAME_SPEED);
	}

	cout << endl << "GAME OVER";
	
	char response;

	cout << endl << "Play again? (y/n): ";
	cin >> response;

		if (response == 'y' || response == 'Y') {
			running = true;

			main();
		}
		else {
			return 0;
		}
}
