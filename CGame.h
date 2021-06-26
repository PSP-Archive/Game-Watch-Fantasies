#ifndef CGAME_H_
#define CGAME_H_

#include <oslib/oslib.h>
#include "CMario.h"
#include "CDigitDrawer.h"
#include "CMenu.h"

class CGame
{
//==============================================================================

  public:
  
  // Constructor
  CGame(void);

  // Destructor
  virtual ~CGame(void);
  
  // Load next level
  void goToNextLevel(const bool p_save = true);
  
  // Main play method, called at each frame
  void play(void);
  
  // Draw everything, at each frame
  void draw(void);
  
//==============================================================================

  private:
  
  // Copy constructor and operator '=' forbidden
  CGame(const CGame &p_source);
  const CGame &operator=(const CGame &p_right);
  
  // Build levels with LCD digits
  void buildLevel(void);

  // Draw LCD ghosts if necessary
  void drawLCDGhosts(void);
  
  // Mario sprite
  CMario m_mario;
  
  // Game state
  T_GAME_STATE m_state;
  
  // Background image
  OSL_IMAGE * m_background;

  // Platforms image
  OSL_IMAGE * m_platforms;
  
  // Save copy of the platform image, when we have to draw on it
  OSL_IMAGE * m_platformsSave;

  // Level number
  u8 m_level;
  
  // For drawing the digital numbers on screen
  CDigitDrawer m_digitDrawer;
  
  // For drawing title and end screen
  CMenu m_menu;

//==============================================================================
};

#endif
