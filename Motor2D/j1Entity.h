#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "p2Point.h"
#include "p2SString.h"
#include "Animation.h"
#include "j1Physics.h"
#include "PugiXml\src\pugixml.hpp"

class j1App;

enum LifeState { ALIVE, WAITING_FOR_CLEANUP, DEAD};
enum EntityType { ENTITY_PLAYER, ENTITY_WALKER, ENTITY_BIRD};

class Entity 
{
private:
	bool enabled = true;

public:

	Entity(fPoint position, EntityType type);

	// Called before the first frame
	virtual bool Start();

	// Called each loop iteration
	virtual bool Update(float dt);

	virtual bool PreUpdate(float dt);

	void UpdateCurrentTile();
	virtual void Respawn() {};

public:
	p2SString		name;
	LifeState		life_state;

	fPoint			start_position;
	fPoint			position;
	fPoint			velocity;
	fPoint			acceleration;
	fPoint			movement_acceleration;

	iPoint			current_tile;

	Collider*		collider = nullptr;
	EntityType		type;

	Direction_x		x_axis_collision = NONE_X;
	Direction_y		y_axis_collision = NONE_Y;

	bool			grounded = false;

	iPoint destination_tile = { -1,-1 };


protected:
	SDL_Texture*	texture = nullptr;
	Animation		idle_right;
	Animation		idle_left;
	Animation		left;
	Animation		right;
	Animation		death;
	Animation*		current_animation = nullptr;

	
	float animation_speed;

};

#endif // __j1ENTITY_H__