#include "CGame.h"
#include "def.h"
#include "CSaveManager.h"
#include "CSoundManager.h"

//==============================================================================

CGame::CGame(void) :
  m_state(GAME_STATE_TITLE),
  m_background(NULL),
  m_platforms(NULL),
  m_platformsSave(NULL),
  m_level(0)
{
  // Load background
  m_background = oslLoadImageFile("res/background.png", OSL_IN_RAM, OSL_PF_8888);
  if (m_background == NULL) oslFatalError("File res/background.png is missing!");
  // By default, load first level
  m_platforms = oslLoadImageFile("res/level_01.png", OSL_IN_VRAM, OSL_PF_5551);
  if (m_platforms == NULL) oslFatalError("File res/level_01.png is missing!");
}

//==============================================================================

CGame::~CGame(void)
{
  if (m_platforms != NULL)
  {
    oslDeleteImage(m_platforms);
    m_platforms = NULL;
  }
  if (m_platformsSave != NULL)
  {
    oslDeleteImage(m_platformsSave);
    m_platformsSave = NULL;
  }
  if (m_background != NULL)
  {
    oslDeleteImage(m_background);
    m_background = NULL;
  }
}

//==============================================================================

void CGame::goToNextLevel(const bool p_save)
{
  // Increment level number
  ++m_level;
  // Save the new level if requested
  if (p_save)
  {
    CSaveManager::instance().setLevel(m_level);
    CSaveManager::instance().save();
  }
  // Delete existing images
  if (m_platforms != NULL)
  {
    oslDeleteImage(m_platforms);
    m_platforms = NULL;
  }
  if (m_platformsSave != NULL)
  {
    oslDeleteImage(m_platformsSave);
    m_platformsSave = NULL;
  }
  // File name = "level_xx.png"
  char l_fileName[32];
  sprintf(l_fileName, "res/level_%02d.png", m_level);
  // Load platforms image(s) depending on the level
  if (m_level == LEVEL_WITH_TIME || m_level == LEVEL_WITH_SCORE)
  {
    m_platformsSave = oslLoadImageFile(l_fileName, OSL_IN_RAM, OSL_PF_5551);
    if (m_platformsSave == NULL) oslFatalError("File %s is missing!", l_fileName);
    m_platforms = oslCreateImage(480, 272, OSL_IN_VRAM, OSL_PF_5551);
    if (m_platforms == NULL) oslFatalError("Unable to create image!");
  }
  else
  {
    m_platforms = oslLoadImageFile(l_fileName, OSL_IN_VRAM, OSL_PF_5551);
    if (m_platforms == NULL) oslFatalError("File %s is missing!", l_fileName);
  }
  // Put Mario in his initial position for the new level
  m_mario.reset(m_level);
}

//==============================================================================

void CGame::play(void)
{
  // Manage buttons depending on the game state
  switch(m_state)
  {
//------------------------------------------------------------------------------
    case GAME_STATE_TITLE:
      switch(m_menu.playTitleScreen())
      {
        case MENU_EVENT_NEW_GAME:
          // Start a new game
          CSaveManager::instance().reset();
          goToNextLevel();
          CSoundManager::instance().playSound(SOUND_EVENT_BG_MUSIC);
          m_state = GAME_STATE_PLAYING;
          break;
        case MENU_EVENT_CONTINUE:
          // Continue saved game
          m_level = CSaveManager::instance().getLevel() - 1;
          goToNextLevel(false);
          CSoundManager::instance().playSound(SOUND_EVENT_BG_MUSIC);
          m_state = GAME_STATE_PLAYING;
          break;
        case MENU_EVENT_QUIT:
          // Bye
          oslQuit();
          break;
        default:
          break;
      }
      break;
//------------------------------------------------------------------------------
    case GAME_STATE_PLAYING:
      // Modify platforms for special levels
      buildLevel();
      // Manage buttons
      if (osl_keys->pressed.start)
        m_state = GAME_STATE_PAUSED;
      #ifdef DEBUG
      else if (osl_keys->pressed.R)
        goToNextLevel();
      #endif
      else
      {
        // Move Mario depending on the buttons pressed and the current level
        m_mario.play(m_platforms);
        // Check whether the level is beaten
        if (m_mario.beatLevel())
        {
          if (m_level == NB_LEVELS)
          {
            // Game beaten! Submit the new score
            CSaveManager::instance().submitRecord();
            // Rebuild level to have the last step displayed
            buildLevel();
            // Go to the end screen
            m_state = GAME_STATE_END;
          }
          else
          {
            // Next level, please
            goToNextLevel();
          }
        }
      }
      break;
//------------------------------------------------------------------------------
    case GAME_STATE_PAUSED:
      // Manage buttons
      if (osl_keys->pressed.start)
      {
        m_menu.resetCursor();
        m_state = GAME_STATE_PLAYING;
      }
      else
      {
        // Navigate through the pause menu
        switch(m_menu.playPauseScreen())
        {
          case MENU_EVENT_RESET_LEVEL:
            // Reset Mario position
            m_mario.reset(m_level);
            m_state = GAME_STATE_PLAYING;
            break;
          case MENU_EVENT_NEW_GAME:
            // Restart from the beginning
            CSaveManager::instance().reset();
            m_level = 0;
            goToNextLevel();
            m_state = GAME_STATE_PLAYING;
            break;
          case MENU_EVENT_CHANGE_GRAVITY:
            // Switch gravity
            if (CMario::m_gravity == GRAVITY_EARTH) CMario::m_gravity = GRAVITY_MOON;
            else CMario::m_gravity = GRAVITY_EARTH;
            break;
          case MENU_EVENT_MUSIC:
            // Switch music on/off
            CSoundManager::m_bgMusic = !CSoundManager::m_bgMusic;
            CSoundManager::instance().pauseMusic();
            break;
          case MENU_EVENT_QUIT:
            // See ya
            oslQuit();
            break;
          default:
            break;
        }
      }
      break;
//------------------------------------------------------------------------------
    case GAME_STATE_END:
      switch(m_menu.playEndScreen())
      {
        case MENU_EVENT_NEW_GAME:
          // Restart from the beginning
          CSaveManager::instance().reset();
          m_level = 0;
          goToNextLevel();
          m_state = GAME_STATE_PLAYING;
          break;
        case MENU_EVENT_QUIT:
          // Guess what this function does
          oslQuit();
          break;
        default:
          break;
      }
      break;
//------------------------------------------------------------------------------
    default:
      break;
  }
}

