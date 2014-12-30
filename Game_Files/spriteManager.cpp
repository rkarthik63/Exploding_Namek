#include <iostream>
#include <string>
#include <iomanip>
#include "gamedata.h"
#include "spriteManager.h"
#include "vector2f.h"
#include <algorithm>

class rotateScaledSpriteCompare {
public:
	bool operator()(const RotateScaledSprite* lhs, const RotateScaledSprite* rhs) {
		return lhs->getScale() < rhs->getScale();
	}
};
/*
SpriteManager& SpriteManager::getInstance() {
	static SpriteManager instance;
	return instance;
}*/

// Constructor
SpriteManager::SpriteManager() :
 bg1("back1", Gamedata::getInstance().getXmlInt("back1/factor") ),
 bg2("back2", Gamedata::getInstance().getXmlInt("back2/factor") ),
 fg1("back3", Gamedata::getInstance().getXmlInt("back3/factor") ),
 fg2("back4", Gamedata::getInstance().getXmlInt("back4/factor") ),
 sprites(),
 currentSprite(0),
 meteorSurface(IOManager::getInstance().loadAndSet(Gamedata::getInstance().getXmlStr("meteor/file"), Gamedata::getInstance().getXmlBool("meteor/transparency")) ),
 meteor(),
 vegetaExploded( false ),
 friezaExploded( false ),
 meteorExploded( false ),
 explodeVegetaPtr( NULL ),
 explodeFriezaPtr( NULL ),
 explodeMeteorPtr( NULL ),
 meteorDensity(Gamedata::getInstance().getXmlInt("meteor/meteorDensity")),
 godMode(false),
 vegetaLost(false)
{ 
  sprites.push_back( new Vegeta("vegeta") );
  Vegeta* p = static_cast<Vegeta* >(sprites[0]);
  sprites.push_back( new Frieza("frieza", p));
  makeMeteor();    
}

// Destructor 
SpriteManager::~SpriteManager() { 
  for (unsigned i = 0; i < sprites.size(); ++i) {
    delete sprites[i];
  }
  sprites.clear();
//  delete explodeMeteor;
  SDL_FreeSurface(meteorSurface);
      
  std::list<RotateScaledSprite *>::iterator itr = meteor.begin();
  RotateScaledSprite* tmp = NULL;
  while (itr != meteor.end() ) {
  	tmp = *itr;
  	itr = meteor.erase(itr);
  	delete tmp;
  }
  if (explodeVegetaPtr != NULL)  {
  	delete explodeVegetaPtr;
  }
  if (explodeFriezaPtr != NULL) {
  	delete explodeFriezaPtr;
  }
}

// Retrieve the Drawable* for the current sprite
const Drawable* SpriteManager::getSpriteCurrentSprite() const {
	return sprites[currentSprite];
}
// Retrieve the current sprite in integer variables
int SpriteManager::getCurrentSprite() const {
	return currentSprite;
}

