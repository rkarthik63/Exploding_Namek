#include <iostream>
#include <cstdlib>
#include <cmath>
#include "SDL/SDL_rotozoom.h"
#include "rotatingSprite.h"
#include "gamedata.h"
#include "drawable.h"

RotateScaledSprite::RotateScaledSprite(const std::string& name, SDL_Surface* surface, const int x) :
	Drawable(name,
		 Vector2f( (Gamedata::getInstance().getXmlInt(name+"/startLoc/x") + (x * ( static_cast<float>(Gamedata::getInstance().getXmlInt("world/width")) / (Gamedata::getInstance().getXmlInt("meteor/meteorDensity")) ) ) ), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
                 Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"), 
                    Gamedata::getInstance().getXmlInt(name+"/speedY") )
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
	worldHeight( Gamedata::getInstance().getXmlInt("world/height") ),
	angle(5)
{ 
	velocityY( velocityY() * scale);
}
	                

RotateScaledSprite::RotateScaledSprite( const RotateScaledSprite& s ) :
	Drawable(s.getName(), s.getPosition(), s.getVelocity() ),
	scale(s.scale), 
	scaledSurface(s.scaledSurface), 
	frame(s.frame),
	frameWidth(s.frameWidth),
	frameHeight(s.frameHeight),
	worldWidth(s.worldWidth),
	worldHeight(s.worldHeight),
	angle(s.angle)
{ }
	 
const RotateScaledSprite& RotateScaledSprite::operator=( const RotateScaledSprite& rhs) {
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
	angle = rhs.angle;
	return *this;
}

void RotateScaledSprite::draw() const {
	Uint32 x = static_cast<Uint32>( X() );
	Uint32 y = static_cast<Uint32>( Y() );
	frame->draw(x, y);
}

void RotateScaledSprite::update( Uint32 ticks ) {
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  
  angle += 5;

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
  
