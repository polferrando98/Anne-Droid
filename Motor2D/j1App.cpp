#include <iostream> 

#include "p2Defs.h"
#include "p2Log.h"

#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1EntityManager.h"
#include "j1Player.h"
#include "j1Physics.h"
#include "j1FadeToBlack.h"
#include "j1Pathfinding.h"

#include "Brofiler\Brofiler.h"

// Constructor
j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_START(ptimer);
	save_requested = load_requested = false;

	input = new j1Input();
	win = new j1Window();
	render = new j1Render();
	tex = new j1Textures();
	audio = new j1Audio();
	scene = new j1Scene();
	fade = new j1FadeToBlack();
	map = new j1Map();
	player = new j1Player();
	physics = new j1Physics();
	entity_manager = new j1EntityManager();
	pathfinding = new j1PathFinding();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(map);
	
	AddModule(scene);
	AddModule(fade);
	AddModule(entity_manager);

	AddModule(player);
	AddModule(physics);
	AddModule(pathfinding);


	// render last to swap buffer
	AddModule(render);
	
	PERF_PEEK(ptimer);
}

// Destructor
j1App::~j1App()
{
	// release modules
	p2List_item<j1Module*>* item = modules.end;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.clear();
}

void j1App::AddModule(j1Module* module)
{
	module->Init();
	modules.add(module);
}

// Called before render is available
bool j1App::Awake()
{
	PERF_START(ptimer);

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	bool ret = false;

	config = LoadConfig(config_file);


	if (config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title.create(app_config.child("title").child_value());
		organization.create(app_config.child("organization").child_value());

		
		fps_cap = app_config.attribute("framerate_cap").as_uint(30);
		max_fps = app_config.attribute("max_framerate").as_uint(60);
	}

	if (ret)
	{
		ret = LoadSaveFile();
	}



	if (ret == true)
	{
		p2List_item<j1Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	PERF_PEEK(ptimer);
	return ret;
}

// Called before the first frame
bool j1App::Start()
{
	PERF_START(ptimer);
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		if (item->data->IsEnabled())
			ret = item->data->Start();
		item = item->next;
	}
	startup_time.Start();

	PERF_PEEK(ptimer);

	return ret;
}

// Called each loop iteration
bool j1App::Update()
{
	BROFILER_CATEGORY("Update", Profiler::Color::BlueViolet)
	bool ret = true;
	PrepareUpdate();

	if (input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if (ret == true)
		ret = PreUpdate();

	if (ret == true)
		ret = DoUpdate();

	if (ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
pugi::xml_node j1App::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file("config.xml");

	if (result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("config");

	return ret;
}

// ---------------------------------------------
void j1App::PrepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;
	int frame_time_read = frame_time.Read();
	dt = frame_time_read;
	if (dt > 1000 / App->fps_cap)
	{
		dt = 1000 / App->fps_cap;
	}
	frame_time.Start();
}

// ---------------------------------------------
void j1App::FinishUpdate()
{
	if (save_requested) {
		real_save();
	}

	if (load_requested) {
		real_load();
	}

	load_requested = false;
	save_requested = false;

	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	float avg_fps = float(frame_count) / startup_time.ReadSec();
	float seconds_since_startup = startup_time.ReadSec();	
	uint32 last_frame_ms = frame_time.Read();
	uint32 frames_on_last_update = prev_last_sec_frame_count;

	static char title[256];
	sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %02u Last sec frames: %i  Time since startup: %.3f Frame Count: %lu ",
		avg_fps, last_frame_ms, frames_on_last_update, seconds_since_startup, frame_count);
	App->win->SetTitle(title);

	int fps_diferential_ms = 1000 / fps_cap - last_frame_ms;

	if (fps_diferential_ms > 0)
	{
		SDL_Delay(fps_diferential_ms);
	}
	

}

// Call modules before each loop iteration
bool j1App::PreUpdate()
{
	BROFILER_CATEGORY("PreUpdate", Profiler::Color::HotPink)
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		if (!item->data->IsEnabled())
			continue;

		ret = item->data->PreUpdate(dt);
	}

	return ret;
}

// Call modules on each loop iteration
bool j1App::DoUpdate()
{
	BROFILER_CATEGORY("DoUpdate", Profiler::Color::DodgerBlue)
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		if (!item->data->IsEnabled())
			continue;

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool j1App::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdate", Profiler::Color::Cyan)
	bool ret = true;
	p2List_item<j1Module*>* item;
	j1Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		if (!item->data->IsEnabled())
			continue;

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool j1App::CleanUp()
{
	PERF_START(ptimer);
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.end;

	while (item != NULL && ret == true)
	{
		if (!item->data->IsEnabled())
			ret = item->data->CleanUp();
		item = item->prev;
	}

	PERF_PEEK(ptimer);
	return ret;
}

// ---------------------------------------
int j1App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* j1App::GetArgv(int index) const
{
	if (index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* j1App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* j1App::GetOrganization() const
{
	return organization.GetString();
}


void j1App::load()
{
	load_requested = true;
}

bool j1App::LoadSaveFile()
{
	bool ret = true;

	pugi::xml_parse_result result = save_game_file.load_file("save_file.xml");

	if (result == NULL)  //if there is no file let's create it
	{
		pugi::xml_node save_node = save_game_file.append_child("save");
		save_node.append_child("app");

	}
	save_node = save_game_file.child("save");

	return ret;
}

// ---------------------------------------
void j1App::save() 
{
	save_requested = true;
}

// ---------------------------------------
bool j1App::real_save()
{
	p2List_item<j1Module*>* item;
	item = modules.start;

	while (item != NULL)
	{
		if (save_node.child(item->data->name.GetString())) {
			item->data->save(save_node.child(item->data->name.GetString()));
		}
		else {
			save_node.append_child(item->data->name.GetString());
			item->data->save(save_node.child(item->data->name.GetString()));
		}
		item = item->next;
	}


	save_game_file.save_file("save_file.xml");
	return false;
}

bool j1App::real_load()
{
	p2List_item<j1Module*>* item;
	item = modules.start;

	while (item != NULL)
	{
		item->data->load(save_node.child(item->data->name.GetString()));
		item = item->next;
	}
	return false;
}