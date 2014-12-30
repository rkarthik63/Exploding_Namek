#include "frieza.h"
#include "frameFactory.h"
#include "cmath"
#include "gamedata.h"

// Functor to calculate the direction
class CalculateDirection {
public:
	CalculateDirection() : radius(1000), xDiff(0), yDiff(0) {}
	float getRadius() const {
		return radius;
	}
	float operator()(const Vector2f& A, const Vector2f& B) {
		float angle;
		if ( (radius = calculateRadius(A, B) ) != 0 ) {
			angle = acos( xDiff / radius ) * 180 / M_PI;
		}	
		return angle;
	}
private:
	float radius;
	float xDiff;
	float yDiff;
	// Calculates the radius
	float calculateRadius(const Vector2f& A, const Vector2f& B) {
		xDiff = B[0] - A[0];		// Calculates x2 - x1, x2 is of Frieza
		yDiff = A[1] - B[1];		
		return hypot(xDiff, yDiff);
	}
}; 

Frieza::Frieza(const std::string& n, Vegeta* p) :
	Player(n),
	vegetaPos(),
	firingDistance( Gamedata::getInstance().getXmlInt( ( Gamedata::getInstance().getXmlStr(n+"/Bullet"))+"/distance")  ),
	shouldFriezaFire ( false ),
	distanceToVegeta(1000),
	angle(0),
	blastInterval(Gamedata::getInstance().getXmlInt( ( Gamedata::getInstance().getXmlStr(n+"/Bullet"))+"/interval")),
	timeSinceLastBlast(0),
	sound(SDLSound::getInstance() ),
	MODE(NORMAL)
	{ 
	 	Xdirection = false;
	 	p->attach(this);
	 }
 
Frieza::Frieza(const Frieza& f) :
	Player(f),
	vegetaPos(f.vegetaPos),
	firingDistance(f.firingDistance),
	shouldFriezaFire(f.shouldFriezaFire),
	distanceToVegeta(f.distanceToVegeta),
	angle(f.angle),
	blastInterval(f.blastInterval),
	timeSinceLastBlast(f.timeSinceLastBlast),
	sound(f.sound),
	MODE(f.MODE)
	{ }

void Frieza::draw() const {
Uint32 x = static_cast<Uint32>(X());
Uint32 y = static_cast<Uint32>(Y());
  if (Xdirection)
  	curMovement[currentFrame]->draw(x, y);
  else 
  	curMovement[currentFrame]->drawleft(x, y);
  bullets.draw();
  healthBar.drawReverse();
}
void Frieza::update(Uint32 ticks) {
	advanceFrame(ticks);
	timeSinceLastBlast += ticks;
	Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
	setPosition(getPosition() + incr);
	
	Vector2f ref;
	
	if ( Y() < 0) {
		velocityY( abs( velocityY() ) );
  	}
  	if ( Y() > worldHeight-frameHeight) {
  		Y(worldHeight-frameHeight);
		velocityY( -abs( velocityY() ) );
  	}

	if ( X() < 9500) {
		X(9500);
		velocityX( abs( velocityX() ) );
  	}
  
  	if ( X() > worldWidth-frameWidth) {
  		X(worldWidth-frameWidth);
    		velocityX( -abs( velocityX() ) );
    	}    	
    	
    	if ( !keyPressedX ) stop();
  	if ( !keyPressedY ) stop();
    	
    	keyPressedX = false;
    	keyPressedY = false;  	  
    	blastKey = false;
    	bullets.update(ticks);
    	healthBar.update(ticks);
    	reOrientFrieza();
    	
    	if ( shouldFriezaFire ) {
     		if ( timeSinceLastBlast > blastInterval ) {
    			blast();
    			if (timeSinceLastBlast > 1000) sound[1];
    		}    		
	}
	if ( timeSinceLastBlast > 1100 ) {
		timeSinceLastBlast = 0;
	}
	if ( MODE ) {
		Vector2f newPos( ((rand() % 400) + 9600) , (rand() % 480) + 20);
		setPosition(newPos);
		MODE = NORMAL;
	}	
}	

// Frieza Movement
void Frieza::moveRight() { 
	keyPressedX = true;
	Xdirection = true;
	if ( X() > 0 ) {
		velocityX( 250 );
	}
}
void Frieza::moveLeft() { 
	keyPressedX = true;
	Xdirection = false;
	if ( X() > 0 ) {
		velocityX( -250 );
	}
}
void Frieza::moveUp() { 
	keyPressedY = true;
	if ( Y() > 0 ) {
		velocityY(-200);
	}
}
void Frieza::moveDown() { 
	keyPressedY = true;
	if ( Y() > 0 ) {
		velocityY(200);
	}
}	

void Frieza::blast() {

	blastKey = true;
	curMovement = frameBLAST;

	float y = Y() + 29.0;	
	
	float Xcomp = abs ( minSpeed * cos(angle * M_PI / 180) );
	float Ycomp = abs ( minSpeed * sin(angle * M_PI / 180) );

	float vX = vegetaPos[0];
	float vY = vegetaPos[1];	
	
	Xcomp = ( vX < X() )? (-1 * abs(Xcomp) ) : abs(Xcomp);
	Ycomp = ( vY < Y() )? (-1 * abs(Ycomp) ) : abs(Ycomp);
	
	if ( Xdirection ) {
		float x = X() + getFrame()->getWidth();
		bullets.shoot( Vector2f(x,y), Vector2f(abs( Xcomp + velocityX() ), (velocityY() + Ycomp) ) );				
	}
	else {
		float x = X() - ( getFrame()->getWidth() ) / 2;
		bullets.shoot( Vector2f(x,y), Vector2f( ( -(abs( Xcomp - velocityX() ) ) ), (velocityY() + Ycomp) ) );
	}	
}

float Frieza::getDirection(const Vector2f& A, const Vector2f& B) {
	CalculateDirection directionFunctor;
	float angle = directionFunctor(A, B);
	distanceToVegeta = directionFunctor.getRadius();
	return angle;
}

void Frieza::reOrientFrieza() {
	float vX = vegetaPos[0];
	float fX = this->X();
	if ( (fX < vX) && (!Xdirection) ) {
		Xdirection = true;		
	}
	else if ( (fX > vX) && (Xdirection)) {
		Xdirection = false;
	}
}
