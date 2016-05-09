#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "p2Point.h"
#include "Animation.h"

struct SDL_Texture;
struct Collider;

class Enemy
{
protected:

	Animation* enemy_animation;
	Collider* collider;

public:
	iPoint position;
	//Miliseconds since the init
	Uint32 current_time;
	//Miliseconds since the last timed action
	Uint32 last_time;
	//Fire Rate
	Uint32 fire_rate;

public:
	Enemy(int x, int y);
	virtual ~Enemy();

	const Collider* GetCollider() const;

	virtual void Move() {};
	virtual void Draw(SDL_Texture* sprites);
	virtual void Attack(){};
	virtual void Focus(){}; //Looks player position

};

#endif // __ENEMY_H__