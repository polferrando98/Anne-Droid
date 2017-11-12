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
};

