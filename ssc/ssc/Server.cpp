//#include "Server.h"
//#include "Object.h"
//#include <iostream>
//#include <vector>
//#include "protocol.h"
//#pragma warning(disable: 4996)
//using namespace std;
//
//
//Server::Server()
//{
//
//
//	p.push_back(Player(230, 600));
//	p.push_back(Player(500, 600));
//
//	m_static_map.push_back(Map(MAP::BUTTON, 200, 120));
//	m_static_map.push_back(Map(MAP::BUTTON, 1100, 120));
//	m_static_map.push_back(Map(MAP::PLAT, 200, 150));
//	m_static_map.push_back(Map(MAP::PLAT, 1100, 150));
//	m_static_map.push_back(Map(MAP::PLAT, 500, 150));
//
//
//	m_obstacle.push_back(Obstacle(OBSTACLE::BLADE, 100, 500));
//	m_obstacle.push_back(Obstacle(OBSTACLE::BLADE, 300, 500));
//
//	m_monster.push_back(Monster(MONSTER::PLANT, 200, 100));
//	m_monster.push_back(Monster(MONSTER::PIG, 500, 100));
//	Client_Count = -1;
//}
//
//Server::~Server()
//{
//
//}
//
//void Server::err_display(int err_num)
//{
//	std::wcout.imbue(std::locale("Korean"));
//	WCHAR* lpmsgbuf;
//
//	FormatMessage(
//		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
//		NULL, err_num,
//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//		(LPTSTR)&lpmsgbuf, 0, 0
//
//	);
//	std::wcout << lpmsgbuf << std::endl;
//	while (true);
//	LocalFree(lpmsgbuf);
//}
//
//void Server::err_quit(const char* msg)
//{
//	LPVOID lpmsgbuf;
//
//	FormatMessage(
//		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
//		NULL, WSAGetLastError(),
//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//		(LPTSTR)&lpmsgbuf, 0, NULL
//	);
//	MessageBox(NULL, (LPCTSTR)lpmsgbuf, (LPCSTR)msg, MB_ICONERROR);
//	LocalFree(lpmsgbuf);
//	exit(-1);
//}
//
//int Server::recvn(SOCKET s, char* buf, int len, int flags)
//{
//	int received;
//	char* ptr = buf;
//	int left = len;
//
//	while (left > 0) {
//		received = recv(s, ptr, left, flags);
//		if (received == SOCKET_ERROR) {
//			return SOCKET_ERROR;
//		}
//		else if (received == 0)
//			break;
//
//		left -= received;
//		ptr += received;
//	}
//
//	return (len - left);
//
//}
//
//int Server::InitServer()
//{
//	WSADATA wsa;
//	SOCKADDR_IN sock_addr;
//	if (WSAStartup(MAKEWORD(2, 2), &wsa))
//		return -1;
//
//	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//
//	sock_addr.sin_family = AF_INET;
//	sock_addr.sin_port = htons(SERVER_PORT);
//	sock_addr.sin_addr.s_addr = INADDR_ANY;
//
//	int ret = bind(server_socket, reinterpret_cast<sockaddr*>(&sock_addr), sizeof(sock_addr));
//	if (SOCKET_ERROR == ret)
//	{
//		std::cout << "bind Error\n";
//		int err_num = WSAGetLastError();
//		err_display(err_num);
//	}
//	ret = listen(server_socket, SOMAXCONN);
//	if (SOCKET_ERROR == ret) {
//		std::cout << "listen Error\n";
//		int err_num = WSAGetLastError();
//		err_display(err_num);
//	}
//
//	SOCKET client_sock;
//	SOCKADDR_IN clientaddr;
//	int addrlen;
//
//
//	while (1)
//	{
//		//accept()
//		addrlen = sizeof(clientaddr);
//		client_sock = accept(server_socket, (SOCKADDR*)&clientaddr, &addrlen);
//		if (client_sock == INVALID_SOCKET) {
//			//err_display("accept()");
//			std::cout << "!!!" << std::endl;
//		}
//		printf("Connected Client IP : %s\n", inet_ntoa(clientaddr.sin_addr));
//
//		hThread = CreateThread(NULL,0, Client_Thread, (LPVOID)client_sock, 0, NULL);//HandleClient ?????? ????, clientSock?? ?????????? ????
//
//	}
//
//	//return 1;
//
//}
//
//
//void Server::LoginServer()
//{
//}
//
//void Server::LobbyServer()
//{
//}
//
//void Server::UpdateStatus()
//{
//
//}
//
//void Server::UpdateCollision()
//{
//
//}
//
//void Server::InGame()
//{
//	while (true) {
//		UpdateStatus();
//		UpdateCollision();
//	}
//}
