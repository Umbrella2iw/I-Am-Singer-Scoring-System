#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

#define WIDTH 108 // �������̨��� 
#define HEIGHT 39 // �������̨�߶� 
#define MAX_SINGERS 100 // ���ó�ʼ������������� 

// ��������ṹ�� 
struct Song
{
	char name[5]; // �������ƣ�4���ֽ� 
	int judgerScores[5]; // ��λ��ί�Ĵ��
	int audienceScore; // ���ڴ��
	int totalScore; // �ܷ� 
};
typedef struct Song Song;// ���¶�������ṹ���������ʽ�����ӱ�ݡ� 
// ������ֽṹ�� 
struct Singer
{
	int id; // ���ֵı�� 
	char name[5]; // ���ֵ����� 
	Song song; // ���������ĸ��� 
};
typedef struct Singer Singer;// ���¶�����ֽṹ���������ʽ�����ӱ�ݡ� 
// �������״̬ö�� 
enum State
{
	MainMenu, // ���������˵� 
	DisplayMessage, // ��������ʾ��Ϣ���� 
	InputData, // ���������������Ϣ 
	DisplaySingerData, // ��������ʾ������Ϣ 
	InsertData, // ��������Ӹ�����Ϣ
	DeleteData, // ������ɾ��������Ϣ 
} curState = MainMenu, prevState = MainMenu; // ���嵱ǰ����״̬��������һ������״̬ 
typedef enum State State;// ���¶���״̬ö�ٵ�������ʽ�����ӱ�ݡ� 

// ��������õ�ö������ 
enum Direction
{
	HorizontalLeft, // ����������� 
	HorizontalRight, // ����������� 
	VerticalUp, // ����������� 
	VerticalDown // ����������� 
};
typedef enum Direction Direction; // ����������ö�� ʹ��ʱ���ӷ��� 

HANDLE hOut; // ��������̨�������
// �����˵�ѡ����ı��浽һ�������У���ѭ������ʾ���˵������Ӽ�� 
char menuChoicesStr[][15] = {"1.���������Ϣ", "2.���������Ϣ", "3.�ɼ�����", "4.���������Ϣ", "5.��Ӹ�����Ϣ", "6.��ѯ������Ϣ", "7.ɾ��������Ϣ", "8.�˳�ϵͳ"};

Singer singers[MAX_SINGERS]; // �����������飬��MAX_SINGERS������ʼ������Ĵ�С����������ô�С�������������ڴ�ռ䣬��֤��������Խ�硣 
int singerAmount = 0; // ��ǰ�Ѿ�����¼�ĸ�������  
int curPage = 0, maxPage = 0;// ��ʾ������Ϣ��ʱ���õ��ı������ֱ��ʾ��ǰ�ǵڼ�ҳ���ܹ��ж���ҳ����Ϊ���ֿ��ܺܶ࣬һ����ʾ��ȫ�����Բ��÷�ҳ 

void initSystem();// ��ʼ��ϵͳ�趨�����ÿ���̨��С
void displayMenu();// ��ʾϵͳ���˵�
void inputSingerData();// ¼���������
void displaySingerInfo();// ��ʾ�������� 
void displayMessage(char* message);// ������ת����ʾ��Ϣ���ڣ���ʾһ����Ϣ������Ϊmessage 
void insertSingerInfo(); // ��Ӹ�����Ϣ 
void searchSingerInfo(); // ��ѯ������Ϣ 
void deleteSingerInfo(); // ɾ��������Ϣ 
void toCurState(); // �ó���ת����ǰ��״̬���ڵĻ��� 
void sortSingers(); // �����еĸ�����Ϣ�������� 
void saveSingersInfo(); // ������ֵ���Ϣ 
void sendWarn(char* warnMsg); // ����һ�����棬�����������˵� 
int checkSingerExists(int id); // Ѱ��һ��ָ����ŵĸ����Ƿ������ݿ��� ��������򷵻��������ݿ��е����� �������򷵻�-1 
int getMin(int a, int b);// �ȴ�С����������С����һ�� 

void setCursorVisible(int visible);// ���ÿ���̨�Ĺ���Ƿ�ɼ� 
void setCursorPosition(int x, int y);// ���ÿ���̨�������� 

