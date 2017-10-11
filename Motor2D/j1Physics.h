#pragma once
#ifndef __j1PHYSICS_H__
#define __j1PHYSICS_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

// ----------------------------------------------------
class j1Physics : public j1Module
{
public:

	j1Physics();

	// Destructor
	virtual ~j1Physics();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before quitting
	bool CleanUp();


private:


public:


private:

};

#endif // __j1MAP_H__