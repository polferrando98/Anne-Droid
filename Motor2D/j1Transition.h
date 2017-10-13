#ifndef __j1Transition_H__
#define __j1Transition_H__

#include "j1Module.h"
#include "Animation.h"
#include "Globals.h"


struct SDL_Texture;


class j1Transition :
	public j1Module
{
public:

	j1Transition();
	virtual ~j1Transition();

private:

	SDL_Texture* background_graphics;
	
	Animation trans;
	

	int actual = 0;
	int timer = 0;
public:
	bool Init();
	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();


};

#endif
