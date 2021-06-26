#include "CMenu.h"
#include "CSaveManager.h"
#include "CSoundManager.h"
#include "CMario.h"

//==============================================================================

CMenu::CMenu(void) :
  m_cursor(0)
{
}

//==============================================================================

CMenu::~CMenu(void)
{
}

void CMenu::resetCursor(void)
{
  m_cursor = 0;
}

//==============================================================================

void CMenu::drawBaseWindow(void) const
{
  char l_line[128];
  // Title of the window
  oslDrawFillRect(72, 40, 407, 230, RGBA(0, 0, 0, 170));
  sprintf(l_line, "Game&Watch Fantasies v2.0");
  oslDrawString((480 - oslGetStringWidth(l_line)) / 2, 50, l_line);
  oslDrawLine (112, 65, 367, 65, RGB(255, 255, 255));
  // Bottom
  oslDrawString(190, 210, "Mia - http://beyondds.free.fr");
}

//==============================================================================

void CMenu::drawTitleScreen(void) const
{
  char l_line[128];
  // Get saved data
  u32 l_best(CSaveManager::instance().getRecord());
  u8 l_level(CSaveManager::instance().getLevel());
  // Base window
  drawBaseWindow();
  // Text
  if (l_best < ULONG_MAX) sprintf(l_line, "Best: %lu", l_best);
  else sprintf(l_line, "Best: [none]");
  oslDrawString(90, 100, l_line);
  sprintf(l_line, "%s New game", m_cursor == 0 ? CURSOR_SIGN : EMPTY_SIGN);
  oslDrawString(90, 140, l_line);
  if (l_level > 1) sprintf(l_line, "%s Continue [level %u]", m_cursor == 1 ? CURSOR_SIGN : EMPTY_SIGN, l_level);
  else sprintf(l_line, "%s Continue [no saved game]", m_cursor == 1 ? CURSOR_SIGN : EMPTY_SIGN);
  oslDrawString(90, 150, l_line);
  sprintf(l_line, "%s Quit", m_cursor == 2 ? CURSOR_SIGN : EMPTY_SIGN);
  oslDrawString(90, 160, l_line);
}

//==============================================================================

const T_MENU_EVENT CMenu::playTitleScreen(void)
{
  T_MENU_EVENT l_retVal(MENU_EVENT_NONE);
  // React on buttons
  if (osl_keys->pressed.down)
  {
    if (m_cursor < TITLE_NB_MENU_ITEMS - 1)
      ++m_cursor;
  }
  else if (osl_keys->pressed.up)
  {
    if (m_cursor > 0)
      --m_cursor;
  }
  else if (osl_keys->pressed.circle || osl_keys->pressed.cross)
  {
    switch(m_cursor)
    {
      // New game
      case 0: l_retVal = MENU_EVENT_NEW_GAME; break;
      // Continue
      case 1:
        if (CSaveManager::instance().getLevel() > 1)
        {
          l_retVal = MENU_EVENT_CONTINUE;
          m_cursor = 0;
        }
        break;
      // Quit
      case 2: l_retVal = MENU_EVENT_QUIT; break;
      // Default
      default: break;
    }
  }
  return l_retVal;
}

//==============================================================================

void CMenu::drawEndScreen(void) const
{
  char l_line[128];
  // Base window
  drawBaseWindow();
  // Text
  oslDrawString(90, 90, "A winner is you!");
  sprintf(l_line, "Beeps: %lu", CSaveManager::instance().getNbBeeps());
  oslDrawString(90, 120, l_line);
  sprintf(l_line, "Best:  %lu", CSaveManager::instance().getRecord());
  oslDrawString(90, 130, l_line);
  sprintf(l_line, "%s New game", m_cursor == 0 ? CURSOR_SIGN : EMPTY_SIGN);
  oslDrawString(90, 160, l_line);
  sprintf(l_line, "%s Quit", m_cursor == 1 ? CURSOR_SIGN : EMPTY_SIGN);
  oslDrawString(90, 170, l_line);
}

//==============================================================================

const T_MENU_EVENT CMenu::playEndScreen(void)
{
  T_MENU_EVENT l_retVal(MENU_EVENT_NONE);
  // React on buttons
  if (osl_keys->pressed.down)
  {
    if (m_cursor < END_NB_MENU_ITEMS - 1)
      ++m_cursor;
  }
  else if (osl_keys->pressed.up)
  {
    if (m_cursor > 0)
      --m_cursor;
  }
  else if (osl_keys->pressed.circle || osl_keys->pressed.cross)
  {
    switch(m_cursor)
    {
      // New game
      case 0: l_retVal = MENU_EVENT_NEW_GAME; break;
      // Quit
      case 1: l_retVal = MENU_EVENT_QUIT; break;
      // Default
      default: break;
    }
  }
  return l_retVal;
}

//==============================================================================

void CMenu::drawPauseScreen(void) const
{
  char l_line[128];
  // Base window
  drawBaseWindow();
  // Text
  sprintf(l_line, "Pause [%lu beeps]", CSaveManager::instance().getNbBeeps());
  oslDrawString(90, 100, l_line);
  sprintf(l_line, "%s Reset level", m_cursor == 0 ? CURSOR_SIGN : EMPTY_SIGN);
  oslDrawString(90, 130, l_line);
  sprintf(l_line, "%s New game", m_cursor == 1 ? CURSOR_SIGN : EMPTY_SIGN);
  oslDrawString(90, 140, l_line);
  sprintf(l_line, "%s Gravity: %s", m_cursor == 2 ? CURSOR_SIGN : EMPTY_SIGN, CMario::m_gravity == GRAVITY_EARTH ? "Earth" : "Moon");
  oslDrawString(90, 150, l_line);
  sprintf(l_line, "%s Music: %s", m_cursor == 3 ? CURSOR_SIGN : EMPTY_SIGN, CSoundManager::m_bgMusic ? "On" : "Off");
  oslDrawString(90, 160, l_line);
  sprintf(l_line, "%s Quit", m_cursor == 4 ? CURSOR_SIGN : EMPTY_SIGN);
  oslDrawString(90, 170, l_line);
}

//==============================================================================

const T_MENU_EVENT CMenu::playPauseScreen(void)
{
  T_MENU_EVENT l_retVal(MENU_EVENT_NONE);
  // React on buttons
  if (osl_keys->pressed.down)
  {
    if (m_cursor < PAUSE_NB_MENU_ITEM - 1)
      ++m_cursor;
  }
  else if (osl_keys->pressed.up)
  {
    if (m_cursor > 0)
      --m_cursor;
  }
  else if (osl_keys->pressed.circle || osl_keys->pressed.cross)
  {
    switch(m_cursor)
    {
      // Reset level
      case 0: l_retVal = MENU_EVENT_RESET_LEVEL; break;
      // New game
      case 1: l_retVal = MENU_EVENT_NEW_GAME; m_cursor = 0; break;
      // Gravity
      case 2: l_retVal = MENU_EVENT_CHANGE_GRAVITY; break;
      // Music
      case 3: l_retVal = MENU_EVENT_MUSIC; break;
      // Quit
      case 4: l_retVal = MENU_EVENT_QUIT; break;
      // Default
      default: break;
    }
  }
  return l_retVal;
}
