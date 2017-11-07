#pragma once
#ifndef __j1PHYSICS_H__
#define __j1PHYSICS_H__

#include "SDL/include/SDL.h"
#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

enum COLLIDER_TYPE {PLAYER, WALL, DEATH, DOOR, ICE};
enum DIRECTION_X { NONE_X, LEFT, RIGHT};
enum DIRECTION_Y { NONE_Y, UP, DOWN };

struct Collider{
	Collider(SDL_Rect *rectangle, COLLIDER_TYPE type, float friction);
	void UpdatePosition(fPoint* newPos);
	SDL_Rect rect;
	COLLIDER_TYPE type;
	bool visble;
	float friction = 0;
};


// ----------------------------------------------------
class j1Physics : public j1Module
{
public:

	j1Physics();

	// Destructor
	virtual ~j1Physics();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before quitting
	bool CleanUp();

	bool Update(float dt);

	void Debug_draw() const;

	void UpdatePlayerPhysics(fPoint *position, fPoint *velocity, fPoint *acceleration, Collider* collider, DIRECTION_X* colliding_x, DIRECTION_Y* colliding_y);

	void  CheckGroundCollisions(fPoint *position, fPoint *velocity, fPoint *acceleration, Collider* collider, DIRECTION_X* colliding_x, DIRECTION_Y* colliding_y);

	void  checkDeathCollisions(fPoint *position, fPoint *velocity, fPoint *acceleration, Collider* collider);

	void CheckDoorEntry(fPoint *position, fPoint *velocity, fPoint *acceleration, Collider* collider);

	Collider* AddCollider(SDL_Rect *rect, const COLLIDER_TYPE type);

	bool checkColliders(Collider object_col, COLLIDER_TYPE type_to_ignore);

	bool rectsAreEqual(COLLIDER_TYPE type_1, COLLIDER_TYPE type_2);

private:

	SDL_Rect intersection = { 0,0,0,0 };
	SDL_Rect empty_rect = { 0,0,0,0 };
	Collider* collided = nullptr;

public:

	p2List<Collider*>	collider_list;
	bool debug_mode = false;

	//HARDCODE
	float gravity = 9.8f;
	Uint8 alpha = 80;

	float friction = 1.0f;

private:

};



#endif // __j1MAP_H__