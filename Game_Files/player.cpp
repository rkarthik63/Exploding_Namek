#include "player.h"
#include "gamedata.h"
#include "frameFactory.h"

Player::Player(const std::string& name ) :
	Drawable(name, Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
		 Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"), Gamedata::getInstance().getXmlInt(name+"/speedY") ) ),
	frameSTAND( FrameFactory::getInstance().getFrames( Gamedata::getInstance().getXmlStr(name+"/stand") ) ),
	frameBLAST( FrameFactory::getInstance().getFrames(Gamedata::getInstance().getXmlStr(name+"/blast") ) ),	
	curMovement(frameSTAND),
	worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
	worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
	currentFrame(0),
	numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
	frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
	timeSinceLastFrame( 0 ),
	frameWidth( frameSTAND[0]->getWidth() ),
	frameHeight(frameSTAND[0]->getHeight() ),
	Xdirection(true),
	Ydirection(true),
	keyPressedX(false),
	keyPressedY(false),
	blastKey(false),
	slowFactor(0.95),
	bulletName( Gamedata::getInstance().getXmlStr(name+"/Bullet") ),
	bullets( bulletName ),
	minSpeed( Gamedata::getInstance().getXmlInt(bulletName+"/speedX") ),
	healthBar( Gamedata::getInstance().getXmlStr(name+"/healthhud")),
	strategy(new PerPixelCollisionStrategy)
 {}
 
Player::Player(const Player& s) :
	Drawable(s),
	frameSTAND( s.frameSTAND ),
	frameBLAST( s.frameBLAST ),	
	curMovement( s.curMovement ),
	worldWidth( s.worldWidth ),
	worldHeight( s.worldHeight ),
	currentFrame( s.currentFrame ),
	numberOfFrames( s.numberOfFrames ),
	frameInterval( s.frameInterval ),
	timeSinceLastFrame( s.timeSinceLastFrame ),
	frameWidth( s.frameWidth),
	frameHeight( s.frameHeight ),
	Xdirection( s.Xdirection ),
	Ydirection( s.Ydirection ),
	keyPressedX( s.keyPressedX ),
	keyPressedY( s.keyPressedY),
	blastKey(s.blastKey),
	slowFactor(s.slowFactor),
	bulletName(s.bulletName),
	bullets(s.bullets),
	minSpeed(s.minSpeed),
	healthBar(s.healthBar),
	strategy(new PerPixelCollisionStrategy)
 { }
 
void Player::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
		currentFrame = (currentFrame+1) % (numberOfFrames );
		timeSinceLastFrame = 0;
	}
}


void Player::stop() {
	if (!keyPressedX) velocityX(slowFactor * velocityX() );
	if (!keyPressedY) velocityY(0);	
	curMovement = frameSTAND;
}

