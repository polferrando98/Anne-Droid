
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
	//HARDCODE 
	float friction; 
	const float extra_friction = 0.2f;
	const float extra_friction_2 = 0.002f;
	
	j1Player();
	virtual ~j1Player();

	bool Start();
	bool CleanUp();
	bool Update(float dt);

	void ApplyMaxVelocity();

	void createCol(SDL_Rect* rect);

	bool load(pugi::xml_node&save);
	bool save(pugi::xml_node&save) const;

	void DoJump();

	fPoint position;
	fPoint velocity;
	fPoint acceleration;
	Collider *player_coll;
	bool grounded = false;
	bool double_jump_avaliable = true;
	
	//HARDCODE
	fPoint maxVelocity;
	float acceleration_x = 10.0f;
	float jump_speed = 200.0f;

	Direction_x colliding_x = NONE_X;
	Direction_y colliding_y = NONE_Y;

	Direction_x last_direction_x = NONE_X;
	Direction_y last_direction_y = NONE_Y;

	Direction_x current_direction_x = NONE_X;
	Direction_y current_direction_y = NONE_Y;


private:
	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	Animation idle;
	Animation idle_left;
	Animation jump;
	Animation jump_left;
	Animation left;
	Animation right;
	Animation death;

	/////////HARDCODE

	int speed = 5;
	int player_min_y;
	int direction;

};


#endif
