#ifndef CMARIO_H_
#define CMARIO_H_

#include <oslib/oslib.h>
#include "def.h"

class CMario
{
//==============================================================================

  public:
  
  // Constructor
  CMario(void);
  
  // Destructor
  virtual ~CMario(void);
  
  // Reset the sprite for a new level
  void reset(const u8 p_levelNumber);

  // Draw Mario
  void draw(void);
  
  // Move Mario when buttons are pressed
  void play(OSL_IMAGE * &p_level);
  
  // Return whether Mario has beaten the current level, or not
  const bool beatLevel(void) const;
  
  // Gravity
  static u8 m_gravity;

//==============================================================================
  
  private:
  
  // Copy constructor and operator '=' forbidden
  CMario(const CMario &p_source);
  const CMario &operator=(const CMario &p_right);

  // Move one step left, right or up (jump)
  void move(const T_DIRECTION p_direction, OSL_IMAGE * &p_level);

  // Check horizontal / vertical position of Mario when moving
  const bool xIsOk(const int p_x, OSL_IMAGE * &p_level) const;
  const bool yIsOk(const int p_y, OSL_IMAGE * &p_level) const;
  
  // Pad Mario against an obstacle to the right, left or up
  // return: true if Mario actually moved, false otherwise
  const bool pad(const T_DIRECTION p_direction, OSL_IMAGE * &p_level);
  
  // Adjust Mario's position after a move
  void adjustPosition(const T_DIRECTION p_direction, OSL_IMAGE * &p_level);
  
  // Go to next frame
  void nextFrame(void);
  
  // Spriteset
  OSL_IMAGE * m_sprite;
  
  // Frame index
  int m_frameIndex;
  
  // Mario state
  T_MARIO_STATE m_state;

  // Gravity timer
  u8 m_gravityTimer;
  
//==============================================================================
};

#endif
