#pragma once
#include "UIElement.h"
class Picture :
	public UIElement
{
public:
	Picture(iPoint position);
	~Picture();
	bool Update(float dt);

public:

};

