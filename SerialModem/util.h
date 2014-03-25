#ifndef _SERIAL_MODEM_VARS__H_
#define _SERIAL_MODEM_VARS__H_

#ifndef MAX
#define MAX(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })
#endif

#ifndef MIN
#define MIN(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })
#endif

namespace Modem {
  static const int SUCCESS = 1;
  static const int NO_RESPONSE = 0;
  static const int ERROR = -1;
  static const int FAILURE = -2;


  const char ESC_ETX    = 0x03;  // Ends socket connection
  const char ESC_DLE    = 0x10;  // Escapes ETX and DLE within Payload
  const char ESC_CR     = 0x0D;
  const char ESC_NL     = 0x0A;
  const char ESC_CTRL_Z = 0x1A;

  // Utility method that does a sprintf and stores the result in a shared buffer
  char *cgb_sprintf(const char *format, ...);

  /**
   * __PROGMEM_STR is a string wrapper used for PROGMEM strings to bring them into the SRAM heap temporarily
   */
  class __PROGMEM_STR {
  public:
    __PROGMEM_STR(PROGMEM_PTR ptr);
    ~__PROGMEM_STR(void);

    operator const char*() { return _buffer; }
    operator char*() { return _buffer; }

  protected:
    char *_buffer;
  };

  /**
   * CircularGapBuffer is a two mode buffer helper tool to aid
   * with getting the most out of limited SRAM via a shared buffer.
   * 1] Gap mode is a simple left/right shared buffer splitter primarily used when PROGMEM strings are being used
   * 2] Circular mode is when we are reading data from the modem and parsing for responses
   */
  class CircularGapBuffer {
  public:
    CircularGapBuffer(char *ref, uint16_t size);
    ~CircularGapBuffer();

    // Gap Mode
    uint16_t sizeAvailableForLeft();
    inline char * left() { return _data; }
    char * reserveRight(uint16_t size);

    // Circular Mode
    void appendCircular(char ch);
    char * strstr(char *str);

  private:
    void beginGap();
    void beginCircular();

    uint8_t _mode;
    char *_data;
    char *_ptrStart;
    char *_ptrEnd;
    uint16_t _gapRight;
    uint16_t _size;
  };
}

extern Modem::CircularGapBuffer *g_gapBuffer;

#endif