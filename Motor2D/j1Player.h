#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "j1Module.h"
#include "p2Point.h"
#include "Animation.h"
#include "Globals.h"

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
	update_status Update();
	void OnCollision(Collider*, Collider*);
	

	PLAYER_STATE state = IDLE;
	
	int intro_state;
	short int final_anim;
	fPoint init_pos;
	float parabol = 0.65f;
	fPoint position;

	iPoint GetPosition() {
		return toiPoint(position);
	}

	
	int level_stage;

private:
	SDL_Texture* graphics = nullptr;
	SDL_Texture* godmode = nullptr;
	Animation* current_animation = nullptr;
	Animation idle;




	Collider* collider_body;
	Collider* collider_feet;
	

	PLAYER_STATE prev_state = IDLE;


	int speed;
	int player_min_y;
	int direction;

	void checkInput();
	void processInput();
	

};

void FinishDeath();
#endif