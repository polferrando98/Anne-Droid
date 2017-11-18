#include "Bird.h"
#include "j1App.h"
#include "j1Textures.h"
#include "Animation.h"
#include "p2Log.h"


Bird::Bird(fPoint position) : Entity(position, ENTITY_BIRD)
{

	idle_right.LoadEnemySprites("ovni");

}

bool Bird::Start()
{
	current_animation = &idle_right;
	Entity::Start();
	SDL_Rect colrect = { 0,0,60,60 };
	//HARDCODE
	colrect.x = position.x;
	colrect.y = position.y;

	collider = App->physics->AddCollider(&colrect, COL_WALKER);


	movement_acceleration.x = 1.0f;
	movement_acceleration.y = 1.0f;

	texture = App->tex->Load("textures/ovni.png");

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

	UpdateCurrentTile();
	return ret;
}

bool Bird::GoToDestination()
{
	iPoint bad_destination = { -1,-1 };
	iPoint dest;

	if (dest == bad_destination) {
		LOG("Error, empty destination");
		return false;
	}

	if (current_tile != dest) {
		fPoint direction;
		direction.x = dest.x - position.x;
		direction.y = dest.y - position.y;

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
