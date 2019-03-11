#include "rules.h"
#include <stdlib.h>


bool changeOnRange(int &data, int min, int max) {
	if (data < min) {
		data = min;
		return true;
	}
	else if (data > max) {
		data = max;
		return true;
	}
	else
		return false;
}


bool operator == (const XY & p1, const XY & p2) {
	return p1.x == p2.x && p1.y == p2.y;
}

bool operator!=(const XY & p1, const XY & p2) {
	return !(p1.x == p2.x && p1.y == p2.y);
}


void creatEmptyGame(Game &game, const GameOptions &options) {
	game.options = options;
	game.board.clear();
	game.shownAmount = 0;
	game.flagsAmount = 0;
	for (int j = 0; j < options.Width; j++) {
		std::vector <Field> column;
		for (int i = 0; i < options.Height; i++) {
			Field field = { false, false, false, 0 };
			column.push_back(field);
		}
		game.board.push_back(column);
	}
}


int nextBombAmount(const Game &game, const int x, const int y) {
	int bombAmount = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0)
				continue;
			if (!(0 <= x + i && x + i < game.options.Width))
				continue;
			if (!(0 <= y + j && y + j < game.options.Height))
				continue;
			if (game.board[x + i][y + j].ifBomb) {
				bombAmount++;
			}
		}
	}
	return bombAmount;
}

void allocateNumbers(Game &game) {
	for (int i = 0; i < game.options.Width; i++)
		for (int j = 0; j < game.options.Height; j++)
			game.board[i][j].nextBombAmount = nextBombAmount(game, i, j);
}

void creatGame(Game &game, const GameOptions &options) {
	game.options = options;
	game.board.clear();
	std::vector<XY> possibleBombPlace;
	game.shownAmount = 0;
	game.flagsAmount = 0;
	for (int j = 0; j < options.Width; j++) {
		std::vector <Field> column;
		for (int i = 0; i < options.Height; i++) {
			Field field = { false, false, false, 0 };
			column.push_back(field);
			XY coordinates = { j, i };
			possibleBombPlace.push_back(coordinates);
		}
		game.board.push_back(column);
	}
	for (int i = 0; i < options.bombAmount; i++) {
		int place = rand() % possibleBombPlace.size();
		XY bombCoordinates = possibleBombPlace[place];
		game.board[bombCoordinates.x][bombCoordinates.y].ifBomb = true;
		possibleBombPlace.erase(possibleBombPlace.begin() + place);
	}
	allocateNumbers(game);
}

void showReq(Game &game, int x, int y) {

	if (!(0 <= x && x < game.options.Width))
		return;
	if (!(0 <= y && y < game.options.Height))
		return;
	if (game.board[x][y].ifShowed)
		return;
	game.board[x][y].ifShowed = true;
	game.shownAmount++;
	if (game.board[x][y].nextBombAmount > 0)
		return;

	for (int i = 0; i < wayToGoSize; i++) {
		if (!(0 <= x + wayToGo[i].x && x + wayToGo[i].x < game.options.Width))
			continue;
		if (!(0 <= y + wayToGo[i].y && y + wayToGo[i].y < game.options.Height))
			continue;
		if (!game.board[x + wayToGo[i].x][y + wayToGo[i].y].ifBomb) {
			showReq(game, x + wayToGo[i].x, y + wayToGo[i].y);
		}

	}
}

void showWholeBoard(Game &game) {
	for (int i = 0; i < game.options.Height; i++) {
		for (int j = 0; j < game.options.Width; j++) {
			game.board[j][i].ifShowed = true;
		}
	}

}
