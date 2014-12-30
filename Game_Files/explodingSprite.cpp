#include <iostream>
#include <cmath>
#include "explodingSprite.h"
#include "vector2f.h"


ExplodingSprite::ExplodingSprite(const Sprite& s) :
  Sprite(s), 
  chunks(), 
  freeList(),
  frames(),
  chunkListPtr( &chunks ),
  freeListPtr( &freeList ) 
{
  makeChunks(Gamedata::getInstance().getXmlInt(s.getName()+"/chunk/size"));
}

ExplodingSprite::ExplodingSprite(const std::string& name, const Vector2f& pos, const Vector2f& vel) :
	Sprite(name, pos, vel),
	chunks(),
	freeList(),
	frames(),
	chunkListPtr( &chunks ),
	freeListPtr( &freeList ) 
	{
		makeChunks(Gamedata::getInstance().getXmlInt(name+"/chunk/size"));
	}
	
ExplodingSprite::ExplodingSprite(const std::string& name, const Vector2f& pos, const Vector2f& vel, const Frame* frm) :
	Sprite(name, pos, vel, frm),
	chunks(),
	freeList(),
	frames(),
	chunkListPtr(&chunks),
	freeListPtr(&freeList) 
	{
		makeChunks(Gamedata::getInstance().getXmlInt(name+"/chunk/size"));
	}
	
ExplodingSprite::ExplodingSprite( const ExplodingSprite&s ) :
	Sprite(s),
	chunks(),
	freeList(s.freeList),
	frames(),
	chunkListPtr( s.chunkListPtr),
	freeListPtr( s.freeListPtr) 
	{ 
		makeChunks(Gamedata::getInstance().getXmlInt(s.getName()+"/chunk/size"));	
	}
	
ExplodingSprite::~ExplodingSprite() { 
  for ( unsigned int i = 0; i < frames.size(); ++i ) {
    delete frames[i]; // ExplodingSprite made them, so it deletes them
  }
  chunks.clear();     // clearing a vector is a waste of time, but ...
  freeList.clear();   // still ...
}

void ExplodingSprite::draw() const { 
  // Override draw; use the draw in Chunk, which derives from Sprite.
  // So the draw we're using is actually in Sprite
  
 // std::cout << "Chunk List size: " << chunkCount() << std::endl;
  std::list<Chunk>::const_iterator ptr = chunkListPtr->begin();
/*  
  std::cout << "Chunk List size: " << chunks.size() << std::endl;
  std::cout << "Chunk List Pointer: " << chunkListPtr << std::endl;
  std::cout << "Chunk List Pointer: " << &chunks << std::endl;   
  
  std::cout << "Free List Pointer: " << freeListPtr << std::endl;
  std::cout << "Free List Pointer: " << &freeList << std::endl; */
  
  while ( ptr != chunkListPtr->end() ) {
	ptr->draw();
   	++ptr;
 }
}

void ExplodingSprite::update(Uint32 ticks) { 
  std::list<Chunk>::iterator ptr = chunkListPtr->begin();
  while (ptr != chunkListPtr->end()) {
    ptr->update(ticks);
    if (ptr->goneTooFar()) {  // Check to see if we should free a chunk
      freeListPtr->push_back(*ptr);
      ptr = chunkListPtr->erase(ptr);
    }
    else ++ptr;
  }
}

void ExplodingSprite::makeChunks(unsigned int n) { 
  // Break the SDL_Surface into n*n squares; where each square
  // has width and height of frameWidth/n and frameHeight/n
  unsigned int chunk_width = std::max(1u, getFrame()->getWidth()/n);
  unsigned int chunk_height = std::max(1u, getFrame()->getHeight()/n);
  int speedx = static_cast<int>(velocityX()); // Wanna test for zero...
  int speedy = static_cast<int>(velocityY()); // Make sure it's an int.
  if (speedx == 0) speedx = 1; // Make sure it's not 0;
  if (speedy == 0) speedy = 1; // Make sure it's not 0;

  // Get the SDL_Surface so we can chunk it:
  SDL_Surface* spriteSurface(getFrame()->getSurface()); 
  Sint16 source_y = getFrame()->getSourceY();
  while  (source_y < getFrame()->getHeight() ) {
    Sint16 source_x = getFrame()->getSourceX();
    while ( source_x < getFrame()->getWidth() ) {
      // Give each chunk it's own speed/direction:
      float sx = (rand() % speedx + 40) * (rand()%2?-1:1); // 'cause %0 is 
      float sy = (rand() % speedy + 40) * (rand()%2?-1:1); // float except
      Frame* frame = 
            new Frame(spriteSurface, chunk_width, chunk_height,
                  source_x, source_y
                );
      Chunk chunk(
                Vector2f(X()+source_x,   // x coord of destination
                         Y()+source_y), // y coord of destination
                Vector2f(sx, sy),
                getName()+"/chunk",
                frame);
      // chunks uses value semantics, as does frames, but there's
      // a big difference:
      chunks.push_back(chunk);
      frames.push_back(frame);
      source_x += chunk_width;
    }
    source_y += chunk_height;
  }

}

// This function resets the position and the velocity of all the chunks
// The real reason, pointers to the lists have been declared is to reset the position and swap
// the pointers to the list. 

void ExplodingSprite::resetChunks(const Drawable* d) {
	if ( freeListPtr->empty() )  { return; }
	else {
		std::list<Chunk>::iterator fItr = freeListPtr->begin();
		std::vector<Frame *>::iterator frameItr = frames.begin();
		// Update the positions of all the chunks in the freeList
		for (; ( (fItr != freeListPtr->end() ) && (frameItr != frames.end() ) ) ; ++fItr, ++frameItr ) {
			float posX = d->X() + (*frameItr)->getSourceX();
			float posY = d->Y() + (*frameItr)->getSourceY();
			Vector2f resetChunk(posX, posY);		
			fItr->reset( resetChunk );
		}	
	}
	std::cout << "In heere " << std::endl;
	std::list<Chunk> *tmp = freeListPtr;
	freeListPtr = chunkListPtr;
	chunkListPtr = tmp;
}

void ExplodingSprite::resetChunks() {
	if ( freeListPtr->empty() )  { return; }
	else {
		std::list<Chunk>::iterator fItr = freeListPtr->begin();
		std::vector<Frame *>::iterator frameItr = frames.begin();
		// Update the positions of all the chunks in the freeList
		for (; ( (fItr != freeListPtr->end() ) && (frameItr != frames.end() ) ) ; ++fItr, ++frameItr ) {
			float posX = X() + (*frameItr)->getSourceX();
			float posY = Y() + (*frameItr)->getSourceY();
			Vector2f resetChunk(posX, posY);		
			fItr->reset( resetChunk );
		}	
	}
	std::cout << "In heere " << std::endl;
 	std::list<Chunk> *tmp = freeListPtr;
	freeListPtr = chunkListPtr;
	chunkListPtr = tmp;
}
