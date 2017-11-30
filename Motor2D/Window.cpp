#include "Window.h"


Window::Window(iPoint position) :InteractiveUIElement(position, WINDOW)
{

}

bool Window::Update(float dt)
{
	bool ret;

	ManagePositionChanges();

	ret = CheckPositionRect(type);

	ManageEvents();

	ManageState();

	ManageDrag();

	Draw();

	element_last_event = element_event;
	return ret;
}

Window::~Window()
{
}
