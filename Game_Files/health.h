#include "vector2f.h"
#include "ioManager.h"
#include "aaline.h"
#include <string>

class Health {
public:
  Health(const std::string& name);
  Health(int sx, int sy, int tl, int cl, 
         float t, int inc, Uint32 c, float sp);
  void draw() const;
  void drawReverse() const;
  void update(Uint32);
  void reset() { currentLength = totalLength; }
  void resetReverse() { currentLength = 0; }
  void decrement();
  void decrementMore();
  void decrementLittleMore();
  bool isEmpty() const {
  	return currentLength == 0;
  }
  bool isFull() const {
  	return (totalLength == currentLength);
  } 
  
private:
  SDL_Surface* screen;
  Vector2f start;
  int totalLength;
  int currentLength;
  int thick;
  int increments;
  float interval;
  int deltaTime;
  const Uint32 RED;
  const Uint32 GRAY;
  const Uint32 BLACK;
  const Uint32 color;
  void drawBox() const;
  //Health(const Health&);
  Health& operator=(const Health&);
};
