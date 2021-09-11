#pragma once

#include <SDL_FontCache/SDL_FontCache.h>
#include <windows.h>

#include <string>

#include "XBRenderer.h"

class CXBFont {
 public:
  CXBFont() = default;
  ~CXBFont();

  HRESULT Create(XBRenderer *renderer, const char *font_path);
  HRESULT Destroy();

  SDL_Rect Draw(LPCSTR str, float x, float y) const;

  inline SDL_Rect Draw(LPCSTR str, int x, int y) const {
    return Draw(str, static_cast<float>(x), static_cast<float>(y));
  }

  [[nodiscard]] inline SDL_Rect Draw(LPCSTR str,
                                       const SDL_FPoint &coordinates) const {
    return Draw(str, coordinates.x, coordinates.y);
  }

  [[nodiscard]] inline SDL_Rect Draw(LPCSTR str,
                                       const SDL_Point &coordinates) const {
    return Draw(str, coordinates.x, coordinates.y);
  }

  template <typename... VarArgs>
  inline SDL_Rect DrawFmt(float x, float y, LPCSTR message,
                            VarArgs &&...args) const {
    int string_length = snprintf(nullptr, 0, message, args...);
    std::string buf;
    buf.resize(string_length);

    snprintf(&buf[0], string_length + 1, message, args...);
    return Draw(buf.c_str(), x, y);
  }

  template <typename... VarArgs>
  inline SDL_Rect DrawFmt(int x, int y, LPCSTR message,
                            VarArgs &&...args) const {
    int string_length = snprintf(nullptr, 0, message, args...);
    std::string buf;
    buf.resize(string_length);

    snprintf(&buf[0], string_length + 1, message, args...);
    return Draw(buf.c_str(), x, y);
  }

 private:
  FC_Font *font_{nullptr};
  XBRenderer *renderer_{nullptr};
};
