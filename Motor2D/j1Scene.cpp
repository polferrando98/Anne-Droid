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
	//App->map->PlaceColliders();

	//fPoint debugPosition = { 600,3000 };

	fPoint debugBirdPosition = { 700, 1000 };
	fPoint debugPlayerPosition = { 400,1000 };


	//App->entity_manager->CreateEntity(debugPosition,ENTITY_WALKER);
	player_entity = App->entity_manager->CreateEntity(debugPlayerPosition, ENTITY_PLAYER);

	bird = App->entity_manager->CreateEntity(debugBirdPosition, ENTITY_BIRD);

	p_clicked.SetToZero();

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate(float dt)
{
	// debug pathfing ------------------
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	//p = App->map->MapToWorld(p.x, p.y);



	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		if (App->map->data.is_level_1 == false)
		{
			App->map->data.is_level_1 == true;
			App->map->CleanUp();
			App->map->Load("1.xml");
		}
		App->player->position.x = App->map->data.player_start_position.x;
		App->player->position.y = App->map->data.player_start_position.y;
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		App->player->position.x = App->map->data.player_start_position.x;
		App->player->position.y = App->map->data.player_start_position.y;
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->load();

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->save();

	if (camera_change == true)
		CameraFollowPlayer();

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		DebugCamera(RIGHT, NONE_Y);

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		DebugCamera(LEFT, NONE_Y);

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		DebugCamera(NONE_X, UP);

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		DebugCamera(NONE_X, DOWN);

	App->map->Draw();


	/*CheckDoorEntrance();*/
	// Debug pathfinding ------------------------------
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		App->render->Blit(debug_tex, pos.x, pos.y);
	}

	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count());


	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void j1Scene::ChangeMap()
{
	App->map->CleanUp();
	App->physics->CleanUp();
	App->map->Load("2.tmx");
	App->map->PlaceColliders();
	App->entity_manager->player_entity->position.x = App->map->data.player_start_position.x;
	App->entity_manager->player_entity->position.y = App->map->data.player_start_position.y;
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
	if (App->physics->debug_mode) {
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

//void j1Scene::CheckDoorEntrance()
//{
//	if (App->entity_manager->player_entity->position.x >= 3000 && App->entity_manager->player_entity->position.y <= 100)
//	{
//		ChangeMap();
//	}
//}