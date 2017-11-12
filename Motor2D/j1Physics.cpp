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
		DebugDraw();

	return UPDATE_CONTINUE;
}

void j1Physics::DebugDraw() const
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
			case COL_WALKER:
				App->render->DrawQuad(pCollider->rect, 255, 0, 0, alpha, true, true); //Red
				break;
			case DOOR:
				App->render->DrawQuad(pCollider->rect, 102, 51, 0, alpha, true, true); //Brown
				break;
			default:
				App->render->DrawQuad(pCollider->rect, 102, 102, 102, alpha, true, true); //Grey
				break;
			}
		}
	}
}

void j1Physics::UpdateEntityPhysics(Entity & entity)
{
}

//The idea was that it could be used for any moving object, but in the case of this game it is just the player
void j1Physics::UpdatePlayerPhysics(fPoint &position, fPoint &velocity, fPoint &acceleration, Collider* collider, Direction_x &colliding_x, Direction_y & colliding_y) 
{
	ManageGroundCollisions(&position, &velocity, acceleration, collider, colliding_x, colliding_y);
	checkDeathCollisions(&position, velocity, acceleration, collider);
	CheckDoorEntry(position, velocity, acceleration, collider);
}

void j1Physics::ManageGroundCollisions(fPoint *position, fPoint *velocity, fPoint acceleration, Collider* collider, Direction_x& colliding_x, Direction_y& colliding_y)
{
	Collider newCollider = *collider;
	fPoint newPosition;
	fPoint pos_differential;

	//The idea is to go one axis at a time

	//X_AXIS
	newPosition = calculateNewPosition(*position, *velocity, acceleration, X_axis);

	newCollider.rect.x = newPosition.x;
	pos_differential.x = newPosition.x - position->x;

	colliding_x = checkGroundXCollisions(newCollider, pos_differential);

	if (colliding_x == RIGHT || colliding_x == LEFT) {
		velocity->x = 0;
	}
	else {
		velocity->x += acceleration.x;
		position->x += velocity->x;
	}

	newCollider.rect.x = position->x; //if this is commented player gets stuck to walls

	//Y_AXIS
	newPosition = calculateNewPosition(*position, *velocity, acceleration, Y_axis);

 	newCollider.rect.y = newPosition.y;
	pos_differential.y = newPosition.y - position->y;

	colliding_y = checkGroundYCollisions(newCollider, pos_differential);

	if (colliding_y == UP || colliding_y == DOWN) {
		velocity->y = 0;
		//friction = collided->friction;
	}
	else {
		velocity->y += acceleration.y;
		position->y += velocity->y;
	}
}

Direction_x j1Physics::checkGroundXCollisions(Collider new_collider, fPoint pos_differential) const
{
	Direction_x colliding_x = NONE_X;
	if (checkColliders(new_collider, WALL)) {
		if (pos_differential.x > 0)
			colliding_x = RIGHT;
		if (pos_differential.x < 0)
			colliding_x = LEFT;
	}
	return colliding_x;
}

Direction_y j1Physics::checkGroundYCollisions(Collider new_collider, fPoint pos_differential) const
{
	Direction_y colliding_y = NONE_Y;
	if (checkColliders(new_collider, WALL)) {
		if (pos_differential.y > 0)
			colliding_y = DOWN;
		if (pos_differential.y < 0)
			colliding_y = UP;
	}
	return colliding_y;
}



fPoint j1Physics::calculateNewPosition(fPoint position, fPoint velocity, fPoint acceleration, Axis axis = BOTH_AXIS) const
{
	fPoint newVelocity;
	fPoint newPosition;

	switch (axis)
	{
	case BOTH_AXIS:
		newVelocity.x = velocity.x + acceleration.x;
		newPosition.x = position.x + newVelocity.x;
		newVelocity.y = velocity.y + acceleration.y;
		newPosition.y = position.y + newVelocity.y;
		break;
	case X_axis:
		newVelocity.x = velocity.x + acceleration.x;
		newPosition.x = position.x + newVelocity.x;
		break;
	case Y_axis:
		newVelocity.y = velocity.y + acceleration.y;
		newPosition.y = position.y + newVelocity.y;
		break;
	}

	return newPosition;
}

