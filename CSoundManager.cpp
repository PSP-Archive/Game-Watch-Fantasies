#include "CSoundManager.h"

//==============================================================================

CSoundManager& CSoundManager::instance(void)
{
  // Singletons are cool
  static CSoundManager g_soundManager;
  return g_soundManager;
}

//==============================================================================

CSoundManager::CSoundManager(void) :
  m_step(NULL),
  m_jump(NULL),
  m_music(NULL)
{
  // Load all sound effects and stream the music
  m_step = oslLoadSoundFile("res/step.wav", OSL_FMT_NONE);
  if (m_step == NULL)
    oslFatalError("File res/step.wav is missing!");
  m_jump = oslLoadSoundFile("res/jump.wav", OSL_FMT_NONE);
  if (m_jump == NULL)
    oslFatalError("File res/jump.wav is missing!");
  m_music = oslLoadSoundFile("res/music.bgm", OSL_FMT_STREAM);
  if (m_music == NULL)
    oslFatalError("File res/music.bgm is missing!");
  oslSetSoundLoop(m_music, 1);
}

//==============================================================================

CSoundManager::~CSoundManager(void)
{
  if (m_step != NULL)
  {
    oslDeleteSound(m_step);
    m_step = NULL;
  }
  if (m_jump != NULL)
  {
    oslDeleteSound(m_jump);
    m_jump = NULL;
  }
  if (m_music != NULL)
  {
    oslDeleteSound(m_music);
    m_music = NULL;
  }
}

//==============================================================================

void CSoundManager::playSound(const T_SOUND_EVENT p_sound)
{
  // Play the required sound
  switch(p_sound)
  {
    case SOUND_EVENT_STEP:
      oslPlaySound(m_step, 1);
      break;
    case SOUND_EVENT_JUMP:
      oslPlaySound(m_jump, 1);
      break;
    case SOUND_EVENT_BG_MUSIC:
      oslPlaySound(m_music, 0);
      break;
    default:
      break;
  }
}

//==============================================================================

void CSoundManager::pauseMusic(void)
{
  oslPauseSound(m_music, -1);
}
