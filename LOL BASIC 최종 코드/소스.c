#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <malloc.h>
#include <conio.h>
#include <time.h>
#include <mmsystem.h>

#include"resource.h"

#pragma comment(lib, "winmm.lib")

#define TRUE	    1
#define FALSE	    0

//�����Ǻ� �迭�� �ε���
#define TOP         0
#define JUNGLE      1
#define MID         2
#define AD_CARRY    3
#define SUPPORTER   4

//������ ����
#define LineMAX     5

//�� è�Ǿ� ����
#define Champ_num   144

//�����Ǻ� è�Ǿ� ���� - > è�� ����+1
#define TopMax      41
#define MidMax      33
#define JungleMax   31
#define AdMax       18
#define SupMax      26
#define AllMax      145

//���� ����
#define qMAX        42

// è�Ǿ� �̸� �ִ� ����
#define MAX_NAME    20

// color��� ���� 
#define BLACK 0 
#define BLUE 1 
#define GREEN 2 
#define CYAN 3 
#define RED 4 
#define MAGENTA 5 
#define BROWN 6 
#define LIGHTGRAY 7 
#define DARKGRAY 8 
#define LIGHTBLUE 9 
#define LIGHTGREEN 10 
#define LIGHTCYAN 11 
#define LIGHTRED 12 
#define LIGHTMAGENTA 13 
#define YELLOW 14 
#define WHITE 15 

// �ܼ� ������ â�� �ڵ鰪
static HWND hWnd;
// ���α׷��� �ν��Ͻ� �ڵ鰪
static HINSTANCE hInst;

// Ʈ���� ����� ���� ����� ���Ḯ��Ʈ �ڷ���
typedef struct _node
{
	char name[MAX_NAME];           // è�Ǿ� �̸� or ����
	// è�Ǿ� ����
	int difficulty; // ���̵�
	// ����
	int assassin;
	int warrior;
	int wizard;

	int supporter;
	int tanker;
	int ad;
	//�·�
	double win_rate;

	struct _node * pnode;    // �θ� ���
	struct _node * cnode;    // �ڽ� ���
	struct _node * snode;    // ���� ���
	int child_num;           // �ڽ� ��� ����
} Node;

typedef struct _Tree
{
	Node * root;
} Tree;

//������ ��õ ���� ����ü
typedef struct _Qustion {
	char question[500];
	int answer[LineMAX];
}Question;

// 2.è�Ǿ� ��õ - ����� ����
typedef struct _play
{
	char name[Champ_num][MAX_NAME];
	int num_cham;

	int line;
	int difficulty; // ���̵�
	// ����
	int assassin;
	int warrior;
	int wizard;
	int supporter;
	int tanker;
	int ad;

} Player;

// --------------------------------------------------------
void Play_Game(void);
void PLAY_Rec_Line(void);
void delay(clock_t n);      //�ð� ����
//�̹��� ��� * nChamp�� è�Ǿ� �迭�� Ž���� ����
void Rendering(int x, int y, int nChamp);
void gotoxy(int x, int y);  //Ŀ���� ��ġ ����

// ----------------------------------------------------------

// Ʈ�� ADT
Node * createNode(char name[]);
void createTree(Tree * qtree, char name[]);
void createchild(Tree * qtree, char par_name[], char name[]);
Node * find_node(Node * node, char node_name[]);

// -------------------------------------------------------------

// LOL - ���� - è�Ǿ� : Ʈ�� ����
void SettingTree(Tree * qtree);
void SettingTop(Tree * qtree);
void SettingJungle(Tree * qtree);
void SettingMid(Tree * qtree);
void SettingAD(Tree * qtree);
void SettingSupport(Tree * qtree);
void SettingRate(Tree * qtree);

//-------------------------------------------------------------------

// 1.������ ��õ ( �迭�� �ؽ�Ʈ ���� ���)

void add_question(char Q_list1[][200], char Q_list2[][200], char Q_list3[][200], char Q_list4[][200], char Q_list5[][200], char Q_list6[][200], char Q_list7[][200], char Q_list8[][200], char Q_list9[][200]);
void add_answer(Question Q_list[]);
void add_line(Question Q, int Line_rec[]);
void play_question(Question Q_list[], int Line_rec[], char Q_list1[][200], char Q_list2[][200], char Q_list3[][200], char Q_list4[][200], char Q_list5[][200], char Q_list6[][200], char Q_list7[][200], char Q_list8[][200], char Q_list9[][200]);
void display_top_trait();
void display_mid_trait();
void display_jungle_trait();
void display_ad_carry_trait();
void display_supporter_trait();
void display_position_trait();
void more_trait();
void display_result(int idx);
int get_result(int Line_rec[]);
void PLAY_Rec_Line(void);

// -----------------------------------------------------------

// 2. è�Ǿ� ��õ ���
void recommand(Tree * qtree);

Player * init_player();                // ����� ����ü �ʱ�ȭ
void select_diff(Player * player);     // ���̵� ����
void select_line(Player * player);     // ���� ����
void select_work(Player * player);     // ���ұ� ����
void rec_display();                    // è�Ǿ� ��õ ���÷���
void player_data(Player * player);     // ����� ���� ���� ���
void recommand_champ(Player * player); // ��õ è�Ǿ� ���

// ��õ è���� ����� è�� �迭�� ����
void save_cham(Tree * qtree, Player * player);

// -------------------------------------------------------------

// 3. è�Ǿ� ���丮 �� ����
void Story_Strategy();

void Story_Strategy_display();      // ���丮 �� ���� ���÷���
void story_display();               // 2) ���丮 ���÷���
void print_story();                 // 2) ���丮 ����Ʈ ����
void strategy_display();            // 3) ���� ���÷���
void print_strategy();              // 3) ���� ����Ʈ ����

// -------------------------------------------------------------

// 4. ���� ��Ÿ �м�
void current_meta(Tree * LOL);

void meta_display();                 // ��Ÿ �м� ���÷���
void win_rate(Tree * LOL);           // 1) �·� ������
void all_win_rate();                 // 1-1) ��ü �·� ���
void print_line_rate(Node * pnode);  // 1-2) ���κ� �·� ����
void line_win_rate(Tree * LOL);      // 1-2) ���κ� �·� ���
void pick_rate();                    // 2) �ȷ� ���
void ban_rate();                     // 3) ��� ���

// -------------------------------------------------------------

// 5. 1��1 ���� ���� ( �迭�� �ؽ�Ʈ ���� ���)

// ��� è�Ǿ� �迭 ����
void make_champion_array(char arr[][20]);
void Play_Game(Tree * LOL);

// ------------------------------------------------------------

//�ð� ����
void delay(clock_t n)
{
	clock_t start = clock();
	while (clock() - start < n);
}

//�̹��� ��� * nChamp�� è�Ǿ� �迭�� Ž���� ����
void Rendering(int x, int y, int nChamp)
{
	// DC�� �ڵ鰪�� ���� ������ �����Ѵ�.(hDC : ����ȭ��DC, hMemDC : �̹����� ���� DC)
	// Device Context�� �׷��ȿ� �ʿ��� ��� �ɼ��� �־�� ����ü��� �� �� �ִ�.
	// �׸��׸��� �׸��� �׸� ȭ���̶� ����ȴ�.
	HDC hDC, hMemDC;
	// �ĸ�����̴�. static ������ ���������μ�, �޸��� Data������ ����Ǵ� �Լ��� ������ ������� �ʰ� �޸𸮿� �����ִ� �����̴�.
	static HDC hBackDC;
	// �̹��� ��Ʈ���� �ڵ鰪�� ������ �����̴�.
	HBITMAP hBitmap, hOldBitmap, hBackBitmap;
	// �ڵ鰪���κ��� ���� ���� ��Ʈ�� ����ü.
	BITMAP Bitmap;
	// ���� �������� Rect��(ũ��)�� ���´�. Rect�� ������, ������ �Ʒ��� ���� ������ ���簢���� ��Ÿ���� ����ü�̴�.
	RECT WindowRect;
	GetWindowRect(hWnd, &WindowRect);

	// ���� �������� DC �ڵ鰪�� ���´�. GetWindowDC(hWnd)�� �����ϴ�.
	hDC = GetDC(hWnd);
	// hDC�� ȣȯ�Ǵ� DC�� �޸𸮿� ����� �ڵ鰪�� ��ȯ�Ѵ�.
	hBackDC = CreateCompatibleDC(hDC);
	hMemDC = CreateCompatibleDC(hDC);

	// ��Ʈ�� �޸𸮸� �Ҵ��ϰ� �ڵ��� ��ȯ�Ѵ�.
	hBackBitmap = CreateCompatibleBitmap(hDC, WindowRect.right, WindowRect.bottom);
	// �׸� ��ȭ���� �غ��Ѵ�.
	hOldBitmap = (HBITMAP)SelectObject(hBackDC, hBackBitmap);
	// ��Ʈ���� �ε��Ͽ� �ڵ��� ��ȯ�Ѵ�. resource.h�� ������ define�Ǿ��ִ� �ҷ��� ���ҽ��� �ε��Ѵ�.
	hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(nChamp));//IDE 1 == 101////////////////////////////////////
	// ũ�⸦ �ޱ����� ��Ʈ�� ����ü�� ��Ʈ�� �ڵ�κ��� �����Ѵ�.
	GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
	// �о�� ��Ʈ�� ���ҽ��� �޸� DC�� �����Ѵ�.
	SelectObject(hMemDC, hBitmap);

	// hMemDC�� �̹����� hBackDC�� ���ϴ� ��ġ�� ��Ӻ����Ų��.(����Ϸ��� �̹����� �ĸ���ۿ� �����Ų��.)
	BitBlt(hBackDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, SRCCOPY);
	// hBackDC(�ĸ� ����)�� �ϼ��� �׸��� ȭ������ ��Ӻ����Ų��.
	BitBlt(hDC, x, y, x + Bitmap.bmWidth, y + Bitmap.bmHeight, hBackDC, 0, 0, SRCCOPY);///��µǴ� �̹����� ��ġ ����!!!!

	// �޸𸮿� ������Ʈ�� �����Ѵ�.
	DeleteObject(SelectObject(hBackDC, hBackBitmap));
	DeleteObject(hBitmap);
	DeleteDC(hBackDC);
	DeleteDC(hMemDC);

	ReleaseDC(hWnd, hDC);
}
// ui
void gotoxy(int x, int y);
void deletecursor();
void picture();

//�÷�
void textcolor(int foreground, int background);

// BGM
void StopSound();
void LoginSound();
void MainSound();


// -------------------------------------------------------------

// Ʈ�� ADT
Node * createNode(char name[])
{
	Node * newNode = (Node*)malloc(sizeof(Node));
	strncpy(newNode->name, name, sizeof(newNode->name));
	newNode->child_num = 0;
	newNode->cnode = NULL;
	newNode->pnode = NULL;
	newNode->snode = NULL;
	newNode->assassin = 0;
	newNode->supporter = 0;
	newNode->tanker = 0;
	newNode->warrior = 0;
	newNode->wizard = 0;
	newNode->ad = 0;
	newNode->difficulty = 0;
	newNode->win_rate = 0;
	return newNode;
}

void createTree(Tree * qtree, char name[])
{
	qtree->root = createNode(name);
}

void createchild(Tree * qtree, char par_name[], char name[])
{
	Node * par_node = find_node(qtree->root, par_name);
	if (par_node == NULL) return;

	if (par_node->child_num == 0) {
		par_node->cnode = createNode(name);
		par_node->cnode->pnode = par_node;
		par_node->child_num++;
	}
	else {
		Node * child_node = par_node->cnode;
		for (int i = 1; i < par_node->child_num; i++)
			child_node = child_node->snode;
		child_node->snode = createNode(name);
		child_node->snode->pnode = par_node;
		par_node->child_num++;
	}
}

Node * find_node(Node * node, char node_name[])
{
	if (node == NULL) return NULL;
	if (strcmp(node->name, node_name) == 0) {
		return node;
	}
	else {
		Node * finded = find_node(node->cnode, node_name);
		if (finded != NULL) return finded;
		finded = find_node(node->snode, node_name);
		if (finded != NULL) return finded;
	}
	return NULL;
}

//---------------------------------------------------------------

// LOL - ���� - è�Ǿ� : Ʈ�� ����
void SettingTree(Tree * qtree)
{
	createTree(qtree, "LOL");
	createchild(qtree, "LOL", "Top");
	createchild(qtree, "LOL", "Jungle");
	createchild(qtree, "LOL", "Mid");
	createchild(qtree, "LOL", "AD");
	createchild(qtree, "LOL", "Support");
	SettingTop(qtree);
	SettingJungle(qtree);
	SettingMid(qtree);
	SettingAD(qtree);
	SettingSupport(qtree);
}

void SettingTop(Tree * qtree)
{
	char champ_name[MAX_NAME];
	Node * cham_node;
	int diff[TopMax] = { 5,9,8,6,2,10,3,8,8,3,2,4,5,3,7,6,5,4,5,7,4,5,3,6,8,5,4,5,7,5,4,4,7,5,7,5,6,4,3,8 };
	int ass[TopMax] = { 0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,1,1,1,1,0 };
	int war[TopMax] = { 1,1,1,1,1,1,1,1,1,0,1,1,1,1,0,1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1,0,1,1,0 };
	int wiz[TopMax] = { 0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1 };
	int tan[TopMax] = { 1,0,1,1,1,0,1,0,0,1,1,0,1,1,0,1,1,1,1,0,1,1,1,1,1,0,1,0,0,1,1,0,0,0,1,0,0,0,0,0 };
	double win[TopMax] = { 52.03,50.81,49.85,50.31,50.14,51.82,50.06,48.71,50.59,50.96,48.40,48.81,50.37,51.66,49.48,50.49,50.47,50.46,48.40,45.86,51.16,50.32,48.00,53.92,50.36,48.25,51.56,51.17,49.11,49.66,50.05,50.31,51.05,51.07,53.14,49.29,50.23,50.45,50.68,49.54 };
	FILE * file_pointer = NULL;
	file_pointer = fopen("Top.txt", "r");
	for (int i = 0; i < TopMax; i++) {
		fgets(champ_name, MAX_NAME, file_pointer);
		createchild(qtree, "Top", champ_name);
		cham_node = find_node(qtree->root, champ_name);
		cham_node->difficulty = diff[i];
		cham_node->assassin = ass[i];
		cham_node->warrior = war[i];
		cham_node->wizard = wiz[i];
		cham_node->tanker = tan[i];
		cham_node->win_rate = win[i];
	}
	fclose(file_pointer);
}

void SettingJungle(Tree * qtree)
{
	char champ_name[MAX_NAME];
	Node * cham_node;
	int diff[JungleMax] = { 5,3,4,4,8,5,3,6,4,4,9,4,4,5,2,3,7,9,3,7,3,10,5,8,6,8,4,5,5,6 };
	int ass[JungleMax] = { 0,0,1,0,1,0,1,1,1,1,1,0,0,0,1,0,0,0,1,0,0,1,0,0,1,1,0,0,0,0 };
	int war[JungleMax] = { 1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,0,0,1,1,1,1,0,1,1,0,1,1,0,1,1 };
	int wiz[JungleMax] = { 1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0,1,0,0 };
	int sup[JungleMax] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0 };
	int tan[JungleMax] = { 0,0,0,1,0,1,0,0,0,0,0,1,1,1,0,1,0,0,0,1,1,0,1,1,0,0,0,0,1,1 };
	double win[JungleMax] = { 48.28,49.82,49.40,48.59,48.43,53.78,48.91,48.17,50.42,52.75,49.36,52.32,51.27,50.65,50.76,52.61,49.75,51.97,50.70,50.10,50.44,50.42,49.98,49.92,48.88,50.74,49.89,51.42,46.95,50.31 };
	FILE * file_pointer = NULL;
	file_pointer = fopen("Jungle.txt", "r");
	for (int i = 0; i < JungleMax; i++) {
		fgets(champ_name, MAX_NAME, file_pointer);
		createchild(qtree, "Jungle", champ_name);
		cham_node = find_node(qtree->root, champ_name);
		cham_node->difficulty = diff[i];
		cham_node->assassin = ass[i];
		cham_node->warrior = war[i];
		cham_node->wizard = wiz[i];
		cham_node->supporter = sup[i];
		cham_node->tanker = tan[i];
		cham_node->win_rate = win[i];
	}
	fclose(file_pointer);
}

void SettingMid(Tree * qtree)
{
	char champ_name[MAX_NAME];
	Node * cham_node;
	int diff[MidMax] = { 5,4,7,5,9,6,6,7,9,5,8,8,5,7,9,6,10,10,8,7,7,8,5,4,8,7,10,8,6,7,9,6 };
	int ass[MidMax] = { 0,0,0,0,1,0,1,0,0,1,0,0,1,0,0,0,0,1,1,0,1,0,0,0,1,0,0,1,0,1,0,1 };
	int war[MidMax] = { 0,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1 };
	int wiz[MidMax] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,1,1,1,1,1,1,1,0,0,1,0 };
	int sup[MidMax] = { 1,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0 };
	double win[MidMax] = { 50.94,49.42,45.82,51.76,47.83,48.24,53.38,50.06,50.14,48.44,50.82,47.17,52.35,51.51,48.27,50.39,50.82,49.83,51.02,49.83,50.69,51.39,47.95,50.74,51.16,50.56,51.29,49.68,51.86,50.21,49.50,51.87 };
	FILE * file_pointer = NULL;
	file_pointer = fopen("Mid.txt", "r");
	for (int i = 0; i < MidMax; i++) {
		fgets(champ_name, MAX_NAME, file_pointer);
		createchild(qtree, "Mid", champ_name);
		cham_node = find_node(qtree->root, champ_name);
		cham_node->difficulty = diff[i];
		cham_node->assassin = ass[i];
		cham_node->warrior = war[i];
		cham_node->wizard = wiz[i];
		cham_node->supporter = sup[i];
		cham_node->win_rate = win[i];
	}
	fclose(file_pointer);
}

void SettingAD(Tree * qtree)
{
	char champ_name[MAX_NAME];
	Node * cham_node;
	int diff[AdMax] = { 8,6,1,2,8,4,4,7,5,6,6,6,7,6,6,4,6 };
	int ass[AdMax] = { 0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1 };
	double win[AdMax] = { 50.72,48.01,49.40,48.86,51.04,54.12,51.05,48.55,51.08,49.55,53.29,49.37,46.57,48.52,51.92,49.02,52.52 };
	FILE * file_pointer = NULL;
	file_pointer = fopen("AD.txt", "r");
	for (int i = 0; i < AdMax; i++) {
		fgets(champ_name, MAX_NAME, file_pointer);
		createchild(qtree, "AD", champ_name);
		cham_node = find_node(qtree->root, champ_name);
		cham_node->difficulty = diff[i];
		cham_node->assassin = ass[i];
		cham_node->ad = 1;
		cham_node->win_rate = win[i];
	}
	fclose(file_pointer);
}

