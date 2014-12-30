#include <vector>
#include <list>
#include "chunk.h"

class ExplodingSprite : public Sprite {
public:
  ExplodingSprite(const Sprite& s);
  ExplodingSprite(const std::string& name, const Vector2f& pos, const Vector2f& vel);
  ExplodingSprite(const std::string& name, const Vector2f& pos, const Vector2f& vel, const Frame* frm);
  virtual ~ExplodingSprite();
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  void makeChunks(unsigned int);
  unsigned int chunkCount() const { return chunkListPtr->size(); }
  unsigned int freeCount()  const { return freeListPtr->size(); }
  bool isChunkEmpty() const {
  	return ( chunkListPtr->empty() );
  }
  
  // For resetting the chunkList and the freeList
  void resetChunks(const Drawable* d);
  void resetChunks();
  
  ExplodingSprite(const ExplodingSprite&); // Explicit disallow (Item 6)
 

  
private:
  std::list<Chunk> chunks; // An ExplodingSprite is a list of sprite chunks
  std::list<Chunk> freeList; // When a chunk gets out of range it goes here
  std::vector<Frame*> frames; // Each chunk has a Frame
  
  std::list<Chunk> *chunkListPtr;		// Points to the chunk List
  std::list<Chunk> *freeListPtr;		// Points to the free List
 
  ExplodingSprite& operator=(const ExplodingSprite&); // (Item 6) 
};
