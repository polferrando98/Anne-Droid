#include "Walker.h"
#include "j1App.h"
#include "j1Textures.h"


Walker::Walker(fPoint position) : Entity(position,ENTITY_WALKER)
{

}

bool Walker::Start()
{
	texture = App->tex->Load("textures/player_sprites.png");
	return true;
}

bool Walker::Update(float dt)
{
	Entity::Update(dt);
	return true;
}



Walker::~Walker()
{

}