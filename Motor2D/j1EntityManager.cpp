#include "j1EntityManager.h"
#include "j1Entity.h"
#include "Walker.h"


j1EntityManager::j1EntityManager()
{
	name.create("EntityManager");
}


j1EntityManager::~j1EntityManager()
{
}

bool j1EntityManager::Start()
{
	return true;
}

bool j1EntityManager::CleanUp()
{
	return true;
}

bool j1EntityManager::Update(float dt)
{
	return true;
}

Entity * j1EntityManager::AddEnemy(fPoint position, EntityType type)
{

	switch (type)
	{
	case ENTITY_PLAYER:
		break;
	case ENTITY_WALKER:
		break;
	default:
		break;
	}
	return nullptr;
}
