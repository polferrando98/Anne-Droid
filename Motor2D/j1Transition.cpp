#include "j1Transition.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Input.h"
#include "j1FadeToBlack.h"
#include "j1Scene.h"
#include "j1Audio.h"

#include "j1Scene_2.h"

#include "SDL/include/SDL_timer.h"

j1Transition::j1Transition()
{
}


j1Transition::~j1Transition()
{
}

bool j1Transition::Init() {
	timer = SDL_GetTicks();
	return true;
}

bool j1Transition::Start() {
	
	bool ret = true;
	
	//load transition scene

	//if (background_graphics == nullptr)
	//	ret = false;

	return ret;
}

bool j1Transition::PreUpdate() {
	return UPDATE_CONTINUE;
}
bool j1Transition::Update() {
	bool ret = false;
	//draw transition scene
	/*ret = App->render->Blit(background_graphics, 0, 0, nullptr, 0.2f);
	App->render->Blit(sprite_graphics, (SCREEN_WIDTH / 2) - 16, (SCREEN_HEIGHT / 2) - 26, &trans.GetCurrentFrame().rect);*/

	//fade from one this to level2
	/*App->fade->FadeToBlack(this, App->scene_2, 5);*/
	

	return ret ? update_status::UPDATE_CONTINUE : update_status::UPDATE_ERROR;

}

bool j1Transition::PostUpdate() {

	return UPDATE_CONTINUE;
}

bool j1Transition::CleanUp() {
	bool ret = true;
	
	//unload graphics
	/*ret = App->textures->Unload(background_graphics);*/
	



	return ret;
}