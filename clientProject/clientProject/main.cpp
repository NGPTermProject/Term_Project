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
#include "Map.h"
#include "State.h"
#include "Define.h"
#include "Object.h"
#include "Network.h"
#include "protocol.h"
//#include "Timer.h"

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

int stage = 1;  //0이면 로그인 화면
short MyId;
//////////////////////////
// 로그인 화면에서 키보드 입력하면 id창에 입력
// id창에 글자가 있을때 엔터 누르면(서버에 송신) 게임 시작
//////////////////////////
vector<Player> p;

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
//vector<Bullet> vec_bullet;
//vector<cs_obstacle> bullet;
cs_bullet bullet[50];

int FirstMapSize = 7;		//첫 번째 맵 발판 개수
int SecondMapSize = 15;	//FirstMapSize+8
int FirstMonsterSize = 4;//첫 번째 몬스터  개수
int SecondMonsterSize = 9;	//FirstMonsterSize+5
int FirstObstacleSize = 2;//첫 번째 장애물 개수
int SecondObstacleSize = 5;	//FirstObstacleSize+3

bool CheckCollision(float x, float y);

void send_login()
{
	cs_packet_login packet;
	packet.size = sizeof(packet);
	packet.packet_type = CS_PACKET_LOGIN;

	send(sock, reinterpret_cast<char*>(&packet), packet.size, 0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
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
	UpdateWindow(hWnd);

	CreateThread(NULL, 0, Recv_Thread, NULL, 0, NULL);

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


	send_login();

	sc_login_ok packet;
	recvn(sock, reinterpret_cast<char*>(&packet), sizeof(packet), 0);
	cout << static_cast<int>(packet.packet_type) << endl;
	if (packet.packet_type == SC_PACKET_LOGIN_OK) {
		cout << "login succsess" << endl;
		MyId = packet.id;
		keyinfo.id = packet.id;
		cout << "플레이어 : " << MyId << endl;
		//break;
	}


	int StartTime;
	while (1) {
	//	if (stage != 0) {
			StartTime = GetTickCount64();
			while (GetTickCount64() - StartTime <= 1) {}
			{
				recvn(sock, (char*)&p_info, sizeof(p_info), 0);

				for (int i = 0; i < 2; ++i) {
					p[p_info[i].id].setState(p_info[i].state);
					p[p_info[i].id].setPos(p_info[i].x, p_info[i].y);
					p[p_info[i].id].setJumpCount(p_info[i].jumpCount);
					p[p_info[i].id].setDir(p_info[i].dir);
				}

				recvn(sock, (char*)&cs_obs, sizeof(cs_obs), 0);
				{
					for (int i = 0; i < 2; ++i) {
						m_obstacle[i].x = cs_obs[i].x;
						m_obstacle[i].y = cs_obs[i].y;
					}
				}

				recvn(sock, (char*)&put, sizeof(put), 0);


				if (put.isClick) {
					m_map.push_back(Map(MAP::PLAT, put.x, put.y));
				}
				if (stage == 1) {
					for (int i = 0; i < FirstMapSize; ++i)
						m_static_map[i].setState(put.isPush[i]);
				}
				else if (stage == 2) {
					for (int i = FirstMapSize; i < SecondMapSize; ++i)
						m_static_map[i].setState(put.isPush[i]);
				}
				else if (stage == 3) {
					for (int i = SecondMapSize; i < m_static_map.size(); ++i)
						m_static_map[i].setState(put.isPush[i]);
				}


				if (put.AttackMonsterId != -1) {
					m_monster[put.AttackMonsterId].Attack();
					//vec_bullet.push_back(m_monster[put.AttackMonsterId].Attack());
					//cs_obstacle s;
					//bullet.push_back(s);
				}
				/*if (put.bulletsize != vec_bullet.size()) {
					vec_bullet.erase(vec_bullet.begin() + vec_bullet.size() - 1);
				}*/

				recvn(sock, (char*)&bullet, sizeof(bullet), 0);
				for (int i = 0; i < 15; ++i) {
					//if (bullet[i].isColl) vec_bullet[i].setisColl(true);
					m_bullet[i].x = bullet[i].x;
					m_bullet[i].y = bullet[i].y;
					m_bullet[i].type = bullet[i].type;
					m_bullet[i].imageCount = bullet[i].imageCount;
					m_bullet[i].imageSizeX = bullet[i].imageSizeX;
					m_bullet[i].imageSizeY = bullet[i].imageSizeY;
					m_bullet[i].anim = bullet[i].anim;
					m_bullet[i].isColl = bullet[i].isColl;
					m_bullet[i].isStart = bullet[i].isStart;

				}
			}
			/*	sc_button bt;
			recvn(sock, (char*)&bt, sizeof(bt), 0);
			for (int i = 0; i < 2; ++i)
				m_static_map[i].setState(bt.isPush[i]);*/


				//	recvn(sock, (char*)&hero, sizeof(hero), 0);
				//	recvn(sock, (char*)&boss, sizeof(boss), 0);
		}
//	}
	closesocket(sock);
	//exit(1);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	static HDC memdc1;
	static HDC hdc, memdc2;
	HBITMAP hBitmap1, hBitmap2;
	HBRUSH hBrush, oldBrush;



	// 충돌 박스 



	// 물리
	static LARGE_INTEGER tTime;
	static int block_count;


	RECT rect = { 675, 450, 825, 465 }; // 글을 쓸 공간 지정
	static int map_current_count = 0;
	static int map_max_count = 5;

	static bool isJumping = false;

	// 지역변수는 메시지가 발생할 때마다 초기화되므로 값을 계속 유지하기 위해서 static 사용
	static TCHAR str[512];

	switch (uMsg) {
		if (stage == 0) {
	case WM_CHAR:  // 키보드 입력
		int                str_len;
		str_len = lstrlen(str);

		if ((TCHAR)wParam == '\b') // 백 스페이스일 경우
			memmove(str + (str_len - 1), str + str_len, sizeof(TCHAR));
		else if ((TCHAR)wParam == VK_RETURN) {
			if (str_len != 0) {
				//send_login();

				//sc_login_ok packet;
				//recvn(sock, reinterpret_cast<char*>(&packet), sizeof(packet), 0);
				//cout << static_cast<int>(packet.packet_type) << endl;
				//if (packet.packet_type == SC_PACKET_LOGIN_OK) {
				//	cout << "login succsess" << endl;
				//	MyId = packet.id;
				//	keyinfo.id = packet.id;
				//	cout << "플레이어 : " << MyId << endl;
				//	//break;
				//}
				stage = 1;
				cout << str << ", " << stage << endl;
			}
		}
		else
		{
			// WM_CHAR 메시지는 입력된 문자를 wParam으로 전달한다.
			str[str_len] = (TCHAR)wParam;
			str[str_len + 1] = 0;
		}
		break;
		}
	case WM_CREATE:

		// QueryPerformanceCounter(&tTime);

		 //콘솔 
		AllocConsole();
		_tfreopen(_T("CONOUT$"), _T("w"), stdout);
		_tfreopen(_T("CONIN$"), _T("r"), stdin);
		_tfreopen(_T("CONERR$"), _T("w"), stderr);
		//

		LoadImage();

		p.push_back(Player(200, 600, 0));
		p.push_back(Player(400, 600, 1));



		// 여기 부분에 추가.


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
		m_static_map.push_back(Map(MAP::LONG, 432, 342));
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
		m_monster.push_back(Monster(MONSTER::TREE, 1392, 180));
		m_monster.push_back(Monster(MONSTER::TREE, 1392, 480));
		m_monster.push_back(Monster(MONSTER::TREE, 1392, 780));
		m_monster.push_back(Monster(MONSTER::PIG, 500, 520));
		//second

		m_monster.push_back(Monster(MONSTER::RPLANT, 48, 563));
		m_monster.push_back(Monster(MONSTER::TREE, 1392, 404));
		m_monster.push_back(Monster(MONSTER::RPLANT, 336, 467));
		m_monster.push_back(Monster(MONSTER::PIG, 624, 728));
		m_monster.push_back(Monster(MONSTER::PIG, 816, 280));
		m_monster.push_back(Monster(MONSTER::PIG, 1080, 696));
		m_monster.push_back(Monster(MONSTER::PIG, 1200, 696));



		m_obstacle.push_back(Obstacle(OBSTACLE::BLADE, 100, 500));
		m_obstacle.push_back(Obstacle(OBSTACLE::BLADE, 300, 500));
		//first

		m_obstacle.push_back(Obstacle(OBSTACLE::MIDDLE_UP, 912, 406));
		m_obstacle.push_back(Obstacle(OBSTACLE::SHORT, 144, 280));
		m_obstacle.push_back(Obstacle(OBSTACLE::LONG, 432, 402));
		//second

		m_obstacle.push_back(Obstacle(OBSTACLE::LONG, 300, 800));
		m_obstacle.push_back(Obstacle(OBSTACLE::LONG, 1100, 800));
		m_obstacle.push_back(Obstacle(OBSTACLE::LONG_UP, 576, -100));


		//Timer::Reset();
		SetTimer(hWnd, 1, 10, NULL);
		//
		//InitClient();
		break;

	case WM_TIMER:
		switch (wParam) {

		case 1:
		//	if (stage != 0) {
				//int StartTime;
				send(sock, (char*)&keyinfo, sizeof(keyinfo), 0);
				keyinfo.isClick = false;

				//StartTime = GetTickCount64();
				//while (GetTickCount64() - StartTime <= 10) {}
				/*send(sock, (char*)&keyinfo, sizeof(keyinfo), 0);
				keyinfo.isClick = false;*/
				//keyinfo.jump = false;

				for (int i = 0; i < 2; ++i)
					p[i].animation();

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
				//Bullet 애니메이션
				//for (int i = 0; i < vec_bullet.size(); ++i) {

				//	//vec_bullet[i].animation();
				//	if (vec_bullet[i].isColl && vec_bullet[i].anim == 0) {
				//		vec_bullet.erase(vec_bullet.begin() + i);
				//	}
				//	
				//}

				break;
			}
			InvalidateRect(hWnd, NULL, false);
	//	}
		break;

	case WM_KEYDOWN:
		if (stage != 0) {
			switch (wParam)
			{
			case VK_SPACE:
				if (p[MyId].getJumpCount() < 2) {
					keyinfo.jump = true;
				}
				break;
			case VK_LEFT:
				keyinfo.left = true;
				break;
			case VK_RIGHT:
				keyinfo.right = true;
				break;
			case 49:
				cout << "stage 1" << endl;
				stage = 1;
				p[0].setPos(200, 600);
				p[1].setPos(400, 600);
				break;
			case 50:
				cout << "stage 2" << endl;
				stage = 2;
				p[0].setPos(200, 600);
				p[1].setPos(400, 600);
				break;
			case 51:
				cout << "stage 3" << endl;
				stage = 3;
				p[0].setPos(384, 320);
				p[1].setPos(480, 320);
				break;
			}
			break;
		}
		break;
	case WM_KEYUP:
		if (stage != 0) {
			if (wParam == VK_LEFT) {
				keyinfo.left = false;

			}
			if (wParam == VK_RIGHT) {
				keyinfo.right = false;

			}
			if (wParam == VK_SPACE) {
				isJumping = false;
			}
			keyinfo.jump = false;
		}
		break;

	case WM_LBUTTONDOWN:
		if (stage != 0) {
			if (map_current_count != map_max_count) {
				if (CheckCollision(LOWORD(lParam), HIWORD(lParam))) {
					keyinfo.isClick = true;
					keyinfo.x = LOWORD(lParam);
					keyinfo.y = HIWORD(lParam);
					map_current_count++;
					//m_map.push_back(Map(MAP::PLAT,LOWORD(lParam), HIWORD(lParam)));	 
					//InvalidateRect(hWnd, NULL, false);
				}
			}
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
			img_Login.Draw(memdc1, 675, 320, 150, 15);  //아이디 입력 창
			RECT rect = { 685, 320, 835, 465 }; // 글을 쓸 공간 지정
			DrawText(memdc1, str, -1, &rect, DT_VCENTER | DT_WORDBREAK);
			RECT r = { 590, 320,740,465 };
			DrawText(memdc1, L"ID: ", -1, &r, DT_CENTER | DT_VCENTER);
		}
		else {

			//다른 오브젝트 발판 그리기
			for (int i = 0; i < m_map.size(); ++i)
				m_map[i].draw(memdc1);

			if (stage == 1) {
				for (int i = 0; i < FirstMapSize; ++i)
					m_static_map[i].draw(memdc1);
				for (int i = 0; i < FirstMonsterSize; ++i) {
					m_monster[i].draw(memdc1);
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
				}
				for (int i = SecondObstacleSize; i < m_obstacle.size(); ++i) {
					m_obstacle[i].draw(memdc1);
				}
			}

			//플레이어 그리기
			for (int i = 0; i < p.size(); ++i) {
				p[i].draw(memdc1);
			}


			for (int i = 0; i < 15; ++i) {
				if (m_bullet[i].isStart)
					m_bullet[i].draw(memdc1);
			}

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

bool CheckCollision(float x, float y) {
	for (int i = 0; i < m_map.size(); ++i) {
		if (m_map[i].x - 48 <= x && x <= m_map[i].x + 48 &&
			m_map[i].y - 16 <= y && y <= m_map[i].y + 16) {
			return false;
		}
	}
	for (int i = 0; i < m_static_map.size(); ++i) {
		if (m_static_map[i].x - 48 <= x && x <= m_static_map[i].x + 48 &&
			m_static_map[i].y - 16 <= y && y <= m_static_map[i].y + 16) {
			return false;
		}
	}
	return true;
}