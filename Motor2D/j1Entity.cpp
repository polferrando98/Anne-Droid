#include "j1Entity.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Physics.h"
#include "j1Map.h"

Entity::Entity(fPoint position, EntityType type)
{
	start_position = position;
	this->position = position;
	this->type = type;
	life_state = ALIVE;
	App->physics->LoadPhysicsValues();

	movement_acceleration.y = 9.8f;
	movement_acceleration.x = 0.25f;

	App->physics->AddCollider(&defaultRect, PLAYER);
}


bool Entity::Update(float dt)
{
	AnimationFrame frame = current_animation->GetCurrentFrame();
	App->render->Blit(texture, position.x, position.y, &frame.rect);
	return true;
}

bool Entity::PreUpdate(float dt)
{
	return true;
}

void Entity::UpdateCurrentTile()
{
	current_tile = App->map->WorldToMap(position.x + (collider->rect.w/2), position.y + (collider->rect.h / 2));
}
