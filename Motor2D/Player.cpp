#include "Player.h"
#include "j1App.h"
#include "j1Textures.h"

#include "j1Input.h"



Player::Player(fPoint position) : Entity(position, ENTITY_PLAYER)
{
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

	current_animation = &idle_left;
}

bool Player::Start()
{
	Entity::Start();
	SDL_Rect colrect = { 0,0,110,193 };

	colrect.x = position.x;
	colrect.y = position.y;

	//HARDCODE


	collider = App->physics->AddCollider(&colrect, PLAYER);
	texture = App->tex->Load("textures/player_sprites.png");

	movement_acceleration.x = 10.0f;

	return true;
}

bool Player::Update(float dt)
{
	App->physics->UpdateEntityPhysics(*this, dt);
	collider->UpdatePosition(position);
	Entity::Update(dt);


	if (y_axis_collision == DOWN) {
		grounded = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		acceleration.x = -movement_acceleration.x;
		//last_direction_x = LEFT;
		//current_direction_x = LEFT;
	}

	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		acceleration.x = movement_acceleration.x;
		//last_direction_x = RIGHT;
		//current_direction_x = RIGHT;
	}
	else if (grounded) {
		//current_direction_x = NONE_X;

		//friction

		App->physics->ApplyFriction(&velocity, &acceleration);
	}
	else {
		//Friction in the air is not realistic, but makes the game feel better
	}


	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		//DoJump();
	}



	//ApplyMaxVelocity();

	// Direction


	//switch (last_direction_x)
	//{
	//case LEFT:
	//	current_animation = &idle_left;
	//	if (!grounded)
	//		current_animation = &jump_left;
	//	break;
	//case RIGHT:
	//	current_animation = &idle;
	//	if (!grounded)
	//		current_animation = &jump;
	//	break;
	//}

	//if (grounded) {
	//	switch (current_direction_x)
	//	{
	//	case LEFT:
	//		current_animation = &left;
	//		break;
	//	case RIGHT:
	//		current_animation = &right;
	//		break;
	//	default:
	//		break;
	//	}
	//	double_jump_avaliable = true;
	//}
	return true;
}
