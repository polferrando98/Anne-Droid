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
			switch (pCollider->type)
			{
			case PLAYER:
				App->render->DrawQuad(pCollider->rect, 0, 255, 0, alpha, true, true); //Green
				break;
			case WALL:
				App->render->DrawQuad(pCollider->rect, 0, 0, 255, alpha, true, true); //Blue
				break;
			case DEATH:
				App->render->DrawQuad(pCollider->rect, 255, 0, 0, alpha, true, true); //Red
				break;
			case DOOR:
				App->render->DrawQuad(pCollider->rect, 102, 51, 0, alpha, true, true); //Brown
				break;
			}
		}
	}
}

void j1Physics::UpdatePlayerPhysics(fPoint * position, fPoint * velocity, fPoint * acceleration, Collider* collider)  //If it was another game, this could be adapted for any moving object
{
	checkWallCollisions(position, velocity, acceleration, collider);
	checkDeathCollisions(position, velocity, acceleration, collider);
}

void j1Physics::checkWallCollisions(fPoint * position, fPoint * velocity, fPoint * acceleration, Collider* collider)
{
		Collider newCollider = *collider;
	bool colliding_x = false;
	bool colliding_y = false;

	fPoint newVelocity;
	fPoint newPosition;
	fPoint pos_differential;

	newVelocity.x = velocity->x + acceleration->x;
	newPosition.x = position->x + newVelocity.x;
	newCollider.rect.x = newPosition.x;
	pos_differential.x = newPosition.x - position->x;
	
	if (pos_differential.x != 0) {
		if (checkColliders(&newCollider,WALL))
			colliding_x = true;
	}

	newCollider.rect.x = position->x;

	newVelocity.y = velocity->y + acceleration->y;
	newPosition.y = position->y + newVelocity.y;
	newCollider.rect.y = newPosition.y;
	pos_differential.y = newPosition.y - position->y;

	if (pos_differential.y != 0) {
		if (checkColliders(&newCollider,WALL))
			colliding_y = true;
	}
	
	if (!colliding_y) {
		velocity->y += acceleration->y;
		position->y += velocity->y;
	}
	else {
		velocity->y = 0;
	}

	if (!colliding_x) {
		velocity->x += acceleration->x;
		position->x += velocity->x;
	}
	else {
		velocity->x = 0;
	}
}

void j1Physics::checkDeathCollisions(fPoint * position, fPoint * velocity, fPoint * acceleration, Collider * collider)
{
	Collider newCollider = *collider;
	bool colliding_x = false;


	fPoint newVelocity;
	fPoint newPosition;

	newVelocity.x = velocity->x + acceleration->x;
	newPosition.x = position->x + newVelocity.x;
	newCollider.rect.x = newPosition.x;


	newVelocity.y = velocity->y + acceleration->y;
	newPosition.y = position->y + newVelocity.y;
	newCollider.rect.y = newPosition.y;


	if (checkColliders(&newCollider,DEATH))
			colliding_x = true;
}

Collider* j1Physics::AddCollider(SDL_Rect *rect, const COLLIDER_TYPE type)
{
	Collider *pCollider = nullptr;

	pCollider = new Collider(rect, type);
	collider_list.add(pCollider);

	return pCollider;
}

bool j1Physics::checkColliders(Collider* object_col, COLLIDER_TYPE type_to_collide)
{
	p2List_item<Collider*>* collider_iterator_b;

	SDL_Rect rect_a;
	SDL_Rect rect_b;

	int col_count = collider_list.count();


	rect_a = object_col->rect;

	for (collider_iterator_b = collider_list.start; collider_iterator_b != NULL; collider_iterator_b = collider_iterator_b->next)
	{
		if (collider_iterator_b->data->type != object_col->type && collider_iterator_b->data->type == type_to_collide) {  //HARDCODE
			rect_b = collider_iterator_b->data->rect;

			if (!rectsAreEqual(rect_a, rect_b)) {
				bool intersect = SDL_IntersectRect(&rect_a, &rect_b, &intersection);
				if (intersect)
					return true;
			}
		}
	}

	return false;
}

bool j1Physics::rectsAreEqual(SDL_Rect rect_a, SDL_Rect rect_b)
{
	bool ret = true;
	if (rect_a.x != rect_b.x)
		ret = false;
	if (rect_a.y != rect_b.y)
		ret = false;
	if (rect_a.w != rect_b.w)
		ret = false;
	if (rect_a.h != rect_b.h)
		ret = false;
	return ret;
}

DIRECTION j1Physics::getDirectionFromIntersection(Collider* source)
{
	
	return DIRECTION();
}


Collider::Collider(SDL_Rect *rectangle, COLLIDER_TYPE type)
{
	this->rect = *rectangle;
	visble = true;
	this->type = type;
}

void Collider::UpdatePosition(fPoint *newPos)
{
	rect.x = (int)newPos->x;
	rect.y = (int)newPos->y;
}
