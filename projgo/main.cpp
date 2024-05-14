#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <cstdio>
#include"conio2.h"
#include"math.h"
#include <windows.h>

/* Comment: in the final program declare appropriate constants, e.g.,
   to eliminate from your program numerical values by replacing them
   with well defined identifiers */
#define LROW 5
#define LKOLUMN 5

#define BROW 5
#define BKOLUMN 50
#define BSIZE 9

#define ARROWUP 0x48
#define ARROWDOWN 0x50
#define ARROWRIGHT 0x4d
#define ARROWLEFT 0x4b
#define N 0x6e
#define I 0x69
#define S 0x73
#define ENTER 0x0d
#define ESC 0x1b
#define M 0x6d
#define L 0x6c

#define KBIALE 2
#define KCZARNE 1
#define PPOLE 0



void instruction(int *punktyc,int *punktyb,int *wruch) {
	textcolor(WHITE);
	gotoxy(LKOLUMN, LROW);
	cputs("GO");
	gotoxy(LKOLUMN, LROW + 1);
	cputs("Jakub Grunwald 193346");
	gotoxy(LKOLUMN, LROW + 3);
	cputs("arows   = movement");
	gotoxy(LKOLUMN, LROW + 4);
	cputs("q       = exit");
	gotoxy(LKOLUMN,LROW + 5);
	cputs("n       = new game");
	gotoxy(LKOLUMN, LROW + 6);
	cputs("enter   = confirm move");
	gotoxy(LKOLUMN, LROW + 7);
	cputs("esc     = undo");
	gotoxy(LKOLUMN, LROW + 8);
	cputs("i       = place a rock");
	gotoxy(LKOLUMN, LROW + 9);
	if (*wruch==0)
	cputs("m       = handicap");
	char buffer[40];
	gotoxy(LKOLUMN, LROW + 11);
	sprintf(buffer, "Punkty CZARNE=%d BIALE=%d", *punktyc, *punktyb);
	cputs(buffer);
	textcolor(LIGHTBLUE);
}

int settingsizeofboard(int *wsizepocz)
{
	int znak;
	gotoxy(LKOLUMN, LROW);
	cputs("SET SIZE OF BOARD");
	gotoxy(LKOLUMN, LROW + 1);
	cputs("1  = 9X9");
	gotoxy(LKOLUMN, LROW + 2);
	cputs("2  = 13X13");
	gotoxy(LKOLUMN, LROW + 3);
	cputs("3  = 19X19");
	gotoxy(LKOLUMN, LROW + 4);
	cputs("4  = OTHER");
	gotoxy(LKOLUMN, LROW + 5);
	znak = getch();
	if (znak == '1')
	{
		return *wsizepocz = 9;
	}
	else if (znak == '2')
	{
		return*wsizepocz = 13;
	}
	else if (znak == '3')
	{
		return *wsizepocz = 19;
	}
	else if (znak == '4')
	{
		textbackground(BLACK);
		gotoxy(LKOLUMN, LROW + 5);
		cputs("ENTER BOARD SIZE:");
		_setcursortype(_NORMALCURSOR);

		CONSOLE_SCREEN_BUFFER_INFO info;
		int width,height;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
		width = info.srWindow.Right - info.srWindow.Left + 1;
		height = info.srWindow.Bottom - info.srWindow.Top + 1;

		int flag = 0;
		int buffer[10] = { 0 };
		int i = 0;
		do {
			znak = getche();
			buffer[i] = znak - 48;
			i++;
			flag++;

		} while (znak != ENTER);
		int sizeb = 0;
		for (int j = 0; j < 10; j++)
		{
		
				sizeb += buffer[j] * int(pow(10, flag - 2));
				flag--;
			
		}
		clrscr();
		_setcursortype(_NOCURSOR);
		if (BKOLUMN>LKOLUMN+28)
		{
			if(sizeb> width - BKOLUMN)
				sizeb = width - BKOLUMN;
				if (sizeb > height)
			{
				sizeb = height - BROW - 2;
			}
		}
		else if (BKOLUMN < LKOLUMN)
		{
			if (sizeb > LKOLUMN)
				sizeb = LKOLUMN - 3-BKOLUMN;
				if (sizeb > height)
			{
				sizeb = height - BROW - 2;
			}
		}
		else if (sizeb>height)
		{
			sizeb = height - BROW - 2;
		}
		return *wsizepocz = sizeb;
	}
	else
		return *wsizepocz = 9;
}

