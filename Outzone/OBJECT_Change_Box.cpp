#include "Application.h"
#include "OBJECT_Change_Box.h"
#include "ModuleCollision.h"
#include "ModuleRender.h"
#include "ModuleObjects.h"

Change_Box::Change_Box(int x, int y) : Object(x, y)
{
	box.PushBack({ 48, 4, 32, 32 });
	flash.PushBack({ 109, 12, 16, 16 });
	flash.PushBack({ 109, 32, 16, 16 });
	flash.speed = 0.25f;
	object_animation = &box;
	type = CHANGE_BOX;
	collider = App->collision->AddCollider({ 0, 0, 28, 28 }, COLLIDER_TYPE::COLLIDER_CHEST, (Module*)App->objects);
}

void Change_Box::Update(){
	position.x += 6;
	position.y += 10;
	object_animation = &flash;
	App->collision->EraseCollider(collider);
	collider = App->collision->AddCollider({position.x, position.y, 15, 15 }, COLLIDER_TYPE::COLLIDER_OBJECT, (Module*)App->objects);
}