#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "UIElement.h"

#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes

struct _TTF_Font;

class Label;
class Picture;
class Button;
class Window;

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void ManageFocus();

	// Gui creation functions
	UIElement* AddUIElement(iPoint position, UIType type);

	Label* AddUIText(iPoint position, p2SString text, _TTF_Font* font = NULL);

	Picture* AddUIPicture(iPoint position, p2SString texture_name = "", SDL_Rect section = { 0,0,0,0 });

	Button* AddUIButton(iPoint position, p2SString label_text = "",p2SString texture_name = "", SDL_Rect up = { 0,0,0,0 }, SDL_Rect hover = { 0,0,0,0 }, SDL_Rect down = { 0,0,0,0 });

	Window* AddUIWindow(iPoint position, SDL_Rect section = { 0,0,0,0 });

	SDL_Texture* GetAtlas() const;

public:
	bool debug_draw = false;

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;


	p2List<UIElement*> elements;
	UIElement* curr_element; 

	uint curr_tab = 0; 

	
};

#endif // __j1GUI_H__