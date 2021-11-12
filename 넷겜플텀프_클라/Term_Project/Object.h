class Objects
{
public:
	 Objects();
	~ Objects();

	float x = 0;
	float y = 0;
	
	int anim;
	int imageSizeX;
	int imageSizeY;

	int imageCount;

	int collsionHelper[4];

	void animation();

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


 class Map : public Objects {
 public:
	 static int count;

	 int type = 0;
	 int state = 0;

	 Map()
	 {
		 //¹öÆ°
		 collsionHelper[0] = 16;
		 collsionHelper[1] = 10;
		 collsionHelper[2] = 32;
		 collsionHelper[3] = 32;


	 }

	 Map(float m_x, float m_y)
	 {
		 x = m_x;
		 y = m_y;
		 // ¸Ê
		 collsionHelper[0] = 16;
		 collsionHelper[1] = 10;
		 collsionHelper[2] = 48;
		 collsionHelper[3] = 48;
	 }
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
		 if (type == 0) {
			 imageCount = 2;
			 imageSizeX = 64;
			 imageSizeY = 64;
		 }
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


	 int collsion_helper_y = 8;

	 const float PLAYER_GRAVITY = 30.8f; //30;
	 float accY = PLAYER_GRAVITY;
	 float velY = 0;

	 CImage idle = player_idle;
	 CImage move = player_move;
	 CImage jump = player_jump;
	 CImage fall = player_fall;
 public:

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

	 bool getisRanding()
	 {
		 return isRanding;
	 }

	 Pos getPos()
	 {
		 return pos;
	 }

	 void UpdateGravity()
	 {
		 velY += accY;
		 pos.y += (velY + accY) * 0.03f;
	 }

	 void SwitchState(int m_state)
	 {
		 state = m_state;
	 }

	 void setPlayerRanding(float pos_y)
	 {
		 SwitchState(PLAYER::IDLE);
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
			 state = PLAYER::MOVE;
			 dir = P_DIR_LEFT;
			 pos.x -= (speed * 0.016f);
		 }
		 if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		 {
			 state = PLAYER::MOVE;
			 dir = P_DIR_RIGHT;
			 pos.x += (speed * 0.016f);
		 }
	 }

	 void Jump()
	 {
		 SwitchState(PLAYER::JUMP);
		 accY = PLAYER_GRAVITY;
		 //isJump = true;
		 isRanding = false;
		 velY = -400.5f;
		 jumpCount++;
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
			 idle.Draw(hdc, pos.x - imageSizeX / 2, pos.y - imageSizeY / 2, imageSizeX, imageSizeY, anim, dir, imageSizeX, imageSizeY);
			 break;

		 case PLAYER::MOVE:
			 imageCount = 12;
			 move.Draw(hdc, pos.x - imageSizeX / 2, pos.y - imageSizeY / 2, imageSizeX, imageSizeY, anim, dir, imageSizeX, imageSizeY);
			 break;

		 case PLAYER::JUMP:
			 jump.Draw(hdc, pos.x - imageSizeX / 2, pos.y - imageSizeY / 2, imageSizeX, imageSizeY, 0, dir, imageSizeX, imageSizeY);
			 break;

		 case PLAYER::FALL:
			 fall.Draw(hdc, pos.x - imageSizeX / 2, pos.y - imageSizeY / 2, imageSizeX, imageSizeY, 0, dir, imageSizeX, imageSizeY);
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

	 void animation()
	 {
		 anim += imageSizeX;
		 if (anim >= imageSizeX * (imageCount - 1))
			 anim = 0;
	 }

	 Player(float m_x, float m_y)
	 {
		 pos.setPos(m_x, m_y);

		 state = PLAYER::IDLE;
		 imageCount = 11;
		 idle.Load("frog_idle.png");
		 move.Load("frog_move.png");
		 jump.Load("frog_jump.png");
		 fall.Load("frog_fall.png");
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

		 imageSizeX = 32;
		 imageSizeY = 32;
		 imageCount = 10;
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



