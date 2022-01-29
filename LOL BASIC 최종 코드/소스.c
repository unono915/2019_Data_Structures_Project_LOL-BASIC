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

//포지션별 배열의 인덱스
#define TOP         0
#define JUNGLE      1
#define MID         2
#define AD_CARRY    3
#define SUPPORTER   4

//포지션 개수
#define LineMAX     5

//총 챔피언 개수
#define Champ_num   144

//포지션별 챔피언 개수 - > 챔프 개수+1
#define TopMax      41
#define MidMax      33
#define JungleMax   31
#define AdMax       18
#define SupMax      26
#define AllMax      145

//퀴즈 개수
#define qMAX        42

// 챔피언 이름 최대 길이
#define MAX_NAME    20

// color상수 지정 
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

// 콘솔 윈도우 창의 핸들값
static HWND hWnd;
// 프로그램의 인스턴스 핸들값
static HINSTANCE hInst;

// 트리를 만들기 위해 사용할 연결리스트 자료형
typedef struct _node
{
	char name[MAX_NAME];           // 챔피언 이름 or 기준
	// 챔피언 정보
	int difficulty; // 난이도
	// 역할
	int assassin;
	int warrior;
	int wizard;

	int supporter;
	int tanker;
	int ad;
	//승률
	double win_rate;

	struct _node * pnode;    // 부모 노드
	struct _node * cnode;    // 자식 노드
	struct _node * snode;    // 형제 노드
	int child_num;           // 자식 노드 개수
} Node;

typedef struct _Tree
{
	Node * root;
} Tree;

//포지션 추천 질문 구조체
typedef struct _Qustion {
	char question[500];
	int answer[LineMAX];
}Question;

