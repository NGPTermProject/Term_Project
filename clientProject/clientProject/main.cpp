#pragma warning(disable: 4996)

#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <random>
#include <time.h>
#include <atlimage.h>
#include <string.h>
#include <vector>
#include <list>

#include "Image.h"
#include "State.h"
#include "Define.h"
#include "Object.h"
#include "Network.h"
#include "protocol.h"

using namespace std;


HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Term Project";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
DWORD WINAPI Recv_Thread(LPVOID arg);


//#define NETWORK  //네트워크 기능 켜기

// 시간을 구하기 위한 변수들
LARGE_INTEGER g_tSecond;
LARGE_INTEGER g_tTime;
float         g_fDeltaTime;

int stage = 0;  //0이면 로그인 화면
int MyId;

//////////////////////////
// 로그인 화면에서 키보드 입력하면 id창에 입력
// id창에 글자가 있을때 엔터 누르면(서버에 송신) 게임 시작
//////////////////////////
vector<Player> p;

bool InitClient();
void InitGameObject();
cs_send_player_id id;
cs_send_player p_info[2];
cs_send_keyinfo keyinfo;
sc_put_object put;
vector<Map> m_map;
vector<Obstacle> m_obstacle;
Map m_button[2];
vector<Map> m_static_map;

cs_obstacle cs_obs[2];

vector<Monster> m_monster;
Bullet m_bullet[20];
cs_bullet bullet[20];

int map_current_count = 0;
int map_max_count = 5;

int FirstMapSize = 7;
int SecondMapSize = 15;
int FirstMonsterSize = 4;
int SecondMonsterSize = 9;
int FirstObstacleSize = 2;
int SecondObstacleSize = FirstObstacleSize + 5;

bool isConnect = false;

int RedAnim = 0;
int BlueAnim = 0;

cs_login_button login_button;
cs_login_info login_info;
cs_start_game start_game_info;
bool EnterOtherPlayer;

bool GameStart;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
#ifdef NETWORK
	if (!InitClient())
		return 1;
#endif
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground =
		(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);
	hWnd = CreateWindow
	(lpszClass, lpszWindowName,
		WS_OVERLAPPEDWINDOW,
		0, 0, Window_Size_X, Window_Size_Y,
		NULL, (HMENU)NULL,
		hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	CreateThread(NULL, 0, Recv_Thread, NULL, 0, NULL);
	UpdateWindow(hWnd);


	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}


