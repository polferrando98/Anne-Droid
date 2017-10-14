

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
	jump.speed = 0.15f;


	right.PushBack({ 58, 985, 103, 195 });
	right.PushBack({ 289, 986, 103, 198 });
	right.PushBack({ 527, 986, 131, 186 });
	right.PushBack({ 762, 985, 124, 192 });
	right.PushBack({ 1018, 985, 119, 192 });
	right.PushBack({ 1249, 985, 135, 200 });
	right.PushBack({ 1477, 986, 147, 189 });

	right.loop = true;
	right.speed = 0.12f;


	left.PushBack({ 1755, 1245, 108, 195 });
	left.PushBack({ 1525, 1246, 103, 198 });
	left.PushBack({ 1259, 1246, 131, 186 });
	left.PushBack({ 1031, 1245, 124, 192 });
	left.PushBack({ 780, 1245, 119, 192 });
	left.PushBack({ 533, 1245, 135, 200 });
	left.PushBack({ 293, 1246, 147, 189 });
	left.PushBack({ 58, 1247, 126, 191 });

	left.loop = true;
	left.speed = 0.12f;

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


	direction = 0;

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

	App->physics->UpdatePhysics(&position, &velocity, &acceleration, player_coll);
	current_animation = &idle;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		App->player->current_animation = &left;
		acceleration.x = -acceleration_x;
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		App->player->current_animation = &right;
		acceleration.x = acceleration_x;
	}
	else {
		ApplyFriction();
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
		App->player->current_animation = &jump;
		velocity.y = -jump_speed;
	}

	//to test if animations work properly
	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_REPEAT)
		current_animation = &jump;
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_REPEAT)
		current_animation = &right;
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_REPEAT)
		current_animation = &left;

	if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
		LOG("BREAK!");



	ApplyMaxVelocity();

	player_coll->rect->x = position.x;
	player_coll->rect->y = position.y;

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
				acceleration.x = -0.01;
			else if (velocity.x < 0)
				acceleration.x = +0.01;

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
