

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
	name.create("player");

	graphics = NULL;
	current_animation = NULL;

	//idle animation

	idle.PushBack({ 50,16,110,193 });
	idle.PushBack({ 291,16,110,193 });
	idle.PushBack({ 532,16,110,193 });
	idle.PushBack({ 773,16,110,193 });
	idle.PushBack({ 1014,16,110,193 });

	idle.PushBack({ 1014,16,110,193 });
	idle.PushBack({ 773,16,110,193 });
	idle.PushBack({ 532,16,110,193 });
	idle.PushBack({ 291,16,110,193 });
	idle.PushBack({ 50,16,110,193 });

	idle.loop = true;
	idle.speed = 0.15f;

	//idle_left

	idle_left.PushBack({1052,2272,110,187});
	idle_left.PushBack({ 811,2272,110,187 });
	idle_left.PushBack({ 570,2272,110,188 });
	idle_left.PushBack({ 328,2271,111,188 });
	idle_left.PushBack({ 87,2271,110,188 });
	idle_left.PushBack({ 1052,2029,110,189 });
	idle_left.PushBack({ 811,2029,110,189 });
	idle_left.PushBack({ 570,2030,110,188 });
	idle_left.PushBack({ 328,2031,111,187 });
	idle_left.PushBack({ 87,2031,110,187 });

	idle_left.loop = true;
	idle_left.speed = 0.15f;


	//right jump
	jump.PushBack({ 64, 507, 115, 186 });
	jump.PushBack({ 292, 505, 131, 193 });
	jump.PushBack({ 520, 504, 136, 180 });
	jump.PushBack({ 520, 504, 136, 180 });
	jump.PushBack({ 758, 504, 137, 178 });
	jump.PushBack({ 998, 505, 141, 172 });
	jump.PushBack({ 1236, 505, 153, 165 });
	jump.PushBack({ 1474, 505, 154, 190 });
	jump.PushBack({ 25, 746, 154, 189 });
	jump.PushBack({ 280, 746, 142, 200 });

	jump.loop = true;
	jump.speed = 0.09f;

	//left jump

	jump_left.PushBack({ 60,1530,154,190 });
	jump_left.PushBack({ 299,1530,153,165 });
	jump_left.PushBack({549,1530,141,172 });
	jump_left.PushBack({ 793,1529,137,178 });
	jump_left.PushBack({ 1032,1529,136,180 });
	jump_left.PushBack({ 1265,1530,131,193 });
	jump_left.PushBack({ 1509,1532,115,186 });


	jump_left.PushBack({ 1020,1772,132,198 });
	jump_left.PushBack({ 1226,1771,142,200 });
	jump_left.PushBack({1509,1771,154,189});
	
	jump_left.loop = true;
	jump_left.speed = 0.09f;

	

	right.PushBack({ 58, 985, 103, 195 });
	right.PushBack({ 289, 986, 103, 198 });
	right.PushBack({ 527, 986, 131, 186 });
	right.PushBack({ 762, 985, 124, 192 });
	right.PushBack({ 1018, 985, 119, 192 });
	right.PushBack({ 1249, 985, 135, 200 });
	right.PushBack({ 1477, 986, 147, 189 });

	right.loop = true;
	right.speed = 0.12f;

	left.PushBack({ 533, 1245, 135, 200 });
	left.PushBack({ 293, 1246, 147, 189 });
	left.PushBack({ 58, 1247, 126, 191 });
	left.PushBack({ 780, 1245, 119, 192 });
	left.PushBack({ 1031, 1245, 124, 192 });
	left.PushBack({ 1259, 1246, 131, 186 });
	left.PushBack({ 1525, 1246, 103, 198 });
	left.PushBack({ 1755, 1245, 108, 195 });
	
	
	left.loop = true;
	left.speed = 0.12f;


	//death
	death.PushBack({98,2470,111,212});
	death.PushBack({ 234,2470,114,215 });
	death.PushBack({ 350,2470,129,221});
	death.PushBack({ 483,2470,137,225 });
	death.PushBack({ 620,2470,140,221 });
	death.PushBack({ 760,2470,120,220 });
	death.PushBack({ 880,2470,120,220});
	death.PushBack({ 1000,2470,120,220 });
	death.PushBack({ 1172,2470,120,220 });

	death.loop = false;
	death.speed = 0.15f;

	////////////////HARDCODE
	position.x = 0;
	position.y = 3000;

	velocity.x = 0;
	velocity.y = 0;

	////GRAVITY
	acceleration.x = 0;
	acceleration.y = gravity;


	collider_rect.h = 185;
	collider_rect.w = 100;
	collider_rect.x = position.x;
	collider_rect.y = position.y;

	maxVelocity.x = 15;
}

