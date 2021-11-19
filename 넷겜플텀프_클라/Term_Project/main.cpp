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
#include "Obstacle.h"
#include "Network.h"
#include "protocol.h"

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";
LPCTSTR lpszWindowName = "Term Project";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);


// �ð��� ���ϱ� ���� ������
LARGE_INTEGER g_tSecond;
LARGE_INTEGER g_tTime;
float         g_fDeltaTime;

/// ///////////////////////////////////
//#define NETWORK  //��Ʈ��ũ ��� �ѱ�
int stage = 0;  //0�̸� �α��� ȭ��

cs_packet_login csLogin;
sc_login_ok scLogin;
cs_put_button csPutButton;
sc_move scMove;
cs_mouse_input csMouse;
int str_len;

MAP m_map[10];   //����ڰ� Ŭ���� ����
MAP m_static_map[100];  //������ ��ġ�� �ִ� ����
int m_count = 0;  //���� ��ġ�� ����� ��ġ ���� ��
int m_max_count = 5; //��ġ�� �� �ִ� �ִ� ���� ��
int block_count = 3;  //������ ��ġ�� ������ ����

bool InitClient();
void SendID();
void RecvID();
void CheckID(sc_login_ok);
void SendKey(int, bool);
void RecvPlayerPos(Player&, float&);
bool CheckObjectCollision(float x, float y);
void SendObjectInfo();
void RecvObjectInfo();
///////////////////////////////////////

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

struct MAP {
	static int count;
	int x;
	int y;

	int type = 0;
	int state = 0;
};

struct Player {
	float x;
	float y;
	int dir;
	int imageCount;
	int state;
	int anim;
	int speed;

	int imageSize;

	//void ()
};

struct Monster {
	float x;
	float y;

	int type;
	bool attack;
	int imageCount;
	int anim;

	int imageSize;

};

struct Bullet {
	float x = 0;
	float y = 0;

	int imageSize = 0;


	float dx = 3;
	float dy = 3;

	int dir = 1;

	int type = 0;
	int imageCount = 0;
	int anim = 0;
	bool isColl = false;

	Bullet(int m_type, int m_x, int m_y)
	{
		x = m_x;
		y = m_y;
		type = m_type;
		anim = 0;
		if (type == 0)
			imageCount = 2;
		else if (type == 1)
			imageCount = 4;

	}

	void Move()
	{
		if (type = 1) {

			float gravity = 0.3f;
			dy += gravity;
			x += dx;
			y += dy;

		}
	}

	bool Coll()
	{
		if (y > 780)
			return true;
		else
			return false;
	}


};

