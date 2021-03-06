﻿#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Pathfinding.h"
#include "j1EntityManager.h"
#include "j1Gui.h"
#include "Button.h"
#include "Picture.h"
#include "Label.h"
#include "Window.h"
#include "Walker.h"
#include "j1Physics.h"
#include "Brofiler\Brofiler.h"
#include "j1Timer.h"
#include "j1FadeToBlack.h"


j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	SetUpLevel(START_MENU);

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate(float dt)
{

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("Scene Update", Profiler::Color::DodgerBlue);

	ManageInput();

	if (timer_label)
	{
		sprintf_s(timerbuffer, "%.2f", timer.ReadSec());
		p2SString timer_string = timerbuffer;
		timer_string += "s";
		timer_label->SetText(timer_string);
	}

	if (gears_collected >= 5) {
		player_lives++;
		gears_collected = 0;
		gears_number->SetText(gears_collected);
		SetUpLivesIconsAndGears();
	}

	if (camera_change == true)
		CameraFollowPlayer();

	App->map->Draw();

	if (App->debug_mode) {
		DrawDebugPathfinding();
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	BROFILER_CATEGORY("Scene Post Update", Profiler::Color::Cyan)

		if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
			post_update_ret = false;


	if (level_to_load_on_postUpdate != LEVEL_NONE)
	{
		ChangeMap(level_to_load_on_postUpdate);
		level_to_load_on_postUpdate = LEVEL_NONE;
	}

	return post_update_ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	CleanLevel();
	LOG("Freeing scene");

	return true;
}

bool j1Scene::ChangeMap(Levels next_level)
{

	CleanLevel();

	SetUpLevel(next_level);

	return true;
}

void j1Scene::ResetOvnis()
{
	for (p2List_item<Entity*> *ovni_iterator = ovnis.start; ovni_iterator; ovni_iterator = ovni_iterator->next)
	{
		ovni_iterator->data->Respawn();
	}
}

void j1Scene::CameraFollowPlayer()
{
	uint w, h;
	App->win->GetWindowSize(w, h);

	if (player_entity != nullptr) {
		App->render->camera.x = -player_entity->position.x + w / 3;
		App->render->camera.y = -player_entity->position.y + h / 2;
	}
}

void j1Scene::DebugCamera(Direction_x type, Direction_y type2)
{
	camera_change = false;
	if (App->debug_mode) {
		float cam_speed = 10;

		if (type == RIGHT && type2 == NONE_Y)
		{
			App->render->camera.x -= cam_speed;
		}
		if (type == LEFT && type2 == NONE_Y)
		{
			App->render->camera.x += cam_speed;
		}
		if (type == NONE_X && type2 == UP)
		{
			App->render->camera.y += cam_speed;
		}
		if (type == NONE_X && type2 == DOWN)
		{
			App->render->camera.y -= cam_speed;
		}

	}
}


void j1Scene::ManageInput()
{
	BROFILER_CATEGORY("Scene Manage Input", Profiler::Color::DodgerBlue);


	//Shortcuts to navigation

	if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
		ChangeMap(START_MENU);

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		ChangeMap(LEVEL_1);

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		ChangeMap(LEVEL_2);

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		ChangeMap(LEVEL_3);

	if (App->input->GetKey(SDL_SCANCODE_7) == KEY_DOWN)
		ChangeMap(SETTINGS);


	//Debug funcitonality

	/// ​Start from the very first level
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		ChangeMap(LEVEL_1);

	/// ​Start from the beginning of the current level
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		player_entity->position = App->map->data.player_start_position;

	/// Save the current state
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->save();

	/// Load the current state
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->load();

	/// ​Start from the very first level
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		App->gui->debug_draw = !App->gui->debug_draw;

	/// ​Start from the very first level
	if ((App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN))
		App->debug_mode = !App->debug_mode;

	/// ​Start from the very first level
	if ((App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN))
		App->god_mode = !App->god_mode;

	/// ​Start from the very first level
	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
		App->fps_capped = !App->fps_capped;



	//Debug camera
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		DebugCamera(RIGHT, NONE_Y);

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		DebugCamera(LEFT, NONE_Y);

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		DebugCamera(NONE_X, UP);

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		DebugCamera(NONE_X, DOWN);
}

void j1Scene::DrawDebugPathfinding()
{
	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		App->render->Blit(debug_tex, pos.x, pos.y);
	}
}

