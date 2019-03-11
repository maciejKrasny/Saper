#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "Draw.h"
#include "rules.h"

const float FPS = 60;
view openView = MENU;
bool ifShowMessage = false;
std::string messageText = "";
Statistics statistics[3];
#define statisticsFile "statystyki.txt"
#define boardFile "plansza.txt"
int whichLevel = 0;

void showMessage(std::string text) {
	ifShowMessage = true;
	messageText = text;
}

void loadStatistics() {
	FILE * file = fopen(statisticsFile, "rt");
	if (file == NULL) {
		statistics[0] = { 0, 0, 32767 };
		statistics[1] = { 0, 0, 32767 };
		statistics[2] = { 0, 0, 32767 };
	}
	else {
		int played, winnings, best_time;
		fscanf(file, "%d %d %d\n", &played, &winnings, &best_time);
		statistics[0] = { played, winnings, best_time };
		fscanf(file, "%d %d %d\n", &played, &winnings, &best_time);
		statistics[1] = { played, winnings, best_time };
		fscanf(file, "%d %d %d\n", &played, &winnings, &best_time);
		statistics[2] = { played, winnings, best_time };
		fclose(file);
	}
}

void saveStatistics() {
	FILE * file = fopen(statisticsFile, "w+t");
	if (file != NULL) {
		fprintf(file, "%d %d %d\n", statistics[0].played, statistics[0].winnings, statistics[0].best_time);
		fprintf(file, "%d %d %d\n", statistics[1].played, statistics[1].winnings, statistics[1].best_time);
		fprintf(file, "%d %d %d\n", statistics[2].played, statistics[2].winnings, statistics[2].best_time);
		fclose(file);
	}
}

void saveBoard(const Game &game) {
	FILE * file = fopen(boardFile, "w+t");
	if (file != NULL) {
		fprintf(file, "%d %d %d\n", game.options.Height, game.options.Width, game.options.bombAmount);
		for (int i = 0; i < game.options.Width; i++) {
			for (int j = 0; j < game.options.Height; j++) {
				if (game.board[i][j].ifBomb) {
					fprintf(file, "%d %d\n", i, j);
				}
			}
		}
		fclose(file);
	}
}

bool loadBoard(Game &game) {
	FILE * file = fopen(boardFile, "rt");
	if (file == NULL) {
		return false;
	}
	else {
		int height, width, bombAmount;
		fscanf(file, "%d %d %d\n", &height, &width, &bombAmount);
		GameOptions options = { bombAmount, height, width };
		creatEmptyGame(game, options);
		int x, y;
		for (int i = 0; i < bombAmount; i++) {
			fscanf(file, "%d %d\n", &x, &y);
			game.board[x][y].ifBomb = true;
		}
		allocateNumbers(game);
		return true;
	}
}

