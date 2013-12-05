#include "..\deleaker\mem2.h"
#include "..\include\lib_clara.h"
#include "..\include\dir.h"

#include "game.h"
#include "rand.h"

#define MYMAX(a,b) (a>b? a:b)
#define MYMIN(a,b) (a<b? a:b)


int CGame::Combination(int n, int k)
{
	return factorial[n] / factorial[k] / factorial[n-k];
}

//число возможных ходов на данном шаге
int CGame::NextSteps(int step)
{
	return NUMFIELDS - 2*step;
}

int CGame::GetCurrentComboNum()
{
	int ncurpos=0;
	//номер комбинации крестиков
	for(int i=nstep, nlast=0, freeitems = NUMFIELDS; i > 0 ; nlast++, freeitems--)
	{
		if( field[nlast]!= XVALUE)
		{
			ncurpos += Combination( freeitems, i ) - Combination( freeitems-1, i );
		}else
		{
			i--;
		}
	}

	//помножаем на число комбинаций ноликов
	ncurpos *= Combination( NUMFIELDS-nstep, nstep );

	//прибавляем номер комбинации ноликов
	for(int i=nstep, nlast=0, freeitems = NUMFIELDS-nstep; i > 0 ; nlast++)
	{
		if( field[nlast]!=XVALUE)
		{
			if(field[nlast]!=OVALUE)
			{
				ncurpos += Combination( freeitems, i ) - Combination( freeitems-1, i );
			}else
			{
				i--;
			}
			freeitems--;
		}
	}
	return ncurpos;
}

void CGame::FindSmartestSteps(int* steps)
{
	int d[ MYMAX(XVALUE+1, OVALUE+1) ][2]; memset(d, 0, sizeof(d) );
	int r[ MYMAX(XVALUE+1, OVALUE+1) ][SIDESIZE]; memset(r, 0, sizeof(r) );
	int c[ MYMAX(XVALUE+1, OVALUE+1) ][SIDESIZE]; memset(c, 0, sizeof(c) );

	for(int y=0; y<SIDESIZE; y++)
	{
		for(int x=0; x<SIDESIZE; x++)
		{
			r[field[y*SIDESIZE+x]][y]++;
			c[field[y*SIDESIZE+x]][x]++;
		}
		d[field[y*SIDESIZE+y]][0]++;
		d[field[y*SIDESIZE+SIDESIZE-1-y]][1]++;
	}

	for(int y=0; y<SIDESIZE; y++)
	{
		for(int x=0; x<SIDESIZE; x++)
		{
			if(field[y*SIDESIZE+x]==0)
			{
				bool win=false;
				bool loose=false;

				if(r[XVALUE][y]==SIDESIZE-1) win=true;
				if(c[XVALUE][x]==SIDESIZE-1) win=true;
				if( (y == x) && (d[XVALUE][0] == SIDESIZE-1) ) win=true;
				if( (y == SIDESIZE-1-x) && (d[XVALUE][1] == SIDESIZE-1) ) win=true;

				if(r[OVALUE][y]==SIDESIZE-1) loose=true;
				if(c[OVALUE][x]==SIDESIZE-1) loose=true;
				if( (y == x) && (d[OVALUE][0] == SIDESIZE-1) ) loose=true;
				if( (y == SIDESIZE-1-x) && (d[OVALUE][1] == SIDESIZE-1) ) loose=true;

				if(win)
					*steps += HELPWIN;
				else
					if(loose)
						*steps += HELPLOOSE;

				steps++;
			}
		}
	}
}

bool CGame::IsWin()
{
	for(int n=MYMIN(XVALUE,OVALUE); n<=MYMAX(XVALUE,OVALUE); n++)
	{
		int d1=0;
		int d2=0;
		for(int i=0; i<SIDESIZE; i++)
		{
			int r=0;
			int c=0;
			for(int j=0; j<SIDESIZE; j++)
			{
				if( field[i*SIDESIZE+j] == n )
					r++;
				if( field[i+j*SIDESIZE] == n )
					c++;
			}
			if( r==SIDESIZE || c==SIDESIZE )
				return true;

			if( field[i*SIDESIZE + i] == n)
				d1++;
			if( field[i*SIDESIZE + SIDESIZE-1-i] == n)
				d2++;
		}
		if( d1==SIDESIZE || d2==SIDESIZE )
			return true;
	}

	return false;
}

void CGame::UpdateStats(int delta)
{
	/*
	if(delta>=0)
	{
	*/

	for(int i=0; i<=nstep; i++)
	{
		stepsTable[ stepStack[i] ] += delta;
	}
	/*
}else
	{
	for(int i=nstep;
	(i>=0) && ((stepsTable[ stepStack[i] ] += delta)<=0);
	i--)
	{
}
}
	*/
}

