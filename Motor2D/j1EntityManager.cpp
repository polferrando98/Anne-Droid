#include "j1EntityManager.h"
#include "j1Entity.h"
#include "Walker.h"
#include "Player.h"
#include "Bird.h"


j1EntityManager::j1EntityManager()
{
	name.create("EntityManager");
}


j1EntityManager::~j1EntityManager()
{
}


bool j1EntityManager::CleanUp()
{
	p2List_item<Entity*>* entities_iterator;
	for (entities_iterator = entites.start; entities_iterator != nullptr; entities_iterator = entities_iterator->next)
	{
		RELEASE(entities_iterator->data);
	}
	player_entity = nullptr;
	entites.clear();

	return true;
}

bool j1EntityManager::PreUpdate(float dt)
{
	bool ret = true;
	p2List_item<Entity*>* entities_iterator;
	entities_iterator = entites.start;
	Entity* entity = nullptr;

	for (entities_iterator = entites.start; entities_iterator != NULL && ret == true; entities_iterator = entities_iterator->next)
	{
		entity = entities_iterator->data;

		ret = entity->PreUpdate(dt);
	}

	return ret;
}

bool j1EntityManager::Update(float dt)
{
	bool ret = true;
	p2List_item<Entity*>* entities_iterator;
	entities_iterator = entites.start;
	Entity* entity = nullptr;

	for (entities_iterator = entites.start; entities_iterator != NULL && ret == true; entities_iterator = entities_iterator->next)
	{
		entity = entities_iterator->data;
		ret = entity->Update(dt);
	}

	return ret;
}

Entity * j1EntityManager::CreateEntity(fPoint position, EntityType type)
{

	Entity* new_entity = nullptr;

	switch (type)
	{
	case ENTITY_PLAYER:
		new_entity = new Player(position);
		player_entity = new_entity;
		break;
	case ENTITY_WALKER:
		new_entity = new Walker(position);
		break;
	case ENTITY_BIRD:
		new_entity = new Bird(position);
		break;
	default:
		break;
	}

	entites.add(new_entity);

	return new_entity;
}


