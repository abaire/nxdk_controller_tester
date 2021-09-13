#ifndef NXDK_CONTROLLER_TESTER_DEBUGOUTPUT_H
#define NXDK_CONTROLLER_TESTER_DEBUGOUTPUT_H

#include <SDL.h>

#ifdef _DEBUG
#define PRINTMSG(c)  DbgPrint c
#else
#define PRINTMSG(c)
#endif

inline void ErrorPrintLastError(LPCSTR message) {
  PRINTMSG(("[ERROR]: %s 0x%x", message, GetLastError()));
}

template <typename... VarArgs>
inline void ErrorPrintLastError(LPCSTR message, VarArgs &&...args) {
#ifdef _DEBUG
  int string_length = snprintf(nullptr, 0, message, args...);
  std::string buf;
  buf.resize(string_length);

  snprintf(&buf[0], string_length + 1, message, args...);
  ErrorPrintLastError(buf.c_str());
#endif
}


inline void ErrorPrintSDLError(LPCSTR message) {
  PRINTMSG(("[ERROR]: %s - %s", message, SDL_GetError()));
}

template <typename... VarArgs>
inline void ErrorPrintSDLError(LPCSTR message, VarArgs &&...args) {
#ifdef _DEBUG
  int string_length = snprintf(nullptr, 0, message, args...);
  std::string buf;
  buf.resize(string_length);

  snprintf(&buf[0], string_length + 1, message, args...);
  ErrorPrintSDLError(buf.c_str());
#endif
}

#endif  // NXDK_CONTROLLER_TESTER_DEBUGOUTPUT_H
