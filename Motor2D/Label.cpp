#include "Label.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Fonts.h"


Label::Label(iPoint position, p2SString text, SDL_Color color) : UIElement(position, LABEL)
{
	this->texture = App->font->Print(text.GetString(), color);
	UpdateSize();
	this->text = text;
	this->color = color;
}

bool Label::Update(float dt)
{
	Draw();
	return true;
}

void Label::SetText(p2SString text)
{
	this->text = text;
	texture = App->font->Print(text.GetString(), color, font);
	UpdateSize();
}

void Label::SetFont(_TTF_Font* font)
{
	this->font = font;
	texture = App->font->Print(text.GetString(), color, font);
	UpdateSize();
}

void Label::UpdateSize()
{
	SDL_QueryTexture(texture, 0, 0, &size.x, &size.y);
}

Label::~Label()
{
}