void LoadImage();
bool CollisionHelper(RECT, RECT);
void PlayerMove(float, float);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	static HDC hdc, memdc1, memdc2;
	HBITMAP hBitmap1, hBitmap2;
	HBRUSH hBrush, oldBrush;

	// Player ��ġ, �ִϸ��̼�, ũ��, ĭ�� ���


	// Map

	static MAP m_button[2];

	// �浹 �ڽ� 
	static RECT p_rect;

	static Monster m_monster[2];

	static vector<Bullet> vec_bullet;

	// ����
	static float dx, dy;
	static float accX, accY;
	static float velX, velY;
	static float  PLAYER_ACC = 1.f;
	static float  PLAYER_FRICTION = -0.2f;
	static float  PLAYER_GRAVITY = 90.8;

	static bool isRanding = false;
	static bool isJump = false;

	static int jumpCount = 0;

	static LARGE_INTEGER tTime;
	static int block_count;

	static Player p;

	RECT               rect = { 675, 450, 825, 465 }; // ���� �� ���� ����

	switch (uMsg) {
		if (stage == 0) {
	case WM_CHAR:  // Ű���� �Է�
		str_len = lstrlen(csLogin.name);

		if ((TCHAR)wParam == '\b') // �� �����̽��� ���
			memmove(csLogin.name + (str_len - 1), csLogin.name + str_len, sizeof(char));
		else if ((TCHAR)wParam == VK_RETURN) {
			if (str_len != 0) {
#ifdef NETWORK
				SendID();
				RecvID();
				CheckID(scLogin);
#endif
			}
			stage = 1;
		}
		else
		{
			// WM_CHAR �޽����� �Էµ� ���ڸ� wParam���� �����Ѵ�.
			csLogin.name[str_len] = (TCHAR)wParam;
			csLogin.name[str_len + 1] = 0;
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
		p.x = 230;
		p.y = 650;

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

		//Monster
		m_monster[0].x = 300;
		m_monster[0].y = 550;
		m_monster[0].attack = false;
		m_monster[0].type = 0;
		m_monster[0].imageCount = 10;
		m_monster[0].anim = 0;

		//p.x = 9900;
		//p.y = 550;

		player = frog_idle;
		p.state = PLAYER::IDLE;

		SetTimer(hWnd, 1, 1, NULL);
		//

		break;

	case WM_TIMER:
		switch (wParam) {

		case 1:

			accY = PLAYER_GRAVITY;
			velY += accY;
			p.y += (velY + accY) * 0.03f;

			//�ϰ� ���� && ���� ����
			if (velY > 0 || isRanding) {
				if (velY > 0) p.state = PLAYER::FALL;
				int check = 0;

				int b_check = 0;
				for (int i = 0; i < 2; ++i) {
					// �÷��̾� ���� ����
					if (p.y + 16 >= m_button[i].y - 16 && p.y - 8 <= m_button[i].y - 10 &&
						p.x < m_button[i].x + 32 && p.x > m_button[i].x - 32)
					{
						p.state = PLAYER::IDLE;
						accY = 0;
						velY = 0;
						p.y = m_button[i].y - 16;
						PLAYER_GRAVITY = 0;
						isRanding = true;
						jumpCount = 0;
						check++;
						m_button[i].state = 1;

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



				for (int i = 0; i < m_count; ++i) {
					int collsize = 0;

					// �߷°��ӿ������� vely���� Ŀ���� ���� ����ϴ°� ������.
					if (velY > 600) collsize = 8;
					else collsize = 0;

					if (p.y + 16 >= m_map[i].y - 16 && p.y - collsize <= m_map[i].y - 10 &&
						p.x < m_map[i].x + 48 && p.x > m_map[i].x - 48)
					{
						p.state = PLAYER::IDLE;
						accY = 0;
						velY = 0;
						p.y = m_map[i].y - 32;
						PLAYER_GRAVITY = 0;
						isRanding = true;
						jumpCount = 0;
						check++;
					}

					if (p.y + 16 >= m_static_map[i].y - 16 && p.y - collsize <= m_static_map[i].y - 10 &&
						p.x < m_static_map[i].x + 48 && p.x > m_static_map[i].x - 48 && i < 2)
					{
						p.state = PLAYER::MOVE;
						accY = 0;
						velY = 0;
						p.y = m_static_map[i].y - 32;
						PLAYER_GRAVITY = 0;
						isRanding = true;
						jumpCount = 0;
						check++;
					}
				}
				if (check == 0) {
					PLAYER_GRAVITY = 30.8f;
					isRanding = false;
				}
				else check = 0;
			}

			// �� ����     
			if (p.y > 780) {
				p.state = PLAYER::IDLE;
				accY = 0;
				velY = 0;
				PLAYER_GRAVITY = 0;
				p.y = 780;
				jumpCount = 0;
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


			// 0�� ��� - Ű���� �Է¿� ���� ���
			if (stage != 0) {
				if (GetAsyncKeyState(VK_LEFT) & 0x8000)
				{
#ifdef NETWORK
					SendKey(VK_LEFT, TRUE);
					RecvPlayerPos();
#else
					p.state = PLAYER::MOVE;
					p.dir = P_DIR_LEFT;
					p.x -= (p.speed * 0.016f);
					velX -= 10;
#endif
				}
#ifdef NETWORK
				else {
					SendKey(VK_LEFT, FALSE);
				}
#endif
				if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
				{
#ifdef NETWORK
					SendKey(VK_RIGHT, TRUE);
					RecvPlayerPos();
#else
					p.state = PLAYER::MOVE;
					p.dir = P_DIR_RIGHT;
					p.x += (p.speed * 0.016f);
					velX = 10;
#endif
				}
#ifdef NETWORK
				else {
					SendKey(VK_RIGHT, FALSE);
				}
#endif
			}
			///////////////////// �̹��� �ִϸ��̼�///////////////////////////////
			//Player �ִϸ��̼�
			p.anim += P_IMAGE_SIZE;
			if (p.anim >= P_IMAGE_SIZE * (p.imageCount - 1))
				p.anim = 0;

			//Monster �ִϸ��̼�
			m_monster[0].anim += 32;
			if (m_monster[0].anim >= 32 * (m_monster[0].imageCount - 1))
			{
				if (m_monster[0].attack)
				{
					//�Ѿ� ����
					Bullet bullet(m_monster[0].type, m_monster[0].x, m_monster[0].y);
					vec_bullet.push_back(bullet);

					m_monster[0].imageCount = 10;
					m_monster[0].attack = false;
					//m_monster[0].
				}
				m_monster[0].anim = 0;
				m_monster[0].attack = true;
				m_monster[0].anim = 0;
				m_monster[0].imageCount = 7;
			}

			//Bullet �ִϸ��̼�
			for (int i = 0; i < vec_bullet.size(); ++i) {
				vec_bullet[i].anim += 64;
				cout << vec_bullet[i].anim << endl;

				if (vec_bullet[i].anim >= 64 * (vec_bullet[i].imageCount - 1)) {
					vec_bullet[i].anim = 0;

					if (vec_bullet[i].isColl)
						vec_bullet.erase(vec_bullet.begin() + i);
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
			case VK_SPACE:
				if (jumpCount < 2) {
					p.state = PLAYER::JUMP;
					accY = 0;
					PLAYER_GRAVITY = 30.f;
					isJump = true;
					isRanding = false;
					velY = -400.5f;
					jumpCount++;
					break;
				}
			case VK_SHIFT:
				m_monster[0].attack = true;
				m_monster[0].anim = 0;
				m_monster[0].imageCount = 7;
				break;
			}
			break;
		}
	case WM_KEYUP:
		p.state = PLAYER::IDLE;
		p.speed = 300.f;
		break;
	case WM_LBUTTONDOWN:
#ifdef NETWORK
		if (m_count != m_max_count) {
			if (CheckObjectCollision(LOWORD(lParam), HIWORD(lParam))) {
				SendObjectInfo();
				m_map[m_count].x = LOWORD(lParam);
				m_map[m_count].y = HIWORD(lParam);
				m_count++;
				InvalidateRect(hWnd, NULL, false);
			}
		}
#else
		if (m_count != m_max_count) {
			m_map[m_count].x = LOWORD(lParam);
			m_map[m_count].y = HIWORD(lParam);
			m_count++;
		}
		InvalidateRect(hWnd, NULL, false);
#endif
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
			for (int i = 0; i < m_count; ++i)
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


			if (m_monster[0].attack == false) {
				//���� Ÿ�Կ� ���� ������ ���� �Ѿ� or ��ź ����
				if (m_monster[0].type == 0)
					img_Bomb_Monster_Idle.Draw(memdc1, m_monster[0].x - 16, m_monster[0].y - 16, 32, 32, m_monster[0].anim, 0, 32, 32);
				else
					img_Bomb_Monster_Idle.Draw(memdc1, m_monster[0].x - 16, m_monster[0].y - 16, 32, 32, m_monster[0].anim, 0, 32, 32);

			}
			else {
				//���� Ÿ�Կ� ���� ������ ���� �Ѿ� or ��ź ����
				if (m_monster[0].type == 0)
					img_Bomb_Monster_Attack.Draw(memdc1, m_monster[0].x - 16, m_monster[0].y - 16, 32, 32, m_monster[0].anim, 0, 32, 32);
				else
					img_Bomb_Monster_Attack.Draw(memdc1, m_monster[0].x - 16, m_monster[0].y - 16, 32, 32, m_monster[0].anim, 0, 32, 32);
			}

			for (int i = 0; i < vec_bullet.size(); ++i) {
				if (vec_bullet[i].isColl)
					img_Bomb_Coll.Draw(memdc1, vec_bullet[i].x - 32, vec_bullet[i].y - 32, 64, 64, vec_bullet[i].anim, 0, 64, 64);
				else
					img_Bomb.Draw(memdc1, vec_bullet[i].x - 32, vec_bullet[i].y - 32, 64, 64, vec_bullet[i].anim, 0, 64, 64);

			}


			// 4. �÷��̾� �׸���
			switch (p.state)
			{
			case PLAYER::IDLE:
				p.imageCount = 11;
				player = frog_idle;
				player.Draw(memdc1, p.x - 16, p.y - 16, 32, 32, p.anim, p.dir, 32, 32);
				break;

			case PLAYER::MOVE:
				p.imageCount = 12;
				player = frog_move;
				player.Draw(memdc1, p.x - 16, p.y - 16, 32, 32, p.anim, p.dir, 32, 32);
				break;

			case PLAYER::JUMP:
				player = frog_jump;
				player.Draw(memdc1, p.x - 16, p.y - 16, 32, 32, 0, p.dir, 32, 32);
				break;
			case PLAYER::FALL:
				player = frog_fall;
				player.Draw(memdc1, p.x - 16, p.y - 16, 32, 32, 0, p.dir, 32, 32);
				break;
			case PLAYER::ATTACK:
				p.imageCount = 7;
				player = frog_attack;
				player.Draw(memdc1, p.x - 16, p.y - 16, 32, 32, p.anim, 0, 32, 32);
				break;
			case PLAYER::DEAD:
				break;
			}
		}

		BitBlt(hdc, 0, 0, Window_Size_X, Window_Size_Y, memdc1, 0, 0, SRCCOPY);
		if (stage == 0) //id�Է�
			DrawText(hdc, csLogin.name, -1, &rect, DT_CENTER | DT_VCENTER);

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

	//img_Bullet_Monster_Attack.Load();
	//img_Bullet_Monster_Idle.Load();



	//�÷��̾�
	frog_idle.Load("frog_idle.png");
	frog_move.Load("frog_move.png");
	frog_jump.Load("frog_jump.png");
	frog_fall.Load("frog_fall.png");
	frog_attack.Load("frog_attack.png");

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

	WSAData wsa;
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
	int retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) {
		err_quit("connet()");
	}
	return true;
}

void SendID() {
	csLogin.packet_type = CS_PACKET_LOGIN;
	int retval = send(sock, (char*)&csLogin, sizeof(csLogin), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
}

void RecvID() {
	char buf[BUFSIZE];
	int retval = recv(sock, buf, sizeof(sc_login_ok), 0);
	if (retval == SOCKET_ERROR) {
		exit(1);
	}

	buf[retval] = '\0';
	scLogin = *(sc_login_ok*)buf;
}

void CheckID(sc_login_ok scLogin) {
	if (scLogin.size != 0)
		stage = 1;
}

void SendKey(int key, bool IsPush) {
	csPutButton.size = key;
	csPutButton.packet_type = CS_PUT_BUTTON;
	csPutButton.isPush = IsPush;
	int retval = send(sock, (char*)&csPutButton, sizeof(csPutButton), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
}

void RecvPlayerPos(Player& p, float& vel) {
	char buf[BUFSIZE];
	int retval = recv(sock, buf, sizeof(sc_move), 0);
	if (retval == SOCKET_ERROR) {
		exit(1);
	}
	buf[retval] = '\0';
	scMove = *(sc_move*)buf;

	p.state = PLAYER::MOVE;
	p.dir = scMove.dir;
	p.x = scMove.x;
	if (p.dir == P_DIR_RIGHT)
		vel = 10;
	else
		vel -= 10;

}

bool CheckObjectCollision(float x, float y) {
	for (int i = 0; i < m_count; ++i) {
		if (m_map[i].x - 48 <= x && x <= m_map[i].x + 48 &&
			m_map[i].y - 16 <= y && y <= m_map[i].y + 16) {
			return false;
		}
	}
	for (int i = 0; i < block_count; ++i) {
		if (m_static_map[i].x - 48 <= x && x <= m_static_map[i].x + 48 &&
			m_static_map[i].y - 16 <= y && y <= m_static_map[i].y + 16) {
			return false;
		}
	}
	csMouse.packet_type = CS_MOUSE_INPUT;
	csMouse.x = x;
	csMouse.y = y;
	return true;
}

void SendObjectInfo() {
	int retval = send(sock, (char*)&csMouse, sizeof(csMouse), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
}

void RecvObjectInfo() {

}