#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "j1Pathfinding.h"
#include "j1EntityManager.h"
#include "j1Gui.h"
#include "Button.h"
#include "Window.h"
#include "Walker.h"

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
	debug_tex = App->tex->Load("maps/tile.png");
	App->map->data.is_level_1 = true;
	App->map->Load("1.tmx");

	App->map->PlaceTileColliders();
	App->map->ReadPositions();

	for (p2List_item<fPoint> *ovni_iterator = App->map->data.ovni_position_list.start; ovni_iterator; ovni_iterator = ovni_iterator->next)
	{
		ovnis.add(App->entity_manager->CreateEntity(ovni_iterator->data, ENTITY_BIRD));
	}
	

	player_entity = App->entity_manager->CreateEntity(App->map->data.player_start_position, ENTITY_PLAYER);

	///// ///// ///// ///// /////         UI		  ///// ///// ///// ///// ///// ///// 

	Window* test_win = App->gui->AddUIWindow({ 0,0 }, { 30,538,420,477 });
	test_win->MoveInPercentage({ 0,0 });

	Button* terms_of_use_but = App->gui->AddUIButton({ -10,200 }, "", "gui/button-atlas.png", { 0,0,127,23 }, { 0,50,127,23 }, { 169,0,127,23 }); //LoginButton
	

	terms_of_use_but->listener = App->scene;
	terms_of_use_but->MoveInPercentage({ 60,50 });

	terms_of_use_but->SetParent((UIElement*)test_win);


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
	ManageInput();

	if (camera_change == true)
		CameraFollowPlayer();

	App->map->Draw();

	App->physics->CheckDoorEntry(player_entity->position, player_entity->velocity, player_entity->collider);



	if (App->debug_mode) {
		DrawDebugPathfinding();
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	CheckDoorEntrance();

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool j1Scene::ChangeMap()
{
	current_level++;

	SetCurrentLevel();

	CleanLevel();

	LoadCurrentLevel();

	App->map->ReadPositions();

	player_entity->collider = App->physics->AddCollider(&player_entity->defaultRect, PLAYER);

	App->map->PlaceTileColliders();

	player_entity = App->entity_manager->CreateEntity(App->map->data.player_start_position, ENTITY_PLAYER);

	for (p2List_item<fPoint> *ovni_iterator = App->map->data.ovni_position_list.start; ovni_iterator; ovni_iterator = ovni_iterator->next)
	{
		ovnis.add(App->entity_manager->CreateEntity(ovni_iterator->data, ENTITY_BIRD));
	}

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


void j1Scene::CheckDoorEntrance()
{
	LOG("%f, %f", App->entity_manager->player_entity->position.x, App->entity_manager->player_entity->position.y);
	if (App->entity_manager->player_entity->position.x >= App->map->data.door_position.x && App->entity_manager->player_entity->position.y <= App->map->data.door_position.y)
	{
		if (current_level == 1)
		ChangeMap();

	}
}

void j1Scene::ManageInput()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		if (App->map->data.is_level_1 == false)
		{
			App->map->data.is_level_1 == true;
			App->map->CleanUp();
			App->map->Load("1.tmx");
		}
		player_entity->position = App->map->data.player_start_position;
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		player_entity->position = App->map->data.player_start_position;
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->load();

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->save();

	if ((App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN))
		App->debug_mode = !App->debug_mode;

	if ((App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN))
		App->god_mode = !App->god_mode;

	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
		App->fps_capped = !App->fps_capped;


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

void j1Scene::SetCurrentLevel()
{
	switch (current_level_enum)
	{
	case START_MENU:
		break;
	case LEVEL_1:
		current_level_enum = LEVEL_2;
		break;
	case LEVEL_2:
		break;
	case END:
		break;
	default:
		break;
	}
}

void j1Scene::LoadCurrentLevel()
{
	switch (current_level_enum)  //WIP
	{
	case START_MENU:
		break;
	case LEVEL_1:
		App->map->Load("1.tmx");
		break;
	case LEVEL_2:
		App->map->Load("2.tmx");
		break;
	case END:
		App->map->Load("3.tmx");
		break;
	default:
		break;
	}
}

void j1Scene::CleanLevel()
{
	App->map->CleanUp();
	App->physics->CleanUp();

	ovnis.clear();

	App->entity_manager->DeleteOvnis();

}

bool j1Scene::load(pugi::xml_node &save)
{
	
	player_entity->position.x = save.child("player").attribute("x").as_int();
	player_entity->position.y = save.child("player").attribute("y").as_int();

	return true;
}

bool j1Scene::save(pugi::xml_node &save) const
{
	if (save.child("player") == NULL) {
		save.append_child("player");
	}

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

	return true;
}