// 2.챔피언 추천 - 사용자 설정
typedef struct _play
{
	char name[Champ_num][MAX_NAME];
	int num_cham;

	int line;
	int difficulty; // 난이도
	// 역할
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
void delay(clock_t n);      //시간 지연
//이미지 출력 * nChamp는 챔피언 배열의 탐색을 위함
void Rendering(int x, int y, int nChamp);
void gotoxy(int x, int y);  //커서의 위치 변경

// ----------------------------------------------------------

// 트리 ADT
Node * createNode(char name[]);
void createTree(Tree * qtree, char name[]);
void createchild(Tree * qtree, char par_name[], char name[]);
Node * find_node(Node * node, char node_name[]);

// -------------------------------------------------------------

// LOL - 라인 - 챔피언 : 트리 생성
void SettingTree(Tree * qtree);
void SettingTop(Tree * qtree);
void SettingJungle(Tree * qtree);
void SettingMid(Tree * qtree);
void SettingAD(Tree * qtree);
void SettingSupport(Tree * qtree);
void SettingRate(Tree * qtree);

//-------------------------------------------------------------------

// 1.포지션 추천 ( 배열과 텍스트 파일 사용)

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

// 2. 챔피언 추천 모듈
void recommand(Tree * qtree);

Player * init_player();                // 사용자 구조체 초기화
void select_diff(Player * player);     // 난이도 선택
void select_line(Player * player);     // 라인 선택
void select_work(Player * player);     // 역할군 선택
void rec_display();                    // 챔피언 추천 디스플레이
void player_data(Player * player);     // 사용자 선택 정보 출력
void recommand_champ(Player * player); // 추천 챔피언 출력

// 추천 챔프를 사용자 챔프 배열에 저장
void save_cham(Tree * qtree, Player * player);

// -------------------------------------------------------------

// 3. 챔피언 스토리 및 공략
void Story_Strategy();

void Story_Strategy_display();      // 스토리 및 공략 디스플레이
void story_display();               // 2) 스토리 디스플레이
void print_story();                 // 2) 스토리 사이트 실행
void strategy_display();            // 3) 공략 디스플레이
void print_strategy();              // 3) 공략 사이트 실행

// -------------------------------------------------------------

// 4. 현재 메타 분석
void current_meta(Tree * LOL);

void meta_display();                 // 메타 분석 디스플레이
void win_rate(Tree * LOL);           // 1) 승률 선택지
void all_win_rate();                 // 1-1) 전체 승률 출력
void print_line_rate(Node * pnode);  // 1-2) 라인별 승률 순위
void line_win_rate(Tree * LOL);      // 1-2) 라인별 승률 출력
void pick_rate();                    // 2) 픽률 출력
void ban_rate();                     // 3) 밴률 출력

// -------------------------------------------------------------

// 5. 1대1 랜덤 게임 ( 배열과 텍스트 파일 사용)

// 모든 챔피언 배열 생성
void make_champion_array(char arr[][20]);
void Play_Game(Tree * LOL);

// ------------------------------------------------------------

//시간 지연
void delay(clock_t n)
{
	clock_t start = clock();
	while (clock() - start < n);
}

//이미지 출력 * nChamp는 챔피언 배열의 탐색을 위함
void Rendering(int x, int y, int nChamp)
{
	// DC의 핸들값을 받을 변수를 선언한다.(hDC : 실제화면DC, hMemDC : 이미지를 담을 DC)
	// Device Context는 그래픽에 필요한 모든 옵션을 넣어둔 구조체라고 볼 수 있다.
	// 그림그릴때 그림을 그릴 화폭이라 보면된다.
	HDC hDC, hMemDC;
	// 후면버퍼이다. static 변수는 정적변수로서, 메모리의 Data영역에 저장되는 함수가 끝나도 사라지지 않고 메모리에 남아있는 변수이다.
	static HDC hBackDC;
	// 이미지 비트맵의 핸들값을 저장할 변수이다.
	HBITMAP hBitmap, hOldBitmap, hBackBitmap;
	// 핸들값으로부터 얻어올 실제 비트맵 구조체.
	BITMAP Bitmap;
	// 현재 윈도우의 Rect값(크기)를 얻어온다. Rect는 왼쪽위, 오른쪽 아래의 값을 가지는 정사각형을 나타내는 구조체이다.
	RECT WindowRect;
	GetWindowRect(hWnd, &WindowRect);

	// 현재 윈도우의 DC 핸들값을 얻어온다. GetWindowDC(hWnd)도 가능하다.
	hDC = GetDC(hWnd);
	// hDC와 호환되는 DC를 메모리에 만들어 핸들값을 반환한다.
	hBackDC = CreateCompatibleDC(hDC);
	hMemDC = CreateCompatibleDC(hDC);

	// 비트맵 메모리를 할당하고 핸들을 반환한다.
	hBackBitmap = CreateCompatibleBitmap(hDC, WindowRect.right, WindowRect.bottom);
	// 그릴 도화지를 준비한다.
	hOldBitmap = (HBITMAP)SelectObject(hBackDC, hBackBitmap);
	// 비트맵을 로드하여 핸들을 반환한다. resource.h에 정수로 define되어있는 불러온 리소스를 로드한다.
	hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(nChamp));//IDE 1 == 101////////////////////////////////////
	// 크기를 받기위해 비트맵 구조체를 비트맵 핸들로부터 생성한다.
	GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
	// 읽어온 비트맵 리소스를 메모리 DC에 선택한다.
	SelectObject(hMemDC, hBitmap);

	// hMemDC의 이미지를 hBackDC의 원하는 위치로 고속복사시킨다.(출력하려는 이미지를 후면버퍼에 복사시킨다.)
	BitBlt(hBackDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, SRCCOPY);
	// hBackDC(후면 버퍼)의 완성된 그림을 화면으로 고속복사시킨다.
	BitBlt(hDC, x, y, x + Bitmap.bmWidth, y + Bitmap.bmHeight, hBackDC, 0, 0, SRCCOPY);///출력되는 이미지의 위치 조절!!!!

	// 메모리와 오브젝트를 해지한다.
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

//컬러
void textcolor(int foreground, int background);

// BGM
void StopSound();
void LoginSound();
void MainSound();


// -------------------------------------------------------------

// 트리 ADT
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

// LOL - 라인 - 챔피언 : 트리 생성
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

// 시작 선택지
void main_disply(Tree * qtree)
{
	system("cls");
	picture();
	int key;
	int i = 0;
	int x, y = 0;

	gotoxy(31, 2);
	printf("[롤]   입문자를  위한  기초  프로그램");
	gotoxy(16, 3);
	printf("□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□");
	gotoxy(16, 4);
	printf("□■□□□■■■□■□□□□■■□□□■□□□■■□■■■□□■■□");
	gotoxy(16, 5);
	printf("□■□□□■□■□■□□□□■□■□■□■□■□□□□■□□■□□□");
	gotoxy(16, 6);
	printf("□■□□□■□■□■□□□□■■□□■■■□□■□□□■□□■□□□");
	gotoxy(16, 7);
	printf("□■□□□■□■□■□□□□■□■□■□■□□□■□□■□□■□□□");
	gotoxy(16, 8);
	printf("□■■■□■■■□■■■□□■■□□■□■□■■□□■■■□□■■□");
	gotoxy(16, 9);
	printf("□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□");
	gotoxy(16, 10);
	printf("");
	gotoxy(16, 11);
	printf("");

	gotoxy(34, 13);
	printf("\t 1. 포지션 추천\n");
	gotoxy(34, 14);
	printf("\t 2. 챔피언 추천\n");
	gotoxy(34, 15);
	printf("\t 3. 챔피언 스토리 및 공략\n");
	gotoxy(34, 16);
	printf("\t 4. 현재 메타 분석\n");
	gotoxy(34, 17);
	printf("\t 5. 1대1 랜덤픽 싸움\n");
	gotoxy(34, 18);
	printf("\t -----------------------\n");
	gotoxy(34, 19);
	printf("\t 0. 프로그램 종료\n");

	gotoxy(82, 23);
	textcolor(LIGHTRED, BLACK);
	printf("▣▣▣▣▣▣▣▣");
	gotoxy(98, 23);
	textcolor(LIGHTMAGENTA, BLACK);
	printf("▣");

	gotoxy(82, 24);
	textcolor(LIGHTRED, BLACK);
	printf("▣");
	gotoxy(84, 24);
	textcolor(BROWN, BLACK);
	printf("▧");
	gotoxy(86, 24);
	textcolor(GREEN, BLACK);
	printf("□□□□□");
	gotoxy(96, 24);
	textcolor(YELLOW, BLACK);
	printf("▣");
	gotoxy(98, 24);
	textcolor(LIGHTCYAN, BLACK);
	printf("▣");

	gotoxy(82, 25);
	textcolor(LIGHTRED, BLACK);
	printf("▣");
	gotoxy(84, 25);
	textcolor(GREEN, BLACK);
	printf("□");
	gotoxy(86, 25);
	textcolor(BROWN, BLACK);
	printf("▧");
	gotoxy(88, 25);
	textcolor(GREEN, BLACK);
	printf("□□□");
	gotoxy(94, 25);
	textcolor(YELLOW, BLACK);
	printf("▣");
	gotoxy(96, 25);
	textcolor(GREEN, BLACK);
	printf("□");
	gotoxy(98, 25);
	textcolor(LIGHTCYAN, BLACK);
	printf("▣");

	gotoxy(82, 26);
	textcolor(LIGHTRED, BLACK);
	printf("▣");
	gotoxy(84, 26);
	textcolor(GREEN, BLACK);
	printf("□□");
	gotoxy(88, 26);
	textcolor(BROWN, BLACK);
	printf("▧");
	gotoxy(90, 26);
	textcolor(GREEN, BLACK);
	printf("□");
	gotoxy(92, 26);
	textcolor(YELLOW, BLACK);
	printf("▣");
	gotoxy(94, 26);
	textcolor(GREEN, BLACK);
	printf("□□");
	gotoxy(98, 26);
	textcolor(LIGHTCYAN, BLACK);
	printf("▣");

	gotoxy(82, 27);
	textcolor(LIGHTRED, BLACK);
	printf("▣");
	gotoxy(84, 27);
	textcolor(GREEN, BLACK);
	printf("□□□");
	gotoxy(90, 27);
	textcolor(YELLOW, BLACK);
	printf("▣");
	gotoxy(92, 27);
	textcolor(GREEN, BLACK);
	printf("□□□");
	gotoxy(98, 27);
	textcolor(LIGHTCYAN, BLACK);
	printf("▣");

	gotoxy(82, 28);
	textcolor(LIGHTRED, BLACK);
	printf("▣");
	gotoxy(84, 28);
	textcolor(GREEN, BLACK);
	printf("□□");
	gotoxy(88, 28);
	textcolor(YELLOW, BLACK);
	printf("▣");
	gotoxy(90, 28);
	textcolor(GREEN, BLACK);
	printf("□");
	gotoxy(92, 28);
	textcolor(BROWN, BLACK);
	printf("▧");
	gotoxy(94, 28);
	textcolor(GREEN, BLACK);
	printf("□□");
	gotoxy(98, 28);
	textcolor(LIGHTCYAN, BLACK);
	printf("▣");

	gotoxy(82, 29);
	textcolor(LIGHTRED, BLACK);
	printf("▣");
	gotoxy(84, 29);
	textcolor(GREEN, BLACK);
	printf("□");
	gotoxy(86, 29);
	textcolor(YELLOW, BLACK);
	printf("▣");
	gotoxy(88, 29);
	textcolor(GREEN, BLACK);
	printf("□□□");
	gotoxy(94, 29);
	textcolor(BROWN, BLACK);
	printf("▧");
	gotoxy(96, 29);
	textcolor(GREEN, BLACK);
	printf("□");
	gotoxy(98, 29);
	textcolor(LIGHTCYAN, BLACK);
	printf("▣");

	gotoxy(82, 30);
	textcolor(LIGHTRED, BLACK);
	printf("▣");
	gotoxy(84, 30);
	textcolor(YELLOW, BLACK);
	printf("▣");
	gotoxy(86, 30);
	textcolor(GREEN, BLACK);
	printf("□□□□□");
	gotoxy(96, 30);
	textcolor(BROWN, BLACK);
	printf("▧");
	gotoxy(98, 30);
	textcolor(LIGHTCYAN, BLACK);
	printf("▣");

	gotoxy(82, 31);
	textcolor(LIGHTBLUE, BLACK);
	printf("▣");
	gotoxy(84, 31);
	textcolor(LIGHTCYAN, BLACK);
	printf("▣▣▣▣▣▣▣▣");

	gotoxy(33, 32);
	textcolor(LIGHTGRAY, BLACK);
	printf("");

	//ㅂ ━ ┃ ┏ ┓ ┛ ┗
	//중앙UI
	gotoxy(34, 25);
	printf("┌");
	for (i = 0; i < 38; i++)
		printf("─");
	printf("┐");

	gotoxy(34, 31);
	printf("└");
	for (i = 0; i < 38; i++)
		printf("─");
	printf("┘");

	y = 25;
	for (i = 0; i < 5; i++)
	{
		y++;
		gotoxy(73, y);
		printf("│");

	}

	y = 25;
	for (i = 0; i < 5; i++)
	{
		y++;
		gotoxy(34, y);
		textcolor(LIGHTMAGENTA, LIGHTMAGENTA);
		printf("■");
	}
	textcolor(LIGHTGRAY, BLACK);
	//UI(챔프 얼굴칸)
	gotoxy(26, 26);
	printf("┌");
	for (i = 0; i < 6; i++)
		printf("─");

	y = 26;
	for (i = 0; i < 3; i++)
	{
		y++;
		gotoxy(26, y);
		printf("│");

	}
	gotoxy(26, 30);
	printf("┗");
	for (i = 0; i < 6; i++)
		printf("─");
	// P
	gotoxy(36, 26);
	printf("┏━┓");
	gotoxy(36, 27);
	printf("┗━┛");
	//Q
	gotoxy(40, 26);
	printf("┏━━━┓");
	gotoxy(40, 27);
	printf("┃ Ｑ┃");
	gotoxy(40, 28);
	printf("┗━━━┛");
	//W
	gotoxy(46, 26);
	printf("┏━━━┓");
	gotoxy(46, 27);
	printf("┃ Ｗ┃");
	gotoxy(46, 28);
	printf("┗━━━┛");
	//E
	gotoxy(52, 26);
	printf("┏━━━┓");
	gotoxy(52, 27);
	printf("┃ Ｅ┃");
	gotoxy(52, 28);
	printf("┗━━━┛");
	//R
	gotoxy(58, 26);
	printf("┏━━━┓");
	gotoxy(58, 27);
	printf("┃ Ｒ┃");
	gotoxy(58, 28);
	printf("┗━━━┛");
	//D
	gotoxy(64, 26);
	printf("┏━┓");
	gotoxy(64, 27);
	printf("┗━┛");
	//F
	gotoxy(68, 26);
	printf("┏━┓");
	gotoxy(68, 27);
	printf("┗━┛");
	//체력
	gotoxy(37, 29);
	textcolor(GREEN, GREEN);
	printf(" ▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣ ");
	gotoxy(37, 30);
	textcolor(BLUE, BLUE);
	printf(" ▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣▣ ");

	//마나
	printf("");

	// 마무리
	textcolor(LIGHTGRAY, BLACK);
	gotoxy(3, 33);
	printf("");
	gotoxy(3, 34);
	printf("");

	key = _getch();
	if (key == '1') {
		system("cls");
		//1.라인 추천을 위한 함수
		PLAY_Rec_Line();
	}
	else if (key == '2') {
		//2. 챔피언 추천을 위한 함수
		recommand(qtree);
	}
	else if (key == '3') {
		//3. 챔피언 스토리 및 공략을 위한 함수
		Story_Strategy();
	}
	else if (key == '4') {
		// 메타 분석을 위한 함수
		current_meta(qtree);
	}
	else if (key == '5') {
		system("cls");
		//5. 랜덤게임을 위한 함수
		Play_Game(qtree);//랜덤게임 시작
	}
	else if (key == '0') {
		system("cls");
		printf("프로그램을 종료합니다!\n");
		exit('\0');
	}
	else {
	}
}

//-------------------------------------------------------------------

// 1.포지션 추천 ( 배열과 텍스트 파일 사용)
void add_question(char Q_list1[][200], char Q_list2[][200], char Q_list3[][200], char Q_list4[][200], char Q_list5[][200], char Q_list6[][200], char Q_list7[][200], char Q_list8[][200], char Q_list9[][200]) {//질문 추가

	strcpy((&Q_list1[0]), "나는 상대방이 불편해 보이면 비위를 맞추려고 한다.");
	strcpy((&Q_list1[1]), "나는 일이 잘못되었을 때 자주 상대방의 탓으로 돌린다.");
	strcpy((&Q_list1[2]), "나는 무슨 일이던지 조목조목 따지는 편이다.");
	strcpy((&Q_list1[3]), "난 생각이 자주 바뀌고 동시에 여러가지 행동을 하는 편이다.");
	strcpy((&Q_list1[4]), "나는 타인의 평가에 구애받지 않고 내 의견을 말한다.");
	strcpy((&Q_list2[0]), "나는 관계나 일이 잘못되었을 때 자주 내 탓으로 돌린다.");
	strcpy((&Q_list2[1]), "나는 다른 사람들의 의견을 무시하고 내 의견을 주장하는 편이다.");
	strcpy((&Q_list2[2]), "나는 이성적이고 차분하며 냉정하다.");
	strcpy((&Q_list2[3]), "나는 다른 사람들로부터 정신이 없거나 산만하다는 소리를 듣는다.");
	strcpy((&Q_list2[4]), "나는 부정적인 감정도 솔직히 표현한다.");
	strcpy((&Q_list3[0]), "나는 지나치게 남을 의식해서 나의 생각이나 감정을 표현하는걸 두려워한다.");
	strcpy((&Q_list3[1]), "나는 내 의견이 받아들여지지 않았을때 화를 낸다.");
	strcpy((&Q_list3[2]), "나는 나의 견해를 분명히 표현하기 위해 객관적인 자료를 자주 인용한다.");
	strcpy((&Q_list3[3]), "나는 상황에 적절하지 못한 말이나 행동을 자주 하고 딴전을 피우는 편이다.");
	strcpy((&Q_list3[4]), "나는 다른 사람이 내게 부탁을 할 때 내가 원치 않으면 거절한다.");
	strcpy((&Q_list4[0]), "나는 사람들의 얼굴 표정,감정,말투에 신경을 많이 쓴다.");
	strcpy((&Q_list4[1]), "나는 타인의 결점이나 잘못을 잘 찾아내서 비판한다.");
	strcpy((&Q_list4[2]), "나는 실수하지 않으려고 애를 쓰는 편이다.");
	strcpy((&Q_list4[3]), "나는 곤란하거나 난처할 때는 농담이나 유머로 그 상황을 바꾸려 한다.");
	strcpy((&Q_list4[4]), "나는 나 자신에 대해 편한하게 느낀다.");
	strcpy((&Q_list5[0]), "나는 타인을 배려하고 잘 돌보아 주는 편이다.");
	strcpy((&Q_list5[1]), "나는 명력적이고 지시적인 말투를 자주 사용하기 때문에 상대가 공격받았다는 느낌이 들 때가 있다.");
	strcpy((&Q_list5[2]), "나는 불편한 상황을 그대로 넘기지 못하고 시시비비를 따지는 편이다.");
	strcpy((&Q_list5[3]), "나는 불편한 상황에서 안절부절못하거나 가만히 있지 못한다.");
	strcpy((&Q_list5[4]), "나는 모험을 두려워 하지 않는다.");
	strcpy((&Q_list6[0]), "나는 다른 사람들이 나를 싫어할까 두려워서 위축되거나 불안을 느낄 때가 많다.");
	strcpy((&Q_list6[1]), "나는 사소한 일에도 잘 흥분하거나 화를 낸다.");
	strcpy((&Q_list6[2]), "나는 현명하고 침착하지만 냉정하단 말을 자주 듣는다.");
	strcpy((&Q_list6[3]), "나는 한 주제에 집중하기 보단 화제를 자주 바꾼다.");
	strcpy((&Q_list6[4]), "나는 다양한 경험에 개방적이다.");
	strcpy((&Q_list7[0]), "나는 타인의 요청을 거절하지 못하는 편이다.");
	strcpy((&Q_list7[1]), "나는 자주 근육이 긴장되고 못이 뻣뻣하며 혈압이 오르는 것을 느끼곤 한다.");
	strcpy((&Q_list7[2]), "나는 나의 감정을 표현하기 힘들고 혼자라고 느낄때가 많다.");
	strcpy((&Q_list7[3]), "나는 분위기가 침체되거나 지루해지면 분위기를 바꾸려고 한다.");
	strcpy((&Q_list7[4]), "나는 나만의 독특한 개성을 존중한다.");
	strcpy((&Q_list8[0]), "나는 나 자신이 가치가 없는 것 같아 우울하게 느껴질 때가 많다.");
	strcpy((&Q_list8[1]), "나는 타인으로부터 비판적이거나 융통성 없다는 말을 듣기도 한다.");
	strcpy((&Q_list8[2]), "나는 목소리가 단조롭고 무표정하며 경직된 자세를 취하는 편이다.");
	strcpy((&Q_list8[3]), "나는 불안하면 호홉이 고르지 못하고 머리가 어지러운 경험을 하기도 한다.");
	strcpy((&Q_list8[4]), "나는 누가 내 의견에 반대해도 감정이 상하지 않는다.");
	strcpy((&Q_list9[0]), "나는 솔로가 편하다! 커플 저리가!!");
	strcpy((&Q_list9[1]), "나는 관종이다!");


}
void add_answer(Question Q_list[]) {//대답 추가 //가능성 감소시키는거 추가할지말지 고민!
	int i;
	for (i = 0; i < qMAX; i++) { //Q.answer값을 모두 -1로 초기화
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
void add_line(Question Q, int Line_rec[]) {//질문의 답에 따른 결과를 조작

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
void play_question(Question Q_list[], int Line_rec[], char Q_list1[][200], char Q_list2[][200], char Q_list3[][200], char Q_list4[][200], char Q_list5[][200], char Q_list6[][200], char Q_list7[][200], char Q_list8[][200], char Q_list9[][200]) {//질문 시작
	system("cls");
	picture();

	int x, y = 0;
	int i, j;
	int z;
	gotoxy(0, 3);
	printf("┣");
	for (z = 0; z < 99; z++)
		printf("━");
	printf("┫");
	gotoxy(0, 4);
	printf("┃");

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
		printf("┃");
		gotoxy(0, 5);
		printf("┣");
		for (z = 0; z < 71; z++)
			printf("━");
		printf("┳");
		for (z = 0; z < 27; z++)
			printf("━");
		printf("┫");

		gotoxy(0, 7);
		printf("┃");
		gotoxy(72, 6);
		printf("┃");
		gotoxy(72, 7);
		printf("┃");
		gotoxy(72, 8);
		printf("┃");
		gotoxy(72, 9);
		printf("┃");
		gotoxy(72, 10);
		printf("┃");
		gotoxy(75, 7);
		printf("P : 이전 질문");
		gotoxy(75, 8);
		printf("N : 다음 질문");
		gotoxy(75, 9);
		printf("X : 여기까지만 대답할래요");

		gotoxy(33, 7);
		printf("1. YES");
		gotoxy(33, 9);
		printf("2. NO");
		gotoxy(0, 11);
		printf("┣");
		for (z = 0; z < 71; z++)
			printf("━");
		printf("┻");
		for (z = 0; z < 27; z++)
			printf("━");
		printf("┫");
		gotoxy(33, 12);
		printf(">>");
		key = _getch();
		gotoxy(0, 13);
		printf("┣");
		for (z = 0; z < 99; z++)
			printf("━");
		printf("┫");
		if (key == 0) {
			printf("라인추천을 종료합니다.");
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
				printf("첫 번째 질문입니다.\n");
				delay(1000);
				i--;
			}
			else {
				i -= 2;
			}
		}//이전 혹은 다음 질문으로 넘어가는 기능
		else if (key == 'n') {
			if (i == 41) {
				printf("마지막 질문입니다.\n");
				i--;
				delay(1000);
			}
		}
		else if (key == 'x') {
			if (i == 0) {
				printf("아직 자료가 부족합니다.\n");
				delay(1000);
				i--;
			}
			else {
				i = 42;//for문을 종료시키기 위함
			}
		}
		else {

			i--;

		}

		system("cls");
	}

}
void display_top_trait() {
	//탑
	system("cls");
	picture();
	gotoxy(44, 2);
	printf("포지션 별 성향  탑");
	gotoxy(26, 4);
	printf("□□□□□□□□□□□□□□□□□□□□□□□□□");
	gotoxy(26, 5);
	printf("□□■■■■■□□□■■■■■□□□■■■■■□□");
	gotoxy(26, 6);
	printf("□□□□■□□□□□■□□□■□□□■□□□■□□");
	gotoxy(26, 7);
	printf("□□□□■□□□□□■□□□■□□□■■■■■□□");
	gotoxy(26, 8);
	printf("□□□□■□□□□□■□□□■□□□■□□□□□□");
	gotoxy(26, 9);
	printf("□□□□■□□□□□■■■■■□□□■□□□□□□");
	gotoxy(26, 10);
	printf("□□□□□□□□□□□□□□□□□□□□□□□□□");

	gotoxy(25, 12);
	printf("┏");
	gotoxy(75, 12);
	printf("┓");
	gotoxy(27, 13);
	printf("힘든 상황 속에서도 꾸준하게 버텨서 게임을 이끈다");
	gotoxy(27, 15);
	printf("상황판단 능력이 뛰어나서 이니시 각을 잘 본다");
	gotoxy(27, 17);
	printf("혼자가 편하다 / 외롭지 않아서 솔로로 지내는거다");
	gotoxy(27, 19);
	printf("텔레포트를 통한 운영으로 게임의 승리에 기여한다");
	gotoxy(25, 20);
	printf("┗");
	gotoxy(75, 20);
	printf("┛");
	printf("\n");
}
void display_mid_trait() {
	//미드
	system("cls");
	picture();
	gotoxy(44, 2);
	printf("포지션 별 성향  미드");
	gotoxy(26, 4);
	printf("□□□□□□□□□□□□□□□□□□□□□□□□□");
	gotoxy(26, 5);
	printf("□□■□□□■□□□■■■■■□□□■■■■□□□");
	gotoxy(26, 6);
	printf("□□■■□■■□□□□□■□□□□□■□□□■□□");
	gotoxy(26, 7);
	printf("□□■□■□■□□□□□■□□□□□■□□□■□□");
	gotoxy(26, 8);
	printf("□□■□□□■□□□□□■□□□□□■□□□■□□");
	gotoxy(26, 9);
	printf("□□■□□□■□□□■■■■■□□□■■■■□□□");
	gotoxy(26, 10);
	printf("□□□□□□□□□□□□□□□□□□□□□□□□□");

	gotoxy(25, 12);
	printf("┏");
	gotoxy(75, 12);
	printf("┓");
	gotoxy(27, 13);
	printf("에이스 기질이 있다 ");
	gotoxy(27, 15);
	printf("개인 피지컬을 통해 게임을 지배한다");
	gotoxy(27, 17);
	printf("주목받는것을 좋아한다 / 살짝 관종 끼가 있다");
	gotoxy(27, 19);
	printf("빠른 맵리딩을 통해 아군들을 지원하러 간다");
	gotoxy(27, 21);
	printf("메인 딜러로써 책임감이 막중하다");
	gotoxy(25, 22);
	printf("┗");
	gotoxy(75, 22);
	printf("┛");
	printf("\n");
}
void display_jungle_trait() {
	//정글
	system("cls");
	picture();
	gotoxy(44, 2);
	printf("포지션 별 성향  정글");
	gotoxy(26, 4);
	printf("□□□□□□□□□□□□□□□□□□□□□□□□□");
	gotoxy(26, 5);
	printf("□□■■■■■□□□■■■■■□□□■□□□□□□");
	gotoxy(26, 6);
	printf("□□□□■□□□□□■□□□□□□□■□□□□□□");
	gotoxy(26, 7);
	printf("□□■□■□□□□□■□□■■□□□■□□□□□□");
	gotoxy(26, 8);
	printf("□□■□■□□□□□■□□□■□□□■□□□□□□");
	gotoxy(26, 9);
	printf("□□■■■□□□□□■■■■■□□□■■■■■□□");
	gotoxy(26, 10);
	printf("□□□□□□□□□□□□□□□□□□□□□□□□□");

	gotoxy(25, 12);
	printf("┏");
	gotoxy(75, 12);
	printf("┓");
	gotoxy(27, 13);
	printf("김병만 그 자체다");
	gotoxy(27, 15);
	printf("상대 정글과의 심리전을 즐긴다");
	gotoxy(27, 17);
	printf("탐험을 좋아한다 / 와드로 맵을 밝혀나간다 ");
	gotoxy(27, 19);
	printf("명석한 전자두뇌로 승리를 위해 이 게임을 설계한다");
	gotoxy(25, 20);
	printf("┗");
	gotoxy(75, 20);
	printf("┛");
	printf("\n");
}
void display_ad_carry_trait() {
	//원거리 딜러
	system("cls");
	picture();
	gotoxy(44, 2);
	printf("포지션 별 성향  원거리 딜러");
	gotoxy(26, 4);
	printf("□□□□□□□□□□□□□□□□□□□□□□□□□");
	gotoxy(26, 5);
	printf("□□□□□□■■■□□□□□□■■■■□□□□□□");
	gotoxy(26, 6);
	printf("□□□□□■□□□■□□□□□■□□□■□□□□□");
	gotoxy(26, 7);
	printf("□□□□□■■■■■□□□□□■□□□■□□□□□");
	gotoxy(26, 8);
	printf("□□□□□■□□□■□□□□□■□□□■□□□□□");
	gotoxy(26, 9);
	printf("□□□□□■□□□■□□□□□■■■■□□□□□□");
	gotoxy(26, 10);
	printf("□□□□□□□□□□□□□□□□□□□□□□□□□");

	gotoxy(25, 12);
	printf("┏");
	gotoxy(75, 12);
	printf("┓");
	gotoxy(27, 13);
	printf("주목받는것을 좋아한다 / 살짝 관종 끼가 있다");
	gotoxy(27, 15);
	printf("끝까지 살아남기 위한 회피능력과 참을성이 필요하다");
	gotoxy(27, 17);
	printf("서포터와의 팀워크가 매우 중요하다 ");
	gotoxy(27, 19);
	printf("꾸준한 성장으로 후반 게임을 지배한다");
	gotoxy(25, 20);
	printf("┗");
	gotoxy(75, 20);
	printf("┛");
	printf("\n");
}
void display_supporter_trait() {
	//서포터
	system("cls");
	picture();
	gotoxy(44, 2);
	printf("포지션 별 성향  서포터");
	gotoxy(26, 4);
	printf("□□□□□□□□□□□□□□□□□□□□□□□□□");
	gotoxy(26, 5);
	printf("□□■■■■■□□□■■■■■□□□■■■■■□□");
	gotoxy(26, 6);
	printf("□□■□□□□□□□■□□□■□□□□□■□□□□");
	gotoxy(26, 7);
	printf("□□■■■■■□□□■■■■■□□□□□■□□□□");
	gotoxy(26, 8);
	printf("□□□□□□■□□□■□□□□□□□□□■□□□□");
	gotoxy(26, 9);
	printf("□□■■■■■□□□■□□□□□□□□□■□□□□");
	gotoxy(26, 10);
	printf("□□□□□□□□□□□□□□□□□□□□□□□□□");

	gotoxy(25, 12);
	printf("┏");
	gotoxy(75, 12);
	printf("┓");
	gotoxy(27, 13);
	printf("빛...");
	gotoxy(27, 15);
	printf("어머니...");
	gotoxy(27, 17);
	printf("꼼꼼하며 자상하여 팀원의 멘탈을 케어한다");
	gotoxy(27, 19);
	printf("남자는 일등신랑감, 여자는 현모양처다");
	gotoxy(25, 20);
	printf("┗");
	gotoxy(75, 20);
	printf("┛");
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
	printf("┣");
	for (z = 0; z < 99; z++)
		printf("━");
	printf("┫");

	gotoxy(40, 25);
	printf("X : 초기 메뉴로 돌아가기");

	gotoxy(0, 26);
	printf("┣");
	for (z = 0; z < 99; z++)
		printf("━");
	printf("┫");

	while (1) {
		key = _getch();
		if (key == 'x') {
			system("cls");
			printf("초기 메뉴로 돌아갑니다.\n");
			break;
		}
		else {

		}
	}
}
void display_result(int idx) {
	if (idx == TOP) {
		printf("당신에게 가장 적합한 포지션은 '탑' 입니다.\n");
		display_top_trait();
		more_trait();
	}
	else if (idx == MID) {
		printf("당신에게 가장 적합한 포지션은 '미드' 입니다.\n");
		display_mid_trait();
		more_trait();
	}
	else if (idx == JUNGLE) {
		printf("당신에게 가장 적합한 포지션은 '정글' 입니다.\n");
		display_jungle_trait();
		more_trait();
	}
	else if (idx == AD_CARRY) {
		printf("당신에게 가장 적합한 포지션은 '원거리 딜러' 입니다.\n");
		display_ad_carry_trait();
		more_trait();
	}
	else if (idx == SUPPORTER) {
		printf("당신에게 가장 적합한 포지션은 '서포터' 입니다.\n");
		display_supporter_trait();
		more_trait();
	}
	else if (idx == -1) {
		printf("\n테스트를 정상적으로 진행하지 않으셨습니다.\n다시 진행해주세요\n");
		delay(1500);
		PLAY_Rec_Line();
	}
}
int get_result(int Line_rec[]) {//가장 배열값이 높은 idx(포지션) 반환
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
	Question Q_list[qMAX];//질문 리스트
	add_question(Q1, Q2, Q3, Q4, Q5, Q6, Q7, Q8, Q9);//질문 추가
	add_answer(Q_list);//질문 답추가
	play_question(Q_list, Line_rec, Q1, Q2, Q3, Q4, Q5, Q6, Q7, Q8, Q9);//질문 시작
	display_result(get_result(Line_rec));//질문의 결과 출력
}

// end - 1. 포지션 추천

// -----------------------------------------------------------
//2. 챔피언 추천

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
				// Player 챔프 배열 초기화
				for (int i = 0; i < Champ_num; i++) {
					strncpy(player->name[i], arr, sizeof(player->name[i]));
				}
				player->num_cham = 0;
				// 추천 챔프 저장
				save_cham(qtree, player);
				//추천 챔프 출력
				recommand_champ(player);
			}
			_getch();
		}
		else if (choice == '5') {
			player = init_player();
			printf("설정 정보가 초기화 되었습니다.\n");
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

// 사용자 구조체 초기화
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
	// 역할
	player->assassin = 0;
	player->warrior = 0;
	player->wizard = 0;
	player->supporter = 0;
	player->tanker = 0;
	player->ad = 0;
	return player;
}

