#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "Draw.h"
#include "rules.h"
#include <string>


ALLEGRO_COLOR red;
ALLEGRO_COLOR green;
ALLEGRO_COLOR black;
ALLEGRO_COLOR grey;
ALLEGRO_COLOR dark_grey;
ALLEGRO_COLOR pink;
ALLEGRO_COLOR white;
ALLEGRO_COLOR blue_win;
ALLEGRO_COLOR blue_number;
ALLEGRO_COLOR fair_green;
ALLEGRO_COLOR orange;

ALLEGRO_FONT * fonty;
ALLEGRO_DISPLAY * window;
int lineHeight;
ALLEGRO_SAMPLE * explosion;
ALLEGRO_SAMPLE * victory;

ALLEGRO_COLOR * numbersColors[9] = { &grey, &blue_number, &green, &orange, &red, &pink, &black, &black, &black };

void initDraw(ALLEGRO_DISPLAY * windowy, ALLEGRO_SAMPLE * sound1, ALLEGRO_SAMPLE * sound2) {
	orange = al_map_rgb(244, 133, 42);
	red = al_map_rgb(244, 67, 54);
	blue_win = al_map_rgb(10,108,189);
	blue_number = al_map_rgb(7,33,59);
	white = al_map_rgb(255, 255, 255);
	fair_green = al_map_rgb(118, 255, 3);
	green = al_map_rgb(76, 175, 80);
	black = al_map_rgb(0, 0, 0);
	grey = al_map_rgb(224, 224, 224);
	dark_grey = al_map_rgb(122, 122, 122);
	pink = al_map_rgb(255, 51, 204);
	fonty = al_load_ttf_font("heroes_legend.ttf", 10, 0);
	lineHeight = al_get_font_line_height(fonty);
	explosion = sound1;
	victory = sound2;
	window = windowy;
}

void resizeIfNeed(int width, int height) {
	if (al_get_display_height(window) != height || al_get_display_width(window) != width)
		al_resize_display(window, width, height);
}

void drawMessage(std::string text) {
	al_draw_filled_rectangle(widthM / 6, widthM / 6, 5 * widthM / 6, widthM / 6 + 2 * heightB, red); 
	al_draw_multiline_text(fonty, white, 2.5 * widthM / 5, (widthM / 5 + widthM / 5 + heightB) / 2 - lineHeight / 2, 3 * widthM / 5, 20, ALLEGRO_ALIGN_CENTER, text.c_str());
}

void coordinatesFieldFromMouse(XY &position, int x, int y) {
	if (y < topPart)
		position = nullXY;
	else {
		position.x = x / fieldSize;
		position.y = (y - topPart) / fieldSize;
	}
}

bool ifClickedButton(const Button &button, int x, int y) {
	return (x > button.lg.x && x < button.pd.x && y > button.lg.y && y < button.pd.y);
}


void drawButton(const Button &button) {

	al_draw_filled_rectangle(button.lg.x-5, button.lg.y-5, button.pd.x+1, button.pd.y+1, grey);
	al_draw_filled_rectangle(button.lg.x-3, button.lg.y-3, button.pd.x+3, button.pd.y+3, grey);
	al_draw_text(fonty, black, (button.lg.x + button.pd.x) / 2, (button.lg.y + button.pd.y) / 2 - lineHeight / 2, ALLEGRO_ALIGN_CENTRE, button.text.c_str());
}

void drawInput(const Button &button, int numberToEnter, bool ifFlagged) {
	al_draw_filled_rectangle(button.lg.x - 5, button.lg.y - 3, button.pd.x + 3, button.pd.y + 1, grey);
	al_draw_filled_rectangle(button.lg.x-3, button.lg.y-3, button.pd.x+3, button.pd.y+3, grey);
	al_draw_text(fonty, black, (button.lg.x + button.pd.x) / 2, (button.lg.y + button.pd.y) / 2 - lineHeight / 2, ALLEGRO_ALIGN_CENTER, std::to_string(numberToEnter).c_str());
	if (ifFlagged)
		al_draw_rectangle(button.lg.x, button.lg.y, button.pd.x, button.pd.y, white, 2);
}

