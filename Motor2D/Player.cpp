#include "Player.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Scene.h"


Player::Player(fPoint position) : Entity(position, ENTITY_PLAYER)
{
	texture = NULL;
	current_animation = NULL;

	idle_left.LoadSprites("idle_left");
	idle_right.LoadSprites("idle");
	jump.LoadSprites("jump");
	jump_left.LoadSprites("jump_left");
	right.LoadSprites("right");
	left.LoadSprites("left");
	//death.LoadSprites("death");
	current_animation = &idle_right;

	defaultRect = { 0,0,76,123 };

	defaultRect.x = position.x;
	defaultRect.y = position.y;

	collider = App->physics->AddCollider(&defaultRect, PLAYER);
	texture = App->tex->Load("textures/player_sprites.png");

	grounded = true;


	//HARDCODE
	acceleration.y = 1.1f;

	max_velocity.x = 10.0f;
	max_velocity.y = 10.0f;
}


bool Player::Update(float dt)
{
	App->physics->UpdateEntityPhysics(*this, dt);
	
	collider->UpdatePosition(position);

	Entity::Update(dt);
	
	UpdateCurrentTile();
	
	App->physics->ApplyMaxVelocity(*this);
	Move();
	ManageAnimation();
	if (y_axis_collision == DOWN) {
		grounded = true;
	}
	// Direction
	double_jump_avaliable = false;

	if (!App->god_mode)
	{
		if (life_state == DEAD) 
		{
			Respawn();
			App->scene->ResetOvnis();
		}
	}

	return true;
}

void Player::Move()
{
	
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		acceleration.x = -movement_acceleration.x;
		last_direction_x = LEFT;
		current_direction_x = LEFT;
		action = MOVING;
	}

	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		acceleration.x = movement_acceleration.x;
		last_direction_x = RIGHT;
		current_direction_x = RIGHT;
		action = MOVING;
	}
	else if (grounded) {
		current_direction_x = NONE_X;
		action = STATIC;
		App->physics->ApplyFriction(velocity, acceleration);
	}
	else {
		App->physics->ApplyFriction(velocity, acceleration);
	}

	
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		DoJump();
	}

	ApplyMaxVelocity();

}

void Player::ManageAnimation()
{
	if (action == STATIC) {
		switch (last_direction_x)
		{
		case LEFT:
			current_animation = &idle_left;
			if (!grounded)
				current_animation = &jump_left;
			break;
		case RIGHT:
			current_animation = &idle_right;
			if (!grounded)
				current_animation = &jump;
			break;
		}

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
		}
		else {
			
			switch (current_direction_x)
			{
			case LEFT:
				current_animation = &jump_left;
				
				break;
			case RIGHT:
				current_animation = &jump;
				
				break;
			default:
				break;
			}
		}
		
		
}

void Player::DoJump()
{
	App->audio->PlayFx(1);
	{
		if (!grounded && double_jump_avaliable) {
			jump.current_frame = 0.0f;
			jump_left.current_frame = 0.0f;
			velocity.y = -JUMP_SPEED;
			double_jump_avaliable = false;

		}
		if (grounded) {
			jump.current_frame = 0.0f;
			jump_left.current_frame = 0.0f;
			velocity.y = -JUMP_SPEED;

			grounded = false;
		}
	}
}

void Player::ApplyMaxVelocity()
{
	
		if (velocity.x != 0) {
			if (velocity.x > max_velocity.x)
				velocity.x = max_velocity.x;
			else if (velocity.x < -max_velocity.x)
				velocity.x = -max_velocity.x;
		}

}

void Player::Respawn()
{
	acceleration.SetToZero();
	velocity.SetToZero();
	position.x = App->map->data.player_start_position.x;
	position.y = App->map->data.player_start_position.y;
	life_state = ALIVE;
}



