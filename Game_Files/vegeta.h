#ifndef _VEGETA_H
#define _VEGETA_H

#include "player.h"

class Vegeta : public Player {
public:
	Vegeta(const std::string&);
	Vegeta(const Vegeta&);					
	// Inherited from drawable
	virtual void draw() const;
	virtual void update(Uint32 ticks);	

	// Vegeta Movement
	virtual void moveRight();
	virtual void moveLeft();
	virtual void moveUp();
	virtual void moveDown();
	virtual void blast();
	void dash(int d);
	
	// Decrement Health
        virtual	void decrementHealthBar() {
        	healthBar.decrement();
        }
        
        void decrementMoreHealth() {
        	healthBar.decrementLittleMore(); 
        }        
        
        
        virtual bool isHealthEmpty() {
        	return healthBar.isEmpty();
        }
        
       	virtual void resetHealth() {
		healthBar.reset();
	}
	// Part of Observer Pattern
	void attach(Player* p) { 
		playerList.push_back(p);
	}
	void detach(Player* p) {
		std::list<Player *>::iterator pItr = playerList.begin();
		while ( pItr != playerList.end() ) {
			if ( p == (*pItr) ) {
				pItr = playerList.erase(pItr);
			}
			else {
				++pItr;
			}
		}	
	}
	void notify() const;
	void resetKioskMode() {
		kioskMode = false;
	}
private:
	// For Vegeta Movement
	std::vector<Frame *> frameX;
	std::vector<Frame *> frameUP;
	std::vector<Frame *> frameDOWN;
	std::vector<Frame *> frameDASH;
	
	bool dashKey;
	bool kioskMode;
	
	std::list<Player* > playerList;	
	
	const Vegeta& operator=(const Vegeta&);		// item 6

};
#endif /* end _VEGETA_H*/