void SettingSupport(Tree * qtree)
{
	char champ_name[MAX_NAME];
	Node * cham_node;
	int diff[SupMax] = { 5,5,6,5,4,5,1,9,8,3,4,4,4,3,7,7,2,7,7,6,5,3,5,7,9 };
	int ass[SupMax] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0 };
	int war[SupMax] = { 0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0 };
	int wiz[SupMax] = { 1,1,0,0,0,1,1,1,1,0,1,0,1,1,0,0,1,1,1,1,1,0,0,0,1 };
	int tan[SupMax] = { 1,0,1,0,1,0,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,1,1,0,0 };
	double win[SupMax] = { 47.13,52.32,52.49,50.74,50.93,49.83,50.60,51.56,51.50,48.02,50.56,50.65,53.45,50.77,50.71,49.02,44.66,51.84,52.01,53.30,49.38,52.76,50.26,51.06,50.42 };
	FILE * file_pointer = NULL;
	file_pointer = fopen("Support.txt", "r");
	for (int i = 0; i < SupMax; i++) {
		fgets(champ_name, MAX_NAME, file_pointer);
		createchild(qtree, "Support", champ_name);
		cham_node = find_node(qtree->root, champ_name);
		cham_node->difficulty = diff[i];
		cham_node->assassin = ass[i];
		cham_node->warrior = war[i];
		cham_node->wizard = wiz[i];
		cham_node->supporter = 1;
		cham_node->tanker = tan[i];
		cham_node->win_rate = win[i];
	}
	fclose(file_pointer);
}

//------------------------------------------------------------------

// ���� ������
void main_disply(Tree * qtree)
{
	system("cls");
	picture();
	int key;
	int i = 0;
	int x, y = 0;

	gotoxy(31, 2);
	printf("[��]   �Թ��ڸ�  ����  ����  ���α׷�");
	gotoxy(16, 3);
	printf("�����������������������������������");
	gotoxy(16, 4);
	printf("�����������������������������������");
	gotoxy(16, 5);
	printf("�����������������������������������");
	gotoxy(16, 6);
	printf("�����������������������������������");
	gotoxy(16, 7);
	printf("�����������������������������������");
	gotoxy(16, 8);
	printf("�����������������������������������");
	gotoxy(16, 9);
	printf("�����������������������������������");
	gotoxy(16, 10);
	printf("");
	gotoxy(16, 11);
	printf("");

	gotoxy(34, 13);
	printf("\t 1. ������ ��õ\n");
	gotoxy(34, 14);
	printf("\t 2. è�Ǿ� ��õ\n");
	gotoxy(34, 15);
	printf("\t 3. è�Ǿ� ���丮 �� ����\n");
	gotoxy(34, 16);
	printf("\t 4. ���� ��Ÿ �м�\n");
	gotoxy(34, 17);
	printf("\t 5. 1��1 ������ �ο�\n");
	gotoxy(34, 18);
	printf("\t -----------------------\n");
	gotoxy(34, 19);
	printf("\t 0. ���α׷� ����\n");

	gotoxy(82, 23);
	textcolor(LIGHTRED, BLACK);
	printf("�âââââââ�");
	gotoxy(98, 23);
	textcolor(LIGHTMAGENTA, BLACK);
	printf("��");

	gotoxy(82, 24);
	textcolor(LIGHTRED, BLACK);
	printf("��");
	gotoxy(84, 24);
	textcolor(BROWN, BLACK);
	printf("��");
	gotoxy(86, 24);
	textcolor(GREEN, BLACK);
	printf("������");
	gotoxy(96, 24);
	textcolor(YELLOW, BLACK);
	printf("��");
	gotoxy(98, 24);
	textcolor(LIGHTCYAN, BLACK);
	printf("��");

	gotoxy(82, 25);
	textcolor(LIGHTRED, BLACK);
	printf("��");
	gotoxy(84, 25);
	textcolor(GREEN, BLACK);
	printf("��");
	gotoxy(86, 25);
	textcolor(BROWN, BLACK);
	printf("��");
	gotoxy(88, 25);
	textcolor(GREEN, BLACK);
	printf("����");
	gotoxy(94, 25);
	textcolor(YELLOW, BLACK);
	printf("��");
	gotoxy(96, 25);
	textcolor(GREEN, BLACK);
	printf("��");
	gotoxy(98, 25);
	textcolor(LIGHTCYAN, BLACK);
	printf("��");

	gotoxy(82, 26);
	textcolor(LIGHTRED, BLACK);
	printf("��");
	gotoxy(84, 26);
	textcolor(GREEN, BLACK);
	printf("���");
	gotoxy(88, 26);
	textcolor(BROWN, BLACK);
	printf("��");
	gotoxy(90, 26);
	textcolor(GREEN, BLACK);
	printf("��");
	gotoxy(92, 26);
	textcolor(YELLOW, BLACK);
	printf("��");
	gotoxy(94, 26);
	textcolor(GREEN, BLACK);
	printf("���");
	gotoxy(98, 26);
	textcolor(LIGHTCYAN, BLACK);
	printf("��");

	gotoxy(82, 27);
	textcolor(LIGHTRED, BLACK);
	printf("��");
	gotoxy(84, 27);
	textcolor(GREEN, BLACK);
	printf("����");
	gotoxy(90, 27);
	textcolor(YELLOW, BLACK);
	printf("��");
	gotoxy(92, 27);
	textcolor(GREEN, BLACK);
	printf("����");
	gotoxy(98, 27);
	textcolor(LIGHTCYAN, BLACK);
	printf("��");

	gotoxy(82, 28);
	textcolor(LIGHTRED, BLACK);
	printf("��");
	gotoxy(84, 28);
	textcolor(GREEN, BLACK);
	printf("���");
	gotoxy(88, 28);
	textcolor(YELLOW, BLACK);
	printf("��");
	gotoxy(90, 28);
	textcolor(GREEN, BLACK);
	printf("��");
	gotoxy(92, 28);
	textcolor(BROWN, BLACK);
	printf("��");
	gotoxy(94, 28);
	textcolor(GREEN, BLACK);
	printf("���");
	gotoxy(98, 28);
	textcolor(LIGHTCYAN, BLACK);
	printf("��");

	gotoxy(82, 29);
	textcolor(LIGHTRED, BLACK);
	printf("��");
	gotoxy(84, 29);
	textcolor(GREEN, BLACK);
	printf("��");
	gotoxy(86, 29);
	textcolor(YELLOW, BLACK);
	printf("��");
	gotoxy(88, 29);
	textcolor(GREEN, BLACK);
	printf("����");
	gotoxy(94, 29);
	textcolor(BROWN, BLACK);
	printf("��");
	gotoxy(96, 29);
	textcolor(GREEN, BLACK);
	printf("��");
	gotoxy(98, 29);
	textcolor(LIGHTCYAN, BLACK);
	printf("��");

	gotoxy(82, 30);
	textcolor(LIGHTRED, BLACK);
	printf("��");
	gotoxy(84, 30);
	textcolor(YELLOW, BLACK);
	printf("��");
	gotoxy(86, 30);
	textcolor(GREEN, BLACK);
	printf("������");
	gotoxy(96, 30);
	textcolor(BROWN, BLACK);
	printf("��");
	gotoxy(98, 30);
	textcolor(LIGHTCYAN, BLACK);
	printf("��");

	gotoxy(82, 31);
	textcolor(LIGHTBLUE, BLACK);
	printf("��");
	gotoxy(84, 31);
	textcolor(LIGHTCYAN, BLACK);
	printf("�âââââââ�");

	gotoxy(33, 32);
	textcolor(LIGHTGRAY, BLACK);
	printf("");

	//�� �� �� �� �� �� ��
	//�߾�UI
	gotoxy(34, 25);
	printf("��");
	for (i = 0; i < 38; i++)
		printf("��");
	printf("��");

	gotoxy(34, 31);
	printf("��");
	for (i = 0; i < 38; i++)
		printf("��");
	printf("��");

	y = 25;
	for (i = 0; i < 5; i++)
	{
		y++;
		gotoxy(73, y);
		printf("��");

	}

	y = 25;
	for (i = 0; i < 5; i++)
	{
		y++;
		gotoxy(34, y);
		textcolor(LIGHTMAGENTA, LIGHTMAGENTA);
		printf("��");
	}
	textcolor(LIGHTGRAY, BLACK);
	//UI(è�� ��ĭ)
	gotoxy(26, 26);
	printf("��");
	for (i = 0; i < 6; i++)
		printf("��");

	y = 26;
	for (i = 0; i < 3; i++)
	{
		y++;
		gotoxy(26, y);
		printf("��");

	}
	gotoxy(26, 30);
	printf("��");
	for (i = 0; i < 6; i++)
		printf("��");
	// P
	gotoxy(36, 26);
	printf("������");
	gotoxy(36, 27);
	printf("������");
	//Q
	gotoxy(40, 26);
	printf("����������");
	gotoxy(40, 27);
	printf("�� �Ѧ�");
	gotoxy(40, 28);
	printf("����������");
	//W
	gotoxy(46, 26);
	printf("����������");
	gotoxy(46, 27);
	printf("�� �צ�");
	gotoxy(46, 28);
	printf("����������");
	//E
	gotoxy(52, 26);
	printf("����������");
	gotoxy(52, 27);
	printf("�� �Ŧ�");
	gotoxy(52, 28);
	printf("����������");
	//R
	gotoxy(58, 26);
	printf("����������");
	gotoxy(58, 27);
	printf("�� �Ҧ�");
	gotoxy(58, 28);
	printf("����������");
	//D
	gotoxy(64, 26);
	printf("������");
	gotoxy(64, 27);
	printf("������");
	//F
	gotoxy(68, 26);
	printf("������");
	gotoxy(68, 27);
	printf("������");
	//ü��
	gotoxy(37, 29);
	textcolor(GREEN, GREEN);
	printf(" �âââââââââââââââ� ");
	gotoxy(37, 30);
	textcolor(BLUE, BLUE);
	printf(" �âââââââââââââââ� ");

	//����
	printf("");

	// ������
	textcolor(LIGHTGRAY, BLACK);
	gotoxy(3, 33);
	printf("");
	gotoxy(3, 34);
	printf("");

	key = _getch();
	if (key == '1') {
		system("cls");
		//1.���� ��õ�� ���� �Լ�
		PLAY_Rec_Line();
	}
	else if (key == '2') {
		//2. è�Ǿ� ��õ�� ���� �Լ�
		recommand(qtree);
	}
	else if (key == '3') {
		//3. è�Ǿ� ���丮 �� ������ ���� �Լ�
		Story_Strategy();
	}
	else if (key == '4') {
		// ��Ÿ �м��� ���� �Լ�
		current_meta(qtree);
	}
	else if (key == '5') {
		system("cls");
		//5. ���������� ���� �Լ�
		Play_Game(qtree);//�������� ����
	}
	else if (key == '0') {
		system("cls");
		printf("���α׷��� �����մϴ�!\n");
		exit('\0');
	}
	else {
	}
}

//-------------------------------------------------------------------