void j1Scene::LoadCurrentLevel(Levels next_level)
{
	App->render->camera.x = 0;
	App->render->camera.y = 0;

	switch (next_level)  //WIP
	{
	case START_MENU:
		App->map->Load("menu.tmx");
		break;
	case LEVEL_1:
		App->map->Load("1.tmx");
		break;
	case LEVEL_2:
		App->map->Load("2.tmx");
		break;
	case LEVEL_3:
		App->map->Load("3.tmx");
		break;
	case SETTINGS:
		App->map->Load("menu.tmx");
		break;
	case CREDITS:
		App->map->Load("menu.tmx");
		break;
	case END:
		App->map->Load("menu.tmx");
		break;
	default:
		break;
	}
}

void j1Scene::CleanLevel()
{
	gears_incon = nullptr;
	timer_icon = nullptr;
	score_icon = nullptr;
	gears_number = nullptr;

	timer_label = nullptr;

	score_number = nullptr;

	player_entity = nullptr;


	App->physics->CleanUp();

	App->map->CleanUp();

	ovnis.clear();

	App->entity_manager->CleanUp();

}

void j1Scene::SetUpLevel(Levels next_level)
{
	PERF_START(timer);

	LoadCurrentLevel(next_level);

	SetUpUI(next_level);

	App->map->PlaceTileColliders();

	if (App->map->ReadPositions()) {

		player_entity = App->entity_manager->CreateEntity(App->map->data.player_start_position, ENTITY_PLAYER); 

		for (p2List_item<fPoint> *ovni_iterator = App->map->data.ovni_position_list.start; ovni_iterator; ovni_iterator = ovni_iterator->next)
		{
			ovnis.add(App->entity_manager->CreateEntity(ovni_iterator->data, ENTITY_BIRD));
		}
		AddGears();
	}

	current_level = next_level;
}

void j1Scene::SetUpUI(Levels next_level)
{
	App->gui->CleanAllUI();
	DeleteGearList();

	if (next_level == LEVEL_1 || next_level == LEVEL_2 || next_level == LEVEL_3)
	{
		SetUpLivesIconsAndGears();
	}

	switch (next_level)  //WIP
	{
	case START_MENU:
	{
		App->fade->FadeToBlack(2.0f);
		App->gui->AddUIButton({ 50,30 }, "Play", this, button_up_rect_section, button_hover_rect_section, button_down_rect_section, button_disabled_rect_section);
		Button* continue_button = App->gui->AddUIButton({ 50,40 }, "Continue", this, button_up_rect_section, button_hover_rect_section, button_down_rect_section, button_disabled_rect_section);
		App->gui->AddUIButton({ 50,50 }, "Settings", this, button_up_rect_section, button_hover_rect_section, button_down_rect_section, button_disabled_rect_section);
		App->gui->AddUIButton({ 50,60 }, "Credits", this, button_up_rect_section, button_hover_rect_section, button_down_rect_section, button_disabled_rect_section);
		App->gui->AddUIButton({ 50,70 }, "Exit", this, button_up_rect_section, button_hover_rect_section, button_down_rect_section, button_disabled_rect_section);

		if (!App->save_file_exists)
			continue_button->Disable();
	}
	break;
	case LEVEL_1:
	{
		App->fade->FadeToBlack(3.0f);
		
		break;
	}
	case LEVEL_2:
	{
		App->fade->FadeToBlack(3.0f);
		
		break;
	}
	case SETTINGS:
		App->gui->AddUIText({ 50,20 }, "Settings");
		App->gui->AddUIButton({ 35,40 }, "Music -", this, button_up_rect_section, button_hover_rect_section, button_down_rect_section, button_disabled_rect_section);
		App->gui->AddUIButton({ 60,40 }, "Music +", this, button_up_rect_section, button_hover_rect_section, button_down_rect_section, button_disabled_rect_section);
		App->gui->AddUIButton({ 35,60 }, "Sound Fx -", this, button_up_rect_section, button_hover_rect_section, button_down_rect_section, button_disabled_rect_section);
		App->gui->AddUIButton({ 60,60 }, "Sound Fx +", this, button_up_rect_section, button_hover_rect_section, button_down_rect_section, button_disabled_rect_section);
		App->gui->AddUIButton({ 47,77 }, "Menu", this, button_up_rect_section, button_hover_rect_section, button_down_rect_section, button_disabled_rect_section);
		
		break;
	case CREDITS:
		App->gui->AddUIText({ 50,35 }, "MIT License Copyright(c)[2017]");
		App->gui->AddUIText({ 50,50 }, "By Nina López and Pol Ferrando");
		App->gui->AddUIButton({ 47,77 }, "Menu", this, button_up_rect_section, button_hover_rect_section, button_down_rect_section, button_disabled_rect_section);
		break;
	case END:
		App->gui->AddUIText({ 50,30 }, "Congratulations, you completed the game!");
		App->gui->AddUIText({ 50,40 }, "Thanks for playing, hope you enjoyed it :)");
		App->gui->AddUIButton({ 50,60 }, "Menu", this, button_up_rect_section, button_hover_rect_section, button_down_rect_section, button_disabled_rect_section);
		break;
	default:
		break;
	}
}

