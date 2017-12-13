#include "UIElement.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Window.h"

void UIElement::Draw()  
{
	SetDrawPosition();
	if (SDL_RectEmpty(&section)) {
		App->render->Blit(texture, draw_positon.x, draw_positon.y);
	}
	else {
		App->render->Blit(texture, draw_positon.x, draw_positon.y, &section);
	}
}

void UIElement::SetDrawPosition()
{
	if (!parent)
	{
		if (move_with_camera)
		{
			draw_positon.x = position.x - App->render->camera.x;
			draw_positon.y = position.y - App->render->camera.y;
		}

		else
			draw_positon = position;
	}

	else
	{
		if (move_with_camera)
		{
			draw_positon.x = position.x - App->render->camera.x + parent->position.x;
			draw_positon.y = position.y - App->render->camera.y + parent->position.y;
		}

		else
			draw_positon = position + parent->position;
	}
}


void UIElement::DebugDraw()
{
	if (!SDL_RectEmpty(&section))
		App->render->DrawQuad({ draw_positon.x, draw_positon.y, section.w, section.h }, 255, 0, 0, 255, false);

	else { //if section is empty we will draw the full texture
		iPoint size;
		SDL_QueryTexture(texture, 0, 0, &size.x, &size.y);
		App->render->DrawQuad({ draw_positon.x, draw_positon.y, size.x, size.y }, 255, 0, 0, 255, false);
	}
}

void UIElement::MoveInPercentage(fPoint position_in_percentage)
{
	position_in_percentage.x /= float(100);
	position_in_percentage.y /= float(100);
	fPoint new_position;
	iPoint window_size;
	SDL_GetWindowSize(App->win->window, &window_size.x, &window_size.y);
	position.x = (window_size.x * position_in_percentage.x);
	position.y = (window_size.y * position_in_percentage.y);

	AdjustToPivot();
}

void UIElement::AdjustToPivot()
{
	if (!SDL_RectEmpty(&section)) {
		position.x -= (section.w / 2);
		position.y -= (section.h / 2);
	}
	else { //if section is empty we will draw the full texture
		iPoint size;
		SDL_QueryTexture(texture, 0, 0, &size.x, &size.y);
		position.x -= size.x / 2;
		position.y -= size.y / 2;
	}

}

void UIElement::UpdateOldPos()
{
	old_position = draw_positon;
}

bool UIElement::PositionChanged()
{
	return (old_position != draw_positon) ? true : false;
}

void UIElement::SetParent(UIElement * parent)
{
	this->parent = parent;
	parent->sons.add(this);
}