// Draw the sprites in the 
void SpriteManager::drawSprites() const {
	std::list<RotateScaledSprite *>::const_iterator mItr = meteor.begin();
	std::list<RotateScaledSprite *>::const_iterator cItr25 = meteor.begin();
	std::list<RotateScaledSprite *>::const_iterator cItr75 = meteor.begin();
	std::list<RotateScaledSprite *>::const_iterator mEtr = meteor.end();
	
	std::advance(cItr25, static_cast<int>(meteorDensity/4));
	std::advance(cItr75, static_cast<int>(3 *(meteorDensity/4) ) );

	bg1.draw();
	
	while( mItr != cItr25 ) {	
		(*mItr)->draw();
		++mItr;
	}
	bg2.draw();
	while( mItr != cItr75) {	
		(*mItr)->draw();
		++mItr;
	}
	
	fg2.draw();
	fg1.draw();
	while( mItr != mEtr) {	
		(*mItr)->draw();
		++mItr;
	}
	
	if ( !vegetaExploded ) {
		sprites[0]->draw();
	}	
	
	if ( !friezaExploded ) {
		sprites[1]->draw();
	}	
		
	if ( vegetaExploded ) {
		explodeVegetaPtr->draw();		
	}
	
	if ( friezaExploded ) {
		explodeFriezaPtr->draw();
	}
	
	if (meteorExploded) {
		explodeMeteorPtr->draw();	
	}	
}
// Update the sprites in the vector
void SpriteManager::updateSprites(int ticks) {
	for (unsigned int i = 0; i < sprites.size(); ++i) {
		sprites[i]->update(ticks);
	}	
	for (std::list<RotateScaledSprite *>::iterator mItr = meteor.begin(); mItr != meteor.end(); ++mItr ) {	
		(*mItr)->update(ticks);
	}	
	bg1.update();
	bg2.update();
  	fg1.update();
  	fg2.update();
  	
  	// Check for player Collisionss
  	if ( checkForVegetaCollisions() && (!godMode)) {
		vegetaHealthDecrement();
		if ( vegetaHealthIsZero() ) {
			vegetaExploded = true;
			explodeVegetaNow();
		}
	}
	
	if ( vegetaExploded )	{
		explodeVegetaPtr->update(ticks);
		if ( explodeVegetaPtr->isChunkEmpty() ) {
			delete explodeVegetaPtr;
			explodeVegetaPtr = NULL;			
			vegetaExploded = false;
			vegetaLost = true;			
			static_cast<Vegeta* >(sprites[0])->resetHealth();		
		}		
	}
	
	if ( friezaExploded )	{
		explodeFriezaPtr->update(ticks);
		if ( explodeFriezaPtr->isChunkEmpty() ) {
			delete explodeFriezaPtr;
			explodeFriezaPtr = NULL;
			friezaExploded = false;
			vegetaLost = true;		
			static_cast<Frieza* >(sprites[1])->resetHealth();
		}		
	}	
	
	if ( meteorExploded ) {
		explodeMeteorPtr->update(ticks);
	}
	
	if (meteorExploded && explodeMeteorPtr->isChunkEmpty() ) {
		meteorExploded = false;
		ExplodingSprite* tmp = explodeMeteorPtr;
		explodeMeteorPtr = NULL;
		delete tmp;	
	}
	
	// Check for Bullet Collisions and explode meteors
	checkForBulletMeteorCollisions();		
	playersBulletCollisionCheck();
}
// Tracks the next sprite
void SpriteManager::trackNextSprite() {
	currentSprite = (currentSprite+1) % sprites.size();
}

void SpriteManager::makeMeteor() {
	for (int i = 0; i < meteorDensity; ++i ) {	
		meteor.push_back( new RotateScaledSprite("meteor", meteorSurface, i) );
	}
	meteor.sort( rotateScaledSpriteCompare() ); 
}

void SpriteManager::vegetaStop() {
	static_cast<Vegeta* >(sprites[0])->stop();
//	static_cast<Frieza* >(sprites[1])->stop();		
}

void SpriteManager::vegetaLeft() {
	static_cast<Vegeta* >(sprites[0])->moveLeft();
//	static_cast<Frieza* >(sprites[1])->moveLeft();		
}

void SpriteManager::vegetaRight() {
	static_cast<Vegeta* >(sprites[0])->moveRight();
//	static_cast<Frieza* >(sprites[1])->moveRight();		
}

void SpriteManager::vegetaUp() {
	static_cast<Vegeta* >(sprites[0])->moveUp();
//	static_cast<Frieza* >(sprites[1])->moveUp();		
}

void SpriteManager::vegetaDown() {	
	static_cast<Vegeta* >(sprites[0])->moveDown();
//	static_cast<Frieza* >(sprites[1])->moveDown();		
}

void SpriteManager::vegetaDash(int d) {
	static_cast<Vegeta* >(sprites[0])->dash(d);
}

void SpriteManager::vegetaBlast() {
	static_cast<Vegeta* >(sprites[0])->blast();
//	static_cast<Frieza* >(sprites[1])->blast();	
}


