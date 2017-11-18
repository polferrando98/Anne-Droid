
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

	//fPoint maxVelocity;
	float acceleration_x;
	float jump_speed;

	Direction_x colliding_x = NONE_X;
	Direction_y y_axis_collision = NONE_Y;

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


};


#endif
