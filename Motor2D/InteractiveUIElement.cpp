#include "InteractiveUIElement.h"
#include "j1App.h"
#include "j1Input.h"
#include "p2Log.h"

#include "j1Render.h"
#include "j1Window.h"
#include "j1Module.h"

InteractiveUIElement::InteractiveUIElement(iPoint position, UIType type) : UIElement(position, type)
{
	this->position = position;
	this->type = type;

	SetDrawPosition();
	SetPositionRect();
}

InteractiveUIElement::~InteractiveUIElement()
{
}

void InteractiveUIElement::SetPositionRect()
{
	position_rect =
	{
		draw_positon.x,
		draw_positon.y,
		section.w,
		section.h
	};
}

void InteractiveUIElement::ManageEvents()
{
	SDL_Point pos;
	App->input->GetMousePosition(pos.x, pos.y);
	if (move_with_camera) {
		pos.x -= App->render->camera.x;
		pos.y -= App->render->camera.y;
	}

	switch (state)
	{
	case ELEMENT_UP:
		if (SDL_PointInRect(&pos, &position_rect))
		{
			element_event = MOUSE_ENTER;

			LOG("MOUSE HAS ENTERED");
		}
		break;
	case ELEMENT_HOVER:
		if (!SDL_PointInRect(&pos, &position_rect))
		{
			element_event = MOUSE_LEAVE;
			LOG("MOUSE HAS LEFT");
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
			element_event = CLICK_DOWN;
		}
		break;
	case ELEMENT_DOWN:
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		{
			element_event = CLICK_UP;
			if (listener)
				listener->OnButtonClick(this, element_event);
			break;
		}
	}
}

void InteractiveUIElement::ManagePositionChanges()
{
	if (PositionChanged()) {
		AdjustToPivot();
		SetPositionRect();
	}
}

bool InteractiveUIElement::CheckPositionRect(UIType type)
{
	if (SDL_RectEmpty(&position_rect))
	{
		LOG("Error, button without position rect, type = %d", type);
		return false;
	}
	return true;
}

void InteractiveUIElement::saveMousePos(iPoint & mousePos)
{
	App->input->GetMousePosition(mousePos.x, mousePos.y);
}

bool InteractiveUIElement::ManageDrag()
{
	if (!draggabel)
		return false;

	if (element_event != element_last_event) {
		if (element_event == CLICK_DOWN)
			saveMousePos(begin_drag_point);
	}

	if (state == ELEMENT_DOWN) {
		if (parent)
			if (parent->state == ELEMENT_DOWN)
				return true;

		iPoint curr_mouse_pos;
		App->input->GetMousePosition(curr_mouse_pos.x, curr_mouse_pos.y);

		iPoint differential = curr_mouse_pos - begin_drag_point;

		position += differential;

		SetPositionRect();
		saveMousePos(begin_drag_point);
	}
	return true;
}

void InteractiveUIElement::ManageState()
{
	switch (element_event)
	{
	case MOUSE_ENTER:
		state = ELEMENT_HOVER;
		break;
	case MOUSE_LEAVE:
		state = ELEMENT_UP;
		break;
	case CLICK_DOWN:
		state = ELEMENT_DOWN;
		break;
	case CLICK_UP:
		state = ELEMENT_HOVER;
		break;
	}
}

void InteractiveUIElement::Draw()
{
	SetPositionRect();
	SetDrawPosition();
	if (SDL_RectEmpty(&section)) {
		App->render->Blit(texture, draw_positon.x, draw_positon.y);
	}
	else {
		App->render->Blit(texture, draw_positon.x, draw_positon.y, &section);
	}
}
