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
<<<<<<< HEAD
=======
#include "Network.h"
>>>>>>> bd35982f805b508162779f5d003104e66d415c7e

using namespace std;

HINSTANCE g_hInst;
<<<<<<< HEAD
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Term Project";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

=======
LPCTSTR lpszClass = "Window Class Name";
LPCTSTR lpszWindowName = "Term Project";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

//#define NETWORK  //네트워크 기능 켜기

>>>>>>> bd35982f805b508162779f5d003104e66d415c7e
// 시간을 구하기 위한 변수들
LARGE_INTEGER g_tSecond;
LARGE_INTEGER g_tTime;
float         g_fDeltaTime;

<<<<<<< HEAD


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
=======
int stage = 0;  //0이명 로그인 화면
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
>>>>>>> bd35982f805b508162779f5d003104e66d415c7e

};

struct Bullet {
<<<<<<< HEAD
    float x = 0;
    float y = 0;

    int imageSize =0;


    float dx = 3;
    float dy = 3;

    int dir = 1;

    int type = 0;
    int imageCount = 0;
    int anim = 0;
    bool isColl = false;

    Bullet(int m_type,int m_x, int m_y)
    {
      x = m_x;
      y = m_y;
      type = m_type;
      anim = 0;
      if(type == 0)
          imageCount = 2;
      else if(type == 1)
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
void PlayerMove(float , float);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    static HDC hdc, memdc1, memdc2;
    HBITMAP hBitmap1, hBitmap2;
    HBRUSH hBrush, oldBrush;

    // Player 위치, 애니메이션, 크기, 칸수 등등


    // Map

    static MAP m_map[10];
    static MAP m_static_map[100];
    static MAP m_button[2];     
    static int m_count = 0;
    static int m_max_count = 5;
    // 충돌 박스 
    static RECT p_rect;

    static Monster m_monster[2];

    static vector<Bullet> vec_bullet;

    // 물리
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
    switch (uMsg) {

    case WM_CREATE:

       // QueryPerformanceCounter(&tTime);

        //콘솔 
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


        // 여기 부분에 추가.
        m_static_map[0].x = 200;
        m_static_map[0].y = 150;

        m_static_map[1].x = 1100;
        m_static_map[1].y = 150;

        m_static_map[2].x = 500;
        m_static_map[2].y = 150;
        block_count = 3;

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

            //하강 시작 && 발판 착지
            if (velY > 0 || isRanding) {
                if (velY > 0) p.state = PLAYER::FALL;
                int check = 0;

                int b_check = 0;
                for (int i = 0; i < 2; ++i) {
                    // 플레이어 버른 누름
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
                    // 안누름
                    else {
                        m_button[i].state = 0;
                    }



                    //서버 코드에서 버튼 state가 둘다 1 일때 다음 스테이지.

                    for(int i = 0; i < 2; ++i)
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

                    // 중력가속영향으로 vely값이 커져서 발판 통과하는걸 막아줌.
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
                        p.x < m_static_map[i].x + 48 && p.x > m_static_map[i].x - 48 && i <2)
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

           // 땅 착지     
            if (p.y > 780) {
                p.state = PLAYER::IDLE;
                accY = 0;
                velY = 0;
                PLAYER_GRAVITY = 0;
                p.y = 780;
                jumpCount = 0;
            }
            

            for (int i = 0; i < vec_bullet.size(); ++i) {
                if(vec_bullet[i].Coll() == false)
                    vec_bullet[i].Move();

                if (vec_bullet[i].Coll() && vec_bullet[i].isColl == false) {
                    vec_bullet[i].isColl = true;
                    vec_bullet[i].anim = 0;
                    vec_bullet[i].imageCount = 6;
                }
            }


            // 0번 기능 - 키보드 입력에 따른 기능
            if (GetAsyncKeyState(VK_LEFT) & 0x8000)
            {
                p.state = PLAYER::MOVE;
                p.dir = P_DIR_LEFT;
                p.x -= (p.speed * 0.016f);
                velX -= 10;
            }

            if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
            {
                p.state = PLAYER::MOVE;
                p.dir = P_DIR_RIGHT;
                p.x += (p.speed * 0.016f);
                velX = 10;
            }

            ///////////////////// 이미지 애니메이션///////////////////////////////
            //Player 애니메이션
            p.anim += P_IMAGE_SIZE;
            if (p.anim >= P_IMAGE_SIZE * (p.imageCount - 1))
                p.anim = 0;

            //Monster 애니메이션
            m_monster[0].anim += 32;
            if (m_monster[0].anim >= 32 * (m_monster[0].imageCount - 1))
            {
                if (m_monster[0].attack)
                {
                    //총알 생성
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

            //Bullet 애니메이션
            for (int i = 0; i < vec_bullet.size(); ++i) {
                vec_bullet[i].anim += 64;
                cout << vec_bullet[i].anim << endl;

                if (vec_bullet[i].anim >= 64 * (vec_bullet[i].imageCount - 1)) {
                    vec_bullet[i].anim = 0;

                    if (vec_bullet[i].isColl)
                        vec_bullet.erase(vec_bullet.begin()+i);
                }

            }
            break;
        }
        

        InvalidateRect(hWnd, NULL, false);
        break;

    case WM_KEYDOWN:
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
    case WM_KEYUP:
        p.state = PLAYER::IDLE;
        p.speed = 300.f;
        break;
    case WM_LBUTTONDOWN:
=======
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

	// Player 위치, 애니메이션, 크기, 칸수 등등


	// Map

	static MAP m_map[10];
	static MAP m_static_map[100];
	static MAP m_button[2];
	static int m_count = 0;
	static int m_max_count = 5;
	// 충돌 박스 
	static RECT p_rect;

	static Monster m_monster[2];

	static vector<Bullet> vec_bullet;

	// 물리
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

	RECT               rect = { 675, 450, 825, 465 }; // 글을 쓸 공간 지정


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
		p.x = 230;
		p.y = 650;

		m_button[0].x = 200;
		m_button[0].y = 120;

		m_button[1].x = 1100;
		m_button[1].y = 120;


		// 여기 부분에 추가.
		m_static_map[0].x = 200;
		m_static_map[0].y = 150;

		m_static_map[1].x = 1100;
		m_static_map[1].y = 150;

		m_static_map[2].x = 500;
		m_static_map[2].y = 150;
		block_count = 3;

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

			//하강 시작 && 발판 착지
			if (velY > 0 || isRanding) {
				if (velY > 0) p.state = PLAYER::FALL;
				int check = 0;

				int b_check = 0;
				for (int i = 0; i < 2; ++i) {
					// 플레이어 버른 누름
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
					// 안누름
					else {
						m_button[i].state = 0;
					}



					//서버 코드에서 버튼 state가 둘다 1 일때 다음 스테이지.

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

					// 중력가속영향으로 vely값이 커져서 발판 통과하는걸 막아줌.
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

			// 땅 착지     
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


			// 0번 기능 - 키보드 입력에 따른 기능
			if (stage != 0) {
				if (GetAsyncKeyState(VK_LEFT) & 0x8000)
				{
					p.state = PLAYER::MOVE;
					p.dir = P_DIR_LEFT;
					p.x -= (p.speed * 0.016f);
					velX -= 10;
				}

				if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
				{
					p.state = PLAYER::MOVE;
					p.dir = P_DIR_RIGHT;
					p.x += (p.speed * 0.016f);
					velX = 10;
				}
			}
			///////////////////// 이미지 애니메이션///////////////////////////////
			//Player 애니메이션
			p.anim += P_IMAGE_SIZE;
			if (p.anim >= P_IMAGE_SIZE * (p.imageCount - 1))
				p.anim = 0;

			//Monster 애니메이션
			m_monster[0].anim += 32;
			if (m_monster[0].anim >= 32 * (m_monster[0].imageCount - 1))
			{
				if (m_monster[0].attack)
				{
					//총알 생성
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

			//Bullet 애니메이션
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
>>>>>>> bd35982f805b508162779f5d003104e66d415c7e
		if (m_count != m_max_count) {
			m_map[m_count].x = LOWORD(lParam);
			m_map[m_count].y = HIWORD(lParam);
			m_count++;
		}
		InvalidateRect(hWnd, NULL, false);
<<<<<<< HEAD
        break;

    case WM_PAINT:

        hdc = BeginPaint(hWnd, &ps);

        memdc1 = CreateCompatibleDC(hdc);
        hBitmap1 = CreateCompatibleBitmap(hdc, 11500, Window_Size_Y);
        SelectObject(memdc1, hBitmap1);

        // 1. 배경 그리기
        img_bg.Draw(memdc1, 0, 0, Window_Size_X, Window_Size_Y, 0, 0, img_bg_width, img_bg_height);


        // 2. 발판 그리기
        for(int i = 0 ; i < m_count; ++i)
            img_wall.Draw(memdc1, m_map[i].x - 48, m_map[i].y- 16, 96, 32);


        for (int i = 0; i < block_count; ++i)
            img_wall.Draw(memdc1, m_static_map[i].x - 48, m_static_map[i].y - 16, 96, 32);


        for (int i = 0; i < 2; ++i) {
            if(m_button[i].state ==0 )
                img_ButtonUp.Draw(memdc1, m_button[i].x - 32, m_button[i].y - 16, 64, 32);
            else {
                img_ButtonDown.Draw(memdc1, m_button[i].x - 32, m_button[i].y - 16, 64, 32);
            }
        }
        //ButtonBlue.Draw(memdc1, 200, 200, 64, 32);


        if (m_monster[0].attack == false) {
            //몬스터 타입에 따른 렌더링 변경 총알 or 폭탄 몬스터
            if (m_monster[0].type == 0) 
                img_Bomb_Monster_Idle.Draw(memdc1, m_monster[0].x - 16, m_monster[0].y - 16, 32, 32, m_monster[0].anim, 0, 32, 32);
            else
                img_Bomb_Monster_Idle.Draw(memdc1, m_monster[0].x - 16, m_monster[0].y - 16, 32, 32, m_monster[0].anim, 0, 32, 32);

        }
        else {
            //몬스터 타입에 따른 렌더링 변경 총알 or 폭탄 몬스터
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
 

        // 4. 플레이어 그리기
        switch (p.state)
        {
        case PLAYER::IDLE:
            p.imageCount = 11;
            player = frog_idle;
            player.Draw(memdc1, p.x -16, p.y-16, 32, 32, p.anim, p.dir, 32, 32);
            break;

        case PLAYER::MOVE:
            p.imageCount = 12;
            player = frog_move;
            player.Draw(memdc1, p.x -16 , p.y - 16 , 32, 32, p.anim, p.dir, 32, 32);
            break;

        case PLAYER::JUMP:
            player = frog_jump;
            player.Draw(memdc1, p.x -16, p.y -16, 32, 32, 0, p.dir, 32, 32);
            break;
        case PLAYER::FALL:
            player = frog_fall;
            player.Draw(memdc1, p.x - 16 , p.y - 16, 32, 32, 0, p.dir, 32, 32);
            break;
        case PLAYER::ATTACK:
            p.imageCount = 7;
            player = frog_attack;
            player.Draw(memdc1, p.x - 16, p.y - 16, 32, 32, p.anim, 0, 32, 32);
            break;
        case PLAYER::DEAD:
            break;
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
=======
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
			// 2. 발판 그리기
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
				//몬스터 타입에 따른 렌더링 변경 총알 or 폭탄 몬스터
				if (m_monster[0].type == 0)
					img_Bomb_Monster_Idle.Draw(memdc1, m_monster[0].x - 16, m_monster[0].y - 16, 32, 32, m_monster[0].anim, 0, 32, 32);
				else
					img_Bomb_Monster_Idle.Draw(memdc1, m_monster[0].x - 16, m_monster[0].y - 16, 32, 32, m_monster[0].anim, 0, 32, 32);

			}
			else {
				//몬스터 타입에 따른 렌더링 변경 총알 or 폭탄 몬스터
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


			// 4. 플레이어 그리기
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
>>>>>>> bd35982f805b508162779f5d003104e66d415c7e
}

void LoadImage()
{
<<<<<<< HEAD
    //버튼
    img_ButtonUp.Load(L"button_up.png");
    img_ButtonDown.Load(L"button_down.png");

    //총알
    img_Bullet.Load(L"bullet.png");
    img_Bullet_Coll.Load(L"bullet_coll.png");
    img_Bomb.Load(L"bomb.png");
    img_Bomb_Coll.Load(L"bomb_coll.png");


    //배경
    img_bg.Load(L"background_grass.png");
    img_bg_width = img_bg.GetWidth();
    img_bg_height = img_bg.GetHeight();
    
    //발판
    img_wall.Load(L"block_grass.png");

    //몬스터
    img_Bomb_Monster_Attack.Load(L"monster_bomb1.png");
    img_Bomb_Monster_Idle.Load(L"monster_bomb_idle.png");

    //img_Bullet_Monster_Attack.Load();
    //img_Bullet_Monster_Idle.Load();



    //플레이어
    frog_idle.Load(L"frog_idle.png");
    frog_move.Load(L"frog_move.png");
    frog_jump.Load(L"frog_jump.png");
    frog_fall.Load(L"frog_fall.png");
    frog_attack.Load(L"frog_attack.png");
=======
	//id입력창
	img_Login.Load("white.png");
	//버튼
	img_ButtonUp.Load("button_up.png");
	img_ButtonDown.Load("button_down.png");

	//총알
	img_Bullet.Load("bullet.png");
	img_Bullet_Coll.Load("bullet_coll.png");
	img_Bomb.Load("bomb.png");
	img_Bomb_Coll.Load("bomb_coll.png");


	//배경
	img_bg.Load("background_grass.png");
	img_bg_width = img_bg.GetWidth();
	img_bg_height = img_bg.GetHeight();

	//발판
	img_wall.Load("block_grass.png");

	//몬스터
	img_Bomb_Monster_Attack.Load("monster_bomb1.png");
	img_Bomb_Monster_Idle.Load("monster_bomb_idle.png");

	//img_Bullet_Monster_Attack.Load();
	//img_Bullet_Monster_Idle.Load();



	//플레이어
	frog_idle.Load("frog_idle.png");
	frog_move.Load("frog_move.png");
	frog_jump.Load("frog_jump.png");
	frog_fall.Load("frog_fall.png");
	frog_attack.Load("frog_attack.png");
>>>>>>> bd35982f805b508162779f5d003104e66d415c7e

}

bool CollisionHelper(RECT r1, RECT r2)
{
<<<<<<< HEAD
    if (r1.left > r2.right) return false;
    if (r1.top > r2.bottom) return false;
    if (r1.right < r2.left) return false;
    if (r1.bottom < r2.top) return false;

    return true;
}


=======
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
>>>>>>> bd35982f805b508162779f5d003104e66d415c7e