CGame::CGame()
{
	memset(field, 0 ,sizeof(field)); //EMPTYFIELD

	nstep = 0;//current step number;

	gamestate = GAMEINPROGRESS;

	//таблица факториалов
	factorial[0]=1;
	for(int i=1; i<=NUMFIELDS; i++)
		factorial[i]=factorial[i-1]*i;

	stepsTableSize=0;
	for(int i=0; i < NUMFIELDS/2; i++)
	{
		stepComboPositions[i] = stepsTableSize;

		int xcombos = Combination( NUMFIELDS, i );
		int ocombos = Combination( NUMFIELDS-i, i );

		stepsTableSize += xcombos * ocombos * NextSteps(i);
	}

	stepsTable = new int [stepsTableSize];
	for(int i=0, pos=0; i < NUMFIELDS/2; i++)
	{
		int xcombos = Combination( NUMFIELDS, i );
		int ocombos = Combination( NUMFIELDS-i, i );

		for(int j=0; j < xcombos * ocombos * NextSteps(i); j++)
		{
			stepsTable[ pos++ ] = NUMFIELDS/2 - i;
		}
	}

	scorePC = 0;
	scoreHuman = 0;
	scoreTies = 0;

	//        for(int i=0; i<stepsTableSize; i++)
	//                stepsTable[i] = INITWEIGHT;
}

CGame::~CGame()
{
	delete[] stepsTable;
}


int CGame::GetPCScore()
{
	return scorePC;
}

int CGame::GetHumanScore()
{
	return scoreHuman;
}

int CGame::GetTies()
{
	return scoreTies;
}

const char* CGame::GetField()
{
	return field;
}

int CGame::DoPCStep()
{
	int retcode = -1;
	if( nstep > NUMFIELDS/2 || GetGameState()!=GAMEINPROGRESS )
		return retcode;

	int ncurpos = GetCurrentComboNum();//detect number of current combo

	int nextfreetouse = 0;

	if( nstep < NUMFIELDS/2 )
	{
		// подсказка
		int* tmpsteps = new int[NextSteps(nstep)];
		memcpy( tmpsteps, &stepsTable[ stepComboPositions[nstep] + ncurpos * NextSteps(nstep) ], NextSteps(nstep)*4 );

		FindSmartestSteps( tmpsteps );

		int rndmax = 0;

		//cout << "steps";
		for(int i=0; i<NextSteps(nstep); i++)
		{
			rndmax += tmpsteps[ i ];
			//cout << " " << (int)vnextsteps[ i + stepcombopositions[nstep] + ncurpos * nextsteps(nstep) ];
		}
		//cout << endl;

		int rnd = rand() % rndmax;

		while( rnd >= tmpsteps[ nextfreetouse ])
		{
			rnd -= tmpsteps[ nextfreetouse++ ];
		}
		//nextfreetouse = st1[nstep];

		//cout << "selected free item " <<nextfreetouse<<endl;

		stepStack[ nstep ] = nextfreetouse + stepComboPositions[nstep] + ncurpos * NextSteps(nstep); //запоминаем ход

		delete[] tmpsteps;
	}


	for(int i=0; i<NUMFIELDS; i++ )
	{
		if( 0 == field[i] )
			if( 0 == nextfreetouse-- )
			{
				//cout << "selected real item " << i <<endl;
				field[i] = XVALUE;
				retcode = i;
				break;
			}
	}

	if(IsWin())
	{
		gamestate = PCWIN;
		scorePC++;
		UpdateStats(AWARD);
	}else
	{
		if( nstep == NUMFIELDS/2 )
		{
			gamestate = TIE;
			scoreTies++;
			UpdateStats(BACKING);
		}
	}

	return retcode;
}


CGame::GAMESTATE CGame::GetGameState()
{
	return gamestate;
}

int CGame::DoHumanStep(int pos)
{
	int retcode=-1;
	if( nstep > NUMFIELDS/2 || GetGameState()!=GAMEINPROGRESS )
		return retcode;

	if( field[ pos ]!= EMPTYFIELD )
		return retcode;

	field[ pos ] = OVALUE;

	if(IsWin())
	{
		gamestate = HUMANWIN;
		UpdateStats(PENALTY);
		scoreHuman++;
	}

	nstep++;

	return retcode;
}

int CGame::ReadStats( const wchar_t* fileName )
{
	w_chdir(GetDir(DIR_ELFS_CONFIG|MEM_INTERNAL));

    int f = w_fopen(fileName, WA_Read, 0x1FF, 0);
	if( f >= 0 )
	{
		w_fread( f, &scorePC, sizeof(scorePC) );
		w_fread( f, &scoreHuman, sizeof(scoreHuman)  );
		w_fread( f, &scoreTies, sizeof(scoreTies) );
		w_fread( f, stepsTable, sizeof(stepsTable[0]) * stepsTableSize  );
		w_fclose(f);
	}
	return 0;
}

int CGame::WriteStats( const wchar_t* fileName )
{
	w_chdir(GetDir(DIR_ELFS_CONFIG|MEM_INTERNAL));

	int f= w_fopen( fileName, WA_Write+WA_Create+WA_Truncate, 0x1FF, 0);
	w_fwrite( f, &scorePC, sizeof(scorePC) );
	w_fwrite( f, &scoreHuman, sizeof(scoreHuman) );
	w_fwrite( f, &scoreTies, sizeof(scoreTies) );
	w_fwrite( f, stepsTable, sizeof(stepsTable[0]) * stepsTableSize );
	w_fclose(f);
	return 0;
}
