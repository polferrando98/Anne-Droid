#include "j1Entity.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"

bool Entity::Start()
{
	return false;
}

bool Entity::Update(float dt)
{
	AnimationFrame frame = current_animation->GetCurrentFrame();
	//App->render->Blit(texture, position.x, position.y, &frame.rect);
	return true;
}
