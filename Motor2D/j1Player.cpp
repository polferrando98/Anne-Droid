//
//
//#include "j1App.h"
//#include "j1Textures.h"
//#include "j1Input.h"
//#include "j1Render.h"
//#include "j1Player.h"
//#include "j1Physics.h"
//#include "j1Scene.h"
//#include  "Animation.h"
//#include <math.h>
//#include "j1Audio.h"
//#include "SDL/include/SDL_timer.h"
//#include "j1FadeToBlack.h"
//#include "Animation.h"
//#include "p2Log.h"
//
//j1Player::j1Player()
//{
//	name.create("player");
//
//	graphics = NULL;
//	current_animation = NULL;
//
//	idle.LoadSprites("idle");
//	idle_left.LoadSprites("idle_left");
//	jump.LoadSprites("jump");
//	jump_left.LoadSprites("jump_left");
//	right.LoadSprites("right");
//	left.LoadSprites("left");
//	death.LoadSprites("death");
//}
//
//j1Player::~j1Player()
//{
//
//}
//
//// Load assets
//bool j1Player::Start()
//{
//	App->physics->LoadPhysicsValues();
//	current_animation = &idle;
//	LOG("Loading player textures");
//	graphics = App->tex->Load("textures/player_sprites.png");
//
//	SDL_Rect colrect = { 0,0,110,193 }; 
//	createCol(&colrect);
//
//	//HARDCODE
//
//	
//
//	return true;
//}
//
//bool j1Player::CleanUp() {
//	LOG("Unloading player");
//
//	App->tex->UnLoad(graphics);
//	return true;
//}
//
//
//bool j1Player::Update(float dt)
//{
//	//// Draw everything --------------------------------------
//	//AnimationFrame frame = current_animation->GetCurrentFrame();
//	//App->physics->UpdatePlayerPhysics(position, velocity, acceleration, player_coll, colliding_x, y_axis_collision);
//
//	//if (y_axis_collision == DOWN) {
//	//	grounded = true;
//	//}
//	//velocity.x = velocity.x *dt;
//	//velocity.y = velocity.y * dt;
//	//if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
//	//	acceleration.x = -acceleration_x;
//	//	last_direction_x = LEFT;
//	//	current_direction_x = LEFT;
//	//}
//
//	//else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
//	//	acceleration.x = acceleration_x;
//	//	last_direction_x = RIGHT;
//	//	current_direction_x = RIGHT;
//	//}
//	//else if (grounded) {
//	//	current_direction_x = NONE_X;
//
//	//	//friction
//
//	//	App->physics->ApplyFriction(&velocity, &acceleration);
//	//}
//	//else {
//	//	//Friction in the air is not realistic, but makes the game feel better
//	//}
//
//
//	//if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
//	//{
//	//	DoJump();
//	//}
//
//
//
//	//ApplyMaxVelocity();
//
//	//// Direction
//
//
//	//switch (last_direction_x)
//	//{
//	//case LEFT:
//	//	current_animation = &idle_left;
//	//	if (!grounded)
//	//		current_animation = &jump_left;
//	//	break;
//	//case RIGHT:
//	//	current_animation = &idle;
//	//	if (!grounded)
//	//		current_animation = &jump;
//	//	break;
//	//}
//
//	//if (grounded) {
//	//	switch (current_direction_x)
//	//	{
//	//	case LEFT:
//	//		current_animation = &left;
//	//		break;
//	//	case RIGHT:
//	//		current_animation = &right;
//	//		break;
//	//	default:
//	//		break;
//	//	}
//	//	double_jump_avaliable = true;
//	//}
//
//
//
//	//player_coll->UpdatePosition(position);
//	//App->render->Blit(graphics, position.x, position.y, &frame.rect);
//
//	//colliding_x = NONE_X;
//	//y_axis_collision = NONE_Y;
//
//
//	return UPDATE_CONTINUE;
//
//}
////
////void j1Player::ApplyMaxVelocity()
////{
////	if (velocity.x != 0) {
////		if (velocity.x > maxVelocity.x)
////			velocity.x = maxVelocity.x;
////		else if (velocity.x < -maxVelocity.x)
////			velocity.x = -maxVelocity.x;
////	}
////}
//
//void j1Player::createCol(SDL_Rect* Rect)
//{
//	player_coll = App->physics->AddCollider(Rect, Collider_Type::PLAYER);
//}
//
//bool j1Player::load(pugi::xml_node &save)
//{
//	position.x = save.child("player").attribute("x").as_int();
//	position.y = save.child("player").attribute("y").as_int();
//
//	return true;
//}
//
//bool j1Player::save(pugi::xml_node &save) const
//{
//	if (save.child("player") == NULL) {
//		save.append_child("player");
//	}
//
//	if (save.child("player").attribute("x") == NULL) {
//		save.child("player").append_attribute("x") = position.x;
//	}
//	else {
//		save.child("player").attribute("x").set_value(position.x);
//	}
//
//	if (save.child("player").attribute("y") == NULL) {
//		save.child("player").append_attribute("y") = position.y;
//	}
//	else {
//		save.child("player").attribute("y").set_value(position.y);
//	}
//
//	return true;
//}
//
//void j1Player::DoJump()
//{
//	App->audio->PlayFx(1);
//	if (!grounded && double_jump_avaliable) {
//
//		jump.current_frame = 0.0f;
//		jump_left.current_frame = 0.0f;
//		velocity.y = -jump_speed;
//		double_jump_avaliable = false;
//
//	}
//	if (grounded) {
//		jump.current_frame = 0.0f;
//		jump_left.current_frame = 0.0f;
//		velocity.y = -jump_speed;
//		grounded = false;
//	}
//}
