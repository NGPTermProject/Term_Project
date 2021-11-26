//#include "Server.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma once
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32")
#pragma comment(lib,"winmm")
#include <vector>
#include "Object.h"
#include "protocol.h"
#include <iostream>
using namespace std;


//Server server;

void err_display(int err_num);
void err_quit(const char* msg);
int recvn(SOCKET s, char* buf, int len, int flags);
DWORD WINAPI Client_Thread(LPVOID arg);
vector<Map> m_map;
vector<Map> m_static_map;
Map m_button[2];
vector<Player> player;
vector<Monster> m_monster;
Bullet m_bullet[20];

vector<Obstacle> m_obstacle;

HANDLE hThread;

int Client_Count = -1;
sc_send_player_id p_id;
sc_send_player sc_p[2];
sc_obstacle sc_obs[2];
sc_bullet bullet[20];
sc_recv_keyinfo keyinfo;
CRITICAL_SECTION cs;
short client_id;
bool c_left[2];
bool c_right[2];
sc_put_object put[2];
sc_button sc_b;

int main()
{

	//server.InitServer();
	player.push_back(Player(200, 600, 0));
	player.push_back(Player(400 , 600, 1));
	m_static_map.push_back(Map(MAP::BUTTON, 200, 120));
	m_static_map.push_back(Map(MAP::BUTTON, 1100, 120));
	m_static_map.push_back(Map(MAP::PLAT, 200, 150));
	m_static_map.push_back(Map(MAP::PLAT, 1100, 150));
	m_static_map.push_back(Map(MAP::PLAT, 500, 150));


	m_obstacle.push_back(Obstacle(OBSTACLE::BLADE, 100, 500));
	m_obstacle.push_back(Obstacle(OBSTACLE::BLADE, 300, 500));
	sc_obs[0].x = 100;
	sc_obs[0].y = 500;
	sc_obs[1].x = 300;
	sc_obs[1].y = 500;


	m_monster.push_back(Monster(MONSTER::PLANT, 200, 100 ,100));
	m_monster.push_back(Monster(MONSTER::PIG, 500, 100 , 350));



	WSADATA wsa;
	SOCKADDR_IN sock_addr;
	if (WSAStartup(MAKEWORD(2, 2), &wsa))
		return -1;

	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(SERVER_PORT);
	sock_addr.sin_addr.s_addr = INADDR_ANY;

	int ret = bind(server_socket, reinterpret_cast<sockaddr*>(&sock_addr), sizeof(sock_addr));
	if (SOCKET_ERROR == ret)
	{
		std::cout << "bind Error\n";
		int err_num = WSAGetLastError();
		err_display(err_num);
	}
	ret = listen(server_socket, SOMAXCONN);
	if (SOCKET_ERROR == ret) {
		std::cout << "listen Error\n";
		int err_num = WSAGetLastError();
		err_display(err_num);
	}

	InitializeCriticalSection(&cs);

	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	
	bool flag = TRUE;
	
	while (1)
	{
		//accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(server_socket, (SOCKADDR*)&clientaddr, &addrlen);
		setsockopt(client_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));

		if (client_sock == INVALID_SOCKET) {
			//err_display("accept()");
			std::cout << "!!!" << std::endl;
		}
		printf("Connected Client IP : %s\n", inet_ntoa(clientaddr.sin_addr));

	
		Client_Count++;

		hThread = CreateThread(NULL, 0, Client_Thread, (LPVOID)client_sock, 0, NULL);//HandleClient 쓰레드 실행, clientSock을 매개변수로 전달

	}
	DeleteCriticalSection(&cs);
	
	closesocket(server_socket);
	
	WSACleanup();
	
	return 0;
}


