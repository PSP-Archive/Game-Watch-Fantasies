#include "CMario.h"
#include "CSoundManager.h"
#include "CSaveManager.h"

//==============================================================================

CMario::CMario(void) :
  m_sprite(NULL),
  m_frameIndex(0),
  m_state(MARIO_STATE_STANDING),
  m_gravityTimer(0)
{
  // Load spriteset
  m_sprite = oslLoadImageFile("res/spriteset_mario.png", OSL_IN_RAM, OSL_PF_5551);
  if (m_sprite == NULL)
    oslFatalError("File res/spriteset_mario.png is missing!");
  // Set to first frame
  oslSetImageFrameSize(m_sprite, MARIO_WIDTH, MARIO_HEIGHT);
  oslSetImageFrame(m_sprite, 0);
}

//==============================================================================

CMario::~CMario(void)
{
  if (m_sprite != NULL)
  {
    oslDeleteImage(m_sprite);
    m_sprite = NULL;
  }
}

//==============================================================================

void CMario::reset(const u8 p_levelNumber)
{
  // Put Mario in his initial position, depending on the level
  switch(p_levelNumber)
  {
    case 1:
      m_sprite->x = 115;
      m_sprite->y = 216;
      break;
    case 4:
      m_sprite->x = 0;
      m_sprite->y = 111;
      break;
    case 7:
      m_sprite->x = 0;
      m_sprite->y = 24;
      break;
    case 8:
      m_sprite->x = 30;
      m_sprite->y = 216;
      break;
    case 9:
      m_sprite->x = 0;
      m_sprite->y = 120;
      break;
    case 11:
      m_sprite->x = 0;
      m_sprite->y = 160;
      break;
    case 13:
      m_sprite->x = 0;
      m_sprite->y = 203;
      break;
    case 14:
      m_sprite->x = 0;
      m_sprite->y = 176;
      break;
    case 16:
      m_sprite->x = 0;
      m_sprite->y = 30;
      break;
    default:
      m_sprite->x = 0;
      m_sprite->y = 216;
      break;
  }
  m_frameIndex = 0;
  m_gravityTimer = 0;
  m_state = MARIO_STATE_STANDING;
  oslSetImageFrame(m_sprite, 0);
}

//==============================================================================

void CMario::draw(void)
{
  oslDrawImage(m_sprite);
  #ifdef DEBUG
  char l_line[128];
  sprintf(l_line, "Mario: %d   %d   %d       ", m_frameIndex, m_state, m_gravityTimer);
  oslDrawString(8, 18, l_line);
  #endif
}

//==============================================================================

void CMario::move(const T_DIRECTION p_direction, OSL_IMAGE * &p_level)
{
  bool l_moved(false);
  bool l_jump(false);
  // Move the sprite
  switch(p_direction)
  {
//------------------------------------------------------------------------------
    case DIRECTION_RIGHT:
      if (xIsOk(m_sprite->x + MARIO_GAP_X, p_level))
      {
        m_sprite->x += MARIO_GAP_X;
        l_moved = true;
      }
      else
        l_moved = pad(DIRECTION_RIGHT, p_level);
      if (l_moved) adjustPosition(DIRECTION_UP, p_level);
      break;
//------------------------------------------------------------------------------
    case DIRECTION_LEFT:
      if (xIsOk(m_sprite->x - MARIO_GAP_X, p_level))
      {
        m_sprite->x -= MARIO_GAP_X;
        l_moved = true;
      }
      else
        l_moved = pad(DIRECTION_LEFT, p_level);
      if (l_moved) adjustPosition(DIRECTION_UP, p_level);
      break;
//------------------------------------------------------------------------------
    case DIRECTION_UP:
      if (yIsOk(m_sprite->y - MARIO_GAP_Y, p_level))
      {
        m_sprite->y -= MARIO_GAP_Y;
        l_moved = true;
        l_jump = true;
      }
      else
      {
        l_moved = pad(DIRECTION_UP, p_level);
        l_jump = l_moved;
      }
      if (l_moved) adjustPosition(DIRECTION_RIGHT, p_level);
      break;
//------------------------------------------------------------------------------
    case DIRECTION_DOWN:
      l_moved = pad(DIRECTION_DOWN, p_level);
      if (l_moved) adjustPosition(DIRECTION_RIGHT, p_level);
      break;
    default:
      break;
  }
//------------------------------------------------------------------------------
  // Update Mario if he moved
  if (l_moved)
  {
    // Beep
    if (l_jump) CSoundManager::instance().playSound(SOUND_EVENT_JUMP);
    else if (p_direction != DIRECTION_DOWN) CSoundManager::instance().playSound(SOUND_EVENT_STEP);
    // Nb of beeps + 1
    if (p_direction != DIRECTION_DOWN)
      CSaveManager::instance().oneMoreBeep();
    // Change frame
    if (l_jump)
    {
      if (!oslGetImagePixel(p_level, m_sprite->x + MARIO_HITBOX_X, m_sprite->y + MARIO_HEIGHT))
        // Mario is in the air => jump frame
        oslSetImageFrame(m_sprite, 9);
      else
      {
        nextFrame();
      }
    }
    else
    {
      nextFrame();
    }
  }
}

