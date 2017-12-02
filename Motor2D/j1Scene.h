#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1Physics.h"
#include "j1Map.h"
#include "p2PQueue.h"

struct SDL_Texture;

class Entity;

enum Levels {START_MENU, LEVEL_1, LEVEL_2, END};

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

	bool ChangeMap();

	void ResetOvnis();

	void DebugCamera(Direction_x type, Direction_y type2);

	bool load(pugi::xml_node &save);
	bool save(pugi::xml_node &save) const;

private:

	void CameraFollowPlayer();
	void CheckDoorEntrance();
	void ManageInput();
	void DrawDebugPathfinding();
	void SetCurrentLevel();
	void LoadCurrentLevel();
	void CleanLevel();

	SDL_Texture* debug_tex;

public:
	Entity* player_entity = nullptr;
	p2List<Entity*> ovnis;
	p2PQueue<Level>* levels;

	Levels current_level_enum = LEVEL_1;
	Levels next_level;

	//HARDCODE

	Collider* col1;

	p2List<SDL_Rect>* col_rects;

	SDL_Rect col_rect;
	SDL_Rect col_rect2;

	//HARDCODE

	int current_level = 1;  //Should be deleted

	bool loading = false;

	bool camera_change = true;
};

#endif // __j1SCENE_H__