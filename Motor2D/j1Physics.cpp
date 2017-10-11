#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Physics.h"
#include <math.h>


j1Physics::j1Physics() : j1Module()
{
	
}

// Destructor
j1Physics::~j1Physics()
{}

// Called before render is available
bool j1Physics::Awake(pugi::xml_node& config)
{


	return true;
}


bool j1Physics::CleanUp()
{

	return true;
}



