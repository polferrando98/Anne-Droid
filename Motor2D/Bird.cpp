#include "Bird.h"
#include "j1App.h"
#include "j1Textures.h"
#include "Animation.h"
#include "p2Log.h"
#include "j1EntityManager.h"
#include "j1Pathfinding.h"


Bird::Bird(fPoint position) : Entity(position, ENTITY_BIRD)
{

	idle_right.LoadEnemySprites("ovni");

}

bool Bird::Start()
{
	current_animation = &idle_right;
	Entity::Start();
	SDL_Rect colrect = { 0,0,53,50 };
	//HARDCODE
	colrect.x = position.x;
	colrect.y = position.y;

	collider = App->physics->AddCollider(&colrect, COL_WALKER);


	movement_acceleration.x = 1.85f;
	movement_acceleration.y = 1.15f;

	texture = App->tex->Load("textures/ovni.png");

	return true;
}

bool Bird::PreUpdate(float dt)
{


	return true;
}

bool Bird::Update(float dt)
{
	bool ret = true;
	//App->physics->UpdateEntityPhysics(*this, dt);

	collider->UpdatePosition(position);

	// Debug follow path ------------------------------

	//HARDCODE
	float detection_radius = 300.0f;
	if (position.DistanceTo(App->entity_manager->player_entity->position) < detection_radius) {
		int path_steps = App->pathfinding->CreatePath(current_tile, App->entity_manager->player_entity->current_tile);
		//SPAGHETTI
		if (App->pathfinding->GetLastPath()->At(1) != nullptr)
			destination_tile = *App->pathfinding->GetLastPath()->At(1);
		else
			destination_tile = *App->pathfinding->GetLastPath()->At(0);

		ret = GoToDestination();
	}
	else {
		acceleration.SetToZero();
	}

	position = App->physics->calculateNewPosition(position, velocity, acceleration,BOTH_AXIS);

	Entity::Update(dt);

	UpdateCurrentTile();
	return ret;
}

bool Bird::GoToDestination()
{
	iPoint bad_destination = { -1,-1 };

	if (destination_tile == bad_destination) {
		LOG("Error, empty destination");
		return false;
	}

	if (current_tile != destination_tile) {
		fPoint direction;
		direction.x = destination_tile.x - current_tile.x;
		direction.y = destination_tile.y - current_tile.y;


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
