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
	char	name[MAX_NAME_SIZE];
};

struct sc_login_ok {
	unsigned char size;
	char	packet_type;
	int		id;
	short	x, y;
};


//cs
struct cs_move {
	short id;
	unsigned char size;
	char packet_type;
	bool left;
	bool jump;
	bool right;
	int type; //��,�� ���� 
};

struct cs_mouse_input{
	short id;
	unsigned char size;
	char packet_type;
	bool isClick;
	float x,y;
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

struct cs_send_player_id {
	short id;
};

struct cs_send_player {
	short id;
	int state;
	
	
	float x;
	float y;
	
	int jumpCount;
	int dir;
};

struct cs_send_keyinfo {
	short id;
	short size;
	char packet_type;
	bool left = false;
	bool right = false;
	bool jump = false;
	bool isClick = false;
	float x = 0;
	float y = 0;

	//bool isColl = false;
	//int collId = -1;
};

struct cs_obstacle {
	float x;
	float y; 
	bool isColl;
};

struct sc_button {
	bool isPush[2];
};

struct cs_bullet {
	float x;
	float y;
	bool isColl = false;
	int type = 0;
	int imageCount;
	int	imageSizeX;
	int	imageSizeY;
	int anim;
};
struct sc_put_object {
	unsigned char size;
	char packet_type;
	float x, y;
	int type;		//������ ���Դ��� �˸��� ����, Bullet �� Bullet�� ������ �ٸ����̰� Map��ġ�� �ɼ�������. 
	bool isClick;
	bool isPush[2];
	int AttackMonsterId;
	int bulletsize = 0;


};



#pragma pack(pop)




//for (int i = 0; i < m_obstacle.size(); ++i) {
//	m_obstacle[i].animation();
//	if (p.CollsionByObstacle(m_obstacle[i])) {
//		m_map.clear();
//	}
//	m_obstacle[i].Move();

//}
//for (int i = 0; i < vec_bullet.size(); ++i) {
//	vec_bullet[i].Update();
//	if (p.CollsionByObstacle(vec_bullet[i]) && vec_bullet[i].getisColl() != true) {
//		vec_bullet[i].setisColl(true);
//		m_map.clear();
//	}
//}
////�ϰ� ���� && ���� ����
//if (p.getVely() > 0 || p.getisRanding()) {
//	if (p.getVely() > 0) p.SwitchState(PLAYER::FALL);
//	int check = 0;
//	int b_check = 0;
//	
//		////���� �ڵ忡�� ��ư state�� �Ѵ� 1 �϶� ���� ��������.

//		//for (int i = 0; i < 2; ++i)
//		//	if (m_button[i].getState() == 1)
//		//	{
//		//		b_check++;
//		//	}
//		//if (b_check == 2)
//		//	m_button[0].x = 200;
//		//else
//		//	b_check = 0;


//	for (int i = 0; i < m_map.size(); ++i) {
//		if (p.getVely() > 600) p.setCollisonHelperY(8);
//		else p.setCollisonHelperY(0);

//		if (p.FallingCollsionOtherObject(m_map[i]))
//		{
//			p.setPlayerRanding(m_map[i].y - 32);
//			check++;
//		}
//	}

//	for (int i = 0; i < 2; ++i) {
//		// �÷��̾� ���� ����
//		if (p.FallingCollsionOtherObject(m_static_map[i]))
//		{
//			p.setPlayerRanding(m_static_map[i].y - 16);
//			m_static_map[i].setState(true);
//			check++;
//		}
//		// �ȴ���
//		else {
//			m_static_map[i].setState(false);
//		}
//	}


//	for (int i = 2; i <m_static_map.size(); ++i) {
//		if (p.getVely() > 600) p.setCollisonHelperY(8);
//		else p.setCollisonHelperY(0);

//		if (p.FallingCollsionOtherObject(m_static_map[i]))
//		{
//			p.setPlayerRanding(m_static_map[i].y - 32);
//			check++;
//		}
//	}

//	if (check == 0) {
//		p.setGravity();
//	}
//	else check = 0;
//}

//// �� ����     
//if (p.getPos().y > 780) {
//	p.setPlayerRanding(780);
//}

//if (stage != 0) {
//	p.Move();
//}
