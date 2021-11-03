namespace PLAYER
{
	enum P_State {
		IDLE,
		MOVE,
		JUMP,
		FALL,
		ATTACK,
		DEAD
	};
}

namespace MONSTER
{
	enum M_State {
		IDLE,
		DEAD
	};
}

static PLAYER::P_State p_state;
static MONSTER::M_State m_state;
