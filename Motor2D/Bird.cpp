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
	SDL_Rect colrect = { 0,0,60,60 };
	//HARDCODE
	colrect.x = position.x;
	colrect.y = position.y;

	collider = App->physics->AddCollider(&colrect, COL_WALKER);
	texture = App->tex->Load("textures/player_sprites.png");

	movement_acceleration.x = 1.0f;
	movement_acceleration.y = 1.0f;
	return true;
}

bool Bird::Update(float dt)
{
	bool ret = true;
	//App->physics->UpdateEntityPhysics(*this, dt);

	collider->UpdatePosition(position);

	ret = GoToDestination();

	position = App->physics->calculateNewPosition(position, velocity, acceleration,BOTH_AXIS);

	Entity::Update(dt);
	return ret;
}

bool Bird::GoToDestination()
{
	iPoint bad_destination = { -1,-1 };
	if (destination_tile == bad_destination)
		return false;

	if (currentTile != destination_tile) {
		fPoint direction;
		destination_tile.x = destination_tile.x - position.x;
		destination_tile.y = destination_tile.y - position.y;

		if (direction.x > 0) {
			acceleration.x = movement_acceleration.x;
		}
		if (direction.x < 0) {
			acceleration.x = -movement_acceleration.x;
		}
		if (direction.y > 0) {
			acceleration.y = movement_acceleration.y;
		}
		if (direction.y < 0) {
			acceleration.y = -movement_acceleration.y;
		}
	}
	else {
		acceleration.y = 0;
		acceleration.x = 0;
	}
	return true;
}

Bird::~Bird()
{
}
