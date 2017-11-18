#include "Bird.h"
#include "j1App.h"
#include "j1Textures.h"
#include "Animation.h"


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
	texture = App->tex->Load("textures/ovni.png");
	return true;
}

bool Bird::Update(float dt)
{

	//App->physics->UpdateEntityPhysics(*this, dt);

	collider->UpdatePosition(position);
	Entity::Update(dt);
	return true;
}

void Bird::GoToTile(iPoint destination)
{
	if (currentTile != destination)
	{
		
	}

}

Bird::~Bird()
{
}
