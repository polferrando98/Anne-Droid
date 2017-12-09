#ifndef __BUTTON_H__
#define __BUTTON_H__
#include "InteractiveUIElement.h"
//#include "UIElement.h"
class Picture; 
class Label;

class Button :
	public InteractiveUIElement
{
public:
	Button(iPoint position);
	bool Update(float dt);
	void CenterLabel();

	void ManageState();

	~Button();

public:
	Label*		label = nullptr;
	iPoint		label_offset = { 0,0 };


	iPoint begin_drag_point;
};

#endif