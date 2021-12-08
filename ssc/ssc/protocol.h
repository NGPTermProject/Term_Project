#pragma once

const short SERVER_PORT = 9000;



#pragma pack (push, 1)


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
	bool isStart = false;

};

struct sc_button {
	bool isPush[2];
};

struct sc_put_object {

	float x, y;
	bool isClick;
	bool isPush[2];
	int AttackMonsterId = -1;
	bool clear = false ;
	int Current_Stage;
};

struct sc_login_button {
	bool buttonCheck[2];
	int buttonid;
	int id;
};

struct sc_start_game {
	bool gamestart;
	int stage;
};




#pragma pack(pop)
