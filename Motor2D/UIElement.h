#ifndef __UIELEMENT_H__
#define __UIELEMENT_H__
#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"
#include "p2Point.h"
#include "p2List.h"
#include "SDL\include\SDL_rect.h"

struct SDL_Texture;
enum UIType { LABEL, PICTURE, BUTTON, WINDOW };
enum ElementState { ELEMENT_UP, ELEMENT_HOVER, ELEMENT_DOWN, ELEMENT_DISABLED };

class j1Module;


class UIElement
{
public:
	UIElement(iPoint position, UIType type) {
		this->position = position;
		this->texture = texture;
		this->type = type;

	};
	~UIElement() {};


	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
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

	void Draw();

	void SetDrawPosition();

	void DebugDraw();

	void MoveInPercentage(fPoint position_in_percentage);

	void AdjustToPivot();

	void UpdateOldPos();

	bool PositionChanged();

	void SetParent(UIElement* parent);

public:
	p2SString			name = "No name";
	iPoint				position = { 0, 0 };
	iPoint				draw_positon = { 0,0 };
	iPoint				old_position = position;
	fPoint				pivot = { 0.5,0.5 };
	int					z = 0;
		
	SDL_Texture*		texture = nullptr;

	UIType				type;

	SDL_Rect			section = { 0,0,0,0 };

	bool				move_with_camera = true;
	bool				focus = true;

	uint				id =0; 

	j1Module*			listener = nullptr;
	UIElement*			parent = nullptr;
	p2List<UIElement*>	sons;

	ElementState		state = ELEMENT_UP;
};

#endif