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

	App->map->data.is_level_1 = true;

	App->map->Load("1.tmx");

	App->map->PlaceTileColliders();
	//App->map->PlaceColliders();

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
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
			App->map->Load("level_1.xml");
		}
		App->player->position.x = App->map->data.player_start_position.x;
		App->player->position.y = App->map->data.player_start_position.y;
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		App->player->position.x = App->map->data.player_start_position.x;
		App->player->position.y = App->map->data.player_start_position.y;
	}

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->load();

	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->save();



	uint w, h;
	App->win->GetWindowSize(w, h);

	App->render->camera.x = -App->player->position.x + w / 3;
	App->render->camera.y = -App->player->position.y + h / 2;

	App->map->Draw();



	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count());

	/*App->win->SetTitle(title.GetString());*/
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
	App->map->Load("level_2.tmx");
	App->map->PlaceColliders();
	App->player->position.x = App->map->data.player_start_position.x;
	App->player->position.y = App->map->data.player_start_position.y;
}

