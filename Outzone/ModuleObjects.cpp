#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleObjects.h"
#include "ModuleParticles.h"
#include "ModuleTextures.h"
#include "Object.h"
#include "OBJECT_Change_Box.h"


#define SPAWN_MARGIN 50

ModuleObjects::ModuleObjects()
{
	for (uint i = 0; i < MAX_OBJECTS; ++i)
		objects[i] = nullptr;
}

// Destructor
ModuleObjects::~ModuleObjects()
{
}

bool ModuleObjects::Start()
{
	// Create a prototype for each enemy available so we can copy them around
	sprites = App->textures->Load("objects_sprites.png");

	return true;
}

update_status ModuleObjects::PreUpdate()
{
	// check camera position to decide what to spawn
	for (uint i = 0; i < MAX_OBJECTS; ++i)
	{
		if (queue[i].type != OBJECT_TYPES::UNKNOWN)
		{
			if (queue[i].y * (-SCREEN_SIZE) < App->render->camera.y + (App->render->camera.h * SCREEN_SIZE) + SPAWN_MARGIN)
			{
				SpawnObject(queue[i]);
				queue[i].type = OBJECT_TYPES::UNKNOWN;
				LOG("Spawning object at %d", queue[i].x * SCREEN_SIZE);
			}
		}
	}

	return UPDATE_CONTINUE;
}

// Called before render is available
update_status ModuleObjects::Update()
{

	for (uint i = 0; i < MAX_OBJECTS; ++i)
		if (objects[i] != nullptr) objects[i]->Draw(sprites);


	return UPDATE_CONTINUE;
}

update_status ModuleObjects::PostUpdate()
{
	// check camera position to decide what to spawn
	for (uint i = 0; i < MAX_OBJECTS; ++i)
	{
		if (objects[i] != nullptr)
		{
			if ((objects[i]->position.y * -2) < (App->render->camera.y - (App->render->camera.h * 2)))
			{
				LOG("DeSpawning object at %d", objects[i]->position.x * SCREEN_SIZE);
				delete objects[i];
				objects[i] = nullptr;
			}
		}
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleObjects::CleanUp()
{
	LOG("Freeing all objects");

	App->textures->Unload(sprites);

	for (uint i = 0; i < MAX_OBJECTS; ++i)
	{
		if (objects[i] != nullptr)
		{
			delete objects[i];
			objects[i] = nullptr;
		}
	}

	return true;
}

bool ModuleObjects::AddObject(OBJECT_TYPES type, int x, int y)
{
	bool ret = false;

	for (uint i = 0; i < MAX_OBJECTS; ++i)
	{
		if (queue[i].type == OBJECT_TYPES::UNKNOWN)
		{
			queue[i].type = type;
			queue[i].x = x;
			queue[i].y = y;
			ret = true;
			break;
		}
	}

	return ret;
}

void ModuleObjects::SpawnObject(const ObjectInfo& info)
{
	// find room for the new enemy
	uint i = 0;
	for (; objects[i] != nullptr && i < MAX_OBJECTS; ++i);

	if (i != MAX_OBJECTS)
	{
		switch (info.type)
		{
		case OBJECT_TYPES::CHANGE_BOX:
			objects[i] = new Change_Box(info.x, info.y);
			break;

		case OBJECT_TYPES::UPGRADE_BOX:
			//
			break;
		}
	}
}

void ModuleObjects::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_OBJECTS; ++i)
	{
		if (c1->type == COLLIDER_OBJECT && c2->type == COLLIDER_PLAYER_SHOT)
		{
			if (objects[i] != nullptr && objects[i]->GetCollider() == c1)
			{
				App->particles->AddParticle(App->particles->basic_robot_explosion, App->objects->objects[i]->position.x, App->objects->objects[i]->position.y, COLLIDER_NONE, UNDEFINED);
				delete objects[i];
				objects[i] = nullptr;
				break;
			}
		}
	}
}