void eventQueue(ALLEGRO_EVENT_QUEUE * eventyQueue) {
	ALLEGRO_EVENT event;
	bool redraw = false;
	bool ifPlayExplosion = true;
	Game game;
	XY clickPosition = nullXY;
	buttonRole lastClickedButton = NONE;
	XY lostPlace = nullXY;
	gameState gameState = during;
	time_t start, end;
	GameOptions createdOptions = { 0, 0, 0 };
	while (true)
	{
		al_wait_for_event(eventyQueue, &event);
		if (event.type == ALLEGRO_EVENT_TIMER)
			redraw = true;
		else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			break;
		else if (event.type == ALLEGRO_EVENT_KEY_CHAR) {
			if (openView == CUSTOM) {
				if (event.keyboard.unichar >= '0' && event.keyboard.unichar <= '9') {
					int number = event.keyboard.unichar - '0';
					if (lastClickedButton == INPUT_SZEROKOSC) {
						createdOptions.Width = createdOptions.Width * 10 + number;
					}
					else if (lastClickedButton == INPUT_WYSOKOSC) {
						createdOptions.Height = createdOptions.Height * 10 + number;
					}
					else if (lastClickedButton == INPUT_ILOSC_BOMB) {
						createdOptions.bombAmount = createdOptions.bombAmount * 10 + number;
					}
				}
				else if (event.keyboard.unichar == 8) {
					if (lastClickedButton == INPUT_SZEROKOSC) {
						createdOptions.Width /= 10;
					}
					else if (lastClickedButton == INPUT_WYSOKOSC) {
						createdOptions.Height /= 10;
					}
					else if (lastClickedButton == INPUT_ILOSC_BOMB) {
						createdOptions.bombAmount /= 10;
					}
				}
			}
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			ifShowMessage = false;
			lastClickedButton = whichButtonWasClicked(event.mouse.x, event.mouse.y, openView);
			if (openView == MENU) {
				if (lastClickedButton == NOWAGRA)
					openView = POZIOMY;
				else if (lastClickedButton == UTWORZPLANSZE)
					openView = CUSTOM;
				else if (lastClickedButton == WCZYTAJPLANSZE) {
					if (loadBoard(game)) {
						openView = GRA;
						ifPlayExplosion = true;
						lostPlace = nullXY;
						gameState = during;
						whichLevel = 3;
						time(&start);
					}
					else
						showMessage("Brak zapisanej gry");
				}
				else if (lastClickedButton == B_STATYSTYKI)
					openView = STATYSTYKI;
				else if (lastClickedButton == WYJSCIE)
					break;
			}
			else if (openView == POZIOMY) {
				if (lastClickedButton == LATWY) {
					GameOptions options = { 10 ,9 , 9 };
					openView = GRA;
					creatGame(game, options);
					ifPlayExplosion = true;
					lostPlace = nullXY;
					gameState = during;
					whichLevel = 0;
					time(&start);
				}
				else if (lastClickedButton == SREDNI) {
					GameOptions opcje = { 40 ,16 , 16 };
					openView = GRA;
					creatGame(game, opcje);
					ifPlayExplosion = true;
					lostPlace = nullXY;
					gameState = during;
					whichLevel = 1;
					time(&start);
				}
				else if (lastClickedButton == TRUDNY) {
					GameOptions opcje = { 99 ,16 , 30 };
					openView = GRA;
					creatGame(game, opcje);
					ifPlayExplosion = true;
					lostPlace = nullXY;
					gameState = during;
					whichLevel = 2;
					time(&start);
				}
				else if (lastClickedButton == POWROT) {
					openView = MENU;
				}
			}
			else if (openView == CUSTOM) {
				if (lastClickedButton == POWROT) {
					openView = MENU;
				}
				else if (lastClickedButton == LOSOWE_BOMBY) {
					if (changeOnRange(createdOptions.Height, 9, 24) | changeOnRange(createdOptions.Width, 9, 30) | changeOnRange(createdOptions.bombAmount, 10, createdOptions.Width*createdOptions.Height) | changeOnRange(createdOptions.bombAmount, 10, 668)) {
						showMessage("Podane wielkosci wykraczaja poza dozwolony zakres, wartosci zostaly ustawione na domyslne.");
					}
					else {
						openView = GRA;
						creatGame(game, createdOptions);
						ifPlayExplosion = true;
						lostPlace = nullXY;
						gameState = during;
						whichLevel = 3;
						time(&start);
					}
				}
				else if (lastClickedButton == MANUALNE_BOMBY) {
					if (changeOnRange(createdOptions.Height, 9, 24) | changeOnRange(createdOptions.Width, 9, 30)) {
						showMessage("Podane wielkosci wykraczaja poza dozwolony zakres, wartosci zostaly ustawione na domyslne.");
					}
					else {
						openView = ROZMIESZCZANIE;
						createdOptions.bombAmount = 0;
						creatEmptyGame(game, createdOptions);
					}

				}
			}
			else if (openView == ROZMIESZCZANIE) {
				coordinatesFieldFromMouse(clickPosition, event.mouse.x, event.mouse.y);
				if (clickPosition == nullXY) {
					saveBoard(game);
					openView = MENU;
					showMessage("ZAPISANO");
				}
				else {
					game.board[clickPosition.x][clickPosition.y].ifBomb = !game.board[clickPosition.x][clickPosition.y].ifBomb;
					game.options.bombAmount += game.board[clickPosition.x][clickPosition.y].ifBomb ? 1 : -1;
				}
			}
			else if (openView == GRA) {
				coordinatesFieldFromMouse(clickPosition, event.mouse.x, event.mouse.y);
				if (clickPosition != nullXY)
					if (event.mouse.button == 1) {
						if (lostPlace == nullXY && gameState == during) {
							if (!game.board[clickPosition.x][clickPosition.y].ifFlagged) {
								if (game.board[clickPosition.x][clickPosition.y].ifBomb) { // przegrana
									showWholeBoard(game);
									lostPlace = clickPosition;
									openView = KONIEC_GRY;
									gameState = lost;
									if (whichLevel < 3) {
										statistics[whichLevel].played++;
									}
								}
								else {
									showReq(game, clickPosition.x, clickPosition.y);
								}
							}
							if (game.options.bombAmount == (game.options.Height * game.options.Width - game.shownAmount)) { // wygrana
								gameState = win;
								openView = KONIEC_GRY;
								if (whichLevel < 3) {
									statistics[whichLevel].played++;
									statistics[whichLevel].winnings++;
									if (difftime(end, start) < statistics[whichLevel].best_time)
										statistics[whichLevel].best_time = difftime(end, start);
								}
							}
						}
					}
					else {
						if (!game.board[clickPosition.x][clickPosition.y].ifShowed) {
							game.board[clickPosition.x][clickPosition.y].ifFlagged = !game.board[clickPosition.x][clickPosition.y].ifFlagged;
							game.flagsAmount += game.board[clickPosition.x][clickPosition.y].ifFlagged ? 1 : -1;
						}

					}
			}
			else if (openView == STATYSTYKI) {
				if (lastClickedButton == POWROT)
					openView = MENU;
			}
			else if (openView == KONIEC_GRY) {
				openView = MENU;
			}
		}
		if (redraw && al_event_queue_is_empty(eventyQueue)) {
			if (openView == MENU)
				drawMenu();
			else if (openView == POZIOMY)
				drawLevels();
			else if (openView == CUSTOM)
				drawCustom(createdOptions, lastClickedButton);
			else if (openView == GRA)
				drawBoard(game, lostPlace, lostPlace == nullXY ? difftime(time(&end), start) : difftime(end, start));
			else if (openView == KONIEC_GRY) {
				drawBoard(game, lostPlace, difftime(end, start));
				drawWin(gameState, ifPlayExplosion);
				ifPlayExplosion = false;
			}
			else if (openView == STATYSTYKI) {
				drawStatistics(statistics);
			}
			else if (openView == ROZMIESZCZANIE) {
				drawInsertionBoard(game);
			}
			if (ifShowMessage)
				drawMessage(messageText);
			al_flip_display();
			redraw = false;
		}
	}
}


