#pragma once

const short SERVER_PORT = 9000;

#pragma pack (push, 1)
<<<<<<< HEAD
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
=======
>>>>>>> RemotTest2

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

struct cs_send_keyinfo {
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
	bool isStart = false;
};

struct sc_update {
	unsigned char size;
	char packet_type;
	float x, y;
	int type;		//������ ���Դ��� �˸��� ����, Bullet �� Bullet�� ������ �ٸ����̰� Map��ġ�� �ɼ�������. 
	bool isClick;
	bool isPush[2];
	int AttackMonsterId = -1;
	bool clear = false;
	int Current_Stage;
};

struct sc_start_game {
	bool gamestart;
	int stage;
};
#pragma pack(pop)
