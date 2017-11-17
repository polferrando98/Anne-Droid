#include "Bird.h"
#include "j1App.h"
#include "j1Textures.h"



Bird::Bird(fPoint position) : Entity(position, ENTITY_BIRD)
{
	idle_left.PushBack({ 1052,2272,110,100 });
	idle_left.PushBack({ 811,2272,110,100 });
	idle_left.PushBack({ 570,2272,110,100 });
	idle_left.PushBack({ 328,2271,111,100 });
	idle_left.PushBack({ 87,2271,110,100 });
	idle_left.PushBack({ 1052,2029,110,100 });
	idle_left.PushBack({ 811,2029,110,100 });
	idle_left.PushBack({ 570,2030,110,100 });
	idle_left.PushBack({ 328,2031,111,100 });
	idle_left.PushBack({ 87,2031,110,100 });

	idle_left.loop = true;
	idle_left.speed = animation_speed;

	current_animation = &idle_left;
}

bool Bird::Start()
{
	Entity::Start();
	SDL_Rect colrect = { 0,0,100,100 };
	//HARDCODE
	colrect.x = position.x;
	colrect.y = position.y;

	collider = App->physics->AddCollider(&colrect, COL_WALKER);
	texture = App->tex->Load("textures/player_sprites.png");
	return true;
}

bool Bird::Update(float dt)
{
	collider->UpdatePosition(position);
	Entity::Update(dt);
	return true;
}

Bird::~Bird()
{
}