void savefile(int **plansza1,int **plansza2,int *wruch,int *punktyb,int* punktyc,int sizepocz)
{
	char znak;
	textbackground(BLACK);
	gotoxy(LKOLUMN, LROW + 22);
	cputs("ENTER FILE NAME:");
	_setcursortype(_NORMALCURSOR);

	char filename[20] = {0};
	int i = 0;
	do {
		znak = getche();
		if (znak != ENTER)
		{
			filename[i] = (char)znak;
			i++;
		}
	} while (znak != ENTER);

	FILE* plik;

	plik = fopen(filename, "w");
	if (plik == NULL) {
		char buffer[40];
		gotoxy(LKOLUMN, LROW + 13);
		sprintf(buffer, "Error");
		cputs(buffer);
	}
	for (int i = 0; i < sizepocz; i++)
	{
		for (int j = 0; j < sizepocz; j++)
		{
			fwrite(&plansza1[i][j], sizeof(plansza1), 1, plik);
		}
	}
	for (int i = 0; i < sizepocz; i++)
	{
		for (int j = 0; j < sizepocz; j++)
		{
			fwrite(&plansza2[i][j], sizeof(plansza1), 1, plik);
		}
	}
	fwrite(wruch, sizeof(int), 1, plik);
	fwrite(punktyb, sizeof(int), 1, plik);
	fwrite(punktyc, sizeof(int), 1, plik);
	fclose(plik);
	_setcursortype(_NOCURSOR);
}

void readfile(int** plansza1, int** plansza2, int *wruch, int *punktyb, int *punktyc, int sizepocz)
{
	char znak;
	textbackground(BLACK);
	gotoxy(LKOLUMN, LROW + 22);
	cputs("ENTER FILE NAME:");
	_setcursortype(_NORMALCURSOR);

	char filename[20] = { 0 };
	int i = 0;
	do {
		znak = getche();
		if (znak != ENTER)
		{
			filename[i] = (char)znak;
			i++;
		}
	} while (znak != ENTER);
	FILE* plik;

	plik = fopen(filename, "r");
	if (plik == NULL) {
		char buffer[40];
		gotoxy(LKOLUMN, LROW + 13);
		sprintf(buffer, "Wrong file name try again");
		cputs(buffer);
		znak = getch();
		_setcursortype(_NOCURSOR);
		return;
	}
	for (int i = 0; i < sizepocz; i++)
	{
		for (int j = 0; j < sizepocz; j++)
		{
			fread(&plansza1[i][j], sizeof(plansza1), 1, plik);
		}
	}
	for (int i = 0; i < sizepocz; i++)
	{
		for (int j = 0; j < sizepocz; j++)
		{
			fread(&plansza2[i][j], sizeof(plansza1), 1, plik);
		}
	}
	fread(wruch, sizeof(int), 1, plik);
	fread(punktyb, sizeof(int), 1, plik);
	fread(punktyc, sizeof(int), 1, plik);
	fclose(plik);
	_setcursortype(_NOCURSOR);
}

