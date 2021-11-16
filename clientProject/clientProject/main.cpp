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

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";
LPCTSTR lpszWindowName = "Term Project";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);


//#define NETWORK  //네트워크 기능 켜기

// 시간을 구하기 위한 변수들
LARGE_INTEGER g_tSecond;
LARGE_INTEGER g_tTime;
float         g_fDeltaTime;

int stage = 0;  //0이면 로그인 화면
//////////////////////////
// 로그인 화면에서 키보드 입력하면 id창에 입력
// id창에 글자가 있을때 엔터 누르면(서버에 송신) 게임 시작
//////////////////////////

bool InitClient();

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
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

bool CollisionHelper(RECT, RECT);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	static HDC memdc1;
	static HDC hdc, memdc2;
	HBITMAP hBitmap1, hBitmap2;
	HBRUSH hBrush, oldBrush;


	static vector<Map> m_map;
	static vector<Map> m_static_map;
	static Map m_button[2];
	
	// 충돌 박스 

	static vector<Monster> m_monster;
	static vector<Bullet> vec_bullet;
	static vector<Obstacle> m_obstacle;


	// 물리
	static LARGE_INTEGER tTime;
	static int block_count;

	static Player p(230,600);

	RECT rect = { 675, 450, 825, 465 }; // 글을 쓸 공간 지정


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


		// 여기 부분에 추가.

		m_static_map.push_back(Map(MAP::BUTTON, 200, 120));
		m_static_map.push_back(Map(MAP::BUTTON, 1100, 120));
		m_static_map.push_back(Map(MAP::PLAT, 200, 150));
		m_static_map.push_back(Map(MAP::PLAT, 1100, 150));
		m_static_map.push_back(Map(MAP::PLAT, 500, 150));


		m_obstacle.push_back(Obstacle(OBSTACLE::BLADE, 100, 500));
		m_obstacle.push_back(Obstacle(OBSTACLE::BLADE, 300, 500));

	//	static Obstacle obstacle(OBSTACLE::BLADE, 100, 500);
