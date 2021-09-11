#pragma once

#include <SDL_FontCache/SDL_FontCache.h>
#include <string>
#include <windows.h>

#include "XBRenderer.h"

class CXBFont {
 public:
  CXBFont() = default;
  ~CXBFont();

  HRESULT Create(XBRenderer *renderer, const char *font_path);
  HRESULT Destroy();

  std::pair<float, float> draw(const std::string &str,
                               std::pair<float, float> coordinates);

 private:
  FC_Font *font;
  XBRenderer *renderer;
};