//==============================================================================

const bool CMario::xIsOk(const int p_x, OSL_IMAGE * &p_level) const
{
  // Check whether Mario at the given x coordinate overlaps with an obstacle 
  bool l_result(true);
  for (u8 l_i = 0; l_result && (l_i < MARIO_WIDTH); ++l_i)
    if (oslGetImagePixel(p_level, p_x + l_i, m_sprite->y + MARIO_HITBOX_Y))
      l_result = false;
  return l_result;
}

//==============================================================================

const bool CMario::yIsOk(const int p_y, OSL_IMAGE * &p_level) const
{
  // Check whether Mario at the y coordinate overlaps with an obstacle 
  bool l_result(true);
  for (u8 l_i = 0; l_result && (l_i < MARIO_HEIGHT); ++l_i)
    if (oslGetImagePixel(p_level, m_sprite->x + MARIO_HITBOX_X, p_y + l_i))
      l_result = false;
  return l_result;
}

//==============================================================================

const bool CMario::pad(const T_DIRECTION p_direction, OSL_IMAGE * &p_level)
{
  bool l_result(false);
  // Move Mario pixel by pixel till he encounters an obstacle (i.e. a non-transparent pixel)
  // A transparent pixel has a color value of 0
  switch(p_direction)
  {
    case DIRECTION_RIGHT:
      for (u8 l_i = 0; (l_i < MARIO_GAP_X) && !oslGetImagePixel(p_level, m_sprite->x + MARIO_WIDTH, m_sprite->y + MARIO_HITBOX_Y); ++l_i)
      {
        ++m_sprite->x;
        if (!l_result) l_result = true;
      }
      break;
    case DIRECTION_LEFT:
      for (u8 l_i = 0; (l_i < MARIO_GAP_X) && !oslGetImagePixel(p_level, m_sprite->x - 1, m_sprite->y + MARIO_HITBOX_Y); ++l_i)
      {
        --m_sprite->x;
        if (!l_result) l_result = true;
      }
      break;
    case DIRECTION_UP:
      for (u8 l_i = 0; (l_i < MARIO_GAP_Y) && !oslGetImagePixel(p_level, m_sprite->x + MARIO_HITBOX_X, m_sprite->y - 1); ++l_i)
      {
        --m_sprite->y;
        if (!l_result) l_result = true;
      }
      break;
    case DIRECTION_DOWN:
      for (u8 l_i = 0; (l_i < MARIO_GAP_Y) && !oslGetImagePixel(p_level, m_sprite->x + MARIO_HITBOX_X, m_sprite->y + MARIO_HEIGHT); ++l_i)
      {
        ++m_sprite->y;
        if (!l_result) l_result = true;
      }
      break;
    default:
      break;
  }
  return l_result;
}

