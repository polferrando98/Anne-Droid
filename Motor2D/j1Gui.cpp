#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "Picture.h"
#include "Label.h"
#include "Button.h"
#include "Window.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
	curr_element = nullptr;
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	//Hardcode
	atlas_file_name = "gui/atlas.png";

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		debug_draw = !debug_draw;

	}

	ManageFocus();

	if (!elements.start)
		return ret;
	for (p2List_item<UIElement*>* element_iterator = elements.start; element_iterator != nullptr; element_iterator = element_iterator->next) {
		ret = element_iterator->data->PreUpdate();
	}

	return ret;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	bool ret = true;
	if (!elements.start)
		return ret;

	for (p2List_item<UIElement*>* element_iterator = elements.start; element_iterator != nullptr; element_iterator = element_iterator->next) {
		ret = element_iterator->data->Update(App->dt);

		if (debug_draw)
			element_iterator->data->DebugDraw();
		element_iterator->data->UpdateOldPos();
	}

	return ret;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	return true;
}

void j1Gui::ManageFocus()
{
	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
	{
		if (curr_tab == elements.count() - 1)
			curr_tab = 0;
		else
			curr_tab++;

		switch (elements.At(curr_tab)->data->type)
		{
		case BUTTON:
			LOG("BUTTON");
			break;

		case PICTURE:
			LOG("PICTURE");
			break;

		case LABEL:
			LOG("LABEL");
			break;
		}

	}
}

void j1Gui::CleanAllUI()
{
	if (elements.start) {
		p2List_item<UIElement*>* element_iterator;
		for (element_iterator = elements.start; element_iterator != NULL; element_iterator = element_iterator->next)
		{
			RELEASE(element_iterator->data)
		}
		elements.clear();
	}
}

UIElement* j1Gui::AddUIElement(iPoint position, UIType type)
{
	UIElement* elem;
	switch (type)
	{
	case LABEL:
		elem = new Label(position);
		break;
	case PICTURE:
		elem = new Picture(position);
		elem->texture = atlas;
		break;
	case BUTTON:
		elem = new Button(position);
		elem->texture = atlas;
		break;
	default:
		return nullptr;
		break;
	}
	elements.add(elem);
	return elem;
}

Label* j1Gui::AddUIText(iPoint position, p2SString text, _TTF_Font* font)
{
	UIElement* elem;

	elem = new Label(position, text);
	Label* new_label = (Label*)elem;

	elem->id = elements.count();

	if (font != NULL) {
		new_label->SetFont(font);
	}

	elements.add(elem);

	return new_label;
}

Picture* j1Gui::AddUIPicture(iPoint position, p2SString texture_name, SDL_Rect section)
{
	UIElement* elem;

	elem = new Picture(position);
	Picture* new_picture = (Picture*)elem;

	elem->id = elements.count();

	if (texture_name != "") {
		SDL_Texture* newTexture = nullptr;
		newTexture = App->tex->Load(texture_name.GetString());
		elem->texture = newTexture;
	}
	else
		elem->texture = atlas;

	if (!SDL_RectEmpty(&section))
		elem->section = section;

	elements.add(elem);

	return new_picture;
}

Button * j1Gui::AddUIButton(iPoint position, p2SString label_text, j1Module* lisener, SDL_Rect up, SDL_Rect hover, SDL_Rect down)
{
	UIElement* elem;

	elem = new Button(position);
	Button* new_Button = (Button*)elem;

	elem->id = elements.count();

	if (label_text != "") {
		new_Button->label->SetText(label_text);
	}


	elem->texture = atlas;

	if (!SDL_RectEmpty(&up)) {
		new_Button->up = up;
		new_Button->section = up;
		new_Button->SetPositionRect();
	}

	if (!SDL_RectEmpty(&hover)) {
		new_Button->hover = hover;
	}

	if (!SDL_RectEmpty(&down)) {
		new_Button->down = down;
	}

	new_Button->MoveInPercentage({ (float)position.x,(float)position.y });

	elements.add(elem);

	return new_Button;
}

Window * j1Gui::AddUIWindow(iPoint position, SDL_Rect section)
{
	UIElement* elem;

	elem = new Window(position);
	Window* new_window = (Window*)elem;

	elem->id = elements.count();

	elem->texture = atlas;

	if (!SDL_RectEmpty(&section))
		elem->section = section;

	elements.add(elem);

	return new_window;
}

// const getter for atlas
SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------

