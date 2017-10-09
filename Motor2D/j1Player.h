#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "j1Module.h"
#include "p2Point.h"


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
	void RemoveReadyLabel();

	PLAYER_STATE state = IDLE;
	bool grenade_on = false;
	bool bthrowing;
	int intro_state;
	short int final_anim;
	fPoint init_pos;
	float parabol = 0.65f;
	Mix_Chunk* grenade_explosion;
	fPoint position;
	iPoint GetPosition() {
		return toiPoint(position);
	}
	int lives;
	int level_stage;
	int prev_level_stage;
	bool restart_anim;
	uint grenades;
	iPoint level_dimensions;
	Label* ready_label;


private:
	SDL_Texture* graphics = nullptr;
	SDL_Texture* godmode = nullptr;
	Animation* current_animation = nullptr;
	Animation forward;
	Animation backward;
	Animation left;
	Animation right;
	Animation up_right;
	Animation up_left;
	Animation down_right;
	Animation down_left;
	Animation death;
	Animation drown;
	Animation leave_heli;
	Animation bye_anim;
	Animation throw_grenade;



	fPoint prev_position;
	Mix_Chunk* shoot;
	Mix_Music* death_music;
	Mix_Music* game_over_music;
	Collider* collider_body;
	Collider* collider_feet;
	Particle fire;	//Fire shot particle

	float grenade_speed;
	bool b_godmode;

	PLAYER_STATE prev_state = IDLE;
	bool shooting;
	bool grenade1;


	int speed;
	int player_min_y;
	int direction;
	iPoint shooting_position;
	fPoint shooting_angle;
	fPoint shooting_angle_delta = { 0.15f, 0.15f };
	int shot_delay_frames = 10, frames_since_last_shot = 0;

	void checkInput();
	void processInput();
	void wallCollision(Collider* self, Collider* other);
	void waterCollision();
	void enemyCollision();
	void rotateShootingAngle();
	void Final();

	void Drown();

};

void FinishDeath();
#endif