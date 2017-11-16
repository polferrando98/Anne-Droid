#include "Player.h"
#include "j1App.h"
#include "j1Textures.h"

#include "j1Input.h"



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
	death.LoadSprites("death");

}

bool Player::Start()
{
	
	current_animation = &idle_right;
	Entity::Start();
	SDL_Rect colrect = { 0,0,76,123 };

	colrect.x = position.x;
	colrect.y = position.y;

	collider = App->physics->AddCollider(&colrect, PLAYER);
	texture = App->tex->Load("textures/player_sprites.png");

	return true;
}

bool Player::Update(float dt)
{
	App->physics->UpdateEntityPhysics(*this, dt);
	collider->UpdatePosition(position);
	Entity::Update(dt);

	Move();


	if (y_axis_collision == DOWN) {
		grounded = true;
	}

	
	// Direction


	/*switch (last_direction_x)
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
	*/
	return true;
}

void Player::Move()
{
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		current_animation = &right;
		position.x += velocity.x;
		velocity.x += 200.0f;
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{ 
		current_animation = &left;
		position.x += velocity.x;
		velocity.x -= 200.0f;
	}
}

 