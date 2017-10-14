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

	App->map->Load("level_1.tmx");
	App->audio->PlayMusic("audio/music/anne-droid-music.ogg");

	current_map = App->map->Load("level_1.tmx");


	///////////////////////HARDCODE

	//col_rect = { 0, 3280, 10000, 10000 };
	//col_rect2 = { 1000, 3080, 10000, 10000 };

	//col1 = App->physics->AddCollider(&col_rect, COLLIDER_TYPE::WALL);
	//col1 = App->physics->AddCollider(&col_rect2, COLLIDER_TYPE::WALL);

	//PlaceColliders();
	App->map->PlaceColliders();
	
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
	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame();

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame();

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += App->render->camera_speed;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= App->render->camera_speed;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += App->render->camera_speed;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= App->render->camera_speed;

	uint w, h; 
	App->win->GetWindowSize(w, h); 

	App->render->camera.x = -App->player->position.x + w/3;
	App->render->camera.y = -App->player->position.y + h/2;

	App->map->Draw();


	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count());

	App->win->SetTitle(title.GetString());
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void j1Scene::PlaceColliders()
{
	ObjectGroup object_group = current_map->objectGroup;
	p2List_item<Object*>* collider_object;
	p2List_item<SDL_Rect>* rect_item;
	SDL_Rect Rect;
	
	for (collider_object = object_group.objects.start; collider_object; collider_object = collider_object->next) {
		Rect.x = collider_object->data->x;
		Rect.y = collider_object->data->y;
		Rect.w = collider_object->data->x;
		Rect.h = collider_object->data->x;
		
		col_rects->add(Rect);

		App->physics->AddCollider(&col_rects->end->data, WALL);
	}

}