// 난이도 설정
void select_diff(Player * player)
{
	system("cls");
	picture();

	int i;
	int diff;

	gotoxy(35, 2);
	printf("난이도를 선택해주세요");
	gotoxy(0, 5);
	printf("┣");
	for (i = 0; i < 99; i++)
		printf("━");
	printf("┫");
	gotoxy(35, 7);
	printf("1) 하");
	gotoxy(35, 9);
	printf("2) 중");
	gotoxy(35, 11);
	printf("3) 상");
	gotoxy(0, 13);
	printf("┣");
	for (i = 0; i < 99; i++)
		printf("━");
	printf("┫");
	gotoxy(35, 15);

	printf("0) 이전 메뉴");
	gotoxy(0, 17);
	printf("┣");
	for (i = 0; i < 99; i++)
		printf("━");
	printf("┫");

	diff = _getch();
	if (diff == '3') {
		player->difficulty = 8;
		gotoxy(25,19);
		printf("플레이어의 난이도 설정이 완료되었습니다!\n");
		Sleep(1000);
		_getch();
	}
	else if (diff == '2') {
		player->difficulty = 5;
		gotoxy(25, 19);
		printf("플레이어의 난이도 설정이 완료되었습니다!\n");
		Sleep(1000);
		_getch();
	}
	else if (diff == '1') {
		player->difficulty = 1;
		gotoxy(25, 19);
		printf("플레이어의 난이도 설정이 완료되었습니다!\n");
		Sleep(1000);
		_getch();
	}
	else if (diff == '0') {
		gotoxy(25, 19);
		printf("이전 메뉴로 돌아갑니다");
		Sleep(1000);
		_getch();
	}
	else {
		system("cls");
		select_diff(player);
	}
}

// 라인 설정
void select_line(Player * player)
{
	system("cls");
	picture();

	int i;
	int line; 
	gotoxy(35, 2);
	printf("라인을 선택해주세요");

	gotoxy(0, 5);
	printf("┣");
	for (i = 0; i < 99; i++)
		printf("━");
	printf("┫");

	gotoxy(35, 7);
	printf("1) 탑");
	gotoxy(35, 9);
	printf("2) 정글");
	gotoxy(35, 11);
	printf("3) 미드");
	gotoxy(35, 13);
	printf("4) 원딜");
	gotoxy(35, 15);
	printf("5) 서폿");

	gotoxy(0, 17);
	printf("┣");
	for (i = 0; i < 99; i++)
		printf("━");
	printf("┫");
	gotoxy(35, 19);

	printf("0) 이전 메뉴");
	gotoxy(0, 21);
	printf("┣");
	for (i = 0; i < 99; i++)
		printf("━");
	printf("┫");
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
		printf("플레이어의 라인 설정이 완료되었습니다!\n");
		Sleep(1000);
		_getch();
	}
	else if (line == '0') {
		gotoxy(25, 23);
		printf("이전 메뉴로 돌아갑니다\n");
		_getch();
	}
	else {
		system("cls");
		select_line(player);
	}
}

// 역할군 설정
void select_work(Player * player)
{
	
	system("cls");
	picture();

	int i;
	int work;
	gotoxy(35, 2);
	printf("선호 역할군을 선택해주세요");

	gotoxy(0, 5);
	printf("┣");
	for (i = 0; i < 99; i++)
		printf("━");
	printf("┫");

	gotoxy(35, 7);
	printf("1) 암살자");
	gotoxy(35, 9);
	printf("2) 전사");
	gotoxy(35, 11);
	printf("3) 마법사");
	gotoxy(35, 13);
	printf("4) 서포터");
	gotoxy(35, 15);
	printf("5) 탱커");
	gotoxy(35, 17);
	printf("6) 원거리 딜러");

	gotoxy(0, 19);
	printf("┣");
	for (i = 0; i < 99; i++)
		printf("━");
	printf("┫");
	gotoxy(35, 21);

	printf("0) 이전 메뉴");
	gotoxy(0, 23);
	printf("┣");
	for (i = 0; i < 99; i++)
		printf("━");
	printf("┫");
	
	work = _getch();
	if (work == '1') {
		player->assassin = 1;
		gotoxy(25, 25);
		printf("플레이어의 역할군 설정이 완료되었습니다!\n");
		Sleep(1000);
		_getch();
	}
	else if (work == '2') {
		player->warrior = 1;
		gotoxy(25, 25);
		printf("플레이어의 역할군 설정이 완료되었습니다!\n");
		Sleep(1000);
		_getch();
	}
	else if (work == '3') {
		player->wizard = 1;
		gotoxy(25, 25);
		printf("플레이어의 역할군 설정이 완료되었습니다!\n");
		Sleep(1000);
		_getch();
	}
	else if (work == '4') {
		player->supporter = 1;
		gotoxy(25, 25);
		printf("플레이어의 역할군 설정이 완료되었습니다!\n");
		Sleep(1000);
		_getch();
	}
	else if (work == '5') {
		player->tanker = 1;
		gotoxy(25, 25);
		printf("플레이어의 역할군 설정이 완료되었습니다!\n");
		Sleep(1000);
		_getch();
	}
	else if (work == '6') {
		player->ad = 1;
		gotoxy(25, 25);
		printf("플레이어의 역할군 설정이 완료되었습니다!\n");
		Sleep(1000);
		_getch();
	}
	else if (work == '0') {
		gotoxy(25, 25);
		printf("이전 메뉴로 돌아갑니다\n");
		Sleep(1000);
		_getch();
	}
	else {
		select_work(player);
	}
}

// 챔피언 추천 선택지
void rec_display()
{
	int i;
	system("cls");
	picture();
	gotoxy(45, 1);
	printf("챔피언 추천");
	gotoxy(0, 2);
	printf("┣");
	for (i = 0; i < 99; i++)
		printf("━");
	printf("┫");
	gotoxy(20, 3);
	printf("해당 메뉴들을 선택하여 사용자 선호 정보를 설정해주세요!\n\n");
	gotoxy(20, 4);
	printf("   난이도, 라인은 한가지만 선택 가능합니다!\n");
	gotoxy(20, 5);
	printf("   역할군은 여러개 선택 가능합니다!(하나씩 선택해주세요!)\n");
	gotoxy(20, 6);
	printf("   잘못 설정하셨다면, 5를 눌러 설정 초기화 가능합니다!\n\n");
	gotoxy(20, 7);
	printf("설정 정보들을 종합하여 챔피언을 추천해드립니다!*^^*\n");
	gotoxy(0, 8);
	printf("┣");
	for (i = 0; i < 99; i++)
		printf("━");
	printf("┫");
	gotoxy(40, 10);
	printf("1. 난이도 선택\n");
	gotoxy(40, 12);
	printf("2. 라인 선택\n");
	gotoxy(40, 14);
	printf("3. 역할군 선택\n");
	gotoxy(40, 16);
	printf("4. 종합 챔피언 추천\n");
	gotoxy(40, 18);
	printf("5. 설정 초기화\n");
	gotoxy(0, 20);
	printf("┣");
	for (i = 0; i < 99; i++)
		printf("━");
	printf("┫");
	gotoxy(40,21);
	printf("0. 뒤로 가기\n");
	gotoxy(0, 22);
	printf("┣");
	for (i = 0; i < 99; i++)
		printf("━");
	printf("┫");
	
}
// 사용자 선택 정보 출력
void player_data(Player * player)
{
	int i;
	system("cls");
	picture();

	gotoxy(34, 0);
	printf("┳");

	for (i = 0; i < 8; i++)
	{
		gotoxy(34, 1+i);
		printf("┃");
	}
	gotoxy(0, 2);
	printf("┣");
	for (i = 0; i < 33; i++)
		printf("━");
	printf("┫");

	gotoxy(0,8);
	printf("┣");
	for (i = 0; i < 33; i++)
		printf("━");
	printf("┛");
	gotoxy(9, 1);
	printf("사용자 설정 정보");
	if (player->difficulty == 0 && player->line == -1 && player->assassin == 0 && player->warrior == 0 && player->wizard == 0 && player->supporter == 0 && player->tanker == 0 && player->ad == 0)
	{
		gotoxy(3, 4);
		printf("설정된 사용자 정보가 없습니다!\n");
		Sleep(2000);
	}
	else 
	{
		if (player->difficulty != 0) {
			gotoxy(6,3);
			if (player->difficulty == 8)
				printf("난이도 :  상 \n");
			if (player->difficulty == 5)
				printf("난이도 :  중 \n");
			if (player->difficulty == 1)
				printf("난이도 :  하 \n");
		}
		gotoxy(6, 4);
		if (player->line != -1)
			printf("라인   : ");
		if (player->line == 0)
			printf(" 탑 \n");
		else if (player->line == 1)
			printf(" 정글 \n");
		else if (player->line == 2)
			printf(" 미드 \n");
		else if (player->line == 3)
			printf(" 원딜 \n");
		else if (player->line == 4)
			printf(" 서폿 \n");
		if (player->assassin == 1 || player->warrior == 1 || player->wizard == 1 || player->supporter == 1 || player->tanker == 1 || player->ad == 1) 
		{
			gotoxy(6, 5);
			printf("역활군 :  ");
			if (player->assassin == 1)
				printf("암살자 ");
			if (player->warrior == 1)
				printf("전사 ");
			gotoxy(16, 6);
			if (player->wizard == 1)
				printf("마법사 ");
			if (player->supporter == 1)
				printf("서포터 ");
			gotoxy(16, 7);
			if (player->tanker == 1)
				printf("탱커 ");
			if (player->ad == 1)
				printf("원거리 딜러 ");

		}
		Sleep(2000);
	}
}

