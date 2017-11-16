#include "j1Entity.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Physics.h"
#include "j1Map.h"
Entity::Entity(fPoint position, EntityType type)
{
	this->position = position;
	this->type = type;
	life_state = ALIVE;
}
bool Entity::Start()
{
	App->physics->LoadPhysicsValues();

	movement_acceleration.y = 10.0f;
	movement_acceleration.x = 0.3f;
	return true;
}

bool Entity::Update(float dt)
{
	AnimationFrame frame = current_animation->GetCurrentFrame();
	UpdateCurrentTile();
	App->render->Blit(texture, position.x, position.y, &frame.rect);
	return true;
}

void Entity::UpdateCurrentTile()
{
	currentTile = App->map->WorldToMap(position.x, position.y);
}

