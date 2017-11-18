#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "p2Point.h"
#include "p2SString.h"
#include "Animation.h"
#include "j1Physics.h"
#include "PugiXml\src\pugixml.hpp"

class j1App;

enum LifeState { ALIVE, WAITING_FOR_CLEANUP};
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

	void UpdateCurrentTile();

	iPoint getDestination();
	void SetDestination(iPoint dest);

public:
	p2SString		name;
	LifeState		life_state;

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




protected:
	SDL_Texture*	texture = nullptr;
	Animation		idle_right;
	Animation		idle_left;
	Animation		left;
	Animation		right;
	Animation		death;
	Animation*		current_animation = nullptr;

	
	float animation_speed;


private:
	iPoint destination_tile = { -1,-1 };
};

#endif // __j1ENTITY_H__