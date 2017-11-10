#pragma once
#include "j1Module.h"
#include "j1Entity.h"

class j1EntityManager : public j1Module
{
public:
	j1EntityManager();
	~j1EntityManager();

	bool Start();
	bool CleanUp();
	bool Update(float dt);

	Entity* AddEnemy(fPoint position,EntityType type);


private:
	p2List<Entity*>	modules;
};