// 1.������ ��õ ( �迭�� �ؽ�Ʈ ���� ���)
void add_question(char Q_list1[][200], char Q_list2[][200], char Q_list3[][200], char Q_list4[][200], char Q_list5[][200], char Q_list6[][200], char Q_list7[][200], char Q_list8[][200], char Q_list9[][200]) {//���� �߰�

	strcpy((&Q_list1[0]), "���� ������ ������ ���̸� ������ ���߷��� �Ѵ�.");
	strcpy((&Q_list1[1]), "���� ���� �߸��Ǿ��� �� ���� ������ ſ���� ������.");
	strcpy((&Q_list1[2]), "���� ���� ���̴��� �������� ������ ���̴�.");
	strcpy((&Q_list1[3]), "�� ������ ���� �ٲ�� ���ÿ� �������� �ൿ�� �ϴ� ���̴�.");
	strcpy((&Q_list1[4]), "���� Ÿ���� �򰡿� ���ֹ��� �ʰ� �� �ǰ��� ���Ѵ�.");
	strcpy((&Q_list2[0]), "���� ���質 ���� �߸��Ǿ��� �� ���� �� ſ���� ������.");
	strcpy((&Q_list2[1]), "���� �ٸ� ������� �ǰ��� �����ϰ� �� �ǰ��� �����ϴ� ���̴�.");
	strcpy((&Q_list2[2]), "���� �̼����̰� �����ϸ� �����ϴ�.");
	strcpy((&Q_list2[3]), "���� �ٸ� �����κ��� ������ ���ų� �길�ϴٴ� �Ҹ��� ��´�.");
	strcpy((&Q_list2[4]), "���� �������� ������ ������ ǥ���Ѵ�.");
	strcpy((&Q_list3[0]), "���� ����ġ�� ���� �ǽ��ؼ� ���� �����̳� ������ ǥ���ϴ°� �η����Ѵ�.");
	strcpy((&Q_list3[1]), "���� �� �ǰ��� �޾Ƶ鿩���� �ʾ����� ȭ�� ����.");
	strcpy((&Q_list3[2]), "���� ���� ���ظ� �и��� ǥ���ϱ� ���� �������� �ڷḦ ���� �ο��Ѵ�.");
	strcpy((&Q_list3[3]), "���� ��Ȳ�� �������� ���� ���̳� �ൿ�� ���� �ϰ� ������ �ǿ�� ���̴�.");
	strcpy((&Q_list3[4]), "���� �ٸ� ����� ���� ��Ź�� �� �� ���� ��ġ ������ �����Ѵ�.");
	strcpy((&Q_list4[0]), "���� ������� �� ǥ��,����,������ �Ű��� ���� ����.");
	strcpy((&Q_list4[1]), "���� Ÿ���� �����̳� �߸��� �� ã�Ƴ��� �����Ѵ�.");
	strcpy((&Q_list4[2]), "���� �Ǽ����� �������� �ָ� ���� ���̴�.");
	strcpy((&Q_list4[3]), "���� ����ϰų� ��ó�� ���� ����̳� ���ӷ� �� ��Ȳ�� �ٲٷ� �Ѵ�.");
	strcpy((&Q_list4[4]), "���� �� �ڽſ� ���� �����ϰ� ������.");
	strcpy((&Q_list5[0]), "���� Ÿ���� ����ϰ� �� ������ �ִ� ���̴�.");
	strcpy((&Q_list5[1]), "���� ������̰� �������� ������ ���� ����ϱ� ������ ��밡 ���ݹ޾Ҵٴ� ������ �� ���� �ִ�.");
	strcpy((&Q_list5[2]), "���� ������ ��Ȳ�� �״�� �ѱ��� ���ϰ� �ýú�� ������ ���̴�.");
	strcpy((&Q_list5[3]), "���� ������ ��Ȳ���� �����������ϰų� ������ ���� ���Ѵ�.");
	strcpy((&Q_list5[4]), "���� ������ �η��� ���� �ʴ´�.");
	strcpy((&Q_list6[0]), "���� �ٸ� ������� ���� �Ⱦ��ұ� �η����� ����ǰų� �Ҿ��� ���� ���� ����.");
	strcpy((&Q_list6[1]), "���� ����� �Ͽ��� �� ����ϰų� ȭ�� ����.");
	strcpy((&Q_list6[2]), "���� �����ϰ� ħ�������� �����ϴ� ���� ���� ��´�.");
	strcpy((&Q_list6[3]), "���� �� ������ �����ϱ� ���� ȭ���� ���� �ٲ۴�.");
	strcpy((&Q_list6[4]), "���� �پ��� ���迡 �������̴�.");
	strcpy((&Q_list7[0]), "���� Ÿ���� ��û�� �������� ���ϴ� ���̴�.");
	strcpy((&Q_list7[1]), "���� ���� ������ ����ǰ� ���� �����ϸ� ������ ������ ���� ������ �Ѵ�.");
	strcpy((&Q_list7[2]), "���� ���� ������ ǥ���ϱ� ����� ȥ�ڶ�� �������� ����.");
	strcpy((&Q_list7[3]), "���� �����Ⱑ ħü�ǰų� ���������� �����⸦ �ٲٷ��� �Ѵ�.");
	strcpy((&Q_list7[4]), "���� ������ ��Ư�� ������ �����Ѵ�.");
	strcpy((&Q_list8[0]), "���� �� �ڽ��� ��ġ�� ���� �� ���� ����ϰ� ������ ���� ����.");
	strcpy((&Q_list8[1]), "���� Ÿ�����κ��� �������̰ų� ���뼺 ���ٴ� ���� ��⵵ �Ѵ�.");
	strcpy((&Q_list8[2]), "���� ��Ҹ��� �����Ӱ� ��ǥ���ϸ� ������ �ڼ��� ���ϴ� ���̴�.");
	strcpy((&Q_list8[3]), "���� �Ҿ��ϸ� ȣȩ�� ���� ���ϰ� �Ӹ��� �������� ������ �ϱ⵵ �Ѵ�.");
	strcpy((&Q_list8[4]), "���� ���� �� �ǰ߿� �ݴ��ص� ������ ������ �ʴ´�.");
	strcpy((&Q_list9[0]), "���� �ַΰ� ���ϴ�! Ŀ�� ������!!");
	strcpy((&Q_list9[1]), "���� �����̴�!");


}
void add_answer(Question Q_list[]) {//��� �߰� //���ɼ� ���ҽ�Ű�°� �߰��������� ���!
	int i;
	for (i = 0; i < qMAX; i++) { //Q.answer���� ��� -1�� �ʱ�ȭ
		(Q_list[i]).answer[i] = -1;
	}
	for (i = 0; i < 5; i++) {
		if ((i % 5) == 0) {
			(Q_list[i]).answer[0] = SUPPORTER;
		}
		else if ((i % 5) == 1) {
			(Q_list[i]).answer[0] = TOP;
		}
		else if ((i % 5) == 2) {
			(Q_list[i]).answer[0] = MID;
			(Q_list[i]).answer[1] = JUNGLE;
			(Q_list[i]).answer[2] = AD_CARRY;
		}
		else if ((i % 5) == 3) {
			(Q_list[i]).answer[0] = JUNGLE;
		}
		else if ((i % 5) == 4) {
			(Q_list[i]).answer[0] = AD_CARRY;
			(Q_list[i]).answer[1] = SUPPORTER;
		}
	}
	(Q_list[40]).answer[0] = TOP;
	(Q_list[40]).answer[1] = MID;
	(Q_list[40]).answer[2] = JUNGLE;

	(Q_list[41]).answer[0] = MID;
	(Q_list[41]).answer[1] = AD_CARRY;
}
void add_line(Question Q, int Line_rec[]) {//������ �信 ���� ����� ����

	for (int i = 0; i < LineMAX; i++) {
		if (Q.answer[i] == TOP) {
			Line_rec[TOP]++;
		}

		else if (Q.answer[i] == JUNGLE) {
			Line_rec[JUNGLE]++;
		}
		else if (Q.answer[i] == MID) {
			Line_rec[MID]++;
		}
		else if (Q.answer[i] == AD_CARRY) {
			Line_rec[AD_CARRY]++;
		}

		else if (Q.answer[i] == SUPPORTER) {
			Line_rec[SUPPORTER]++;
		}

	}

}
void play_question(Question Q_list[], int Line_rec[], char Q_list1[][200], char Q_list2[][200], char Q_list3[][200], char Q_list4[][200], char Q_list5[][200], char Q_list6[][200], char Q_list7[][200], char Q_list8[][200], char Q_list9[][200]) {//���� ����
	system("cls");
	picture();

	int x, y = 0;
	int i, j;
	int z;
	gotoxy(0, 3);
	printf("��");
	for (z = 0; z < 99; z++)
		printf("��");
	printf("��");
	gotoxy(0, 4);
	printf("��");

	for (i = 0; i < qMAX; i++) {
		int key;
		if (0 <= i && i < 5) {
			printf("Q%d)", i + 1);
			printf("%s", (Q_list1[i]));
		}
		else if (5 <= i && i < 10) {
			printf("Q%d)", i + 1);
			printf("%s", (Q_list2[i - 5]));
		}
		else if (10 <= i && i < 15) {
			printf("Q%d)", i + 1);
			printf("%s", (Q_list3[i - 10]));
		}
		else if (15 <= i && i < 20) {
			printf("Q%d)", i + 1);
			printf("%s", (Q_list4[i - 15]));
		}
		else if (20 <= i && i < 25) {
			printf("Q%d)", i + 1);
			printf("%s", (Q_list5[i - 20]));
		}
		else if (25 <= i && i < 30) {
			printf("Q%d)", i + 1);
			printf("%s", (Q_list6[i - 25]));
		}
		else if (30 <= i && i < 35) {
			printf("Q%d)", i + 1);
			printf("%s", (Q_list7[i - 30]));
		}
		else if (35 <= i && i < 40) {
			printf("Q%d)", i + 1);
			printf("%s", (Q_list8[i - 35]));
		}
		else if (40 <= i && i < 42) {
			printf("Q%d)", i + 1);
			printf("%s", (Q_list9[i - 40]));
		}
		gotoxy(100, 4);
		printf("��");
		gotoxy(0, 5);
		printf("��");
		for (z = 0; z < 71; z++)
			printf("��");
		printf("��");
		for (z = 0; z < 27; z++)
			printf("��");
		printf("��");

		gotoxy(0, 7);
		printf("��");
		gotoxy(72, 6);
		printf("��");
		gotoxy(72, 7);
		printf("��");
		gotoxy(72, 8);
		printf("��");
		gotoxy(72, 9);
		printf("��");
		gotoxy(72, 10);
		printf("��");
		gotoxy(75, 7);
		printf("P : ���� ����");
		gotoxy(75, 8);
		printf("N : ���� ����");
		gotoxy(75, 9);
		printf("X : ��������� ����ҷ���");

		gotoxy(33, 7);
		printf("1. YES");
		gotoxy(33, 9);
		printf("2. NO");
		gotoxy(0, 11);
		printf("��");
		for (z = 0; z < 71; z++)
			printf("��");
		printf("��");
		for (z = 0; z < 27; z++)
			printf("��");
		printf("��");
		gotoxy(33, 12);
		printf(">>");
		key = _getch();
		gotoxy(0, 13);
		printf("��");
		for (z = 0; z < 99; z++)
			printf("��");
		printf("��");
		if (key == 0) {
			printf("������õ�� �����մϴ�.");
			break;
		}
		else if (key == '1') {
			add_line(Q_list[i], Line_rec);
		}
		else if (key == '2') {
			system("cls");
		}
		else if (key == 'p') {
			if (i == 0) {
				printf("ù ��° �����Դϴ�.\n");
				delay(1000);
				i--;
			}
			else {
				i -= 2;
			}
		}//���� Ȥ�� ���� �������� �Ѿ�� ���
		else if (key == 'n') {
			if (i == 41) {
				printf("������ �����Դϴ�.\n");
				i--;
				delay(1000);
			}
		}
		else if (key == 'x') {
			if (i == 0) {
				printf("���� �ڷᰡ �����մϴ�.\n");
				delay(1000);
				i--;
			}
			else {
				i = 42;//for���� �����Ű�� ����
			}
		}
		else {

			i--;

		}

		system("cls");
	}

}
void display_top_trait() {
	//ž
	system("cls");
	picture();
	gotoxy(44, 2);
	printf("������ �� ����  ž");
	gotoxy(26, 4);
	printf("��������������������������");
	gotoxy(26, 5);
	printf("��������������������������");
	gotoxy(26, 6);
	printf("��������������������������");
	gotoxy(26, 7);
	printf("��������������������������");
	gotoxy(26, 8);
	printf("��������������������������");
	gotoxy(26, 9);
	printf("��������������������������");
	gotoxy(26, 10);
	printf("��������������������������");

	gotoxy(25, 12);
	printf("��");
	gotoxy(75, 12);
	printf("��");
	gotoxy(27, 13);
	printf("���� ��Ȳ �ӿ����� �����ϰ� ���߼� ������ �̲���");
	gotoxy(27, 15);
	printf("��Ȳ�Ǵ� �ɷ��� �پ�� �̴Ͻ� ���� �� ����");
	gotoxy(27, 17);
	printf("ȥ�ڰ� ���ϴ� / �ܷ��� �ʾƼ� �ַη� �����°Ŵ�");
	gotoxy(27, 19);
	printf("�ڷ���Ʈ�� ���� ����� ������ �¸��� �⿩�Ѵ�");
	gotoxy(25, 20);
	printf("��");
	gotoxy(75, 20);
	printf("��");
	printf("\n");
}
void display_mid_trait() {
	//�̵�
	system("cls");
	picture();
	gotoxy(44, 2);
	printf("������ �� ����  �̵�");
	gotoxy(26, 4);
	printf("��������������������������");
	gotoxy(26, 5);
	printf("��������������������������");
	gotoxy(26, 6);
	printf("��������������������������");
	gotoxy(26, 7);
	printf("��������������������������");
	gotoxy(26, 8);
	printf("��������������������������");
	gotoxy(26, 9);
	printf("��������������������������");
	gotoxy(26, 10);
	printf("��������������������������");

	gotoxy(25, 12);
	printf("��");
	gotoxy(75, 12);
	printf("��");
	gotoxy(27, 13);
	printf("���̽� ������ �ִ� ");
	gotoxy(27, 15);
	printf("���� �������� ���� ������ �����Ѵ�");
	gotoxy(27, 17);
	printf("�ָ�޴°��� �����Ѵ� / ��¦ ���� ���� �ִ�");
	gotoxy(27, 19);
	printf("���� �ʸ����� ���� �Ʊ����� �����Ϸ� ����");
	gotoxy(27, 21);
	printf("���� �����ν� å�Ӱ��� �����ϴ�");
	gotoxy(25, 22);
	printf("��");
	gotoxy(75, 22);
	printf("��");
	printf("\n");
}
void display_jungle_trait() {
	//����
	system("cls");
	picture();
	gotoxy(44, 2);
	printf("������ �� ����  ����");
	gotoxy(26, 4);
	printf("��������������������������");
	gotoxy(26, 5);
	printf("��������������������������");
	gotoxy(26, 6);
	printf("��������������������������");
	gotoxy(26, 7);
	printf("��������������������������");
	gotoxy(26, 8);
	printf("��������������������������");
	gotoxy(26, 9);
	printf("��������������������������");
	gotoxy(26, 10);
	printf("��������������������������");

	gotoxy(25, 12);
	printf("��");
	gotoxy(75, 12);
	printf("��");
	gotoxy(27, 13);
	printf("�躴�� �� ��ü��");
	gotoxy(27, 15);
	printf("��� ���۰��� �ɸ����� ����");
	gotoxy(27, 17);
	printf("Ž���� �����Ѵ� / �͵�� ���� ���������� ");
	gotoxy(27, 19);
	printf("���� ���ڵγ��� �¸��� ���� �� ������ �����Ѵ�");
	gotoxy(25, 20);
	printf("��");
	gotoxy(75, 20);
	printf("��");
	printf("\n");
}
void display_ad_carry_trait() {
	//���Ÿ� ����
	system("cls");
	picture();
	gotoxy(44, 2);
	printf("������ �� ����  ���Ÿ� ����");
	gotoxy(26, 4);
	printf("��������������������������");
	gotoxy(26, 5);
	printf("��������������������������");
	gotoxy(26, 6);
	printf("��������������������������");
	gotoxy(26, 7);
	printf("��������������������������");
	gotoxy(26, 8);
	printf("��������������������������");
	gotoxy(26, 9);
	printf("��������������������������");
	gotoxy(26, 10);
	printf("��������������������������");

	gotoxy(25, 12);
	printf("��");
	gotoxy(75, 12);
	printf("��");
	gotoxy(27, 13);
	printf("�ָ�޴°��� �����Ѵ� / ��¦ ���� ���� �ִ�");
	gotoxy(27, 15);
	printf("������ ��Ƴ��� ���� ȸ�Ǵɷ°� �������� �ʿ��ϴ�");
	gotoxy(27, 17);
	printf("�����Ϳ��� ����ũ�� �ſ� �߿��ϴ� ");
	gotoxy(27, 19);
	printf("������ �������� �Ĺ� ������ �����Ѵ�");
	gotoxy(25, 20);
	printf("��");
	gotoxy(75, 20);
	printf("��");
	printf("\n");
}
void display_supporter_trait() {
	//������
	system("cls");
	picture();
	gotoxy(44, 2);
	printf("������ �� ����  ������");
	gotoxy(26, 4);
	printf("��������������������������");
	gotoxy(26, 5);
	printf("��������������������������");
	gotoxy(26, 6);
	printf("��������������������������");
	gotoxy(26, 7);
	printf("��������������������������");
	gotoxy(26, 8);
	printf("��������������������������");
	gotoxy(26, 9);
	printf("��������������������������");
	gotoxy(26, 10);
	printf("��������������������������");

	gotoxy(25, 12);
	printf("��");
	gotoxy(75, 12);
	printf("��");
	gotoxy(27, 13);
	printf("��...");
	gotoxy(27, 15);
	printf("��Ӵ�...");
	gotoxy(27, 17);
	printf("�Ĳ��ϸ� �ڻ��Ͽ� ������ ��Ż�� �ɾ��Ѵ�");
	gotoxy(27, 19);
	printf("���ڴ� �ϵ�Ŷ���, ���ڴ� �����ó��");
	gotoxy(25, 20);
	printf("��");
	gotoxy(75, 20);
	printf("��");
	printf("\n");
}
void display_position_trait() {
	system("cls");
	picture();
	display_top_trait();
	display_mid_trait();
	display_jungle_trait();
	display_ad_carry_trait();
	display_supporter_trait();
}
void more_trait() {
	int key;
	int z;
	picture();

	gotoxy(0, 23);
	printf("��");
	for (z = 0; z < 99; z++)
		printf("��");
	printf("��");

	gotoxy(40, 25);
	printf("X : �ʱ� �޴��� ���ư���");

	gotoxy(0, 26);
	printf("��");
	for (z = 0; z < 99; z++)
		printf("��");
	printf("��");

	while (1) {
		key = _getch();
		if (key == 'x') {
			system("cls");
			printf("�ʱ� �޴��� ���ư��ϴ�.\n");
			break;
		}
		else {

		}
	}
}
void display_result(int idx) {
	if (idx == TOP) {
		printf("��ſ��� ���� ������ �������� 'ž' �Դϴ�.\n");
		display_top_trait();
		more_trait();
	}
	else if (idx == MID) {
		printf("��ſ��� ���� ������ �������� '�̵�' �Դϴ�.\n");
		display_mid_trait();
		more_trait();
	}
	else if (idx == JUNGLE) {
		printf("��ſ��� ���� ������ �������� '����' �Դϴ�.\n");
		display_jungle_trait();
		more_trait();
	}
	else if (idx == AD_CARRY) {
		printf("��ſ��� ���� ������ �������� '���Ÿ� ����' �Դϴ�.\n");
		display_ad_carry_trait();
		more_trait();
	}
	else if (idx == SUPPORTER) {
		printf("��ſ��� ���� ������ �������� '������' �Դϴ�.\n");
		display_supporter_trait();
		more_trait();
	}
	else if (idx == -1) {
		printf("\n�׽�Ʈ�� ���������� �������� �����̽��ϴ�.\n�ٽ� �������ּ���\n");
		delay(1500);
		PLAY_Rec_Line();
	}
}
int get_result(int Line_rec[]) {//���� �迭���� ���� idx(������) ��ȯ
	int Max = 0;
	int idx = -1;
	int zxzx;
	for (zxzx = 0; zxzx < LineMAX; zxzx++) {
		if (Max < Line_rec[zxzx]) {
			Max = Line_rec[zxzx];
			idx = zxzx;
		}
	}
	return idx;
}

void PLAY_Rec_Line(void) {
	char Q1[5][200];
	char Q2[5][200];
	char Q3[5][200];
	char Q4[5][200];
	char Q5[5][200];
	char Q6[5][200];
	char Q7[5][200];
	char Q8[5][200];
	char Q9[2][200];
	int Line_rec[LineMAX] = { 0, };
	Question Q_list[qMAX];//���� ����Ʈ
	add_question(Q1, Q2, Q3, Q4, Q5, Q6, Q7, Q8, Q9);//���� �߰�
	add_answer(Q_list);//���� ���߰�
	play_question(Q_list, Line_rec, Q1, Q2, Q3, Q4, Q5, Q6, Q7, Q8, Q9);//���� ����
	display_result(get_result(Line_rec));//������ ��� ���
}

// end - 1. ������ ��õ

// -----------------------------------------------------------
//2. è�Ǿ� ��õ

void recommand(Tree * qtree)
{
	Player * player = init_player();
	char arr[MAX_NAME] = { 0, };
	int choice;

	while (TRUE) {
		system("cls");
		rec_display();
		choice = _getch();
		system("cls");
		if (choice == '1') {
			select_diff(player);
		}
		else if (choice == '2') {
			select_line(player);
		}
		else if (choice == '3') {
			select_work(player);
		}
		else if (choice == '4') {
			player_data(player);
			if (player->difficulty != 0 || player->line != -1 || player->assassin != 0 || player->warrior != 0 || player->wizard != 0 || player->supporter != 0 || player->tanker != 0 || player->ad != 0) {
				// Player è�� �迭 �ʱ�ȭ
				for (int i = 0; i < Champ_num; i++) {
					strncpy(player->name[i], arr, sizeof(player->name[i]));
				}
				player->num_cham = 0;
				// ��õ è�� ����
				save_cham(qtree, player);
				//��õ è�� ���
				recommand_champ(player);
			}
			_getch();
		}
		else if (choice == '5') {
			player = init_player();
			printf("���� ������ �ʱ�ȭ �Ǿ����ϴ�.\n");
			_getch();
		}
		else if (choice == '0') {
			break;
		}
		else {
		}
		printf("\n\n");
	}
}

// ����� ����ü �ʱ�ȭ
Player * init_player()
{
	Player * player = (Player*)malloc(sizeof(Player));
	char arr[MAX_NAME] = { 0, };
	for (int i = 0; i < Champ_num; i++) {
		strncpy(player->name[i], arr, sizeof(player->name[i]));
	}
	player->num_cham = 0;
	player->line = -1;
	player->difficulty = 0;
	// ����
	player->assassin = 0;
	player->warrior = 0;
	player->wizard = 0;
	player->supporter = 0;
	player->tanker = 0;
	player->ad = 0;
	return player;
}

// ���̵� ����
void select_diff(Player * player)
{
	system("cls");
	picture();

	int i;
	int diff;

	gotoxy(35, 2);
	printf("���̵��� �������ּ���");
	gotoxy(0, 5);
	printf("��");
	for (i = 0; i < 99; i++)
		printf("��");
	printf("��");
	gotoxy(35, 7);
	printf("1) ��");
	gotoxy(35, 9);
	printf("2) ��");
	gotoxy(35, 11);
	printf("3) ��");
	gotoxy(0, 13);
	printf("��");
	for (i = 0; i < 99; i++)
		printf("��");
	printf("��");
	gotoxy(35, 15);

	printf("0) ���� �޴�");
	gotoxy(0, 17);
	printf("��");
	for (i = 0; i < 99; i++)
		printf("��");
	printf("��");

	diff = _getch();
	if (diff == '3') {
		player->difficulty = 8;
		gotoxy(25,19);
		printf("�÷��̾��� ���̵� ������ �Ϸ�Ǿ����ϴ�!\n");
		Sleep(1000);
		_getch();
	}
	else if (diff == '2') {
		player->difficulty = 5;
		gotoxy(25, 19);
		printf("�÷��̾��� ���̵� ������ �Ϸ�Ǿ����ϴ�!\n");
		Sleep(1000);
		_getch();
	}
	else if (diff == '1') {
		player->difficulty = 1;
		gotoxy(25, 19);
		printf("�÷��̾��� ���̵� ������ �Ϸ�Ǿ����ϴ�!\n");
		Sleep(1000);
		_getch();
	}
	else if (diff == '0') {
		gotoxy(25, 19);
		printf("���� �޴��� ���ư��ϴ�");
		Sleep(1000);
		_getch();
	}
	else {
		system("cls");
		select_diff(player);
	}
}

