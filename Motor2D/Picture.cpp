#include "Picture.h"
#include "j1App.h"
#include "j1Render.h"

Picture::Picture(iPoint position) : UIElement(position, PICTURE)
{

}

bool Picture::Update(float dt)
{
	Draw();
	return true;
}

Picture::~Picture()
{
}
