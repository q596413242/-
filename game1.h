#include<iostream>
#include<fstream>
#include<iomanip>
#include<conio.h>
#include<stdlib.h>
#include"windows.h"
using namespace std;

void initialize(int);
void Pass();
void Play();

int MAXV[2], np;
char Step[100][2], no, bo;
char** A;

struct path
{
	char** loc;
	path* next;
};
struct conta
{
	char loc[2];
	conta** next;
};

class Game              //类：每一幅图  
{
public:
	void Graph()
	{
		system("cls");
		acco = 0; cout << "Pass" << np << endl;
		for (int i = 0; i < MAXV[0]; i++)
		{
			for (int j = 0; j < MAXV[1]; j++)
			{
				switch (A[i][j])
				{
				case '0':cout << setw(2) << " "; break;   //空白
				case '1':SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
					cout << setw(2) << "∑"; break;   //墙
				case '2':cout << setw(2) << "〇"; break;   //箱子
				case '7':cout << setw(2) << "#"; break;   //白箱子
				case '3':SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
					cout << setw(2) << "♀"; Role[0] = i; Role[1] = j; break;           //人物
				case '4':cout << setw(2) << "·"; break;   //目的地
				case '5':cout << setw(2) << "⊙"; acco++; break;  //到达
				case '8':cout << setw(2) << "%"; break;   //白箱达
				case '6':SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
					cout << setw(2) << "♂"; Role[0] = i; Role[1] = j;                  //人物目的地
				}
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			}
			cout << endl;
		}
	}

	void MoveGrid()
	{
		char ch;
		do
		{
			for (ch = _getch(); (ch != -32 && ch != 27 && ch != 26);)
				ch = _getch();
			if (ch == -32)
				ch = _getch();
			if (ch == 27)
				Menu();
			else if (ch == 26)
				LastStep();                               //返回上一步
			else if (MoveJudge(ch))
			{
				Move(ch);
				Graph();
			}
		} while (acco != goal);
		cout << "win" << endl << "按任意键继续 下一关";
		_getch(); system("cls");
		np++;
		initialize(np);
		Play();
	}
	int goal;

protected:
	void Menu()
	{
		system("cls");
		int k = 0; char menu[5][8] = { {"Back"},{"Restart"}, {"Pass"},{"Exit"} ,{"Help" } }, ch;
		do
		{
			system("cls");
			for (int i = 0; i < 5; i++)
			{
				if (i == k)
					cout << setw(8) << "->";
				else
					cout << setw(8) << " ";
				cout << menu[i] << endl;
			}
			cout << "    游戏中Ctrl+z撤销上一步          请不要瞎jb按" << endl;
			for (ch = _getch(); (ch != 72 && ch != 80);)
			{
				while (ch != -32 && ch != 13)
					ch = _getch();
				if (ch == 13)
					break;
				ch = _getch();
			}
			if (ch == 72)
				if (k != 0)
					k = (k - 1) % 5;
				else
					k = 4;
			if (ch == 80)
				k = (k + 1) % 5;
		} while (ch != 13);
		switch (k)
		{
		case 0:Graph(); return;
		case 1:initialize(np); Play(); break;
		case 2:Pass(); break;
		case 3:exit(0);
		case 4:int acco1 = acco; FindAPath(); acco = acco1;
		}
	}

	bool MoveJudge(char k)               //75-left 72-up 77-right 80-down 
	{
		if (k == 75)
			if (A[Role[0]][Role[1] - 1] == '1' || ((A[Role[0]][Role[1] - 1] == '2' || A[Role[0]][Role[1] - 1] == '7' || A[Role[0]][Role[1] - 1] == '5' || A[Role[0]][Role[1] - 1] == '8') && (A[Role[0]][Role[1] - 2] == '1' || A[Role[0]][Role[1] - 2] == '2' || A[Role[0]][Role[1] - 2] == '7' || A[Role[0]][Role[1] - 2] == '5' || A[Role[0]][Role[1] - 2] == '8')))
				return false;
		if (k == 72)
			if (A[Role[0] - 1][Role[1]] == '1' || ((A[Role[0] - 1][Role[1]] == '2' || A[Role[0] - 1][Role[1]] == '7' || A[Role[0] - 1][Role[1]] == '5' || A[Role[0] - 1][Role[1]] == '8') && (A[Role[0] - 2][Role[1]] == '1' || A[Role[0] - 2][Role[1]] == '2' || A[Role[0] - 2][Role[1]] == '7' || A[Role[0] - 2][Role[1]] == '5' || A[Role[0] - 2][Role[1]] == '8')))
				return false;
		if (k == 77)
			if (A[Role[0]][Role[1] + 1] == '1' || ((A[Role[0]][Role[1] + 1] == '2' || A[Role[0]][Role[1] + 1] == '7' || A[Role[0]][Role[1] + 1] == '5' || A[Role[0]][Role[1] + 1] == '8') && (A[Role[0]][Role[1] + 2] == '1' || A[Role[0]][Role[1] + 2] == '2' || A[Role[0]][Role[1] + 2] == '7' || A[Role[0]][Role[1] + 2] == '5' || A[Role[0]][Role[1] + 2] == '8')))
				return false;
		if (k == 80)
			if (A[Role[0] + 1][Role[1]] == '1' || ((A[Role[0] + 1][Role[1]] == '2' || A[Role[0] + 1][Role[1]] == '7' || A[Role[0] + 1][Role[1]] == '5' || A[Role[0] + 1][Role[1]] == '8') && (A[Role[0] + 2][Role[1]] == '1' || A[Role[0] + 2][Role[1]] == '2' || A[Role[0] + 2][Role[1]] == '7' || A[Role[0] + 2][Role[1]] == '5' || A[Role[0] + 2][Role[1]] == '8')))
				return false;
		return true;
	}