void j1Physics::checkDeathCollisions(fPoint * position, fPoint & velocity, fPoint & acceleration, Collider * collider)
{
	Collider newCollider = *collider;
	bool colliding_x = false;

	fPoint newVelocity;
	fPoint newPosition;

	newVelocity.x = velocity.x + acceleration.x;
	newPosition.x = position->x + newVelocity.x;
	newCollider.rect.x = newPosition.x;


	newVelocity.y = velocity.y + acceleration.y;
	newPosition.y = position->y + newVelocity.y;
	newCollider.rect.y = newPosition.y;


	if (checkColliders(newCollider, DEATH)) {
		position->x = App->map->data.player_start_position.x;
		position->y = App->map->data.player_start_position.y;
	}
}

void j1Physics::CheckDoorEntry(fPoint & position, fPoint & velocity, fPoint & acceleration, Collider * collider)
{
	Collider newCollider = *collider;
	bool colliding_x = false;

	fPoint newVelocity;
	fPoint newPosition;

	newVelocity.x = velocity.x + acceleration.x;
	newPosition.x = position.x + newVelocity.x;
	newCollider.rect.x = newPosition.x;

	newVelocity.y = velocity.y + acceleration.y;
	newPosition.y = position.y + newVelocity.y;
	newCollider.rect.y = newPosition.y;

	if (checkColliders(newCollider, DOOR)) {
		App->scene->ChangeMap();
	}
}

Collider* j1Physics::AddCollider(SDL_Rect *rect, const Collider_Type type)
{
	Collider *pCollider = nullptr;

	pCollider = new Collider(rect, type,100.0f);
	collider_list.add(pCollider);

	return pCollider;
}

bool j1Physics::checkColliders(Collider object_col, Collider_Type type_to_collide) const
{
	p2List_item<Collider*>* collider_iterator_b;

	SDL_Rect rect_a;
	SDL_Rect rect_b;

	SDL_Rect intersection = { 0,0,0,0 };

	int col_count = collider_list.count();



	rect_a = object_col.rect;

	for (collider_iterator_b = collider_list.start; collider_iterator_b != NULL; collider_iterator_b = collider_iterator_b->next)
	{
		if (collider_iterator_b->data->type != object_col.type && collider_iterator_b->data->type == type_to_collide) {  
			rect_b = collider_iterator_b->data->rect;

			if (!SameType(object_col.type, collider_iterator_b->data->type)) {
				bool intersect = SDL_IntersectRect(&rect_a, &rect_b, &intersection);
				if (intersect) {
					return true;
					//collided = collider_iterator_b->data; I dont know what this did
				}
			}
		}
		
	}

	return false;
}

inline bool j1Physics::SameType(Collider_Type type_1, Collider_Type type_2) const
{
	bool ret = true;
	if (type_1 != type_2)
		ret = false;
	return ret;
}

Collider::Collider(SDL_Rect *rectangle, Collider_Type type, float friction)
{
	this->rect = *rectangle;
	visble = true;
	this->type = type;
	this->friction = friction;
}

void Collider::UpdatePosition(fPoint newPos)
{
	rect.x = (int)newPos.x;
	rect.y = (int)newPos.y;
}


void j1Physics::ApplyFriction(fPoint* velocity, fPoint* acceleration)
{
	TileSet* set = App->map->data.tilesets.At(0)->data;
	Tile* tile = set->tiles.At(0)->data;
	friction = tile->friction;

	if (abs(velocity->x) != 0) {
		if (velocity->x > 0)
			acceleration->x = -friction;
		else if (velocity->x < 0)
			acceleration->x = +friction;

		if (abs(velocity->x) <= friction) {
			if (velocity->x > 0)
				acceleration->x = -extra_friction;
			else if (velocity->x < 0)
				acceleration->x = +extra_friction;

			if (abs(velocity->x) < extra_friction) {
				if (velocity->x > 0)
					acceleration->x = -extra_friction_2;
				else if (velocity->x < 0)
					acceleration->x = +extra_friction_2;
			}
		}
	}
}