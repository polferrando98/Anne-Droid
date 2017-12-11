#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1Physics.h"
#include "j1Map.h"
#include "p2PQueue.h"

struct SDL_Texture;

class Entity;
class Picture;

enum Levels { LEVEL_NONE, START_MENU, LEVEL_1, LEVEL_2, LEVEL_3, SETTINGS, CREDITS, END };

struct Level {
	int index;
};

class j1Scene : public j1Module
{
public:
	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate(float dt);

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool ChangeMap(Levels next_level); //

	void ResetOvnis();

	void DebugCamera(Direction_x type, Direction_y type2);

	bool load(pugi::xml_node &save);
	bool save(pugi::xml_node &save) const;

	void OnButtonClick(UIElement* trigger, Mouse_UI_Event mouse_event) override;

	void SetUpLivesIcons();

	void AddGears();

private:

	void CameraFollowPlayer();
	void CheckDoorEntrance();
	void ManageInput();
	void DrawDebugPathfinding();


	void SetCurrentLevel();
	void LoadCurrentLevel(Levels next_level);
	void CleanLevel();

	void SetUpLevel(Levels next_level);

	void SetUpUI(Levels next_level);


	void AddGearColliders();




public:

	Entity* player_entity = nullptr;
	p2List<Entity*> ovnis;
	

	Levels current_level = LEVEL_NONE;
	Levels level_to_load_on_postUpdate = LEVEL_NONE;

	bool camera_change = true;

	int player_lives = 3;

private:

	SDL_Texture* debug_tex;
	bool post_update_ret = true;

	//HARDCODE
	int button_width = 214;
	int button_height = 64;
	SDL_Rect button_up_rect_section = { 0,0,button_width,button_height };
	SDL_Rect button_hover_rect_section = { 0,100,button_width,button_height };
	SDL_Rect button_down_rect_section = { 0,200,button_width,button_height };
	SDL_Rect button_disabled_rect_section = { 0,300,button_width,button_height };

	p2List<Picture*> lives_icons;
	p2List<Picture*> gears;

};

#endif // __j1SCENE_H__