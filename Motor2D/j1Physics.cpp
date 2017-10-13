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

void j1Physics::UpdatePhysics(fPoint * position, fPoint * velocity, fPoint * acceleration, Collider* collider)
{
	Collider newCollider = *collider;
	bool colliding_x = false;
	bool colliding_y = false;

	//for some reason this does not work
	//fPoint newVelocity = *velocity + *acceleration;
	//fPoint newPosition = *position + newVelocity;

	fPoint newVelocity;
	newVelocity.x = velocity->x + acceleration->x;
	newVelocity.y = velocity->y + acceleration->y;

	fPoint newPosition;
	newPosition.x = position->x + newVelocity.x;
	newPosition.y = position->y + newVelocity.y;

	newCollider.rect->x = newPosition.x;
	newCollider.rect->y = newPosition.y;

	fPoint pos_differential;

	pos_differential.x = newPosition.x - position->x;
	pos_differential.y = newPosition.y - position->y;


	//if (checkCollisions(&newCollider)) {		
	//	if (pos_differential.y < 0) {
	//		velocity->y += acceleration->y;
	//		position->y += velocity->y;
	//	}
	//	if (intersection.w > 10) {
	//		velocity->x += acceleration->x;
	//		position->x += velocity->x;
	//	}
	//}
	//else {
	//	velocity->y += acceleration->y;
	//	position->y += velocity->y;
	//	velocity->x += acceleration->x;
	//	position->x += velocity->x;
	//}

	if (pos_differential.y != 0) {
		if (checkCollisions(&newCollider))
			colliding_y = true;
	}

	if (pos_differential.x != 0) {
		if (checkCollisions(&newCollider)) {
			if (colliding_y) {
				if (intersection.x != position->x)
					colliding_x = true;
			}
			else
				colliding_x = true;
		}
	}

	if (!colliding_y) {
		velocity->y += acceleration->y;
		position->y += velocity->y;
	}
	if (!colliding_x) {
		velocity->x += acceleration->x;
		position->x += velocity->x;
	}
}

Collider* j1Physics::AddCollider(SDL_Rect *rect, const COLLIDER_TYPE type)
{
	Collider *pCollider = nullptr;

	pCollider = new Collider(rect, type);
	collider_list.add(pCollider);

	return pCollider;
}

bool j1Physics::checkCollisions(Collider* object_col)
{
	p2List_item<Collider*>* collider_iterator_b;

	SDL_Rect* rect_a = nullptr;
	SDL_Rect* rect_b = nullptr;

	int col_count = collider_list.count();


	rect_a = object_col->rect;

	for (collider_iterator_b = collider_list.start; collider_iterator_b != NULL; collider_iterator_b = collider_iterator_b->next)
	{
		rect_b = collider_iterator_b->data->rect;

		if (rect_b != rect_a) {
			bool intersect = SDL_IntersectRect(rect_a, rect_b, &intersection);
			if (intersect)
				return true;
		}
	}

	return false;
}


Collider::Collider(SDL_Rect *rectangle, COLLIDER_TYPE type)
{
	this->rect = rectangle;
	visble = true;
	this->type = type;
}
