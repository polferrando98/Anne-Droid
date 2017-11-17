#pragma once
#include "j1Entity.h"
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

protected:
	Animation jump;
	Animation jump_left;

private:
	Direction_x last_direction_x = NONE_X;
	Direction_y last_direction_y = NONE_Y;

	Direction_x current_direction_x = NONE_X;
	Direction_y current_direction_y = NONE_Y;
};

