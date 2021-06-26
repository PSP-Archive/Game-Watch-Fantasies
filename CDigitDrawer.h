#ifndef CDIGITDRAWER_H_
#define CDIGITDRAWER_H_

#include <oslib/oslib.h>
#include "def.h"

class CDigitDrawer
{
//==============================================================================

  public:
  
  // Constructor
  CDigitDrawer(void);
  
  // Destructor
  virtual ~CDigitDrawer(void);
  
  // Draw the score to an image
  void drawScore(OSL_IMAGE * &p_image, const int p_x, const int p_y);

  // Draw current time to an image
  void drawTime(OSL_IMAGE * &p_image, const int p_x, const int p_y);

  // Draw a number to the current buffer
  void drawNumberToCurrentBuffer(const u32 p_number, const int p_x, const int p_y, const int p_nbDigits);

//==============================================================================
  
  private:
  
  // Copy constructor and operator '=' forbidden
  CDigitDrawer(const CDigitDrawer &p_source);
  const CDigitDrawer &operator=(const CDigitDrawer &p_right);
  
  // Init time zone to get correct local time
  void initTimezone(void);
  
  // Digit spriteset
  OSL_IMAGE * m_digits;
  
//==============================================================================
};

#endif
