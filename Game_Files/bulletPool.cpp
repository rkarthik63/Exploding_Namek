#include <iostream>
#include <cmath>
#include "ioManager.h"
#include "gamedata.h"
#include "bullet.h"
#include "bulletPool.h"

BulletPool::~BulletPool() {
 SDL_FreeSurface(bulletSurface);
 delete bulletFrame; 
}

BulletPool::BulletPool( const std::string& n) :
 name(n),
 bulletSurface(IOManager::getInstance().loadAndSet(Gamedata::getInstance().getXmlStr(n+"/file"), true ) ),
 bulletFrame( new Frame(bulletSurface, bulletSurface->w, bulletSurface->h, 0, 0) ),
 frameInterval(Gamedata::getInstance().getXmlInt(n+"/interval")),
 timeSinceLastFrame(0),
 bulletList(),
 freeList()
 { }
 
 
BulletPool::BulletPool(const BulletPool& b) :
 name(b.name),
 //strategy(b.strategy),
 bulletSurface(b.bulletSurface),
 bulletFrame(b.bulletFrame),
 frameInterval(b.frameInterval),
 timeSinceLastFrame(b.timeSinceLastFrame),
 bulletList(b.bulletList),
 freeList(b.freeList)
{ }

void BulletPool::shoot(const Vector2f& pos, const Vector2f& vel) {
 if ( timeSinceLastFrame > frameInterval ) {
  // If no bullets in the pool, make one:
  if ( freeList.empty() ) {
  	Bullet b(name, pos, vel, bulletFrame);
  	bulletList.push_back(b);
  }
  else {
  	Bullet b = freeList.front();
  	freeList.pop_front();
  	b.reset();
  	b.setVelocity(vel);
  	b.setPosition(pos);
  	bulletList.push_back(b);
  }
  timeSinceLastFrame = 0;
 }
}

void BulletPool::draw() const {
// IOManager::getInstance().printMessageValueAt("Bullets: ", bulletList.size(), 20, 10);
// IOManager::getInstance().printMessageValueAt("Bullets: ", freeList.size(), 20, 30); 
 std::list<Bullet>::const_iterator itr = bulletList.begin();
 while ( itr != bulletList.end() ) {
 	itr->draw();
 	++itr;
 }
}

void BulletPool::update(Uint32 ticks) {
 timeSinceLastFrame += ticks;
 std::list<Bullet>::iterator itr = bulletList.begin();
 while ( itr != bulletList.end() ) {
 	itr->update(ticks);
 	if ( itr->goneTooFar() ) {                 // Check to see if the we should free the Bullet
 		freeList.push_back(*itr);
 		itr = bulletList.erase(itr);
 	}
 	else ++itr;
 }
}

bool BulletPool::hasBulletCollided(const Drawable* d) {
	if ( bulletList.size() == 0 ) { return false; }
	std::list<Bullet>::iterator itr = bulletList.begin();
	while ( itr != bulletList.end() ) {
		if ( itr->collidedWith(d) ) {
			freeList.push_back(*itr);
	 		itr = bulletList.erase(itr);
	 		return true;
		}
		else {
			++itr;
		}
	}
	return false;
}

void BulletPool::drawHUD() {
	IOManager::getInstance().printMessageValueAt("Active Bullets: ", bulletList.size(), 15, 190);
	IOManager::getInstance().printMessageValueAt("Free Bullets: ", freeList.size(), 15, 205);
}

void BulletPool::clearList() {
 std::list<Bullet>::iterator bItr = bulletList.begin();
 std::list<Bullet>::iterator fItr = freeList.begin();
 
 while( bItr != bulletList.end() ) {
 	bItr = bulletList.erase(bItr);
 }
 bulletList.clear();
 
 while( fItr != freeList.end() ) {
 	fItr = freeList.erase(fItr);
 }
 freeList.clear(); 
}


