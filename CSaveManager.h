#ifndef CSAVEMANAGER_H_
#define CSAVEMANAGER_H_

#include <oslib/oslib.h>
#include "def.h"

class CSaveManager
{
//==============================================================================

  public:
  
  // Accessor to the singleton
  static CSaveManager& instance(void);

  // Destructor
  virtual ~CSaveManager(void);
  
  // Add a beep
  void oneMoreBeep(void);
  
  // Set current level
  void setLevel(const u8 p_level);
  
  // Returns the current number of beeps
  const u32 &getNbBeeps(void) const;

  // Returns the current record
  const u32 &getRecord(void) const;
  
  // Returns the current level
  const u8 getLevel(void) const;
  
  // Save the new record if the current is beaten
  void submitRecord(void);
  
  // Save the values in the file
  void save(void);
  
  // Reset
  void reset(void);

//==============================================================================

  private:
  
  // Forbidden
  CSaveManager(void);
  CSaveManager(const CSaveManager &p_source);
  const CSaveManager &operator=(const CSaveManager &p_right);

  // Read saved data
  void load(void);
  
  // Saved items: level, beeps, record
  T_SAVE_DATA m_savedItems;
  
//==============================================================================
};

#endif
