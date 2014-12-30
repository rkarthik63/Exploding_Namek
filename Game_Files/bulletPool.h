#ifndef _BULLETPOOL_H
#define _BULLETPOOL_H
#include <list>
#include "bullet.h"


class BulletPool {

public:
 BulletPool(const std::string& );
 BulletPool(const BulletPool& );
 ~BulletPool();
 
 // Doesn't derive from Drawable but has a draw and an update function
 void draw() const;
 void update( Uint32 ticks);
 void shoot(const Vector2f& pos, const Vector2f& vel);
 
 unsigned int bulletCount() const { return bulletList.size(); }
 unsigned int freeCount() const { return freeList.size(); }
 bool shooting() const { return bulletList.empty(); }
 
 bool hasBulletCollided(const Drawable* d);
 void drawHUD();
 void clearList();
 

private:
 std::string name;
 SDL_Surface* bulletSurface;
 Frame* bulletFrame;
 
 float frameInterval;
 float timeSinceLastFrame;
 
 mutable std::list<Bullet> bulletList;		// Bullet is a list of Bullet
 mutable std::list<Bullet> freeList;		// when a Bullet is out of range
 
 BulletPool& operator=(const BulletPool&);		// (Item 6)

};

#endif /* _BULLETPOOL_H */ 
