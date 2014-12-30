#include <iostream>
#include "drawable.h"

class ScaledSprite : public Drawable {
public:
	ScaledSprite( const std::string&, SDL_Surface*, const int x );
	ScaledSprite( const ScaledSprite& );
	~ScaledSprite();
	
	const ScaledSprite& operator=( const ScaledSprite& );
	bool operator<(const ScaledSprite& rhs ) const {
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
};

