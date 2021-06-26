#ifndef DEF_H_
#define DEF_H_

//==============================================================================

//#define DEBUG

#define MARIO_WIDTH 21
#define MARIO_HEIGHT 24
#define MARIO_GAP_X 40
#define MARIO_GAP_Y 40
#define MARIO_NB_FRAMES 9
#define MARIO_HITBOX_X 10
#define MARIO_HITBOX_Y 11

#define GRAVITY_EARTH 24
#define GRAVITY_MOON 32

#define NB_LEVELS 16
#define LEVEL_WITH_TIME 14
#define LEVEL_WITH_SCORE 16

#define SAVE_FILE "res/GWF.sav"

#define DIGIT_WIDTH 35
#define DIGIT_HEIGHT 78
#define DIGIT_GHOST_ALPHA 12

#define CURSOR_SIGN "->"
#define EMPTY_SIGN "  "
#define TITLE_NB_MENU_ITEMS 3
#define END_NB_MENU_ITEMS 2
#define PAUSE_NB_MENU_ITEM 5

//==============================================================================

// Game states
typedef enum
{
  GAME_STATE_TITLE = 0,
  GAME_STATE_PLAYING,
  GAME_STATE_PAUSED,
  GAME_STATE_END
}
T_GAME_STATE;

// 4 directions
typedef enum
{
  DIRECTION_RIGHT = 0,
  DIRECTION_LEFT,
  DIRECTION_UP,
  DIRECTION_DOWN
}
T_DIRECTION;

// Mario states
typedef enum
{
  MARIO_STATE_STANDING = 0,
  MARIO_STATE_JUMPING
}
T_MARIO_STATE;

// Sounds and music
typedef enum
{
  SOUND_EVENT_STEP = 0,
  SOUND_EVENT_JUMP,
  SOUND_EVENT_BG_MUSIC
}
T_SOUND_EVENT;

// Saved data
typedef struct STRUCT_SAVE_DATA
{
  u8 m_level;
  u32 m_beeps;
  u32 m_record;
  STRUCT_SAVE_DATA(void): m_level(1), m_beeps(0), m_record(ULONG_MAX) {}
  private:
  STRUCT_SAVE_DATA(const STRUCT_SAVE_DATA &p_source);
  const STRUCT_SAVE_DATA &operator=(const STRUCT_SAVE_DATA &p_right);
}
T_SAVE_DATA;

// Menu events
typedef enum
{
  MENU_EVENT_NONE = 0,
  MENU_EVENT_NEW_GAME,
  MENU_EVENT_CONTINUE,
  MENU_EVENT_CHANGE_GRAVITY,
  MENU_EVENT_MUSIC,
  MENU_EVENT_QUIT,
  MENU_EVENT_RESET_LEVEL
}
T_MENU_EVENT;

//==============================================================================

#endif
