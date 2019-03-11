#include "rules.h"
#include <string>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>


#ifndef DRAW
#define DRAW

#define fieldSize 30
#define size 3
#define topPart 30
#define widthM 480
#define heightM 640
#define heightB 60

enum view
{
	MENU, GRA, POZIOMY, KONIEC_GRY, CUSTOM, STATYSTYKI, ROZMIESZCZANIE
};

enum buttonRole
{
	NONE, NOWAGRA, UTWORZPLANSZE, WCZYTAJPLANSZE, B_STATYSTYKI, WYJSCIE, LATWY, SREDNI, TRUDNY, POWROT, INPUT_SZEROKOSC, INPUT_WYSOKOSC, INPUT_ILOSC_BOMB, STWORZ_GRE, LOSOWE_BOMBY, MANUALNE_BOMBY
};

void initDraw(ALLEGRO_DISPLAY * windowy, ALLEGRO_SAMPLE * sound1, ALLEGRO_SAMPLE * sound2);
void drawBoard(const Game &game, const XY &lost, int gameTime);
void coordinatesFieldFromMouse(XY &position, int x, int y);
void drawMenu();
buttonRole whichButtonWasClicked(int x, int y, view view);
void drawWin(gameState gameState, bool ifPlayedExplosion);
void drawLevels();
void drawCustom(const GameOptions &options, buttonRole marked);
void drawMessage(std::string text);
void drawStatistics(Statistics statistics[3]);
void drawInsertionBoard(const Game &game);

struct Button {
	XY lg;
	XY pd;
	std::string text;
	buttonRole role;
};

const Button bNowaGra = { { widthM / 5, widthM / 5 },{ 4 * widthM / 5, widthM / 5 + heightB }, "NOWA  GRA", NOWAGRA };
const Button bUtworzPlansze = { { bNowaGra.lg.x, bNowaGra.pd.y + 13 },{ bNowaGra.pd.x, bNowaGra.pd.y + heightB + 13 }, "UTWORZ  PLANSZE", UTWORZPLANSZE };
const Button bWczytajPlansze = { { bUtworzPlansze.lg.x, bUtworzPlansze.pd.y + 13 },{ bUtworzPlansze.pd.x, bUtworzPlansze.pd.y + heightB + 13 }, "WCZYTAJ  UTWORZONA  PLANSZE", WCZYTAJPLANSZE };
const Button bStatystki = { { bWczytajPlansze.lg.x, bWczytajPlansze.pd.y + 13 },{ bWczytajPlansze.pd.x, bWczytajPlansze.pd.y + heightB + 13 }, "STATYSTYKI", B_STATYSTYKI };
const Button bWyjscie = { { bStatystki.lg.x, bStatystki.pd.y + 13 },{ bStatystki.pd.x, bStatystki.pd.y + heightB + 13 }, "WYJSCIE", WYJSCIE };

const Button bLatwy = { { widthM / 5, widthM / 5 },{ 4 * widthM / 5, widthM / 5 + heightB }, "LATWY", LATWY };
const Button bSredni = { { bLatwy.lg.x, bLatwy.pd.y + 13 },{ bLatwy.pd.x, bLatwy.pd.y + heightB + 13 }, "SREDNI", SREDNI };
const Button bTrudny = { { bSredni.lg.x, bSredni.pd.y + 13 },{ bSredni.pd.x, bSredni.pd.y + heightB + 13 }, "TRUDNY", TRUDNY };

const Button bSzerokosc = { { widthM / 5, widthM / 10 },{ 4 * widthM / 5, widthM / 10 + heightB }, "SZEROKOSC  (9-30)" };
const Button inputSzerokosc = { { bSzerokosc.lg.x, bSzerokosc.pd.y },{ bSzerokosc.pd.x, bSzerokosc.pd.y + heightB }, "", INPUT_SZEROKOSC };
const Button bWysokosc = { { inputSzerokosc.lg.x, inputSzerokosc.pd.y + 13 },{ inputSzerokosc.pd.x, inputSzerokosc.pd.y + heightB + 13 }, "WYSOKOSC  (9-24)" };
const Button inputWysokosc = { { bWysokosc.lg.x, bWysokosc.pd.y },{ bWysokosc.pd.x, bWysokosc.pd.y + heightB }, "", INPUT_WYSOKOSC };
const Button bIloscBomb = { { inputWysokosc.lg.x, inputWysokosc.pd.y + 13 },{ inputWysokosc.pd.x, inputWysokosc.pd.y + heightB + 13 }, "ILOSC  BOMB  (10-668)" };
const Button inputIloscBomb = { { bIloscBomb.lg.x, bIloscBomb.pd.y },{ bIloscBomb.pd.x, bIloscBomb.pd.y + heightB }, "", INPUT_ILOSC_BOMB };
const Button bLosoweRozmieszczenieBomb = { { inputIloscBomb.lg.x, inputIloscBomb.pd.y + 13 },{ widthM / 2 - 5, (inputIloscBomb.pd.y + heightB + 13) }, "LOSOWO", LOSOWE_BOMBY };
const Button bManualneRozmieszczenieBomb = { { widthM / 2 + 5, inputIloscBomb.pd.y + 13 },{ inputIloscBomb.pd.x, inputIloscBomb.pd.y + heightB + 13 }, "MANUALNIE", MANUALNE_BOMBY };
const Button bPowrot = { { bLosoweRozmieszczenieBomb.lg.x, bLosoweRozmieszczenieBomb.pd.y + 13 },{ bManualneRozmieszczenieBomb.pd.x, bManualneRozmieszczenieBomb.pd.y + heightB + 13 }, "POWROT", POWROT };

#endif
