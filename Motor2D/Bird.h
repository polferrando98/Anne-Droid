#pragma once
#include "j1Entity.h"

class Bird :
	public Entity
{
public:
	Bird(fPoint position);
	~Bird();

	bool Start();
	bool Update(float dt);



	bool GoToDestination();

};