int main() {
	srand(time(NULL));
	al_init();
	al_install_mouse();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_install_audio();
	al_init_acodec_addon();
	al_install_keyboard();

	al_reserve_samples(2);
	ALLEGRO_SAMPLE * victory = al_load_sample("fanfare.ogg");
	ALLEGRO_SAMPLE * explosion = al_load_sample("wy_buszek.ogg");
	ALLEGRO_DISPLAY * windowy = al_create_display(640, 480);
	ALLEGRO_EVENT_QUEUE * eventyQueue = al_create_event_queue();
	ALLEGRO_TIMER * timer = al_create_timer(1.0 / FPS);

	initDraw(windowy, explosion, victory);
	al_register_event_source(eventyQueue, al_get_display_event_source(windowy));
	al_register_event_source(eventyQueue, al_get_timer_event_source(timer));
	al_register_event_source(eventyQueue, al_get_mouse_event_source());
	al_register_event_source(eventyQueue, al_get_keyboard_event_source());
	al_start_timer(timer);
	loadStatistics();
	eventQueue(eventyQueue);

	saveStatistics();

	al_destroy_sample(victory);
	al_destroy_sample(explosion);
	al_destroy_timer(timer);
	al_destroy_display(windowy);
	al_destroy_event_queue(eventyQueue);
	return 0;
}