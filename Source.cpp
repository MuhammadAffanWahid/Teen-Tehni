#include <iostream>
#include <conio.h>
#include<math.h>
#include <Windows.h>
#include<time.h>
using namespace std;

void DisplayBackground(int bdim);
void DisplayBoxWithOneDiagonal(int bdim, int sr, int sc, char sym, bool D1);
void gotoRowCol(int rpos, int cpos);
void getRowColbyLeftClick(int& rpos, int& cpos);
void init(char B[][5], int& turn, int &bdim);
void DisplayBoard(char B[][5], int bdim);
void ChangeColour(int k);
void PlayerSelection(int& r, int& c,int bdim);
void move(char B[][5], int sr, int sc, int er, int ec);
void Swap(char& A, char& B);
bool IsValidSelection(char B[][5], int sr, int sc, int bdim, int turn);
void TurnChange(int& turn);
bool IsValidMove(char B[][5], int sr, int sc, int er, int ec, int turn);
bool IsGameEnd(char B[][5]);
void DisplayWinner(char B[][5]);

int main()
{
	char B[5][5];
	int sr, sc, er, ec, turn;
	int bdim;
	srand(time(0));
	init(B, turn,bdim);
	DisplayBackground(bdim);
	do
	{
		DisplayBoard(B, bdim);
		do
		{
			do
			{
				PlayerSelection(sr, sc, bdim);
				if (!IsValidSelection(B, sr, sc, bdim, turn))
				{
					gotoRowCol(bdim, 5 * bdim);
					cout << "Invalid Selection!";
					Sleep(800);
					gotoRowCol(bdim, 5 * bdim);
					cout << "                  ";
				}
			} while (!IsValidSelection(B, sr, sc, bdim, turn));
			PlayerSelection(er, ec, bdim);
			if (!IsValidMove(B, sr, sc, er, ec, turn))
			{
				gotoRowCol(bdim + 1, 5 * bdim);
				cout << "Invalid Move";
				Sleep(800);
				gotoRowCol(bdim + 1, 5 * bdim);
				cout << "            ";
			}
		} while (!IsValidMove(B, sr, sc, er, ec, turn));
		move(B, sr, sc, er, ec);
		TurnChange(turn);
	} while (!IsGameEnd(B));
	DisplayWinner(B);
	return _getch();
}

void DisplayBackground(int bdim)
{
	char sym = '*';
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if ((i + j) % 2 == 0) DisplayBoxWithOneDiagonal(bdim, i * bdim - i, j * bdim - j, sym, 1);
			else DisplayBoxWithOneDiagonal(bdim, i * bdim - i, j * bdim - j, sym, 0);
		}
	}
}
void DisplayBoxWithOneDiagonal(int bdim, int sr, int sc,char sym, bool D1)
{
	for (int i = 0; i < bdim; i++)
	{
		for (int j = 0; j < bdim; j++)
		{
			if (D1)
			{
				if (i == 0 || i == bdim - 1 || j == 0 || j == bdim - 1 || i == j)
				{
					gotoRowCol(sr + i, sc + j);
					cout << sym;
				}
			}
			else
			{
				if (i == 0 || i == bdim - 1 || j == 0 || j == bdim - 1 || i == bdim-j-1)
				{
					gotoRowCol(sr + i, sc + j);
					cout << sym;
				}
			}
		}
	}
}
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}
void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
void init(char B[][5], int& turn, int &bdim)
{
	for (int i = 0; i < 5; i++)
	{
		B[0][i] = B[1][i] = 'A';
		B[3][i] = B[4][i] = 'B';
	}
	B[2][0] = B[2][1] = 'A';
	B[2][3] = B[2][4] = 'B';
	B[2][2] = '-';
	turn = rand() % 2;
	bdim = 8;
}
void ChangeColour(int k)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, k);
	if (k > 255)
		k = 1;
}
void DisplayBoard(char B[][5], int bdim)
{
	char sym = -37;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (B[i][j] == 'A')
			{
				ChangeColour(4);   
				gotoRowCol(bdim * i - i, bdim * j - j);
				cout << sym;
				ChangeColour(7);
			}
			else if (B[i][j] == 'B')
			{
				ChangeColour(2);
				gotoRowCol(bdim * i - i, bdim * j - j);
				cout << sym;
				ChangeColour(7);
			}
			else
			{
				gotoRowCol(bdim * i - i, bdim * j - j);
				cout << '*';
			}
		}
	}
}
void PlayerSelection(int& r, int& c,int bdim)
{
	bdim--;
	do
	{
		getRowColbyLeftClick(r, c);
	} while (r % bdim != 0 || c % bdim != 0 || r > 4 * bdim || c > 4 * bdim);
	r = r / bdim;
	c = c / bdim;
}
void move(char B[][5], int sr, int sc, int er, int ec)
{
	int dr = er - sr;
	int dc = ec - sc;
	Swap(B[sr][sc], B[er][ec]);
	if ((abs(dr) == 2 || abs(dc) == 2)) B[sr + dr / 2][sc + dc / 2] = '-';
}
void Swap(char& A, char& B)
{
	char T = A;
	A = B;
	B = T;
}
bool IsValidSelection(char B[][5], int sr, int sc, int bdim, int turn)
{
	if (turn == 0 && B[sr][sc] == 'A') return true;
	if (turn == 1 && B[sr][sc] == 'B') return true;
	return false;
}
void TurnChange(int& turn)
{
	turn = (turn + 1) % 2;
}
bool IsValidMove(char B[][5], int sr, int sc, int er, int ec, int turn)
{
	int dr = er - sr;
	int dc = ec - sc;
	if (B[er][ec] != '-') return false;
	if ((sr + sc) % 2 != 0 && dr != 0 && dc != 0) return false;
	if (turn == 0 && B[er][ec] == 'A') return false;
	if (turn == 1 && B[er][ec] == 'B') return false;
	if (abs(dr) > 2 || abs(dc) > 2) return false;
	if ((abs(dr) == 2 || abs(dc) == 2) && B[sr + dr / 2][sc + dc / 2] != 'B' && turn == 0) return false;
	if ((abs(dr) == 2 || abs(dc) == 2) && B[sr + dr / 2][sc + dc / 2] != 'A' && turn == 1) return false;
	return true;
}
bool IsGameEnd(char B[][5])
{
	bool Afound=false, Bfound=false;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (B[i][j] == 'A') Afound = true;
			if (B[i][j] == 'B') Bfound = true;
		}
	}
	return !(Afound && Bfound);
}
void DisplayWinner(char B[][5])
{
	system("cls");
	bool Afound = false;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (B[i][j] == 'A') Afound = true;
		}
	}
	if (!Afound) cout << "Player With Green Gotis Is The Winner!!!";
	else cout << "Player With Red Gotis Is The Winner!!!";
}