void j1Scene::AddGears()
{
	if (!gears.start) {
		for (p2List_item<iPoint> *gear_iterator = App->map->data.gear_position_list.start; gear_iterator; gear_iterator = gear_iterator->next)
		{
			Picture* new_gear;
			new_gear = App->gui->AddUIPicture(gear_iterator->data, { 350,0,100,100 });
			new_gear->move_with_camera = false;
			gears.add(new_gear);
		}
	}


	App->physics->DeleteGearColliders();
	AddGearColliders();
	LOG("HMM");
}

void j1Scene::DeleteGearPictureFromCollider(Collider * col)
{
	p2List_item<Picture*>* gear_to_delete = nullptr;

	for (p2List_item<Picture*>* gears_iterator = gears.start; gears_iterator; gears_iterator = gears_iterator->next) {
		iPoint gears_iterator_pos = { gears_iterator->data->position.x,gears_iterator->data->position.y };
		iPoint col_pos = { col->rect.x,col->rect.y };
		if (col_pos == gears_iterator_pos) {
			App->gui->DeleteElement((UIElement*)gears_iterator->data);
			gear_to_delete = gears_iterator;
			App->audio->PlayFx(App->audio->gearfx);
			gears_collected++;
			score += 100;
			char buffer[50];
			char buffer_2[50];
			sprintf_s(buffer, "%d", gears_collected);
			sprintf_s(buffer_2, "%d", score);
			gears_number->SetText(buffer);
			score_number->SetText(buffer_2);
		}
	}

	if (gear_to_delete)
	gears.del(gear_to_delete);
}

void j1Scene::GoToNextLevelOnPostUpdate()
{
	switch (current_level)
	{
	case LEVEL_NONE:
		break;
	case START_MENU:
		break;
	case LEVEL_1:
		level_to_load_on_postUpdate = LEVEL_2;
		break;
	case LEVEL_2:
		level_to_load_on_postUpdate = LEVEL_3;
		break;
	case LEVEL_3:
		level_to_load_on_postUpdate = END;
		break;
	case SETTINGS:
		break;
	case CREDITS:
		break;
	case END:
		break;
	default:
		break;
	}
}

void j1Scene::onFadeInEnd()
{
}


void j1Scene::AddGearColliders()
{
	for (p2List_item<Picture*>* gears_iterator = gears.start; gears_iterator; gears_iterator = gears_iterator->next) {
		SDL_Rect gear_rect{
			gears_iterator->data->position.x,
			gears_iterator->data->position.y,
			100,
			100
		};
		App->physics->AddCollider(&gear_rect, COL_GEAR);
	}
}

void j1Scene::DeleteGearList()
{
	//for (p2List_item<Picture*>* gears_iterator = gears.start; gears_iterator; gears_iterator = gears_iterator->next) {
	//	RELEASE(gears_iterator->data);
	//}
	gears.clear();
}

void j1Scene::ResetGame()
{
	App->scene->player_lives = 3;
	App->scene->gears_collected = 0;
	App->scene->score = 0;
	App->scene->level_to_load_on_postUpdate = START_MENU;
}

