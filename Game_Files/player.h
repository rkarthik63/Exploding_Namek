#ifndef _PLAYER_H
#define _PLAYER_H
#include <string>
#include <vector>

#include "drawable.h"
#include "frame.h"
#include "bulletPool.h"
#include "health.h"
#include "collisionStrategy.h"

class Player : public Drawable {
public:
	Player(const std::string&);
 	Player(const Player&);
 	
 	
 	// Derived from drawable
 	virtual void draw() const = 0;
	virtual void update(Uint32 ticks) = 0;	
	virtual ~Player() { 
		delete strategy;
	}
	// Player Movement
	void stop();
	virtual void moveRight() = 0;
	virtual void moveLeft() = 0;
	virtual void moveUp() = 0;
	virtual void moveDown() = 0;

	virtual void blast() = 0;
	
	virtual const Frame* getFrame() const { 
		return curMovement[currentFrame]; 
	}
	// Check for collisions with the current object
	bool collidedWith(const Drawable* d) const {
		return strategy->execute(*this, *d);
	}
	
	// Decrease player's health
	virtual void decrementHealthBar() = 0;
	virtual bool isHealthEmpty() = 0;
	virtual void resetHealth() = 0;	
	
		
	// Check for bullet Collisions
	bool checkBulletCollisions(const Drawable* d) {
		return	bullets.hasBulletCollided(d);
	}
	// Draw HUD
	void drawHUD() {
		bullets.drawHUD();
	}
	void clearBulletList() {
		bullets.clearList();
	}
		
protected:	
	const std::vector<Frame *> frameSTAND;
	const std::vector<Frame *> frameBLAST;
	std::vector<Frame *> curMovement;
	
	int worldWidth;
	int worldHeight;

	unsigned currentFrame;
	unsigned numberOfFrames;
	unsigned frameInterval;
	float timeSinceLastFrame;

	int frameWidth;
	int frameHeight;

	bool Xdirection;
	bool Ydirection;	
	
	bool keyPressedX;
	bool keyPressedY;		
	
	bool blastKey;
	float slowFactor;
	
	// Bullets
	std::string bulletName;
	BulletPool bullets;
	float minSpeed;
	
	// Health HUD
	Health healthBar;
	
	// Collision with Meteors
	CollisionStrategy* strategy;
	
	// For advancing the frame
	void advanceFrame(Uint32 ticks);
	const Player& operator=(const Player&);
};

#endif /* end _PLAYER_H */
