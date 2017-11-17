#pragma once
#ifndef __j1PHYSICS_H__
#define __j1PHYSICS_H__

#include "SDL/include/SDL.h"
#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"


enum Collider_Type {PLAYER, WALL, DEATH, DOOR, ICE, COL_WALKER};
enum Direction_x { NONE_X, LEFT, RIGHT};
enum Direction_y { NONE_Y, UP, DOWN };
enum Axis {BOTH_AXIS, X_axis, Y_axis};

struct Collider{
	Collider(SDL_Rect *rectangle, Collider_Type type, float friction);
	void UpdatePosition(fPoint newPos);
	SDL_Rect rect;
	Collider_Type type;
	bool visble;
	float friction = 0;
};

class Entity;
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

	void DebugDraw() const;

	void UpdateEntityPhysics(Entity &entity, float dt);
	void UpdatePlayerPhysics(fPoint &position, fPoint &velocity, Collider* collider, Direction_x & colliding_x, Direction_y & colliding_y);
	void  ManageGroundCollisions(fPoint *position, fPoint *velocity, Collider* collider, Direction_x& colliding_x, Direction_y&colliding_y);
	Direction_x checkGroundXCollisions(Collider new_collider, fPoint pos_differential) const;
	Direction_y checkGroundYCollisions(Collider new_collider, fPoint pos_differential) const;
	fPoint calculateNewPosition(fPoint position, fPoint velocity, fPoint acceleration, Axis axis) const;
	void  checkDeathCollisions(fPoint * position, fPoint & velocity, Collider * collider);
	void CheckDoorEntry(fPoint & position, fPoint & velocity, Collider * collider);

	Collider* AddCollider(SDL_Rect *rect, const Collider_Type type);

	bool checkColliders(Collider object_col, Collider_Type type_to_ignore) const;

	inline bool SameType(Collider_Type type_1, Collider_Type type_2) const;

	void ApplyFriction(fPoint & velocity, fPoint & acceleration);

	void LoadPhysicsValues();

private:


	SDL_Rect empty_rect = { 0,0,0,0 };
	Collider* collided = nullptr;

public:

	p2List<Collider*>	collider_list;
	bool debug_mode = true;

	
	Uint8 alpha = 80;

	float friction;
	float extra_friction;
	float extra_friction_2;


private:
	pugi::xml_document	physics_file;
};



#endif // __j1MAP_H__