// ���� ����
void select_line(Player * player)
{
	system("cls");
	picture();

	int i;
	int line; 
	gotoxy(35, 2);
	printf("������ �������ּ���");

	gotoxy(0, 5);
	printf("��");
	for (i = 0; i < 99; i++)
		printf("��");
	printf("��");

	gotoxy(35, 7);
	printf("1) ž");
	gotoxy(35, 9);
	printf("2) ����");
	gotoxy(35, 11);
	printf("3) �̵�");
	gotoxy(35, 13);
	printf("4) ����");
	gotoxy(35, 15);
	printf("5) ����");

	gotoxy(0, 17);
	printf("��");
	for (i = 0; i < 99; i++)
		printf("��");
	printf("��");
	gotoxy(35, 19);

	printf("0) ���� �޴�");
	gotoxy(0, 21);
	printf("��");
	for (i = 0; i < 99; i++)
		printf("��");
	printf("��");
	line = _getch();
	if (line == '1' || line == '2' || line == '3' || line == '4' || line == '5') {
		if (line == '1')
			player->line = TOP;
		else if (line == '2')
			player->line = JUNGLE;
		else if (line == '3')
			player->line = MID;
		else if (line == '4')
			player->line = AD_CARRY;
		else if (line == '5')
			player->line = SUPPORTER;
		gotoxy(25, 23);
		printf("�÷��̾��� ���� ������ �Ϸ�Ǿ����ϴ�!\n");
		Sleep(1000);
		_getch();
	}
	else if (line == '0') {
		gotoxy(25, 23);
		printf("���� �޴��� ���ư��ϴ�\n");
		_getch();
	}
	else {
		system("cls");
		select_line(player);
	}
}

// ���ұ� ����
void select_work(Player * player)
{
	
	system("cls");
	picture();

	int i;
	int work;
	gotoxy(35, 2);
	printf("��ȣ ���ұ��� �������ּ���");

	gotoxy(0, 5);
	printf("��");
	for (i = 0; i < 99; i++)
		printf("��");
	printf("��");

	gotoxy(35, 7);
	printf("1) �ϻ���");
	gotoxy(35, 9);
	printf("2) ����");
	gotoxy(35, 11);
	printf("3) ������");
	gotoxy(35, 13);
	printf("4) ������");
	gotoxy(35, 15);
	printf("5) ��Ŀ");
	gotoxy(35, 17);
	printf("6) ���Ÿ� ����");

	gotoxy(0, 19);
	printf("��");
	for (i = 0; i < 99; i++)
		printf("��");
	printf("��");
	gotoxy(35, 21);

	printf("0) ���� �޴�");
	gotoxy(0, 23);
	printf("��");
	for (i = 0; i < 99; i++)
		printf("��");
	printf("��");
	
	work = _getch();
	if (work == '1') {
		player->assassin = 1;
		gotoxy(25, 25);
		printf("�÷��̾��� ���ұ� ������ �Ϸ�Ǿ����ϴ�!\n");
		Sleep(1000);
		_getch();
	}
	else if (work == '2') {
		player->warrior = 1;
		gotoxy(25, 25);
		printf("�÷��̾��� ���ұ� ������ �Ϸ�Ǿ����ϴ�!\n");
		Sleep(1000);
		_getch();
	}
	else if (work == '3') {
		player->wizard = 1;
		gotoxy(25, 25);
		printf("�÷��̾��� ���ұ� ������ �Ϸ�Ǿ����ϴ�!\n");
		Sleep(1000);
		_getch();
	}
	else if (work == '4') {
		player->supporter = 1;
		gotoxy(25, 25);
		printf("�÷��̾��� ���ұ� ������ �Ϸ�Ǿ����ϴ�!\n");
		Sleep(1000);
		_getch();
	}
	else if (work == '5') {
		player->tanker = 1;
		gotoxy(25, 25);
		printf("�÷��̾��� ���ұ� ������ �Ϸ�Ǿ����ϴ�!\n");
		Sleep(1000);
		_getch();
	}
	else if (work == '6') {
		player->ad = 1;
		gotoxy(25, 25);
		printf("�÷��̾��� ���ұ� ������ �Ϸ�Ǿ����ϴ�!\n");
		Sleep(1000);
		_getch();
	}
	else if (work == '0') {
		gotoxy(25, 25);
		printf("���� �޴��� ���ư��ϴ�\n");
		Sleep(1000);
		_getch();
	}
	else {
		select_work(player);
	}
}

// è�Ǿ� ��õ ������
void rec_display()
{
	int i;
	system("cls");
	picture();
	gotoxy(45, 1);
	printf("è�Ǿ� ��õ");
	gotoxy(0, 2);
	printf("��");
	for (i = 0; i < 99; i++)
		printf("��");
	printf("��");
	gotoxy(20, 3);
	printf("�ش� �޴����� �����Ͽ� ����� ��ȣ ������ �������ּ���!\n\n");
	gotoxy(20, 4);
	printf("   ���̵�, ������ �Ѱ����� ���� �����մϴ�!\n");
	gotoxy(20, 5);
	printf("   ���ұ��� ������ ���� �����մϴ�!(�ϳ��� �������ּ���!)\n");
	gotoxy(20, 6);
	printf("   �߸� �����ϼ̴ٸ�, 5�� ���� ���� �ʱ�ȭ �����մϴ�!\n\n");
	gotoxy(20, 7);
	printf("���� �������� �����Ͽ� è�Ǿ��� ��õ�ص帳�ϴ�!*^^*\n");
	gotoxy(0, 8);
	printf("��");
	for (i = 0; i < 99; i++)
		printf("��");
	printf("��");
	gotoxy(40, 10);
	printf("1. ���̵� ����\n");
	gotoxy(40, 12);
	printf("2. ���� ����\n");
	gotoxy(40, 14);
	printf("3. ���ұ� ����\n");
	gotoxy(40, 16);
	printf("4. ���� è�Ǿ� ��õ\n");
	gotoxy(40, 18);
	printf("5. ���� �ʱ�ȭ\n");
	gotoxy(0, 20);
	printf("��");
	for (i = 0; i < 99; i++)
		printf("��");
	printf("��");
	gotoxy(40,21);
	printf("0. �ڷ� ����\n");
	gotoxy(0, 22);
	printf("��");
	for (i = 0; i < 99; i++)
		printf("��");
	printf("��");
	
}
// ����� ���� ���� ���
void player_data(Player * player)
{
	int i;
	system("cls");
	picture();

	gotoxy(34, 0);
	printf("��");

	for (i = 0; i < 8; i++)
	{
		gotoxy(34, 1+i);
		printf("��");
	}
	gotoxy(0, 2);
	printf("��");
	for (i = 0; i < 33; i++)
		printf("��");
	printf("��");

	gotoxy(0,8);
	printf("��");
	for (i = 0; i < 33; i++)
		printf("��");
	printf("��");
	gotoxy(9, 1);
	printf("����� ���� ����");
	if (player->difficulty == 0 && player->line == -1 && player->assassin == 0 && player->warrior == 0 && player->wizard == 0 && player->supporter == 0 && player->tanker == 0 && player->ad == 0)
	{
		gotoxy(3, 4);
		printf("������ ����� ������ �����ϴ�!\n");
		Sleep(2000);
	}
	else 
	{
		if (player->difficulty != 0) {
			gotoxy(6,3);
			if (player->difficulty == 8)
				printf("���̵� :  �� \n");
			if (player->difficulty == 5)
				printf("���̵� :  �� \n");
			if (player->difficulty == 1)
				printf("���̵� :  �� \n");
		}
		gotoxy(6, 4);
		if (player->line != -1)
			printf("����   : ");
		if (player->line == 0)
			printf(" ž \n");
		else if (player->line == 1)
			printf(" ���� \n");
		else if (player->line == 2)
			printf(" �̵� \n");
		else if (player->line == 3)
			printf(" ���� \n");
		else if (player->line == 4)
			printf(" ���� \n");
		if (player->assassin == 1 || player->warrior == 1 || player->wizard == 1 || player->supporter == 1 || player->tanker == 1 || player->ad == 1) 
		{
			gotoxy(6, 5);
			printf("��Ȱ�� :  ");
			if (player->assassin == 1)
				printf("�ϻ��� ");
			if (player->warrior == 1)
				printf("���� ");
			gotoxy(16, 6);
			if (player->wizard == 1)
				printf("������ ");
			if (player->supporter == 1)
				printf("������ ");
			gotoxy(16, 7);
			if (player->tanker == 1)
				printf("��Ŀ ");
			if (player->ad == 1)
				printf("���Ÿ� ���� ");

		}
		Sleep(2000);
	}
}

// ��õ è�Ǿ� ���
void recommand_champ(Player * player)
{
	int k;
	int x = 0, y = 0;
	if (player->num_cham == 0) {
		gotoxy(25, 3);
		printf("\t �ش� ������ ��� �����ϴ� è���� �����ϴ�\n");
		gotoxy(0, 8);
		printf("��");
		for (k = 0; k < 99; k++)
			printf("��");
		printf("��");
	}
	else {
		gotoxy(55, 1);
		printf("��õ è�� ���\n");
		gotoxy(34, 2);
		printf("��");
		for (k = 0; k < 65; k++)
			printf("��");
		printf("��");
		gotoxy(55, 5);
		printf("è�Ǿ� �� = %d \n", player->num_cham);
		gotoxy(34, 8);
		printf("��");
		for (k = 0; k < 65; k++)
			printf("��");
		printf("��");
		y = 10;
		for (int i = 0,k=0; i < player->num_cham; i++) 
		{
			gotoxy(4+14*k, y);
			printf("%s",player->name[i]);
			y+=2;
			if (y ==32)
			{
				y = 10;
				k++;
			}
		}
		
		/*if(i<11)
		{
		printf("\t %s\n", player->name[i]);
		}
		else if (11<=i<23)
		{

		gotoxy(25, 10+j-11-k);
		printf("\t %s\n", player->name[i]);
		k++;
		}
		else if(23<=i<35)
		{

		gotoxy(45, 9+j-23-k);
		printf("\t %s\n", player->name[i]);
		k++;
		}*/
		_getch();
	}
}

// ��õ è���� ����� è�� �迭�� ����
void save_cham(Tree * qtree, Player * player)
{
	char champ_name[MAX_NAME];
	int line = player->line;
	Node * cham_node = NULL;

	// ���� ����O -> Ư�� ���� ��ȸ
	if (line == 1 || line == 2 || line == 3 || line == 4 || line == 5) {
		FILE * file_pointer = NULL;
		int num;
		if (line == 0) {
			file_pointer = fopen("Top.txt", "r");
			num = TopMax;
		}
		else if (line == 1) {
			file_pointer = fopen("Jungle.txt", "r");
			num = JungleMax;
		}
		else if (line == 2) {
			file_pointer = fopen("Mid.txt", "r");
			num = MidMax;
		}
		else if (line == 3) {
			file_pointer = fopen("AD.txt", "r");
			num = AdMax;
		}
		else if (line == 4) {
			file_pointer = fopen("Support.txt", "r");
			num = SupMax;
		}

		for (int i = 0; i < num; i++) {
			fgets(champ_name, MAX_NAME, file_pointer);
			cham_node = find_node(qtree->root, champ_name);
			// ���̵�X
			if (player->difficulty == 0) {
				// ���ұ� X
				if (player->assassin == 0 && player->warrior == 0 && player->wizard == 0 && player->supporter == 0 && player->tanker == 0 && player->ad == 0) {
					strncpy(player->name[player->num_cham], cham_node->name, sizeof(player->name[player->num_cham]));
					player->num_cham++;
				}
				// ���ұ� O
				else {
					if (player->assassin == 1) {
						if (cham_node->assassin == 0)
							continue;
					}
					if (player->warrior == 1) {
						if (cham_node->warrior == 0)
							continue;
					}
					if (player->wizard == 1) {
						if (cham_node->wizard == 0)
							continue;
					}
					if (player->supporter == 1) {
						if (cham_node->supporter == 0)
							continue;
					}
					if (player->tanker == 1) {
						if (cham_node->tanker == 0)
							continue;
					}
					if (player->ad == 1) {
						if (cham_node->ad == 0)
							continue;
					}
					strncpy(player->name[player->num_cham], cham_node->name, sizeof(player->name[player->num_cham]));
					player->num_cham++;
				}
			}
			//���̵�O
			else {
				int a, b;
				if (player->difficulty == 8) {
					a = 10;
					b = 8;
				}
				if (player->difficulty == 5) {
					a = 7;
					b = 5;
				}
				if (player->difficulty == 1) {
					a = 4;
					b = 1;
				}
				if (cham_node->difficulty <= a && cham_node->difficulty >= b) {
					// ���ұ� X
					if (player->assassin == 0 && player->warrior == 0 && player->wizard == 0 && player->supporter == 0 && player->tanker == 0 && player->ad == 0) {
						strncpy(player->name[player->num_cham], cham_node->name, sizeof(player->name[player->num_cham]));
						player->num_cham++;
					}
					// ���ұ� O
					else {
						int x = 1;
						if (player->assassin == 1) {
							if (cham_node->assassin == 0)
								continue;
						}
						if (player->warrior == 1) {
							if (cham_node->warrior == 0)
								continue;
						}
						if (player->wizard == 1) {
							if (cham_node->wizard == 0)
								continue;
						}
						if (player->supporter == 1) {
							if (cham_node->supporter == 0)
								continue;
						}
						if (player->tanker == 1) {
							if (cham_node->tanker == 0)
								continue;
						}
						if (player->ad == 1) {
							if (cham_node->ad == 0)
								continue;
						}
						strncpy(player->name[player->num_cham], cham_node->name, sizeof(player->name[player->num_cham]));
						player->num_cham++;
					}
				}
			}
		}
		fclose(file_pointer);
	} //

	// ���� ����X -> ��� è�Ǿ� ��ȸ
	else {
		FILE * file_pointer = NULL;
		file_pointer = fopen("All.txt", "r");
		for (int i = 0; i < Champ_num; i++) {
			fgets(champ_name, MAX_NAME, file_pointer);
			cham_node = find_node(qtree->root, champ_name);
			// ���̵�X
			if (player->difficulty == 0) {
				// ���ұ� X
				if (player->assassin == 0 && player->warrior == 0 && player->wizard == 0 && player->supporter == 0 && player->tanker == 0 && player->ad == 0) {
					strncpy(player->name[player->num_cham], cham_node->name, sizeof(player->name[player->num_cham]));
					player->num_cham++;
				}
				// ���ұ� O
				else {
					if (player->assassin == 1) {
						if (cham_node->assassin == 0)
							continue;
					}
					if (player->warrior == 1) {
						if (cham_node->warrior == 0)
							continue;
					}
					if (player->wizard == 1) {
						if (cham_node->wizard == 0)
							continue;
					}
					if (player->supporter == 1) {
						if (cham_node->supporter == 0)
							continue;
					}
					if (player->tanker == 1) {
						if (cham_node->tanker == 0)
							continue;
					}
					if (player->ad == 1) {
						if (cham_node->ad == 0)
							continue;
					}
					strncpy(player->name[player->num_cham], cham_node->name, sizeof(player->name[player->num_cham]));
					player->num_cham++;
				}
			}
			//���̵�O
			else {
				int a, b;
				if (player->difficulty == 8) {
					a = 10;
					b = 8;
				}
				if (player->difficulty == 5) {
					a = 7;
					b = 5;
				}
				if (player->difficulty == 1) {
					a = 4;
					b = 1;
				}
				if (cham_node->difficulty <= a && cham_node->difficulty >= b) {
					// ���ұ� X
					if (player->assassin == 0 && player->warrior == 0 && player->wizard == 0 && player->supporter == 0 && player->tanker == 0 && player->ad == 0) {
						strncpy(player->name[player->num_cham], cham_node->name, sizeof(player->name[player->num_cham]));
						player->num_cham++;
					}
					// ���ұ� O
					else {
						if (player->assassin == 1) {
							if (cham_node->assassin == 0)
								continue;
						}
						if (player->warrior == 1) {
							if (cham_node->warrior == 0)
								continue;
						}
						if (player->wizard == 1) {
							if (cham_node->wizard == 0)
								continue;
						}
						if (player->supporter == 1) {
							if (cham_node->supporter == 0)
								continue;
						}
						if (player->tanker == 1) {
							if (cham_node->tanker == 0)
								continue;
						}
						if (player->ad == 1) {
							if (cham_node->ad == 0)
								continue;
						}
						strncpy(player->name[player->num_cham], cham_node->name, sizeof(player->name[player->num_cham]));
						player->num_cham++;
					}
				}
			}
		}
		fclose(file_pointer);
	}
}

// end - 2. è�Ǿ� ��õ

// -------------------------------------------------------------

// 3. è�Ǿ� ���丮 �� ����
void Story_Strategy()
{
	system("cls");
	picture();
	int i;
	int key;
	while (TRUE) {
		system("cls");
		Story_Strategy_display();
		key = _getch();
		if (key == '1') {

			system("cls");
			picture();

			gotoxy(30, 10);
			printf("��");
			for (i = 0; i < 33; i++)
				printf("��");
			printf("��");
			gotoxy(30, 12);
			printf("��");
			for (i = 0; i < 33; i++)
				printf("��");
			printf("��");

			gotoxy(30, 11);
			printf("�� è�Ǿ� ���� ����Ʈ�� �̵��մϴ�!��\n");
			Sleep(3000);
			system("start http://gameinfo.leagueoflegends.co.kr/ko/game-info/champions/");
			_getch();
		}
		else if (key == '2') {
			system("cls");
			story_display();
			print_story();
		}
		else if (key == '3') {
			system("cls");
			picture();

			gotoxy(30, 10);
			printf("��");
			for (i = 0; i < 35; i++)
				printf("��");
			printf("��");
			gotoxy(30, 12);
			printf("��");
			for (i = 0; i < 35; i++)
				printf("��");
			printf("��");
			gotoxy(30, 11);
			printf("��  è�Ǿ� �м� ����Ʈ�� �̵��մϴ�! ��");
			Sleep(3000);
			system("start https://www.op.gg/champion/statistics");
			_getch();
		}
		else if (key == '4') {
			system("cls");
			strategy_display();
			print_strategy();
		}
		else if (key == '0') {
			break;
		}
		else {
		}
	}
}

