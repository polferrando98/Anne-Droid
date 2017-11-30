#pragma once
#include "InteractiveUIElement.h"
class Window :
	public InteractiveUIElement
{
public:
	Window(iPoint position);

	bool Update(float dt);

	~Window();
};

