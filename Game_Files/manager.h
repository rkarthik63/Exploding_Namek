#include <vector>
#include <SDL.h>
#include "ioManager.h"
#include "clock.h"
#include "viewport.h"
#include "spriteManager.h"
#include "aaline.h"
#include "sound.h"

class Manager {
public:
  Manager ();
  ~Manager ();
  void pause() { 
  	clock.pause(); 
  	sound.toggleMusic();
  }
  void unpause() { 
  	clock.unpause();
  	sound.toggleMusic();
  }

  void play();
  
private:
  const bool env;
  const int HUD_HEIGHT;
  const int HUD_WIDTH;
  const IOManager& io;				// Singleton
  Clock& clock;					// Singleton

  SDL_Surface * const screen;			// Screen
  Viewport& viewport;				// Singleton
  SpriteManager* spriteLord;			// Lord of the Sprites (Meyer's Singleton)

  bool makeVideo;
  int frameCount;
  const std::string username;
  const std::string title;
  const int frameMax;
  bool HUDkey;
  int timer;
  SDLSound& sound;

  void drawHUD(SDL_Surface* screen, int x, int y) const;
  void draw() const;
  void update();
  
  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
};