void board(int size, int posx, int posy, int **plansza)
{
	for (int i = 0; i < size; i++)
	{
		gotoxy(posx, posy);
		clreol();
		textbackground(YELLOW);
		if ((i == 0) || (i == size - 1))
		{
			for (int k = 0; k < size; k++)
			{
				textcolor(4);
				putch('%');

			}
			gotoxy(posx, posy + 1);
		}
		else
		{
			for (int j = 0; j < size; j++)
			{
				if (j == 0 || j == size - 1)
				{
					textbackground(YELLOW);
					putch('|');
				}
				else
				{	
					if (plansza[j-1][i-1] == 1)
					{
						clreol();
						textbackground(BLACK);
						textcolor(BLACK);
						putch('X');
						textbackground(YELLOW);
						textcolor(4);
					}
					else if (plansza[j-1][i-1] == 2)
					{
						clreol();
						textbackground(WHITE);
						textcolor(WHITE);
						putch('X');
						textbackground(YELLOW);
						textcolor(4);
					}
					else 
					{
						textbackground(YELLOW);
						putch('+');
					}	
				}
				textbackground(BLACK);
			}
		}
		posy = posy + 1;
		textbackground(BLACK);
	}
	textcolor(LIGHTBLUE);
}

void selfkill(int posx, int posy, int **plansza,int kolor,int opositekolor,int size)
{
	if (plansza[posx][posy] == kolor)
	{
		if (posx == 0 && posy == 0)
		{
			if ((plansza[posx][posy + 1] == opositekolor) && (plansza[posx + 1][posy] == opositekolor))
			{
				plansza[posx][posy] = PPOLE;
			}
		}
		else if (posx == size - 1 && posy == 0)
		{
			if ((plansza[posx - 1][posy] == opositekolor) && (plansza[posx][posy+ 1] == opositekolor))
			{
				plansza[posx][posy] = PPOLE;
			}
		}
		else if (posx == 0 && posy == size - 1)
		{
			if ((plansza[posx][posy - 1] == opositekolor) && (plansza[posx + 1][posy] == opositekolor))
			{
				plansza[posx][posy] = PPOLE;	
			}
		}
		else if (posx == size - 1 && posy == size - 1)
		{
			if ((plansza[posx - 1][posy] == opositekolor) && (plansza[posx][posy - 1] == opositekolor))
			{
				plansza[posx][posy] = PPOLE;
			}
		}

		else if (posx == 0)
		{
			if ((plansza[posx][posy + 1] == opositekolor) && (plansza[posx + 1][posy] == opositekolor) && (plansza[posx][posy - 1] == opositekolor))
			{
				plansza[posx][posy] = PPOLE;
			}
		}
		else if (posx == size - 1)
		{
			if ((plansza[posx][posy - 1] == opositekolor) && (plansza[posx - 1][posy] == opositekolor) && (plansza[posx][posy + 1] == opositekolor))
			{
				plansza[posx][posy] = PPOLE;
			}
		}
		else if (posy == 0)
		{
			if ((plansza[posx + 1][posy] == opositekolor) && (plansza[posx][posy + 1] == opositekolor) && (plansza[posx - 1][posy] == opositekolor))
			{
				plansza[posx][posy] = PPOLE;
			}
		}
		else if (posy == size - 1)
		{
			if ((plansza[posx + 1][posy] == opositekolor) && (plansza[posx][posy - 1] == opositekolor) && (plansza[posx - 1][posy] == opositekolor))
			{
				plansza[posx][posy] = PPOLE;	
			}
		}
		else if ((plansza[posx - 1][posy] == PPOLE) || (plansza[posx + 1][posy] == PPOLE) || (plansza[posx][posy - 1] == PPOLE) || (plansza[posx][posy + 1] == PPOLE))
		{
			plansza[posx][posy] = kolor;
		}
		else if ((plansza[posx - 1][posy] == opositekolor) && (plansza[posx + 1][posy] == opositekolor) && (plansza[posx][posy - 1] == opositekolor) && (plansza[posx][posy + 1] == opositekolor))
		{
			plansza[posx][posy] = PPOLE;
		}
	}
}
	