//==============================================================================

void CGame::drawLCDGhosts(void)
{
  switch(m_level)
  {
    case LEVEL_WITH_TIME:
      oslSetAlpha(OSL_FX_ALPHA, DIGIT_GHOST_ALPHA);
      m_digitDrawer.drawNumberToCurrentBuffer(88, 353, 89, 2);
      m_digitDrawer.drawNumberToCurrentBuffer(88, 233, 89, 2);
      m_digitDrawer.drawNumberToCurrentBuffer(88, 113, 89, 2);
      oslSetAlpha(OSL_FX_RGBA, DIGIT_GHOST_ALPHA);
      break;
    case LEVEL_WITH_SCORE:
      oslSetAlpha(OSL_FX_ALPHA, DIGIT_GHOST_ALPHA);
      m_digitDrawer.drawNumberToCurrentBuffer(888888, 313, 80, 6);
      oslSetAlpha(OSL_FX_RGBA, DIGIT_GHOST_ALPHA);
      break;
    default:
      break;
  }
}

//==============================================================================

void CGame::draw(void)
{
  // Draw the background
  oslDrawImage(m_background);
  // Draw LCD ghost if necessary
  drawLCDGhosts();
  // Draw the platforms
  oslDrawImage(m_platforms);
  // State-specific zone
  switch(m_state)
  {
    case GAME_STATE_TITLE: m_menu.drawTitleScreen(); break;
    case GAME_STATE_PLAYING: m_mario.draw(); break;
    case GAME_STATE_PAUSED: m_menu.drawPauseScreen(); break;
    case GAME_STATE_END: m_menu.drawEndScreen(); break;
    default: break;
  }
  #ifdef DEBUG
  char l_line[128];
  sprintf(l_line, "Save data: %u   %lu   %lu       ", CSaveManager::instance().getLevel(), CSaveManager::instance().getNbBeeps(), CSaveManager::instance().getRecord());
  oslDrawString(8, 8, l_line);
  #endif
}

//==============================================================================

void CGame::buildLevel(void)
{
  // Modify platforms for special levels
  switch(m_level)
  {
    case LEVEL_WITH_TIME:
      // Init the platform image to its initial value (i.e. without the digits)
      oslCopyImageTo(m_platforms, m_platformsSave);
      // Draw time on it
      m_digitDrawer.drawTime(m_platforms, 353, 89);
      // Wait for GPU
      oslSyncDrawing();
      break;
    case LEVEL_WITH_SCORE:
      // Init the platform image to its initial value (i.e. without the digits)
      oslCopyImageTo(m_platforms, m_platformsSave);
      // Draw score on it
      m_digitDrawer.drawScore(m_platforms, 313, 80);
      // Wait for GPU
      oslSyncDrawing();
      break;
    default:
      break;
  }
}