void j1Scene::SetUpLivesIconsAndGears()
{
	//Lives
	if (lives_icons.start) {
		for (p2List_item<Picture*>* lives_icons_iterator = lives_icons.start; lives_icons_iterator; lives_icons_iterator = lives_icons_iterator->next) {
			App->gui->DeleteElement((UIElement*)lives_icons_iterator->data);
		}
		lives_icons.clear();
	}

	int lives_icon_margin = 100;

	for (int i = 0; i < player_lives; i++) {
		lives_icons.add(App->gui->AddUIPicture({ 100 + lives_icon_margin * i, 50 }, { 250,0,100,100 }));
	}

	timer_icon = App->gui->AddUIPicture({ 1050, 55 }, timer_icon_section);
	score_icon = App->gui->AddUIPicture({ 1052, 140 }, score_icon_section);

	//Gears icon
	if (!gears_incon)
		gears_incon = App->gui->AddUIPicture({ 100, 150 }, gears_icon_section);
	else {
		if (App->gui->DeleteElement(gears_incon))
			RELEASE(gears_incon);
		gears_incon = App->gui->AddUIPicture({ 100, 150 }, gears_icon_section);
	}
	
		
	//Geras number
	char buffer[50];
	char buffer_2[9999];
	sprintf_s(buffer, "%d", gears_collected);
	sprintf_s(buffer_2, "%d", score);

	if (!gears_number) {
		gears_number = App->gui->AddUIText({ 150, 100 }, buffer);
		gears_number->position = { 200,175 }; //SPAGHUETTI
	}
	else {
		App->gui->DeleteElement(gears_number);
		gears_number = App->gui->AddUIText({ 150, 100 }, buffer);
		gears_number->position = { 200,175 }; //SPAGHUETTI
	}


	//Timer

	if (!timerbuffer) 
		timer_label = App->gui->AddUIText({ 150, 100 }, timerbuffer);
	else {
		App->gui->DeleteElement(timer_label);
		timer_label = App->gui->AddUIText({ 150, 100 }, timerbuffer);
	}
	timer_label->pivot = { 0.0f,0.0f };
	timer_label->MoveInPercentage({ 80,10 }); //SPAGHUETTI

	//Score
	if (!score_number) {
		score_number = App->gui->AddUIText({ 1350, 25 }, buffer_2);

	}
	else {
		App->gui->DeleteElement(score_number);
		score_number = App->gui->AddUIText({ 1350, 25 }, buffer_2);
	}
	score_number->pivot = { 0.0f,0.0f };
	score_number->MoveInPercentage({80,20}); //SPAGHUETTI

}

bool j1Scene::load(pugi::xml_node &save)
{
	if (save != NULL) {
		if (save.child("player").attribute("level").as_int() != current_level)
			ChangeMap((Levels)(save.child("player").attribute("level").as_int()));
		player_entity->position.x = save.child("player").attribute("x").as_int();
		player_entity->position.y = save.child("player").attribute("y").as_int();
	}

	return true;
}

bool j1Scene::save(pugi::xml_node &save) const
{
	if (player_entity) {
		if (save.child("player") == NULL) {
			save.append_child("player");
			save.child("player").append_attribute("x") = player_entity->position.x;
			save.child("player").append_attribute("y") = player_entity->position.y;
			save.child("player").append_attribute("level") = (int)current_level;
		}
		else {
			if (save.child("player").attribute("x") == NULL) {
				save.child("player").append_attribute("x") = player_entity->position.x;
			}
			else {
				save.child("player").attribute("x").set_value(player_entity->position.x);
			}

			if (save.child("player").attribute("y") == NULL) {
				save.child("player").append_attribute("y") = player_entity->position.y;
			}
			else {
				save.child("player").attribute("y").set_value(player_entity->position.y);
			}
			if (save.child("player").attribute("level") == NULL) {
				save.child("player").append_attribute("level") = (int)current_level;
			}
			else {
				save.child("player").attribute("level").set_value((int)current_level);
			}
		}
	}

	return true;
}

void j1Scene::OnButtonClick(UIElement * trigger, Mouse_UI_Event mouse_event)
{
	if (trigger->name == "Play")
		level_to_load_on_postUpdate = LEVEL_1;

	if (trigger->name == "Continue")
		App->load();

	if (trigger->name == "Settings")
		level_to_load_on_postUpdate = SETTINGS;

	if (trigger->name == "Credits")
		level_to_load_on_postUpdate = CREDITS;

	if (trigger->name == "Exit")
		post_update_ret = false;

	if (trigger->name == "Music -")
		App->audio->volumeDown();
	if (trigger->name == "Music +")
		App->audio->volumeUp();

	if (trigger->name == "Menu") {
		level_to_load_on_postUpdate = START_MENU;
		ResetGame();
	}
	/*if (trigger->name == "Sound Fx -")
		App->audio->volumeDown();
	if (trigger->name == "Sound Fx +")
		App->audio->volumeUp();*/

}