DWORD WINAPI Recv_Thread(LPVOID arg)
{
	// send, recv 함수 출력값 저장용
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	MyId = -1;

	while (1) {
		//PlayerID 전달.
		recvn(sock, (char*)&id, sizeof(cs_send_player_id), 0);
		if (id.id == 1) {
			EnterOtherPlayer = true;
			break;
		}
		MyId = id.id;
		keyinfo.id = id.id;
		login_button.id = MyId;
	}


	if (MyId == 0) {
		MyId = 0;
		keyinfo.id = 0;
		login_button.id = 0;
	}
	if (MyId == -1) {
		MyId = 1;
		keyinfo.id = 1;
		login_button.id = 1;
	}
	cout << MyId << endl;
	
	recvn(sock, (char*)&start_game_info, sizeof(cs_start_game), 0);
	cout << "게임시작" << endl;
	if (start_game_info.gamestart) {
		cout << "게임시작" << endl;
		stage = start_game_info.stage;
		isConnect = true;
	}
	else {
		cout << "오류" << endl;
	}

	
	//------------------------------//


	int StartTime;
	while (1) {
		StartTime = GetTickCount64();
		while (GetTickCount64() - StartTime <= 10) { }
		{	
			send(sock, (char*)&keyinfo, sizeof(keyinfo), 0);
			keyinfo.isClick = false;
			keyinfo.jump = false;
			recvn(sock, (char*)&p_info, sizeof(p_info), 0);

			for (int i = 0; i < 2; ++i) {
				p[i].getPlayerInfo(p_info[p_info[i].id]);
			}

			recvn(sock, (char*)&cs_obs, sizeof(cs_obs), 0);
			{
		
				for (int i = 0; i < 2; ++i) {
					if (stage == 1) {
						m_obstacle[i].x = cs_obs[i].x;
						m_obstacle[i].y = cs_obs[i].y;
					}
					if (stage == 2) {
						m_obstacle[FirstObstacleSize+ i].x = cs_obs[i].x;
						m_obstacle[FirstObstacleSize + i].y = cs_obs[i].y;
					}
					if (stage == 3) {
						m_obstacle[SecondObstacleSize + i].x = cs_obs[i].x;
						m_obstacle[SecondObstacleSize + i].y = cs_obs[i].y;
					}
				}
			}

			recvn(sock, (char*)&put, sizeof(put), 0);
			if (put.isClick) {
				m_map.push_back(Map(MAP::PLAT, put.x, put.y));
			}

			if (stage == 1) {
				for (int i = 0; i < 2; ++i)
					m_static_map[i].setState(put.isPush[i]);
			}
			if (stage == 2) {
				for (int i = FirstMapSize; i < FirstMapSize+2; ++i)
					m_static_map[i].setState(put.isPush[i%2]);
			}
			if (stage == 3) {
				for (int i = SecondMapSize; i < SecondMapSize+2; ++i)
					m_static_map[i].setState(put.isPush[i%2]);
			}
			
			if (put.AttackMonsterId != -1) {
				m_monster[put.AttackMonsterId].Attack();

			}
			if (put.clear) {
				if (m_map.size() != 0) {
					m_map.clear();
					map_current_count = 0;
				}
			}
			stage = put.Stage;
			if (stage == 0)stage = 1;

			recvn(sock, (char*)&bullet, sizeof(bullet), 0);

			for (int i = 0; i < 20; ++i) {
				m_bullet[i].getBulletInfo(bullet[i]);
			}


		}



	}
	closesocket(sock);
	exit(1);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	static HDC memdc1;
	static HDC hdc, memdc2;
	HBITMAP hBitmap1, hBitmap2;
	HBRUSH hBrush, oldBrush;


	
	static int block_count;
	
	static HBITMAP buttonBitmap;
	static HWND Button1;


	static bool isJumping = false ;

	// 지역변수는 메시지가 발생할 때마다 초기화되므로 값을 계속 유지하기 위해서 static 사용
	static TCHAR str[512];

	switch (uMsg) {

	case WM_CREATE:



		M_LoadImage();

		p.push_back(Player(200 , 600, 0));
		p.push_back(Player(400 , 600, 1));


		InitGameObject();

		SetTimer(hWnd, 1, 10, NULL);
			buttonBitmap = (HBITMAP)LoadImage(NULL, TEXT("res/frogButtonStrat.bmp"), IMAGE_BITMAP,
				0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
			Button1 = CreateWindow(L"button", L"PressToPlay",
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
				620, 350, 200, 43, hWnd, (HMENU)1, g_hInst, NULL);
			SendMessage(Button1, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)buttonBitmap);
		
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 1:
			id.id = 3;
			send(sock, (char*)&id, sizeof(cs_send_player_id), 0);
			DestroyWindow(Button1);
	

			break;
		}
		break;

	case WM_TIMER:
		switch (wParam) {

		case 1:
			if (MyId == 1) {
				if(Button1)DestroyWindow(Button1);
			}

			static bool push = true;

			if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
				if (push) {
					if (p[MyId].getJumpCount() < 2) {
						keyinfo.jump = true;
					}
					push = false;
				}
			}
			else {
				push = true;
			}
	

			for (int i = 0; i < 2; ++i)
				p[i].animation();

			if (stage == 0) {
				RedAnim += 64;
				if (RedAnim >= 768)
					RedAnim = 0;

				if (EnterOtherPlayer) {
					BlueAnim += 64;
					if (BlueAnim >= 768)
						BlueAnim = 0;
				}
			}

			if (stage == 1) {
				for (int i = 0; i < FirstMonsterSize; ++i) {
					m_monster[i].animation();
				}
				for (int i = 0; i < FirstObstacleSize; ++i) {
					m_obstacle[i].animation();
				}
			}
			else if (stage == 2) {
				for (int i = FirstMonsterSize; i < SecondMonsterSize; ++i) {
					m_monster[i].animation();
				}
				for (int i = FirstObstacleSize; i < SecondObstacleSize; ++i) {
					m_obstacle[i].animation();
				}
			}
			else if (stage == 3) {
				for (int i = SecondMonsterSize; i < m_monster.size(); ++i) {
					m_monster[i].animation();
				}
				for (int i = SecondObstacleSize; i < m_obstacle.size(); ++i) {
					m_obstacle[i].animation();
				}
			}


			break;
		}


		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_KEYDOWN:
		if (stage == 0) {

		}
		else {
			switch (wParam)
			{

			case VK_LEFT:
				keyinfo.left = true;
				break;
			case VK_RIGHT:
				keyinfo.right = true;
				break;
			}
			break;
		}
		break;
	case WM_KEYUP:
		if (wParam == VK_LEFT) {
			keyinfo.left = false;

		}
		if (wParam == VK_RIGHT) {
			keyinfo.right = false;

		}
		if (wParam == VK_SPACE) {
			isJumping  = false;
		}
		InvalidateRect(hWnd, NULL, false);

		break;
	
	case WM_LBUTTONDOWN:
		if (map_current_count != map_max_count) {
			keyinfo.isClick = true;
			keyinfo.x = LOWORD(lParam);
			keyinfo.y = HIWORD(lParam);
			map_current_count++;
	
		}
		break;

	case WM_PAINT:

		hdc = BeginPaint(hWnd, &ps);

		memdc1 = CreateCompatibleDC(hdc);
		hBitmap1 = CreateCompatibleBitmap(hdc, 11500, Window_Size_Y);

		SelectObject(memdc1, hBitmap1);

		// 1. 배경 그리기
		img_bg.Draw(memdc1, 0, 0, Window_Size_X, Window_Size_Y, 0, 0, img_bg_width, img_bg_height);

		if (stage == 0) {
			img_LoginBG.Draw(memdc1, 0, 0, Window_Size_X, Window_Size_Y, 0, 0, img_Loginbg_width, img_Loginbg_height);
		
			img_LodingFrogRed.Draw(memdc1, 460, 570, 64, 64, RedAnim, 0, 64, 64);
			img_LodingFrogBlue.Draw(memdc1, 960, 565, 64, 64, BlueAnim, 64, 64, 64);

		}
		else if (stage !=4){

			//다른 오브젝트 발판 그리기

			
			for (int i = 0; i < m_map.size(); ++i)
				m_map[i].draw(memdc1);

			if (stage == 1) {
				for (int i = 0; i < FirstMapSize; ++i)
					m_static_map[i].draw(memdc1);
				for (int i = 0; i < FirstMonsterSize; ++i) {
					m_monster[i].draw(memdc1);
					if (m_bullet[i].isStart)
						m_bullet[i].draw(memdc1);
				}
				for (int i = 0; i < FirstObstacleSize; ++i) {
					m_obstacle[i].draw(memdc1);
				}
			}
			else if (stage == 2) {
				for (int i = FirstMapSize; i < SecondMapSize; ++i)
					m_static_map[i].draw(memdc1);
				for (int i = FirstMonsterSize; i < SecondMonsterSize; ++i) {
					m_monster[i].draw(memdc1);
					if (m_bullet[i].isStart)
						m_bullet[i].draw(memdc1);
				}
				for (int i = FirstObstacleSize; i < SecondObstacleSize; ++i) {
					m_obstacle[i].draw(memdc1);
				}
			}
			else if (stage == 3) {
				for (int i = SecondMapSize; i < m_static_map.size(); ++i)
					m_static_map[i].draw(memdc1);
				for (int i = SecondMonsterSize; i < m_monster.size(); ++i) {
					m_monster[i].draw(memdc1);
					if (m_bullet[i].isStart)
						m_bullet[i].draw(memdc1);
				}
				for (int i = SecondObstacleSize; i < m_obstacle.size(); ++i) {
					m_obstacle[i].draw(memdc1);
				}
			}


			//플레이어 그리기
			for (int i = 0; i < p.size(); ++i) {
				p[i].draw(memdc1);
			}


	

		}
		else {
			img_Ending.Draw(memdc1, 0, 0, Window_Size_X, Window_Size_Y, 0, 0, img_bg_width, img_bg_height);
		}

		BitBlt(hdc, 0, 0, Window_Size_X, Window_Size_Y, memdc1, 0, 0, SRCCOPY);

		DeleteObject(SelectObject(memdc1, hBitmap1));
		DeleteDC(memdc1);
		EndPaint(hWnd, &ps);

		break;

	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}



