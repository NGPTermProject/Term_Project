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


//#define NETWORK  //��Ʈ��ũ ��� �ѱ�

// �ð��� ���ϱ� ���� ������
LARGE_INTEGER g_tSecond;
LARGE_INTEGER g_tTime;
float         g_fDeltaTime;

int stage = 0;  //0�̸� �α��� ȭ��
//////////////////////////
// �α��� ȭ�鿡�� Ű���� �Է��ϸ� idâ�� �Է�
// idâ�� ���ڰ� ������ ���� ������(������ �۽�) ���� ����
//////////////////////////

bool InitClient();
void LoadImage();

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
	static Map m_static_map[100];
	static Map m_button[2];
	
	static int m_max_count = 5;
	// �浹 �ڽ� 

	static vector<Monster> m_monster;

	static vector<Bullet> vec_bullet;

	// ����
	static LARGE_INTEGER tTime;
	static int block_count;

	static Player p(230,600);

	RECT rect = { 675, 450, 825, 465 }; // ���� �� ���� ����


	// ���������� �޽����� �߻��� ������ �ʱ�ȭ�ǹǷ� ���� ��� �����ϱ� ���ؼ� static ���
	static TCHAR str[512];

	switch (uMsg) {
		if (stage == 0) {
	case WM_CHAR:  // Ű���� �Է�
		int                str_len;
		str_len = lstrlen(str);

		if ((TCHAR)wParam == '\b') // �� �����̽��� ���
			memmove(str + (str_len - 1), str + str_len, sizeof(TCHAR));
		else if ((TCHAR)wParam == VK_RETURN) {
			if (str_len != 0) {
#ifdef NETWORK
				//id ������ ������ �κ��
				memcpy(buf, str, str_len);
				retval = send(sock, buf, strlen(buf), 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}
				//������ �ޱ�
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
			// WM_CHAR �޽����� �Էµ� ���ڸ� wParam���� �����Ѵ�.
			str[str_len] = (TCHAR)wParam;
			str[str_len + 1] = 0;
		}
		break;
		}
	case WM_CREATE:

		// QueryPerformanceCounter(&tTime);

		 //�ܼ� 
		AllocConsole();
		_tfreopen(_T("CONOUT$"), _T("w"), stdout);
		_tfreopen(_T("CONIN$"), _T("r"), stdin);
		_tfreopen(_T("CONERR$"), _T("w"), stderr);
		//

		LoadImage();

		m_button[0].x = 200;
		m_button[0].y = 120;

		m_button[1].x = 1100;
		m_button[1].y = 120;


		// ���� �κп� �߰�.
		m_static_map[0].x = 200;
		m_static_map[0].y = 150;

		m_static_map[1].x = 1100;
		m_static_map[1].y = 150;

		m_static_map[2].x = 500;
		m_static_map[2].y = 150;
		block_count = 3;

		//Monster

		m_monster.push_back(Monster(0, 200, 100));
		m_monster.push_back(Monster(0, 500, 100));


		SetTimer(hWnd, 1, 1, NULL);
		//

		break;

	case WM_TIMER:
		switch (wParam) {

		case 1:

			p.UpdateGravity();

			//�ϰ� ���� && ���� ����
			if (p.getVely() > 0 || p.getisRanding()) {
				if (p.getVely() > 0) p.SwitchState(PLAYER::FALL);
				int check = 0;
				int b_check = 0;
				for (int i = 0; i < 2; ++i) {
					// �÷��̾� ���� ����
					if (p.FallingCollsionOtherObject(m_button[i]))
					{					
						p.setPlayerRanding(m_button[i].y-16);
						m_button[i].state = 1;
						check++;
					}
					// �ȴ���
					else {
						m_button[i].state = 0;
					}



					//���� �ڵ忡�� ��ư state�� �Ѵ� 1 �϶� ���� ��������.

					for (int i = 0; i < 2; ++i)
						if (m_button[i].state == 1)
						{
							b_check++;
						}
					if (b_check == 2)
						m_button[0].x = 200;
					else
						b_check = 0;
				}



				for (int i = 0; i < m_map.size(); ++i) {
					//p.setCollisonHelperY(0);
					// �߷°��ӿ������� vely���� Ŀ���� ���� ����ϴ°� ������.
					if (p.getVely() > 600) p.setCollisonHelperY(8);
					else p.setCollisonHelperY(0);

					if (p.FallingCollsionOtherObject(m_map[i]))
					{
						p.setPlayerRanding(m_map[i].y - 32);
						check++;
					}
				}

				for (int i = 0; i < 2; ++i) {
					//p.setCollisonHelperY(0);
					// �߷°��ӿ������� vely���� Ŀ���� ���� ����ϴ°� ������.
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

			// �� ����     
			if (p.getPos().y > 780) {
				p.setPlayerRanding(780);
			}


			for (int i = 0; i < vec_bullet.size(); ++i) {
				if (vec_bullet[i].Coll() == false)
					vec_bullet[i].Move();

				if (vec_bullet[i].Coll() && vec_bullet[i].isColl == false) {
					vec_bullet[i].isColl = true;
					vec_bullet[i].anim = 0;
					vec_bullet[i].imageCount = 6;
				}
			}


			if (stage != 0) {
				p.Move();
			}


			///////////////////// �̹��� �ִϸ��̼�///////////////////////////////
			p.animation();
			//Monster �ִϸ��̼�
			for(int i = 0; i < m_monster.size() ; ++i)
				m_monster[i].animation();
			//Bullet �ִϸ��̼�
			for (int i = 0; i < vec_bullet.size(); ++i) {
				
				vec_bullet[i].animation();
				if (vec_bullet[i].isColl && vec_bullet[i].anim == 0)
					vec_bullet.erase(vec_bullet.begin() + i);

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
					break;
				}
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
		if (m_map.size()-1 != m_max_count) {
			m_map.push_back(Map(LOWORD(lParam), HIWORD(lParam)));	 
			InvalidateRect(hWnd, NULL, false);
		}
		break;

	case WM_PAINT:

		hdc = BeginPaint(hWnd, &ps);

		memdc1 = CreateCompatibleDC(hdc);
		hBitmap1 = CreateCompatibleBitmap(hdc, 11500, Window_Size_Y);

		SelectObject(memdc1, hBitmap1);

		// 1. ��� �׸���
		img_bg.Draw(memdc1, 0, 0, Window_Size_X, Window_Size_Y, 0, 0, img_bg_width, img_bg_height);

		if (stage == 0) {
			img_Login.Draw(memdc1, 675, 450, 150, 15);  //���̵� �Է� â
		}
		else {
			// 2. ���� �׸���
			for (int i = 0; i < m_map.size(); ++i)
				img_wall.Draw(memdc1, m_map[i].x - 48, m_map[i].y - 16, 96, 32);


			for (int i = 0; i < block_count; ++i)
				img_wall.Draw(memdc1, m_static_map[i].x - 48, m_static_map[i].y - 16, 96, 32);


			for (int i = 0; i < 2; ++i) {
				if (m_button[i].state == 0)
					img_ButtonUp.Draw(memdc1, m_button[i].x - 32, m_button[i].y - 16, 64, 32);
				else {
					img_ButtonDown.Draw(memdc1, m_button[i].x - 32, m_button[i].y - 16, 64, 32);
				}
			}
			//ButtonBlue.Draw(memdc1, 200, 200, 64, 32);


			for (int i = 0; i < m_monster.size(); ++i) {
				if (m_monster[i].attack == false) {
					//���� Ÿ�Կ� ���� ������ ���� �Ѿ� or ��ź ����
					if (m_monster[i].type == 0)
						img_Bomb_Monster_Idle.Draw(memdc1, m_monster[i].x - 16, m_monster[i].y - 16, 32, 32, m_monster[i].anim, 0, 32, 32);
					else
						img_Bomb_Monster_Idle.Draw(memdc1, m_monster[i].x - 16, m_monster[i].y - 16, 32, 32, m_monster[i].anim, 0, 32, 32);

				}
				else {
					//���� Ÿ�Կ� ���� ������ ���� �Ѿ� or ��ź ����
					if (m_monster[0].type == 0)
						img_Bomb_Monster_Attack.Draw(memdc1, m_monster[i].x - 16, m_monster[i].y - 16, 32, 32, m_monster[i].anim, 0, 32, 32);
					else
						img_Bomb_Monster_Attack.Draw(memdc1, m_monster[i].x - 16, m_monster[i].y - 16, 32, 32, m_monster[i].anim, 0, 32, 32);
				}
			}
			for (int i = 0; i < vec_bullet.size(); ++i) {
				if (vec_bullet[i].isColl)
					img_Bomb_Coll.Draw(memdc1, vec_bullet[i].x - 32, vec_bullet[i].y - 32, 64, 64, vec_bullet[i].anim, 0, 64, 64);
				else
					img_Bomb.Draw(memdc1, vec_bullet[i].x - 32, vec_bullet[i].y - 32, 64, 64, vec_bullet[i].anim, 0, 64, 64);

			}


			// 4. �÷��̾� �׸���
			p.draw(memdc1);
		}




		// --- RECT �׽�Ʈ ---

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

		// --- RECT �׽�Ʈ ---


		BitBlt(hdc, 0, 0, Window_Size_X, Window_Size_Y, memdc1, 0, 0, SRCCOPY);
		if (stage == 0) //id�Է�
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

void LoadImage()
{
	//id�Է�â
	img_Login.Load("white.png");
	//��ư
	img_ButtonUp.Load("button_up.png");
	img_ButtonDown.Load("button_down.png");

	//�Ѿ�
	img_Bullet.Load("bullet.png");
	img_Bullet_Coll.Load("bullet_coll.png");
	img_Bomb.Load("bomb.png");
	img_Bomb_Coll.Load("bomb_coll.png");


	//���
	img_bg.Load("background_grass.png");
	img_bg_width = img_bg.GetWidth();
	img_bg_height = img_bg.GetHeight();

	//����
	img_wall.Load("block_grass.png");

	//����
	img_Bomb_Monster_Attack.Load("monster_bomb1.png");
	img_Bomb_Monster_Idle.Load("monster_bomb_idle.png");
	
	//�÷��̾�
	player_idle.Load("frog_idle.png");
	player_move.Load("frog_move.png");
	player_jump.Load("frog_jump.png");
	player_fall.Load("frog_fall.png");

}

bool CollisionHelper(RECT r1, RECT r2)
{
	if (r1.left > r2.right) return false;
	if (r1.top > r2.bottom) return false;
	if (r1.right < r2.left) return false;
	if (r1.bottom < r2.top) return false;

	return true;
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


