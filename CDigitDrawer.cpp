#include "CDigitDrawer.h"
#include "CSaveManager.h"

//==============================================================================

CDigitDrawer::CDigitDrawer(void) :
  m_digits(NULL)
{
  initTimezone();
  // Load the digits spriteset
  m_digits = oslLoadImageFile("res/spriteset_digits.png", OSL_IN_RAM, OSL_PF_5551);
  if (m_digits == NULL) oslFatalError("File res/spriteset_digits.png is missing!");
  // Configure frame
  oslSetImageFrameSize(m_digits, DIGIT_WIDTH, DIGIT_HEIGHT);
}

//==============================================================================

CDigitDrawer::~CDigitDrawer(void)
{
  // Delete digits spriteset
  if (m_digits != NULL)
  {
    oslDeleteImage(m_digits);
    m_digits = NULL;
  }
}

//==============================================================================

void CDigitDrawer::initTimezone(void)
{
  int l_tzOffset = 0;
  int l_dst = 0;
  sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_TIMEZONE, &l_tzOffset);
  sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_DAYLIGHTSAVINGS, &l_dst);
  int l_tzOffsetAbs = l_tzOffset < 0 ? -l_tzOffset : l_tzOffset;
  int l_hours = l_tzOffsetAbs / 60;
  int l_minutes = l_tzOffsetAbs - l_hours * 60;
  char * l_tz = NULL;
  if (l_dst == 1)
  {
    l_tz = new char(14);
    sprintf(l_tz, "GMT%s%02i:%02i DST", l_tzOffset < 0 ? "+" : "-", l_hours, l_minutes);
  }
  else
  {
    l_tz = new char(10);
    sprintf(l_tz, "GMT%s%02i:%02i", l_tzOffset < 0 ? "+" : "-", l_hours, l_minutes);
  }
  setenv("TZ", l_tz, 1);
  tzset();
  delete [] l_tz;
}

//==============================================================================

void CDigitDrawer::drawScore(OSL_IMAGE * &p_image, const int p_x, const int p_y)
{
  // Set draw buffer to the image
  oslSetDrawBuffer(p_image);
  oslSetAlphaWrite(OSL_FXAW_SET, 255, 0);
  oslSetAlphaTest(OSL_FXAT_GREATER, 0);
  // Draw the number
  drawNumberToCurrentBuffer(CSaveManager::instance().getNbBeeps(), p_x, p_y, 6);
  // Restore the default draw buffer
  oslSetDrawBuffer(OSL_DEFAULT_BUFFER);
}

//==============================================================================

void CDigitDrawer::drawTime(OSL_IMAGE * &p_image, const int p_x, const int p_y)
{
  // Set draw buffer to the image
  oslSetDrawBuffer(p_image);
  oslSetAlphaWrite(OSL_FXAW_SET, 255, 0);
  oslSetAlphaTest(OSL_FXAT_GREATER, 0);
  // Get current time as a struct tm
  time_t l_rawtime = sceKernelLibcTime(NULL);
  struct tm * l_timeinfo = localtime(&l_rawtime);
  // Draw the seconds, minutes and hours
  drawNumberToCurrentBuffer(l_timeinfo->tm_sec, p_x, p_y, 2);
  drawNumberToCurrentBuffer(l_timeinfo->tm_min, p_x - 120, p_y, 2);
  drawNumberToCurrentBuffer(l_timeinfo->tm_hour, p_x - 240, p_y, 2);
  // Draw the ':' separators
  oslDrawFillRect(p_x - 66, p_y + 31, p_x - 60, p_y + 35, RGB(0, 0, 0));
  oslDrawFillRect(p_x - 66, p_y + 42, p_x - 60, p_y + 46, RGB(0, 0, 0));
  oslDrawFillRect(p_x - 186, p_y + 31, p_x - 180, p_y + 35, RGB(0, 0, 0));
  oslDrawFillRect(p_x - 186, p_y + 42, p_x - 180, p_y + 46, RGB(0, 0, 0));
  // Restore the default draw buffer
  oslSetDrawBuffer(OSL_DEFAULT_BUFFER);
}

//==============================================================================

void CDigitDrawer::drawNumberToCurrentBuffer(const u32 p_number, const int p_x, const int p_y, const int p_nbDigits)
{
  // Set the initial position
  m_digits->x = p_x;
  m_digits->y = p_y;
  // Get the number to display as a string 
  char l_number[16];
  sprintf(l_number, "%0*lu", p_nbDigits, p_number);
  u8 l_i = strlen(l_number);
  if (l_i > p_nbDigits) l_i = p_nbDigits;
  for (--l_i; l_i != UCHAR_MAX ; --l_i)
  {
    // Set the correct frame
    oslSetImageFrame(m_digits, l_number[l_i] - 48);
    // Draw the digit
    oslDrawImage(m_digits);
    // Update the x position for the next digit
    m_digits->x -= MARIO_GAP_X;
  }
}
