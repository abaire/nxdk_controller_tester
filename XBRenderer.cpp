#include "XBRenderer.h"

#include <SDL_image.h>
#include <hal/debug.h>
#include <hal/video.h>

#include <algorithm>
#include <cmath>

// One line of text with the default font_ is 31 pixels high.
// FIXME: Should probably be dynamic and dependent of font_ settings.
static const unsigned int FONT_TEX_SIZE = 31;

void XBRenderer::UpdateFromCurrentVideoMode() {
  VIDEO_MODE xmode = XVideoGetMode();
  height_ = xmode.height;
  width_ = xmode.width;
}

XBRenderer::XBRenderer() {
  UpdateFromCurrentVideoMode();
  window_flags_ = SDL_WINDOW_SHOWN;

  overscan_comp_x_ = static_cast<int>(width_ * 0.075);
  overscan_comp_y_ = static_cast<int>(height_ * 0.075);
  menu_item_count_ = (height_ - (overscan_comp_y_ * 2)) / FONT_TEX_SIZE;
  lower_half_ = menu_item_count_ / 2;
  upper_half_ = ceil(menu_item_count_ / 2.0);
}

XBRenderer::~XBRenderer() {
  if (renderer_) {
    SDL_DestroyRenderer(renderer_);
  }
  if (window_) {
    SDL_DestroyWindow(window_);
  }
}

int XBRenderer::Init(LPCSTR title) {
  if (!width_ || !height_) {
    UpdateFromCurrentVideoMode();
    assert(width_ > 0);
    assert(height_ > 0);
    if (!width_ || !height_) {
      return -1;
    }
  }

  window_ =
      SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       width_, height_, window_flags_);
  if (window_ == nullptr) {
    return 1;
  }

  renderer_ = SDL_CreateRenderer(window_, -1, render_flags_);
  if (renderer_ == nullptr) {
    return 2;
  }

  SDL_SetRenderDrawBlendMode(GetRenderer(), SDL_BLENDMODE_BLEND);
  SetDrawColor(0x00, 0x00, 0x00);
  Clear();

  return 0;
}

int XBRenderer::Clear() {
  int ret = SDL_RenderClear(renderer_);
  return ret;
}

void XBRenderer::Flip(BOOL vsync) {
#ifdef NXDK
  if (vsync) {
    XVideoWaitForVBlank();
  }
#endif

  SDL_RenderPresent(renderer_);
}

int XBRenderer::SetDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  return SDL_SetRenderDrawColor(renderer_, r, g, b, a);
}

int XBRenderer::FillRect(const SDL_Rect &rect) {
  return SDL_RenderFillRect(renderer_, &rect);
}

void XBRenderer::DrawTexture(SDL_Texture *tex, SDL_Rect &src, SDL_Rect &dst) {
  SDL_RenderCopy(renderer_, tex, &src, &dst);
}

void XBRenderer::DrawTexture(SDL_Texture *tex, SDL_Rect &dst) {
  SDL_RenderCopy(renderer_, tex, nullptr, &dst);
}

void XBRenderer::DrawTexture(SDL_Texture *tex, int x, int y) {
  SDL_Rect dst = {x, y, 0, 0};
  SDL_QueryTexture(tex, nullptr, nullptr, &dst.w, &dst.h);
  DrawTexture(tex, dst);
}

void XBRenderer::BlitSurface(SDL_Surface *bg, SDL_Surface *fg, int offset) {
  SDL_Rect dst = {offset, offset, fg->w, fg->h};
  SDL_SetSurfaceBlendMode(fg, SDL_BLENDMODE_BLEND);
  SDL_BlitSurface(fg, NULL, bg, &dst);
}
void XBRenderer::DrawBackgroundTexture(SDL_Texture *tex) {
  DrawTexture(tex, 0, 0);
}