// ���丮 �� ���� ���÷���
void Story_Strategy_display()
{
	int i;
	int x, y = 0;
	picture();
	gotoxy(35,5);
	printf("��");
	for (i = 0; i < 30; i++)
		printf("��");
	printf("��");
	x = 35;
	y = 5;
	for (i = 0; i < 12; i++)
	{
		y++;
		gotoxy(x, y);
		printf("��");
	}
	x = 66;
	y = 5;
	for (i = 0; i < 12; i++)
	{
		y++;
		gotoxy(x, y);
		printf("��");
	}
	gotoxy(40, 6);
	printf(" 1. è�Ǿ� ���� ����Ʈ\n");
	gotoxy(40, 8);
	printf(" 2. è�Ǿ� ���丮 ����\n");
	gotoxy(40, 10);
	printf(" 3. è�Ǿ� �м� ����Ʈ\n");
	gotoxy(40,12);
	printf(" 4. ���� è�Ǿ� ����\n");
	gotoxy(35,14);
	printf("��");
	for (i = 0; i < 30; i++)
		printf("��");
	printf("��");
	gotoxy(40,16);
	printf(" 0. �ڷ� ����\n");
	gotoxy(35,18);
	printf("��");
	for (i = 0; i < 30; i++)
		printf("��");
	printf("��");

}

// 2) ���丮 ���÷���
void story_display()
{
	int i;
	int x = 0, y = 0;
	picture();
	gotoxy(3, 2);
	printf("��");
	for (i = 0; i < 93; i++)
		printf("��");
	printf("��");
	x = 3;
	y = 2;
	for (i = 0; i < 19; i++)
	{
		y++;
		gotoxy(x, y);
		printf("��");
	}
	x = 97;
	y = 2;
	for (i = 0; i < 19; i++)
	{
		y++;
		gotoxy(x, y);
		printf("��");
	}
	gotoxy(26, 3);
	printf("���丮 ���⸦ ���ϴ� è�Ǿ��� �Է��ϼ���!\n");
	gotoxy(84, 3);
	printf(" 0. �ڷ� ����\n");
	gotoxy(5,5);
	printf("���� ������ ���÷�ũ �׶󰡽� �׷��̺��� ���� ���� ������ ��ƿ���� ���� ������ ������ �ϴ޸�");
	gotoxy(5, 6);
	printf("���� �ٸ��콺 ���ֳ̾� �巹�̺� ������ ��ĭ ���ӽ� ���� ���� ������ ������ ������ ����");
	gotoxy(5, 7);
	printf("��þ� ��� ����� ���� ���� ������ ������ �� ����ī�� ������ ������Ʈ ��ī���� �𸣰���");
	gotoxy(5, 8);
	printf("���� �ڻ� �̽� ���� �ٵ� �ٷ罺 ���� ���̰� ���� ������ �������� ���� �귣�� ����̸�");
	gotoxy(5, 9);
	printf("����ũ��ũ ���丣 �ǻ� ���̿� ���Ϸ��� ���� ���־ƴ� �ҳ� �Ҷ�ī �� ���ٳ� ������ ��ī��");
	gotoxy(5, 10);
	printf("�ú� �� ¥�� �ŵ�� ������ ������ �Ƹ� �ƹ��� �ƿ췼���� �� ���̹� ������ ��Į�� ��Ʈ�Ͻ�");
	gotoxy(5, 11);
	printf("�˸���Ÿ �ִ� �ִϺ�� �ֽ� �߽��� ���� ������ ���� ���� �����Ƴ� �ö��� �丯 ��� �츣��");
	gotoxy(5, 12);
	printf("���� ���� �̷����� �̺� ����� �϶���� �ڸ��� 4�� �ھ� ���̶� ��ũ �ܳ� �轺 ����");
	gotoxy(5, 13);
	printf("���� ���̽� ���� ���� �� ������ ¡ũ�� �ʰ��� ī���� ī�� ī��� ī���� ī�ÿ���� ī�̻�");
	gotoxy(5, 14);
	printf("ī���� īŸ���� Į����Ÿ �ɳ� ����Ʋ�� ���� ���� �ڱ׸� �ڸ�Ű �� Ŭ���� Ų�巹�� Ÿ�� Ż��");
	gotoxy(5, 15);
	printf("Ż���� Ž ��ġ Ʈ���� Ʈ����Ÿ�� Ʈ���ٹ̾� Ʈ����Ƽ�� ����Ʈ Ʈ��ġ Ƽ�� ����ũ ���׿�");
	gotoxy(5, 16);
	printf("�ǵ齺ƽ �ǿ��� ���� ���̸ӵ��� ��ī��");
	
	gotoxy(3, 4);
	printf("��");
	for (i = 0; i < 93; i++)
		printf("��");
	printf("��");

	gotoxy(3, 18);
	printf("��");
	for (i = 0; i < 93; i++)
		printf("��");
	printf("��");
	gotoxy(35, 20);
	printf(">> ");
	
	gotoxy(3, 22);
	printf("��");
	for (i = 0; i < 93; i++)
		printf("��");
	printf("��");

	gotoxy(83, 2);
	printf("��");
	gotoxy(83, 3);
	printf("��");
	gotoxy(83, 4);
	printf("��");
	gotoxy(38, 20);
}

// 2) ���丮 ����Ʈ ����
void print_story()
{
	int i;
	int x = 0, y = 0;
	char champ[MAX_NAME];
	scanf("%s", champ);
	system("cls");
	picture();

	gotoxy(22, 4);
	printf("��");
	for (i = 0; i < 48; i++)
		printf("��");
	printf("��");
	x = 22;
	y = 4;
	for (i = 0; i < 1; i++)
	{
		y++;
		gotoxy(x, y);
		printf("��");
	}
	x = 71;
	y = 4;
	for (i = 0; i < 1; i++)
	{
		y++;
		gotoxy(x, y);
		printf("��");
	}
	gotoxy(22, 6);
	printf("��");
	for (i = 0; i < 48; i++)
		printf("��");
	printf("��");
	gotoxy(24, 5);
	if (strcmp(champ, "����") == 0) {
		printf("������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Garen");
		_getch();
	}
	else if (strcmp(champ, "������") == 0) {
		printf("�������� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/galio");
		_getch();
	}
	else if (strcmp(champ, "���÷�ũ") == 0 || strcmp(champ, "����") == 0) {
		printf("���÷�ũ�� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/gangplank");
		_getch();
	}
	else if (strcmp(champ, "�׶󰡽�") == 0 || strcmp(champ, "�۰�") == 0) {
		printf("�׶󰡽��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/gragas");
		_getch();
	}
	else if (strcmp(champ, "�׷��̺���") == 0 || strcmp(champ, "�׺�") == 0) {
		printf("�׷��̺����� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/graves");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/nami");
		_getch();
	}
	else if (strcmp(champ, "������") == 0) {
		printf("�������� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/nasus");
		_getch();
	}
	else if (strcmp(champ, "��ƿ����") == 0 || strcmp(champ, "��ƿ") == 0) {
		printf("��ƿ������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/nautilus");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/nocturne");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/nunu");
		_getch();
	}
	else if (strcmp(champ, "�ϴ޸�") == 0) {
		printf("�ϴ޸��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/nidalee");
		_getch();
	}
	else if (strcmp(champ, "�ٸ��콺") == 0 || strcmp(champ, "�ٸ�") == 0) {
		printf("�ٸ��콺�� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/darius");
		_getch();
	}
	else if (strcmp(champ, "���ֳ̾�") == 0) {
		printf("���ֳ̾��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/diana");
		_getch();
	}
	else if (strcmp(champ, "�巹�̺�") == 0) {
		printf("�巹�̺��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/draven");
		_getch();
	}
	else if (strcmp(champ, "������") == 0) {
		printf("�������� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/ryze");
		_getch();
	}
	else if (strcmp(champ, "���ӽ�") == 0) {
		printf("���ӽ��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/rammus");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/lux");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/rumble");
		_getch();
	}
	else if (strcmp(champ, "������") == 0) {
		printf("�������� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/renekton");
		_getch();
	}
	else if (strcmp(champ, "������") == 0) {
		printf("�������� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Leona");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Rengar");
		_getch();
	}
	else if (strcmp(champ, "��þ�") == 0) {
		printf("��þ��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Lucian");
		_getch();
	}
	else if (strcmp(champ, "���") == 0) {
		printf("����� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Lulu");
		_getch();
	}
	else if (strcmp(champ, "�����") == 0) {
		printf("������� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/LeBlanc");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("�� ���� ���丮 ����Ʈ�� �̵��մϴ�!\n"); 
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/LeeSin");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Riven");
		_getch();
	}
	else if (strcmp(champ, "������") == 0 || strcmp(champ, "����") == 0) {
		printf("�������� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Lissandra");
		_getch();
	}
	else if (strcmp(champ, "��������") == 0 || strcmp(champ, "����") == 0) {
		printf("������ ���� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/MasterYi");
		_getch();
	}
	else if (strcmp(champ, "����ī��") == 0) {
		printf("����ī���� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Maokai");
		_getch();
	}
	else if (strcmp(champ, "������") == 0) {
		printf("�������� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Malzahar");
		_getch();
	}
	else if (strcmp(champ, "������Ʈ") == 0 || strcmp(champ, "����") == 0) {
		printf("������Ʈ�� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Malphite");
		_getch();
	}
	else if (strcmp(champ, "��ī����") == 0 || strcmp(champ, "��") == 0) {
		printf("��ī������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Mordekaiser");
		_getch();
	}
	else if (strcmp(champ, "�𸣰���") == 0 || strcmp(champ, "����") == 0) {
		printf("�𸣰����� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Morgana");
		_getch();
	}
	else if (strcmp(champ, "�����ڻ�") == 0 || strcmp(champ, "����") == 0) {
		printf("�����ڻ��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/DrMundo");
		_getch();
	}
	else if (strcmp(champ, "�̽�����") == 0 || strcmp(champ, "����") == 0) {
		printf("�̽� ������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/MissFortune");
		_getch();
	}
	else if (strcmp(champ, "�ٷ罺") == 0) {
		printf("�ٷ罺�� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Varus");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Vi");
		_getch();
	}
	else if (strcmp(champ, "���̰�") == 0) {
		printf("���̰��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Veigar");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Vayne");
		_getch();
	}
	else if (strcmp(champ, "��������") == 0 || strcmp(champ, "����") == 0) {
		printf("���������� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Volibear");
		_getch();
	}
	else if (strcmp(champ, "�귣��") == 0) {
		printf("�귣���� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Brand");
		_getch();
	}
	else if (strcmp(champ, "����̸�") == 0 || strcmp(champ, "����") == 0) {
		printf("����̸��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Vladimir");
		_getch();
	}
	else if (strcmp(champ, "����ũ��ũ") == 0 || strcmp(champ, "����") == 0) {
		printf("����ũ��ũ�� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Blitzcrank");
		_getch();
	}
	else if (strcmp(champ, "���丣") == 0) {
		printf("���丣�� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Viktor");
		_getch();
	}
	else if (strcmp(champ, "�ǻ�") == 0) {
		printf("�ǻ��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Poppy");
		_getch();
	}
	else if (strcmp(champ, "���̿�") == 0) {
		printf("���̿��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Sion");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Shaco");
		_getch();
	}
	else if (strcmp(champ, "���־ƴ�") == 0 || strcmp(champ, "����") == 0) {
		printf("���־ƴ��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Sejuani");
		_getch();
	}
	else if (strcmp(champ, "�ҳ�") == 0) {
		printf("�ҳ��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Sona");
		_getch();
	}
	else if (strcmp(champ, "�Ҷ�ī") == 0) {
		printf("�Ҷ�ī�� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Soraka");
		_getch();
	}
	else if (strcmp(champ, "��") == 0) {
		printf("���� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Shen");
		_getch();
	}
	else if (strcmp(champ, "���ٳ�") == 0) {
		printf("���ٳ��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Shyvana");
		_getch();
	}
	else if (strcmp(champ, "������") == 0) {
		printf("�������� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Swain");
		_getch();
	}
	else if (strcmp(champ, "��ī��") == 0) {
		printf("��ī���� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Skarner");
		_getch();
	}
	else if (strcmp(champ, "�ú�") == 0) {
		printf("�ú��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Sivir");
		_getch();
	}
	else if (strcmp(champ, "��¥��") == 0) {
		printf("�� ¥���� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/XinZhao");
		_getch();
	}
	else if (strcmp(champ, "�ŵ��") == 0) {
		printf("�ŵ���� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Syndra");
		_getch();
	}
	else if (strcmp(champ, "������") == 0) {
		printf("�������� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Singed");
		_getch();
	}
	else if (strcmp(champ, "������") == 0) {
		printf("�������� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Thresh");
		_getch();
	}
	else if (strcmp(champ, "�Ƹ�") == 0) {
		printf("�Ƹ��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Ahri");
		_getch();
	}
	else if (strcmp(champ, "�ƹ���") == 0 || strcmp(champ, "����") == 0) {
		printf("�ƹ����� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Amumu");
		_getch();
	}
	else if (strcmp(champ, "��Į��") == 0) {
		printf("��Į���� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Akali");
		_getch();
	}
	else if (strcmp(champ, "��Ʈ�Ͻ�") == 0 || strcmp(champ, "��Ʈ") == 0) {
		printf("��Ʈ�Ͻ��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Aatrox");
		_getch();
	}
	else if (strcmp(champ, "�˸���Ÿ") == 0 || strcmp(champ, "�˸�") == 0) {
		printf("�˸���Ÿ�� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Alistar");
		_getch();
	}
	else if (strcmp(champ, "�ִ�") == 0) {
		printf("�ִ��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Annie");
		_getch();
	}
	else if (strcmp(champ, "�ִϺ��") == 0) {
		printf("�ִϺ���� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Anivia");
		_getch();
	}
	else if (strcmp(champ, "�ֽ�") == 0) {
		printf("�ֽ��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Ashe");
		_getch();
	}
	else if (strcmp(champ, "������") == 0) {
		printf("�������� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Elise");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Wukong");
		_getch();
	}
	else if (strcmp(champ, "�����Ƴ�") == 0 || strcmp(champ, "����") == 0) {
		printf("�����Ƴ��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Orianna");
		_getch();
	}
	else if (strcmp(champ, "�ö���") == 0) {
		printf("�ö����� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Olaf");
		_getch();
	}
	else if (strcmp(champ, "�丯") == 0) {
		printf("�丯�� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Yorick");
		_getch();
	}
	else if (strcmp(champ, "���") == 0) {
		printf("����� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Udyr");
		_getch();
	}
	else if (strcmp(champ, "�츣��") == 0) {
		printf("�츣���� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Urgot");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Warwick");
		_getch();
	}
	else if (strcmp(champ, "�̷�����") == 0 || strcmp(champ, "�̷�") == 0) {
		printf("�̷������� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Irelia");
		_getch();
	}
	else if (strcmp(champ, "�̺�") == 0) {
		printf("�̺��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Evelynn");
		_getch();
	}
	else if (strcmp(champ, "�����") == 0 || strcmp(champ, "����") == 0) {
		printf("������� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Ezreal");
		_getch();
	}
	else if (strcmp(champ, "�ڸ���4��") == 0 || strcmp(champ, "�߹�") == 0) {
		printf("�ڸ��� 4���� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/JarvanIV");
		_getch();
	}
	else if (strcmp(champ, "���̶�") == 0) {
		printf("���̶��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Zyra");
		_getch();
	}
	else if (strcmp(champ, "��ũ") == 0) {
		printf("��ũ�� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Zac");
		_getch();
	}
	else if (strcmp(champ, "�ܳ�") == 0) {
		printf("�ܳ��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Janna");
		_getch();
	}
	else if (strcmp(champ, "�轺") == 0) {
		printf("�轺�� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Jax");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Zed");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Xerath");
		_getch();
	}
	else if (strcmp(champ, "���̽�") == 0) {
		printf("���̽��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Jayce");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Ziggs");
		_getch();
	}
	else if (strcmp(champ, "������") == 0) {
		printf("�������� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Zilean");
		_getch();
	}
	else if (strcmp(champ, "¡ũ��") == 0) {
		printf("¡ũ���� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Jinx");
		_getch();
	}
	else if (strcmp(champ, "�ʰ���") == 0) {
		printf("�ʰ����� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/ChoGath");
		_getch();
	}
	else if (strcmp(champ, "ī����") == 0) {
		printf("ī������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Karma");
		_getch();
	}
	else if (strcmp(champ, "ī���") == 0) {
		printf("ī����� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Kassadin");
		_getch();
	}
	else if (strcmp(champ, "ī����") == 0) {
		printf("ī������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Karthus");
		_getch();
	}
	else if (strcmp(champ, "ī�ÿ����") == 0 || strcmp(champ, "ī��") == 0) {
		printf("ī�ÿ������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Cassiopeia");
		_getch();
	}
	else if (strcmp(champ, "ī����") == 0) {
		printf("ī������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/KhaZix");
		_getch();
	}
	else if (strcmp(champ, "īŸ����") == 0 || strcmp(champ, "īŸ") == 0) {
		printf("īŸ������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Katarina");
		_getch();
	}
	else if (strcmp(champ, "�ɳ�") == 0) {
		printf("�ɳ��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Kennen");
		_getch();
	}
	else if (strcmp(champ, "����Ʋ��") == 0 || strcmp(champ, "��Ʋ") == 0) {
		printf("����Ʋ���� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Caitlyn");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Kayle");
		_getch();
	}
	else if (strcmp(champ, "�ڱ׸�") == 0) {
		printf("�ڱ׸��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/KogMaw");
		_getch();
	}
	else if (strcmp(champ, "�ڸ�Ű") == 0) {
		printf("�ڸ�Ű ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Corki");
		_getch();
	}
	else if (strcmp(champ, "��") == 0) {
		printf("���� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Quinn");
		_getch();
	}
	else if (strcmp(champ, "Ÿ��") == 0) {
		printf("Ÿ���� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Taric");
		_getch();
	}
	else if (strcmp(champ, "Ż��") == 0) {
		printf("Ż���� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Talon");
		_getch();
	}
	else if (strcmp(champ, "Ʈ����") == 0) {
		printf("Ʈ������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Trundle");
		_getch();
	}
	else if (strcmp(champ, "Ʈ����Ÿ��") == 0 || strcmp(champ, "ƮŸ") == 0) {
		printf("Ʈ����Ÿ���� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Tristana");
		_getch();
	}
	else if (strcmp(champ, "Ʈ���ٹ̾�") == 0 || strcmp(champ, "Ʈ��") == 0) {
		printf("Ʈ���ٹ̾��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Tryndamere");
		_getch();
	}
	else if (strcmp(champ, "Ʈ����Ƽ������Ʈ") == 0 || strcmp(champ, "Ʈ��") == 0) {
		printf("Ʈ����Ƽ�� ����Ʈ�� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/TwistedFate");
		_getch();
	}
	else if (strcmp(champ, "Ʈ��ġ") == 0) {
		printf("Ʈ��ġ�� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Twitch");
		_getch();
	}
	else if (strcmp(champ, "Ƽ��") == 0) {
		printf("Ƽ���� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Teemo");
		_getch();
	}
	else if (strcmp(champ, "���׿�") == 0) {
		printf("���׿��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Pantheon");
		_getch();
	}
	else if (strcmp(champ, "�ǵ齺ƽ") == 0 || strcmp(champ, "�ǵ�") == 0) {
		printf("�ǵ齺ƽ�� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Fiddlesticks");
		_getch();
	}
	else if (strcmp(champ, "�ǿ���") == 0) {
		printf("�ǿ����� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Fiora");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Fizz");
		_getch();
	}
	else if (strcmp(champ, "���̸ӵ���") == 0 || strcmp(champ, "����") == 0) {
		printf("���̸ӵ����� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Heimerdinger");
		_getch();
	}
	else if (strcmp(champ, "��ī��") == 0) {
		printf("��ī���� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Hecarim");
		_getch();
	}
	else if (strcmp(champ, "�߽���") == 0) {
		printf("�߽����� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Yasuo");
		_getch();
	}
	else if (strcmp(champ, "������") == 0) {
		printf("�������� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/VelKoz");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Braum");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Gnar");
		_getch();
	}
	else if (strcmp(champ, "������") == 0) {
		printf("�������� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Azir");
		_getch();
	}
	else if (strcmp(champ, "Į����Ÿ") == 0 || strcmp(champ, "Į��") == 0) {
		printf("Į����Ÿ�� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Kalista");
		_getch();
	}
	else if (strcmp(champ, "������") == 0) {
		printf("�������� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/RekSai");
		_getch();
	}
	else if (strcmp(champ, "�ٵ�") == 0) {
		printf("�ٵ��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Bard");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Ekko");
		_getch();
	}
	else if (strcmp(champ, "Ž��ġ") == 0 || strcmp(champ, "��ġ") == 0) {
		printf("Ž ��ġ�� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/TahmKench");
		_getch();
	}
	else if (strcmp(champ, "Ų�巹��") == 0 || strcmp(champ, "Ų��") == 0) {
		printf("Ų�巹���� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Kindred");
		_getch();
	}
	else if (strcmp(champ, "�϶����") == 0 || strcmp(champ, "�϶�") == 0) {
		printf("�϶������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Illaoi");
		_getch();
	}
	else if (strcmp(champ, "��") == 0) {
		printf("���� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Jhin");
		_getch();
	}
	else if (strcmp(champ, "�ƿ췼���¼�") == 0 || strcmp(champ, "�Ƽ�") == 0) {
		printf("�ƿ췼���� ���� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/AurelionSol");
		_getch();
	}
	else if (strcmp(champ, "Ż����") == 0) {
		printf("Ż������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Taliyah");
		_getch();
	}
	else if (strcmp(champ, "Ŭ����") == 0) {
		printf("Ŭ������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Kled");
		_getch();
	}
	else if (strcmp(champ, "���̹�") == 0) {
		printf("���̹��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Ivern");
		_getch();
	}
	else if (strcmp(champ, "ī��") == 0) {
		printf("ī���� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Camille");
		_getch();
	}
	else if (strcmp(champ, "�ھ�") == 0) {
		printf("�ھ��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Xayah");
		_getch();
	}
	else if (strcmp(champ, "��ĭ") == 0) {
		printf("��ĭ�� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Rakan");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Ornn");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Zoe");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Kayn");
		_getch();
	}
	else if (strcmp(champ, "ī�̻�") == 0) {
		printf("ī�̻��� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/KaiSa");
		_getch();
	}
	else if (strcmp(champ, "����ũ") == 0) {
		printf("����ũ�� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Pyke");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Neeko");
		_getch();
	}
	else if (strcmp(champ, "���Ϸ���") == 0) {
		printf("���Ϸ����� ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Sylas");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���丮 ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Yuumi");
		_getch();
	}
	else if (strcmp(champ, "0") == 0) {
		Story_Strategy_display();
	}
	else {
		strategy_display();
		print_strategy();
	}

}