buttonRole whichButtonWasClicked(int x, int y, view view) {
	if (view == MENU) {
		if (ifClickedButton(bNowaGra, x, y))
			return bNowaGra.role;
		if (ifClickedButton(bUtworzPlansze, x, y))
			return bUtworzPlansze.role;
		if (ifClickedButton(bWczytajPlansze, x, y))
			return bWczytajPlansze.role;
		if (ifClickedButton(bStatystki, x, y))
			return bStatystki.role;
		if (ifClickedButton(bWyjscie, x, y))
			return bWyjscie.role;
	}
	else if (view == POZIOMY) {
		if (ifClickedButton(bLatwy, x, y))
			return bLatwy.role;
		if (ifClickedButton(bSredni, x, y))
			return bSredni.role;
		if (ifClickedButton(bTrudny, x, y))
			return bTrudny.role;
		if (ifClickedButton(bPowrot, x, y))
			return bPowrot.role;
	}
	else if (view == CUSTOM) {
		if (ifClickedButton(bPowrot, x, y))
			return bPowrot.role;
		if (ifClickedButton(inputSzerokosc, x, y))
			return inputSzerokosc.role;
		if (ifClickedButton(inputWysokosc, x, y))
			return inputWysokosc.role;
		if (ifClickedButton(inputIloscBomb, x, y))
			return inputIloscBomb.role;
		if (ifClickedButton(bLosoweRozmieszczenieBomb, x, y))
			return bLosoweRozmieszczenieBomb.role;
		if (ifClickedButton(bManualneRozmieszczenieBomb, x, y))
			return bManualneRozmieszczenieBomb.role;
	}
	else if (view == STATYSTYKI) {
		if (ifClickedButton(bPowrot, x, y))
			return bPowrot.role;
	}
	return NONE;
}


void drawField(const Field &field, const int x, const int y, bool ifLost, bool gameEnd) {
	int currentX = x * fieldSize;
	int currentY = topPart + y * fieldSize;

	al_draw_filled_rectangle(currentX, currentY, currentX + fieldSize, currentY + fieldSize, ifLost ? red : grey);

	if (field.ifFlagged) {
		al_draw_filled_rectangle(currentX, currentY, currentX + fieldSize, currentY + fieldSize, green);
		al_draw_filled_triangle(currentX + fieldSize / 2, currentY, currentX + fieldSize, currentY + fieldSize / 4, currentX + fieldSize / 2, currentY + fieldSize / 2, red);
		al_draw_line(currentX + fieldSize / 2, currentY, currentX + fieldSize / 2, currentY + fieldSize, black, size);
	}
	else if (field.ifBomb) {
		al_draw_filled_circle(currentX + fieldSize / 2, currentY + fieldSize / 2, fieldSize / 4, black);
		al_draw_line(currentX, currentY + fieldSize / 2, currentX + fieldSize, currentY + fieldSize / 2, black, size);
		al_draw_line(currentX + fieldSize / 2, currentY, currentX + fieldSize / 2, currentY + fieldSize, black, size);
		al_draw_line(currentX + fieldSize / 4, currentY + fieldSize / 4, currentX + 3 * fieldSize / 4, currentY + 3 * fieldSize / 4, black, size);
		al_draw_line(currentX + fieldSize / 4, currentY + 3 * fieldSize / 4, currentX + 3 * fieldSize / 4, currentY + fieldSize / 4, black, size);
	}
	else if (field.nextBombAmount > 0) {
		al_draw_text(fonty, *numbersColors[field.nextBombAmount], currentX + fieldSize / 2, currentY + fieldSize / 2 - lineHeight / 2, ALLEGRO_ALIGN_CENTRE, std::to_string(field.nextBombAmount).c_str());
	}

	if (!field.ifShowed && !field.ifFlagged) {
		al_draw_filled_rectangle(currentX, currentY, currentX + fieldSize, currentY + fieldSize, orange);

	}

	if (!field.ifBomb && field.ifFlagged && gameEnd) {
		al_draw_line(currentX, currentY, currentX + fieldSize, currentY + fieldSize, red, size);
		al_draw_line(currentX, currentY + fieldSize, currentX + fieldSize, currentY, red, size);
	}

	al_draw_rectangle(currentX, currentY, currentX + fieldSize, currentY + fieldSize, black, size);
}

