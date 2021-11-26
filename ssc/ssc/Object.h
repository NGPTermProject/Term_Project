#pragma once
#include "State.h"
#include "protocol.h"
class Objects
{
public:
	Objects() {}
	~Objects() {}

	float x = 0;
	float y = 0;
	
	int anim = 0;
	int imageSizeX = 0;
	int imageSizeY = 0;

	int imageCount = 0;

	int collsionHelper[4];

	void animation() {
		anim += imageSizeX;
		if (anim >= imageSizeX * (imageCount - 1))
			anim = 0;
	};


};





 class Obstacle : public Objects {
 public:
	 int type;
	 int dir  = 1;
	 void Move() {
		 x += 5 * dir;
		 if (x > 1450) {
			 x = 1450;
			 dir *= -1;
		 }
		 if (x < 50) {
			 x = 50;
			 dir *= -1;
		 }
	 }
	 float getPosX()
	 {
		 return x;
	 }
	 float getPosY()
	 {
		 return y;
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

 class Monster : public Objects {
 public:


	 int type = 0;
	 bool attack = false;
	 int AttackDelay = 0;
	 int AttackCount = 0;
	 Monster(int m_type, float m_x, float m_y, int m_ad)
	 {
		 x = m_x;
		 y = m_y;
		 type = m_type;
		 AttackDelay = m_ad;
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

	 //Bullet Attack() {
		// anim = 0;
		// imageCount = 7;
		// attack = true;
		// return Bullet(type, x, y);
	 //}

	 void Update() {

		 AttackCount++;
		 if (AttackDelay < AttackCount) {
			 AttackCount = 0;
			 attack = true;
		 }
	 }
	 bool getisAttack() {
		 return attack;
	 }

	 void setisAttack(bool b)
	 {
		 attack = b;
	 }

	 float getPosX()
	 {
		 return x;
	 }
	 float getPosY()
	 {
		 return y;
	 }


 };




 class Bullet : public Objects {
 public:

	 float dx = 3;
	 float dy = 3;

	 int dir = 1;

	 int type = 0;
	 bool isColl = false;
	 bool isStart = false;
	 
	 int anim_max = 0;

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
	 Bullet()
	 {

	 };

	 bool getisColl()
	 {
		 return isColl;
	 }

	 void setisColl(bool b)
	 {
		 dy = 0;
		 isColl = b;
		 anim = 0;
		 if(type == MONSTER::PIG)
			imageCount = 6;
	 }




	 void Move()
	 {
		 if (type == MONSTER::PIG) {
			 float gravity = 0.3f;
			 dy += gravity;
			 x += dx;
			 y += dy;
		 }
		 if (type == MONSTER::PLANT) {
			 x += dx* 5.f;
		 }
		 
	 }
	 void Update()
	 {

		 if (Coll() == false)
			 Move();

		 if (Coll() && isColl == false) {
			 setisColl(true);
		 }
	 }
	 bool Coll()
	 {
		 if (y > 780)
			 return true;
		 else
			 return false;
	 }


	 void SetBulletInfo(sc_bullet &bullet) {
		 bullet.x =x;
		 bullet.y =y;
		 bullet.type = type;
		 bullet.imageCount = imageCount;
		 bullet.imageSizeX = imageSizeX;
		 bullet.imageSizeY = imageSizeY;
		 bullet.anim = anim;
		 bullet.isStart = isStart;
		 bullet.isColl = isColl;
	 }

	 void InitBullet(Monster m_monster) {
		 type = m_monster.type;
		 x = m_monster.getPosX();
		 y = m_monster.getPosY();

		 if (type == MONSTER::PIG) {
			 imageCount = 2;
			 imageSizeX = 64;
			 imageSizeY = 64;
			 anim_max = 220;

		 }
		 else if (type == MONSTER::PLANT) {
			imageCount = 3;
			imageSizeX = 24;
			imageSizeY = 24;
			anim_max = 20;

		 }
		 isStart = true;
	 }
		
	 void DeadAnimation(Monster m_monster){
		 x = m_monster.getPosX();
		 y = m_monster.getPosY();

		 if (type == MONSTER::PIG) {
			 imageCount = 2;

		 }
		 else if (type == MONSTER::PLANT) {
			 imageCount = 3;

		 }
		  isStart = false;
		  isColl = false;
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

	 short id;

 public:
	 void setDir(int m_dir)
	 {
		 dir = m_dir;
	 }

	 int getDir() 
	 {
		 return dir;
	 }

	 short getID()
	 {
		 return id;
	 }
	 int getState()
	 {
		 return state;
	 }
	 int getAnim()
	 {
		 return anim;
	 }
	 int getImageCount()
	 {
		 return imageCount;
	 }
	 bool getisRanding()
	 {
		 return isRanding;
	 }
	 Pos getPos()
	 {
		 return pos;
	 }
	 void setStartLine(int x, int y) {
		 pos.setPos(x, y);
		 velY = 0;
	 }
	 int getJumpCount()
	 {
		 return jumpCount;
	 }
	 


	 void setId(short m_id)
	 {
		 id = m_id;
	 }
	 int get_max_create_map()
	 {
		 return max_create_map;
	 }

	 void set_max_create_map_plus()
	 {
		 max_create_map++;
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

	

	 void Jump()
	 {
		 accY = PLAYER_GRAVITY;
		 //isJump = true;
		 isRanding = false;
		 velY = -400.5f;
		 jumpCount++;
		 SwitchState(PLAYER::JUMP);
	 }


	void move(float m_x)
	{
		pos.x += m_x;
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

	 Player(float m_x, float m_y, short m_id)
	 {
		 pos.setPos(m_x, m_y);

		 state = PLAYER::IDLE;
		 imageCount = 11;
		 id = m_id;
	 }

	 Player();
	 short getId() {
		 return id;
	 }
	 //bool ()


	 void setPlayerInfo(sc_send_player &player)
	 {
		player.id = id;
		player.state =getState();
		player.x = getPos().x;
		player.y = getPos().y;
		player.dir = getDir();
		player.jumpCount = getJumpCount();
	 }
 };
