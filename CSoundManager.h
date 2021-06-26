#ifndef CSOUNDMANAGER_H_
#define CSOUNDMANAGER_H_

#include <oslib/oslib.h>
#include "def.h"

class CSoundManager
{
//==============================================================================

  public:
  
  // Accessor to the singleton
  static CSoundManager& instance(void);

  // Destructor
  virtual ~CSoundManager(void);
  
  // Play a sound effect
  void playSound(const T_SOUND_EVENT p_sound);

  // Pause/resume the bg music
  void pauseMusic(void);

  // Play background music or not
  static bool m_bgMusic;
  
//==============================================================================

  private:
  
  // Forbidden
  CSoundManager(void);
  CSoundManager(const CSoundManager &p_source);
  const CSoundManager &operator=(const CSoundManager &p_right);
  
  // Sounds
  OSL_SOUND * m_step;
  OSL_SOUND * m_jump;
  
  // Background music
  OSL_SOUND * m_music;

//==============================================================================
};

#endif