void drawBoard(const Game &game, const XY &lost, int gameTime) {

	al_clear_to_color(black);
	int windowWidth = game.options.Width * fieldSize;
	int windowHeight = topPart + game.options.Height * fieldSize;
	resizeIfNeed(windowWidth, windowHeight);
	al_draw_filled_rectangle(0, 0, windowWidth, topPart, grey);
	al_draw_filled_rectangle(5, 5, 60, topPart - 5, black);
	al_draw_text(fonty, red, 10, topPart / 2 - lineHeight / 2 +4, ALLEGRO_ALIGN_LEFT, std::to_string(gameTime).c_str());
	al_draw_filled_rectangle(windowWidth - 65, 5, windowWidth - 5, topPart - 5, black);
	al_draw_text(fonty, red, windowWidth - 60, topPart / 2 - lineHeight / 2 +4, ALLEGRO_ALIGN_LEFT, std::to_string(game.options.bombAmount - game.flagsAmount).c_str());
	for (int i = 0; i < game.options.Height; i++) {
		for (int j = 0; j < game.options.Width; j++) {
			drawField(game.board[j][i], j, i, j == lost.x && i == lost.y, lost != nullXY);
		}
	}
}

void drawMenu() {
	resizeIfNeed(widthM, heightM);
	al_clear_to_color(dark_grey);
	drawButton(bNowaGra);
	drawButton(bUtworzPlansze);
	drawButton(bWczytajPlansze);
	drawButton(bStatystki);
	drawButton(bWyjscie);

}

void drawLevels() {
	resizeIfNeed(widthM, heightM);
	al_clear_to_color(dark_grey);
	drawButton(bLatwy);
	drawButton(bSredni);
	drawButton(bTrudny);
	drawButton(bPowrot);
}

void drawCustom(const GameOptions &options, buttonRole marked) {
	resizeIfNeed(widthM, heightM);
	al_clear_to_color(dark_grey);
	drawButton(bSzerokosc);
	drawInput(inputSzerokosc, options.Width, inputSzerokosc.role == marked);
	drawButton(bWysokosc);
	drawInput(inputWysokosc, options.Height, inputWysokosc.role == marked);
	drawButton(bIloscBomb);
	drawInput(inputIloscBomb, options.bombAmount, inputIloscBomb.role == marked);
	drawButton(bLosoweRozmieszczenieBomb);
	drawButton(bManualneRozmieszczenieBomb);
	drawButton(bPowrot);
}


