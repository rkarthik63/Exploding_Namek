#include <iostream>
#include <string>
#include <iomanip>
#include "multisprite.h"
#include "sprite.h"
#include "rotatingSprite.h"
#include "gamedata.h"
#include "manager.h"

Manager::~Manager() { 
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind. 
  delete spriteLord;
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  HUD_HEIGHT(140),
  HUD_WIDTH(350),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  viewport( Viewport::getInstance() ),
  spriteLord( new SpriteManager() ),
  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  HUDkey(true),
  timer(0),
  sound(SDLSound::getInstance())
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);
  viewport.setObjectToTrack(spriteLord->getSpriteCurrentSprite());
}

void Manager::drawHUD(SDL_Surface* screen, int x, int y) const {
  const Uint32 BLUE = SDL_MapRGB(screen->format, 0x00, 0x00, 0xff);
  Draw_AALine(screen, x, y+HUD_HEIGHT/2, x+HUD_WIDTH,y+HUD_HEIGHT/2, HUD_HEIGHT, 0xff, 0xff, 0xff, 0xff/2);
  Draw_AALine(screen, x,y, x+HUD_WIDTH,y, BLUE);
  Draw_AALine(screen, x,y, x,y+HUD_HEIGHT, BLUE);
    
  io.printMessageAt("DRAGON BALL Z", 85, 85);  
  io.printMessageValueAt("Seconds: ", clock.getSeconds(), 15, 125);
  io.printMessageValueAt("fps: ", clock.getAvgFps(), 15, 140);
  io.printMessageAt("Press T to switch sprites", 15, 155); 
  
  spriteLord->drawBulletHUD();
}


void Manager::draw() const {
  spriteLord->drawSprites();
 
  viewport.draw();
  io.printMessageAt("Press F1 to toggle HUD", 45, 60);
  if (HUDkey) drawHUD(screen, 5, 80);
  io.printMessageAt(title, 30, 450); 
  if (spriteLord->isGodModeOn() ) {
  	io.printMessageAt("GOD MODE ON", 370, 50);  
  }
  SDL_Flip(screen);
}


void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}

void Manager::update() {
  ++clock;
  Uint32 ticks = clock.getElapsedTicks();
  spriteLord->updateSprites(ticks);
  
  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }
  viewport.update(); // always update viewport last
  if ( (timer = clock.getSeconds() ) == 3 ) {
  	HUDkey = false;
  }
}

void Manager::play() {
  SDL_Event event;
  
  bool done = false;
  bool keyCatch = false; 
  
  spriteLord->setCheckVegeta();
   
  while ( not done ) {
  
    if ( ( spriteLord->checkVegeta()) ) {
		spriteLord->vegetaResetPosition();
      		spriteLord->resetCheckVegeta();
		spriteLord->deleteMeteors();
		spriteLord->makeMeteor();
		sound.startMusic();		
       		done = true;
       		break;
    }  	
    SDL_PollEvent(&event);
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if (event.type ==  SDL_QUIT) { done = true; break; }
    if(event.type == SDL_KEYUP) { 
      keyCatch = false; 
    }
    if(event.type == SDL_KEYDOWN) {
    
      if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
        done = true;
        break;    
      }            
      if ( keystate[SDLK_r] ) {
      		spriteLord->vegetaResetPosition();
      		spriteLord->resetCheckVegeta();
		spriteLord->deleteMeteors();
		spriteLord->makeMeteor();
		sound.startMusic();		
      }

      if (keystate[SDLK_RIGHT] && keystate[SDLK_LEFT]) {
      		spriteLord->vegetaStop();      
      }
      
      else if (keystate[SDLK_RIGHT] && keystate[SDLK_SPACE] ) {
      	spriteLord->vegetaStop();      
      }
      else if (keystate[SDLK_LEFT] && keystate[SDLK_SPACE] ) {
      	spriteLord->vegetaStop();      
      }     
      
      else if(keystate[SDLK_RIGHT]) {
   		
      		if (keystate[SDLK_z]) {
			spriteLord->vegetaDash(1);
		}	
		else {
			spriteLord->vegetaRight();
		}
      }
      
      else if(keystate[SDLK_LEFT]) {
		if (keystate[SDLK_z]) {
			spriteLord->vegetaDash(0);
		}	
		else {
			spriteLord->vegetaLeft();
		}
      }
      
      if (keystate[SDLK_UP] && keystate[SDLK_DOWN]) {
      		spriteLord->vegetaStop();      
      }
      
      else if (keystate[SDLK_UP] && keystate[SDLK_SPACE] ) {
      	spriteLord->vegetaStop();      
      }
      else if (keystate[SDLK_DOWN] && keystate[SDLK_SPACE] ) {
      	spriteLord->vegetaStop();      
      }
              
      else if(keystate[SDLK_DOWN]) {
       	       spriteLord->vegetaDown();
      }
      
      else if(keystate[SDLK_UP]) {
       	       spriteLord->vegetaUp();
      }
      
      if (  keystate[SDLK_SPACE] && keystate[SDLK_RIGHT] && keystate[SDLK_LEFT] ) {
      	spriteLord->vegetaStop();
      }
      
      else if (  keystate[SDLK_SPACE]  && keystate[SDLK_LEFT] ) {
      	spriteLord->vegetaStop();
      }
      
      else if (  keystate[SDLK_SPACE] && keystate[SDLK_RIGHT] ) {
      	spriteLord->vegetaStop();
      }
      
      else if (  keystate[SDLK_SPACE]  && keystate[SDLK_DOWN]) {
      	spriteLord->vegetaStop();
      }
      
      else if (  keystate[SDLK_SPACE]  && keystate[SDLK_UP]) {
      	spriteLord->vegetaStop();
      }

      else if (keystate[SDLK_SPACE]){
       	       spriteLord->vegetaBlast();       	 
//       	       int n = event.key.keysym.sym - SDLK_0;
       	       sound[0];
      }  
      
      if ( keystate[SDLK_g] && !keyCatch) {
      		keyCatch = true;
      		spriteLord->vegetaGodMode();	
      }
        
      if ( keystate[SDLK_F1] && !keyCatch) {
       	keyCatch = true;
       	HUDkey = (HUDkey) ? false : true;
      }
        
      if (keystate[SDLK_t] && !keyCatch) {
        keyCatch = true;
        spriteLord->trackNextSprite();
        viewport.setObjectToTrack(spriteLord->getSpriteCurrentSprite());
      }
      if (keystate[SDLK_s] && !keyCatch) {
        keyCatch = true;
        clock.toggleSloMo();
      }
        if (keystate[SDLK_p] && !keyCatch) {
          keyCatch = true;
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
    }
    draw();
    update();
   }
 }
