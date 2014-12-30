#ifndef _BULLET_H
#define _BULLET_H

#include <iostream>
#include "sprite.h"
#include "gamedata.h"
#include "collisionStrategy.h"

class Bullet : public Sprite { 
public:
	explicit Bullet( const string& name, const Vector2f& pos, const Vector2f& vel, const Frame* frm) :
	Sprite (name, pos, vel, frm),
	distance (0),
	maxDistance(Gamedata::getInstance().getXmlInt(name+"/distance")),
	tooFar(false),
	strategy(new PerPixelCollisionStrategy)
	{ }
	
	virtual void update(Uint32 ticks);
	bool goneTooFar() const { return tooFar; }
	void reset() {
		tooFar = false;
		distance = 0;
	}
	
	~Bullet();
	Bullet(const Bullet&);
	
	// Check for collisions with the current object
	bool collidedWith(const Drawable* d) const {
		return strategy->execute(*this, *d);
	}

private:
	float distance;
	float maxDistance;
	bool tooFar;
	CollisionStrategy* strategy;
	
	const Bullet& operator=(const Bullet&);
};

#endif /* end _BULLET_H */
