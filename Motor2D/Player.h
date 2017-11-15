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

protected:
	Animation jump;
	Animation jump_left;
};

