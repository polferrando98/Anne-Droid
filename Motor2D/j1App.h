#ifndef __j1APP_H__
#define __j1APP_H__

#include "p2List.h"
#include "p2PQueue.h"
#include "j1Module.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1PerfTimer.h"
#include "j1Timer.h"

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1Scene;
class j1FadeToBlack;
class j1Map;
class j1EntityManager;
class j1Player;
class j1Physics;
class j1PathFinding;


class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void save();
	void load();

private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadSaveFile();

	bool real_save();
	bool real_load();

	

public:

	// Modules
	j1Window*			win;
	j1Input*			input;
	j1Render*			render;
	j1Textures*			tex;
	j1Audio*			audio;
	j1Scene*			scene;
	j1FadeToBlack*		fade;
	j1Map*				map;
	j1Player*			player;
	j1Physics*			physics;
	j1EntityManager*	entity_manager;
	j1PathFinding*		pathfinding;

	float				dt;
	int					max_fps;
	int					fps_cap;
	bool				fps_capped = true;

	bool				debug_mode = false;
	bool				god_mode = false;
private:

	p2List<j1Module*>	modules;
	int					argc;
	char**				args;

	p2SString			title;
	p2SString			organization;

	bool save_requested;
	bool load_requested;
	pugi::xml_document	config_file;
	pugi::xml_document	save_game_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;
	pugi::xml_node		save_node;

	p2SString			load_game;
	mutable p2SString	save_game;
	
	j1PerfTimer			delay_time;

	j1PerfTimer			ptimer;
	uint64				frame_count = 0;
	j1Timer				startup_time;
	j1Timer				frame_time;
	j1Timer				last_sec_frame_time;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;
};

extern j1App* App; // No student is asking me about that ... odd :-S

#endif