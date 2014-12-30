// spriteManager is a singleton
// Manages the sprites and the world's for the animation

#include <vector>
#include <list>
#include <SDL.h>
#include "ioManager.h"
#include "viewport.h"
#include "multisprite.h"
#include "sprite.h"
#include "rotatingSprite.h"
#include "world.h"
#include "player.h"
#include "scaledSprite.h"
#include "explodingSprite.h"
#include "vector2f.h"
#include "ioManager.h"
#include "vegeta.h"
#include "frieza.h"

class SpriteManager
{
	public:
//		static SpriteManager& getInstance();
		SpriteManager ();
		~SpriteManager (); 
		const Drawable* getSpriteCurrentSprite() const;	// Returns the currentsprite from the vector
		int getCurrentSprite() const;
		void drawSprites() const;			// Draw the sprites in the vector 
		void updateSprites(int ticks);			// update the sprites in the vector 
		void trackNextSprite();				// Tracks the next sprite 	
		
		// Vegeta Movement
		void vegetaStop();
		void vegetaLeft();
		void vegetaRight();
		void vegetaUp();
		void vegetaDown();
		void vegetaDash(int d);
		void vegetaBlast();
		void vegetaResetPosition();
		
		// Players' Bullet Collision Check
		void playersBulletCollisionCheck();
		
		
		void vegetaGodMode() {
			godMode = (godMode) ? false: true;
		}
		bool isGodModeOn() const {
			return godMode;
		}				
		// Check for Vegeta collisions
		bool checkForVegetaCollisions() const;
		// Check for Bullet collisions and explode the meteors		
		void checkForBulletMeteorCollisions();
		// Decrement the Health
		void vegetaHealthDecrement() {
			static_cast<Vegeta* >(sprites[0])->decrementHealthBar();
		}
		bool vegetaHealthIsZero() {
			return	static_cast<Vegeta* >(sprites[0])->isHealthEmpty();
		}		
		// Bullet HUD
		void drawBulletHUD() {
			static_cast<Vegeta* >(sprites[0])->drawHUD();
		}
		bool checkVegeta() const {
			return vegetaLost;
		}
		void setCheckVegeta() {
			vegetaLost = false;
		}
		void resetCheckVegeta() {
			vegetaLost = true;
		}
		
		void makeMeteor();
		void deleteMeteors();
	private:
		World bg1;
		World bg2;
		World fg1;
		World fg2;	
		std::vector<Drawable *> sprites;
		int currentSprite;
		
		SDL_Surface* meteorSurface;
		std::list<RotateScaledSprite *> meteor;
		
		bool vegetaExploded;
		bool friezaExploded;
		bool meteorExploded;

		// Exploding Vegeta
		ExplodingSprite* explodeVegetaPtr;
		ExplodingSprite* explodeFriezaPtr;		
		ExplodingSprite* explodeMeteorPtr;
		
		int meteorDensity;
		// God Mode
		bool godMode;
		bool vegetaLost;
		

		SpriteManager (const SpriteManager&); 
		const SpriteManager& operator=(const SpriteManager&);
						
		// Private Functions


		// explode the Vegeta
		void explodeVegetaNow();
		void explodeFriezaNow();

};
