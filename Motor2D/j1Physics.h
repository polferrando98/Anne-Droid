#pragma once
#ifndef __j1PHYSICS_H__
#define __j1PHYSICS_H__

#include "SDL/include/SDL.h"
#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

enum COLLIDER_TYPE {PLAYER, WALL};

struct Collider{
	Collider(SDL_Rect *rectangle, COLLIDER_TYPE type);
	SDL_Rect *rect;
	COLLIDER_TYPE type;
	bool visble;
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

	void UpdatePhysics(fPoint *position, fPoint *velocity, fPoint *acceleration, Collider* collider);

	Collider* AddCollider(SDL_Rect *rect, const COLLIDER_TYPE type);

	void checkCollisions(Collider* object_col);


private:

	SDL_Rect* intersection = nullptr;

public:

	p2List<Collider*>	collider_list;


	//HARDCODING
	float gravity = 9.8f;
	Uint8 alpha = 80;

private:

};



#endif // __j1MAP_H__