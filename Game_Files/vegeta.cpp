#include "vegeta.h"
#include "frameFactory.h"
#include "frieza.h"

Vegeta::Vegeta(const std::string& n) :
	Player(n),
	frameX( FrameFactory::getInstance().getFrames( Gamedata::getInstance().getXmlStr(n+"/right")) ),
	frameUP( FrameFactory::getInstance().getFrames( Gamedata::getInstance().getXmlStr(n+"/up") ) ),
	frameDOWN( FrameFactory::getInstance().getFrames(Gamedata::getInstance().getXmlStr(n+"/down") ) ),
	frameDASH( FrameFactory::getInstance().getFrames(Gamedata::getInstance().getXmlStr(n+"/dash") ) ),
	dashKey(false),
	kioskMode(false),
	playerList()	
{ }

Vegeta::Vegeta(const Vegeta& v) :
	Player(v),
	frameX( v.frameX ),
	frameUP( v.frameUP ),
	frameDOWN(v.frameDOWN),
	frameDASH(v.frameDASH),
	dashKey(v.dashKey),
	kioskMode(v.kioskMode),
	playerList(v.playerList)
	{ }


void Vegeta::moveRight() {
	keyPressedX = true;
	Xdirection = true;
	if ( X() > 0 ) {
		velocityX( 250 );
	}
	curMovement = frameX;
}

void Vegeta::moveLeft() {
	keyPressedX = true;
	Xdirection = false;
	if ( X() > 0 ) {
		velocityX( -250 );
	}
	curMovement = frameX;
}

void Vegeta::moveUp() {
	keyPressedY = true;
	if ( Y() > 0 ) {
		velocityY(-200);
	}
	curMovement = frameUP;
}

void Vegeta::moveDown() {
	keyPressedY = true;
	if ( Y() > 0 ) {
		velocityY(200);
	}
	curMovement = frameDOWN;
}

void Vegeta::dash(int d) {
	keyPressedX = true;
	dashKey = true;
	if ( X() > 0 ) {
		if (d) {
			Xdirection = true;
			velocityX( 500 );
		}
		else {
			Xdirection = false;
			velocityX( -500 );
		}
	}
	curMovement = frameDASH;
}

void Vegeta::draw() const {
Uint32 x = static_cast<Uint32>(X());
Uint32 y = static_cast<Uint32>(Y());
  if (Xdirection)
  	curMovement[currentFrame]->draw(x, y);
  else 
  	curMovement[currentFrame]->drawleft(x, y);
  bullets.draw();
  healthBar.draw();  	
}

void Vegeta::update(Uint32 ticks) {
	advanceFrame(ticks);
	Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
	Vector2f prevPos = getPosition();
	Vector2f curPos = getPosition() + incr;
	setPosition(getPosition() + incr);
	if ( prevPos != curPos ) {
		notify();
	}
	
	Vector2f ref;
	
	if ( X() < 9000 && kioskMode) {
		X(9000)	;
		velocityX( abs( velocityX() ) );
	}
	
	if ( X() > 9000 ) {
		kioskMode = true;
	}
		
	if ( Y() < 0) {
		velocityY( abs( velocityY() ) );
  	}
  	if ( Y() > worldHeight-frameHeight) {
  		Y(worldHeight-frameHeight);
		velocityY( -abs( velocityY() ) );
  	}

	if ( X() < 0) {
		X(0);
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
    	dashKey = false;    	
    	blastKey = false;
    	bullets.update(ticks);
    	healthBar.update(ticks);

}  	  	

void Vegeta::blast() {

	stop();

	blastKey = true;
	curMovement = frameBLAST;

	float y = Y() + 29.0;
	if ( Xdirection ) {
		float x = X() + getFrame()->getWidth();
 		bullets.shoot( Vector2f(x,y), Vector2f(abs(minSpeed+velocityX()), velocityY()) );
 	}
 	else {
		float x = X() - ( getFrame()->getWidth() ) / 2;
 		bullets.shoot( Vector2f(x,y), Vector2f( ( (-1.5 ) * ( abs( -minSpeed - velocityX() ) ) ), velocityY()) );
 	}
 	
}	

void Vegeta::notify() const {
		std::list<Player *>::const_iterator pItr = playerList.begin();
		while ( pItr != playerList.end() ) {
			if ( dynamic_cast<Frieza*>(*pItr) ) {
				static_cast<Frieza*>(*pItr++)->UpdatePos( getPosition() );
			}
		}
}
