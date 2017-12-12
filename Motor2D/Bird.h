#pragma once
#include "j1Entity.h"

struct PathList;

class Bird :
	public Entity
{
public:
	Bird(fPoint position);
	~Bird();


	bool PreUpdate(float dt);
	bool Update(float dt);

	//PathList path_to_follow;

	void Respawn();
	
	bool GoToDestination();



private:


};