j1Player::~j1Player()
{

}

// Load assets
bool j1Player::Start()
{
	current_animation = &idle;


	/*direction = 1;*/


	LOG("Loading player textures");
	graphics = App->tex->Load("textures/player_sprites.png");

	player_coll = App->physics->AddCollider(&collider_rect, COLLIDER_TYPE::PLAYER);

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
	current_animation = &idle;
	//if (direction == 1)
	//	current_animation = &idle;
	//else
	//	current_animation = &idle_left;
	//App->physics->UpdatePhysics(&position, &velocity, &acceleration, player_coll);
	



	App->physics->UpdatePlayerPhysics(&position, &velocity, &acceleration, player_coll);
	
	//death animatiton test
	/*if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN) 
		
		App->player->current_animation = &death;*/

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		/*direction = -1;*/
		App->player->current_animation = &left;
		acceleration.x = -acceleration_x;


		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		{
			velocity.y = -jump_speed;
			acceleration.y = gravity;
			acceleration.x = 0;
			App->player->current_animation = &jump_left;

		}
	

	}

	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		/*direction = 1;*/
		App->player->current_animation = &right;
		acceleration.x = acceleration_x;
	//	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	//	{
	//		velocity.y = -jump_speed;
	//		acceleration.y = gravity;
	//		acceleration.x = 0;
	//		App->player->current_animation = &jump;
	//	}
	//}
	//
	//else if (current_animation == &idle || current_animation == &idle_left ) {
	//	ApplyFriction();
	//	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	//	{
	//		velocity.y = -jump_speed;
	//		acceleration.y = gravity;
	//		acceleration.x = 0;
	//		if(current_animation == &idle)
	//			App->player->current_animation = &jump;
	//		else
	//			App->player->current_animation = &jump_left;
	//	}
	//}


	
	
	
	}
	
	else {
		ApplyFriction();

	}


	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		velocity.y = -jump_speed;
		acceleration.y = gravity;
		acceleration.x = 0;
		App->player->current_animation = &jump_left;
	}

	ApplyMaxVelocity();

	player_coll->UpdatePosition(&position);
	App->render->Blit(graphics, position.x, position.y, &frame.rect);

	return UPDATE_CONTINUE;

}

void j1Player::ApplyFriction() {
	if (abs(velocity.x) != 0) {
		if (velocity.x > 0)
			acceleration.x = -friction_x;
		else if (velocity.x < 0)
			acceleration.x = +friction_x;

		if (abs(velocity.x) <= friction_x) {
			if (velocity.x > 0)
				acceleration.x = -0.1;
			else if (velocity.x < 0)
				acceleration.x = +0.1;

			if (abs(velocity.x) < 0.01) {
				if (velocity.x > 0)
					acceleration.x = -0.001;
				else if (velocity.x < 0)
					acceleration.x = +0.001;
			}
		}
	}
}

void j1Player::ApplyMaxVelocity()
{
	if (velocity.x != 0) {
		if (velocity.x > maxVelocity.x)
			velocity.x = maxVelocity.x;
		else if (velocity.x < -maxVelocity.x)
			velocity.x = -maxVelocity.x;
	}
}