//==============================================================================

void CMario::adjustPosition(const T_DIRECTION p_direction, OSL_IMAGE * &p_level)
{
  // Adjust Mario's horizontal or vertical position if he is overlapping too much with the platforms
  switch(p_direction)
  {
    case DIRECTION_UP:
    case DIRECTION_DOWN:
      while(oslGetImagePixel(p_level, m_sprite->x + MARIO_HITBOX_X, m_sprite->y + MARIO_HEIGHT - 1) &&
            !oslGetImagePixel(p_level, m_sprite->x + MARIO_HITBOX_X, m_sprite->y - 1))
        --m_sprite->y;
      while(oslGetImagePixel(p_level, m_sprite->x + MARIO_HITBOX_X, m_sprite->y) &&
            !oslGetImagePixel(p_level, m_sprite->x + MARIO_HITBOX_X, m_sprite->y + MARIO_HEIGHT))
        ++m_sprite->y;
      break;
    case DIRECTION_RIGHT:
    case DIRECTION_LEFT:
      while(oslGetImagePixel(p_level, m_sprite->x, m_sprite->y + MARIO_HITBOX_Y) &&
            !oslGetImagePixel(p_level, m_sprite->x + MARIO_WIDTH, m_sprite->y + MARIO_HITBOX_Y))
        ++m_sprite->x;
      while(oslGetImagePixel(p_level, m_sprite->x + MARIO_WIDTH - 1, m_sprite->y + MARIO_HITBOX_Y) &&
            !oslGetImagePixel(p_level, m_sprite->x - 1, m_sprite->y + MARIO_HITBOX_Y))
        --m_sprite->x;
      break;
    default:
      break;
  }
}

//==============================================================================

void CMario::play(OSL_IMAGE * &p_level)
{
  // Move Mario when buttons are pressed
  switch(m_state)
  {
//------------------------------------------------------------------------------
    case MARIO_STATE_STANDING:
      if (osl_keys->pressed.right)
        move(DIRECTION_RIGHT, p_level);
      else if (osl_keys->pressed.left)
        move(DIRECTION_LEFT, p_level);
      else if (osl_keys->pressed.circle || osl_keys->pressed.cross)
        move(DIRECTION_UP, p_level);
      break;
//------------------------------------------------------------------------------
    case MARIO_STATE_JUMPING:
      if (osl_keys->pressed.right)
        move(DIRECTION_RIGHT, p_level);
      else if (osl_keys->pressed.left)
        move(DIRECTION_LEFT, p_level);
      // Gravity
      if (m_gravityTimer)
      {
        --m_gravityTimer;
        if (!m_gravityTimer)
          move(DIRECTION_DOWN, p_level);
      }
      break;
//------------------------------------------------------------------------------
    default:
      break;
  }
  // Update Mario state
  if (oslGetImagePixel(p_level, m_sprite->x + MARIO_HITBOX_X, m_sprite->y + MARIO_HEIGHT))
  {
    if (m_state == MARIO_STATE_JUMPING)
    {
      m_state = MARIO_STATE_STANDING;
      nextFrame();
    }
  }
  else
  {
    if (m_state == MARIO_STATE_STANDING)
      m_state = MARIO_STATE_JUMPING;
  }
  // Manage gravity timer
  switch(m_state)
  {
    case MARIO_STATE_JUMPING:
      if (!m_gravityTimer) m_gravityTimer = m_gravity;
      break;
    case MARIO_STATE_STANDING:
      if (m_gravityTimer) m_gravityTimer = 0;
      break;
    default:
      break;
  }
  // Uncache image
  oslUncacheImage(p_level);
}

//==============================================================================

const bool CMario::beatLevel(void) const
{
  return (m_sprite->x + MARIO_WIDTH >= 480);
}

//==============================================================================

void CMario::nextFrame(void)
{
  m_frameIndex = (m_frameIndex + 1) % MARIO_NB_FRAMES;
  oslSetImageFrame(m_sprite, m_frameIndex);
}
