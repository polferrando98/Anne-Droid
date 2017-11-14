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

	uchar* data = NULL;
	if (App->map->CreateWalkabilityMap(App->map->data.width, App->map->data.height, &data))
		App->pathfinding->SetMap(App->map->data.width, App->map->data.height, data);

	RELEASE_ARRAY(data);
	
	App->map->Load("1.tmx");

	App->map->PlaceTileColliders();
	//App->map->PlaceColliders();

	fPoint debugPosition = { 600,3000 };
	fPoint debugPlayerPosition = { 900,3000 };
	App->entity_manager->CreateEntity(debugPosition,ENTITY_WALKER);
	player_entity = App->entity_manager->CreateEntity(debugPlayerPosition, ENTITY_PLAYER);

	p_clicked.SetToZero();

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	// debug pathfing ------------------
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	//p = App->map->MapToWorld(p.x, p.y);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (origin_selected == true)
		{
			App->pathfinding->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}

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


	CameraFollowPlayer();


	App->map->Draw();

	/*int x, y;
	App->input->GetMousePosition(x, y);
	Uint8 alpha = 80;
	int width_tile = App->map->data.tile_width;*/
	//int height_tile = App->map->data.tile_height;

	//if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	//{
	//	p_clicked = App->render->ScreenToWorld(x, y);
	//	p_clicked = App->map->WorldToMap(p_clicked.x, p_clicked.y);
	//	p_clicked = App->map->MapToWorld(p_clicked.x, p_clicked.y);
	//}
	//SDL_Rect prova = { p_clicked.x,p_clicked.y,width_tile,height_tile };
	//App->render->DrawQuad(prova, 204,0, 153, alpha, true, true); //Fuchsia


	// Debug pathfinding ------------------------------
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	//p = App->map->MapToWorld(p.x, p.y);

	App->render->Blit(debug_tex, p.x, p.y);

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

void j1Scene::CameraFollowPlayer()
{
	uint w, h;
	App->win->GetWindowSize(w, h);

	App->render->camera.x = -player_entity->position.x + w / 3;
	App->render->camera.y = -player_entity->position.y + h / 2;
}

