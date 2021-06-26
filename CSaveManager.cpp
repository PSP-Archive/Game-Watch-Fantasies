#include "CSaveManager.h"
#include "def.h"

//==============================================================================

CSaveManager& CSaveManager::instance(void)
{
  static CSaveManager g_saveManager;
  return g_saveManager;
}

//==============================================================================

CSaveManager::CSaveManager(void)
{
  load();
}

//==============================================================================

CSaveManager::~CSaveManager(void)
{
}

//==============================================================================

void CSaveManager::oneMoreBeep(void)
{
  ++m_savedItems.m_beeps;
}

//==============================================================================

const u32 &CSaveManager::getNbBeeps(void) const
{
  return m_savedItems.m_beeps;
}

//==============================================================================

const u32 &CSaveManager::getRecord(void) const
{
  return m_savedItems.m_record;
}

//==============================================================================

const u8 CSaveManager::getLevel(void) const
{
  return m_savedItems.m_level;
}

//==============================================================================

void CSaveManager::reset(void)
{
  m_savedItems.m_level = 1;
  m_savedItems.m_beeps = 0;
}

//==============================================================================

void CSaveManager::load(void)
{
  FILE * l_file(NULL);
  l_file = fopen(SAVE_FILE, "r");
  if (l_file != NULL)
  {
    fread(&m_savedItems, sizeof(m_savedItems), 1, l_file);
    fclose(l_file);
  }
}

//==============================================================================

void CSaveManager::save(void)
{
  FILE * l_file(NULL);
  l_file = fopen(SAVE_FILE, "w");
  if (l_file != NULL)
  {
    fwrite(&m_savedItems, sizeof(m_savedItems), 1, l_file);
    fclose(l_file);
  }
}

//==============================================================================

void CSaveManager::submitRecord(void)
{
  if (m_savedItems.m_beeps < m_savedItems.m_record)
  {
    // Record beaten
    m_savedItems.m_record = m_savedItems.m_beeps;
    // Write to file
    save();
  }
}

//==============================================================================

void CSaveManager::setLevel(const u8 p_level)
{
  m_savedItems.m_level = p_level;
}