bool InitClient() {
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;

	//socket()
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)  err_quit("socket()");

	//connect()
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) {
		err_quit("connet()");
	}
	return true;
}


void InitGameObject()
{

	m_static_map.push_back(Map(MAP::BUTTON, 48, 344));
	m_static_map.push_back(Map(MAP::BUTTON, 1392, 344));
	m_static_map.push_back(Map(MAP::PLAT, 48, 374));
	m_static_map.push_back(Map(MAP::PLAT, 1392, 374));
	m_static_map.push_back(Map(MAP::PLAT, 48, 566));
	m_static_map.push_back(Map(MAP::PLAT, 432, 214));
	m_static_map.push_back(Map(MAP::PLAT, 1008, 214));
	//first

	m_static_map.push_back(Map(MAP::BUTTON, 48, 312));
	m_static_map.push_back(Map(MAP::BUTTON, 432, 312));
	m_static_map.push_back(Map(MAP::PLAT, 500, 550));
	m_static_map.push_back(Map(MAP::PLAT, 288, 150));
	m_static_map.push_back(Map(MAP::PLAT, 432, 342));
	m_static_map.push_back(Map(MAP::PLAT, 1392, 510));
	m_static_map.push_back(Map(MAP::PLAT, 1392, 210));
	m_static_map.push_back(Map(MAP::PLAT, 250, 342));
	//second

	m_static_map.push_back(Map(MAP::BUTTON, 1392, 24));
	m_static_map.push_back(Map(MAP::BUTTON, 1296, 664));
	m_static_map.push_back(Map(MAP::PLAT, 480, 374));
	m_static_map.push_back(Map(MAP::PLAT, 576, 374));
	m_static_map.push_back(Map(MAP::PLAT, 384, 374));
	m_static_map.push_back(Map(MAP::PLAT, 48, 598));
	m_static_map.push_back(Map(MAP::PLAT, 336, 502));
	m_static_map.push_back(Map(MAP::PLAT, 624, 758));
	m_static_map.push_back(Map(MAP::PLAT, 816, 310));
	m_static_map.push_back(Map(MAP::PLAT, 1080, 726));
	m_static_map.push_back(Map(MAP::PLAT, 1200, 726));
	m_static_map.push_back(Map(MAP::PLAT, 1296, 694));
	m_static_map.push_back(Map(MAP::PLAT, 1104, 54));
	m_static_map.push_back(Map(MAP::PLAT, 1392, 54));
	m_static_map.push_back(Map(MAP::PLAT, 1392, 434));


	m_monster.push_back(Monster(MONSTER::PLANT, 1392, 775));
	m_monster.push_back(Monster(MONSTER::RPLANT, 48, 531));
	m_monster.push_back(Monster(MONSTER::PIG, 432, 184));
	m_monster.push_back(Monster(MONSTER::PIG, 1008, 184));
	//first

	m_monster.push_back(Monster(MONSTER::RPLANT, 250, 307));
	m_monster.push_back(Monster(MONSTER::PLANT, 1392, 180));
	m_monster.push_back(Monster(MONSTER::PLANT, 1392, 480));
	m_monster.push_back(Monster(MONSTER::PLANT, 1392, 780));
	m_monster.push_back(Monster(MONSTER::PIG, 500, 520));
	//second

	m_monster.push_back(Monster(MONSTER::RPLANT, 48, 563));
	m_monster.push_back(Monster(MONSTER::PLANT, 1392, 404));
	m_monster.push_back(Monster(MONSTER::RPLANT, 336, 467));
	m_monster.push_back(Monster(MONSTER::PLANT, 624, 728));
	m_monster.push_back(Monster(MONSTER::PLANT, 816, 280));
	m_monster.push_back(Monster(MONSTER::PLANT, 1080, 696));
	m_monster.push_back(Monster(MONSTER::PLANT, 1200, 696));


	m_obstacle.push_back(Obstacle(OBSTACLE::BLADE, 100, 500));
	m_obstacle.push_back(Obstacle(OBSTACLE::BLADE, 300, 500));


	m_obstacle.push_back(Obstacle(OBSTACLE::BLADE, 790, 600));
	m_obstacle.push_back(Obstacle(OBSTACLE::BLADE, 288, 150));

	m_obstacle.push_back(Obstacle(OBSTACLE::MIDDLE_UP, 912, 406));
	m_obstacle.push_back(Obstacle(OBSTACLE::SHORT, 144, 280));
	m_obstacle.push_back(Obstacle(OBSTACLE::LONG, 432, 402));
	//second

	m_obstacle.push_back(Obstacle(OBSTACLE::BLADE, 48, 374));
	m_obstacle.push_back(Obstacle(OBSTACLE::BLADE, 1296, 654));

	m_obstacle.push_back(Obstacle(OBSTACLE::LONG, 300, 800));
	m_obstacle.push_back(Obstacle(OBSTACLE::LONG, 1100, 800));
	m_obstacle.push_back(Obstacle(OBSTACLE::LONG_UP, 576, -100));

}