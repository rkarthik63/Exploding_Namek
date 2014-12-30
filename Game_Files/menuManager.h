#include <SDL.h>
#include "menu.h"
#include "clock.h"
#include "manager.h"

class MenuManager {
public:
  MenuManager ();
  void play();

private:
  bool env;
  SDL_Surface *screen;
  const Clock& clock;
  SDL_Color backColor;
  
  Menu menu;
  bool drawControl;

  void drawBackground() const;
  MenuManager(const MenuManager&);
  MenuManager& operator=(const MenuManager&);
};
