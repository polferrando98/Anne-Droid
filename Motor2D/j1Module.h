// ----------------------------------------------------
// j1Module.h
// Interface for all engine modules
// ----------------------------------------------------

#ifndef __j1MODULE_H__
#define __j1MODULE_H__

#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"

class j1App;
class UIElement;
enum Mouse_UI_Event;

class j1Module
{
private:
	bool enabled = true;

public:

	j1Module() : active(false) //I guess this is in case it does not init
	{}

	void Init()
	{
		active = true;
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool save(pugi::xml_node&) const
	{
		return true;
	}

	bool IsEnabled() const { return enabled; }

	void Enable()
	{
		if (enabled == false)
		{
			enabled = true;
			Start();
		}
	}

	void Disable()
	{
		if (enabled == true)
		{
			enabled = false;
			CleanUp();
		}
	}
	virtual void onFadeInEnd() {}


	virtual void OnButtonClick(UIElement* trigger, Mouse_UI_Event mouse_event) {}

	virtual void OnButtonHover(UIElement* trigger, Mouse_UI_Event mouse_event) {}
public:

	p2SString	name;
	bool		active;

};

#endif // __j1MODULE_H__