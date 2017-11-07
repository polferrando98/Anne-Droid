

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
	idle.speed = animation_speed;

	//idle_left

	idle_left.PushBack({ 1052,2272,110,187 });
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
	idle_left.speed = animation_speed;


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

	jump.loop = false;
	jump.speed = animation_speed;



	//left jump
	jump_left.PushBack({ 1509,1532,115,186 });
	jump_left.PushBack({ 1265,1530,131,193 });
	jump_left.PushBack({ 1032,1529,136,180 });
	jump_left.PushBack({ 793,1529,137,178 });
	jump_left.PushBack({ 549,1530,141,172 });
	jump_left.PushBack({ 299,1530,153,165 });
	jump_left.PushBack({ 60,1530,154,190 });
	jump_left.PushBack({ 1509,1771,154,189 });
	jump_left.PushBack({ 1226,1771,142,200 });
	jump_left.PushBack({ 1020,1772,132,198 });
	
	

	jump_left.loop = false;
	jump_left.speed = animation_speed;



	right.PushBack({ 60, 985, 101, 195 });
	right.PushBack({ 290, 985, 103, 196 });
	right.PushBack({ 525, 985, 130, 187 });
	right.PushBack({ 760, 985, 124, 192 });
	right.PushBack({ 1017, 985, 118, 192 });
	right.PushBack({ 1250, 985, 133, 201 });
	right.PushBack({ 1475, 985, 146, 189 });
	right.PushBack({ 1734, 985, 127, 190 });

	right.loop = true;
	right.speed = animation_speed;

	left.PushBack({ 1755, 1245, 101, 195 });
	left.PushBack({ 1525, 1245, 103, 196 });
	left.PushBack({ 1260, 1245, 130, 187 });
	left.PushBack({ 1032, 1245, 124, 192 });
	left.PushBack({ 780, 1245, 118, 192 });

	left.PushBack({ 530, 1245, 133, 201 });
	left.PushBack({ 294, 1245, 146, 189 });
	left.PushBack({ 60, 1245, 127, 190 });

	left.loop = true;
	left.speed = animation_speed;


	//death
	death.PushBack({ 98,2470,111,212 });
	death.PushBack({ 234,2470,114,215 });
	death.PushBack({ 350,2470,129,221 });
	death.PushBack({ 483,2470,137,225 });
	death.PushBack({ 620,2470,140,221 });
	death.PushBack({ 760,2470,120,220 });
	death.PushBack({ 880,2470,120,220 });
	death.PushBack({ 1000,2470,120,220 });
	death.PushBack({ 1172,2470,120,220 });

	death.loop = false;
	death.speed = animation_speed;

	////////////////HARDCODE
	maxVelocity.x = 20;

	////GRAVITY
	acceleration.y = gravity;
}

j1Player::~j1Player()
{

}

// Load assets
bool j1Player::Start()
{
	current_animation = &idle;

	position.x = App->map->data.player_start_position.x;
	position.y = App->map->data.player_start_position.y;

	/*direction = 1;*/
	acceleration.y = gravity;

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



	App->physics->UpdatePlayerPhysics(&position, &velocity, &acceleration, player_coll, &colliding_x, &colliding_y);

	if (colliding_y == DOWN) {
		grounded = true;
	}


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		acceleration.x = -acceleration_x;
		last_direction_x = LEFT;
		current_direction_x = LEFT;
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		acceleration.x = acceleration_x;
		last_direction_x = RIGHT;
		current_direction_x = RIGHT;
	}
	else if (grounded) {
		current_direction_x = NONE_X;
	}
	else {
		 //Friction in the air is not realistic, but makes the game feel better
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		App->audio->PlayFx(1);
		if (!grounded && double_jump_avaliable) {
			
			jump.current_frame = 0.0f;
			jump_left.current_frame = 0.0f;
			velocity.y = -jump_speed;
			double_jump_avaliable = false;

		}
		if (grounded) {
			jump.current_frame = 0.0f;
			jump_left.current_frame = 0.0f;
			velocity.y = -jump_speed;
			grounded = false;
		}
	}



	ApplyMaxVelocity();

	// Direction


	switch (last_direction_x)
	{
	case LEFT:
		current_animation = &idle_left;
		if (!grounded)
			current_animation = &jump_left;
		break;
	case RIGHT:
		current_animation = &idle;
		if (!grounded)
			current_animation = &jump;
		break;
	}

	if (grounded) {
		switch (current_direction_x)
		{
		case LEFT:
			current_animation = &left;
			break;
		case RIGHT:
			current_animation = &right;
			break;
		default:
			break;
		}
		double_jump_avaliable = true;
	}


	//

	player_coll->UpdatePosition(&position);
	App->render->Blit(graphics, position.x, position.y, &frame.rect);

	colliding_x = NONE_X;
	colliding_y = NONE_Y;

	return UPDATE_CONTINUE;

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

void j1Player::createCol(SDL_Rect* Rect)
{
	player_coll = App->physics->AddCollider(Rect, COLLIDER_TYPE::PLAYER, 0);
}

bool j1Player::load(pugi::xml_node &save)
{
	position.x = save.child("player").attribute("x").as_int();
	position.y = save.child("player").attribute("y").as_int();

	return true;
}

bool j1Player::save(pugi::xml_node &save) const
{
	if (save.child("player") == NULL) {
		save.append_child("player");
	}

	if (save.child("player").attribute("x") == NULL) {
		save.child("player").append_attribute("x") = position.x;
	}
	else {
		save.child("player").attribute("x").set_value(position.x);
	}

	if (save.child("player").attribute("y") == NULL) {
		save.child("player").append_attribute("y") = position.y;
	}
	else {
		save.child("player").attribute("y").set_value(position.y);
	}

	return true;
}
