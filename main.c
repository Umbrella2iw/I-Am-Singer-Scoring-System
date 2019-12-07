#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

#define WIDTH 80
#define HEIGHT 31

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

enum State
{
	MainMenu,
	DisplayMessage,
	InputData,
	DisplaySingerData,
	DisplaySortingResult
} curState = MainMenu, prevState = MainMenu;
typedef enum State State;

HANDLE hOut;

Singer *singers, *sortedSingers;
int singerAmount = 0;

char messages[100];

void initSystem();// ��ʼ��ϵͳ�趨�����ÿ���̨��С
void displayMenu();// ��ʾϵͳ���˵�
void inputSingerData();// ¼���������
void displaySingerData();// ��ʾ�������� 
void displayMessage(char* message);
void toCurState();

void setCursorVisible(int visible);
void setCursorPosition(int x, int y);

void drawBorder();// �����������ı߿� 
void drawWindow(int x, int y, int width, int height);// �ڿ���̨�ϻ���һ������
void drawText(int x, int y, char* text);

int main()
{
	initSystem();
	displayMenu();
	return 0;
}
// ��ʾ���˵� 
void displayMenu()
{
	drawBorder();
	drawWindow(24, 2, 35, 3);
	drawText(32, 3, "���Ǹ��ֱ������ϵͳ");
	drawWindow(26, 6, 30, 3);
	drawText(36, 7, "1.������Ϣ");
	drawWindow(26, 10, 30, 3);
	drawText(36, 11, "2.��ʾ��Ϣ");
	drawWindow(26, 14, 30, 3);
	drawText(36, 15, "3.�������");
	drawWindow(26, 18, 30, 3);
	drawText(36, 19, "4.������Ϣ");
	drawWindow(26, 22, 30, 3);
	drawText(36, 23, "5.��ʾ���");
	drawWindow(26, 26, 30, 3);
	drawText(36, 27, "6.�˳�ϵͳ");
	char c = getch();
	switch (c)
	{
		case '1':
			prevState = MainMenu;
			curState = InputData;
			inputSingerData();
			break;
	}
}
void inputSingerData()
{
	drawBorder();
	printf("��������ֵ�������");
	setCursorVisible(1);
	scanf("%d", &singerAmount);
	if (singerAmount <= 0)
	{
		prevState = MainMenu;
		displayMessage("��������ֻ��Ϊ��������");
	}
	else if (singerAmount >= 20)
	{
		prevState = MainMenu;
		displayMessage("�����������ܴ���20��");
	}
	else
	{
		singers = (Singer*)calloc(singerAmount, sizeof(Singer));
		sortedSingers = (Singer*)calloc(singerAmount, sizeof(Singer));
		drawBorder();
		printf("��������������ĸ�������");
		setCursorVisible(1);
		int num;
		scanf("%d", &num);
		if (num <= 0)
		{
			prevState = MainMenu;
			displayMessage("������ֻ��Ϊ��������");
		}
		else if (num > 10)
		{
			prevState = MainMenu;
			displayMessage("���������ܳ���10�ף�");
		}
		else
		{
			for (int i = 0; i < singerAmount; i++)
			{
				drawBorder();
				setCursorVisible(1);
				singers[i].sings = (Sing*)calloc(num, sizeof(Sing));
				singers[i].singAmount = num;
				sortedSingers[i].sings = (Sing*)calloc(num, sizeof(Sing));
				sortedSingers[i].singAmount = num;
				printf("�������%dλ���ֵ����֣�������5���ַ�����", i + 1);
				scanf("%s", singers[i].name);
				strcpy(sortedSingers[i].name, singers[i].name);
				for (int j = 0; j < num; j++)
				{
					drawBorder();
					setCursorVisible(1);
					printf("�������%d�׸��������ƣ���ί���ƽ���ֺʹ������֣��ÿո������", j + 1);
					scanf("%s %lf %lf", singers[i].sings[j].name, &(singers[i].sings[j].judgerScoreAvg), &(singers[i].sings[j].audienceScoreAvg));
					strcpy(sortedSingers[i].sings[j].name, singers[i].sings[j].name);
					sortedSingers[i].sings[j].judgerScoreAvg = singers[i].sings[j].judgerScoreAvg;
					sortedSingers[i].sings[j].audienceScoreAvg = singers[i].sings[j].audienceScoreAvg;
				}
			}
			displayMessage("������ϣ�"); 
		}
	}
}
void displaySingerInfo()
{
	
}
// ��ʾһ����Ϣ
// message: char* ������ʾ���ַ��� 
void displayMessage(char* message)
{
	drawBorder();
	int len = strlen(message) / 2;
	setCursorPosition(WIDTH / 2 - len, 2);
	printf("%s", message);
	setCursorPosition(WIDTH / 2 - 3, 4);
	printf("�����������");
	getch();
	curState = prevState;
	prevState = DisplayMessage;
	toCurState();
}
void toCurState()
{
	switch (curState)
	{
		case MainMenu:
			displayMenu();
			break;
		case DisplayMessage:
			displayMessage("");
			break;
		case InputData:
			inputSingerData();
			break;
	}
}
// ��ʼ��ϵͳ���� 
void initSystem()
{
	char tmp[50];
	sprintf(tmp, "mode con cols=%d lines=%d", WIDTH, HEIGHT + 1);
	system(tmp);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	setCursorVisible(0);
}
void drawBorder()
{
	system("CLS");
	setCursorVisible(0);
	drawWindow(0, 0, WIDTH, HEIGHT);
}
// �ڿ���̨�����ϻ���һ������
// x: int ��x���꿪ʼ���һ���
// y: int ��y���꿪ʼ���»���
// width: int ���ƵĴ��ڵĿ��
// height: int ���ƵĴ��ڵĸ߶� 
void drawWindow(int x, int y, int width, int height)
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < width; j++)
		{
			setCursorPosition(x + j, y + (height - 1) * i);
			printf("*");
		}
		for (int j = 1; j < height - 1; j++)
		{
			setCursorPosition(x + (width - 1) * i, y + j);
			printf("*");
		}
	}
	setCursorPosition(1, 1);
}
void drawText(int x, int y, char* text)
{
	setCursorPosition(x, y);
	printf(text);
}
// �������ú���SetConsoleCursorInfo��һ����װ
// visible: int �Ƿ���ʾ�����꣬1Ϊ��ʾ��0Ϊ����ʾ 
void setCursorVisible(int visible)
{
	CONSOLE_CURSOR_INFO info;
	GetConsoleCursorInfo(hOut, &info);
	info.bVisible = visible;
	SetConsoleCursorInfo(hOut, &info);
}
// �������ú���SetConsoleCursorPosition��һ����װ
// position: COORD ���ù���ڿ���̨�ϵ����� 
void setCursorPosition(int x, int y)
{
	COORD tempCoord = {x, y};
	SetConsoleCursorPosition(hOut, tempCoord);
}
