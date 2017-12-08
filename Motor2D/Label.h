#pragma once
#include "UIElement.h"

struct _TTF_Font;

class Label :
	public UIElement
{
public:
	Label(iPoint position, p2SString text = "Empty Text", SDL_Color color = { 255,255,255,255 });
	bool Update(float dt);
	void SetText(p2SString text);
	void SetFont(_TTF_Font* font);
	void UpdateSize();
	~Label();

public:
	iPoint size = { 0,0 };

private:
	p2SString text;
	SDL_Color color;
	_TTF_Font* font = NULL; //To be coherent with the default argument in print

};