//		static Obstacle obstacle_map(OBSTACLE::MIDDLE_UP, 100, 500);
		//Monster
		m_monster.push_back(Monster(MONSTER::PLANT, 200, 100));
		m_monster.push_back(Monster(MONSTER::PIG, 500, 100));


		SetTimer(hWnd, 1, 1, NULL);
		//

		break;

	case WM_TIMER:
		switch (wParam) {

		case 1:

			p.UpdateGravity();
			p.animation();

			///////////////////// 이미지 애니메이션///////////////////////////////
			//Monster 애니메이션
			for (int i = 0; i < m_monster.size(); ++i) {
				m_monster[i].animation();
			}
			//Bullet 애니메이션
			for (int i = 0; i < vec_bullet.size(); ++i) {

				vec_bullet[i].animation();

				if (vec_bullet[i].isColl && vec_bullet[i].anim == 0)
					vec_bullet.erase(vec_bullet.begin() + i);

			}

			for (int i = 0; i < m_obstacle.size(); ++i) {
				if (p.CollsionByObstacle(m_obstacle[i])) {
					m_map.clear();
				}
				m_obstacle[i].Move();
				m_obstacle[i].animation();

			}
			for (int i = 0; i < vec_bullet.size(); ++i) {
				vec_bullet[i].Update();
				if (p.CollsionByObstacle(vec_bullet[i]) && vec_bullet[i].getisColl() != true) {
					vec_bullet[i].setisColl(true);
					m_map.clear();
				}
			}
			//하강 시작 && 발판 착지
			if (p.getVely() > 0 || p.getisRanding()) {
				if (p.getVely() > 0) p.SwitchState(PLAYER::FALL);
				int check = 0;
				int b_check = 0;
				
					////서버 코드에서 버튼 state가 둘다 1 일때 다음 스테이지.

					//for (int i = 0; i < 2; ++i)
					//	if (m_button[i].getState() == 1)
					//	{
					//		b_check++;
					//	}
					//if (b_check == 2)
					//	m_button[0].x = 200;
					//else
					//	b_check = 0;


				for (int i = 0; i < m_map.size(); ++i) {
					if (p.getVely() > 600) p.setCollisonHelperY(8);
					else p.setCollisonHelperY(0);

					if (p.FallingCollsionOtherObject(m_map[i]))
					{
						p.setPlayerRanding(m_map[i].y - 32);
						check++;
					}
				}

				for (int i = 0; i < 2; ++i) {
					// 플레이어 버른 누름
					if (p.FallingCollsionOtherObject(m_static_map[i]))
					{
						p.setPlayerRanding(m_static_map[i].y - 16);
						m_static_map[i].setState(true);
						check++;
					}
					// 안누름
					else {
						m_static_map[i].setState(false);
					}
				}


				for (int i = 2; i <m_static_map.size(); ++i) {
					if (p.getVely() > 600) p.setCollisonHelperY(8);
					else p.setCollisonHelperY(0);

					if (p.FallingCollsionOtherObject(m_static_map[i]))
					{
						p.setPlayerRanding(m_static_map[i].y - 32);
						check++;
					}
				}

				if (check == 0) {
					p.setGravity();
				}
				else check = 0;
			}

			// 땅 착지     
			if (p.getPos().y > 780) {
				p.setPlayerRanding(780);
			}





			if (stage != 0) {
				p.Move();
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
				if (p.getJumpCount() < 2) {
					p.Jump();
				}
				break;
			case VK_SHIFT:
				vec_bullet.push_back(m_monster[0].Attack());
				vec_bullet.push_back(m_monster[1].Attack());

				break;
			}
			break;
		}
	case WM_KEYUP:
		p.SwitchState(PLAYER::IDLE);
		break;
	
	case WM_LBUTTONDOWN:
		if (m_map.size()-1 != 5) {
			m_map.push_back(Map(MAP::PLAT,LOWORD(lParam), HIWORD(lParam)));	 
			InvalidateRect(hWnd, NULL, false);
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
			img_Login.Draw(memdc1, 675, 450, 150, 15);  //아이디 입력 창
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
			p.draw(memdc1);


			for (int i = 0; i < vec_bullet.size(); ++i) {
				vec_bullet[i].draw(memdc1);
			}

			for (int i = 0; i < m_obstacle.size(); ++i) {
				m_obstacle[i].draw(memdc1);
			}




		}




		// --- RECT 테스트 ---

		//for (int i = 0; i < bullet_count; ++i) {
		//    Rectangle(memdc1, b_rect[i].left, b_rect[i].top, b_rect[i].right, b_rect[i].bottom);
		//}

		//for (int i = 0; i < MONSTER_AMOUNT; ++i) {
		//    Rectangle(memdc1, m_rect[i].left, m_rect[i].top, m_rect[i].right, m_rect[i].bottom);
		//}

		//for (int i = 0; i < OBS_SERO_COUNT; ++i) {
		//    Rectangle(memdc1, obs_s_rect[i].left, obs_s_rect[i].top, obs_s_rect[i].right, obs_s_rect[i].bottom);
		//}

		//for (int i = 0; i < OBS_GARO_COUNT; ++i) {
		//    Rectangle(memdc1, obs_g_rect[i].left, obs_g_rect[i].top, obs_g_rect[i].right, obs_g_rect[i].bottom);
		//}

		//Rectangle(memdc1, p.x -16, p.y - 16, p.x + 16, p.y+ 16);

		//for (int i = 0; i < w_rect_count; ++i) {            
		//    Rectangle(memdc1, w_rect[i].left, w_rect[i].top, w_rect[i].right, w_rect[i].bottom);
		//}

		// --- RECT 테스트 ---


		BitBlt(hdc, 0, 0, Window_Size_X, Window_Size_Y, memdc1, 0, 0, SRCCOPY);
		if (stage == 0) //id입력
			DrawText(hdc, str, -1, &rect, DT_CENTER | DT_VCENTER);

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


