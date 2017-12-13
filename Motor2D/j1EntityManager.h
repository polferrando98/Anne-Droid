#pragma once
#include "j1Module.h"
#include "j1Entity.h"

class j1EntityManager : public j1Module
{
public:
	j1EntityManager();
	~j1EntityManager();

	bool CleanUp();

	bool PreUpdate(float dt);
	bool Update(float dt);

	Entity* CreateEntity(fPoint position,EntityType type);

public:
	Entity* player_entity = nullptr;
private:
	p2List<Entity*>	entites;
	p2List<Entity*>	ovnies;
};

