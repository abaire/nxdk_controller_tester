#include "XBFont.h"

#include <SDL_FontCache/SDL_FontCache.h>

#include <cassert>

CXBFont::~CXBFont() { Destroy(); }

HRESULT CXBFont::Create(XBRenderer *renderer, const char *path) {
  Destroy();

  renderer_ = renderer;
  font_ = FC_CreateFont();
  assert(font_);

  SDL_Renderer *sdl_renderer = renderer->GetRenderer();
  bool load_success =
      FC_LoadFont(font_, sdl_renderer, path, 20,
                  FC_MakeColor(250, 250, 250, 255), TTF_STYLE_NORMAL);

  assert(load_success);
  if (!load_success) {
    return E_FAIL;
  }

  return S_OK;
}

HRESULT CXBFont::Destroy() {
  renderer_ = nullptr;
  if (font_) {
    FC_FreeFont(font_);
    font_ = nullptr;
  }

  return S_OK;
}

FC_Rect CXBFont::Draw(LPCSTR str, float x, float y) const {
  FC_Draw(font_, renderer_->GetRenderer(), x, y, "%s", str);

  FC_Rect rect = FC_GetBounds(font_, x, y, FC_ALIGN_LEFT,
                              FC_MakeScale(1.0f, 1.0f), "%s", str);

  return rect;
}
