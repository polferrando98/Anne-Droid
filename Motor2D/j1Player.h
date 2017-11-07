
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

	void createCol(SDL_Rect* rect);

	bool load(pugi::xml_node&save);
	bool save(pugi::xml_node&save) const;

	fPoint position;
	fPoint velocity;
	fPoint acceleration;
	Collider *player_coll;
	bool grounded = false;
	bool double_jump_avaliable = true;
	
	//HARDCODE
	fPoint maxVelocity;
	float acceleration_x = 0.3f;
	float gravity = 0.5;
	float jump_speed = 15;

	DIRECTION_X colliding_x = NONE_X;
	DIRECTION_Y colliding_y = NONE_Y;

	DIRECTION_X last_direction_x = NONE_X;
	DIRECTION_Y last_direction_y = NONE_Y;

	DIRECTION_X current_direction_x = NONE_X;
	DIRECTION_Y current_direction_y = NONE_Y;


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

	float animation_speed = 0.19f;

	/////////HARDCODE

	int speed = 5;
	int player_min_y;
	int direction;

};


#endif
