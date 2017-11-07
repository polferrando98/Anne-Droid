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

	p2List_item<Collider*>* item_collider;
	item_collider = collider_list.start;

	while (item_collider != NULL)
	{
		RELEASE(item_collider->data);
		item_collider = item_collider->next;
	}

	collider_list.clear();
	return true;
}

bool j1Physics::Update(float dt)
{
	if ((App->input->GetKey(SDL_SCANCODE_9) == KEY_DOWN))
		debug_mode = !debug_mode;
	if (debug_mode)
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

//The idea was that it could be used for any moving object, but in the case of this game it is just the player
void j1Physics::UpdatePlayerPhysics(fPoint *position, fPoint *velocity, fPoint *acceleration, Collider* collider, DIRECTION_X* colliding_x, DIRECTION_Y* colliding_y) 
{
	checkWallCollisions(position, velocity, acceleration, collider, colliding_x, colliding_y);
	checkDeathCollisions(position, velocity, acceleration, collider);
	CheckDoorEntry(position, velocity, acceleration, collider);
}

void j1Physics::checkWallCollisions(fPoint *position, fPoint *velocity, fPoint *acceleration, Collider* collider, DIRECTION_X* colliding_x, DIRECTION_Y* colliding_y)
{
	Collider newCollider = *collider;
	fPoint newVelocity;
	fPoint newPosition;
	fPoint pos_differential;

	//The idea is to go one axis at a time

	//X_AXIS
	newVelocity.x = velocity->x + acceleration->x;
	newPosition.x = position->x + newVelocity.x;
	newCollider.rect.x = newPosition.x;
	pos_differential.x = newPosition.x - position->x;

	if (checkColliders(&newCollider, WALL)) {
		if (pos_differential.x > 0)
			*colliding_x = RIGHT;
		if (pos_differential.x < 0)
			*colliding_x = LEFT;
	}

	if (*colliding_x == RIGHT || *colliding_x == LEFT) {
		velocity->x = 0;
	}

	newCollider.rect.x = position->x;

	if (*colliding_x != RIGHT && *colliding_x != LEFT) {
		velocity->x += acceleration->x;
		position->x += velocity->x;
	}

	//Y_AXIS
	newVelocity.y = velocity->y + acceleration->y;
	newPosition.y = position->y + newVelocity.y;
	newCollider.rect.y = newPosition.y;
	pos_differential.y = newPosition.y - position->y;

	if (checkColliders(&newCollider, WALL)) {
		if (pos_differential.y > 0)
			*colliding_y = DOWN;
		if (pos_differential.y < 0)
			*colliding_y = UP;
	}

	if (*colliding_y == UP || *colliding_y == DOWN) {
		velocity->y = 0;
		//friction = collided->friction;
	}
	else
		friction = 0.5;

	if (*colliding_y != UP && *colliding_y != DOWN) {
		velocity->y += acceleration->y;
		position->y += velocity->y;
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


	if (checkColliders(&newCollider, DEATH)) {
		position->x = App->map->data.player_start_position.x;
		position->y = App->map->data.player_start_position.y;
	}
}

void j1Physics::CheckDoorEntry(fPoint * position, fPoint * velocity, fPoint * acceleration, Collider * collider)
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


	if (checkColliders(&newCollider, DOOR)) {
		App->scene->ChangeMap();
	}
}

Collider* j1Physics::AddCollider(SDL_Rect *rect, const COLLIDER_TYPE type)
{
	Collider *pCollider = nullptr;

	pCollider = new Collider(rect, type,friction);
	collider_list.add(pCollider);

	return pCollider;
}

void j1Physics::ApplyFriction(fPoint* velocity, fPoint* acceleration)
{
	if (abs(velocity->x) != 0) {
		if (velocity->x > 0)
			acceleration->x = -friction;
		else if (velocity->x < 0)
			acceleration->x = +friction;

		if (abs(velocity->x) <= friction) {
			if (velocity->x > 0)
				acceleration->x = -0.1;
			else if (velocity->x < 0)
				acceleration->x = +0.1;

			if (abs(velocity->x) < 0.01) {
				if (velocity->x > 0)
					acceleration->x = -0.001;
				else if (velocity->x < 0)
					acceleration->x = +0.001;
			}
		}
	}
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
		if (collider_iterator_b->data->type != object_col->type && collider_iterator_b->data->type == type_to_collide) {  
			rect_b = collider_iterator_b->data->rect;

			if (!rectsAreEqual(rect_a, rect_b)) {
				bool intersect = SDL_IntersectRect(&rect_a, &rect_b, &intersection);
				if (intersect) {
					return true;
					collided = collider_iterator_b->data;
				}
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

Collider::Collider(SDL_Rect *rectangle, COLLIDER_TYPE type, float friction)
{
	this->rect = *rectangle;
	visble = true;
	this->type = type;
	this->friction = friction;
}

void Collider::UpdatePosition(fPoint *newPos)
{
	rect.x = (int)newPos->x;
	rect.y = (int)newPos->y;
}