void drawWin(gameState gameState, bool ifPlayedExplosion) {
	al_draw_filled_rectangle(65, 5, 175, topPart - 5, gameState == win ? blue_win : red);
	al_draw_text(fonty, gameState == win ? grey : black, 120, topPart / 2 - lineHeight / 2 +3, ALLEGRO_ALIGN_CENTER, gameState == win ? "WYGRALES" : "PRZEGRALES");
	if (ifPlayedExplosion)
		al_play_sample(gameState == win ? victory : explosion, 1, 1, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
}


void drawInsertionField(const Field &field, const int x, const int y) {
	int currentX = x * fieldSize;
	int currentY = topPart + y * fieldSize;

	al_draw_filled_rectangle(currentX, currentY, currentX + fieldSize, currentY + fieldSize, grey);
	if (field.ifBomb) {
		al_draw_filled_circle(currentX + fieldSize / 2, currentY + fieldSize / 2, fieldSize / 4, black);
		al_draw_line(currentX, currentY + fieldSize / 2, currentX + fieldSize, currentY + fieldSize / 2, black, size);
		al_draw_line(currentX + fieldSize / 2, currentY, currentX + fieldSize / 2, currentY + fieldSize, black, size);
		al_draw_line(currentX + fieldSize / 4, currentY + fieldSize / 4, currentX + 3 * fieldSize / 4, currentY + 3 * fieldSize / 4, black, size);
		al_draw_line(currentX + fieldSize / 4, currentY + 3 * fieldSize / 4, currentX + 3 * fieldSize / 4, currentY + fieldSize / 4, black, size);
	}
	else if (field.nextBombAmount > 0) {
		al_draw_text(fonty, *numbersColors[field.nextBombAmount], currentX + fieldSize / 2, currentY + fieldSize / 2 - lineHeight / 2, ALLEGRO_ALIGN_CENTRE, std::to_string(field.nextBombAmount).c_str());
	}
	al_draw_rectangle(currentX, currentY, currentX + fieldSize, currentY + fieldSize, black, size);
}

void drawInsertionBoard(const Game &game) {
	al_clear_to_color(black);
	int windowWidth = game.options.Width * fieldSize;
	int windowHeight = topPart + game.options.Height * fieldSize;
	resizeIfNeed(windowWidth, windowHeight);
	al_draw_filled_rectangle(0, 0, windowWidth, topPart, green);
	al_draw_filled_rectangle(windowWidth - 65, 5, windowWidth - 5, topPart - 5, black);
	al_draw_text(fonty, white, 10, topPart / 2 - lineHeight / 2 +2, ALLEGRO_ALIGN_LEFT, "GOTOWE");
	al_draw_text(fonty, red, windowWidth - 60, topPart / 2 - lineHeight / 2 +2, ALLEGRO_ALIGN_LEFT, std::to_string(game.options.bombAmount).c_str());
	for (int i = 0; i < game.options.Height; i++) {
		for (int j = 0; j < game.options.Width; j++) {
			drawInsertionField(game.board[j][i], j, i);
		}
	}
}

void drawStatistics(Statistics statistics[3]) {
	resizeIfNeed(widthM, heightM);
	al_clear_to_color(dark_grey);
	int x = widthM / 35, y = heightM / 10, oY = heightM / 20, oX = widthM / 4;
	al_draw_text(fonty, black, x + oX, y, ALLEGRO_ALIGN_LEFT, "ROZEGRANE");
	al_draw_text(fonty, black, x + 2 * oX, y, ALLEGRO_ALIGN_LEFT, "WYGRANE");
	al_draw_text(fonty, black, x + 3 * oX, y, ALLEGRO_ALIGN_LEFT, "NAJLEPSZY");
	al_draw_text(fonty, black, x, y + oY, ALLEGRO_ALIGN_LEFT, "LATWY");
	al_draw_text(fonty, black, x + 1 * oX, y + oY, ALLEGRO_ALIGN_LEFT, std::to_string(statistics[0].played).c_str());
	al_draw_text(fonty, black, x + 2 * oX, y + oY, ALLEGRO_ALIGN_LEFT, std::to_string(statistics[0].winnings).c_str());
	al_draw_text(fonty, black, x + 3 * oX, y + oY, ALLEGRO_ALIGN_LEFT, std::to_string(statistics[0].best_time).c_str());
	al_draw_text(fonty, black, x, y + 2 * oY, ALLEGRO_ALIGN_LEFT, "SREDNI");
	al_draw_text(fonty, black, x + 1 * oX, y + 2 * oY, ALLEGRO_ALIGN_LEFT, std::to_string(statistics[1].played).c_str());
	al_draw_text(fonty, black, x + 2 * oX, y + 2 * oY, ALLEGRO_ALIGN_LEFT, std::to_string(statistics[1].winnings).c_str());
	al_draw_text(fonty, black, x + 3 * oX, y + 2 * oY, ALLEGRO_ALIGN_LEFT, std::to_string(statistics[1].best_time).c_str());
	al_draw_text(fonty, black, x, y + 3 * oY, ALLEGRO_ALIGN_LEFT, "TRUDNY");
	al_draw_text(fonty, black, x + 1 * oX, y + 3 * oY, ALLEGRO_ALIGN_LEFT, std::to_string(statistics[2].played).c_str());
	al_draw_text(fonty, black, x + 2 * oX, y + 3 * oY, ALLEGRO_ALIGN_LEFT, std::to_string(statistics[2].winnings).c_str());
	al_draw_text(fonty, black, x + 3 * oX, y + 3 * oY, ALLEGRO_ALIGN_LEFT, std::to_string(statistics[2].best_time).c_str());
	drawButton(bPowrot);
}


