#include <stdio.h>

#define WIDTH 80
#define HEIGHT 20

struct Sing
{
	char name[10];
	double judgerScoreAvg;
	double audienceScoreAvg;
};
typedef struct Sing Sing;

struct Singer
{
	char name[5];
	Sing* sings;
	int singAmount;
};
typedef struct Singer Singer;

struct Position
{
	int x, y;
};
typedef struct Position Position;

void initSystem();// ��ʼ��ϵͳ�趨�����ÿ���̨��С 
void displayMenu();// ��ʾϵͳ���˵� 

void drawWindow(Position pos, Position size);

int main()
{
	initSystem();
	displayMenu();
	return 0;
}

void displayMenu()
{
	
}

void initSystem()
{
	char tmp[50];
	sprintf(tmp, "mode con cols=%d lines=%d", WIDTH, HEIGHT);
	system(tmp);
}

void drawWindow(Position pos, Position size)
{
	
}
