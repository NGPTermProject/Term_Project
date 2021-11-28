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

int stage = 0;  //0이면 로그인 화면
short MyId;
//////////////////////////
// 로그인 화면에서 키보드 입력하면 id창에 입력
// id창에 글자가 있을때 엔터 누르면(서버에 송신) 게임 시작
//////////////////////////
vector<Player> p;

bool InitClient();
bool CheckCollision(float x, float y);	//같은 위치에 블록 생성 안되게
cs_send_player_id id;
cs_send_player p_info[2];
cs_send_keyinfo keyinfo;
sc_put_object put;
vector<Map> m_map;
vector<Obstacle> m_obstacle;
Map m_button[2];
vector<Map> m_static_map;

cs_obstacle cs_obs[2];

<<<<<<< HEAD

void send_login()
{
	cs_packet_login packet;
	packet.size = sizeof(packet);
	packet.packet_type = CS_PACKET_LOGIN;

	send(sock, reinterpret_cast<char*>(&packet), packet.size, 0);
=======
vector<Monster> m_monster;
vector<Bullet> vec_bullet;
//vector<cs_obstacle> bullet;
cs_bullet bullet[50];
>>>>>>> parent of c122221 (push_bullet)


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

	//PlayerID 전달.
<<<<<<< Updated upstream
	recvn(sock, (char*)&id, sizeof(cs_send_player_id), 0);
	MyId = id.id;
	keyinfo.id = id.id;
	cout << MyId << endl;
=======
	sc_login_ok packet;
	recvn(sock, reinterpret_cast<char*>(&packet), sizeof(packet), 0);
	cout << static_cast<int>(packet.packet_type) << endl;
	if (packet.packet_type == SC_PACKET_LOGIN_OK) {
		cout << "login succsess" << endl;
		MyId = packet.id;
		keyinfo.id = packet.id;
		cout << MyId << endl;
	}
	
	int StartTime;
>>>>>>> Stashed changes
	while (1) {
<<<<<<< HEAD

		recvn(sock, (char*)&p_info, sizeof(p_info), 0);

		for (int i = 0; i < 2; ++i) {
			p[p_info[i].id].setState(p_info[i].state);
			p[p_info[i].id].setPos(p_info[i].x, p_info[i].y);
			p[p_info[i].id].setJumpCount(p_info[i].jumpCount);
			p[p_info[i].id].setDir(p_info[i].dir);

		}

		recvn(sock, (char*)&cs_obs, sizeof(cs_obs), 0);
		{
=======
		StartTime = GetTickCount64();


		while (GetTickCount64() - StartTime <= 10) { }
		{
			recvn(sock, (char*)&p_info, sizeof(p_info), 0);

>>>>>>> parent of c122221 (push_bullet)
			for (int i = 0; i < 2; ++i) {
				m_obstacle[i].x = cs_obs[i].x;
				m_obstacle[i].y = cs_obs[i].y;

<<<<<<< HEAD
			}
		}
=======
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
			for (int i = 0; i < 2; ++i)
				m_static_map[i].setState(put.isPush[i]);
			if (put.AttackMonsterId != -1) {
				vec_bullet.push_back(m_monster[put.AttackMonsterId].Attack());
				//cs_obstacle s;
				//bullet.push_back(s);
			}
			if (put.bulletsize != vec_bullet.size()) {
				vec_bullet.erase(vec_bullet.begin() + vec_bullet.size() - 1);
			}

			recvn(sock, (char*)&bullet, sizeof(bullet), 0);
			for (int i = 0; i < vec_bullet.size(); ++i) {
				//if (bullet[i].isColl) vec_bullet[i].setisColl(true);
				vec_bullet[i].x = bullet[i].x;
				vec_bullet[i].y = bullet[i].y;
				vec_bullet[i].type = bullet[i].type;
				vec_bullet[i].imageCount = bullet[i].imageCount;
				vec_bullet[i].imageSizeX = bullet[i].imageSizeX;
				vec_bullet[i].imageSizeY = bullet[i].imageSizeY;
				vec_bullet[i].anim = bullet[i].anim;
				vec_bullet[i].isColl = bullet[i].isColl;
			}

>>>>>>> parent of c122221 (push_bullet)

		recvn(sock, (char*)&put, sizeof(put), 0);
		if (put.isClick) {
			m_map.push_back(Map(MAP::PLAT, put.x, put.y));
		}
		for (int i = 0; i < 2; ++i)
			m_static_map[i].setState(put.isPush[i]);

		/*	sc_button bt;
		recvn(sock, (char*)&bt, sizeof(bt), 0);
		for (int i = 0; i < 2; ++i)
			m_static_map[i].setState(bt.isPush[i]);*/


			//	recvn(sock, (char*)&hero, sizeof(hero), 0);
			//	recvn(sock, (char*)&boss, sizeof(boss), 0);

	}
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

	static vector<Monster> m_monster;
	static vector<Bullet> vec_bullet;


	// 물리
	static LARGE_INTEGER tTime;
	static int block_count;


	static int map_current_count = 0;
	static int map_max_count = 5;

	static bool isJumping = false;

	// 지역변수는 메시지가 발생할 때마다 초기화되므로 값을 계속 유지하기 위해서 static 사용
	static TCHAR str[512];

	switch (uMsg) {
		//	if (stage == 0) {
	case WM_CHAR:  // 키보드 입력
		int                str_len;
		str_len = lstrlen(str);

		if ((TCHAR)wParam == '\b') // 백 스페이스일 경우
			memmove(str + (str_len - 1), str + str_len, sizeof(TCHAR));
		else if ((TCHAR)wParam == VK_RETURN) {
			if (str_len != 0) {
#ifdef NETWORK
				//id 서버로 보내고 로비로
				memcpy(buf, str, str_len);
				retval = send(sock, buf, strlen(buf), 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}
				//데이터 받기
				retval = recvn(sock, buf, retval, 0);
				if (retval == SOCKET_ERROR) {
					err_display("recv()");
					break;
				}
				else if (retval == 0)
					break;
#endif
				stage = 1;
			}
		}
		else
		{
			// WM_CHAR 메시지는 입력된 문자를 wParam으로 전달한다.
			str[str_len] = (TCHAR)wParam;
			str[str_len + 1] = 0;
		}
		break;
		//3	}
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

		m_obstacle.push_back(Obstacle(OBSTACLE::BLADE, 100, 500));
		m_obstacle.push_back(Obstacle(OBSTACLE::BLADE, 300, 500));

		m_monster.push_back(Monster(MONSTER::PLANT, 1392, 780));
		m_monster.push_back(Monster(MONSTER::PIG, 48, 536));
		m_monster.push_back(Monster(MONSTER::PIG, 432, 184));
		m_monster.push_back(Monster(MONSTER::PIG, 1008, 184));


		//Timer::Reset();
		SetTimer(hWnd, 1, 10, NULL);
		//
		//InitClient();
		break;

	case WM_TIMER:
		switch (wParam) {

		case 1:
<<<<<<< HEAD

			send(sock, (char*)&keyinfo, sizeof(keyinfo), 0);
			keyinfo.isClick = false;
			keyinfo.jump = false;
=======
		
			send(sock, (char*)&keyinfo, sizeof(keyinfo), 0);
			keyinfo.isClick = false;
			//keyinfo.jump = false;
>>>>>>> parent of c122221 (push_bullet)

			for(int i =0 ; i< 2 ; ++i )
				p[i].animation();


			for (int i = 0; i < m_monster.size(); ++i) {
				m_monster[i].animation();
			}
			//Bullet 애니메이션
			for (int i = 0; i < vec_bullet.size(); ++i) {

				vec_bullet[i].animation();

			}
			for (int i = 0; i < m_obstacle.size(); ++i) {
				m_obstacle[i].animation();
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
			isJumping = false;
		}
		keyinfo.jump = false;
		break;

	case WM_LBUTTONDOWN:
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
		break;

	case WM_PAINT:
	{
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

			for (int i = 0; i < m_static_map.size(); ++i)
				m_static_map[i].draw(memdc1);

			for (int i = 0; i < m_monster.size(); ++i) {
				m_monster[i].draw(memdc1);
			}

			//플레이어 그리기
			for (int i = 0; i < p.size(); ++i) {
				p[i].draw(memdc1);
			}


			for (int i = 0; i < vec_bullet.size(); ++i) {
				vec_bullet[i].draw(memdc1);
			}

			for (int i = 0; i < m_obstacle.size(); ++i) {
				m_obstacle[i].draw(memdc1);
			}

		}


		BitBlt(hdc, 0, 0, Window_Size_X, Window_Size_Y, memdc1, 0, 0, SRCCOPY);

		DeleteObject(SelectObject(memdc1, hBitmap1));
		DeleteDC(memdc1);
		EndPaint(hWnd, &ps);

		break;
	}
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