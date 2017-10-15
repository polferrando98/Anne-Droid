
#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "j1Module.h"
#include "p2Point.h"
#include "Animation.h"
#include "Globals.h"
#include "j1Physics.h"

struct SDL_Texture;
struct Mix_Chunk;
class _Mix_Music;
typedef _Mix_Music Mix_Music;
struct Collider;


class j1Player : public j1Module
{
public:
	j1Player();
	virtual ~j1Player();

	bool Start();
	bool CleanUp();
	bool Update(float dt);

	void ApplyFriction();
	void ApplyMaxVelocity();

	fPoint position;
	fPoint velocity;
	fPoint acceleration;
	Collider *player_coll;
	
	//HARDCODE
	fPoint maxVelocity;
	float friction_x = 0.5;
	float acceleration_x = 0.3;
	float gravity = 0.5;
	float jump_speed = 12;

private:
	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	Animation idle;
	Animation jump;
	Animation jump_left;
	Animation left;
	Animation right;

	/////////HARDCODE

	SDL_Rect collider_rect;
};


#endif
