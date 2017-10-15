//
//#include "j1Scene_W.h"
//#include "j1App.h"
//#include "j1Textures.h"
//#include "j1Render.h"
//#include "j1Player.h"
//#include "j1Input.h"
//#include "j1FadeToBlack.h"
//#include "j1Scene.h"
//#include "j1Scene_2.h"
//#include "j1Audio.h"
//
//
//j1Scene_W::j1Scene_W()
//{
//
//}
//
//
//j1Scene_W::~j1Scene_W()
//{
//}
//
//
//bool j1Scene_W::Init() {
//
//
//	screen_welcome = nullptr;
//	return true;
//}
//
//bool j1Scene_W::Start() {
//	bool ret = true;
//
//	/*screen_welcome = App->tex->Load("textures/welcome_scene.png");*/
//	
//	if (screen_welcome == nullptr)
//		ret = false;
//	
//	return ret;
//}
//
//
//
//bool j1Scene_W::Update(float dt){
//	bool ret = false;
//	
//
//	/*App->render->Blit(screen_welcome, 0, 0, nullptr, 0);*/
//	
//	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
//	{
//		App->fade->FadeToBlack(this, App->scene, 0.5f);
//	}
//
//	return ret ? update_status::UPDATE_CONTINUE : update_status::UPDATE_ERROR;
//
//}
//
//bool j1Scene_W::PostUpdate() {
//	return UPDATE_CONTINUE;
//}
//
//bool j1Scene_W::CleanUp() {
//	bool ret = true;
//	
//	/*if (!App->tex->UnLoad(screen_welcome))
//		ret = false;*/
//
//	return ret;
//}