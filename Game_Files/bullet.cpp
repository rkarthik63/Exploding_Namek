#include <iostream>
#include <cmath>
#include "bullet.h"

Bullet::~Bullet() {
	delete strategy;
}

Bullet::Bullet(const Bullet& s) :
	Sprite (s),
	distance (s.distance),
	maxDistance(s.maxDistance),
	tooFar(s.tooFar),
	strategy(new PerPixelCollisionStrategy)
	{}
	
void Bullet::update(Uint32 ticks) {
 Vector2f pos = getPosition();
 Sprite::update(ticks);
 distance += ( hypot( X() - pos[0], Y() - pos[1] ) );
 if ( distance > maxDistance ) tooFar = true;
}


