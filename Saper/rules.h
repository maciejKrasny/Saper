#include <vector>

#ifndef RULES
#define RULES

struct Field {
	bool ifBomb;
	bool ifFlagged;
	bool ifShowed;
	int nextBombAmount;
};

struct GameOptions {
	int bombAmount, Height, Width;
};

struct Game {
	GameOptions options;
	std::vector<std::vector<Field>> board;
	int shownAmount;
	int flagsAmount;
};

struct XY {
	int x;
	int y;
};
enum gameState
{
	during, win, lost
};

struct Statistics {
	int played, winnings, best_time;
};

const XY nullXY = { -9, -9 };

const XY up = { 0, 1 };
const XY down = { 0, -1 };
const XY left = { -1, 0 };
const XY right = { 1, 0 };

#define wayToGoSize 4
const XY wayToGo[] = { up,down,left,right };

bool changeOnRange(int &data, int min, int max);
bool operator == (const XY & p1, const XY & p2);
bool operator!=(const XY & p1, const XY & p2);
void creatGame(Game &game, const GameOptions &options);
void creatEmptyGame(Game &game, const GameOptions &options);
void allocateNumbers(Game &game);
void showReq(Game &game, int x, int y);
void showWholeBoard(Game &game);



#endif