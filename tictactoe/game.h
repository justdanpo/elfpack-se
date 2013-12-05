#ifndef GAME_H
#define GAME_H

#define PENALTY -1
#define BACKING 1
#define AWARD 4

#define HELPWIN 10
#define HELPLOOSE 8 //2 балла *5 вариантов либо 1 балл *3 варианта

#define INITWEIGHT 1

#define SIDESIZE 3
#define NUMFIELDS (SIDESIZE*SIDESIZE)

class CGame
{
public:
	enum GAMESTATE
	{
		GAMEINPROGRESS,
		TIE,
		HUMANWIN,
		PCWIN
	};

	enum {
		EMPTYFIELD = 0,
		XVALUE = 1,
		OVALUE = 2
	};

private:
	char field[NUMFIELDS];
	int factorial[NUMFIELDS+1];
	int stepStack[NUMFIELDS/2];
	int stepComboPositions[NUMFIELDS/2];
	int* stepsTable;
	int stepsTableSize;
	int scorePC;
	int scoreHuman;
	int scoreTies;
	int nstep;

	GAMESTATE gamestate;

	int Combination(int n, int k); //сочетание из n по k
	int NextSteps(int step); //число возможных ходов на данном шаге
	int GetCurrentComboNum(); //получает номер из стостояния поля

	//если компьютер или человек одним ходом могут выиграть, этим ходам придаётся дополнительный вес
	void FindSmartestSteps(int* steps);
	bool IsWin(); //кто-то победил?
	void UpdateStats(int delta); //обновление весов по итогам игры

public:
	CGame();
	~CGame();

	int GetPCScore();
	int GetHumanScore();
	int GetTies();
	GAMESTATE GetGameState();
	const char* GetField();
	int DoPCStep();
	int DoHumanStep(int pos);
	int ReadStats(const wchar_t* fileName);
	int WriteStats(const wchar_t* fileName);
};


#endif
