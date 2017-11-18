#pragma once
#include "j1Entity.h"
enum PlayerAction {MOVING, STATIC};
class Player :
	public Entity
{
public:
	Player(fPoint position);
	~Player();

	bool Start();
	bool Update(float dt);

	void Move();

	void ManageAnimation();

	void DoJump();

public:
	PlayerAction action = STATIC;
protected:
	Animation jump;
	Animation jump_left;

private:
	Direction_x last_direction_x = NONE_X;
	Direction_y last_direction_y = NONE_Y;

	Direction_x current_direction_x = NONE_X;
	Direction_y current_direction_y = NONE_Y;

	bool double_jump_avaliable = false;

	//HARDCODING

#define JUMP_SPEED 20;

};

