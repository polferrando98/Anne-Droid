

#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Scene.h"

#include <math.h>
#include "j1Audio.h"
#include "SDL/include/SDL_timer.h"
#include "j1FadeToBlack.h"
#include "Animation.h"
#include "p2Log.h"

j1Player::j1Player()
{
	graphics = NULL;
	current_animation = NULL;
	
	//idle animation
	idle.PushBack({90,35,208,353});

	idle.PushBack({ 90,35,208,353});
	idle.PushBack({ 90,35,208,353 });
	idle.PushBack({ 90,35,208,353 });
	idle.PushBack({ 90,35,208,353 });
	idle.PushBack({ 90,35,208,353 });
	idle.PushBack({ 90,35,208,353 });
	idle.PushBack({ 90,35,208,353 });
	idle.PushBack({ 90,35,208,353 });
	idle.loop = true;
	idle.speed = 1.0f;

}

j1Player::~j1Player()
{}

// Load assets
bool j1Player::Start()
{
	current_animation = &idle;
	position.x = 5;
	position.y = 500;
	
	direction = 0;
	state = IDLE;
	
	LOG("Loading player textures");
	graphics = App->tex->Load("textures/player_sprites.png");

	return true;
}

bool j1Player::CleanUp() {
	LOG("Unloading player");

	App->tex->UnLoad(graphics);
	return true;
}


bool j1Player::Update(float dt)
{
	// Draw everything --------------------------------------
	AnimationFrame frame = current_animation->GetCurrentFrame();
	App->render->Blit(graphics, 0, 2950, &frame.rect);

	return UPDATE_CONTINUE;
}

void j1Player::checkInput() {

	
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == j1KeyState::KEY_REPEAT)
		{
			state = MOVING_RIGHT | state;
	
		}

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == j1KeyState::KEY_REPEAT)
		{
			state = MOVING_LEFT | state;
		
		}

}

void j1Player::processInput() {

	switch (state) {


	case MOVING_RIGHT:

		direction = 90;
		current_animation = &right;
		break;

	case MOVING_LEFT:

		direction = 270;
		current_animation = &left;
		break;
	}
}

	PLAYER_STATE operator |(PLAYER_STATE p, PLAYER_STATE s) {
		PLAYER_STATE ret = static_cast<PLAYER_STATE>((int)p | (int)s);
		return ret;
	}

	iPoint toiPoint(fPoint a) {
		return{ (int)a.x, (int)a.y };
	}

	fPoint tofPoint(iPoint a) {
		return{ (float)a.x, (float)a.y };
	}

