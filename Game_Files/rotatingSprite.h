#ifndef ROTATINGSPRITE__H
#define ROTATINGSPRITE__H
#include <iostream>
#include "drawable.h"

class RotateScaledSprite : public Drawable {
public:
	RotateScaledSprite( const std::string&, SDL_Surface*, const int x );
	RotateScaledSprite( const RotateScaledSprite& );
	~RotateScaledSprite() { 	
		SDL_FreeSurface( scaledSurface );
		delete frame;		
	}
	
	const RotateScaledSprite& operator=( const RotateScaledSprite& );
	bool operator<(const RotateScaledSprite& rhs ) const {
		return scale < rhs.scale;
	}

	virtual const Frame* getFrame() const  { return frame; } 
	virtual void draw() const;
	virtual void update( Uint32 ticks );
	
	float getScale() const { return scale; }
	
	Vector2f getCenter() const {
		return Vector2f( ( X() + frame->getWidth()/2 ), ( Y() + frame->getHeight()/2 ) );
	}

private:
	double scale;
	SDL_Surface* scaledSurface;
	const Frame* frame;
	
	int frameWidth;
	int frameHeight;
	int worldWidth;
	int worldHeight;
	int angle;
};

#endif