void clear(int **plansza,int *size,int *wruch,int *punktyb,int *punktyc,int **plansza2)
{
	for (int i = 0; i < *size; i++)
	{
		for (int j = 0; j < *size; j++)
		{
			plansza[i][j] = 0;
		}
	}
	for (int i = 0; i < *size; i++)
	{
		for (int j = 0; j < *size; j++)
		{
			plansza2[i][j] = 0;
		}
	}
	*wruch = 0;
	*punktyb = 0;
	*punktyc = 0;
}

void position(int x, int y)
{
	textcolor(WHITE);
	char buffer[40];
	gotoxy(LKOLUMN, LROW + 12);
	sprintf(buffer, "Current postition is: x=%d y=%d",x,y );
	cputs(buffer);
	gotoxy(x, y);
	textcolor(LIGHTBLUE);
}

int points(int kolor, int* punktyb, int* punktyc)
{
	if (kolor == KCZARNE)
	{
		(*punktyc)++;
		return *punktyc;
	}
	else 
	{
		(*punktyb)++;
		return *punktyb;
	}
}

void capturing(int **plansza,int size, int kolor ,int opositekolor,int* punktyb, int* punktyc)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{	
			if (plansza[i][j] == kolor)
			{
				if (i == 0 && j == 0)
				{
					if( (plansza[i][j+1]==opositekolor)&& (plansza[i+1][j]==opositekolor))
					{	
						
						plansza[i][j] = PPOLE;
						points(opositekolor,punktyb,punktyc);
					}
				}
				else if (i == size-1 && j == 0)
				{
					if ((plansza[i-1][j] == opositekolor) && (plansza[i][j+1] == opositekolor))
					{
						plansza[i][j] = PPOLE;
						points(opositekolor, punktyb, punktyc);
					}
				}
				else if (i == 0 && j == size - 1)
				{
					if ((plansza[i][j -1] == opositekolor) && (plansza[i + 1][j] == opositekolor))
					{
						plansza[i][j] = PPOLE;
						points(opositekolor, punktyb, punktyc);
					}
				}
				else if (i == size - 1 && j == size - 1)
				{
					if ((plansza[i-1][j ] == opositekolor) && (plansza[i][j-1] == opositekolor))
					{
						plansza[i][j] = PPOLE;
						points(opositekolor, punktyb, punktyc);
					}
				}

				else if (i==0)
				{
					if ((plansza[i][j+1] == opositekolor) && (plansza[i+1][j] == opositekolor) && (plansza[i][j-1] == opositekolor))
					{
						plansza[i][j] = PPOLE;
						points(opositekolor, punktyb, punktyc);
					}
				}
				else if (i == size - 1)
				{
					if ((plansza[i][j -1] == opositekolor) && (plansza[i -1][j] == opositekolor) && (plansza[i][j +1] == opositekolor))
					{
						plansza[i][j] = PPOLE;
						points(opositekolor, punktyb, punktyc);
					}
				}
				else if ( j == 0)
				{
					if ((plansza[i+1][j] == opositekolor) && (plansza[i][j+1] == opositekolor) && (plansza[i-1][j] == opositekolor))
					{
						plansza[i][j] = PPOLE;
						points(opositekolor, punktyb, punktyc);
					}
				}
				else if (j == size - 1)
				{
					if ((plansza[i+1][j] == opositekolor) && (plansza[i][j-1] == opositekolor) && (plansza[i-1][j] == opositekolor))
					{
						plansza[i][j] = PPOLE;
						points(opositekolor, punktyb, punktyc);
					}
				}
				else if ((plansza[i - 1][j] == PPOLE) || (plansza[i + 1][j] == PPOLE) || (plansza[i][j - 1] == PPOLE) || (plansza[i][j + 1] == PPOLE))
				{
					plansza[i][j] = kolor;
				}
				else if ((plansza[i - 1][j] == opositekolor) && (plansza[i + 1][j] == opositekolor) && (plansza[i][j - 1] == opositekolor) && (plansza[i][j + 1] == opositekolor))
				{
					plansza[i][j] = PPOLE;
					points(opositekolor, punktyb, punktyc);
				}
			}
		}
	}
}

