#include "j1Physics.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Scene.h"

#include <math.h>
#include "j1Audio.h"
#include "SDL/include/SDL_timer.h"
#include "j1FadeToBlack.h"
#include "Animation.h"
#include "p2Log.h"

j1Physics::j1Physics() : j1Module()
{
	name.create("physics");
}

// Destructor
j1Physics::~j1Physics()
{}

// Called before render is available
bool j1Physics::Awake(pugi::xml_node& config)
{


	return true;
}

bool j1Physics::CleanUp()
{

	return true;
}

bool j1Physics::Update(float dt)
{
	Debug_draw();


	return UPDATE_CONTINUE;
}

void j1Physics::Debug_draw() const
{
	p2List_item<Collider*>* collider_iterator;

	Collider* pCollider = nullptr;

	for (collider_iterator = collider_list.start; collider_iterator != NULL; collider_iterator = collider_iterator->next)
	{
		pCollider = collider_iterator->data;

		if (pCollider->visble) {
			App->render->DrawQuad(*pCollider->rect, 0, 255, 0, alpha, true, true);
		}
	}
}

void j1Physics::UpdatePhysics(fPoint * position, fPoint * velocity, fPoint * acceleration)
{

	velocity->y += acceleration->y;

	position->y += velocity->y;
}

Collider* j1Physics::AddCollider(SDL_Rect *rect,const COLLIDER_TYPE type)
{
	Collider *pCollider = nullptr;

	pCollider = new Collider(rect, type);
	collider_list.add(pCollider);

	return pCollider;
}

void j1Physics::checkCollisions()
{
	p2List_item<Collider*>* collider_iterator_a;
	p2List_item<Collider*>* collider_iterator_b;

	Collider* pCollider_a = nullptr;
	Collider* pCollider_b = nullptr;

	for (collider_iterator_a = collider_list.start; collider_iterator_a != NULL; collider_iterator_a = collider_iterator_a->next)
	{
		pCollider_a = collider_iterator_a->data;

		for (collider_iterator_b = collider_list.start; collider_iterator_b != NULL; collider_iterator_b = collider_iterator_b->next)
		{

		}

	}
}

Collider::Collider(SDL_Rect *rectangle, COLLIDER_TYPE type)
{
	this->rect = rectangle;
	visble = true;
	this->type = type;
}