DWORD WINAPI Client_Thread(LPVOID arg)
{
	SOCKET clientSock = (SOCKET)arg;

	int retval;

	// hero.id 송신
	//p_id.id = 
	p_id.id = Client_Count;
	cout << Client_Count << endl;
	send(clientSock, (char*)&p_id, sizeof(sc_send_player_id), 0);


	
	while (1) {
	    int StartTime = (int)GetTickCount64();
		while ((GetTickCount64() - StartTime) <= 10){ }
		{
			recvn(clientSock, (char*)&keyinfo, sizeof(keyinfo), 0);
			if (keyinfo.isClick) {
				m_map.push_back(Map(MAP::PLAT, keyinfo.x, keyinfo.y));
				for (int i = 0; i < 2; ++i) {
					put[i].isClick = true;
					put[i].x = keyinfo.x;
					put[i].y = keyinfo.y;
				}
			}

			EnterCriticalSection(&cs);
			client_id = keyinfo.id;
			if (client_id == keyinfo.id) {
				c_left[client_id] = keyinfo.left;
				c_right[client_id] = keyinfo.right;
			}
			LeaveCriticalSection(&cs);

			player[client_id].UpdateGravity();

			//플레이어 점프 상태에서 충돌 처리 부분
			if (player[client_id].getVely() > 0 || player[client_id].getisRanding()) {
				if (player[client_id].getVely() > 0) player[client_id].SwitchState(PLAYER::FALL);
				int check = 0;

				for (int i = 0; i < m_map.size(); ++i) {
					if (player[client_id].getVely() > 600) player[client_id].setCollisonHelperY(8);
					else player[client_id].setCollisonHelperY(0);

					if (player[client_id].FallingCollsionOtherObject(m_map[i]))
					{
						player[client_id].setPlayerRanding(m_map[i].y - 32);
						check++;
					}
				}

				for (int i = 0; i < 2; ++i) {
					// 플레이어 버른 누름
					if (player[client_id].FallingCollsionOtherObject(m_static_map[i]))
					{
						player[client_id].setPlayerRanding(m_static_map[i].y - 16);
						m_static_map[i].setState(true);

						if (put[client_id].isPush[i] == put[(client_id + 1) % 2].isPush[i]) {
							EnterCriticalSection(&cs);
							put[client_id].isPush[i] = true;
							put[(client_id + 1) % 2].isPush[i] = true;
							LeaveCriticalSection(&cs);
							//다음스테이지
							if (put[client_id].isPush[(i + 1) % 2] || put[(client_id + 1) % 2].isPush[(i + 1) % 2])
								cout << "NextStage" << endl;
						}
						check++;
					}
				}


				for (int i = 2; i < m_static_map.size(); ++i) {
					if (player[client_id].getVely() > 600) player[client_id].setCollisonHelperY(8);
					else player[client_id].setCollisonHelperY(0);

					if (player[client_id].FallingCollsionOtherObject(m_static_map[i]))
					{
						player[client_id].setPlayerRanding(m_static_map[i].y - 32);
						check++;
					}
				}
				if (check == 0) {
					player[client_id].setGravity();
				}
				else check = 0;
			}

			// 땅 착지     
			if (player[client_id].getPos().y > 780) {
				player[client_id].setPlayerRanding(780);
			}


			// --키입력------------------------------------------///
			if (keyinfo.jump == true) {
				player[client_id].Jump();
			}
			if (c_left[client_id] == true) {
				if (player[client_id].getVely() == 0)
					player[client_id].SwitchState(PLAYER::MOVE);
				player[client_id].setDir(32);
				player[client_id].move(-(300 * 0.016f));
			}

			if (c_right[client_id] == true) {
				if (player[client_id].getVely() == 0)
					player[client_id].SwitchState(PLAYER::MOVE);
				player[client_id].setDir(0);
				player[client_id].move((300 * 0.016f));
			}
			///-------------------------------------------------///

			for (int i = 0; i < m_obstacle.size(); ++i) {
				if (m_obstacle[i].type == OBSTACLE::BLADE) {
					EnterCriticalSection(&cs);
					m_obstacle[i].Move();
					sc_obs[i].x = m_obstacle[i].getPosX();
					sc_obs[i].y = m_obstacle[i].getPosY();
					LeaveCriticalSection(&cs);
				}
				if (player[client_id].CollsionByObstacle(m_obstacle[i])) {
					EnterCriticalSection(&cs);
					put[client_id].clear = true;
					put[(client_id + 1) % 2].clear = true;
					m_map.clear();
					player[0].setStartLine(200, 600);
					player[1].setStartLine(400, 600);
					LeaveCriticalSection(&cs);
				}
			}
			for (int i = 0; i < m_monster.size(); ++i) {
				EnterCriticalSection(&cs);
				m_monster[i].Update();
				LeaveCriticalSection(&cs);

				if (m_monster[i].getisAttack()) {
					EnterCriticalSection(&cs);
					put[0].AttackMonsterId = i;
					put[1].AttackMonsterId = i;
					m_bullet[i].InitBullet(m_monster[i]);
					m_monster[i].setisAttack(false);
					LeaveCriticalSection(&cs);

				}
			}

			for (int i = 0; i < 15; ++i) {
				//충돌 후 터지는 애니메이션 재생
				if (m_bullet[i].isColl && m_bullet[i].anim >= m_bullet[i].anim_max) {
					EnterCriticalSection(&cs);
					m_bullet[i].DeadAnimation(m_monster[i]);
					LeaveCriticalSection(&cs);

				}
				//플레이어와 충돌
				if (player[client_id].CollsionByObstacle(m_bullet[i])&&m_bullet[i].isColl == false) {
					EnterCriticalSection(&cs);
					put[client_id].clear = true;
					put[(client_id + 1) % 2].clear = true;
					m_map.clear();
					m_bullet[i].setisColl(true);
					player[0].setStartLine(200, 600);
					player[1].setStartLine(400, 600);
					LeaveCriticalSection(&cs);
				}
				//총알 생성 후 이동 & 애니메이션
				if (m_bullet[i].isStart) {
					EnterCriticalSection(&cs);
					m_bullet[i].animation();
					m_bullet[i].Update();
					LeaveCriticalSection(&cs);
				}
				// 클라이언트에 전달하기 위한 패킷에 총알 정보 전달.
				EnterCriticalSection(&cs);
				m_bullet[i].SetBulletInfo(bullet[i]);
				LeaveCriticalSection(&cs);
			}

			//클라이언트에 전달하기 위한 패킷에 플레이어 정보 전달.
			player[client_id].setPlayerInfo(sc_p[client_id]);

			send(clientSock, (char*)&sc_p, sizeof(sc_p), 0);
			send(clientSock, (char*)&sc_obs, sizeof(sc_obs), 0);
			send(clientSock, (char*)&put[client_id], sizeof(put[client_id]), 0);
			send(clientSock, (char*)&bullet, sizeof(bullet), 0);

			//초기화
			for (int i = 0; i < 2; ++i) {
				put[client_id].isPush[i] = false;

			}
			put[client_id].isClick = false;
			put[client_id].AttackMonsterId = -1;
			put[client_id].clear = false;

		}
		
	}
	return 0;
}
void err_display(int err_num)
{
	std::wcout.imbue(std::locale("Korean"));
	WCHAR* lpmsgbuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_num,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpmsgbuf, 0, 0

	);
	std::wcout << lpmsgbuf << std::endl;
	while (true);
	LocalFree(lpmsgbuf);
}

void err_quit(const char* msg)
{
	LPVOID lpmsgbuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpmsgbuf, 0, NULL
	);
	MessageBox(NULL, (LPCTSTR)lpmsgbuf,(LPCWSTR)msg, MB_ICONERROR);
	LocalFree(lpmsgbuf);
	exit(-1);
}

int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR) {
			return SOCKET_ERROR;
		}
		else if (received == 0)
			break;

		left -= received;
		ptr += received;
	}

	return (len - left);

}