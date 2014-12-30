#ifndef _FRIEZA_H
#define _FRIEZA_H

#include "player.h"
#include "vegeta.h"
#include "sound.h"


class Frieza : public Player {
public:
	Frieza(const std::string&,Vegeta* p);
	Frieza(const Frieza&);					
	// Inherited from drawable
	virtual void draw() const;
	virtual void update(Uint32 ticks);	

	// Frieza Movement
	virtual void moveRight();
	virtual void moveLeft();
	virtual void moveUp();
	virtual void moveDown();
	virtual void blast();
	
	// Decrease health
        virtual	void decrementHealthBar() {
        	healthBar.decrementMore();
        }
        virtual bool isHealthEmpty() {
        	return healthBar.isFull();
        }
      	virtual void resetHealth() {
		healthBar.resetReverse();
	}		
	// Part of Observer Pattern
	void UpdatePos(const Vector2f& pos) {
		vegetaPos = pos;
		angle = getDirection( pos, this->getPosition() );
//		std::cout << "Distance: " << distanceToVegeta << " "<< "Angle: " << angle << " "<< "Firing: " << shouldFriezaFire << " "<< std::endl; 
		if ( ( distanceToVegeta < firingDistance) && (!shouldFriezaFire) ) {
			shouldFriezaFire = true;
		}
		else if ( distanceToVegeta > firingDistance){
			shouldFriezaFire = false;
		}		
	}
	void setMODE(int d) {
		MODE = d;
	}
private:
	enum {NORMAL, EVADE};
	Vector2f vegetaPos;
	float firingDistance;
	bool shouldFriezaFire;
	float distanceToVegeta;
	float angle;
	// Blast Intervals
	float blastInterval;
	float timeSinceLastBlast;
	SDLSound& sound;	
	int MODE;
	
	const Frieza& operator=(const Frieza&);			// Item 6
	
	float getDirection(const Vector2f&, const Vector2f& );
	void reOrientFrieza();
};


#endif /* end _FRIEZA*/
