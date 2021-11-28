#pragma once

const short SERVER_PORT = 9000;

const int  MAX_NAME_SIZE = 20;
const int  MAX_USER = 10;

const char CS_PACKET_LOGIN = 1;
const char CS_PACKET_MOVE = 2;
const char CS_MOUSE_INPUT = 3;
const char CS_CLOSE_ANIMATION = 4;
const char CS_PUT_BUTTON = 5; 

const char SC_PACKET_LOGIN_OK = 1;
const char SC_PACKET_MOVE = 2;
const char SC_PACKET_PUT_OBJECT = 3;
const char SC_PACKET_REMOVE_OBJECT = 4;
const char SC_SET_ANIMATION = 5;
const char SC_CLEAR_STAGE = 6;
const char SC_PLAYER_COLLISION = 7;


#pragma pack (push, 1)
struct cs_packet_login {
	unsigned char size;
	char	packet_type;
};

struct sc_login_ok {
	unsigned char size;
	char	packet_type;
	short		id;
};



struct cs_close_animation{		//�ִϸ��̼� ������ �������� �˸�. 
	unsigned char size;
	char packet_type;
	bool isDone;
};

struct cs_put_button{		//�÷��̾ ��ư�� ������ ������ �˸�. 
	unsigned char size;
	char packet_type;
	bool isPush;
};

//sc

struct sc_move {
	unsigned char size;
	char packet_type;
	float x ,y;
	int dir;
};




struct sc_remove_object {
	unsigned char size;
	char packet_type;
	int type;
	int index;
};

struct sc_set_animation{
	unsigned char size;
	char packet_type;
	int type;			//�ִϸ��̼� Ÿ�� ���� 
	int animCount = 0;		//�ִϸ��̼� ���� ī��Ʈ �ʱ�ȭ 
	int maxCount;	//�ִϸ��̼��� �ִ� ī��Ʈ ����
 
};

struct sc_clear_stage{	//�������� ����
	unsigned char size;
	char packet_type;
	int stage;			 
}; 

struct sc_player_collision{	//���Ϳ� ���� �浹
	unsigned char size;
	char packet_type;
	
	float x,y; 				//�÷��̾� �ʱ� ��ġ��.
	
};

struct sc_send_player_id {
	short id;
};

struct sc_send_player {
	short id;
	int state;
	
	
	float x;
	float y;
	
	int jumpCount;
	int dir;

};
struct sc_recv_keyinfo {
	short id;
	bool left = false;
	bool right = false;
	bool jump = false;
	bool isClick = false;
	float x;
	float y;
};

struct sc_obstacle {
	float x;
	float y;
	bool isColl = false;
};

struct sc_bullet {
	float x;
	float y;
	bool isColl = false;
	int type = 0;
	int imageCount;
	int	imageSizeX;
	int	imageSizeY;
	int anim;
};

struct sc_button {
	bool isPush[2];
};

struct sc_put_object {
	unsigned char size;
	char packet_type;
	float x, y;
	int type;		//������ ���Դ��� �˸��� ����, Bullet �� Bullet�� ������ �ٸ����̰� Map��ġ�� �ɼ�������. 
	bool isClick;
	bool isPush[2];
	int AttackMonsterId = -1;
	int bulletsize = 0;
};



#pragma pack(pop)
