#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1Physics.h"
#include "j1Map.h"
#include "p2PQueue.h"

struct SDL_Texture;

class Entity;

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
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void ChangeMap();

private:

	void CameraFollowPlayer();

public:
	Entity* player_entity = nullptr;

	p2PQueue<Level>* levels;

	//HARDCODE

	Collider* col1;

	p2List<SDL_Rect>* col_rects;

	SDL_Rect col_rect;
	SDL_Rect col_rect2;

	//HARDCODE

	int current_level = 1;
	int next_level = 2;

	bool loading = false;

	iPoint p_clicked;

};

#endif // __j1SCENE_H__