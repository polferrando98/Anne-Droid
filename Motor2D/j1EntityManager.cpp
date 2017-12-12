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
	if (entites.start) {
		p2List_item<Entity*>* entities_iterator;
		for (entities_iterator = entites.start; entities_iterator != nullptr; entities_iterator = entities_iterator->next)
		{
			RELEASE(entities_iterator->data);
		}
		entites.clear();
	}
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

void j1EntityManager::DeleteOvnis()
{
	if (entites.start) {
		p2List_item<Entity*>* entities_iterator;
		for (entities_iterator = entites.start; entities_iterator != nullptr; entities_iterator = entities_iterator->next)
		{
			if (entities_iterator->data->type == ENTITY_BIRD) {
				RELEASE(entities_iterator->data);
				entites.del(entities_iterator);
			}
		}
	}
}

void j1EntityManager::DeletePlayer()
{
	if (entites.start) {
		p2List <Entity*> entitiesToDelete;
		p2List_item<Entity*>* entities_iterator;
		for (entities_iterator = entites.start; entities_iterator != nullptr; entities_iterator = entities_iterator->next)
		{
			if (entities_iterator->data->type == ENTITY_PLAYER) {
				RELEASE(entities_iterator->data);
				player_entity = nullptr;
				entitiesToDelete.add(entities_iterator->data);
			}
		}

		for (entities_iterator = entitiesToDelete.start; entities_iterator != nullptr; entities_iterator = entities_iterator->next)
		{
			entites.del(entites.At(entites.find(entities_iterator->data)));
		}

		entitiesToDelete.clear();
	}
}
