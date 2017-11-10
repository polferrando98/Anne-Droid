#pragma once
#include "j1Module.h"

class j1EntityManager : public j1Module
{
public:
	j1EntityManager();
	~j1EntityManager();

	bool Start();
	bool CleanUp();
	bool Update(float dt);
};

