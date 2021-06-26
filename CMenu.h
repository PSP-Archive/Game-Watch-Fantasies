#ifndef CMENU_H_
#define CMENU_H_

#include <oslib/oslib.h>
#include "def.h"

class CMenu
{
//==============================================================================

  public:
  
  // Constructor
  CMenu(void);
  
  // Destructor
  virtual ~CMenu(void);
  
  // Reset cursor to the first line
  void resetCursor(void);
  
  // Draw title screen
  void drawTitleScreen(void) const;

  // End screen
  void drawEndScreen(void) const;

  // Pause screen
  void drawPauseScreen(void) const;

  // Manage title screen
  const T_MENU_EVENT playTitleScreen(void);

  // Manage end screen
  const T_MENU_EVENT playEndScreen(void);

  // Manage pause screen
  const T_MENU_EVENT playPauseScreen(void);
  
//==============================================================================
  
  private:
  
  // Copy constructor and operator '=' forbidden
  CMenu(const CMenu &p_source);
  const CMenu &operator=(const CMenu &p_right);
  
  // Base window
  void drawBaseWindow(void) const;

  // Current position of the cursor
  u8 m_cursor;
  
//==============================================================================
};

#endif