void SpriteManager::vegetaResetPosition() {
	static_cast<Vegeta* >(sprites[0])->setPosition( Vector2f(120, 320));
	static_cast<Vegeta* >(sprites[0])->resetHealth();
	static_cast<Vegeta* >(sprites[0])->clearBulletList();
	static_cast<Vegeta* >(sprites[0])->resetKioskMode();
	static_cast<Frieza* >(sprites[1])->resetHealth();
	godMode = false;
}


// Check for Vegeta's Collisions with the foreground meteors

bool SpriteManager::checkForVegetaCollisions() const {
	std::list<RotateScaledSprite *>::const_iterator sItr = meteor.begin();
	std::list<RotateScaledSprite *>::const_iterator eItr = meteor.end();
	std::advance(sItr, static_cast<int>(3 *(meteorDensity/4) ) );	
	while ( sItr != eItr ) {
		if ( static_cast<Vegeta* >(sprites[0])->collidedWith((*sItr) ) ) return true;		
		++sItr;
	}
	return false;
}

void SpriteManager::checkForBulletMeteorCollisions() {
	std::list<RotateScaledSprite *>::iterator sItr = meteor.begin();
	std::list<RotateScaledSprite *>::iterator eItr = meteor.end();
	std::advance(sItr, static_cast<int>(3 *(meteorDensity/4) ) );	
	while ( sItr != eItr ) {
		if ( static_cast<Vegeta* >(sprites[0])->checkBulletCollisions((*sItr) ) ) {
			explodeMeteorPtr = new ExplodingSprite("meteor", (*sItr)->getPosition(), Vector2f(150, 150));
			meteorExploded = true;
			// Delete the Meteor
			RotateScaledSprite* tmp = *sItr;
			sItr = meteor.erase(sItr);
			delete tmp; 
			}
		else {
			++sItr;
		} 
	}	
}
// Players' Bullet Collisions
void SpriteManager::playersBulletCollisionCheck() {
	Player* vegeta = static_cast<Vegeta* >(sprites[0]);
	Player* frieza = static_cast<Frieza* >(sprites[1]);
	// Check for vegeta's bullet hitting frieza
	if ( vegeta->checkBulletCollisions(frieza) ) {
		frieza->decrementHealthBar();
		static_cast<Frieza* >(frieza)->setMODE(1);
		if ( frieza->isHealthEmpty() ) {
			friezaExploded = true;						
			explodeFriezaNow();
		}
	}
	// Check for Frieza's Bullet hitting Vegeta
	if ( ( frieza->checkBulletCollisions(vegeta)) && (!godMode) )  {
		 static_cast<Vegeta* >(vegeta)->decrementMoreHealth();		
		if ( vegeta->isHealthEmpty() ) {
			vegetaExploded = true;
			explodeVegetaNow();
		}
	}
}

void SpriteManager::explodeVegetaNow() {
	if ( explodeVegetaPtr != NULL ) {
		ExplodingSprite* tmp = explodeVegetaPtr;
		explodeVegetaPtr = NULL;
		delete tmp;	
	}
	Vegeta* p = static_cast<Vegeta *>(sprites[0]);
	explodeVegetaPtr = new ExplodingSprite("vegeta", p->getPosition(), Vector2f(100, 100), p->getFrame() );		
}

void SpriteManager::explodeFriezaNow() {
	if ( explodeFriezaPtr != NULL ) {
		ExplodingSprite* tmp = explodeFriezaPtr;
		explodeFriezaPtr = NULL;
		delete tmp;	
	}
	Frieza* p = static_cast<Frieza *>(sprites[1]);
	explodeFriezaPtr = new ExplodingSprite("frieza", p->getPosition(), Vector2f(100, 100), p->getFrame() );		
}

void SpriteManager::deleteMeteors() {
  std::list<RotateScaledSprite *>::iterator itr = meteor.begin();
  RotateScaledSprite* tmp = NULL;
  while (itr != meteor.end() ) {
  	tmp = *itr;
  	itr = meteor.erase(itr);
  	delete tmp;
  }
}