// 추천 챔피언 출력
void recommand_champ(Player * player)
{
	int k;
	int x = 0, y = 0;
	if (player->num_cham == 0) {
		gotoxy(25, 3);
		printf("\t 해당 조건을 모두 만족하는 챔프가 없습니다\n");
		gotoxy(0, 8);
		printf("┻");
		for (k = 0; k < 99; k++)
			printf("━");
		printf("┫");
	}
	else {
		gotoxy(55, 1);
		printf("추천 챔프 목록\n");
		gotoxy(34, 2);
		printf("╋");
		for (k = 0; k < 65; k++)
			printf("━");
		printf("┫");
		gotoxy(55, 5);
		printf("챔피언 수 = %d \n", player->num_cham);
		gotoxy(34, 8);
		printf("┻");
		for (k = 0; k < 65; k++)
			printf("━");
		printf("┫");
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

// 추천 챔프를 사용자 챔프 배열에 저장
void save_cham(Tree * qtree, Player * player)
{
	char champ_name[MAX_NAME];
	int line = player->line;
	Node * cham_node = NULL;

	// 라인 설정O -> 특정 라인 순회
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
			// 난이도X
			if (player->difficulty == 0) {
				// 역할군 X
				if (player->assassin == 0 && player->warrior == 0 && player->wizard == 0 && player->supporter == 0 && player->tanker == 0 && player->ad == 0) {
					strncpy(player->name[player->num_cham], cham_node->name, sizeof(player->name[player->num_cham]));
					player->num_cham++;
				}
				// 역할군 O
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
			//난이도O
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
					// 역할군 X
					if (player->assassin == 0 && player->warrior == 0 && player->wizard == 0 && player->supporter == 0 && player->tanker == 0 && player->ad == 0) {
						strncpy(player->name[player->num_cham], cham_node->name, sizeof(player->name[player->num_cham]));
						player->num_cham++;
					}
					// 역할군 O
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

	// 라인 설정X -> 모든 챔피언 순회
	else {
		FILE * file_pointer = NULL;
		file_pointer = fopen("All.txt", "r");
		for (int i = 0; i < Champ_num; i++) {
			fgets(champ_name, MAX_NAME, file_pointer);
			cham_node = find_node(qtree->root, champ_name);
			// 난이도X
			if (player->difficulty == 0) {
				// 역할군 X
				if (player->assassin == 0 && player->warrior == 0 && player->wizard == 0 && player->supporter == 0 && player->tanker == 0 && player->ad == 0) {
					strncpy(player->name[player->num_cham], cham_node->name, sizeof(player->name[player->num_cham]));
					player->num_cham++;
				}
				// 역할군 O
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
			//난이도O
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
					// 역할군 X
					if (player->assassin == 0 && player->warrior == 0 && player->wizard == 0 && player->supporter == 0 && player->tanker == 0 && player->ad == 0) {
						strncpy(player->name[player->num_cham], cham_node->name, sizeof(player->name[player->num_cham]));
						player->num_cham++;
					}
					// 역할군 O
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

// end - 2. 챔피언 추천

// -------------------------------------------------------------

// 3. 챔피언 스토리 및 공략
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
			printf("┏");
			for (i = 0; i < 33; i++)
				printf("━");
			printf("┓");
			gotoxy(30, 12);
			printf("┗");
			for (i = 0; i < 33; i++)
				printf("━");
			printf("┛");

			gotoxy(30, 11);
			printf("┃ 챔피언 정보 사이트로 이동합니다!┃\n");
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
			printf("┏");
			for (i = 0; i < 35; i++)
				printf("━");
			printf("┓");
			gotoxy(30, 12);
			printf("┗");
			for (i = 0; i < 35; i++)
				printf("━");
			printf("┛");
			gotoxy(30, 11);
			printf("┃  챔피언 분석 사이트로 이동합니다! ┃");
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

// 스토리 및 공략 디스플레이
void Story_Strategy_display()
{
	int i;
	int x, y = 0;
	picture();
	gotoxy(35,5);
	printf("┏");
	for (i = 0; i < 30; i++)
		printf("━");
	printf("┓");
	x = 35;
	y = 5;
	for (i = 0; i < 12; i++)
	{
		y++;
		gotoxy(x, y);
		printf("┃");
	}
	x = 66;
	y = 5;
	for (i = 0; i < 12; i++)
	{
		y++;
		gotoxy(x, y);
		printf("┃");
	}
	gotoxy(40, 6);
	printf(" 1. 챔피언 정보 사이트\n");
	gotoxy(40, 8);
	printf(" 2. 챔피언 스토리 보기\n");
	gotoxy(40, 10);
	printf(" 3. 챔피언 분석 사이트\n");
	gotoxy(40,12);
	printf(" 4. 선택 챔피언 공략\n");
	gotoxy(35,14);
	printf("┣");
	for (i = 0; i < 30; i++)
		printf("━");
	printf("┫");
	gotoxy(40,16);
	printf(" 0. 뒤로 가기\n");
	gotoxy(35,18);
	printf("┗");
	for (i = 0; i < 30; i++)
		printf("━");
	printf("┛");

}

// 2) 스토리 디스플레이
void story_display()
{
	int i;
	int x = 0, y = 0;
	picture();
	gotoxy(3, 2);
	printf("┏");
	for (i = 0; i < 93; i++)
		printf("━");
	printf("┓");
	x = 3;
	y = 2;
	for (i = 0; i < 19; i++)
	{
		y++;
		gotoxy(x, y);
		printf("┃");
	}
	x = 97;
	y = 2;
	for (i = 0; i < 19; i++)
	{
		y++;
		gotoxy(x, y);
		printf("┃");
	}
	gotoxy(26, 3);
	printf("스토리 보기를 원하는 챔피언을 입력하세요!\n");
	gotoxy(84, 3);
	printf(" 0. 뒤로 가기\n");
	gotoxy(5,5);
	printf("가렌 갈리오 갱플랭크 그라가스 그레이브즈 나르 나미 나서스 노틸러스 녹턴 누누와 윌럼프 니달리");
	gotoxy(5, 6);
	printf("니코 다리우스 다이애나 드레이븐 라이즈 라칸 람머스 럭스 럼블 레넥톤 레오나 렉사이 렝가");
	gotoxy(5, 7);
	printf("루시안 룰루 르블랑 리신 리븐 리산드라 마스터 이 마오카이 말자하 말파이트 모데카이저 모르가나");
	gotoxy(5, 8);
	printf("문도 박사 미스 포츈 바드 바루스 바이 베이가 베인 벨코즈 볼리베어 브라움 브랜드 블라디미르");
	gotoxy(5, 9);
	printf("블리츠크랭크 빅토르 뽀삐 사이온 사일러스 샤코 세주아니 소나 소라카 쉔 쉬바나 스웨인 스카너");
	gotoxy(5, 10);
	printf("시비르 신 짜오 신드라 신지드 쓰레쉬 아리 아무무 아우렐리온 솔 아이번 아지르 아칼리 아트록스");
	gotoxy(5, 11);
	printf("알리스타 애니 애니비아 애쉬 야스오 에코 엘리스 오공 오른 오리아나 올라프 요릭 우디르 우르곳");
	gotoxy(5, 12);
	printf("워윅 유미 이렐리아 이블린 이즈리얼 일라오이 자르반 4세 자야 자이라 자크 잔나 잭스 제드");
	gotoxy(5, 13);
	printf("제라스 제이스 조이 직스 진 질리언 징크스 초가스 카르마 카밀 카사딘 카서스 카시오페아 카이사");
	gotoxy(5, 14);
	printf("카직스 카타리나 칼리스타 케넨 케이틀린 케인 케일 코그모 코르키 퀸 클레드 킨드레드 타릭 탈론");
	gotoxy(5, 15);
	printf("탈리야 탐 켄치 트런들 트리스타나 트린다미어 트위스티드 페이트 트위치 티모 파이크 판테온");
	gotoxy(5, 16);
	printf("피들스틱 피오라 피즈 하이머딩거 헤카림");
	
	gotoxy(3, 4);
	printf("┣");
	for (i = 0; i < 93; i++)
		printf("━");
	printf("┫");

	gotoxy(3, 18);
	printf("┣");
	for (i = 0; i < 93; i++)
		printf("━");
	printf("┫");
	gotoxy(35, 20);
	printf(">> ");
	
	gotoxy(3, 22);
	printf("┗");
	for (i = 0; i < 93; i++)
		printf("━");
	printf("┛");

	gotoxy(83, 2);
	printf("┳");
	gotoxy(83, 3);
	printf("┃");
	gotoxy(83, 4);
	printf("┻");
	gotoxy(38, 20);
}

// 2) 스토리 사이트 실행
void print_story()
{
	int i;
	int x = 0, y = 0;
	char champ[MAX_NAME];
	scanf("%s", champ);
	system("cls");
	picture();

	gotoxy(22, 4);
	printf("┏");
	for (i = 0; i < 48; i++)
		printf("━");
	printf("┓");
	x = 22;
	y = 4;
	for (i = 0; i < 1; i++)
	{
		y++;
		gotoxy(x, y);
		printf("┃");
	}
	x = 71;
	y = 4;
	for (i = 0; i < 1; i++)
	{
		y++;
		gotoxy(x, y);
		printf("┃");
	}
	gotoxy(22, 6);
	printf("┗");
	for (i = 0; i < 48; i++)
		printf("━");
	printf("┛");
	gotoxy(24, 5);
	if (strcmp(champ, "가렌") == 0) {
		printf("가렌의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Garen");
		_getch();
	}
	else if (strcmp(champ, "갈리오") == 0) {
		printf("갈리오의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/galio");
		_getch();
	}
	else if (strcmp(champ, "갱플랭크") == 0 || strcmp(champ, "갱플") == 0) {
		printf("갱플랭크의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/gangplank");
		_getch();
	}
	else if (strcmp(champ, "그라가스") == 0 || strcmp(champ, "글가") == 0) {
		printf("그라가스의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/gragas");
		_getch();
	}
	else if (strcmp(champ, "그레이브즈") == 0 || strcmp(champ, "그브") == 0) {
		printf("그레이브즈의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/graves");
		_getch();
	}
	else if (strcmp(champ, "나미") == 0) {
		printf("나미의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/nami");
		_getch();
	}
	else if (strcmp(champ, "나서스") == 0) {
		printf("나서스의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/nasus");
		_getch();
	}
	else if (strcmp(champ, "노틸러스") == 0 || strcmp(champ, "노틸") == 0) {
		printf("노틸러스의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/nautilus");
		_getch();
	}
	else if (strcmp(champ, "녹턴") == 0) {
		printf("녹턴의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/nocturne");
		_getch();
	}
	else if (strcmp(champ, "누누") == 0) {
		printf("누누의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/nunu");
		_getch();
	}
	else if (strcmp(champ, "니달리") == 0) {
		printf("니달리의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/nidalee");
		_getch();
	}
	else if (strcmp(champ, "다리우스") == 0 || strcmp(champ, "다리") == 0) {
		printf("다리우스의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/darius");
		_getch();
	}
	else if (strcmp(champ, "다이애나") == 0) {
		printf("다이애나의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/diana");
		_getch();
	}
	else if (strcmp(champ, "드레이븐") == 0) {
		printf("드레이븐의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/draven");
		_getch();
	}
	else if (strcmp(champ, "라이즈") == 0) {
		printf("라이즈의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/ryze");
		_getch();
	}
	else if (strcmp(champ, "람머스") == 0) {
		printf("람머스의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/rammus");
		_getch();
	}
	else if (strcmp(champ, "럭스") == 0) {
		printf("럭스의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/lux");
		_getch();
	}
	else if (strcmp(champ, "럼블") == 0) {
		printf("럼블의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/rumble");
		_getch();
	}
	else if (strcmp(champ, "레넥톤") == 0) {
		printf("레넥톤의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/renekton");
		_getch();
	}
	else if (strcmp(champ, "레오나") == 0) {
		printf("레오나의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Leona");
		_getch();
	}
	else if (strcmp(champ, "렝가") == 0) {
		printf("렝가의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Rengar");
		_getch();
	}
	else if (strcmp(champ, "루시안") == 0) {
		printf("루시안의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Lucian");
		_getch();
	}
	else if (strcmp(champ, "룰루") == 0) {
		printf("룰루의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Lulu");
		_getch();
	}
	else if (strcmp(champ, "르블랑") == 0) {
		printf("르블랑의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/LeBlanc");
		_getch();
	}
	else if (strcmp(champ, "리신") == 0) {
		printf("리 신의 스토리 사이트로 이동합니다!\n"); 
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/LeeSin");
		_getch();
	}
	else if (strcmp(champ, "리븐") == 0) {
		printf("리븐의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Riven");
		_getch();
	}
	else if (strcmp(champ, "리산드라") == 0 || strcmp(champ, "리산") == 0) {
		printf("리산드라의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Lissandra");
		_getch();
	}
	else if (strcmp(champ, "마스터이") == 0 || strcmp(champ, "마이") == 0) {
		printf("마스터 이의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/MasterYi");
		_getch();
	}
	else if (strcmp(champ, "마오카이") == 0) {
		printf("마오카이의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Maokai");
		_getch();
	}
	else if (strcmp(champ, "말자하") == 0) {
		printf("말자하의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Malzahar");
		_getch();
	}
	else if (strcmp(champ, "말파이트") == 0 || strcmp(champ, "말파") == 0) {
		printf("말파이트의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Malphite");
		_getch();
	}
	else if (strcmp(champ, "모데카이저") == 0 || strcmp(champ, "모데") == 0) {
		printf("모데카이저의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Mordekaiser");
		_getch();
	}
	else if (strcmp(champ, "모르가나") == 0 || strcmp(champ, "몰가") == 0) {
		printf("모르가나의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Morgana");
		_getch();
	}
	else if (strcmp(champ, "문도박사") == 0 || strcmp(champ, "문도") == 0) {
		printf("문도박사의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/DrMundo");
		_getch();
	}
	else if (strcmp(champ, "미스포츈") == 0 || strcmp(champ, "미포") == 0) {
		printf("미스 포츈의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/MissFortune");
		_getch();
	}
	else if (strcmp(champ, "바루스") == 0) {
		printf("바루스의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Varus");
		_getch();
	}
	else if (strcmp(champ, "바이") == 0) {
		printf("바이의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Vi");
		_getch();
	}
	else if (strcmp(champ, "베이가") == 0) {
		printf("베이가의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Veigar");
		_getch();
	}
	else if (strcmp(champ, "베인") == 0) {
		printf("베인의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Vayne");
		_getch();
	}
	else if (strcmp(champ, "볼리베어") == 0 || strcmp(champ, "볼베") == 0) {
		printf("볼리베어의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Volibear");
		_getch();
	}
	else if (strcmp(champ, "브랜드") == 0) {
		printf("브랜드의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Brand");
		_getch();
	}
	else if (strcmp(champ, "블라디미르") == 0 || strcmp(champ, "블라디") == 0) {
		printf("블라디미르의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Vladimir");
		_getch();
	}
	else if (strcmp(champ, "블리츠크랭크") == 0 || strcmp(champ, "블츠") == 0) {
		printf("블리츠크랭크의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Blitzcrank");
		_getch();
	}
	else if (strcmp(champ, "빅토르") == 0) {
		printf("빅토르의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Viktor");
		_getch();
	}
	else if (strcmp(champ, "뽀삐") == 0) {
		printf("뽀삐의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Poppy");
		_getch();
	}
	else if (strcmp(champ, "사이온") == 0) {
		printf("사이온의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Sion");
		_getch();
	}
	else if (strcmp(champ, "샤코") == 0) {
		printf("샤코의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Shaco");
		_getch();
	}
	else if (strcmp(champ, "세주아니") == 0 || strcmp(champ, "세주") == 0) {
		printf("세주아니의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Sejuani");
		_getch();
	}
	else if (strcmp(champ, "소나") == 0) {
		printf("소나의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Sona");
		_getch();
	}
	else if (strcmp(champ, "소라카") == 0) {
		printf("소라카의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Soraka");
		_getch();
	}
	else if (strcmp(champ, "쉔") == 0) {
		printf("쉔의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Shen");
		_getch();
	}
	else if (strcmp(champ, "쉬바나") == 0) {
		printf("쉬바나의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Shyvana");
		_getch();
	}
	else if (strcmp(champ, "스웨인") == 0) {
		printf("스웨인의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Swain");
		_getch();
	}
	else if (strcmp(champ, "스카너") == 0) {
		printf("스카너의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Skarner");
		_getch();
	}
	else if (strcmp(champ, "시비르") == 0) {
		printf("시비르의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Sivir");
		_getch();
	}
	else if (strcmp(champ, "신짜오") == 0) {
		printf("신 짜오의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/XinZhao");
		_getch();
	}
	else if (strcmp(champ, "신드라") == 0) {
		printf("신드라의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Syndra");
		_getch();
	}
	else if (strcmp(champ, "신지드") == 0) {
		printf("신지드의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Singed");
		_getch();
	}
	else if (strcmp(champ, "쓰레쉬") == 0) {
		printf("쓰레쉬의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Thresh");
		_getch();
	}
	else if (strcmp(champ, "아리") == 0) {
		printf("아리의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Ahri");
		_getch();
	}
	else if (strcmp(champ, "아무무") == 0 || strcmp(champ, "무무") == 0) {
		printf("아무무의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Amumu");
		_getch();
	}
	else if (strcmp(champ, "아칼리") == 0) {
		printf("아칼리의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Akali");
		_getch();
	}
	else if (strcmp(champ, "아트록스") == 0 || strcmp(champ, "아트") == 0) {
		printf("아트록스의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Aatrox");
		_getch();
	}
	else if (strcmp(champ, "알리스타") == 0 || strcmp(champ, "알리") == 0) {
		printf("알리스타의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Alistar");
		_getch();
	}
	else if (strcmp(champ, "애니") == 0) {
		printf("애니의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Annie");
		_getch();
	}
	else if (strcmp(champ, "애니비아") == 0) {
		printf("애니비아의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Anivia");
		_getch();
	}
	else if (strcmp(champ, "애쉬") == 0) {
		printf("애쉬의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Ashe");
		_getch();
	}
	else if (strcmp(champ, "엘리스") == 0) {
		printf("엘리스의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Elise");
		_getch();
	}
	else if (strcmp(champ, "오공") == 0) {
		printf("오공의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Wukong");
		_getch();
	}
	else if (strcmp(champ, "오리아나") == 0 || strcmp(champ, "오리") == 0) {
		printf("오리아나의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Orianna");
		_getch();
	}
	else if (strcmp(champ, "올라프") == 0) {
		printf("올라프의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Olaf");
		_getch();
	}
	else if (strcmp(champ, "요릭") == 0) {
		printf("요릭의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Yorick");
		_getch();
	}
	else if (strcmp(champ, "우디르") == 0) {
		printf("우디르의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Udyr");
		_getch();
	}
	else if (strcmp(champ, "우르곳") == 0) {
		printf("우르곳의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Urgot");
		_getch();
	}
	else if (strcmp(champ, "워윅") == 0) {
		printf("워윅의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Warwick");
		_getch();
	}
	else if (strcmp(champ, "이렐리아") == 0 || strcmp(champ, "이렐") == 0) {
		printf("이렐리아의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Irelia");
		_getch();
	}
	else if (strcmp(champ, "이블린") == 0) {
		printf("이블린의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Evelynn");
		_getch();
	}
	else if (strcmp(champ, "이즈리얼") == 0 || strcmp(champ, "이즈") == 0) {
		printf("이즈리얼의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Ezreal");
		_getch();
	}
	else if (strcmp(champ, "자르반4세") == 0 || strcmp(champ, "잘반") == 0) {
		printf("자르반 4세의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/JarvanIV");
		_getch();
	}
	else if (strcmp(champ, "자이라") == 0) {
		printf("자이라의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Zyra");
		_getch();
	}
	else if (strcmp(champ, "자크") == 0) {
		printf("자크의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Zac");
		_getch();
	}
	else if (strcmp(champ, "잔나") == 0) {
		printf("잔나의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Janna");
		_getch();
	}
	else if (strcmp(champ, "잭스") == 0) {
		printf("잭스의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Jax");
		_getch();
	}
	else if (strcmp(champ, "제드") == 0) {
		printf("제드의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Zed");
		_getch();
	}
	else if (strcmp(champ, "제라스") == 0) {
		printf("제라스의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Xerath");
		_getch();
	}
	else if (strcmp(champ, "제이스") == 0) {
		printf("제이스의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Jayce");
		_getch();
	}
	else if (strcmp(champ, "직스") == 0) {
		printf("직스의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Ziggs");
		_getch();
	}
	else if (strcmp(champ, "질리언") == 0) {
		printf("질리언의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Zilean");
		_getch();
	}
	else if (strcmp(champ, "징크스") == 0) {
		printf("징크스의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Jinx");
		_getch();
	}
	else if (strcmp(champ, "초가스") == 0) {
		printf("초가스의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/ChoGath");
		_getch();
	}
	else if (strcmp(champ, "카르마") == 0) {
		printf("카르마의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Karma");
		_getch();
	}
	else if (strcmp(champ, "카사딘") == 0) {
		printf("카사딘의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Kassadin");
		_getch();
	}
	else if (strcmp(champ, "카서스") == 0) {
		printf("카서스의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Karthus");
		_getch();
	}
	else if (strcmp(champ, "카시오페아") == 0 || strcmp(champ, "카시") == 0) {
		printf("카시오페아의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Cassiopeia");
		_getch();
	}
	else if (strcmp(champ, "카직스") == 0) {
		printf("카직스의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/KhaZix");
		_getch();
	}
	else if (strcmp(champ, "카타리나") == 0 || strcmp(champ, "카타") == 0) {
		printf("카타리나의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Katarina");
		_getch();
	}
	else if (strcmp(champ, "케넨") == 0) {
		printf("케넨의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Kennen");
		_getch();
	}
	else if (strcmp(champ, "케이틀린") == 0 || strcmp(champ, "케틀") == 0) {
		printf("케이틀린의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Caitlyn");
		_getch();
	}
	else if (strcmp(champ, "케일") == 0) {
		printf("케일의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Kayle");
		_getch();
	}
	else if (strcmp(champ, "코그모") == 0) {
		printf("코그모의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/KogMaw");
		_getch();
	}
	else if (strcmp(champ, "코르키") == 0) {
		printf("코르키 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Corki");
		_getch();
	}
	else if (strcmp(champ, "퀸") == 0) {
		printf("퀸의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Quinn");
		_getch();
	}
	else if (strcmp(champ, "타릭") == 0) {
		printf("타릭의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Taric");
		_getch();
	}
	else if (strcmp(champ, "탈론") == 0) {
		printf("탈론의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Talon");
		_getch();
	}
	else if (strcmp(champ, "트런들") == 0) {
		printf("트런들의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Trundle");
		_getch();
	}
	else if (strcmp(champ, "트리스타나") == 0 || strcmp(champ, "트타") == 0) {
		printf("트리스타나의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Tristana");
		_getch();
	}
	else if (strcmp(champ, "트린다미어") == 0 || strcmp(champ, "트린") == 0) {
		printf("트린다미어의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Tryndamere");
		_getch();
	}
	else if (strcmp(champ, "트위스티드페이트") == 0 || strcmp(champ, "트페") == 0) {
		printf("트위스티드 페이트의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/TwistedFate");
		_getch();
	}
	else if (strcmp(champ, "트위치") == 0) {
		printf("트위치의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Twitch");
		_getch();
	}
	else if (strcmp(champ, "티모") == 0) {
		printf("티모의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Teemo");
		_getch();
	}
	else if (strcmp(champ, "판테온") == 0) {
		printf("판테온의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Pantheon");
		_getch();
	}
	else if (strcmp(champ, "피들스틱") == 0 || strcmp(champ, "피들") == 0) {
		printf("피들스틱의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Fiddlesticks");
		_getch();
	}
	else if (strcmp(champ, "피오라") == 0) {
		printf("피오라의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Fiora");
		_getch();
	}
	else if (strcmp(champ, "피즈") == 0) {
		printf("피즈의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Fizz");
		_getch();
	}
	else if (strcmp(champ, "하이머딩거") == 0 || strcmp(champ, "딩거") == 0) {
		printf("하이머딩거의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Heimerdinger");
		_getch();
	}
	else if (strcmp(champ, "헤카림") == 0) {
		printf("헤카림의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Hecarim");
		_getch();
	}
	else if (strcmp(champ, "야스오") == 0) {
		printf("야스오의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Yasuo");
		_getch();
	}
	else if (strcmp(champ, "벨코즈") == 0) {
		printf("벨코즈의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/VelKoz");
		_getch();
	}
	else if (strcmp(champ, "브라움") == 0) {
		printf("브라움의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Braum");
		_getch();
	}
	else if (strcmp(champ, "나르") == 0) {
		printf("나르의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Gnar");
		_getch();
	}
	else if (strcmp(champ, "아지르") == 0) {
		printf("아지르의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Azir");
		_getch();
	}
	else if (strcmp(champ, "칼리스타") == 0 || strcmp(champ, "칼리") == 0) {
		printf("칼리스타의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Kalista");
		_getch();
	}
	else if (strcmp(champ, "렉사이") == 0) {
		printf("렉사이의 스토리 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://universe.leagueoflegends.com/ko_KR/champion/RekSai");
		_getch();
	}
	else if (strcmp(champ, "바드") == 0) {
		printf("바드의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Bard");
		_getch();
	}
	else if (strcmp(champ, "에코") == 0) {
		printf("에코의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Ekko");
		_getch();
	}
	else if (strcmp(champ, "탐켄치") == 0 || strcmp(champ, "켄치") == 0) {
		printf("탐 켄치의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/TahmKench");
		_getch();
	}
	else if (strcmp(champ, "킨드레드") == 0 || strcmp(champ, "킨드") == 0) {
		printf("킨드레드의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Kindred");
		_getch();
	}
	else if (strcmp(champ, "일라오이") == 0 || strcmp(champ, "일라") == 0) {
		printf("일라오이의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Illaoi");
		_getch();
	}
	else if (strcmp(champ, "진") == 0) {
		printf("진의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Jhin");
		_getch();
	}
	else if (strcmp(champ, "아우렐리온솔") == 0 || strcmp(champ, "아솔") == 0) {
		printf("아우렐리온 솔의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/AurelionSol");
		_getch();
	}
	else if (strcmp(champ, "탈리야") == 0) {
		printf("탈리야의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Taliyah");
		_getch();
	}
	else if (strcmp(champ, "클레드") == 0) {
		printf("클레드의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Kled");
		_getch();
	}
	else if (strcmp(champ, "아이번") == 0) {
		printf("아이번의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Ivern");
		_getch();
	}
	else if (strcmp(champ, "카밀") == 0) {
		printf("카밀의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Camille");
		_getch();
	}
	else if (strcmp(champ, "자야") == 0) {
		printf("자야의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Xayah");
		_getch();
	}
	else if (strcmp(champ, "라칸") == 0) {
		printf("라칸의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Rakan");
		_getch();
	}
	else if (strcmp(champ, "오른") == 0) {
		printf("오른의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Ornn");
		_getch();
	}
	else if (strcmp(champ, "조이") == 0) {
		printf("조이의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Zoe");
		_getch();
	}
	else if (strcmp(champ, "케인") == 0) {
		printf("케인의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Kayn");
		_getch();
	}
	else if (strcmp(champ, "카이사") == 0) {
		printf("카이사의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/KaiSa");
		_getch();
	}
	else if (strcmp(champ, "파이크") == 0) {
		printf("파이크의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Pyke");
		_getch();
	}
	else if (strcmp(champ, "니코") == 0) {
		printf("니코의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Neeko");
		_getch();
	}
	else if (strcmp(champ, "사일러스") == 0) {
		printf("사일러스의 스토리 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://universe.leagueoflegends.com/ko_KR/champion/Sylas");
		_getch();
	}
	else if (strcmp(champ, "유미") == 0) {
		printf("유미의 스토리 사이트로 이동합니다!\n");
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

// 3) 공략 디스플레이
void strategy_display()
{
	int i;
	int x = 0, y = 0;
	picture();
	gotoxy(3, 2);
	printf("┏");
	for (i = 0; i < 93; i++)
		printf("━");
	printf("┓");
	x = 3;
	y = 2;
	for (i = 0; i < 19; i++)
	{
		y++;
		gotoxy(x, y);
		printf("┃");
	}
	x = 97;
	y = 2;
	for (i = 0; i < 19; i++)
	{
		y++;
		gotoxy(x, y);
		printf("┃");
	}
	gotoxy(26, 3);
	printf("공략 보기를 원하는 챔피언을 입력하세요!\n");
	gotoxy(84, 3);
	printf(" 0. 뒤로 가기\n");
	gotoxy(5, 5);
	printf("가렌 갈리오 갱플랭크 그라가스 그레이브즈 나르 나미 나서스 노틸러스 녹턴 누누와 윌럼프 니달리");
	gotoxy(5, 6);
	printf("니코 다리우스 다이애나 드레이븐 라이즈 라칸 람머스 럭스 럼블 레넥톤 레오나 렉사이 렝가");
	gotoxy(5, 7);
	printf("루시안 룰루 르블랑 리신 리븐 리산드라 마스터 이 마오카이 말자하 말파이트 모데카이저 모르가나");
	gotoxy(5, 8);
	printf("문도 박사 미스 포츈 바드 바루스 바이 베이가 베인 벨코즈 볼리베어 브라움 브랜드 블라디미르");
	gotoxy(5, 9);
	printf("블리츠크랭크 빅토르 뽀삐 사이온 사일러스 샤코 세주아니 소나 소라카 쉔 쉬바나 스웨인 스카너");
	gotoxy(5, 10);
	printf("시비르 신 짜오 신드라 신지드 쓰레쉬 아리 아무무 아우렐리온 솔 아이번 아지르 아칼리 아트록스");
	gotoxy(5, 11);
	printf("알리스타 애니 애니비아 애쉬 야스오 에코 엘리스 오공 오른 오리아나 올라프 요릭 우디르 우르곳");
	gotoxy(5, 12);
	printf("워윅 유미 이렐리아 이블린 이즈리얼 일라오이 자르반 4세 자야 자이라 자크 잔나 잭스 제드");
	gotoxy(5, 13);
	printf("제라스 제이스 조이 직스 진 질리언 징크스 초가스 카르마 카밀 카사딘 카서스 카시오페아 카이사");
	gotoxy(5, 14);
	printf("카직스 카타리나 칼리스타 케넨 케이틀린 케인 케일 코그모 코르키 퀸 클레드 킨드레드 타릭 탈론");
	gotoxy(5, 15);
	printf("탈리야 탐 켄치 트런들 트리스타나 트린다미어 트위스티드 페이트 트위치 티모 파이크 판테온");
	gotoxy(5, 16);
	printf("피들스틱 피오라 피즈 하이머딩거 헤카림");

	gotoxy(3, 4);
	printf("┣");
	for (i = 0; i < 93; i++)
		printf("━");
	printf("┫");

	gotoxy(3, 18);
	printf("┣");
	for (i = 0; i < 93; i++)
		printf("━");
	printf("┫");
	gotoxy(35, 20);
	printf(">> ");

	gotoxy(3, 22);
	printf("┗");
	for (i = 0; i < 93; i++)
		printf("━");
	printf("┛");

	gotoxy(83, 2);
	printf("┳");
	gotoxy(83, 3);
	printf("┃");
	gotoxy(83, 4);
	printf("┻");
	gotoxy(38, 20);
}

// 3) 공략 사이트 실행
void print_strategy()
{
	int i;
	int x = 0, y = 0;
	char champ[MAX_NAME];
	scanf("%s", champ);
	system("cls");
	picture();

	gotoxy(23, 4);
	printf("┏");
	for (i = 0; i < 46; i++)
		printf("━");
	printf("┓");
	x = 23;
	y = 4;
	for (i = 0; i < 1; i++)
	{
		y++;
		gotoxy(x, y);
		printf("┃");
	}
	x = 70;
	y = 4;
	for (i = 0; i < 1; i++)
	{
		y++;
		gotoxy(x, y);
		printf("┃");
	}
	gotoxy(23, 6);
	printf("┗");
	for (i = 0; i < 46; i++)
		printf("━");
	printf("┛");
	gotoxy(25, 5);
	if (strcmp(champ, "가렌") == 0) {
		printf("가렌의 공략 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://www.op.gg/champion/Garen/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "갈리오") == 0) {
		printf("갈리오의 공략 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://www.op.gg/champion/galio/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "갱플랭크") == 0 || strcmp(champ, "갱플") == 0) {
		printf("갱플랭크의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/gangplank/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "그라가스") == 0 || strcmp(champ, "글가") == 0) {
		printf("그라가스의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/gragas/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "그레이브즈") == 0 || strcmp(champ, "그브") == 0) {
		printf("그레이브즈의 공략 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://www.op.gg/champion/graves/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "나미") == 0) {
		printf("나미의 공략 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://www.op.gg/champion/nami/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "나서스") == 0) {
		printf("나서스의 공략 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://www.op.gg/champion/nasus/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "노틸러스") == 0 || strcmp(champ, "노틸") == 0) {
		printf("노틸러스의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/nautilus/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "녹턴") == 0) {
		printf("녹턴의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/nocturne/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "누누와윌럼프") == 0 || strcmp(champ, "누누") == 0) {
		printf("누누와 윌럼프의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/nunu/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "니달리") == 0) {
		printf("니달리의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/nidalee/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "다리우스") == 0 || strcmp(champ, "다리") == 0) {
		printf("다리우스의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/darius/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "다이애나") == 0) {
		printf("다이애나의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/diana/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "드레이븐") == 0) {
		printf("드레이븐의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/draven/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "라이즈") == 0) {
		printf("라이즈의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/ryze/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "람머스") == 0) {
		printf("람머스의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/rammus/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "럭스") == 0) {
		printf("럭스의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/lux/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "럼블") == 0) {
		printf("럼블의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/rumble/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "레넥톤") == 0) {
		printf("레넥톤의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/renekton/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "레오나") == 0) {
		printf("레오나의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Leona/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "렝가") == 0) {
		printf("렝가의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Rengar/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "루시안") == 0) {
		printf("루시안의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Lucian/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "룰루") == 0) {
		printf("룰루의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Lulu/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "르블랑") == 0) {
		printf("르블랑의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/LeBlanc/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "리신") == 0) {
		printf("리 신의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/LeeSin/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "리븐") == 0) {
		printf("리븐의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Riven/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "리산드라") == 0 || strcmp(champ, "리산") == 0) {
		printf("리산드라의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Lissandra/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "마스터이") == 0 || strcmp(champ, "마이") == 0) {
		printf("마스터 이의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/MasterYi/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "마오카이") == 0) {
		printf("마오카이의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Maokai/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "말자하") == 0) {
		printf("말자하의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Malzahar/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "말파이트") == 0 || strcmp(champ, "말파") == 0) {
		printf("말파이트의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Malphite/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "모데카이저") == 0 || strcmp(champ, "모데") == 0) {
		printf("모데카이저의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Mordekaiser/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "모르가나") == 0 || strcmp(champ, "몰가") == 0) {
		printf("모르가나의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Morgana/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "문도박사") == 0 || strcmp(champ, "문도") == 0) {
		printf("문도박사의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/DrMundo/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "미스포츈") == 0 || strcmp(champ, "미포") == 0) {
		printf("미스 포츈의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/MissFortune/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "바루스") == 0) {
		printf("바루스의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Varus/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "바이") == 0) {
		printf("바이의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Vi/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "베이가") == 0) {
		printf("베이가의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Veigar/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "베인") == 0) {
		printf("베인의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Vayne/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "볼리베어") == 0 || strcmp(champ, "볼베") == 0) {
		printf("볼리베어의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Volibear/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "브랜드") == 0) {
		printf("브랜드의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Brand/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "블라디미르") == 0 || strcmp(champ, "블라디") == 0) {
		printf("블라디미르의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Vladimir/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "블리츠크랭크") == 0 || strcmp(champ, "블츠") == 0) {
		printf("블리츠크랭크의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Blitzcrank/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "빅토르") == 0) {
		printf("빅토르의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Viktor/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "뽀삐") == 0) {
		printf("뽀삐의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Poppy/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "사이온") == 0) {
		printf("사이온의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Sion/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "샤코") == 0) {
		printf("샤코의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Shaco/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "세주아니") == 0 || strcmp(champ, "세주") == 0) {
		printf("세주아니의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Sejuani/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "소나") == 0) {
		printf("소나의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Sona/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "소라카") == 0) {
		printf("소라카의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Soraka/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "쉔") == 0) {
		printf("쉔의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Shen/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "쉬바나") == 0) {
		printf("쉬바나의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Shyvana/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "스웨인") == 0) {
		printf("스웨인의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Swain/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "스카너") == 0) {
		printf("스카너의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Skarner/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "시비르") == 0) {
		printf("시비르의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Sivir/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "신짜오") == 0) {
		printf("신 짜오의 공략 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://www.op.gg/champion/XinZhao/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "신드라") == 0) {
		printf("신드라의 공략 사이트로 이동합니다!\n");
		Sleep(300);
		system("start https://www.op.gg/champion/Syndra/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "신지드") == 0) {
		printf("신지드의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Singed/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "쓰레쉬") == 0) {
		printf("쓰레쉬의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Thresh/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "아리") == 0) {
		printf("아리의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Ahri/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "아무무") == 0 || strcmp(champ, "무무") == 0) {
		printf("아무무의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Amumu/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "아칼리") == 0) {
		printf("아칼리의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Akali/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "아트록스") == 0 || strcmp(champ, "아트") == 0) {
		printf("아트록스의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Aatrox/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "알리스타") == 0 || strcmp(champ, "알리") == 0) {
		printf("알리스타의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Alistar/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "애니") == 0) {
		printf("애니의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Annie/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "애니비아") == 0) {
		printf("애니비아의 공략 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://www.op.gg/champion/Anivia/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "애쉬") == 0) {
		printf("애쉬의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Ashe/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "엘리스") == 0) {
		printf("엘리스의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Elise/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "오공") == 0) {
		printf("오공의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Wukong/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "오리아나") == 0 || strcmp(champ, "오리") == 0) {
		printf("오리아나의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Orianna/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "올라프") == 0) {
		printf("올라프의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Olaf/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "요릭") == 0) {
		printf("요릭의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Yorick/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "우디르") == 0) {
		printf("우디르의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Udyr/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "우르곳") == 0) {
		printf("우르곳의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Urgot/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "워윅") == 0) {
		printf("워윅의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Warwick/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "이렐리아") == 0 || strcmp(champ, "이렐") == 0) {
		printf("이렐리아의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Irelia/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "이블린") == 0) {
		printf("이블린의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Evelynn/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "이즈리얼") == 0 || strcmp(champ, "이즈") == 0) {
		printf("이즈리얼의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Ezreal/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "자르반4세") == 0 || strcmp(champ, "잘반") == 0) {
		printf("자르반 4세의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/JarvanIV/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "자이라") == 0) {
		printf("자이라의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Zyra/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "자크") == 0) {
		printf("자크의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Zac/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "잔나") == 0) {
		printf("잔나의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Janna/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "잭스") == 0) {
		printf("잭스의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Jax/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "제드") == 0) {
		printf("제드의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Zed/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "제라스") == 0) {
		printf("제라스의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Xerath/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "제이스") == 0) {
		printf("제이스의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Jayce/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "직스") == 0) {
		printf("직스의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Ziggs/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "질리언") == 0) {
		printf("질리언의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Zilean/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "징크스") == 0) {
		printf("징크스의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Jinx/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "초가스") == 0) {
		printf("초가스의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/ChoGath/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "카르마") == 0) {
		printf("카르마의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Karma/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "카사딘") == 0) {
		printf("카사딘의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Kassadin/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "카서스") == 0) {
		printf("카서스의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Karthus/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "카시오페아") == 0 || strcmp(champ, "카시") == 0) {
		printf("카시오페아의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Cassiopeia/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "카직스") == 0) {
		printf("카직스의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/KhaZix/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "카타리나") == 0 || strcmp(champ, "카타") == 0) {
		printf("카타리나의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Katarina/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "케넨") == 0) {
		printf("케넨의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Kennen/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "케이틀린") == 0 || strcmp(champ, "케틀") == 0) {
		printf("케이틀린의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Caitlyn/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "케일") == 0) {
		printf("케일의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Kayle/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "코그모") == 0) {
		printf("코그모의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/KogMaw/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "코르키") == 0) {
		printf("코르키 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Corki/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "퀸") == 0) {
		printf("퀸의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Quinn/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "타릭") == 0) {
		printf("타릭의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Taric/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "탈론") == 0) {
		printf("탈론의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Talon/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "트런들") == 0) {
		printf("트런들의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Trundle/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "트리스타나") == 0 || strcmp(champ, "트타") == 0) {
		printf("트리스타나의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Tristana/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "트린다미어") == 0 || strcmp(champ, "트린") == 0) {
		printf("트린다미어의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Tryndamere/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "트위스티드페이트") == 0 || strcmp(champ, "트페") == 0) {
		printf("트위스티드 페이트의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/TwistedFate/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "트위치") == 0) {
		printf("트위치의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Twitch/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "티모") == 0) {
		printf("티모의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Teemo/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "판테온") == 0) {
		printf("판테온의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Pantheon/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "피들스틱") == 0 || strcmp(champ, "피들") == 0) {
		printf("피들스틱의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Fiddlesticks/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "피오라") == 0) {
		printf("피오라의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Fiora/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "피즈") == 0) {
		printf("피즈의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Fizz/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "하이머딩거") == 0 || strcmp(champ, "딩거") == 0) {
		printf("하이머딩거의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Heimerdinger/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "헤카림") == 0) {
		printf("헤카림의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Hecarim/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "야스오") == 0) {
		printf("야스오의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Yasuo/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "벨코즈") == 0) {
		printf("벨코즈의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/VelKoz/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "브라움") == 0) {
		printf("브라움의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Braum/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "나르") == 0) {
		printf("나르의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Gnar/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "아지르") == 0) {
		printf("아지르의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Azir/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "칼리스타") == 0 || strcmp(champ, "칼리") == 0) {
		printf("칼리스타의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Kalista/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "렉사이") == 0) {
		printf("렉사이의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/RekSai/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "바드") == 0) {
		printf("바드의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Bard/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "에코") == 0) {
		printf("에코의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Ekko/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "탐켄치") == 0 || strcmp(champ, "켄치") == 0) {
		printf("탐 켄치의 공략 사이트로 이동합니다!\n");
		Sleep(3000);
		system("start https://www.op.gg/champion/TahmKench/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "킨드레드") == 0 || strcmp(champ, "킨드") == 0) {
		printf("킨드레드의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Kindred/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "일라오이") == 0 || strcmp(champ, "일라") == 0) {
		printf("일라오이의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Illaoi/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "진") == 0) {
		printf("진의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Jhin/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "아우렐리온솔") == 0 || strcmp(champ, "아솔") == 0) {
		printf("아우렐리온 솔의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/AurelionSol/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "탈리야") == 0) {
		printf("탈리야의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Taliyah/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "클레드") == 0) {
		printf("클레드의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Kled/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "아이번") == 0) {
		printf("아이번의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Ivern/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "카밀") == 0) {
		printf("카밀의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Camille/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "자야") == 0) {
		printf("자야의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Xayah/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "라칸") == 0) {
		printf("라칸의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Rakan/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "오른") == 0) {
		printf("오른의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Ornn/statistics/top");
		_getch();
	}
	else if (strcmp(champ, "조이") == 0) {
		printf("조이의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Zoe/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "케인") == 0) {
		printf("케인의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Kayn/statistics/jungle");
		_getch();
	}
	else if (strcmp(champ, "카이사") == 0) {
		printf("카이사의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/KaiSa/statistics/bot");
		_getch();
	}
	else if (strcmp(champ, "파이크") == 0) {
		printf("파이크의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Pyke/statistics/support");
		_getch();
	}
	else if (strcmp(champ, "니코") == 0) {
		printf("니코의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Neeko/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "사일러스") == 0) {
		printf("사일러스의 공략 사이트로 이동합니다!\n");
		Sleep(3000); 
		system("start https://www.op.gg/champion/Sylas/statistics/mid");
		_getch();
	}
	else if (strcmp(champ, "유미") == 0) {
		printf("유미의 공략 사이트로 이동합니다!\n");
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

// end - 3. 스토리 and 공략

// --------------------------------------------------------------------------

//4. 현재 메타 분석
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

// 메타 분석 디스플레이
void meta_display()
{
	int i;
	int x = 0, y = 0;
	system("cls");
	picture();

	textcolor(WHITE, WHITE);

	//T
	gotoxy(5, 1);
	printf("■■■■■");
	gotoxy(9, 2);
	printf("■");
	gotoxy(9, 3);
	printf("■");
	gotoxy(9, 4);
	printf("■");
	gotoxy(9, 5);
	printf("■");
	gotoxy(9, 6);
	printf("■");
	gotoxy(9, 7);
	printf("■");
	//I
	gotoxy(16, 1);
	printf("■■■■■");
	gotoxy(21, 2);
	printf("■");
	gotoxy(21, 3);
	printf("■");
	gotoxy(21, 4);
	printf("■");
	gotoxy(21, 5);
	printf("■");
	gotoxy(21, 6);
	printf("■");
	gotoxy(16, 7);
	printf("■■■■■");
	//E
	gotoxy(27, 1);
	printf("■■■■■");
	gotoxy(27, 2);
	printf("■");
	gotoxy(27, 3);
	printf("■");
	gotoxy(27, 4);
	printf("■■■■■");
	gotoxy(27, 5);
	printf("■");
	gotoxy(27, 6);
	printf("■");
	gotoxy(27, 7);
	printf("■■■■■");
	//R
	gotoxy(39, 1);
	printf("■■■■■");
	gotoxy(39, 2);
	printf("■");
	gotoxy(47, 2);
	printf("■");
	gotoxy(39, 3);
	printf("■");
	gotoxy(47, 3);
	printf("■");
	gotoxy(39, 4);
	printf("■■■■");
	gotoxy(39, 5);
	printf("■");
	gotoxy(47, 5);
	printf("■");
	gotoxy(39, 6);
	printf("■");
	gotoxy(47, 6);
	printf("■");
	gotoxy(39, 7);
	printf("■");
	gotoxy(47, 7);
	printf("■");

	//L
	gotoxy(54, 1);
	printf("■");
	gotoxy(54, 2);
	printf("■");
	gotoxy(54, 3);
	printf("■");
	gotoxy(54, 4);
	printf("■");
	gotoxy(54, 5);
	printf("■");
	gotoxy(54, 6);
	printf("■");
	gotoxy(54, 7);
	printf("■■■■■");
	//I
	gotoxy(65, 1);
	printf("■■■■■");
	gotoxy(69, 2);
	printf("■");
	gotoxy(69, 3);
	printf("■");
	gotoxy(69, 4);
	printf("■");
	gotoxy(69, 5);
	printf("■");
	gotoxy(69, 6);
	printf("■");
	gotoxy(65, 7);
	printf("■■■■■");
	//S
	gotoxy(76, 1);
	printf("■■■■■");
	gotoxy(76, 2);
	printf("■");
	gotoxy(76, 3);
	printf("■");
	gotoxy(76, 4);
	printf("■■■■■");
	gotoxy(84, 5);
	printf("■");
	gotoxy(84, 6);
	printf("■");
	gotoxy(76, 7);
	printf("■■■■■");
	//T
	gotoxy(87, 1);
	printf("■■■■■");
	gotoxy(91, 2);
	printf("■");
	gotoxy(91, 3);
	printf("■");
	gotoxy(91, 4);
	printf("■");
	gotoxy(91, 5);
	printf("■");
	gotoxy(91, 6);
	printf("■");
	gotoxy(91, 7);
	printf("■");

	textcolor(LIGHTGRAY, BLACK);
	gotoxy(65,9);
	printf("-현재 메타 분석-");

	textcolor(RED, RED);
	//1.
	gotoxy(27, 11);
	printf("■");
	gotoxy(25, 12);
	printf("■■");
	gotoxy(27, 13);
	printf("■");
	gotoxy(27, 14);
	printf("■");
	gotoxy(27, 15);
	printf("■");
	textcolor(DARKGRAY, BLACK);
	gotoxy(30, 15);
	printf("●");
	textcolor(RED, RED);
	//W
	y = 11;
	for (i = 0; i < 4; i++)
	{
		gotoxy(33, y);
		printf("■");
		gotoxy(37, y);
		printf("■");
		gotoxy(41, y);
		printf("■");
		y++;
	}
	gotoxy(35, 15);
	printf("■");
	gotoxy(39, 15);
	printf("■");
	//I
	gotoxy(45, 11);
	printf("■■■■■");
	gotoxy(49, 12);
	printf("■");
	gotoxy(49, 13);
	printf("■");
	gotoxy(49, 14);
	printf("■");
	gotoxy(45, 15);
	printf("■■■■■");
	//N
	y = 11;
	for (i = 0; i < 5; i++)
	{
		gotoxy(57, y);
		printf("■");
		gotoxy(65, y);
		printf("■");
		y++;
	}
	gotoxy(59, 12);
	printf("■");
	gotoxy(61, 13);
	printf("■");
	gotoxy(63, 14);
	printf("■");
	//2.
	textcolor(GREEN, GREEN);
	gotoxy(25, 17);
	printf("■■");
	gotoxy(27, 18);
	printf("■");
	gotoxy(25, 19);
	printf("■■");
	gotoxy(25, 20);
	printf("■");
	gotoxy(25, 21);
	printf("■■");
	textcolor(DARKGRAY, BLACK);
	gotoxy(30, 21);
	printf("●");
	textcolor(GREEN, GREEN);
	//P
	gotoxy(33, 17);
	printf("■■■■");
	gotoxy(33, 18);
	printf("■");
	gotoxy(39, 18);
	printf("■");
	gotoxy(33, 19);
	printf("■■■■");
	gotoxy(33, 20);
	printf("■");
	gotoxy(33, 21);
	printf("■");
	//I
	gotoxy(43, 17);
	printf("■■■");
	gotoxy(45, 18);
	printf("■");
	gotoxy(45, 19);
	printf("■");
	gotoxy(45, 20);
	printf("■");
	gotoxy(43, 21);
	printf("■■■");
	//C
	gotoxy(51, 17);
	printf("■■■");
	gotoxy(51, 18);
	printf("■");
	gotoxy(51, 19);
	printf("■");
	gotoxy(51, 20);
	printf("■");
	gotoxy(51, 21);
	printf("■■■");
	//K
	gotoxy(59, 17);
	printf("■");
	gotoxy(65, 17);
	printf("■");
	gotoxy(59, 18);
	printf("■");
	gotoxy(63, 18);
	printf("■");
	gotoxy(59, 19);
	printf("■■");
	gotoxy(59, 20);
	printf("■");
	gotoxy(63, 20);
	printf("■");
	gotoxy(59, 21);
	printf("■");
	gotoxy(65, 21);
	printf("■");
	//3.
	textcolor(BLUE, BLUE);
	gotoxy(25, 23);
	printf("■■");
	gotoxy(27, 24);
	printf("■");
	gotoxy(25, 25);
	printf("■■");
	gotoxy(27, 26);
	printf("■");
	gotoxy(25, 27);
	printf("■■");
	textcolor(DARKGRAY, BLACK);
	gotoxy(30, 27);
	printf("●");
	textcolor(BLUE, BLUE);
	//B
	gotoxy(33, 23);
	printf("■■■■");
	gotoxy(33, 24);
	printf("■");
	gotoxy(41, 24);
	printf("■");
	gotoxy(33, 25);
	printf("■■■■");
	gotoxy(33, 26);
	printf("■");
	gotoxy(41, 26);
	printf("■");
	gotoxy(33, 27);
	printf("■■■■");
	//A
	gotoxy(47, 23);
	printf("■■■");
	y = 24;
	for (i = 0; i <4 ; i++)
	{
		gotoxy(45, y);
		printf("■");
		gotoxy(53, y);
		printf("■");
		y++;
	}
	gotoxy(45, 26);
	printf("■■■■■");

	//N
	y = 23;
	for (i = 0; i < 5; i++)
	{
		gotoxy(57, y);
		printf("■");
		gotoxy(65, y);
		printf("■");
		y++;
	}
	gotoxy(59, 24);
	printf("■");
	gotoxy(61, 25);
	printf("■");
	gotoxy(63, 26);
	printf("■");
	textcolor(LIGHTGRAY, BLACK);
	//----------------------------------
	gotoxy(44, 29);
	printf("0. Return");
}

// 1) 승률 선택지
void win_rate(Tree * LOL)
{
	system("cls");
	picture();
	int key;
	int i = 0;
	int x, y = 0;
	//--------------------------------
	gotoxy(30, 3);
	printf("┏");
	for (i = 0; i < 30; i++)
		printf("━");
	printf("┓");
	y = 3;
	for (i = 0; i < 3; i++)
	{
		y++;
		gotoxy(30, y);
		printf("┃");
		gotoxy(61, y);
		printf("┃");
	}
	gotoxy(30, 7);
	printf("┗");
	for (i = 0; i < 30; i++)
		printf("━");
	printf("┛");
	gotoxy(35, 5);
	printf("1. 전체 챔피언 승률");
	//-----------------------------------
	gotoxy(30, 9);
	printf("┏");
	for (i = 0; i < 30; i++)
		printf("━");
	printf("┓");
	y = 9;
	for (i = 0; i < 3; i++)
	{
		y++;
		gotoxy(30, y);
		printf("┃");
		gotoxy(61, y);
		printf("┃");
	}
	gotoxy(30,13);
	printf("┗");
	for (i = 0; i < 30; i++)
		printf("━");
	printf("┛");
	gotoxy(35,11);

	printf("2. 라인별 챔피언 승률");
	//-----------------------------------------------
	gotoxy(30, 17);
	printf("┏");
	for (i = 0; i < 30; i++)
		printf("━");
	printf("┓");
	y = 17;
	for (i = 0; i < 3; i++)
	{
		y++;
		gotoxy(30, y);
		printf("┃");
		gotoxy(61, y);
		printf("┃");
	}
	gotoxy(30, 21);
	printf("┗");
	for (i = 0; i < 30; i++)
		printf("━");
	printf("┛");
	gotoxy(35, 19);

	printf("0. 뒤로 가기");

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

// 1-1) 전체 승률 출력
void all_win_rate()
{
	system("cls");
	picture();
	int i = 0;
	int x, y = 0;

	gotoxy(34, 4);
	printf("┏");
	for (i = 0; i < 31; i++)
		printf("━");
	printf("┓");
	
	y = 4;
	for (i = 0; i < 15; i++)
	{
		y++;
		gotoxy(34, y);
		printf("┃");
		gotoxy(66, y);
		printf("┃");
	}

	gotoxy(34, 19);
	printf("┗");
	for (i = 0; i < 31; i++)
		printf("━");
	printf("┛");

	gotoxy(34, 6);
	printf("┣");
	for (i = 0; i < 31; i++)
		printf("━");
	printf("┫");

	gotoxy(42, 6);
	printf("┳");
	gotoxy(54, 6);
	printf("┳");

	gotoxy(34, 8);
	printf("┣");
	for (i = 0; i < 31; i++)
		printf("━");
	printf("┫");

	y = 6;
	for (i = 0; i < 12; i++)
	{
		y++;
		gotoxy(42, y);
		printf("┃");
		gotoxy(54, y);
		printf("┃");
	}
	gotoxy(42, 8);
	printf("╋");
	gotoxy(54, 8);
	printf("╋");
	
	char champ_name[10][MAX_NAME] = { "요릭","람머스","시비르","말자하","질리언","징크스","소나","클레드","아무무","타릭" };
	double champ_win_rate[10] = { 54.15,54.01,53.42,53.40,53.31,53.29,53.22,53.12,42.98,52.82 };
	gotoxy(38, 5);
	printf("챔피언 승률 순위(1~10위)");
	gotoxy(37,7);
	printf("순위");
	gotoxy(46,7);
	printf("챔피언");
	gotoxy(59,7);
	printf("승률");
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
	printf("┻");
	gotoxy(54, 19);
	printf("┻");
	gotoxy(36, 20);
	if (_kbhit() != 0) system("pause");
	_getch();
}

// 1-2) 라인별 승률 순위
void line_win_rate(Tree * LOL)
{
	Node * play_node;
	system("cls");
	int key;
	picture();

	int i = 0;
	int x, y = 0;
	//탑
	gotoxy(39, 4);
	printf("┏");
	for (i = 0; i < 20; i++)
		printf("━");
	printf("┓");

	y = 4;
	for (i = 0; i < 3; i++)
	{
		y++;
		gotoxy(39, y);
		printf("┃");
		gotoxy(60, y);
		printf("┃");
	}

	gotoxy(39, 8);
	printf("┗");
	for (i = 0; i < 20; i++)
		printf("━");
	printf("┛");
	gotoxy(42, 6);
	printf("1.  탑  승률 보기");
	//정글
	gotoxy(28, 9);
	printf("┏");
	for (i = 0; i < 20; i++)
		printf("━");
	printf("┓");

	y = 9;
	for (i = 0; i < 3; i++)
	{
		y++;
		gotoxy(28, y);
		printf("┃");
		gotoxy(49, y);
		printf("┃");
	}

	gotoxy(28, 13);
	printf("┗");
	for (i = 0; i < 20; i++)
		printf("━");
	printf("┛");
	gotoxy(31, 11);
	printf("2. 정글 승률 보기");
	//미드
	gotoxy(50, 9);
	printf("┏");
	for (i = 0; i < 20; i++)
		printf("━");
	printf("┓");

	y = 9;
	for (i = 0; i < 3; i++)
	{
		y++;
		gotoxy(50, y);
		printf("┃");
		gotoxy(71, y);
		printf("┃");
	}

	gotoxy(50, 13);
	printf("┗");
	for (i = 0; i < 20; i++)
		printf("━");
	printf("┛");
	gotoxy(53, 11);
	printf("3. 미드 승률 보기");
	//원딜
	gotoxy(28, 14);
	printf("┏");
	for (i = 0; i < 20; i++)
		printf("━");
	printf("┓");

	y = 14;
	for (i = 0; i < 3; i++)
	{
		y++;
		gotoxy(28, y);
		printf("┃");
		gotoxy(49, y);
		printf("┃");
	}

	gotoxy(28, 18);
	printf("┗");
	for (i = 0; i < 20; i++)
		printf("━");
	printf("┛");
	gotoxy(31, 16);
	printf("4. 원딜 승률 보기");
	//서폿
	gotoxy(50, 14);
	printf("┏");
	for (i = 0; i < 20; i++)
		printf("━");
	printf("┓");

	y = 14;
	for (i = 0; i < 3; i++)
	{
		y++;
		gotoxy(50, y);
		printf("┃");
		gotoxy(71, y);
		printf("┃");
	}

	gotoxy(50, 18);
	printf("┗");
	for (i = 0; i < 20; i++)
		printf("━");
	printf("┛");
	gotoxy(53, 16);
	printf("5. 서폿 승률 보기");
	//뒤로가기
	gotoxy(39, 26);
	printf("┏");
	for (i = 0; i < 20; i++)
		printf("━");
	printf("┓");

	y = 26;
	for (i = 0; i < 1; i++)
	{
		y++;
		gotoxy(39, y);
		printf("┃");
		gotoxy(60, y);
		printf("┃");
	}

	gotoxy(39, 28);
	printf("┗");
	for (i = 0; i < 20; i++)
		printf("━");
	printf("┛");
	gotoxy(45, 27);
	printf("0. 뒤로 가기");

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

// 1-2) 라인별 승률 출력
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
	printf("┏");
	for (i = 0; i < 37; i++)
		printf("━");
	printf("┓");

	y = 4;
	for (i = 0; i < 15; i++)
	{
		y++;
		gotoxy(31, y);
		printf("┃");
		gotoxy(69, y);
		printf("┃");
	}

	gotoxy(31, 19);
	printf("┗");
	for (i = 0; i < 37; i++)
		printf("━");
	printf("┛");

	gotoxy(31, 6);
	printf("┣");
	for (i = 0; i < 37; i++)
		printf("━");
	printf("┫");

	gotoxy(42, 6);
	printf("┳");
	gotoxy(58, 6);
	printf("┳");

	gotoxy(31, 8);
	printf("┣");
	for (i = 0; i < 37; i++)
		printf("━");
	printf("┫");

	y = 6;
	for (i = 0; i < 12; i++)
	{
		y++;
		gotoxy(42, y);
		printf("┃");
		gotoxy(58, y);
		printf("┃");
	}
	gotoxy(42, 8);
	printf("╋");
	gotoxy(58, 8);
	printf("╋");

	gotoxy(35, 5);
	printf("%s 챔피언 승률 순위(1~10위)", pnode->name);
	gotoxy(36, 7);
	printf("순위");
	gotoxy(48, 7);
	printf("챔피언");
	gotoxy(62, 7);
	printf("승률");
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
	printf("┻");
	gotoxy(58, 19);
	printf("┻");
	gotoxy(36, 20);
	if (_kbhit() != 0) system("pause");
	_getch();
}

// 2) 픽률 출력
void pick_rate()
{
	system("cls");
	picture();
	char champ_name[10][MAX_NAME] = { "이즈리얼","카이사","리신","베인","쓰레쉬","유미","럭스","노틸러스","자르반","야스오" };
	double champ_pick_rate[10] = { 48.60,32.08,24.77,22.46,20.48,18.46,18.04,17.19,16.39,16.36 };
	//------------------------------------------------------------
	int i = 0;
	int x, y = 0;

	gotoxy(31, 4);
	printf("┏");
	for (i = 0; i < 37; i++)
		printf("━");
	printf("┓");

	y = 4;
	for (i = 0; i < 15; i++)
	{
		y++;
		gotoxy(31, y);
		printf("┃");
		gotoxy(69, y);
		printf("┃");
	}

	gotoxy(31, 19);
	printf("┗");
	for (i = 0; i < 37; i++)
		printf("━");
	printf("┛");

	gotoxy(31, 6);
	printf("┣");
	for (i = 0; i < 37; i++)
		printf("━");
	printf("┫");

	gotoxy(42, 6);
	printf("┳");
	gotoxy(58, 6);
	printf("┳");

	gotoxy(31, 8);
	printf("┣");
	for (i = 0; i < 37; i++)
		printf("━");
	printf("┫");

	y = 6;
	for (i = 0; i < 12; i++)
	{
		y++;
		gotoxy(42, y);
		printf("┃");
		gotoxy(58, y);
		printf("┃");
	}
	gotoxy(39, 5);
	printf("챔피언 픽률 순위(1~10위)");
	gotoxy(36, 7);
	printf("순위");
	gotoxy(48, 7);
	printf("챔피언");
	gotoxy(62, 7);
	printf("픽률");
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
	printf("┻");
	gotoxy(58, 19);
	printf("┻");
	gotoxy(36, 20);
	if (_kbhit() != 0) system("pause");
	_getch();
}

// 3) 밴률 출력
void ban_rate()
{
	system("cls");
	picture();
	char champ_name[10][MAX_NAME] = { "야스오","모르가나","베인","제드","르블랑","리신","다리우스","노틸러스","파이크","아트록스" };
	double champ_ban_rate[10] = { 60.73,52.37,50.68,43.64,38.07,36.02,33.67,33.22,29.54,26.63 };
	//------------------------------------------------

	int i = 0;
	int x, y = 0;

	gotoxy(31, 4);
	printf("┏");
	for (i = 0; i < 37; i++)
		printf("━");
	printf("┓");

	y = 4;
	for (i = 0; i < 15; i++)
	{
		y++;
		gotoxy(31, y);
		printf("┃");
		gotoxy(69, y);
		printf("┃");
	}

	gotoxy(31, 19);
	printf("┗");
	for (i = 0; i < 37; i++)
		printf("━");
	printf("┛");

	gotoxy(31, 6);
	printf("┣");
	for (i = 0; i < 37; i++)
		printf("━");
	printf("┫");

	gotoxy(42, 6);
	printf("┳");
	gotoxy(58, 6);
	printf("┳");

	gotoxy(31, 8);
	printf("┣");
	for (i = 0; i < 37; i++)
		printf("━");
	printf("┫");

	y = 6;
	for (i = 0; i < 12; i++)
	{
		y++;
		gotoxy(42, y);
		printf("┃");
		gotoxy(58, y);
		printf("┃");
	}
	gotoxy(42, 8);
	printf("╋");
	gotoxy(58, 8);
	printf("╋");

	gotoxy(39, 5);
	printf("챔피언 밴률 순위(1~10위)");
	gotoxy(36, 7);
	printf("순위");
	gotoxy(48, 7);
	printf("챔피언");
	gotoxy(62, 7);
	printf("밴률");
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
	printf("┻");
	gotoxy(58, 19);
	printf("┻");
	gotoxy(36, 20);
	if (_kbhit() != 0) system("pause");
	_getch();
}

// end - 4. 메타분석

// --------------------------------------------------------------------------

// 5. 1대1 랜덤 게임 ( 배열과 텍스트 파일 사용)

// 모든 챔피언 배열 생성
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
		printf("ERROR @@ 텍스트 파일이 존재하지 않습니다\n");
		return 0;
	}

}

//랜덤게임 시작
//랜덤게임 시작
//랜덤게임 시작
void Play_Game(Tree* LOL) {
	{
		// 테스트용으로 입력을 받을 버퍼
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
		
		//모든 챔피언이 담긴 배열
		char allChampion[144][20] = { 0, };
		make_champion_array(allChampion);

		hWnd = GetConsoleWindow();
		hInst = GetModuleHandle(NULL);
		while (1) 
		{
			system("cls");
			gotoxy(0, 0);
			printf("┏");
			for (i = 0; i < 99; i++)
				printf("━");
			printf("┓");
			y = 0;
			for (i = 0; i < 10; i++)
			{
				y++;
				gotoxy(0, y);
				printf("┃");
				gotoxy(100, y);
				printf("┃");
			}
			gotoxy(0, 10);
			printf("┗");
			for (i = 0; i < 99; i++)
				printf("━");
			printf("┛");
			//1
			textcolor(LIGHTGRAY, LIGHTGRAY);
			y = 3;
			for (z = 0; z < 5; z++)
			{
				gotoxy(34, y);
				printf("■");
				gotoxy(66, y);
				printf("■");
				y++;
			}
			//v
			y = 3;
			for (z = 0; z < 2; z++)
			{	gotoxy(40, y);
				printf("■");
				gotoxy(48, y);
				printf("■");
				y++;
			}
			y = 5;
			for (z = 0; z < 2; z++)
			{
				gotoxy(42, y);
				printf("■");
				gotoxy(46, y);
				printf("■");
				y++;
			}
			gotoxy(44, 7);
			printf("■");
			//s
			gotoxy(52, 3);
			printf("■■■■■");
			gotoxy(52, 4);
			printf("■");
			gotoxy(52, 5);
			printf("■■■■■");
			gotoxy(60, 6);
			printf("■");
			gotoxy(52, 7);
			printf("■■■■■");
			textcolor(LIGHTGRAY, BLACK);

			//1P
			
			
			/*gotoxy(30, 10);
			printf("┏");
			for (i = 0; i < 14; i++)
				printf("━");
			printf("┓");
			y = 10;
			for (i = 0; i < 10; i++)
			{
				y++;
				gotoxy(30, y);
				printf("┃");
				gotoxy(45, y);
				printf("┃");
			}
			gotoxy(30,21);
			printf("┗");
			for (i = 0; i < 14; i++)
				printf("━");
			printf("┛");*/
			if (random1 != 0)
			{
				Rendering(200, 200, random1);
			}//2P
			if (random2 != 0) 
			{
				Rendering(600, 200, random2);
			}

			// 루프를돈다.
			gotoxy(35, 21);
			printf("┏");
			for (i = 0; i < 29; i++)
				printf("━");
			printf("┓");
			y = 21;
			for (i = 0; i < 5; i++)
			{
				y++;
				gotoxy(35, y);
				printf("┃");
				gotoxy(65, y);
				printf("┃");
			}
			gotoxy(35, 27);
			printf("┗");
			for (i = 0; i < 29; i++)
				printf("━");
			printf("┛");
			gotoxy(38, 22);
			printf("S : Player 1의 챔피언 뽑기");
			gotoxy(38, 23);
			printf("D : Player 2의 챔피언 뽑기");
			gotoxy(38, 24);
			printf("G : 1대1 대결 시작!");
			gotoxy(38, 26);
			printf("X:초기 화면으로 가기");
			key = _getch();
			if (key == 's') 
			{
				system("cls");
				
				gotoxy(40, 8);
				printf("┏");
				for (i = 0; i < 32; i++)
					printf("━");
				printf("┓");
				gotoxy(40, 9);
				printf("┃");
				gotoxy(73, 9);
				printf("┃");
				gotoxy(40, 10);
				printf("┗");
				for (i = 0; i < 32; i++)
					printf("━");
				printf("┛");
				gotoxy(42, 9);
				printf("챔피언을 무작위로 뽑는중입니다.");
				//printf("\t\t\t아무키나 누르면 뽑힙니다.\n");
				delay(3000);
				system("cls");
				random1 = (rand() % 144 + 101);
				champ_node1 = find_node(LOL->root, allChampion[random1 - 101]);
				jud1++;//player1의 뽑기 여부

				Rendering(440, 200, random1);
				Sleep(500);
				gotoxy(45,24);
				printf("Player 1의 챔피언 : %s", champ_node1->name);
				gotoxy(45, 25);
				printf("승률 = %.2lf%%\n",champ_node1->win_rate);
				
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
				printf("┏");
				for (i = 0; i < 32; i++)
					printf("━");
				printf("┓");
				gotoxy(40, 9);
				printf("┃");
				gotoxy(73, 9);
				printf("┃");
				gotoxy(40, 10);
				printf("┗");
				for (i = 0; i < 32; i++)
					printf("━");
				printf("┛");
				gotoxy(42, 9);
				printf("챔피언을 무작위로 뽑는중입니다.");
				//printf("\t\t\t아무키나 누르면 뽑힙니다.\n");
				delay(3000);
				system("cls");

				random2 = (rand() % 144 + 101);
				champ_node2 = find_node(LOL->root, allChampion[random2 - 101]);
				jud2++;//player1의 뽑기 여부
				Rendering(440, 200, random2);
				Sleep(500);
				gotoxy(45, 24);
				printf("Player 2의 챔피언 : %s", champ_node2->name);
				gotoxy(45, 25);
				printf("승률 = %.2lf%%\n", champ_node2->win_rate);
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
				printf("┏");
				for (i = 0; i < 36; i++)
					printf("━");
				printf("┓");
				gotoxy(39, 9);
				printf("┃");
				gotoxy(76, 9);
				printf("┃");
				gotoxy(39, 10);
				printf("┗");
				for (i = 0; i < 36; i++)
					printf("━");
				printf("┛");
				gotoxy(41, 9);

				if (jud1 == -1 && jud2 != -1)
					printf("Player 1의 챔피언을 뽑아주세요!!\n");
				else if (jud2 == -1 && jud1 != -1)
					printf("Player 2의 챔피언을 뽑아주세요!!\n");
				else if (jud1 == -1 && jud2 == -1)
					printf("Player 1,2 의 챔피언을 뽑아주세요!!\n");
				else if (jud1 != -1 && jud2 != -1) 
				{
					system("cls");
					Sleep(2000);
					Rendering(300, 200, random1); // Player 1 사진
					Sleep(1000);
					gotoxy(35, 22);
					printf("%s", champ_node1->name);
					gotoxy(35, 23);
					printf("%.2lf%%", champ_node1->win_rate); //player 1 승률
					Sleep(1000);
					gotoxy(55, 22);
					printf("VS");
					Sleep(1000);
					Rendering(600, 200, random2); // Player 2 사진
					Sleep(1000);
					gotoxy(70, 22);
					printf("%s", champ_node2->name);	
					gotoxy(70, 23);
					printf("%.2lf%%", champ_node2->win_rate); //player 2 승률
					delay(1000);

					if (champ_node1->win_rate > champ_node2->win_rate) {
						system("cls");
						Sleep(2000);
						Rendering(450, 200, random1);
						Sleep(100);
						gotoxy(45, 21);
						printf("Player 1의 챔피언 %s", champ_node1->name);
						gotoxy(49, 22);
						printf("승리");
						
			
						Sleep(5000);
					}
					else if (champ_node1->win_rate == champ_node2->win_rate) {
						system("cls");
						Sleep(2000);
						gotoxy(47,15);
						printf("무승부!\n");
						Sleep(5000);
					}
					else {
						system("cls");
						Sleep(2000);
						Rendering(450, 200, random2);
						Sleep(100);
						gotoxy(45, 21);
						printf("Player 2의 챔피언 %s", champ_node2->name);
						gotoxy(49, 22);
						printf("승리");
						Sleep(5000);
					}
				
				}
				Sleep(2000);
			}
			else if (key == 'x') {
				system("cls");
				gotoxy(39, 8);
				printf("┏");
				for (i = 0; i < 36; i++)
					printf("━");
				printf("┓");
				gotoxy(39, 9);
				printf("┃");
				gotoxy(76, 9);
				printf("┃");
				gotoxy(39, 10);
				printf("┗");
				for (i = 0; i < 36; i++)
					printf("━");
				printf("┛");
				gotoxy(40, 9);
				printf("초기 화면으로 돌아갑니다.\n");
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

// end - 5. 1대1 랜덤픽 싸움

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

	printf("┏");
	for (i = 0; i < 99; i++)
		printf("━");
	printf("┓");

	for (i = 0; i < 32; i++)
	{
		y++;
		gotoxy(0, y);
		printf("┃");

	}

	gotoxy(0, 32);
	for (i = 0; i <= 99; i++)
		printf("━");

	x = 100;
	y = 0;
	for (i = 0; i < 32; i++)
	{
		y++;
		gotoxy(x, y);
		printf("┃");
	}
	y++;

	gotoxy(0, 32);
	printf("┗");
	gotoxy(100, 32);
	printf("┛");
}

// 컬러
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
	//사운드 파일 위치, SND_ASYNC, SND_LOOP 세가지가 제일 중요
	//SND_ASYNC : 재생하면서 다음코드 실행
	//SND_LOOP : 반복재생

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
	// 콘솔 윈도우 창의 핸들값을 얻어온다.
	hWnd = GetConsoleWindow();
	// 프로그램의 인스턴스 핸들값을 얻어온다.
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