	void Move(char k)
	{
		Step[no][0] = k; Step[no][1] = 0;
		if (k == 75)
			if (A[Role[0]][Role[1] - 1] == '0')
				A[Role[0]][Role[1] - 1] = '3';
			else if (A[Role[0]][Role[1] - 1] == '2')
			{
				if (A[Role[0]][Role[1] - 2] == '4')
					A[Role[0]][Role[1] - 2] = '5';
				else
					A[Role[0]][Role[1] - 2] = '2';
				A[Role[0]][Role[1] - 1] = '3';
				Step[no][1] = 1;
			}
			else if (A[Role[0]][Role[1] - 1] == '7')
			{
				if (A[Role[0]][Role[1] - 2] == '4')
					A[Role[0]][Role[1] - 2] = '8';
				else
					A[Role[0]][Role[1] - 2] = '7';
				A[Role[0]][Role[1] - 1] = '3';
			}
			else if (A[Role[0]][Role[1] - 1] == '4')
				A[Role[0]][Role[1] - 1] = '6';
			else if (A[Role[0]][Role[1] - 1] == '5')
			{
				if (A[Role[0]][Role[1] - 2] == '4')
					A[Role[0]][Role[1] - 2] = '5';
				else
					A[Role[0]][Role[1] - 2] = '2';
				A[Role[0]][Role[1] - 1] = '6';
				Step[no][1] = 1;
			}
			else
			{
				if (A[Role[0]][Role[1] - 2] == '4')
					A[Role[0]][Role[1] - 2] = '8';
				else
					A[Role[0]][Role[1] - 2] = '7';
				A[Role[0]][Role[1] - 1] = '6';
			}
		if (k == 72)
			if (A[Role[0] - 1][Role[1]] == '0')
				A[Role[0] - 1][Role[1]] = '3';
			else if (A[Role[0] - 1][Role[1]] == '2')
			{
				if (A[Role[0] - 2][Role[1]] == '4')
					A[Role[0] - 2][Role[1]] = '5';
				else
					A[Role[0] - 2][Role[1]] = '2';
				A[Role[0] - 1][Role[1]] = '3';
				Step[no][1] = 1;
			}
			else if (A[Role[0] - 1][Role[1]] == '7')
			{
				if (A[Role[0] - 2][Role[1]] == '4')
					A[Role[0] - 2][Role[1]] = '8';
				else
					A[Role[0] - 2][Role[1]] = '7';
				A[Role[0] - 1][Role[1]] = '3';
			}
			else if (A[Role[0] - 1][Role[1]] == '4')
				A[Role[0] - 1][Role[1]] = '6';
			else if (A[Role[0] - 1][Role[1]] == '5')
			{
				if (A[Role[0] - 2][Role[1]] == '4')
					A[Role[0] - 2][Role[1]] = '5';
				else
					A[Role[0] - 2][Role[1]] = '2';
				A[Role[0] - 1][Role[1]] = '6';
				Step[no][1] = 1;
			}
			else
			{
				if (A[Role[0] - 2][Role[1]] == '4')
					A[Role[0] - 2][Role[1]] = '8';
				else
					A[Role[0] - 2][Role[1]] = '7';
				A[Role[0] - 1][Role[1]] = '6';
			}
		if (k == 77)
			if (A[Role[0]][Role[1] + 1] == '0')
				A[Role[0]][Role[1] + 1] = '3';
			else if (A[Role[0]][Role[1] + 1] == '2')
			{
				if (A[Role[0]][Role[1] + 2] == '4')
					A[Role[0]][Role[1] + 2] = '5';
				else
					A[Role[0]][Role[1] + 2] = '2';
				A[Role[0]][Role[1] + 1] = '3'; Step[no][1] = 1;
			}
			else if (A[Role[0]][Role[1] + 1] == '7')
			{
				if (A[Role[0]][Role[1] + 2] == '4')
					A[Role[0]][Role[1] + 2] = '8';
				else
					A[Role[0]][Role[1] + 2] = '7';
				A[Role[0]][Role[1] + 1] = '3';
			}
			else if (A[Role[0]][Role[1] + 1] == '4')
				A[Role[0]][Role[1] + 1] = '6';
			else if (A[Role[0]][Role[1] + 1] == '5')
			{
				if (A[Role[0]][Role[1] + 2] == '4')
					A[Role[0]][Role[1] + 2] = '5';
				else
					A[Role[0]][Role[1] + 2] = '2';
				A[Role[0]][Role[1] + 1] = '6'; Step[no][1] = 1;
			}
			else
			{
				if (A[Role[0]][Role[1] + 2] == '4')
					A[Role[0]][Role[1] + 2] = '8';
				else
					A[Role[0]][Role[1] + 2] = '7';
				A[Role[0]][Role[1] + 1] = '6';
			}
		if (k == 80)
			if (A[Role[0] + 1][Role[1]] == '0')
				A[Role[0] + 1][Role[1]] = '3';
			else if (A[Role[0] + 1][Role[1]] == '2')
			{
				if (A[Role[0] + 2][Role[1]] == '4')
					A[Role[0] + 2][Role[1]] = '5';
				else
					A[Role[0] + 2][Role[1]] = '2';
				A[Role[0] + 1][Role[1]] = '3'; Step[no][1] = 1;
			}
			else if (A[Role[0] + 1][Role[1]] == '7')
			{
				if (A[Role[0] + 2][Role[1]] == '4')
					A[Role[0] + 2][Role[1]] = '8';
				else
					A[Role[0] + 2][Role[1]] = '7';
				A[Role[0] + 1][Role[1]] = '3';
			}
			else if (A[Role[0] + 1][Role[1]] == '4')
				A[Role[0] + 1][Role[1]] = '6';
			else if (A[Role[0] + 1][Role[1]] == '5')
			{
				if (A[Role[0] + 2][Role[1]] == '4')
					A[Role[0] + 2][Role[1]] = '5';
				else
					A[Role[0] + 2][Role[1]] = '2';
				A[Role[0] + 1][Role[1]] = '6'; Step[no][1] = 1;
			}
			else
			{
				if (A[Role[0] + 2][Role[1]] == '4')
					A[Role[0] + 2][Role[1]] = '8';
				else
					A[Role[0] + 2][Role[1]] = '7';
				A[Role[0] + 1][Role[1]] = '6';
			}
		if (A[Role[0]][Role[1]] == '6')
			A[Role[0]][Role[1]] = '4';
		else
			A[Role[0]][Role[1]] = '0';
		no = (no + 1) % 100;
		if (no == bo)
			bo = (bo + 1) % 100;
	}