// 3) ���� ���÷���
void strategy_display()
{
	int i;
	int x = 0, y = 0;
	picture();
	gotoxy(3, 2);
	printf("��");
	for (i = 0; i < 93; i++)
		printf("��");
	printf("��");
	x = 3;
	y = 2;
	for (i = 0; i < 19; i++)
	{
		y++;
		gotoxy(x, y);
		printf("��");
	}
	x = 97;
	y = 2;
	for (i = 0; i < 19; i++)
	{
		y++;
		gotoxy(x, y);
		printf("��");
	}
	gotoxy(26, 3);
	printf("���� ���⸦ ���ϴ� è�Ǿ��� �Է��ϼ���!\n");
	gotoxy(84, 3);
	printf(" 0. �ڷ� ����\n");
	gotoxy(5, 5);
	printf("���� ������ ���÷�ũ �׶󰡽� �׷��̺��� ���� ���� ������ ��ƿ���� ���� ������ ������ �ϴ޸�");
	gotoxy(5, 6);
	printf("���� �ٸ��콺 ���ֳ̾� �巹�̺� ������ ��ĭ ���ӽ� ���� ���� ������ ������ ������ ����");
	gotoxy(5, 7);
	printf("��þ� ��� ����� ���� ���� ������ ������ �� ����ī�� ������ ������Ʈ ��ī���� �𸣰���");
	gotoxy(5, 8);
	printf("���� �ڻ� �̽� ���� �ٵ� �ٷ罺 ���� ���̰� ���� ������ �������� ���� �귣�� ����̸�");
	gotoxy(5, 9);
	printf("����ũ��ũ ���丣 �ǻ� ���̿� ���Ϸ��� ���� ���־ƴ� �ҳ� �Ҷ�ī �� ���ٳ� ������ ��ī��");
	gotoxy(5, 10);
	printf("�ú� �� ¥�� �ŵ�� ������ ������ �Ƹ� �ƹ��� �ƿ췼���� �� ���̹� ������ ��Į�� ��Ʈ�Ͻ�");
	gotoxy(5, 11);
	printf("�˸���Ÿ �ִ� �ִϺ�� �ֽ� �߽��� ���� ������ ���� ���� �����Ƴ� �ö��� �丯 ��� �츣��");
	gotoxy(5, 12);
	printf("���� ���� �̷����� �̺� ����� �϶���� �ڸ��� 4�� �ھ� ���̶� ��ũ �ܳ� �轺 ����");
	gotoxy(5, 13);
	printf("���� ���̽� ���� ���� �� ������ ¡ũ�� �ʰ��� ī���� ī�� ī��� ī���� ī�ÿ���� ī�̻�");
	gotoxy(5, 14);
	printf("ī���� īŸ���� Į����Ÿ �ɳ� ����Ʋ�� ���� ���� �ڱ׸� �ڸ�Ű �� Ŭ���� Ų�巹�� Ÿ�� Ż��");
	gotoxy(5, 15);
	printf("Ż���� Ž ��ġ Ʈ���� Ʈ����Ÿ�� Ʈ���ٹ̾� Ʈ����Ƽ�� ����Ʈ Ʈ��ġ Ƽ�� ����ũ ���׿�");
	gotoxy(5, 16);
	printf("�ǵ齺ƽ �ǿ��� ���� ���̸ӵ��� ��ī��");

	gotoxy(3, 4);
	printf("��");
	for (i = 0; i < 93; i++)
		printf("��");
	printf("��");

	gotoxy(3, 18);
	printf("��");
	for (i = 0; i < 93; i++)
		printf("��");
	printf("��");
	gotoxy(35, 20);
	printf(">> ");

	gotoxy(3, 22);
	printf("��");
	for (i = 0; i < 93; i++)
		printf("��");
	printf("��");

	gotoxy(83, 2);
	printf("��");
	gotoxy(83, 3);
	printf("��");
	gotoxy(83, 4);
	printf("��");
	gotoxy(38, 20);
}

