//
//#include <math.h>
//#include "Globals.h"
//#include "j1App.h"
//#include "j1FadeToBlack.h"
//#include "j1Render.h"
//#include "SDL/include/SDL_render.h"
//#include "SDL/include/SDL_timer.h"
//
//j1FadeToBlack::j1FadeToBlack()
//{
//	screen = { 0, 0, SCREEN_WIDTH * SCREEN_SIZE, SCREEN_HEIGHT * SCREEN_SIZE };
//}
//
//j1FadeToBlack::~j1FadeToBlack()
//{}
//
//// Load assets
//bool j1FadeToBlack::Start()
//{
//	LOG("Preparing Fade Screen");
//	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
//	return true;
//}
//
//// Update: draw background
//update_status j1FadeToBlack::PostUpdate()
//{
//	if (current_step == fade_step::none)
//		return UPDATE_CONTINUE;
//
//	Uint32 now = SDL_GetTicks() - start_time;
//	float normalized = MIN(1.0f, (float)now / (float)total_time);
//
//	switch (current_step)
//	{
//	case fade_step::fade_to_black:
//	{
//		if (now >= total_time)
//		{
//			// TODO 2: enable / disable the modules received when FadeToBlacks() gets called
//			App->render->ResetCamera();
//			module_off->Disable();
//			module_on->Enable();
//			// ---
//			total_time += total_time;
//			start_time = SDL_GetTicks();
//			current_step = fade_step::fade_from_black;
//		}
//	} break;
//
//	case fade_step::fade_from_black:
//	{
//		normalized = 1.0f - normalized;
//
//		if (now >= total_time) {
//			module_on->onFadeInEnd();
//			current_step = fade_step::none;
//		}
//	} break;
//	}
//
//	// Finally render the black square with alpha on the screen
//	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
//	SDL_RenderFillRect(App->render->renderer, &screen);
//
//	return UPDATE_CONTINUE;
//}
//
//// Fade to black. At mid point deactivate one module, then activate the other
//bool j1FadeToBlack::FadeToBlack(j1Module* module_off, j1Module* module_on, float time)
//{
//	bool ret = false;
//	if (current_step == fade_step::none)
//	{
//		this->module_off = module_off;
//		this->module_on = module_on;
//		current_step = fade_step::fade_to_black;
//		start_time = SDL_GetTicks();
//		total_time = (Uint32)(time * 0.5f * 1000.0f);
//		ret = true;
//	}
//
//	return ret;
//}
//
