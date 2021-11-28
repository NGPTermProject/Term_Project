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
vector<Bullet> vec_bullet;
<<<<<<< HEAD
=======

>>>>>>> parent of c122221 (push_bullet)
vector<Obstacle> m_obstacle;
vector<SOCKET> matching_queue;

HANDLE hThread;

int Client_Count = -1;
sc_send_player_id p_id;
sc_send_player sc_p[2];
sc_obstacle sc_obs[2];

sc_recv_keyinfo keyinfo;
CRITICAL_SECTION cs;
short client_id;
bool c_left[2];
bool c_right[2];
sc_put_object put[2];
sc_button sc_b;
<<<<<<< Updated upstream
=======


void send_login_ok(SOCKET s, short id)
{
	sc_login_ok packet;
	packet.size = sizeof(packet);
	packet.packet_type = SC_PACKET_LOGIN_OK;
	packet.id = id;

	send(s, reinterpret_cast<char*>(&packet), sizeof(packet), 0);

}

>>>>>>> Stashed changes
int main()
{

	//server.InitServer();
	player.push_back(Player(200, 600, 0));
	player.push_back(Player(400, 600, 1));
	m_static_map.push_back(Map(MAP::BUTTON, 48, 344));
	m_static_map.push_back(Map(MAP::BUTTON, 1392, 344));
	m_static_map.push_back(Map(MAP::PLAT, 48, 374));
	m_static_map.push_back(Map(MAP::PLAT, 1392, 374));
	m_static_map.push_back(Map(MAP::PLAT, 48, 566));
	m_static_map.push_back(Map(MAP::PLAT, 432, 214));
	m_static_map.push_back(Map(MAP::PLAT, 1008, 214));


	m_obstacle.push_back(Obstacle(OBSTACLE::BLADE, 100, 500));
	m_obstacle.push_back(Obstacle(OBSTACLE::BLADE, 300, 500));
	sc_obs[0].x = 100;
	sc_obs[0].y = 500;
	sc_obs[1].x = 300;
	sc_obs[1].y = 500;


	m_monster.push_back(Monster(MONSTER::PLANT, 1392, 780));
	m_monster.push_back(Monster(MONSTER::PIG, 48, 536));
	m_monster.push_back(Monster(MONSTER::PIG, 432, 184));
	m_monster.push_back(Monster(MONSTER::PIG, 1008, 184));



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

<<<<<<< Updated upstream

		Client_Count++;
=======
		matching_queue.push_back(client_sock);

		cs_packet_login packet;
		recvn(client_sock, reinterpret_cast<char*>(&packet),sizeof(packet), 0);
		cout <<"클라이언트에서 보낸 패킷 : " << static_cast<int>(packet.packet_type) << endl;
>>>>>>> Stashed changes

		if (packet.packet_type == CS_PACKET_LOGIN) {
			Client_Count++;
			if (matching_queue.size() == 2) {
				for (int i = 0; i < 2; ++i) {
					send_login_ok(matching_queue[i], Client_Count);

					cout << "recvn login packet" << endl;

					hThread = CreateThread(NULL, 0, Client_Thread, (LPVOID)client_sock, 0, NULL);//HandleClient 쓰레드 실행, clientSock을 매개변수로 전달
					if (hThread == NULL) { closesocket(client_sock); }
				}
			}
			else {
				for (int i = 0; i < matching_queue.size(); ++i)
					send_login_ok(matching_queue[i], Client_Count);
			}
		}
	
		

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


	int StartTime = GetTickCount64();
	int deltaTime;

	//timeGetTime();
	while (1) {
		int NowTime = GetTickCount64();

		deltaTime = NowTime - StartTime;

		if (deltaTime >= 10) {
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

			//EnterCriticalSection(&cs);
			//cout << "?" << endl;

			//for(int i = 0; i < m_obstacle.size(); ++i)
			//	m_obstacle[i].Move();

			//if (keyinfo.jump) {
			//	for (int i = 0; i < 2; ++i)
			//		m_monster[i].Attack();
			//}

			//for (int i = 0; i < vec_bullet.size(); ++i) {
			//	vec_bullet[i].Update();
			//	if (player[Client_Count].CollsionByObstacle(vec_bullet[i]) && vec_bullet[i].getisColl() != true) {
			//		vec_bullet[i].setisColl(true);
			//		m_map.clear();
			//	}
			//}
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
					LeaveCriticalSection(&cs);
					sc_obs[i].x = m_obstacle[i].getPosX();
					sc_obs[i].y = m_obstacle[i].getPosY();
				}
			}
<<<<<<< HEAD

=======
			for (int i = 0; i < m_monster.size(); ++i) {
				EnterCriticalSection(&cs);
				m_monster[i].Update();
				LeaveCriticalSection(&cs);

				if (m_monster[i].getisAttack()) {
					put[0].AttackMonsterId = i;
					put[1].AttackMonsterId = i;
					vec_bullet.push_back(m_monster[i].Attack());
					//sc_obstacle s;
					//bullet.push_back(s);
					m_monster[i].setisAttack(false);
				}
			}


			//for (int i = 0; i < vec_bullet.size(); ++i) {
			//	if (vec_bullet[i].isColl && vec_bullet[i].anim == 0) {
			//		EnterCriticalSection(&cs);
			//		vec_bullet.erase(vec_bullet.begin() + i);
			//		LeaveCriticalSection(&cs);

			//	}
			//}
			//EnterCriticalSection(&cs);
			//put[0].bulletsize = vec_bullet.size();
			//put[1].bulletsize = vec_bullet.size();
			//LeaveCriticalSection(&cs);
			//for (int i = 0; i < vec_bullet.size(); ++i) {			

			//	//if (player[Client_Count].CollsionByObstacle(vec_bullet[i]) && vec_bullet[i].getisColl() != true) {
			//	if (vec_bullet[i].getisColl()) {
			//		EnterCriticalSection(&cs);
			//		
			//		bullet[i].isColl = true;
			//		//vec_bullet.erase(vec_bullet.begin() + i);
			//		
			//		LeaveCriticalSection(&cs);				
			//	}

			//	EnterCriticalSection(&cs);

			//	vec_bullet[i].animation();
			//	vec_bullet[i].Update();

			//	bullet[i].x = vec_bullet[i].x;
			//	bullet[i].y = vec_bullet[i].y;
			//	bullet[i].type = vec_bullet[i].type;
			//	bullet[i].imageCount = vec_bullet[i].imageCount;
			//	bullet[i].imageSizeX = vec_bullet[i].imageSizeX;
			//	bullet[i].imageSizeY = vec_bullet[i].imageSizeY;
			//	bullet[i].anim = vec_bullet[i].anim;

			//	LeaveCriticalSection(&cs);
			//	//	vec_bullet[i].setisColl(true);
			//	//	m_map.clear();
			//	//}

			//}

			//if (vec_bullet[i].isColl && vec_bullet[i].anim == 0)
			//	vec_bullet.erase(vec_bullet.begin() + i);
>>>>>>> parent of c122221 (push_bullet)


			sc_p[client_id].id = client_id;
			sc_p[client_id].state = player[client_id].getState();
			sc_p[client_id].x = player[client_id].getPos().x;
			sc_p[client_id].y = player[client_id].getPos().y;
			sc_p[client_id].dir = player[client_id].getDir();
			sc_p[client_id].jumpCount = player[client_id].getJumpCount();


			//int b_check = 0;
			//for (int i = 0; i < 2; ++i) {
			//	if (put[client_id].isPush[i]) b_check++;
			//	if (put[(client_id + 1) % 2].isPush[i]) b_check++;

			//}
			//if (b_check == 4) {
			//	cout << "Next Stage" << endl;
			//}
			//else {
			//	b_check = 0;
			//	cout << b_check << endl;
			//}

			send(clientSock, (char*)&sc_p, sizeof(sc_p), 0);
			send(clientSock, (char*)&sc_obs, sizeof(sc_obs), 0);
			send(clientSock, (char*)&put[client_id], sizeof(put[client_id]), 0);

			for (int i = 0; i < 2; ++i) {
				put[client_id].isPush[i] = false;

			}
			put[client_id].isClick = false;

		}
		StartTime = NowTime;
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
	MessageBox(NULL, (LPCTSTR)lpmsgbuf, (LPCWSTR)msg, MB_ICONERROR);
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