void drawBorder();// �����������ı߿� 
void drawWindow(int x, int y, int width, int height);// �ڿ���̨�ϻ���һ������
void drawText(int x, int y, char* text);// �Դ������ҵ�˳����(x, y)�����һ���ı�text
void drawChar(int x, int y, char c);// ��(x, y)�����һ���ַ�c 
void drawLine(int x, int y, Direction direction, int len, char c);// ��(x, y)����������������len����ͬ���ַ� 
void drawGridHeader(); // �����ı�ͷ���Ƶ�����̨�� 
void drawGridSingerInfo(int row, Singer singer); // �����ֵ�ȫ����Ϣ���������� 

int main()
{
	initSystem();
	displayMenu();
	return 0;
}
// ��ʾ���˵� 
void displayMenu()
{
	// ÿ�λص����˵� ��������и��ֵ��������¼����ܹ��ж���ҳ 
	maxPage = singerAmount / 17 + 1;
	int offset = 13;
	drawBorder();
	drawWindow(24 + offset, 2, 35, 3);
	drawText(32 + offset, 3, "���Ǹ��ֱ������ϵͳ");
	// ���7�����ť 
	for (int i = 0; i < 8; i++)
	{
		drawWindow(24 + offset, 6 + 4 * i, 35, 3);
		drawText(34 + offset, 7 + 4 * i, menuChoicesStr[i]);
	}
	// ��ȡ�û����� 
	char c = getch();
	switch (c)
	{
		case '1':
			prevState = MainMenu;
			curState = InputData;
			inputSingerData();
			break;
		case '2':
			prevState = MainMenu;
			curState = DisplaySingerData;
			displaySingerInfo();
			break;
		case '3':
			if (singerAmount > 0)
			{
				sortSingers();
				prevState = MainMenu;
				displayMessage("����ɹ���");
			}
			else
			{
				prevState = MainMenu;
				displayMessage("��ǰû�и�����Ϣ��");
			}
			break;
		case '4':
			if (singerAmount > 0)
			{
				saveSingersInfo();
				prevState = MainMenu;
				displayMessage("����ɹ���");
			}
			else
			{
				prevState = MainMenu;
				displayMessage("��ǰû�и�����Ϣ��");
			}
			break;
		case '5':
			insertSingerInfo();
			break;
		case '6':
			if (singerAmount > 0)
			{
				searchSingerInfo();
			}
			else
			{
				prevState = MainMenu;
				displayMessage("��ǰû�и�����Ϣ��"); 
			}
			break;
		case '7':
			if (singerAmount > 0)
			{
				deleteSingerInfo();
			}
			else
			{
				prevState = MainMenu;
				displayMessage("��ǰû�и�����Ϣ��"); 
			}
			break;
		case '8':
			break;
		default:
			displayMenu();
	}
}
// ���������Ϣ 
void inputSingerData()
{
	drawBorder();
	printf("��������ֵ�������");
	setCursorVisible(1);
	scanf("%d", &singerAmount);
	// ���������Ƿ�Ϸ� 
	if (singerAmount <= 0)
	{
		prevState = MainMenu;
		displayMessage("����ĸ�������ֻ��Ϊ��������");
	}
	else if (singerAmount >= MAX_SINGERS)
	{
		prevState = MainMenu;
		char tmp[30];
		sprintf(tmp, "һ������ĸ����������ܴ���%d��", MAX_SINGERS);
		displayMessage(tmp);
	}
	else
	{
		for (int i = 0; i < singerAmount; i++)
		{
			int _ok = 0;
			// �������������������ݶ��Ϸ����������һ��ѭ���������Ҫ����������ø��ֵ�����һ�� 
			while (!_ok)
			{
				_ok = 1;
				int cnt = 0;
				int _id, _audScore, _judgerScores[5];
				char _name[5] = {'\0'}, _songName[5] = {'\0'};
				drawBorder();
				setCursorVisible(1);
				printf("�������%dλ���ֵı�ţ�������������4���ֽڣ���������Ŀ���ƣ�������4���ֽڣ���������ίͶƱ������λרҵ��ί", i + 1);
				setCursorPosition(1, 2);
				printf("��֣�");
				scanf("%d %s %s %d", &_id, _name, _songName, &_audScore);
				cnt += _audScore;
				for (int j = 0; j < 5; j++)
				{
					scanf("%d", &_judgerScores[j]);
					cnt += _judgerScores[j];
				}
				cnt /= 6;
				if (_id <= 0)
				{
					sendWarn("��ű���Ϊ��������");
					_ok = 0;
				}
				else if (_audScore < 0 || _audScore > 100)
				{
					sendWarn("����ͶƱ����Ϊ0~100��������");
					_ok = 0;
				}
				else
				{
					for (int j = 0; j < 5; j++)
					{
						if (_judgerScores[j] < 0 || _judgerScores[j] > 100)
						{
							char tmpWarn[50];
							sprintf(tmpWarn, "��%dλ��ί�ķ������󣡷���������0~100֮�䣬��Ϊ������", j + 1);
							sendWarn(tmpWarn);
							_ok = 0;
							break;
						}
					}
				}
				if (!_ok)
				{
					continue;
				}
				// ������ݺϷ� �����ݸ�ֵ������ 
				singers[i].id = _id;
				singers[i].song.audienceScore = _audScore;
				singers[i].song.totalScore = cnt;
				strcpy(singers[i].name, _name);
				strcpy(singers[i].song.name, _songName);
				for (int j = 0; j < 5; j++)
				{
					singers[i].song.judgerScores[j] = _judgerScores[j];
				}
			}
		}
		// �������˵� 
		prevState = MainMenu;
		displayMessage("������ϣ�"); 
	}
}
// ��ʾ������Ϣ 
void displaySingerInfo()
{
	drawBorder();
	// �����и��������ݿ������ʾ��Ϣ��������ʾ 
	if (singerAmount > 0)
	{
		// ���Ʊ�ͷ 
		drawGridHeader();
		// ���Ƹ�����Ϣ 
		for (int i = 17 * curPage; i < singerAmount; i++)
		{
			drawGridSingerInfo(i, singers[i]);
		}
		char tmp[50];
		sprintf(tmp, "���ҷ������ҳ����ǰ��%d/%dҳ��������������������", curPage + 1, maxPage);
		drawText(WIDTH / 2 - 24, HEIGHT - 1, tmp);
		// ��ȡ�û�����������û����ڷ�����������»�ȡһ�ΰ���keyCode 
		int key = getch();
		if (key == 224)
		{
			key = getch();
			if (key == 75)
			{
				if (curPage - 1 >= 0)
				{
					curPage--;
				}
			}
			else if (key == 77)
			{
				if (curPage + 1 < maxPage)
				{
					curPage++;
				}
			}
			displaySingerInfo();
		}
		else
		{
			displayMenu();
		}
	}
	else
	{
		prevState = MainMenu;
		displayMessage("�������ݣ�");
	}
}
// �������и��ֵ���Ϣ��singer.dat�ļ��� 
void saveSingersInfo()
{
	FILE *filePtr;
	filePtr = fopen("singer.dat", "w");
	if (filePtr != NULL)
	{
		for (int i = 0; i < singerAmount; i++)
		{
			fprintf(filePtr, "%d %s %s %d ", singers[i].id, singers[i].name, singers[i].song.name, singers[i].song.audienceScore);
			for (int j = 0; j < 5; j++)
			{
				fprintf(filePtr, "%d ", singers[i].song.judgerScores[j]);
			}
			fprintf(filePtr, "%d\n", singers[i].song.totalScore);
		}
		fclose(filePtr);
	}
}
// ���һ���µĸ�����Ϣ 
void insertSingerInfo()
{
	int ok = 0;
	int cnt = 0;
	while (!ok)
	{
		int id, audScore, judgerScore[5];
		char name[5], songName[5];
		ok = 1;
		cnt = 0;
		drawBorder();
		setCursorVisible(1);
		printf("�����뱻��Ӹ��ֵı�ţ�������������4���ֽڣ���������Ŀ���ƣ�������4���ֽڣ���������ίͶƱ������λרҵ��ί");
		setCursorPosition(1, 2);
		printf("��֣�");
		scanf("%d", &id);
		if (checkSingerExists(id) > -1)
		{
			sendWarn("��Ӹ��ֵı�������и��ֵı���ظ���");
			ok = 0;
		}
		else
		{
			if (id < 0)
			{
				sendWarn("��ű���Ϊ��������");
				ok = 0;
			}
			else
			{
				scanf("%s %s %d", name, songName, &audScore);
				if (audScore < 0 || audScore > 100)
				{
					sendWarn("����ͶƱ����Ϊ0~100��������");
					ok = 0;
				}
				else
				{
					cnt += audScore;
					for (int i = 0; i < 5; i++)
					{
						scanf("%d", &judgerScore[i]);
						if (judgerScore[i] < 0 || judgerScore[i] > 100)
						{
							char tmpWarn[50];
							sprintf(tmpWarn, "��%dλ��ί�ķ������󣡷���������0~100֮�䣬��Ϊ������", i + 1);
							sendWarn(tmpWarn);
							ok = 0;
							break;
						}
						cnt += judgerScore[i];
					}
					if (!ok)
					{
						continue;
					}
					cnt /= 6;
					singers[singerAmount].id = id;
					singers[singerAmount].song.audienceScore = audScore;
					strcpy(singers[singerAmount].name, name);
					strcpy(singers[singerAmount].song.name, songName);
					for (int i = 0; i < 5; i++)
					{
						singers[singerAmount].song.judgerScores[i] = judgerScore[i];
					}
					singers[singerAmount++].song.totalScore = cnt;
				}
			}
		}
	}
	prevState = MainMenu;
	displayMessage("�����ɣ�");
}
// ��ѯ������Ϣ 
void searchSingerInfo()
{
	drawBorder();
	setCursorVisible(1);
	printf("������Ҫ��ѯ�ĸ��ֱ�ţ�");
	int id, index;
	scanf("%d", &id);
	index = checkSingerExists(id);
	// ����鵽�ˣ�����ʾ���ֵ���Ϣ �鲻���ͷ��������� 
	if (index > -1)
	{
		drawBorder();
		drawGridHeader();
		drawGridSingerInfo(0, singers[index]); 
		drawText(WIDTH / 2 - 4, HEIGHT - 1, "�������������");
		getch();
		displayMenu();
	}
	else
	{
		prevState = MainMenu;
		displayMessage("���޸ñ�ŵĸ��֣�");
	}
}
// ɾ��������Ϣ 
void deleteSingerInfo()
{
	drawBorder();
	setCursorVisible(1);
	printf("������Ҫɾ���ĸ��ֵı�ţ�");
	int id, index;
	scanf("%d", &id);
	index = checkSingerExists(id);
	// ������ִ��� ��ɾ�� ���򷵻������� 
	if (index > -1)
	{
		// �øø���������������и��ֵ����ݶ���ǰ�ƶ�һ���洢��Ԫ �����ʹﵽ��ɾ�������� 
		for (int i = index; i < singerAmount; i++)
		{
			singers[i] = singers[i + 1];
		}
		singerAmount--;
		prevState = MainMenu;
		displayMessage("ɾ���ɹ���");
	}
	else
	{
		prevState = MainMenu;
		displayMessage("���ֲ����ڣ�");
	}
}
// ��ʾһ����Ϣ
// message: char* ������ʾ���ַ��� 
void displayMessage(char* message)
{
	drawBorder();
	int len = strlen(message) / 2;
	drawWindow(WIDTH / 2 - 32, 1, 65, 7);
	setCursorPosition(WIDTH / 2 - len, 3);
	puts(message);
	setCursorPosition(WIDTH / 2 - 7, 6);
	puts("�����������");
	getch();
	curState = prevState;
	prevState = DisplayMessage;
	toCurState();
}
// ������ת����ǰ��״̬���ڵĻ��� 
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
// �Ե�ǰ���еĸ�����Ϣ��������ʹ����ð�������㷨 
void sortSingers()
{
	for (int i = 0; i < singerAmount - 1; i++)
	{
		for (int j = 0; j < singerAmount - 1 - i; j++)
		{
			if (singers[j].song.totalScore < singers[j + 1].song.totalScore)
			{
				Singer tmp = singers[j];
				singers[j] = singers[j + 1];
				singers[j + 1] = tmp;
			}
		}
	}
}
// ��ʾһ�����棬������ص���һ������
// warnMsg: char* ���ھ�����ַ��� 
void sendWarn(char* warnMsg)
{
	drawBorder();
	int len = strlen(warnMsg) / 2;
	drawWindow(WIDTH / 2 - 32, 1, 65, 7);
	setCursorPosition(WIDTH / 2 - len, 3);
	puts(warnMsg);
	setCursorPosition(WIDTH / 2 - 7, 6);
	puts("�������������������");
	getch();
}
// �ж����ݿ����Ƿ����иñ�ŵĸ�����Ϣ��������򷵻������ݿ��е�λ�� 
// id: int Ҫ���Ҹ��ֵı�� 
int checkSingerExists(int id)
{
	int exists = -1;
	for (int i = 0; i < singerAmount; i++)
	{
		if (singers[i].id == id)
		{
			exists = i;
			break;
		}
	}
	return exists;
}
// ȡ���������н�С���Ǹ�
// a: int ����1
// b: int ����2 
int getMin(int a, int b)
{
	if (a < b)
	{
		return a;
	}
	else
	{
		return b;
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
// ���Ƴ���ı߿�ͬʱ���������ù���λ�� 
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
// ����������(x, y)�����һ���ı�������Ϊtext
// x: ����̨�ĵڼ���
// y: ����̨�ĵڼ���
// text: ��Ҫ������ַ��� 
void drawText(int x, int y, char* text)
{
	setCursorPosition(x, y);
	puts(text);
}
// ��(x, y)�����һ���ַ�
// x: ����̨�ĵڼ���
// y: ����̨�ĵڼ���
// c: ��Ҫ������ַ� 
void drawChar(int x, int y, char c)
{
	setCursorPosition(x, y);
	putchar(c);
}
// ��(x, y)����������������len���ַ�c
// x: ����̨����
// y: ����̨����
// direction: ����ķ���
// len: ������ַ�����
// c: Ҫ������ַ� 
void drawLine(int x, int y, Direction direction, int len, char c)
{
	setCursorPosition(x, y);
	for (int i = 0; i < len; i++)
	{
		switch (direction)
		{
			case HorizontalLeft:
				setCursorPosition(x - i, y);
				break;
			case HorizontalRight:
				setCursorPosition(x + i, y);
				break;
			case VerticalUp:
				setCursorPosition(x, y - i);
				break;
			case VerticalDown:
				setCursorPosition(x, y + i);
				break;
		}
		putchar(c);
	}
}
// ���Ʊ��ı�ͷ 
void drawGridHeader()
{
	// ���Ʊ��
	drawWindow(0, 0, 8, 3);
	drawText(2, 1, "���"); 
	drawWindow(7, 0, 12, 3);
	drawText(9, 1, "��������");
	drawWindow(18, 0, 12, 3);
	drawText(20, 1, "��Ŀ����");
	drawWindow(29, 0, 12, 3);
	drawText(31, 1, "��������");
	drawWindow(40, 0, 13, 3);
	drawText(42, 1, "��ί1�÷�");
	drawWindow(52, 0, 13, 3);
	drawText(54, 1, "��ί2�÷�");
	drawWindow(64, 0, 13, 3);
	drawText(66, 1, "��ί3�÷�");
	drawWindow(76, 0, 13, 3);
	drawText(78, 1, "��ί4�÷�");
	drawWindow(88, 0, 13, 3);
	drawText(90, 1, "��ί5�÷�");
	drawWindow(100, 0, 8, 3);
	drawText(102, 1, "�ܷ�");
}
// ��������Ϣ���Ƶ������
// row: int ��ʹ�õڼ��������������Ϣ��ÿ��֮����һ�зָ�
// singer: Singer ������ĸ�����Ϣ 
void drawGridSingerInfo(int row, Singer singer)
{
	int i = row;
	char tmp[20] = {0};
	Direction dir = HorizontalRight;
	sprintf(tmp, "%d", singer.id);
	setCursorPosition(1, 3 + 2 * i);
	drawText(2, 3 + 2 * i, tmp);
	drawChar(7, 3 + 2 * i, '*');
	strcpy(tmp, singer.name);
	drawText(9, 3 + 2 * i, tmp);
	drawChar(18, 3 + 2 * i, '*');
	strcpy(tmp, singer.song.name);
	drawText(20, 3 + 2 * i, tmp);
	drawChar(29, 3 + 2 * i, '*');
	sprintf(tmp, "%d", singer.song.audienceScore);
	drawText(31, 3 + 2 * i, tmp);
	drawChar(40, 3 + 2 * i, '*');
	for (int j = 0; j < 5; j++)
	{
		sprintf(tmp, "%d", singer.song.judgerScores[i]);
		drawText(42 + 12 * j, 3 + 2 * i, tmp);
		drawChar(52 + 12 * j, 3 + 2 * i, '*');
	}
	sprintf(tmp, "%d", singer.song.totalScore);
	drawText(102, 3 + 2 * i, tmp);
	drawLine(1, 3 + 2 * i + 1, dir, WIDTH - 2, '*');
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