	void FindAPath()
	{
		initialize(np); path* pa = new path; pa->loc = new char* [100]; pa->next = NULL; bcount = goal; conta** ta = new conta * [goal + 1];
		char M[4] = { 75,72,77,80 }; char** B = new char* [MAXV[0]]; char** C = new char* [MAXV[0] + 2]; char** AA = new char* [MAXV[0]];
		C[MAXV[0]] = new char[2 * goal + 2]; C[MAXV[0] + 1] = new char[2 * goal];
		boac = goal = acco = 0;
		for (int i = 0; i < MAXV[0]; i++)
		{
			C[i] = new char[MAXV[1]]; B[i] = new char[MAXV[1]]; AA[i] = new char[MAXV[1]];
			for (int j = 0; j < MAXV[1]; j++)
			{
				if (A[i][j] == '3' || A[i][j] == '6')
				{
					C[MAXV[0]][2 * bcount] = i; C[MAXV[0]][2 * bcount + 1] = j;
				}
				if (A[i][j] == '1')
					C[i][j] = 1;
				else
					C[i][j] = 0;
				if (A[i][j] == '2' || A[i][j] == '5')
				{
					C[MAXV[0]][2 * acco] = i; C[MAXV[0]][2 * acco + 1] = j;
					C[MAXV[0] + 1][2 * acco] = i; C[MAXV[0] + 1][2 * acco + 1] = j;
					ta[acco] = new conta; ta[acco]->loc[0] = i; ta[acco]->loc[1] = j; ta[acco]->next = NULL;
					acco++;
					if (A[i][j] == '2')
						goal++;
				}
				B[i][j] = 0;
				AA[i][j] = A[i][j];
			}
		}
		ta[bcount] = new conta; ta[bcount]->loc[0] = Role[0]; ta[bcount]->loc[1] = Role[1]; ta[bcount]->next = NULL;
		Graph();
		acco = 0;
		char* filed = new char[2 * bcount];
		for (int j = 0; j < 2 * bcount; j++)
			filed[j] = C[MAXV[0] + 1][j];
		if (!P(B, pa, ta, C, M, filed, goal))
		{
			cout << "  无路径" << endl;
			delete[] filed;
			Deleteconta(ta, 0);
			for (int i = 0; i < MAXV[0]; i++)
			{
				delete[] B[i];
				delete[] C[i];
			}
			delete[] C[MAXV[0]]; delete[] C[MAXV[0] + 1];
			delete[] B;
			delete[] C;
		}
		else
		{
			delete[] filed;
			Deleteconta(ta, 0);
			for (int i = 0; i < MAXV[0]; i++)
			{
				delete[] B[i];
				delete[] C[i];
			}
			delete[] C[MAXV[0]]; delete[] C[MAXV[0] + 1];
			delete[] B;
			delete[] C;
			int acco1 = acco;
			PathOut(AA, acco1, pa);
		}
	}
	bool P(char** B, path*& pa, conta**& ta, char** C, char* M,char* filed,int goal1)           //改为广度优先遍历 ，节约堆栈空间
	{
		if (goal == 0)
		{
			goal = acco - 1;
			return true;
		}
		B[Role[0]][Role[1]] = 1; int R[2]; R[0] = Role[0]; R[1] = Role[1];
		char* filed1;
		for (int k = 0; k < 4; k++)
			if (MoveJudge(M[k]))
			{
				Move(M[k]);
				int no1 = no;
				if (no == 0)
					no1 = 100;
				switch (M[k])
				{
				case 75:Role[1]--; break;
				case 72:Role[0]--; break;
				case 77:Role[1]++; break;
				case 80:Role[0]++;
				}
				if (B[Role[0]][Role[1]] == 0)
					if (Step[no1 - 1][1] == 1)
					{
						Adjust(C[MAXV[0] + 1], M[k]);
						if (GridJudge(M[k]) && PathJudge(ta, C))
						{
							acco++;
							if ((M[k] == 75 && A[Role[0]][Role[1] - 1] == '5') || (M[k] == 72 && A[Role[0] - 1][Role[1]] == '5') || (M[k] == 77 && A[Role[0]][Role[1] + 1] == '5') || (M[k] == 80 && A[Role[0] + 1][Role[1]] == '5'))
								goal--;
							if (A[Role[0]][Role[1]] == '6')
								goal++;
							char** B1 = new char* [MAXV[0]];
							for (int i = 0; i < MAXV[0]; i++)
							{
								B1[i] = new char[MAXV[1]];
								for (int j = 0; j < MAXV[1]; j++)
									B1[i][j] = 0;
							}
							filed1 = new char[2 * bcount];
							for (int j = 0; j < 2 * bcount; j++)
								filed1[j] = C[MAXV[0] + 1][j];
							if (P(B1, pa, ta, C, M, filed1, goal))
							{
								if (goal % 100 == 99)
								{
									path* pa1 = new path; pa1->loc = new char* [100]; pa1->next = pa; pa = pa1;
								}
								pa->loc[goal % 100] = new char[3];
								pa->loc[goal % 100][0] = M[k]; pa->loc[goal % 100][1] = R[0]; pa->loc[goal % 100][2] = R[1];
								goal--;
								for (int i = 0; i < MAXV[0]; i++)
									delete[] B1[i];
								delete[] B1;
								delete[] filed1;
								return true;
							}
							delete[] filed1;
							for (int i = 0; i < MAXV[0]; i++)
								delete[] B1[i];
							delete[] B1;
							acco--; goal = goal1;
						}
						for (int j = 0; j < 2 * bcount; j++)
							C[MAXV[0] + 1][j] = filed[j];
						LastStep(M[k], 1);
					}
					else 
					{
						if (P(B, pa, ta, C, M, filed, goal))
							return true;
						LastStep(M[k], 0);
					}
				else
					LastStep(M[k], 0);
				Role[0] = R[0]; Role[1] = R[1];
			}
		return false;
	}
	bool GridJudge(char k)                 //需要一个好的死锁判定
	{
		int L[2]; L[0] = Role[0]; L[1] = Role[1]; int i, j;
		switch (k)
		{
		case 75:L[1]--; if (A[L[0]][L[1] - 1] == '0' || A[L[0]][L[1] - 1] == '4')return true;
			if ((A[L[0] - 1][L[1]] == '1' || A[L[0] - 1][L[1]] == '2' || A[L[0] - 1][L[1]] == '5') && (A[L[0] - 1][L[1] - 1] == '1' || A[L[0] - 1][L[1] - 1] == '2' || A[L[0] - 1][L[1] - 1] == '5'))
			{
				if (A[L[0]][L[1]] == '5' && A[L[0]][L[1] - 1] != '2' && A[L[0] - 1][L[1]] != '2' && A[L[0] - 1][L[1] - 1] != '2')
					return true;
				return false;
			}
			if ((A[L[0] + 1][L[1]] == '1' || A[L[0] + 1][L[1]] == '2' || A[L[0] + 1][L[1]] == '5') && (A[L[0] + 1][L[1] - 1] == '1' || A[L[0] + 1][L[1] - 1] == '2' || A[L[0] + 1][L[1] - 1] == '5'))
			{
				if (A[L[0]][L[1]] == '5' && A[L[0]][L[1] - 1] != '2' && A[L[0] + 1][L[1]] != '2' && A[L[0] + 1][L[1] - 1] != '2')
					return true;
				return false;
			}
			if (A[L[0]][L[1] - 1] == '1')
			{
				if (A[L[0]][L[1]] == '5')
					return true;
				int numofbox = 1, numofgoal = 0;
				for (i = L[0]; A[i - 1][L[1]] != '1'; i--)
				{
					if (A[i - 1][L[1]] == '2')
						numofbox++;
					if (A[i - 1][L[1]] == '4')
						numofgoal++;
				}
				for (j = L[0]; A[j + 1][L[1]] != '1'; j++)
				{
					if (A[j + 1][L[1]] == '2')
						numofbox++;
					if (A[j + 1][L[1]] == '4')
						numofgoal++;
				}
				while (A[i][L[1] - 1] == '1')
					if (i == j)
						if (numofbox <= numofgoal)
							return true;
						else
							return false;
					else
						i++;
			}
			return true;
		case 72:L[0]--; if (A[L[0] - 1][L[1]] == '0' || A[L[0] - 1][L[1]] == '4')return true;
			if ((A[L[0]][L[1] - 1] == '1' || A[L[0]][L[1] - 1] == '2' || A[L[0]][L[1] - 1] == '5') && (A[L[0] - 1][L[1] - 1] == '1' || A[L[0] - 1][L[1] - 1] == '2' || A[L[0] - 1][L[1] - 1] == '5'))
			{
				if (A[L[0]][L[1]] == '5' && A[L[0] - 1][L[1]] != '2' && A[L[0]][L[1] - 1] != '2' && A[L[0] - 1][L[1] - 1] != '2')
					return true;
				return false;
			}
			if ((A[L[0]][L[1] + 1] == '1' || A[L[0]][L[1] + 1] == '2' || A[L[0]][L[1] + 1] == '5') && (A[L[0] - 1][L[1] + 1] == '1' || A[L[0] - 1][L[1] + 1] == '2' || A[L[0] - 1][L[1] + 1] == '5'))
			{
				if (A[L[0]][L[1]] == '5' && A[L[0] - 1][L[1]] != '2' && A[L[0]][L[1] + 1] != '2' && A[L[0] - 1][L[1] + 1] != '2')
					return true;
				return false;
			}
			if (A[L[0] - 1][L[1]] == '1')
			{
				if (A[L[0]][L[1]] == '5')
					return true;
				int numofbox = 1, numofgoal = 0;
				for (i = L[1]; A[L[0]][i - 1] != '1'; i--)
				{
					if (A[L[0]][i - 1] == '2')
						numofbox++;
					if (A[L[0]][i - 1] == '4')
						numofgoal++;
				}
				for (j = L[1]; A[L[0]][j + 1] != '1'; j++)
				{
					if (A[L[0]][j + 1] == '2')
						numofbox++;
					if (A[L[0]][j + 1] == '4')
						numofgoal++;
				}
				while (A[L[0] - 1][i] == '1')
					if (i == j)
						if (numofbox <= numofgoal)
							return true;
						else
							return false;
					else
						i++;
			}
			return true;
		case 77:L[1]++; if (A[L[0]][L[1] + 1] == '0' || A[L[0]][L[1] + 1] == '4')return true;
			if ((A[L[0] - 1][L[1]] == '1' || A[L[0] - 1][L[1]] == '2' || A[L[0] - 1][L[1]] == '5') && (A[L[0] - 1][L[1] + 1] == '1' || A[L[0] - 1][L[1] + 1] == '2' || A[L[0] - 1][L[1] + 1] == '5'))
			{
				if (A[L[0]][L[1]] == '5' && A[L[0]][L[1] + 1] != '2' && A[L[0] - 1][L[1]] != '2' && A[L[0] - 1][L[1] + 1] != '2')
					return true;
				return false;
			}
			if ((A[L[0] + 1][L[1]] == '1' || A[L[0] + 1][L[1]] == '2' || A[L[0] + 1][L[1]] == '5') && (A[L[0] + 1][L[1] + 1] == '1' || A[L[0] + 1][L[1] + 1] == '2' || A[L[0] + 1][L[1] + 1] == '5'))
			{
				if (A[L[0]][L[1]] == '5' && A[L[0]][L[1] + 1] != '2' && A[L[0] + 1][L[1]] != '2' && A[L[0] + 1][L[1] + 1] != '2')
					return true;
				return false;
			}
			if (A[L[0]][L[1] + 1] == '1')
			{
				if (A[L[0]][L[1]] == '5')
					return true;
				int numofbox = 1, numofgoal = 0;
				for (i = L[0]; A[i - 1][L[1]] != '1'; i--)
				{
					if (A[i - 1][L[1]] == '2')
						numofbox++;
					if (A[i - 1][L[1]] == '4')
						numofgoal++;
				}
				for (j = L[0]; A[j + 1][L[1]] != '1'; j++)
				{
					if (A[j + 1][L[1]] == '2')
						numofbox++;
					if (A[j + 1][L[1]] == '4')
						numofgoal++;
				}
				while (A[i][L[1] + 1] == '1')
					if (i == j)
						if (numofbox <= numofgoal)
							return true;
						else
							return false;
					else
						i++;
			}
			return true;
		case 80:L[0]++; if (A[L[0] + 1][L[1]] == '0' || A[L[0] + 1][L[1]] == '4')return true;
			if ((A[L[0]][L[1] - 1] == '1' || A[L[0]][L[1] - 1] == '2' || A[L[0]][L[1] - 1] == '5') && (A[L[0] + 1][L[1] - 1] == '1' || A[L[0] + 1][L[1] - 1] == '2' || A[L[0] + 1][L[1] - 1] == '5'))
			{
				if (A[L[0]][L[1]] == '5' && A[L[0] + 1][L[1]] != '2' && A[L[0]][L[1] - 1] != '2' && A[L[0] + 1][L[1] - 1] != '2')
					return true;
				return false;
			}
			if ((A[L[0]][L[1] + 1] == '1' || A[L[0]][L[1] + 1] == '2' || A[L[0]][L[1] + 1] == '5') && (A[L[0] + 1][L[1] + 1] == '1' || A[L[0] + 1][L[1] + 1] == '2' || A[L[0] + 1][L[1] + 1] == '5'))
			{
				if (A[L[0]][L[1]] == '5' && A[L[0] + 1][L[1]] != '2' && A[L[0]][L[1] + 1] != '2' && A[L[0] + 1][L[1] + 1] != '2')
					return true;
				return false;
			}
			if (A[L[0] + 1][L[1]] == '1')
			{
				if (A[L[0]][L[1]] == '5')
					return true;
				int numofbox = 1, numofgoal = 0;
				for (i = L[1]; A[L[0]][i - 1] != '1'; i--)
				{
					if (A[L[0]][i - 1] == '2')
						numofbox++;
					if (A[L[0]][i - 1] == '4')
						numofgoal++;
				}
				for (j = L[1]; A[L[0]][j + 1] != '1'; j++)
				{
					if (A[L[0]][j + 1] == '2')
						numofbox++;
					if (A[L[0]][j + 1] == '4')
						numofgoal++;
				}
				while (A[L[0] + 1][i] == '1')
					if (i == j)
						if (numofbox <= numofgoal)
							return true;
						else
							return false;
					else
						i++;
			}
			return true;
		}
	}                          
	bool PathJudge(conta**& ta, char** C)  //cnm cnm cnm 的优化
	{
		if (ta[0]->loc[0] > C[MAXV[0] + 1][0] || (ta[0]->loc[0] == C[MAXV[0] + 1][0] && ta[0]->loc[1] > C[MAXV[0] + 1][1]))
		{
			conta** pre = new conta * [bcount + 1]; pre[0] = new conta; pre[0]->loc[0] = C[MAXV[0] + 1][0]; pre[0]->loc[1] = C[MAXV[0] + 1][1];
			for (int i = 1; i < bcount; i++)
			{
				pre[i] = new conta; pre[i]->loc[0] = C[MAXV[0] + 1][2 * i]; pre[i]->loc[1] = C[MAXV[0] + 1][2 * i + 1]; pre[i]->next = NULL;
			}
			pre[bcount] = new conta; pre[bcount]->loc[0] = Role[0]; pre[bcount]->loc[1] = Role[1]; pre[bcount]->next = NULL;
			pre[0]->next = ta; ta = pre; return true;
		}
		conta** pr = ta; int k = 0, i, flag = 0;
		for (i = 0; i < bcount; i++)
		{
			if (pr[k]->loc[0] == C[MAXV[0] + 1][2 * i] && pr[k]->loc[1] == C[MAXV[0] + 1][2 * i + 1])
				goto N;
			if (pr[k]->loc[0] > C[MAXV[0] + 1][2 * i] || (pr[k]->loc[0] == C[MAXV[0] + 1][2 * i] && pr[k]->loc[1] > C[MAXV[0] + 1][2 * i + 1]))
			{
				flag = 1; break;
			}
			while (pr[k]->next != NULL && pr[k]->next[0]->loc[0] < C[MAXV[0] + 1][2 * i])
			{
				pr = pr[k]->next; k = 0;
			}
			if (pr[k]->next == NULL || pr[k]->next[0]->loc[0] > C[MAXV[0] + 1][2 * i])
				break;
			while (pr[k]->next != NULL && pr[k]->next[0]->loc[1] < C[MAXV[0] + 1][2 * i + 1])
			{
				pr = pr[k]->next; k = 0;
			}
			if (pr[k]->next == NULL || pr[k]->next[0]->loc[1] > C[MAXV[0] + 1][2 * i + 1])
				break;
			pr = pr[k]->next; k = 0;
		N:  k++;
		}
		if (i == bcount)
		{
			int queue[100][2], quf = 0, qul = 0, qu[2], p[2], m;
			for (i = 0; i < bcount; i++)
				C[C[MAXV[0] + 1][2 * i]][C[MAXV[0] + 1][2 * i + 1]] = 1;
			C[Role[0]][Role[1]] = 2;
			queue[qul][0] = Role[0]; queue[qul][1] = Role[1]; qul = (qul + 1) % 100;
			while (quf != qul)
			{
				qu[0] = queue[quf][0]; qu[1] = queue[quf][1]; quf = (quf + 1) % 100;
				m = 0;
				while (m < 4)
				{
					switch (m)
					{
					case 0:p[0] = qu[0] - 1; p[1] = qu[1]; break;
					case 1:p[0] = qu[0]; p[1] = qu[1] - 1; break;
					case 2:p[0] = qu[0] + 1; p[1] = qu[1]; break;
					case 3:p[0] = qu[0]; p[1] = qu[1] + 1;
					}
					if (C[p[0]][p[1]] == 0)
					{
						C[p[0]][p[1]] = 2;
						queue[qul][0] = p[0]; queue[qul][1] = p[1]; qul = (qul + 1) % 100;
						if (qul == quf)
						{
							cout << "  队列上溢出" << endl;
							exit(0);
						}
					}
					m++;
				}
			}
			if (C[pr[k]->loc[0]][pr[k]->loc[1]] == 2)
			{
				reC(C);
				return false;
			}
			while (pr[k]->next != NULL)
				if (C[pr[k]->next[0]->loc[0]][pr[k]->next[0]->loc[1]] == 2)
				{
					reC(C);
					return false;
				}
				else
				{
					pr = pr[k]->next; k = 0;
				}
			conta** last = new conta * [1]; last[0] = new conta; last[0]->loc[0] = Role[0]; last[0]->loc[1] = Role[1]; last[0]->next = NULL; pr[k]->next = last;
			reC(C); return true;
		}
		else if (flag == 0)
		{
			conta** last = new conta * [bcount + 1 - i]; int m = 0; last[0] = new conta;
			for (; i < bcount; i++)
			{
				last[m]->loc[0] = C[MAXV[0] + 1][2 * i]; last[m]->loc[1] = C[MAXV[0] + 1][2 * i + 1];  m++; last[m] = new conta; last[m]->next = NULL;
			}
			last[m]->loc[0] = Role[0]; last[m]->loc[1] = Role[1];
			last[0]->next = pr[k]->next; pr[k]->next = last;
			return true;
		}
		else
		{
			conta** last = new conta * [bcount + 1 - i]; int m = 0; last[0] = new conta; last[0]->next = pr[k]->next; pr[k]->next = last;
			for (; i < bcount; i++)
			{
				last[m]->loc[0] = pr[k]->loc[0]; last[m]->loc[1] = pr[k]->loc[1]; m++; last[m] = new conta;
				pr[k]->loc[0] = C[MAXV[0] + 1][2 * i]; pr[k]->loc[1] = C[MAXV[0] + 1][2 * i + 1]; k++;
				last[m]->next = pr[k]->next; pr[k]->next = NULL;
			}
			last[m]->loc[0] = pr[k]->loc[0]; last[m]->loc[1] = pr[k]->loc[1];
			pr[k]->loc[0] = Role[0]; pr[k]->loc[1] = Role[1];
			return true;
		}
	}
	void reC(char** C)
	{
		for (int i = 1; i < MAXV[0] - 1; i++)
			for (int j = 1; j < MAXV[1] - 1; j++)
				if (C[i][j] == 2)
					C[i][j] = 0;
		for (int i = 0; i < bcount; i++)
			C[C[MAXV[0] + 1][2 * i]][C[MAXV[0] + 1][2 * i + 1]] = 0;
	}
	void Deleteconta(conta** ta, int i)
	{
		/*for (int j = 0; j < i; j++)                             //输出遍历过的箱子位置
			cout << "       ";
		for (int j = 0; j < bcount + 1 - i; j++)
			cout << ta[j]->loc[0] + '0'-48 << " " << ta[j]->loc[1] + '0'-48 << "--->";
		cout << endl;*/                                           //
		for (int j = bcount - i; j >= 0; j--)
		{
			if (ta[j]->next != NULL)
				Deleteconta(ta[j]->next, j + i);
			delete[] ta[j];
		}
		delete[] ta;
	}
	void Adjust(char* bloc, char k)               //究极cnm的Bug;
	{
		int i = 0; char tmp[2]; int m;
		while (bloc[2 * i] != Role[0] || bloc[2 * i + 1] != Role[1])
			i++;
		switch (k)
		{
		case 75:bloc[2 * i + 1]--; break;
		case 72:bloc[2 * i]--;
			tmp[0] = bloc[2 * i]; tmp[1] = bloc[2 * i + 1];
			for (m = i; m > 0 && (bloc[2 * m - 2] > tmp[0] || (bloc[2 * m - 2] == tmp[0] && bloc[2 * m - 1] > tmp[1])); m--)
			{
				bloc[2 * m] = bloc[2 * m - 2]; bloc[2 * m + 1] = bloc[2 * m - 1];
			}
			bloc[2 * m] = tmp[0]; bloc[2 * m + 1] = tmp[1];
			break;
		case 77:bloc[2 * i + 1]++; break;
		case 80:bloc[2 * i]++;
			tmp[0] = bloc[2 * i]; tmp[1] = bloc[2 * i + 1];
			for (m = i; m < bcount - 1 && (bloc[2 * m + 2] < tmp[0] || (bloc[2 * m + 2] == tmp[0] && bloc[2 * m + 3] < tmp[1])); m++)
			{
				bloc[2 * m] = bloc[2 * m + 2]; bloc[2 * m + 1] = bloc[2 * m + 3];
			}
			bloc[2 * m] = tmp[0]; bloc[2 * m + 1] = tmp[1];
		}
	}
	void LastStep(char k, int m)
	{
		if (k == 75)
		{
			if (A[Role[0]][Role[1] + 1] == '0')
				A[Role[0]][Role[1] + 1] = '3';
			else
				A[Role[0]][Role[1] + 1] = '6';
			if (m == 1)
				if (A[Role[0]][Role[1] - 1] == '2')
					A[Role[0]][Role[1] - 1] = '0';
				else
					A[Role[0]][Role[1] - 1] = '4';
		}
		if (k == 72)
		{
			if (A[Role[0] + 1][Role[1]] == '0')
				A[Role[0] + 1][Role[1]] = '3';
			else
				A[Role[0] + 1][Role[1]] = '6';
			if (m == 1)
				if (A[Role[0] - 1][Role[1]] == '2')
					A[Role[0] - 1][Role[1]] = '0';
				else
					A[Role[0] - 1][Role[1]] = '4';
		}
		if (k == 77)
		{
			if (A[Role[0]][Role[1] - 1] == '0')
				A[Role[0]][Role[1] - 1] = '3';
			else
				A[Role[0]][Role[1] - 1] = '6';
			if (m == 1)
				if (A[Role[0]][Role[1] + 1] == '2')
					A[Role[0]][Role[1] + 1] = '0';
				else
					A[Role[0]][Role[1] + 1] = '4';
		}
		if (k == 80)
		{
			if (A[Role[0] - 1][Role[1]] == '0')
				A[Role[0] - 1][Role[1]] = '3';
			else
				A[Role[0] - 1][Role[1]] = '6';
			if (m == 1)
				if (A[Role[0] + 1][Role[1]] == '2')
					A[Role[0] + 1][Role[1]] = '0';
				else
					A[Role[0] + 1][Role[1]] = '4';
		}
		if (m == 1)
			if (A[Role[0]][Role[1]] == '3')
				A[Role[0]][Role[1]] = '2';
			else
				A[Role[0]][Role[1]] = '5';
		else
			if (A[Role[0]][Role[1]] == '3')
				A[Role[0]][Role[1]] = '0';
			else
				A[Role[0]][Role[1]] = '4';
	}
	void PathOut(char** AA, int acco1, path * pa)
	{
		no = bo = 0; int num = -1; path* start = pa;
		for (int i = 0; i < MAXV[0]; i++)
			delete[] A[i];
		delete[] A;
		A = AA; 
		for (int i = 1; i < MAXV[0] - 1; i++)
			for (int j = 1; j < MAXV[1] - 1; j++)
				if (A[i][j] == '3' || A[i][j] == '6')
				{
					Role[0] = i; Role[1] = j; goto T;
				}
	T:  cout << "   已找到路径，D键下一步"; char ch;
			while (ch = _getch())
		{
			if (ch == 'a')
			{
				if (num == 0 || num == -1)
				{
					path* pa1 = pa; pa = start;
					if (pa != pa1)
					{
						if (pa->next != NULL)
						{
							while (pa->next != pa1)
								pa = pa->next;
							num = 99;
						}
					}
					else 	continue;
				}
				else
					num--;
				LastStep(pa->loc[(num + 1) % 100][0], 1);
				Role[0] = pa->loc[num + 1][1]; Role[1] = pa->loc[num + 1][2];
				if (A[Role[0]][Role[1]] == '3')
					A[Role[0]][Role[1]] = '0';
				else
					A[Role[0]][Role[1]] = '4';
				Role[0] = pa->loc[num][1]; Role[1] = pa->loc[num][2];
				if (A[Role[0]][Role[1]] == '0')
					A[Role[0]][Role[1]] = '3';
				else
					A[Role[0]][Role[1]] = '6';
				Move(pa->loc[num][0]);
				Graph();
				cout << "  D键下一步,A键上一步 ";
			}
			if (ch == 'd')
				if (num < acco1 % 100 - 1 || pa->next != NULL)
				{
					num++;
					if (num == 100)
					{
						num = 0; pa = pa->next;
					}
					if (A[Role[0]][Role[1]] == '3')
						A[Role[0]][Role[1]] = '0';
					else
						A[Role[0]][Role[1]] = '4';
					Role[0] = pa->loc[num][1]; Role[1] = pa->loc[num][2];
					if (A[Role[0]][Role[1]] == '0')
						A[Role[0]][Role[1]] = '3';
					else
						A[Role[0]][Role[1]] = '6';
					Move(pa->loc[num][0]);
					Graph();
					cout << "  D键下一步,A键上一步 ";
				}
			if (ch == 27)
			{
				Menu();
				cout << "  D键下一步,A键上一步 ";
			}
		}
	}

