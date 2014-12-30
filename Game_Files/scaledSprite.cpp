#include <iostream>
#include <cstdlib>
#include <cmath>
#include "SDL/SDL_rotozoom.h"
#include "scaledSprite.h"
#include "gamedata.h"

int getRandom() {
	int number = Gamedata::getInstance().getRandInRange(0, 75);
	number *= rand() % 2 ? -1 : 1;
	return number;
}

ScaledSprite::ScaledSprite(const std::string& name, SDL_Surface* surface, const int x) :
	Drawable(name,
		 Vector2f( (Gamedata::getInstance().getXmlInt(name+"/startLoc/x") + (x * ( static_cast<float>(Gamedata::getInstance().getXmlInt("world/width")) / (Gamedata::getInstance().getXmlInt("raindrop/rainDensity")) ) ) ), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
                 Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"), 
                    Gamedata::getInstance().getXmlInt(name+"/speedY") + getRandom())
        ),
	scale( Gamedata::getInstance().getRandFloat( Gamedata::getInstance().getXmlFloat(name+"/scale/min"), Gamedata::getInstance().getXmlFloat(name+"/scale/max") ) ),
	scaledSurface( rotozoomSurface(surface, 0, scale, SMOOTHING_ON) ),
	frame (new Frame(scaledSurface, scaledSurface->w, scaledSurface->h,
			 Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
			 Gamedata::getInstance().getXmlInt(name+"/startLoc/y") )
	),
	frameWidth( frame->getWidth() ),
	frameHeight( frame->getHeight() ),
	worldWidth( Gamedata::getInstance().getXmlInt("world/width") ),
	worldHeight( Gamedata::getInstance().getXmlInt("world/height") )
{ }
	                

ScaledSprite::ScaledSprite( const ScaledSprite& s ) :
	Drawable(s.getName(), s.getPosition(), s.getVelocity() ),
	scale(s.scale), 
	scaledSurface(s.scaledSurface), 
	frame(s.frame),
	frameWidth(s.frameWidth),
	frameHeight(s.frameHeight),
	worldWidth(s.worldWidth),
	worldHeight(s.worldHeight)
{ }
	 
const ScaledSprite& ScaledSprite::operator=( const ScaledSprite& rhs) {
	setName( rhs.getName() );
	setPosition( rhs.getPosition() );
	setVelocity( rhs.getVelocity() );
	scale = rhs.scale; 
	scaledSurface = rhs.scaledSurface;
	frame = rhs.frame;
	frameWidth = rhs.frameWidth;
	frameHeight = rhs.frameHeight;
	worldWidth = rhs.worldWidth;
	worldHeight = rhs.worldHeight;
	return *this;
}

ScaledSprite::~ScaledSprite() {
	SDL_FreeSurface( scaledSurface );
	delete frame;
}	

void ScaledSprite::draw() const {
	Uint32 x = static_cast<Uint32>( X() );
	Uint32 y = static_cast<Uint32>( Y() );
	frame->draw(x, y);
}

void ScaledSprite::update( Uint32 ticks ) {
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight ) {
    Vector2f resetPosition(X(), 0);
    setPosition(resetPosition);
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    Vector2f resetPosition(0, Y());
    setPosition(resetPosition);
  }  
}
  