// 3) ���� ����Ʈ ����
void print_strategy()
{
	int i;
	int x = 0, y = 0;
	char champ[MAX_NAME];
	scanf("%s", champ);
	system("cls");
	picture();

	gotoxy(23, 4);
	printf("��");
	for (i = 0; i < 46; i++)
		printf("��");
	printf("��");
	x = 23;
	y = 4;
	for (i = 0; i < 1; i++)
	{
		y++;
		gotoxy(x, y);
		printf("��");
	}
	x = 70;
	y = 4;
	for (i = 0; i < 1; i++)
	{
		y++;
		gotoxy(x, y);
		printf("��");
	}
	gotoxy(23, 6);
	printf("��");
	for (i = 0; i < 46; i++)
		printf("��");
	printf("��");
	gotoxy(25, 5);
	if (strcmp(champ, "����") == 0) {
		printf("������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://www.op.gg/champion/Garen/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "������") == 0) {
		printf("�������� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://www.op.gg/champion/galio/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "���÷�ũ") == 0 || strcmp(champ, "����") == 0) {
		printf("���÷�ũ�� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/gangplank/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "�׶󰡽�") == 0 || strcmp(champ, "�۰�") == 0) {
		printf("�׶󰡽��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/gragas/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "�׷��̺���") == 0 || strcmp(champ, "�׺�") == 0) {
		printf("�׷��̺����� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://www.op.gg/champion/graves/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://www.op.gg/champion/nami/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "������") == 0) {
		printf("�������� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://www.op.gg/champion/nasus/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "��ƿ����") == 0 || strcmp(champ, "��ƿ") == 0) {
		printf("��ƿ������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/nautilus/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/nocturne/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "������������") == 0 || strcmp(champ, "����") == 0) {
		printf("������ �������� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/nunu/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "�ϴ޸�") == 0) {
		printf("�ϴ޸��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/nidalee/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "�ٸ��콺") == 0 || strcmp(champ, "�ٸ�") == 0) {
		printf("�ٸ��콺�� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/darius/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "���ֳ̾�") == 0) {
		printf("���ֳ̾��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/diana/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "�巹�̺�") == 0) {
		printf("�巹�̺��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/draven/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "������") == 0) {
		printf("�������� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/ryze/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "���ӽ�") == 0) {
		printf("���ӽ��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/rammus/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/lux/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/rumble/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "������") == 0) {
		printf("�������� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/renekton/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "������") == 0) {
		printf("�������� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Leona/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Rengar/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "��þ�") == 0) {
		printf("��þ��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Lucian/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "���") == 0) {
		printf("����� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Lulu/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "�����") == 0) {
		printf("������� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/LeBlanc/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("�� ���� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/LeeSin/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Riven/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "������") == 0 || strcmp(champ, "����") == 0) {
		printf("�������� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Lissandra/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "��������") == 0 || strcmp(champ, "����") == 0) {
		printf("������ ���� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/MasterYi/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "����ī��") == 0) {
		printf("����ī���� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Maokai/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "������") == 0) {
		printf("�������� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Malzahar/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "������Ʈ") == 0 || strcmp(champ, "����") == 0) {
		printf("������Ʈ�� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Malphite/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "��ī����") == 0 || strcmp(champ, "��") == 0) {
		printf("��ī������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Mordekaiser/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "�𸣰���") == 0 || strcmp(champ, "����") == 0) {
		printf("�𸣰����� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Morgana/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "�����ڻ�") == 0 || strcmp(champ, "����") == 0) {
		printf("�����ڻ��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/DrMundo/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "�̽�����") == 0 || strcmp(champ, "����") == 0) {
		printf("�̽� ������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/MissFortune/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "�ٷ罺") == 0) {
		printf("�ٷ罺�� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Varus/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Vi/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "���̰�") == 0) {
		printf("���̰��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Veigar/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Vayne/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "��������") == 0 || strcmp(champ, "����") == 0) {
		printf("���������� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Volibear/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "�귣��") == 0) {
		printf("�귣���� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Brand/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "����̸�") == 0 || strcmp(champ, "����") == 0) {
		printf("����̸��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Vladimir/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "����ũ��ũ") == 0 || strcmp(champ, "����") == 0) {
		printf("����ũ��ũ�� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Blitzcrank/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "���丣") == 0) {
		printf("���丣�� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Viktor/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "�ǻ�") == 0) {
		printf("�ǻ��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Poppy/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "���̿�") == 0) {
		printf("���̿��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Sion/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Shaco/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "���־ƴ�") == 0 || strcmp(champ, "����") == 0) {
		printf("���־ƴ��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Sejuani/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "�ҳ�") == 0) {
		printf("�ҳ��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Sona/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "�Ҷ�ī") == 0) {
		printf("�Ҷ�ī�� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Soraka/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "��") == 0) {
		printf("���� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Shen/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "���ٳ�") == 0) {
		printf("���ٳ��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Shyvana/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "������") == 0) {
		printf("�������� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Swain/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "��ī��") == 0) {
		printf("��ī���� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Skarner/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "�ú�") == 0) {
		printf("�ú��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Sivir/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "��¥��") == 0) {
		printf("�� ¥���� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://www.op.gg/champion/XinZhao/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "�ŵ��") == 0) {
		printf("�ŵ���� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(300);
		system("start https://www.op.gg/champion/Syndra/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "������") == 0) {
		printf("�������� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Singed/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "������") == 0) {
		printf("�������� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Thresh/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "�Ƹ�") == 0) {
		printf("�Ƹ��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Ahri/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "�ƹ���") == 0 || strcmp(champ, "����") == 0) {
		printf("�ƹ����� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Amumu/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "��Į��") == 0) {
		printf("��Į���� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Akali/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "��Ʈ�Ͻ�") == 0 || strcmp(champ, "��Ʈ") == 0) {
		printf("��Ʈ�Ͻ��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Aatrox/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "�˸���Ÿ") == 0 || strcmp(champ, "�˸�") == 0) {
		printf("�˸���Ÿ�� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Alistar/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "�ִ�") == 0) {
		printf("�ִ��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Annie/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "�ִϺ��") == 0) {
		printf("�ִϺ���� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://www.op.gg/champion/Anivia/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "�ֽ�") == 0) {
		printf("�ֽ��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Ashe/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "������") == 0) {
		printf("�������� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Elise/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Wukong/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "�����Ƴ�") == 0 || strcmp(champ, "����") == 0) {
		printf("�����Ƴ��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Orianna/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "�ö���") == 0) {
		printf("�ö����� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Olaf/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "�丯") == 0) {
		printf("�丯�� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Yorick/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "���") == 0) {
		printf("����� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Udyr/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "�츣��") == 0) {
		printf("�츣���� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Urgot/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Warwick/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "�̷�����") == 0 || strcmp(champ, "�̷�") == 0) {
		printf("�̷������� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Irelia/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "�̺�") == 0) {
		printf("�̺��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Evelynn/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "�����") == 0 || strcmp(champ, "����") == 0) {
		printf("������� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Ezreal/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "�ڸ���4��") == 0 || strcmp(champ, "�߹�") == 0) {
		printf("�ڸ��� 4���� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/JarvanIV/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "���̶�") == 0) {
		printf("���̶��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Zyra/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "��ũ") == 0) {
		printf("��ũ�� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Zac/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "�ܳ�") == 0) {
		printf("�ܳ��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Janna/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "�轺") == 0) {
		printf("�轺�� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Jax/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Zed/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Xerath/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "���̽�") == 0) {
		printf("���̽��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Jayce/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Ziggs/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "������") == 0) {
		printf("�������� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Zilean/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "¡ũ��") == 0) {
		printf("¡ũ���� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Jinx/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "�ʰ���") == 0) {
		printf("�ʰ����� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/ChoGath/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "ī����") == 0) {
		printf("ī������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Karma/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "ī���") == 0) {
		printf("ī����� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Kassadin/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "ī����") == 0) {
		printf("ī������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Karthus/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "ī�ÿ����") == 0 || strcmp(champ, "ī��") == 0) {
		printf("ī�ÿ������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Cassiopeia/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "ī����") == 0) {
		printf("ī������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/KhaZix/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "īŸ����") == 0 || strcmp(champ, "īŸ") == 0) {
		printf("īŸ������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Katarina/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "�ɳ�") == 0) {
		printf("�ɳ��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Kennen/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "����Ʋ��") == 0 || strcmp(champ, "��Ʋ") == 0) {
		printf("����Ʋ���� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Caitlyn/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Kayle/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "�ڱ׸�") == 0) {
		printf("�ڱ׸��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/KogMaw/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "�ڸ�Ű") == 0) {
		printf("�ڸ�Ű ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Corki/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "��") == 0) {
		printf("���� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Quinn/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "Ÿ��") == 0) {
		printf("Ÿ���� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Taric/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "Ż��") == 0) {
		printf("Ż���� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Talon/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "Ʈ����") == 0) {
		printf("Ʈ������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Trundle/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "Ʈ����Ÿ��") == 0 || strcmp(champ, "ƮŸ") == 0) {
		printf("Ʈ����Ÿ���� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Tristana/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "Ʈ���ٹ̾�") == 0 || strcmp(champ, "Ʈ��") == 0) {
		printf("Ʈ���ٹ̾��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Tryndamere/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "Ʈ����Ƽ������Ʈ") == 0 || strcmp(champ, "Ʈ��") == 0) {
		printf("Ʈ����Ƽ�� ����Ʈ�� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/TwistedFate/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "Ʈ��ġ") == 0) {
		printf("Ʈ��ġ�� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Twitch/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "Ƽ��") == 0) {
		printf("Ƽ���� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Teemo/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "���׿�") == 0) {
		printf("���׿��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Pantheon/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "�ǵ齺ƽ") == 0 || strcmp(champ, "�ǵ�") == 0) {
		printf("�ǵ齺ƽ�� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Fiddlesticks/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "�ǿ���") == 0) {
		printf("�ǿ����� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Fiora/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Fizz/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "���̸ӵ���") == 0 || strcmp(champ, "����") == 0) {
		printf("���̸ӵ����� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Heimerdinger/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "��ī��") == 0) {
		printf("��ī���� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Hecarim/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "�߽���") == 0) {
		printf("�߽����� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Yasuo/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "������") == 0) {
		printf("�������� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/VelKoz/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Braum/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Gnar/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "������") == 0) {
		printf("�������� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Azir/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "Į����Ÿ") == 0 || strcmp(champ, "Į��") == 0) {
		printf("Į����Ÿ�� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Kalista/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "������") == 0) {
		printf("�������� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/RekSai/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "�ٵ�") == 0) {
		printf("�ٵ��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Bard/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Ekko/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "Ž��ġ") == 0 || strcmp(champ, "��ġ") == 0) {
		printf("Ž ��ġ�� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000);
		system("start https://www.op.gg/champion/TahmKench/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "Ų�巹��") == 0 || strcmp(champ, "Ų��") == 0) {
		printf("Ų�巹���� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Kindred/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "�϶����") == 0 || strcmp(champ, "�϶�") == 0) {
		printf("�϶������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Illaoi/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "��") == 0) {
		printf("���� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Jhin/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "�ƿ췼���¼�") == 0 || strcmp(champ, "�Ƽ�") == 0) {
		printf("�ƿ췼���� ���� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/AurelionSol/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "Ż����") == 0) {
		printf("Ż������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Taliyah/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "Ŭ����") == 0) {
		printf("Ŭ������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Kled/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "���̹�") == 0) {
		printf("���̹��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Ivern/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "ī��") == 0) {
		printf("ī���� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Camille/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "�ھ�") == 0) {
		printf("�ھ��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Xayah/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "��ĭ") == 0) {
		printf("��ĭ�� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Rakan/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Ornn/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Zoe/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Kayn/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "ī�̻�") == 0) {
		printf("ī�̻��� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/KaiSa/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "����ũ") == 0) {
		printf("����ũ�� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Pyke/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Neeko/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "���Ϸ���") == 0) {
		printf("���Ϸ����� ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Sylas/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "����") == 0) {
		printf("������ ���� ����Ʈ�� �̵��մϴ�!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Yuumi/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "0") == 0) {
		Story_Strategy_display();
	}
	else {
		story_display();
		print_story();
	}

}

// end - 3. ���丮 and ����

// --------------------------------------------------------------------------

//4. ���� ��Ÿ �м�
void current_meta(Tree * LOL)
{
	int key;
	while (TRUE) {
		system("cls");
		meta_display();
		key = _getch();
		if (key == '1') {
			win_rate(LOL);
		}
		else if (key == '2') {
			pick_rate();
		}
		else if (key == '3') {
			ban_rate();
		}
		else if (key == '0') {
			break;
		}
		else {
		}
	}
}

// ��Ÿ �м� ���÷���
void meta_display()
{
	int i;
	int x = 0, y = 0;
	system("cls");
	picture();

	textcolor(WHITE, WHITE);

	//T
	gotoxy(5, 1);
	printf("������");
	gotoxy(9, 2);
	printf("��");
	gotoxy(9, 3);
	printf("��");
	gotoxy(9, 4);
	printf("��");
	gotoxy(9, 5);
	printf("��");
	gotoxy(9, 6);
	printf("��");
	gotoxy(9, 7);
	printf("��");
	//I
	gotoxy(16, 1);
	printf("������");
	gotoxy(21, 2);
	printf("��");
	gotoxy(21, 3);
	printf("��");
	gotoxy(21, 4);
	printf("��");
	gotoxy(21, 5);
	printf("��");
	gotoxy(21, 6);
	printf("��");
	gotoxy(16, 7);
	printf("������");
	//E
	gotoxy(27, 1);
	printf("������");
	gotoxy(27, 2);
	printf("��");
	gotoxy(27, 3);
	printf("��");
	gotoxy(27, 4);
	printf("������");
	gotoxy(27, 5);
	printf("��");
	gotoxy(27, 6);
	printf("��");
	gotoxy(27, 7);
	printf("������");
	//R
	gotoxy(39, 1);
	printf("������");
	gotoxy(39, 2);
	printf("��");
	gotoxy(47, 2);
	printf("��");
	gotoxy(39, 3);
	printf("��");
	gotoxy(47, 3);
	printf("��");
	gotoxy(39, 4);
	printf("�����");
	gotoxy(39, 5);
	printf("��");
	gotoxy(47, 5);
	printf("��");
	gotoxy(39, 6);
	printf("��");
	gotoxy(47, 6);
	printf("��");
	gotoxy(39, 7);
	printf("��");
	gotoxy(47, 7);
	printf("��");

	//L
	gotoxy(54, 1);
	printf("��");
	gotoxy(54, 2);
	printf("��");
	gotoxy(54, 3);
	printf("��");
	gotoxy(54, 4);
	printf("��");
	gotoxy(54, 5);
	printf("��");
	gotoxy(54, 6);
	printf("��");
	gotoxy(54, 7);
	printf("������");
	//I
	gotoxy(65, 1);
	printf("������");
	gotoxy(69, 2);
	printf("��");
	gotoxy(69, 3);
	printf("��");
	gotoxy(69, 4);
	printf("��");
	gotoxy(69, 5);
	printf("��");
	gotoxy(69, 6);
	printf("��");
	gotoxy(65, 7);
	printf("������");
	//S
	gotoxy(76, 1);
	printf("������");
	gotoxy(76, 2);
	printf("��");
	gotoxy(76, 3);
	printf("��");
	gotoxy(76, 4);
	printf("������");
	gotoxy(84, 5);
	printf("��");
	gotoxy(84, 6);
	printf("��");
	gotoxy(76, 7);
	printf("������");
	//T
	gotoxy(87, 1);
	printf("������");
	gotoxy(91, 2);
	printf("��");
	gotoxy(91, 3);
	printf("��");
	gotoxy(91, 4);
	printf("��");
	gotoxy(91, 5);
	printf("��");
	gotoxy(91, 6);
	printf("��");
	gotoxy(91, 7);
	printf("��");

	textcolor(LIGHTGRAY, BLACK);
	gotoxy(65,9);
	printf("-���� ��Ÿ �м�-");

	textcolor(RED, RED);
	//1.
	gotoxy(27, 11);
	printf("��");
	gotoxy(25, 12);
	printf("���");
	gotoxy(27, 13);
	printf("��");
	gotoxy(27, 14);
	printf("��");
	gotoxy(27, 15);
	printf("��");
	textcolor(DARKGRAY, BLACK);
	gotoxy(30, 15);
	printf("��");
	textcolor(RED, RED);
	//W
	y = 11;
	for (i = 0; i < 4; i++)
	{
		gotoxy(33, y);
		printf("��");
		gotoxy(37, y);
		printf("��");
		gotoxy(41, y);
		printf("��");
		y++;
	}
	gotoxy(35, 15);
	printf("��");
	gotoxy(39, 15);
	printf("��");
	//I
	gotoxy(45, 11);
	printf("������");
	gotoxy(49, 12);
	printf("��");
	gotoxy(49, 13);
	printf("��");
	gotoxy(49, 14);
	printf("��");
	gotoxy(45, 15);
	printf("������");
	//N
	y = 11;
	for (i = 0; i < 5; i++)
	{
		gotoxy(57, y);
		printf("��");
		gotoxy(65, y);
		printf("��");
		y++;
	}
	gotoxy(59, 12);
	printf("��");
	gotoxy(61, 13);
	printf("��");
	gotoxy(63, 14);
	printf("��");
	//2.
	textcolor(GREEN, GREEN);
	gotoxy(25, 17);
	printf("���");
	gotoxy(27, 18);
	printf("��");
	gotoxy(25, 19);
	printf("���");
	gotoxy(25, 20);
	printf("��");
	gotoxy(25, 21);
	printf("���");
	textcolor(DARKGRAY, BLACK);
	gotoxy(30, 21);
	printf("��");
	textcolor(GREEN, GREEN);
	//P
	gotoxy(33, 17);
	printf("�����");
	gotoxy(33, 18);
	printf("��");
	gotoxy(39, 18);
	printf("��");
	gotoxy(33, 19);
	printf("�����");
	gotoxy(33, 20);
	printf("��");
	gotoxy(33, 21);
	printf("��");
	//I
	gotoxy(43, 17);
	printf("����");
	gotoxy(45, 18);
	printf("��");
	gotoxy(45, 19);
	printf("��");
	gotoxy(45, 20);
	printf("��");
	gotoxy(43, 21);
	printf("����");
	//C
	gotoxy(51, 17);
	printf("����");
	gotoxy(51, 18);
	printf("��");
	gotoxy(51, 19);
	printf("��");
	gotoxy(51, 20);
	printf("��");
	gotoxy(51, 21);
	printf("����");
	//K
	gotoxy(59, 17);
	printf("��");
	gotoxy(65, 17);
	printf("��");
	gotoxy(59, 18);
	printf("��");
	gotoxy(63, 18);
	printf("��");
	gotoxy(59, 19);
	printf("���");
	gotoxy(59, 20);
	printf("��");
	gotoxy(63, 20);
	printf("��");
	gotoxy(59, 21);
	printf("��");
	gotoxy(65, 21);
	printf("��");
	//3.
	textcolor(BLUE, BLUE);
	gotoxy(25, 23);
	printf("���");
	gotoxy(27, 24);
	printf("��");
	gotoxy(25, 25);
	printf("���");
	gotoxy(27, 26);
	printf("��");
	gotoxy(25, 27);
	printf("���");
	textcolor(DARKGRAY, BLACK);
	gotoxy(30, 27);
	printf("��");
	textcolor(BLUE, BLUE);
	//B
	gotoxy(33, 23);
	printf("�����");
	gotoxy(33, 24);
	printf("��");
	gotoxy(41, 24);
	printf("��");
	gotoxy(33, 25);
	printf("�����");
	gotoxy(33, 26);
	printf("��");
	gotoxy(41, 26);
	printf("��");
	gotoxy(33, 27);
	printf("�����");
	//A
	gotoxy(47, 23);
	printf("����");
	y = 24;
	for (i = 0; i <4 ; i++)
	{
		gotoxy(45, y);
		printf("��");
		gotoxy(53, y);
		printf("��");
		y++;
	}
	gotoxy(45, 26);
	printf("������");

	//N
	y = 23;
	for (i = 0; i < 5; i++)
	{
		gotoxy(57, y);
		printf("��");
		gotoxy(65, y);
		printf("��");
		y++;
	}
	gotoxy(59, 24);
	printf("��");
	gotoxy(61, 25);
	printf("��");
	gotoxy(63, 26);
	printf("��");
	textcolor(LIGHTGRAY, BLACK);
	//----------------------------------
	gotoxy(44, 29);
	printf("0. Return");
}

// 1) �·� ������
void win_rate(Tree * LOL)
{
	system("cls");
	picture();
	int key;
	int i = 0;
	int x, y = 0;
	//--------------------------------
	gotoxy(30, 3);
	printf("��");
	for (i = 0; i < 30; i++)
		printf("��");
	printf("��");
	y = 3;
	for (i = 0; i < 3; i++)
	{
		y++;
		gotoxy(30, y);
		printf("��");
		gotoxy(61, y);
		printf("��");
	}
	gotoxy(30, 7);
	printf("��");
	for (i = 0; i < 30; i++)
		printf("��");
	printf("��");
	gotoxy(35, 5);
	printf("1. ��ü è�Ǿ� �·�");
	//-----------------------------------
	gotoxy(30, 9);
	printf("��");
	for (i = 0; i < 30; i++)
		printf("��");
	printf("��");
	y = 9;
	for (i = 0; i < 3; i++)
	{
		y++;
		gotoxy(30, y);
		printf("��");
		gotoxy(61, y);
		printf("��");
	}
	gotoxy(30,13);
	printf("��");
	for (i = 0; i < 30; i++)
		printf("��");
	printf("��");
	gotoxy(35,11);

	printf("2. ���κ� è�Ǿ� �·�");
	//-----------------------------------------------
	gotoxy(30, 17);
	printf("��");
	for (i = 0; i < 30; i++)
		printf("��");
	printf("��");
	y = 17;
	for (i = 0; i < 3; i++)
	{
		y++;
		gotoxy(30, y);
		printf("��");
		gotoxy(61, y);
		printf("��");
	}
	gotoxy(30, 21);
	printf("��");
	for (i = 0; i < 30; i++)
		printf("��");
	printf("��");
	gotoxy(35, 19);

	printf("0. �ڷ� ����");

	key = _getch();
	if (key == '1') {
		all_win_rate();
	}
	else if (key == '2') {
		line_win_rate(LOL);
	}
	else if (key == '0') {
	}
	else {
		win_rate(LOL);
	}
}

// 1-1) ��ü �·� ���
void all_win_rate()
{
	system("cls");
	picture();
	int i = 0;
	int x, y = 0;

	gotoxy(34, 4);
	printf("��");
	for (i = 0; i < 31; i++)
		printf("��");
	printf("��");
	
	y = 4;
	for (i = 0; i < 15; i++)
	{
		y++;
		gotoxy(34, y);
		printf("��");
		gotoxy(66, y);
		printf("��");
	}

	gotoxy(34, 19);
	printf("��");
	for (i = 0; i < 31; i++)
		printf("��");
	printf("��");

	gotoxy(34, 6);
	printf("��");
	for (i = 0; i < 31; i++)
		printf("��");
	printf("��");

	gotoxy(42, 6);
	printf("��");
	gotoxy(54, 6);
	printf("��");

	gotoxy(34, 8);
	printf("��");
	for (i = 0; i < 31; i++)
		printf("��");
	printf("��");

	y = 6;
	for (i = 0; i < 12; i++)
	{
		y++;
		gotoxy(42, y);
		printf("��");
		gotoxy(54, y);
		printf("��");
	}
	gotoxy(42, 8);
	printf("��");
	gotoxy(54, 8);
	printf("��");
	
	char champ_name[10][MAX_NAME] = { "�丯","���ӽ�","�ú�","������","������","¡ũ��","�ҳ�","Ŭ����","�ƹ���","Ÿ��" };
	double champ_win_rate[10] = { 54.15,54.01,53.42,53.40,53.31,53.29,53.22,53.12,42.98,52.82 };
	gotoxy(38, 5);
	printf("è�Ǿ� �·� ����(1~10��)");
	gotoxy(37,7);
	printf("����");
	gotoxy(46,7);
	printf("è�Ǿ�");
	gotoxy(59,7);
	printf("�·�");
	y = 8;
	for (int i = 0; i < 10; i++) {
		y++;
		gotoxy(38,y);
		printf("%d.", i + 1);
		gotoxy(45,y);
		printf("%s", champ_name[i]);
		gotoxy(58,y);
		printf("%.2lf%%", champ_win_rate[i]);
	}

	gotoxy(42, 19);
	printf("��");
	gotoxy(54, 19);
	printf("��");
	gotoxy(36, 20);
	if (_kbhit() != 0) system("pause");
	_getch();
}

// 1-2) ���κ� �·� ����
void line_win_rate(Tree * LOL)
{
	Node * play_node;
	system("cls");
	int key;
	picture();

	int i = 0;
	int x, y = 0;
	//ž
	gotoxy(39, 4);
	printf("��");
	for (i = 0; i < 20; i++)
		printf("��");
	printf("��");

	y = 4;
	for (i = 0; i < 3; i++)
	{
		y++;
		gotoxy(39, y);
		printf("��");
		gotoxy(60, y);
		printf("��");
	}

	gotoxy(39, 8);
	printf("��");
	for (i = 0; i < 20; i++)
		printf("��");
	printf("��");
	gotoxy(42, 6);
	printf("1.  ž  �·� ����");
	//����
	gotoxy(28, 9);
	printf("��");
	for (i = 0; i < 20; i++)
		printf("��");
	printf("��");

	y = 9;
	for (i = 0; i < 3; i++)
	{
		y++;
		gotoxy(28, y);
		printf("��");
		gotoxy(49, y);
		printf("��");
	}

	gotoxy(28, 13);
	printf("��");
	for (i = 0; i < 20; i++)
		printf("��");
	printf("��");
	gotoxy(31, 11);
	printf("2. ���� �·� ����");
	//�̵�
	gotoxy(50, 9);
	printf("��");
	for (i = 0; i < 20; i++)
		printf("��");
	printf("��");

	y = 9;
	for (i = 0; i < 3; i++)
	{
		y++;
		gotoxy(50, y);
		printf("��");
		gotoxy(71, y);
		printf("��");
	}

	gotoxy(50, 13);
	printf("��");
	for (i = 0; i < 20; i++)
		printf("��");
	printf("��");
	gotoxy(53, 11);
	printf("3. �̵� �·� ����");
	//����
	gotoxy(28, 14);
	printf("��");
	for (i = 0; i < 20; i++)
		printf("��");
	printf("��");

	y = 14;
	for (i = 0; i < 3; i++)
	{
		y++;
		gotoxy(28, y);
		printf("��");
		gotoxy(49, y);
		printf("��");
	}

	gotoxy(28, 18);
	printf("��");
	for (i = 0; i < 20; i++)
		printf("��");
	printf("��");
	gotoxy(31, 16);
	printf("4. ���� �·� ����");
	//����
	gotoxy(50, 14);
	printf("��");
	for (i = 0; i < 20; i++)
		printf("��");
	printf("��");

	y = 14;
	for (i = 0; i < 3; i++)
	{
		y++;
		gotoxy(50, y);
		printf("��");
		gotoxy(71, y);
		printf("��");
	}

	gotoxy(50, 18);
	printf("��");
	for (i = 0; i < 20; i++)
		printf("��");
	printf("��");
	gotoxy(53, 16);
	printf("5. ���� �·� ����");
	//�ڷΰ���
	gotoxy(39, 26);
	printf("��");
	for (i = 0; i < 20; i++)
		printf("��");
	printf("��");

	y = 26;
	for (i = 0; i < 1; i++)
	{
		y++;
		gotoxy(39, y);
		printf("��");
		gotoxy(60, y);
		printf("��");
	}

	gotoxy(39, 28);
	printf("��");
	for (i = 0; i < 20; i++)
		printf("��");
	printf("��");
	gotoxy(45, 27);
	printf("0. �ڷ� ����");

	key = _getch();
	if (key == '1') {
		play_node = find_node(LOL->root, "Top");
		print_line_rate(play_node);
	}
	else if (key == '2') {
		play_node = find_node(LOL->root, "Jungle");
		print_line_rate(play_node);
	}
	else if (key == '3') {
		play_node = find_node(LOL->root, "Mid");
		print_line_rate(play_node);
	}
	else if (key == '4') {
		play_node = find_node(LOL->root, "AD");
		print_line_rate(play_node);
	}
	else if (key == '5') {
		play_node = find_node(LOL->root, "Support");
		print_line_rate(play_node);
	}
	else if (key == '0') {
		win_rate(LOL);
	}
	else {
		line_win_rate(LOL);
	}
}

// 1-2) ���κ� �·� ���
void print_line_rate(Node * pnode)
{
	system("cls");
	picture();
	Node * save_champ[10];
	Node * play_node;
	Node * rnode;
	Node * mnode = (Node*)malloc(sizeof(Node));
	mnode->win_rate = 0;
	int num = pnode->child_num - 2;
	for (int i = 0; i < 10; i++) {
		play_node = pnode->cnode;
		rnode = mnode;
		for (int j = 0; j < num; j++) {
			if (i != 0)
				if (save_champ[i - 1]->win_rate <= play_node->win_rate) {
					play_node = play_node->snode;
					continue;
				}
			if (play_node->win_rate > rnode->win_rate) {
				rnode = play_node;
			}
			play_node = play_node->snode;
		}
		save_champ[i] = rnode;
	}
	//---------------------------------------
	int i = 0;
	int x, y = 0;

	gotoxy(31, 4);
	printf("��");
	for (i = 0; i < 37; i++)
		printf("��");
	printf("��");

	y = 4;
	for (i = 0; i < 15; i++)
	{
		y++;
		gotoxy(31, y);
		printf("��");
		gotoxy(69, y);
		printf("��");
	}

	gotoxy(31, 19);
	printf("��");
	for (i = 0; i < 37; i++)
		printf("��");
	printf("��");

	gotoxy(31, 6);
	printf("��");
	for (i = 0; i < 37; i++)
		printf("��");
	printf("��");

	gotoxy(42, 6);
	printf("��");
	gotoxy(58, 6);
	printf("��");

	gotoxy(31, 8);
	printf("��");
	for (i = 0; i < 37; i++)
		printf("��");
	printf("��");

	y = 6;
	for (i = 0; i < 12; i++)
	{
		y++;
		gotoxy(42, y);
		printf("��");
		gotoxy(58, y);
		printf("��");
	}
	gotoxy(42, 8);
	printf("��");
	gotoxy(58, 8);
	printf("��");

	gotoxy(35, 5);
	printf("%s è�Ǿ� �·� ����(1~10��)", pnode->name);
	gotoxy(36, 7);
	printf("����");
	gotoxy(48, 7);
	printf("è�Ǿ�");
	gotoxy(62, 7);
	printf("�·�");
	y = 8;

	for (int i = 0; i < 10; i++) {
		y++;
		gotoxy(37, y);
		printf("%d.", i + 1);
		gotoxy(45, y);
		printf("%s", save_champ[i]->name);
		gotoxy(61, y);
		printf("%.2lf%%", save_champ[i]->win_rate);
	}
	gotoxy(42, 19);
	printf("��");
	gotoxy(58, 19);
	printf("��");
	gotoxy(36, 20);
	if (_kbhit() != 0) system("pause");
	_getch();
}

// 2) �ȷ� ���
void pick_rate()
{
	system("cls");
	picture();
	char champ_name[10][MAX_NAME] = { "�����","ī�̻�","����","����","������","����","����","��ƿ����","�ڸ���","�߽���" };
	double champ_pick_rate[10] = { 48.60,32.08,24.77,22.46,20.48,18.46,18.04,17.19,16.39,16.36 };
	//------------------------------------------------------------
	int i = 0;
	int x, y = 0;

	gotoxy(31, 4);
	printf("��");
	for (i = 0; i < 37; i++)
		printf("��");
	printf("��");

	y = 4;
	for (i = 0; i < 15; i++)
	{
		y++;
		gotoxy(31, y);
		printf("��");
		gotoxy(69, y);
		printf("��");
	}

	gotoxy(31, 19);
	printf("��");
	for (i = 0; i < 37; i++)
		printf("��");
	printf("��");

	gotoxy(31, 6);
	printf("��");
	for (i = 0; i < 37; i++)
		printf("��");
	printf("��");

	gotoxy(42, 6);
	printf("��");
	gotoxy(58, 6);
	printf("��");

	gotoxy(31, 8);
	printf("��");
	for (i = 0; i < 37; i++)
		printf("��");
	printf("��");

	y = 6;
	for (i = 0; i < 12; i++)
	{
		y++;
		gotoxy(42, y);
		printf("��");
		gotoxy(58, y);
		printf("��");
	}
	gotoxy(39, 5);
	printf("è�Ǿ� �ȷ� ����(1~10��)");
	gotoxy(36, 7);
	printf("����");
	gotoxy(48, 7);
	printf("è�Ǿ�");
	gotoxy(62, 7);
	printf("�ȷ�");
	y = 8;

	for (int i = 0; i < 10; i++) {
		y++;
		gotoxy(37, y);
		printf("%d.", i + 1);
		gotoxy(45, y);
		printf("%s", champ_name[i]);
		gotoxy(61, y);
		printf("%.2lf%%", champ_pick_rate[i]);
	}
	gotoxy(42, 19);
	printf("��");
	gotoxy(58, 19);
	printf("��");
	gotoxy(36, 20);
	if (_kbhit() != 0) system("pause");
	_getch();
}

// 3) ��� ���
void ban_rate()
{
	system("cls");
	picture();
	char champ_name[10][MAX_NAME] = { "�߽���","�𸣰���","����","����","�����","����","�ٸ��콺","��ƿ����","����ũ","��Ʈ�Ͻ�" };
	double champ_ban_rate[10] = { 60.73,52.37,50.68,43.64,38.07,36.02,33.67,33.22,29.54,26.63 };
	//------------------------------------------------

	int i = 0;
	int x, y = 0;

	gotoxy(31, 4);
	printf("��");
	for (i = 0; i < 37; i++)
		printf("��");
	printf("��");

	y = 4;
	for (i = 0; i < 15; i++)
	{
		y++;
		gotoxy(31, y);
		printf("��");
		gotoxy(69, y);
		printf("��");
	}

	gotoxy(31, 19);
	printf("��");
	for (i = 0; i < 37; i++)
		printf("��");
	printf("��");

	gotoxy(31, 6);
	printf("��");
	for (i = 0; i < 37; i++)
		printf("��");
	printf("��");

	gotoxy(42, 6);
	printf("��");
	gotoxy(58, 6);
	printf("��");

	gotoxy(31, 8);
	printf("��");
	for (i = 0; i < 37; i++)
		printf("��");
	printf("��");

	y = 6;
	for (i = 0; i < 12; i++)
	{
		y++;
		gotoxy(42, y);
		printf("��");
		gotoxy(58, y);
		printf("��");
	}
	gotoxy(42, 8);
	printf("��");
	gotoxy(58, 8);
	printf("��");

	gotoxy(39, 5);
	printf("è�Ǿ� ��� ����(1~10��)");
	gotoxy(36, 7);
	printf("����");
	gotoxy(48, 7);
	printf("è�Ǿ�");
	gotoxy(62, 7);
	printf("���");
	y = 8;

	for (int i = 0; i < 10; i++) {
		y++;
		gotoxy(37, y);
		printf("%d.", i + 1);
		gotoxy(45, y);
		printf("%s", champ_name[i]);
		gotoxy(61, y);
		printf("%.2lf%%", champ_ban_rate[i]);
	}
	gotoxy(42, 19);
	printf("��");
	gotoxy(58, 19);
	printf("��");
	gotoxy(36, 20);
	if (_kbhit() != 0) system("pause");
	_getch();
}

// end - 4. ��Ÿ�м�

// --------------------------------------------------------------------------

// 5. 1��1 ���� ���� ( �迭�� �ؽ�Ʈ ���� ���)

// ��� è�Ǿ� �迭 ����
void make_champion_array(char arr[][20]) {
	FILE * fp;
	int i;
	fp = fopen("All.txt", "r");
	if (fp != NULL) {
		char *pStr;
		for (i = 0; i < 144; i++) {
			pStr = fgets(arr[i], 20, fp);
		}
		fclose(fp);
	}
	else {
		printf("ERROR @@ �ؽ�Ʈ ������ �������� �ʽ��ϴ�\n");
		return 0;
	}

}

//�������� ����
//�������� ����
//�������� ����
void Play_Game(Tree* LOL) {
	{
		// �׽�Ʈ������ �Է��� ���� ����
		char buf[100] = { 0, };
		int i = 0;
		int key;
		int random1=0;
		int random2=0;
		int jud1 = -1;
		int jud2 = -1;

		int x = 0, y = 0;
		int z;

		Node* champ_node1;
		Node* champ_node2;

		

		Rendering(10, 10, 245);
		
		//��� è�Ǿ��� ��� �迭
		char allChampion[144][20] = { 0, };
		make_champion_array(allChampion);

		hWnd = GetConsoleWindow();
		hInst = GetModuleHandle(NULL);
		while (1) 
		{
			system("cls");
			gotoxy(0, 0);
			printf("��");
			for (i = 0; i < 99; i++)
				printf("��");
			printf("��");
			y = 0;
			for (i = 0; i < 10; i++)
			{
				y++;
				gotoxy(0, y);
				printf("��");
				gotoxy(100, y);
				printf("��");
			}
			gotoxy(0, 10);
			printf("��");
			for (i = 0; i < 99; i++)
				printf("��");
			printf("��");
			//1
			textcolor(LIGHTGRAY, LIGHTGRAY);
			y = 3;
			for (z = 0; z < 5; z++)
			{
				gotoxy(34, y);
				printf("��");
				gotoxy(66, y);
				printf("��");
				y++;
			}
			//v
			y = 3;
			for (z = 0; z < 2; z++)
			{	gotoxy(40, y);
				printf("��");
				gotoxy(48, y);
				printf("��");
				y++;
			}
			y = 5;
			for (z = 0; z < 2; z++)
			{
				gotoxy(42, y);
				printf("��");
				gotoxy(46, y);
				printf("��");
				y++;
			}
			gotoxy(44, 7);
			printf("��");
			//s
			gotoxy(52, 3);
			printf("������");
			gotoxy(52, 4);
			printf("��");
			gotoxy(52, 5);
			printf("������");
			gotoxy(60, 6);
			printf("��");
			gotoxy(52, 7);
			printf("������");
			textcolor(LIGHTGRAY, BLACK);

			//1P
			
			
			/*gotoxy(30, 10);
			printf("��");
			for (i = 0; i < 14; i++)
				printf("��");
			printf("��");
			y = 10;
			for (i = 0; i < 10; i++)
			{
				y++;
				gotoxy(30, y);
				printf("��");
				gotoxy(45, y);
				printf("��");
			}
			gotoxy(30,21);
			printf("��");
			for (i = 0; i < 14; i++)
				printf("��");
			printf("��");*/
			if (random1 != 0)
			{
				Rendering(200, 200, random1);
			}//2P
			if (random2 != 0) 
			{
				Rendering(600, 200, random2);
			}

			// ����������.
			gotoxy(35, 21);
			printf("��");
			for (i = 0; i < 29; i++)
				printf("��");
			printf("��");
			y = 21;
			for (i = 0; i < 5; i++)
			{
				y++;
				gotoxy(35, y);
				printf("��");
				gotoxy(65, y);
				printf("��");
			}
			gotoxy(35, 27);
			printf("��");
			for (i = 0; i < 29; i++)
				printf("��");
			printf("��");
			gotoxy(38, 22);
			printf("S : Player 1�� è�Ǿ� �̱�");
			gotoxy(38, 23);
			printf("D : Player 2�� è�Ǿ� �̱�");
			gotoxy(38, 24);
			printf("G : 1��1 ��� ����!");
			gotoxy(38, 26);
			printf("X:�ʱ� ȭ������ ����");
			key = _getch();
			if (key == 's') 
			{
				system("cls");
				
				gotoxy(40, 8);
				printf("��");
				for (i = 0; i < 32; i++)
					printf("��");
				printf("��");
				gotoxy(40, 9);
				printf("��");
				gotoxy(73, 9);
				printf("��");
				gotoxy(40, 10);
				printf("��");
				for (i = 0; i < 32; i++)
					printf("��");
				printf("��");
				gotoxy(42, 9);
				printf("è�Ǿ��� �������� �̴����Դϴ�.");
				//printf("\t\t\t�ƹ�Ű�� ������ �����ϴ�.\n");
				delay(3000);
				system("cls");
				random1 = (rand() % 144 + 101);
				champ_node1 = find_node(LOL->root, allChampion[random1 - 101]);
				jud1++;//player1�� �̱� ����

				Rendering(440, 200, random1);
				Sleep(500);
				gotoxy(45,24);
				printf("Player 1�� è�Ǿ� : %s", champ_node1->name);
				gotoxy(45, 25);
				printf("�·� = %.2lf%%\n",champ_node1->win_rate);
				
				Sleep(5000);
				while (1) {
					
					if (_kbhit()) {
						break;
					}
				}

			}
			else if (key == 'd') {
				system("cls");
				
				gotoxy(40, 8);
				printf("��");
				for (i = 0; i < 32; i++)
					printf("��");
				printf("��");
				gotoxy(40, 9);
				printf("��");
				gotoxy(73, 9);
				printf("��");
				gotoxy(40, 10);
				printf("��");
				for (i = 0; i < 32; i++)
					printf("��");
				printf("��");
				gotoxy(42, 9);
				printf("è�Ǿ��� �������� �̴����Դϴ�.");
				//printf("\t\t\t�ƹ�Ű�� ������ �����ϴ�.\n");
				delay(3000);
				system("cls");

				random2 = (rand() % 144 + 101);
				champ_node2 = find_node(LOL->root, allChampion[random2 - 101]);
				jud2++;//player1�� �̱� ����
				Rendering(440, 200, random2);
				Sleep(500);
				gotoxy(45, 24);
				printf("Player 2�� è�Ǿ� : %s", champ_node2->name);
				gotoxy(45, 25);
				printf("�·� = %.2lf%%\n", champ_node2->win_rate);
				Sleep(5000);
				while (1) {
					
					if (_kbhit())
						break;
				}

			}
			else if (key == 'g') 
			{
				system("cls");
				gotoxy(39, 8);
				printf("��");
				for (i = 0; i < 36; i++)
					printf("��");
				printf("��");
				gotoxy(39, 9);
				printf("��");
				gotoxy(76, 9);
				printf("��");
				gotoxy(39, 10);
				printf("��");
				for (i = 0; i < 36; i++)
					printf("��");
				printf("��");
				gotoxy(41, 9);

				if (jud1 == -1 && jud2 != -1)
					printf("Player 1�� è�Ǿ��� �̾��ּ���!!\n");
				else if (jud2 == -1 && jud1 != -1)
					printf("Player 2�� è�Ǿ��� �̾��ּ���!!\n");
				else if (jud1 == -1 && jud2 == -1)
					printf("Player 1,2 �� è�Ǿ��� �̾��ּ���!!\n");
				else if (jud1 != -1 && jud2 != -1) 
				{
					system("cls");
					Sleep(2000);
					Rendering(300, 200, random1); // Player 1 ����
					Sleep(1000);
					gotoxy(35, 22);
					printf("%s", champ_node1->name);
					gotoxy(35, 23);
					printf("%.2lf%%", champ_node1->win_rate); //player 1 �·�
					Sleep(1000);
					gotoxy(55, 22);
					printf("VS");
					Sleep(1000);
					Rendering(600, 200, random2); // Player 2 ����
					Sleep(1000);
					gotoxy(70, 22);
					printf("%s", champ_node2->name);	
					gotoxy(70, 23);
					printf("%.2lf%%", champ_node2->win_rate); //player 2 �·�
					delay(1000);

					if (champ_node1->win_rate > champ_node2->win_rate) {
						system("cls");
						Sleep(2000);
						Rendering(450, 200, random1);
						Sleep(100);
						gotoxy(45, 21);
						printf("Player 1�� è�Ǿ� %s", champ_node1->name);
						gotoxy(49, 22);
						printf("�¸�");
						
			
						Sleep(5000);
					}
					else if (champ_node1->win_rate == champ_node2->win_rate) {
						system("cls");
						Sleep(2000);
						gotoxy(47,15);
						printf("���º�!\n");
						Sleep(5000);
					}
					else {
						system("cls");
						Sleep(2000);
						Rendering(450, 200, random2);
						Sleep(100);
						gotoxy(45, 21);
						printf("Player 2�� è�Ǿ� %s", champ_node2->name);
						gotoxy(49, 22);
						printf("�¸�");
						Sleep(5000);
					}
				
				}
				Sleep(2000);
			}
			else if (key == 'x') {
				system("cls");
				gotoxy(39, 8);
				printf("��");
				for (i = 0; i < 36; i++)
					printf("��");
				printf("��");
				gotoxy(39, 9);
				printf("��");
				gotoxy(76, 9);
				printf("��");
				gotoxy(39, 10);
				printf("��");
				for (i = 0; i < 36; i++)
					printf("��");
				printf("��");
				gotoxy(40, 9);
				printf("�ʱ� ȭ������ ���ư��ϴ�.\n");
				delay(1500);
				break;
			}
			else {
				system("cls");
			}
			
		}
		return;
	}
}

// end - 5. 1��1 ������ �ο�

// ----------------------------------------------------------------------

// UI
void gotoxy(int x, int y)
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void deletecursor()
{
	CONSOLE_CURSOR_INFO CurInfo;
	CurInfo.dwSize = 1;
	CurInfo.bVisible = FALSE;

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}
void picture()
{
	int i = 0;
	int x, y = 0;

	printf("��");
	for (i = 0; i < 99; i++)
		printf("��");
	printf("��");

	for (i = 0; i < 32; i++)
	{
		y++;
		gotoxy(0, y);
		printf("��");

	}

	gotoxy(0, 32);
	for (i = 0; i <= 99; i++)
		printf("��");

	x = 100;
	y = 0;
	for (i = 0; i < 32; i++)
	{
		y++;
		gotoxy(x, y);
		printf("��");
	}
	y++;

	gotoxy(0, 32);
	printf("��");
	gotoxy(100, 32);
	printf("��");
}

// �÷�
void textcolor(int foreground, int background)
{
	int color = foreground + background * 16;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

//BGM
void StopSound()
{
	PlaySound(NULL, 0, 0);
}
void LoginSound()
{
	PlaySound(TEXT("Login.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	//���� ���� ��ġ, SND_ASYNC, SND_LOOP �������� ���� �߿�
	//SND_ASYNC : ����ϸ鼭 �����ڵ� ����
	//SND_LOOP : �ݺ����

}
void MainSound()
{
	PlaySound(TEXT("KDAPOPSTARS.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

// ---------------------------------------------------------------------
int main()
{
	srand(time(NULL));
	Tree *my_tree;
	SettingTree(&my_tree);
	// �ܼ� ������ â�� �ڵ鰪�� ���´�.
	hWnd = GetConsoleWindow();
	// ���α׷��� �ν��Ͻ� �ڵ鰪�� ���´�.
	hInst = GetModuleHandle(NULL);
	Sleep(1000);
	Rendering(-30, -10, 249);
	LoginSound();
	Sleep(3600);
	StopSound();

	Rendering(0, 0, 247);
	Sleep(1000);
	MainSound();

	while (TRUE) {
		main_disply(&my_tree);
	}
}