	void LastStep()
	{
		if (no == 0)
			if (bo == 0)
				return;
			else
				no = 100;
		no--;
		if (no != bo - 1)
		{
			if (Step[no][0] == 75)
			{
				if (A[Role[0]][Role[1] + 1] == '0')
					A[Role[0]][Role[1] + 1] = '3';
				else
					A[Role[0]][Role[1] + 1] = '6';
				if (Step[no][1] == 1)
					if (A[Role[0]][Role[1] - 1] == '2')
						A[Role[0]][Role[1] - 1] = '0';
					else
						A[Role[0]][Role[1] - 1] = '4';
			}
			if (Step[no][0] == 72)
			{
				if (A[Role[0] + 1][Role[1]] == '0')
					A[Role[0] + 1][Role[1]] = '3';
				else
					A[Role[0] + 1][Role[1]] = '6';
				if (Step[no][1] == 1)
					if (A[Role[0] - 1][Role[1]] == '2')
						A[Role[0] - 1][Role[1]] = '0';
					else
						A[Role[0] - 1][Role[1]] = '4';
			}
			if (Step[no][0] == 77)
			{
				if (A[Role[0]][Role[1] - 1] == '0')
					A[Role[0]][Role[1] - 1] = '3';
				else
					A[Role[0]][Role[1] - 1] = '6';
				if (Step[no][1] == 1)
					if (A[Role[0]][Role[1] + 1] == '2')
						A[Role[0]][Role[1] + 1] = '0';
					else
						A[Role[0]][Role[1] + 1] = '4';
			}
			if (Step[no][0] == 80)
			{
				if (A[Role[0] - 1][Role[1]] == '0')
					A[Role[0] - 1][Role[1]] = '3';
				else
					A[Role[0] - 1][Role[1]] = '6';
				if (Step[no][1] == 1)
					if (A[Role[0] + 1][Role[1]] == '2')
						A[Role[0] + 1][Role[1]] = '0';
					else
						A[Role[0] + 1][Role[1]] = '4';
			}
			if (Step[no][1] == 1)
				if (A[Role[0]][Role[1]] == '3')
					A[Role[0]][Role[1]] = '2';
				else
					A[Role[0]][Role[1]] = '5';
			else
				if (A[Role[0]][Role[1]] == '3')
					A[Role[0]][Role[1]] = '0';
				else
					A[Role[0]][Role[1]] = '4';
			Play();
		}
		else
			no = (no + 1) % 100;
	}
	int Role[2]; int acco, bcount, boac;
};

