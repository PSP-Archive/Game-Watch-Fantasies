// The main library OSLib
#include <oslib/oslib.h>
#include "CGame.h"
#include "CSoundManager.h"
#include "CSaveManager.h"

// Callbacks
PSP_MODULE_INFO("GWF", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

// Gravity
u8 CMario::m_gravity = GRAVITY_EARTH;

// Background music
bool CSoundManager::m_bgMusic = true;

int main()
{
  // Init OSLib
  oslInit(0);
  // Init graphic mode
  oslInitGfx(OSL_PF_8888, 1);
  // Init audio
  oslInitAudio();
  // Set alpha color
  oslSetTransparentColor(RGB(255,0,255));
  oslSetBkColor(RGB(255,0,255));
  
  // Implement the save singleton
  CSaveManager::instance();
  // Implement the sound singleton
  CSoundManager::instance();
  
  // Main game object
  CGame l_game;
  
  // Main loop
  while (!osl_quit)
  {
    oslReadKeys();
    oslStartDrawing();
    // Play the game
    l_game.play();
    // Draw the game
    l_game.draw();
    oslEndDrawing();
    // Wait for frame
    oslSyncFrame();
    oslAudioVSync();
  }
  
  // Quit the program
  oslEndGfx();
  oslQuit();
  return 0;
}
