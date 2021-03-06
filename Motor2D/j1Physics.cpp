#include "j1Physics.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Entity.h"

#include <math.h>
#include "j1Audio.h"
#include "SDL/include/SDL_timer.h"
#include "j1FadeToBlack.h"
#include "Animation.h"
#include "p2Log.h"
#include "PugiXml\src\pugixml.hpp"

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

bool j1Physics::PreUpdate(float dt)
{

	normalize = App->max_fps / (1000 / dt);

	if (normalize < 1) { normalize = 1; }

	return true;
}
bool j1Physics::Update(float dt)
{


	if (App->debug_mode)
	{
		DebugDraw();
	}
	else {
		App->scene->camera_change = true;
	}

	maxVelocity.x = 30;
	return true;
}

void j1Physics::DebugDraw() const
{
	p2List_item<Collider*>* collider_iterator;

	Collider* pCollider = nullptr;

	for (collider_iterator = collider_list.start; collider_iterator != NULL; collider_iterator = collider_iterator->next)
	{
		pCollider = collider_iterator->data;

		if (!App->render->isInsideCam({ pCollider->rect.x,pCollider->rect.y }))
			continue;

		if (pCollider->visble) {
			switch (pCollider->type)
			{
			case PLAYER:
				if (App->god_mode)
					App->render->DrawQuad(pCollider->rect, 244, 235, 66, alpha, true, true); //Yellow
				else
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


bool j1Physics::UpdateEntityPhysics(Entity & entity, float dt)
{

	Collider newCollider = *(entity.collider);
	fPoint newPosition;
	fPoint newVelocity;
	fPoint pos_differential;

	//HARDCODE
	entity.acceleration.y = 1.0f;

	if (entity.type == ENTITY_PLAYER) {
		newPosition = calculateNewPosition(entity.position, entity.velocity, entity.acceleration, BOTH_AXIS);
		if (checkOtherCollisions(newCollider, entity))
			return true;
	}

	//Both Axis

	//Y_AXIS
	newPosition = calculateNewPosition(entity.position, entity.velocity, entity.acceleration, Y_axis);

	newCollider.rect.y = newPosition.y;
	pos_differential.y = newPosition.y - entity.position.y;

	entity.y_axis_collision = checkGroundYCollisions(newCollider, pos_differential, entity);

	if (entity.y_axis_collision == UP || entity.y_axis_collision == DOWN) {
		entity.velocity.y = 0;
		//friction = collided->friction;
	}
	else {
		entity.velocity.y += entity.acceleration.y * normalize;
		entity.position.y += entity.velocity.y * normalize;
	}

	newCollider.rect.y = entity.position.y; //So player wont get stuck to ground

	/*X_AXIS*/
	newPosition = calculateNewPosition(entity.position, entity.velocity, entity.acceleration, X_axis);

	newCollider.rect.x = newPosition.x;
	pos_differential.x = newPosition.x - entity.position.x;

	entity.x_axis_collision = checkGroundXCollisions(newCollider, pos_differential, entity);

	if (entity.x_axis_collision == RIGHT || entity.x_axis_collision == LEFT) {
		entity.velocity.x = 0;
	}
	else {
		entity.velocity.x += entity.acceleration.x * normalize;
		entity.position.x += entity.velocity.x *normalize;
	}

	


	return true;
}

void j1Physics::ApplyMaxVelocity(Entity & entity)
{
	if (entity.velocity.x * normalize != 0) {
		if (entity.velocity.x* normalize > maxVelocity.x)
			entity.velocity.x = maxVelocity.x / normalize;
		else if (entity.velocity.x * normalize < -maxVelocity.x)
			entity.velocity.x = -maxVelocity.x / normalize;
	}
}


Direction_x j1Physics::checkGroundXCollisions(Collider new_collider, fPoint pos_differential, Entity & entity) 
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

Direction_y j1Physics::checkGroundYCollisions(Collider new_collider, fPoint pos_differential, Entity & entity)
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

bool j1Physics::checkOtherCollisions(Collider new_collider, Entity & entity)
{
	bool ret = false;

	if (checkColliders(new_collider, DEATH)) {
		entity.life_state = DEAD;
		ret = true;
	}

	if (Collider* colb = checkColliders(new_collider, COL_GEAR)) {
		ManageGearCollisions(colb);
		ret = true;
	}

	if (checkColliders(new_collider, DOOR)) {
		App->scene->GoToNextLevelOnPostUpdate();
		ret = true;
	}
	return ret;
}


fPoint j1Physics::calculateNewPosition(fPoint position, fPoint velocity, fPoint acceleration, Axis axis = BOTH_AXIS) const
{
	fPoint newVelocity;
	fPoint newPosition;

	switch (axis)
	{
	case BOTH_AXIS:
		newVelocity.x = velocity.x + acceleration.x * normalize;
		newPosition.x = position.x + newVelocity.x * normalize;
		newVelocity.y = velocity.y + acceleration.y * normalize;
		newPosition.y = position.y + newVelocity.y * normalize;
		break;
	case X_axis:
		newVelocity.x = velocity.x + acceleration.x * normalize;
		newPosition.x = position.x + newVelocity.x * normalize;
		break;
	case Y_axis:
		newVelocity.y = velocity.y + acceleration.y * normalize;
		newPosition.y = position.y + newVelocity.y * normalize;
		break;
	}

	return newPosition;
}

Collider* j1Physics::AddCollider(SDL_Rect *rect, const Collider_Type type)
{
	Collider *pCollider = nullptr;

	pCollider = new Collider(rect, type, friction);
	collider_list.add(pCollider);

	return pCollider;
}

Collider* j1Physics::checkColliders(Collider object_col, Collider_Type type_to_collide) const
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
					return collider_iterator_b->data;
				}
			}
		}

	}

	return nullptr;
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


