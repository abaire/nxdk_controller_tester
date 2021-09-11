#include "XBFont.h"

#include <SDL_FontCache/SDL_FontCache.h>
#include <cassert>

CXBFont::~CXBFont() { Destroy(); }

HRESULT CXBFont::Create(XBRenderer *renderer, const char *path) {
  Destroy();

  font = FC_CreateFont();
  assert(font);

  SDL_Renderer *sdl_renderer = renderer->GetRenderer();
  bool load_success = FC_LoadFont(
      font, sdl_renderer, path, 20, FC_MakeColor(250, 250, 250, 255), TTF_STYLE_NORMAL);

  assert(load_success);
  if (!load_success) {
    return E_FAIL;
  }

  return S_OK;
}

HRESULT CXBFont::Destroy() {
  renderer = nullptr;
  if (font) {
    FC_FreeFont(font);
    font = nullptr;
  }

  return S_OK;
}

std::pair<float, float> CXBFont::draw(const std::string &str,
                                      std::pair<float, float> coordinates) {
  FC_Draw(font, renderer->GetRenderer(), std::get<0>(coordinates),
          std::get<1>(coordinates), "%s", str.c_str());

  FC_Rect rect =
      FC_GetBounds(font, std::get<0>(coordinates), std::get<1>(coordinates),
                   FC_ALIGN_LEFT, FC_MakeScale(1.0f, 1.0f), "%s", str.c_str());

  return std::pair<float, float>(rect.w, rect.h);
}
