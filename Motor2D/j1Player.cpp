//
//
//#include "j1App.h"
//#include "j1Textures.h"
//#include "j1Input.h"
//#include "j1Render.h"
//#include "j1Player.h"
//#include "j1Scene.h"
//
//#include <math.h>
//#include "j1Audio.h"
//#include "SDL/include/SDL_timer.h"
//#include "j1FadeToBlack.h"
//#include "Animation.h"
//
//j1Player::j1Player()
//{
//	graphics = NULL;
//	current_animation = NULL;
//	final_anim = false;
//
//
//	//idle animation
//	idle.PushBack({90,35,208,353});
//	idle.loop = true;
//	idle.speed = 1.0f;
//
//}
//
//j1Player::~j1Player()
//{}
//
//// Load assets
//bool j1Player::Start()
//{
//	level_dimensions = ((j1Scene*)App->scene->active)->getLevelDimensions();
//	position.x = (SCREEN_WIDTH / 2) + 20;
//	position.y = (float)((j1Scene*)App->scene->active)->getLevelDimensions().y + 110 - level_stage;
//	level_stage = 0;
//	
//	direction = 0;
//	
//	player_min_y = (int)position.y;
//	
//	state = IDLE;
//	
//	final_anim = 0;
//	
//	LOG("Loading player textures");
//	graphics = App->tex->Load("Game/textures/sprites.png");
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
//// Update: draw background
//update_status j1Player::Update()
//{
//
//	// Draw everything --------------------------------------
//	AnimationFrame frame = current_animation->GetCurrentFrame();
//	SDL_Rect* camera = &App->render->camera;
//
//	if ((int)position.x - frame.pivot.x < 20)
//		position.x = (float)frame.pivot.x + 20.0f;
//	else if ((int)position.x - frame.pivot.x + frame.rect.w > SCREEN_WIDTH + 20)
//		position.x = SCREEN_WIDTH + 20.0f - (float)frame.rect.w + (float)frame.pivot.x;
//	if ((int)position.y - frame.pivot.y < 0)
//		position.y = (float)frame.pivot.y;
//	else if ((int)position.y - frame.pivot.y + frame.rect.h > SCREEN_HEIGHT + (-camera->y / SCREEN_SIZE))
//		position.y = SCREEN_HEIGHT + (-camera->y / SCREEN_SIZE) - (float)frame.rect.h + (float)frame.pivot.y;
//
//	collider_body->rect = { (int)position.x - frame.pivot.x, (int)position.y - frame.pivot.y, frame.rect.w, frame.rect.h - 6 };
//	collider_feet->rect = { (int)position.x - frame.pivot.x, (int)position.y - frame.pivot.y + (frame.rect.h - 6), frame.rect.w, frame.rect.h - (frame.rect.h - 6) };
//
//	int margin = 110; //Must be equal to the player's initial position
//	if (camera->y < 0 && !((j1Scene*)App->current_scene)->intro) {
//		player_min_y = MIN(player_min_y, (int)position.y);
//		camera->y = (-player_min_y + margin) * SCREEN_SIZE;
//		int level_y = (((j1Scene*)App->current_scene)->getLevelDimensions().y) * SCREEN_SIZE;
//		if (camera->y < -level_y)
//			camera->y = -level_y;
//	}
//
//
//	return UPDATE_CONTINUE;
//}
//
//void j1Player::checkInput() {
//
//		if (App->input->keyboard[SDL_SCANCODE_RIGHT] == KEY_STATE::KEY_REPEAT)
//		{
//			state = MOVING_RIGHT | state;
//	
//		}
//
//		if (App->input->keyboard[SDL_SCANCODE_LEFT] == KEY_STATE::KEY_REPEAT)
//		{
//			state = MOVING_LEFT | state;
//		
//		}
//
//}
//
//void j1Player::processInput() {
//
//	switch (state) {
//	
//	
//	case MOVING_RIGHT:
//	
//		direction = 90;
//		current_animation = &right;
//		break;
//	case MOVING_LEFT:
//
//		direction = 270;
//		current_animation = &left;
//		break;
//}
//
//void j1Player::OnCollision(Collider* self, Collider* other) {
//
//}
//
//
//PLAYER_STATE operator |(PLAYER_STATE p, PLAYER_STATE s) {
//	PLAYER_STATE ret = static_cast<PLAYER_STATE>((int)p | (int)s);
//	return ret;
//}
//
//iPoint toiPoint(fPoint a) {
//	return{ (int)a.x, (int)a.y };
//}
//
//fPoint tofPoint(iPoint a) {
//	return{ (float)a.x, (float)a.y };
//}