void j1Physics::ApplyFriction(fPoint & velocity, fPoint & acceleration)
{
	TileSet* set = App->map->data.tilesets.At(0)->data;
	Tile* tile = set->tiles.At(0)->data;
	friction = tile->friction;

	if (abs(velocity.x) != 0) {
		if (velocity.x > 0)
			acceleration.x = -friction;
		else if (velocity.x < 0)
			acceleration.x = +friction;

		if (abs(velocity.x) <= friction) {
			if (velocity.x > 0)
				acceleration.x = -extra_friction;
			else if (velocity.x < 0)
				acceleration.x = +extra_friction;

			if (abs(velocity.x) < extra_friction) {
				if (velocity.x > 0)
					acceleration.x = -extra_friction_2;
				else if (velocity.x < 0)
					acceleration.x = +extra_friction_2;
			}
		}
	}
}

void j1Physics::LoadPhysicsValues() {

	pugi::xml_parse_result result = physics_file.load_file("physicsvalues.xml");
	if (result != NULL)
	{
		pugi::xml_node physics = physics_file.child("physics_values").child("physics");
		extra_friction = physics.child("extra_friction").attribute("value").as_float();
		extra_friction_2 = physics.child("extra_friction_2").attribute("value").as_float();

		pugi::xml_node phy_player = physics_file.child("physics_values").child("player");
		//App->player->position.x = phy_player.child("position_x").attribute("value").as_int();
		//App->player->position.y = phy_player.child("position_y").attribute("value").as_int();
		//App->player->acceleration_x = phy_player.child("acceleration_x").attribute("value").as_float();
		//App->player->acceleration.y = phy_player.child("acceleration_y").attribute("value").as_float();
		//App->player->jump_speed = phy_player.child("jump_speed").attribute("value").as_float();
	/*	App->player->maxVelocity.x = phy_player.child("max_velocity").attribute("value").as_int();*/
	}
}

void j1Physics::DeleteGearColliders()
{
	if (!collider_list.start)
		return;

	p2List <Collider*> colliders_to_delete;

	for (p2List_item<Collider*>* colliders_iterator = collider_list.start; colliders_iterator; colliders_iterator = colliders_iterator->next)
	{
		if (!colliders_iterator->data)
			return;
		if (colliders_iterator->data->type == COL_GEAR)
		{
			RELEASE(colliders_iterator->data);
			colliders_to_delete.add(colliders_iterator->data);
		}
	}

	for (p2List_item<Collider*>* colliders_iterator = colliders_to_delete.start; colliders_iterator; colliders_iterator = colliders_iterator->next)
	{
		collider_list.del(collider_list.At(collider_list.find(colliders_iterator->data)));
	}

	colliders_to_delete.clear();
}

void j1Physics::ManageGearCollisions(Collider *gear_col)
{
	p2List_item<Collider*>* colliders_iterator = nullptr;
	p2List_item<Collider*>* collider_to_delete = nullptr;
	for (colliders_iterator = collider_list.start; colliders_iterator; colliders_iterator = colliders_iterator->next)
	{
		if (gear_col == colliders_iterator->data)
		{
			App->scene->DeleteGearPictureFromCollider(gear_col);
			collider_to_delete = colliders_iterator;
		}
	}

	RELEASE(collider_to_delete->data);
	collider_list.del(collider_to_delete);
}

bool j1Physics::DeleteCollider(Collider * collider_to_delete)
{
	if (!collider_list.start)
		return false;
	int index = collider_list.find(collider_to_delete);
	p2List_item<Collider*>* item_to_delete = collider_list.At(index);
	collider_list.del(item_to_delete);
	return true;
}
