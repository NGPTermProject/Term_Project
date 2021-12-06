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
int MyId;
int hostId;
//////////////////////////
// 로그인 화면에서 키보드 입력하면 id창에 입력
// id창에 글자가 있을때 엔터 누르면(서버에 송신) 게임 시작
//////////////////////////
vector<Player> p;

bool InitClient();
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

int button = -1;
bool buttonCheck[2];
bool isClick;
cs_packet_login login;
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

    hostId = -1;

    while (1) {
        //PlayerID 전달.
        recvn(sock, (char*)&id, sizeof(cs_send_player_id), 0);
        if (id.id == 1) {
            EnterOtherPlayer = true;
            break;
        }
        cout << id.id << endl;
        hostId = id.id;
        keyinfo.id = id.id;
        login_button.id = hostId;
    }


    if (hostId == 0) {
        cout << "호스트입니당" << endl;
        hostId = 0;
        keyinfo.id = 0;
        login_button.id = 0;
    }
    if (hostId == -1) {
        cout << "팀원입니당" << endl;
        hostId = 1;
        keyinfo.id = 1;
        login_button.id = 1;
    }
    cout << hostId << endl;

    cout << "리시브 받기전" << endl;
    recvn(sock, (char*)&start_game_info, sizeof(cs_start_game), 0);
    cout << "게임시작" << endl;
    if (start_game_info.gamestart) {
        cout << "게임시작" << endl;
        stage = start_game_info.stage;
        isConnect = true;
    }
    else {
        cout << "오류 오류 큰일 큰일" << endl;
    }


    //------------------------------//


    int StartTime;
    while (1) {
        StartTime = GetTickCount64();
        while (GetTickCount64() - StartTime <= 10) {}
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
                        m_obstacle[FirstObstacleSize + i].x = cs_obs[i].x;
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
                for (int i = FirstMapSize; i < FirstMapSize + 2; ++i)
                    m_static_map[i].setState(put.isPush[i % 2]);
            }
            if (stage == 3) {
                for (int i = SecondMapSize; i < SecondMapSize + 2; ++i)
                    m_static_map[i].setState(put.isPush[i % 2]);
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

            for (int i = 0; i < 15; ++i) {
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



    // 충돌 박스 

    //playsoun

    // 물리
    static LARGE_INTEGER tTime;
    static int block_count;

    static HBITMAP buttonBitmap;
    static HWND Button1;
    static HWND Button2;
    RECT rect = { 675, 450, 825, 465 }; // 글을 쓸 공간 지정


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
                cout << "?" << endl;

#endif
                //stage = 1;
                //send()
                WideCharToMultiByte(CP_ACP, 0, str, str_len, login.name, 256, NULL, NULL);
                login.id = MyId;
                //send(sock, (char*)&login, sizeof(login), 0);
                //recvn()
            }
        }
        else
        {
            // WM_CHAR 메시지는 입력된 문자를 wParam으로 전달한다.
            str[str_len] = (TCHAR)wParam;
            str[str_len + 1] = 0;
            InvalidateRect(hWnd, NULL, false);

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

        M_LoadImage();


        //Button2 = CreateWindow(L"button", L"PressToPlay",
        //   WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
        //   885, 350, 200, 43, hWnd, (HMENU)2, g_hInst, NULL);
        //SendMessage(Button2, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)buttonBitmap);


        p.push_back(Player(200, 600, 0));
        p.push_back(Player(400, 600, 1));



        // 여기 부분에 추가.


        //m_static_map.push_back(Map(MAP::BUTTON, 200, 120));
        //m_static_map.push_back(Map(MAP::BUTTON, 1100, 120));
        //m_static_map.push_back(Map(MAP::PLAT, 200, 150));
        //m_static_map.push_back(Map(MAP::PLAT, 1100, 150));
        //m_static_map.push_back(Map(MAP::PLAT, 500, 150));

        //m_obstacle.push_back(Obstacle(OBSTACLE::BLADE, 100, 500));
        //m_obstacle.push_back(Obstacle(OBSTACLE::BLADE, 300, 500));

        //m_monster.push_back(Monster(MONSTER::PLANT, 200, 100));
        //m_monster.push_back(Monster(MONSTER::PIG, 500, 100));


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
        m_monster.push_back(Monster(MONSTER::TREE, 1392, 404));
        m_monster.push_back(Monster(MONSTER::RPLANT, 336, 467));
        m_monster.push_back(Monster(MONSTER::PIG, 624, 728));
        m_monster.push_back(Monster(MONSTER::PIG, 816, 280));
        m_monster.push_back(Monster(MONSTER::PIG, 1080, 696));
        m_monster.push_back(Monster(MONSTER::PIG, 1200, 696));


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


        SetTimer(hWnd, 1, 1, NULL);

        //Timer::Reset();
        //SetTimer(hWnd, 1, 10, NULL);
        //
        //InitClient();


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
            //recvn(sock, (char*)&p_info, sizeof(p_info), 0);
            id.id = 3;
            send(sock, (char*)&id, sizeof(cs_send_player_id), 0);
            DestroyWindow(Button1);
            //login_button.buttonid = 0;
            //send(sock, (char*)&button, sizeof(button), 0);
            //recvn(sock, (char*)&buttonCheck, sizeof(buttonCheck), 0);
            //if (!buttonCheck[button]) {
            //   MyId = 0;
            //   cout << "성공" << endl;
            //}
            //else
            //   cout << "이미 먹혓어요 ㅠ" << endl;

            break;


        case 2:
            //login_button.buttonid = 1;
            //send(sock, (char*)&button, sizeof(button), 0);
            //recvn(sock, (char*)&buttonCheck, sizeof(buttonCheck), 0);
            //if (!buttonCheck[button]) {
            //   MyId = 1;
            //   cout << "성공" << endl;
            //}
            //else
            //   cout << "이미 먹혓어요 ㅠ" << endl;

            break;
        }
        break;

    case WM_TIMER:
        switch (wParam) {

        case 1:
            //int StartTime;
            if (hostId == 1) {
                if (Button1)DestroyWindow(Button1);
            }
            //if (isConnect) {
            //   send(sock, (char*)&keyinfo, sizeof(keyinfo), 0);
            //   keyinfo.isClick = false;
            //   keyinfo.jump = false;
            //}
            static bool push = true;

            if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
                if (push) {
                    if (p[hostId].getJumpCount() < 2) {
                        keyinfo.jump = true;
                    }
                    push = false;
                }
            }
            else {
                push = true;
            }
            //if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
            //   keyinfo.left = true;
            //}
            //if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
            //   keyinfo.right = true;
            //}

            //StartTime = GetTickCount64();
            //while (GetTickCount64() - StartTime <= 10) {}
            /*send(sock, (char*)&keyinfo, sizeof(keyinfo), 0);
            keyinfo.isClick = false;*/
            //keyinfo.jump = false;

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
            ////Bullet 애니메이션
            //for (int i = 0; i < vec_bullet.size(); ++i) {

            //   //vec_bullet[i].animation();
            //   if (vec_bullet[i].isColl && vec_bullet[i].anim == 0) {
            //      vec_bullet.erase(vec_bullet.begin() + i);
            //   }
            //
            //}



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
                //case VK_SPACE:
                //   if (p[hostId].getJumpCount() < 2) {
                //      keyinfo.jump = true;
                //   }
                //   break;
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
        //keyinfo.jump = false;
        InvalidateRect(hWnd, NULL, false);

        break;

    case WM_LBUTTONDOWN:
        if (map_current_count != map_max_count) {
            keyinfo.isClick = true;
            keyinfo.x = LOWORD(lParam);
            keyinfo.y = HIWORD(lParam);
            map_current_count++;
            //stage++;
            //m_map.push_back(Map(MAP::PLAT,LOWORD(lParam), HIWORD(lParam)));    
            //InvalidateRect(hWnd, NULL, false);
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
            //img_Login.Draw(memdc1, 675, 320, 150, 15);  //아이디 입력 창


            //RECT rect = { 685, 320, 835, 465 }; // 글을 쓸 공간 지정
            //DrawText(memdc1, str, -1, &rect, DT_VCENTER | DT_WORDBREAK);
            //RECT r = { 590, 320,740,465 };
            //DrawText(memdc1, L"ID: ", -1, &r, DT_CENTER | DT_VCENTER);
            //player_idle.Draw(hdc, pos.x - imageSizeX / 2, pos.y - imageSizeY / 2, imageSizeX, imageSizeY, anim, dir, imageSizeX, imageSizeY);
            img_LodingFrogRed.Draw(memdc1, 460, 570, 64, 64, RedAnim, 0, 64, 64);
            img_LodingFrogBlue.Draw(memdc1, 960, 565, 64, 64, BlueAnim, 64, 64, 64);

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

        BitBlt(hdc, 0, 0, Window_Size_X, Window_Size_Y, memdc1, 0, 0, SRCCOPY);
        //if (stage == 0) //id입력
        //   DrawText(hdc, str, -1, &rect, DT_CENTER | DT_VCENTER);

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

