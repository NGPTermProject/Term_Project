class Objects
{
public:
	 Objects();
	~ Objects();

	float x = 0;
	float y = 0;
	
	int anim = 0;
	int imageSizeX;
	int imageSizeY;

	int imageCount;

	int collsionHelper[4];

	void animation();

	CImage img;

};

 Objects:: Objects()
{
}

 Objects::~ Objects()
{
}

 void Objects::animation()
 {
	 anim += imageSizeX;
	 if (anim >= imageSizeX * (imageCount - 1))
		 anim = 0;
 }


 class Obstacle : public Objects {
 public:
	 int type;

	 void Move() {
		 x += 1;
	 }
	 void draw(HDC hdc)
	 {
		 if (type == OBSTACLE::BLADE)
			img_cutting_blade.Draw(hdc,x - imageSizeX / 2, y - imageSizeY / 2, imageSizeX, imageSizeY, anim, 0, imageSizeX, imageSizeY);
		 if (type == OBSTACLE::LONG_UP)
			 img_Obs_960_96_Up.Draw(hdc, x - imageSizeX / 2, y - imageSizeY / 2, imageSizeX, imageSizeY);
		 if (type == OBSTACLE::LONG)
			 img_Obs_960_96.Draw(hdc, x - imageSizeX / 2, y - imageSizeY / 2, imageSizeX, imageSizeY);
		 if (type == OBSTACLE::MIDDLE_UP)
			 img_Obs_544_96_Up.Draw(hdc, x - imageSizeX / 2, y - imageSizeY / 2, imageSizeX, imageSizeY);
		 if (type == OBSTACLE::MIDDLE)
			 img_Obs_544_96.Draw(hdc, x - imageSizeX / 2, y - imageSizeY / 2, imageSizeX, imageSizeY);
		 if (type == OBSTACLE::SHORT)
			 img_Obs_320_96.Draw(hdc, x - imageSizeX / 2, y - imageSizeY / 2, imageSizeX, imageSizeY);
	 }
	 Obstacle();
	 Obstacle(int m_type ,  float m_x, float m_y) {

		 if (m_type == OBSTACLE::BLADE) {
			 x = m_x;
			 y = m_y;
			 type = m_type;
			 imageSizeX = 64;
			 imageSizeY = 64;
			 imageCount = 5;
		 }
		 if (m_type == OBSTACLE::LONG_UP) {
			 x = m_x;
			 y = m_y;
			 type = m_type;
			 imageSizeX = 96;
			 imageSizeY = 960;
		 }
		 if (m_type == OBSTACLE::LONG) {
			 x = m_x;
			 y = m_y;
			 type = m_type;
			 imageSizeX = 960;
			 imageSizeY = 96;
		 }
		 if (m_type == OBSTACLE::MIDDLE_UP) {
			 x = m_x;
			 y = m_y;
			 type = m_type;
			 imageSizeX = 96;
			 imageSizeY = 544;
		 }
		 if (m_type == OBSTACLE::MIDDLE) {
			 x = m_x;
			 y = m_y;
			 type = m_type;
			 imageSizeX = 544;
			 imageSizeY = 96;
		 }
		 if (m_type == OBSTACLE::SHORT) {
			 x = m_x;
			 y = m_y;
			 type = m_type;
			 imageSizeX = 320;
			 imageSizeY = 96;
		 }
	 }
 };


 class Map : public Objects {
 public:
	 static int count;

	 int type = 0;
	 bool state = false;

	 void setState(bool m_state)
	 {
		 state = m_state;
	 }

	 int getType()
	 {
		 return type;
	 }

	 bool getState()
	 {
		 return state;
	 }

	 void draw(HDC memdc1)
	 {
		 if (type == MAP::BUTTON) {
			 if(state == false)
				img_ButtonUp.Draw(memdc1, x - 32, y - 16, 64, 32);
			 else
				img_ButtonDown.Draw(memdc1, x - 32, y - 16, 64, 32);
		 }
		 if (type == MAP::PLAT)
			 img_wall.Draw(memdc1, x - 48, y - 16, 96, 32);
	 }
	 Map()
	 {


	 }

	 Map(int m_type , float m_x, float m_y)
	 {
		 type = m_type;
		 x = m_x;
		 y = m_y;

		 if (m_type == MAP::PLAT) {
			 collsionHelper[0] = 16;
			 collsionHelper[1] = 10;
			 collsionHelper[2] = 48;
			 collsionHelper[3] = 48;
		 }
		 if (m_type == MAP::BUTTON)
		 {
			 collsionHelper[0] = 16;
			 collsionHelper[1] = 10;
			 collsionHelper[2] = 32;
			 collsionHelper[3] = 32;
		 }
	 }
	 ~Map() {};
 };

 struct Pos
 {
	 float x = 0;
	 float y = 0;

	 void setPos(float m_x, float m_y)
	 {
		 x = m_x;
		 y = m_y;
	 }

 };

 class Bullet : public Objects {
 public:

	 float dx = 3;
	 float dy = 3;

	 int dir = 1;

	 int type = 0;
	 bool isColl = false;

	 Bullet(int m_type, int m_x, int m_y)
	 {
		 x = m_x;
		 y = m_y;
		 type = m_type;
		 anim = 0;
		 if (type == MONSTER::PIG) {
			 imageCount = 2;
			 imageSizeX = 64;
			 imageSizeY = 64;
		 }
		 else if (type == MONSTER::PLANT) {
			 imageCount = 3;
			 imageSizeX = 24;
			 imageSizeY = 24;
		 }

	 }

	 void setisColl(bool b)
	 {
		 isColl = b;
		 anim = 0;
		 if(type == MONSTER::PIG)
			imageCount = 6;
	 }

	 bool getisColl()
	 {
		 return isColl;
	 }

	 void draw(HDC hdc)
	 {
		 if (type == MONSTER::PIG) {
			 if (isColl)
				 img_Bomb_Coll.Draw(hdc, x - 32, y - 32, 64, 64, anim, 0, 64, 64);
			 else {
				 img_Bomb.Draw(hdc, x - 32, y - 32, 64, 64, anim, 0, 64, 64);
			 }
		 }
		 else if (type == MONSTER::PLANT) {
			 if (isColl)
				 img_Bullet_Coll.Draw(hdc, x - 12, y - 12, 24, 24);
			 else {
				 img_Bullet.Draw(hdc, x - 12, y - 12, 24, 24, anim, 0, 24, 24);
			 }
		 }
	 }

	 void Move()
	 {

		float gravity = 0.3f;
		dy += gravity;
		x += dx;
		y += dy;
		 
	 }
	 void Update()
	 {
		 if (Coll() == false)
			 Move();

		 if (Coll() && isColl == false) {
			 isColl = true;
			 anim = 0;
			 imageCount = 6;
		 }
	 }
	 bool Coll()
	 {
		 if (y > 780)
			 return true;
		 else
			 return false;
	 }

	 ~Bullet() {
	 };
 };

 class Player {
 private:

	 Pos pos;
	 int jumpCount = 0;
	 int dir = 0;
	 int state = 0;

	 int anim = 0;
	 int speed = 300;
	 int imageCount = 0;

	 bool isRanding = false;
	 int imageSizeX = 32;
	 int imageSizeY = 32;

	 int max_create_map = 0;

	 int collsion_helper_y = 8;

	 const float PLAYER_GRAVITY = 30.8f; //30;
	 float accY = PLAYER_GRAVITY;
	 float velY = 0;



	 short id = -1;
 public:

	 void setID(short m_id)
	 {
		 id = m_id;
	 }
	 void setState(int m_state)
	 {
		 state = m_state;
	 }

	 void setAnim(int m_anim)
	 {
		 anim = m_anim;
	 }
	 void setImageCount(int m_ic)
	 {
		 imageCount = m_ic;
	 }
	 void setisRanding(bool r)
	 {
		 isRanding = r;
	 }
	 void setPos(float m_x, float m_y)
	 {
		 pos.x = m_x;
		 pos.y = m_y;
	 }
	 void setJumpCount(int jc)
	 {
		 jumpCount = jc;
	 }




	 void setCollisonHelperY(int y)
	 {
		 collsion_helper_y = y;
	 }

	 void setGravity()
	 {
		 accY = PLAYER_GRAVITY;
		 isRanding = false;
	 }

	 float getVely()
	 {
		 return velY;
	 }


	 void UpdateGravity()
	 {
		 velY += accY;
		 pos.y += (velY + accY) * 0.03f;
	 }

	 void setDir(int m_dir)
	 {
		 dir = m_dir;
	 }

	 void setPlayerRanding(float pos_y)
	 {
		 setState(PLAYER::IDLE);
		 accY = 0;
		 velY = 0;
		 pos.y = pos_y;
		 isRanding = true;
		 jumpCount = 0;
	 }

	 void Move()
	 {
		 if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		 {
			 if(velY == 0) 
				 state = PLAYER::MOVE;
			 dir = P_DIR_LEFT;
			 pos.x -= (speed * 0.016f);
		 }
		 if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		 {
			 if (velY == 0)
				state = PLAYER::MOVE;
			 dir = P_DIR_RIGHT;
			 pos.x += (speed * 0.016f);
		 }
	 }

	 void Jump()
	 {
		 accY = PLAYER_GRAVITY;
		 //isJump = true;
		 isRanding = false;
		 velY = -400.5f;
		 jumpCount++;
		 setState(PLAYER::JUMP);
	 }

	 int getJumpCount()
	 {
		 return jumpCount;
	 }


	 void draw(HDC hdc)
	 {
		 switch (state)
		 {
		 case PLAYER::IDLE:
			 imageCount = 11;
			 player_idle.Draw(hdc, pos.x - imageSizeX / 2, pos.y - imageSizeY / 2, imageSizeX, imageSizeY, anim, dir, imageSizeX, imageSizeY);
			 break;

		 case PLAYER::MOVE:
			 imageCount = 12;
			 player_move.Draw(hdc, pos.x - imageSizeX / 2, pos.y - imageSizeY / 2, imageSizeX, imageSizeY, anim, dir, imageSizeX, imageSizeY);
			 break;

		 case PLAYER::JUMP:
			 player_jump.Draw(hdc, pos.x - imageSizeX / 2, pos.y - imageSizeY / 2, imageSizeX, imageSizeY, 0, dir, imageSizeX, imageSizeY);
			 break;

		 case PLAYER::FALL:
			 player_fall.Draw(hdc, pos.x - imageSizeX / 2, pos.y - imageSizeY / 2, imageSizeX, imageSizeY, 0, dir, imageSizeX, imageSizeY);
		 }
	 }

	 bool FallingCollsionOtherObject(Objects obj) {
		 if (pos.y + imageSizeY / 2 >= obj.y - obj.collsionHelper[0] &&
			 pos.y - collsion_helper_y <= obj.y - obj.collsionHelper[1] &&
			 pos.x < obj.x + obj.collsionHelper[2] && pos.x > obj.x - obj.collsionHelper[3])
		 {
			 return true;
		 }
		 else return false;
	 }

	 bool CollsionByObstacle(Objects obj) {
		 if (pos.x - imageSizeX / 2 > obj.x + obj.imageSizeX / 2) return false;
		 if (pos.x + imageSizeX / 2 < obj.x - obj.imageSizeX / 2) return false;
		 if (pos.y + imageSizeY / 2 < obj.y - obj.imageSizeY / 2) return false;
		 if (pos.y - imageSizeY / 2 > obj.y + obj.imageSizeY / 2) return false;

		 return true;

	 }

	 void animation()
	 {
		 anim += imageSizeX;
		 if (anim >= imageSizeX * (imageCount - 1))
			 anim = 0;
	 }

	 Player(float m_x, float m_y , short m_id)
	 {
		 pos.setPos(m_x, m_y);
		 id = m_id;
		 state = PLAYER::IDLE;
		 imageCount = 11;

	 }

	 Player();

	 //bool ()
 };

 class Monster : public Objects {
 public:


	 int type = 0;
	 bool attack = false;

	 Monster(int m_type, float m_x, float m_y)
	 {
		 x = m_x;
		 y = m_y;
		 type = m_type;

		 anim = 0;

		 if (m_type == MONSTER::PIG) {
			 imageSizeX = 32;
			 imageSizeY = 32;
			 imageCount = 10;
		 }
		 if (m_type == MONSTER::PLANT) {
			 imageSizeX = 44;
			 imageSizeY = 42;
			 imageCount = 12;
		 }
	 }

	 void draw(HDC hdc) {
		 
		 if (type == MONSTER::PIG) {
			 if (attack)
				 img_Bomb_Monster_Attack.Draw(hdc, x - 16, y - 16, 32, 32, anim, 0, 32, 32);
			 else {
				 img_Bomb_Monster_Idle.Draw(hdc, x - 16, y - 16, 32, 32, anim, 0, 32, 32);
			 }
		 }
		 if (type == MONSTER::PLANT) {
			 if (attack)
				 img_Plant_Monster_Attack.Draw(hdc, x - 22, y - 21, imageSizeX, 42, anim, 0, imageSizeX, 42);
			 else {
				 img_Plant_Monster_Idle.Draw(hdc, x - 22, y - 21, imageSizeX, 42, anim, 0, imageSizeX, 42);
			 }
		 }
		 

	 }

	 void animation()
	 {
		 anim += imageSizeX;
		 if (anim >= imageSizeX * (imageCount - 1)) {
			 anim = 0;
			 if (attack) {
				 imageCount = 10;
				 attack = false;
			 }
		 }
	 }

	 Bullet Attack() {
		 anim = 0;
		 imageCount = 7;
		 attack = true;
		 return Bullet(type, x, y);
	 }

 };