void ko(int posx, int posy, int* punktyb, int* punktyc, int** plansza,int **plansza2,int *ruch,int size)
{
	if (plansza[posx][posy] == KCZARNE)
	{
		if (plansza[posx][posy] == plansza2[posx][posy])
		{
			plansza[posx][posy] = PPOLE;	
			punktyc--;
			ruch--;
		}
		else
		{

			for (int i = 0; i < size; i++)
			{
				for (int j = 0; j < size; j++)
				{
					if (plansza2[i][j] == KCZARNE)
					{
						plansza2[i][j] = 0;
					}
				}
			}
			plansza2[posx][posy] = KCZARNE;
		}
	}
	else if (plansza[posx][posy] == KBIALE)
	{
		if (plansza[posx][posy] == plansza2[posx][posy])
		{
			plansza[posx][posy] = PPOLE;
			punktyb--;
			ruch--;
		}
		else
		{
			for (int i = 0; i < size; i++)
			{
				for (int j = 0; j < size; j++)
				{
					if (plansza2[i][j] == KBIALE)
					{
						plansza2[i][j] = 0;
					}
				}
			}
			plansza2[posx][posy] = KBIALE;
		}
	}
	
}


int main() {
	int zn = 0, x = BKOLUMN+1, y = BROW+1, attr = 7, back = 0, zero = 0 ;
	int ruch = 0;
	int* wruch = &ruch;
	int Bpunkty = 0;
	int* punktyb = &Bpunkty;
	int Cpunkty = 0;
	int* punktyc = &Cpunkty;
	int sizepocz = 0;
	int* wsizepocz = &sizepocz;
	
#ifndef __cplusplus
	Conio2_Init();
#endif
	
	settitle("Jakub, Grunwald, 193346");
	_setcursortype(_NOCURSOR);
	textbackground(BLACK);
	clrscr();
	settingsizeofboard(wsizepocz);
	int** plansza1 = new int* [sizepocz];
	for (int i = 0; i < sizepocz; ++i)
		plansza1[i] = new int[sizepocz];
	for (int q = 0; q < sizepocz; q++)
	{
		for (int a = 0; a < sizepocz; a++)
		{
			plansza1[q][a] = 0;
		}
	}

	int** plansza2 = new int* [sizepocz];
	for (int i = 0; i < sizepocz; ++i)
		plansza2[i] = new int[sizepocz];
	for (int q = 0; q < sizepocz; q++)
	{
		for (int a = 0; a < sizepocz; a++)
		{
			plansza2[q][a] = 0;
		}
	}

	do {
		textbackground(BLACK);
		clrscr();
		gotoxy(LKOLUMN, LROW);
		gotoxy(x, y);	
		board(sizepocz + 2, BKOLUMN, BROW,plansza1);
		instruction(punktyc,punktyb,wruch);
		position(x - BKOLUMN, y - BROW);
		gotoxy(x, y);
		textbackground(back);
		putch('X');
		textcolor(attr);
		zero = 0;
		zn = getch();
		if(zn == 0) {
			zero = 1;		
			zn = getch();		
			if (zn == ARROWDOWN)
			{
				y++;
				if (y > BROW+sizepocz)
				{
					y--;
				}
			}
			else if (zn == ARROWUP)
			{
				y--;
				if (y < BROW+1)
				{
					y++;
				}
			}
			else if (zn == ARROWLEFT)
			{
				x--;
				if (x < BKOLUMN+1 )
				{
					x++;
				}	
			}
			else if (zn == ARROWRIGHT)
			{
				x++;
				if (x > BKOLUMN + sizepocz )
				{
					x--;
				}
			}		
		} 
		//ruch strzalkami
		else if (zn == N)
		{
			do {
				zn = getch();
				if (zn == ENTER)
				{
					clear(plansza1, wsizepocz, wruch, punktyb, punktyc,plansza2);
					break;
				}
			} while (zn != ESC);	
		}
		//nowa gra
		else if (zn == I)
		{	
			do {
				zn = getch();
				if (zn == ENTER)
				{
					if (*wruch % 2 != 0)
					{
						
						int posx, posy;
						posx = x - BKOLUMN - 1;
						posy = y - BROW - 1;
						if (plansza1[posx][posy] == 0)
						{
							plansza1[posx][posy] = KBIALE;
							ko(posx, posy, punktyb, punktyc, plansza1, plansza2, wruch,sizepocz);
							capturing(plansza1, sizepocz, KCZARNE, KBIALE, punktyb, punktyc);
							selfkill(posx, posy, plansza1, KBIALE, KCZARNE, sizepocz);
							if (plansza1[posx][posy] == KBIALE)
							{
								back = BLACK;
								plansza1[posx][posy] = KBIALE;
								(*wruch)++;
							}
						}
					}
					else if (*wruch % 2 == 0)
					{
						int posx, posy;
						posx = x - BKOLUMN - 1;
						posy = y - BROW - 1;
						if (plansza1[posx][posy] == 0)
						{
							plansza1[posx][posy] = KCZARNE;
							ko(posx, posy, punktyb, punktyc, plansza1, plansza2, wruch,sizepocz);
							capturing(plansza1, sizepocz, KBIALE, KCZARNE, punktyb, punktyc);
							selfkill(posx, posy, plansza1, KCZARNE, KBIALE, sizepocz);
							if (plansza1[posx][posy] == KCZARNE)
							{
								back = WHITE;
								plansza1[posx][posy] = KCZARNE;
								(*wruch)++;
							}
						}
					}
					break;
				}
			} while (zn != ESC);
		}
		//kamienie kladzenie itp
		else if (zn == S)
		{
			savefile(plansza1, plansza2, wruch, punktyb, punktyc, sizepocz);
		}
		else if (zn == L)
		{
			readfile(plansza1, plansza2, wruch, punktyb, punktyc, sizepocz);
		}
		else if (zn == M && ruch==0)
		{
			do {
				textbackground(BLACK);
				clrscr();
				gotoxy(x, y);
				board(sizepocz + 2, BKOLUMN, BROW, plansza1);
				textcolor(WHITE);
				cputs("SETTING HANDICAP");
				gotoxy(x, y);
				putch('X');
				zero = 0;
				zn = getch();
				if (zn == 0) {
					zero = 1;		
					zn = getch();		
					if (zn == ARROWDOWN)
					{
						y++;
						if (y > BROW + sizepocz)
						{
							y--;
						}
					}
					else if (zn == ARROWUP)
					{
						y--;
						if (y < BROW + 1)
						{
							y++;
						}
					}
					else if (zn == ARROWLEFT)
					{
						x--;
						if (x < BKOLUMN + 1)
						{
							x++;
						}
					}
					else if (zn == ARROWRIGHT)
					{
						x++;
						if (x > BKOLUMN + sizepocz)
						{
							x--;
						}
					}
				}
				else if (zn == I)
				{
					int posx, posy;
					posx = x - BKOLUMN - 1;
					posy = y - BROW - 1;
					plansza1[posx][posy] = KCZARNE;
				}


			} while (zn != ENTER);
			for (int i = 0; i < sizepocz; i++)
			{
				for (int j = 0; j < sizepocz; j++)
				{
					if (plansza1[i][j] == KCZARNE)
					{
						(*wruch)++;
						break;
					}	
				}
				break;
			}
			if (ruch == 0)
			{
				back = BLACK;
			}
			else
			back = WHITE;
		}
	} while (zn != 'q');

	_setcursortype(_NORMALCURSOR);	// show the cursor so it will be
					// visible after the program ends
	for (int i = 0; i < sizepocz; ++i) {
		delete[] plansza1[i];
	}
	delete[] plansza1;
	for (int i = 0; i < sizepocz; ++i) {
		delete[] plansza2[i];
	}
	delete[] plansza2;
	return 0;
	}

