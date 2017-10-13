
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

enum PLAYER_STATE : int {
	IDLE = 0,
	JUMP = 1,
	MOVING_RIGHT = 2,
	MOVING_LEFT = 3,
	DEAD = -1
};

PLAYER_STATE operator |(PLAYER_STATE p, PLAYER_STATE s);

fPoint tofPoint(iPoint a);
iPoint toiPoint(fPoint a);

class j1Player : public j1Module
{
public:
	j1Player();
	virtual ~j1Player();

	bool Start();
	bool CleanUp();
	bool Update(float dt);

	PLAYER_STATE state = IDLE;
	fPoint position;
	fPoint velocity;
	fPoint acceleration;
	Collider *player_coll;
	
	//HARDCODE
	fPoint maxVelocity;
	float friction_x = 0.1;

private:
	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	Animation idle;
	Animation jump;
	Animation left;
	Animation right;

	/////////HARDCODE
	int speed = 5;
	int player_min_y;
	int direction;

	SDL_Rect collider_rect;

	void checkInput();
	void processInput();
};


#endif