Game st;

void initialize(int i)            //初始化
{
	st.goal = no = bo = 0;
	char road[70] = "D:\\Microsoft Visual Studio\\Procejt\\tuixiangzi\\pass\\pass";     //文件路径  
	int  m = 0, n = 0;
	if (i == 100)
		i = 0;
	road[55] = i / 10 + '0'; road[56] = i % 10 + '0'; road[57] = '\0';
	ifstream pass(road);
	do                              //第一行
	{
		pass >> i; road[m] = i + '0'; m++;
	} while (i != 6);
	MAXV[1] = m - 1; A = new char* [MAXV[1]]; A[0] = new char[MAXV[1]]; m = 1;
	for (i = 0; i < MAXV[1]; i++)
		A[0][i] = road[i];
	while (pass >> i)                //第一行后
	{
		if (n == 0)
			A[m] = new char[MAXV[1]];
		A[m][n] = i + '0';
		if (i == 4 || i == 5 || i == 6)
			st.goal++;
		if (i == 2 || i == 5)
			no++;
		n++;
		if (n == MAXV[1])
		{
			m++;
			n = 0;
			pass >> i;
		}
	}
	if (st.goal != no)
	{
		cout << "地图有误 请检查  " << np;
		exit(0);
	}
	MAXV[0] = m; no = 0;
	pass.close();
}

void Pass()
{
	system("cls");
	cout << "关卡选择（0--100）  "; cin >> np;
	initialize(np);
	Play();
}

void Play()
{
	st.Game::Graph();
	for (;; st.Game::MoveGrid());
}
