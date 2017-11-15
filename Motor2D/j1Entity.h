#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "p2Point.h"
#include "p2SString.h"
#include "Animation.h"
#include "j1Physics.h"
#include "PugiXml\src\pugixml.hpp"

class j1App;

enum LifeState { ALIVE, WAITING_FOR_CLEANUP};
enum EntityType {ENTITY_PLAYER, ENTITY_WALKER};

class Entity 
{
private:
	bool enabled = true;

public:

	Entity(fPoint position, EntityType type);

	void Init()
	{

	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start();

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt);

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool save(pugi::xml_node&) const
	{
		return true;
	}

public:
	p2SString		name;
	LifeState		life_state;

	fPoint			position;
	fPoint			velocity;
	fPoint			acceleration;
	fPoint			movement_acceleration;

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

};

#endif // __j1ENTITY_H__