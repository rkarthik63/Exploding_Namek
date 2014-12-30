#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include "menuManager.h"
#include "ioManager.h"


MenuManager::MenuManager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  screen( IOManager::getInstance().getScreen() ),
  clock( Clock::getInstance() ),
  backColor(),
  menu(),
  drawControl(false)
{ 
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  backColor.r = Gamedata::getInstance().getXmlInt("back/red");
  backColor.g = Gamedata::getInstance().getXmlInt("back/green");
  backColor.b = Gamedata::getInstance().getXmlInt("back/blue");
  atexit(SDL_Quit); 
}

void MenuManager::drawBackground() const {
  SDL_FillRect( screen, NULL, 
    SDL_MapRGB(screen->format, backColor.r, backColor.g, backColor.b) );
  SDL_Rect dest = {0, 0, 0, 0};
  SDL_BlitSurface( screen, NULL, screen, &dest );
}

void MenuManager::play() {
  bool keyCatch = false; // To get only 1 key per keydown
  SDL_Event event;
  bool done = false;
  // Here, we need to create an instance of the Manager,
  // the one that manages the game not the menu:
  Manager* manager = new Manager();
  
  manager->pause();

  while ( not done ) {

    drawBackground();
    
    if (drawControl) {
	    IOManager::getInstance().printMessageAt("UP arrow to move UP", 275, 90);  
	    IOManager::getInstance().printMessageAt("DOWN arrow to move DOWN", 275, 140);
	    IOManager::getInstance().printMessageAt("RIGHT arrow to move RIGHT", 275, 190);  
	    IOManager::getInstance().printMessageAt("LEFT arrow to move LEFT", 275, 240);
	    IOManager::getInstance().printMessageAt("HOLD Z to DASH while RIGHT or LEFT", 275, 290);
   	    IOManager::getInstance().printMessageAt("G for God Mode", 275, 340);
	    IOManager::getInstance().printMessageAt("R for Reset", 275, 390);
    	    IOManager::getInstance().printMessageAt("b to return to Main Menu", 275, 440);
    }
    else {
    	menu.draw();
    }
    
    SDL_Flip(screen);

    SDL_PollEvent(&event);
    if (event.type ==  SDL_QUIT) { break; }
    if(event.type == SDL_KEYDOWN) {
      switch ( event.key.keysym.sym ) {
        case SDLK_b :
        	drawControl = false;
        	break;      	
        case SDLK_ESCAPE :
        case SDLK_q : {
          done = true;
          break;
        }
        case SDLK_RETURN : {
          if ( !keyCatch ) {
            menu.lightOn();
            if ( menu.getIconClicked() == "Start Game" ) {
              // Here is where we call the play() function in Manager;
              // but first, unpause the game:
              manager->unpause();
              manager->play();
              manager->pause();
            }
            if ( menu.getIconClicked() == "Help" ) {
              // Here is where we explain how to play the game"
              std::cout << "Give help  ..." << std::endl;
              drawControl = true;
            }
            if ( menu.getIconClicked() == "Exit" ) {
              drawBackground();
              menu.draw();
              SDL_Flip(screen);
              SDL_Delay(250);
              done = true;
            }
          }
          break;
        }
        case SDLK_DOWN   : {
          if ( !keyCatch ) {
            menu.increment();
          }
          break;
        }
        case SDLK_UP   : {
          if ( !keyCatch ) {
            menu.decrement();
          }
          break;
        }
        default          : break;
      }
      keyCatch = true;
    }
    if(event.type == SDL_KEYUP) { 
      keyCatch = false; 
      menu.lightOff();
    }
  }
  delete manager;
  manager = NULL;
}
