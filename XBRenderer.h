#ifndef _XBRENDERER_H__
#define _XBRENDERER_H__

#include <SDL.h>
#include <SDL_image.h>
#include <xboxkrnl/xboxkrnl.h>

#include <vector>

class XBRenderer {
 public:
  XBRenderer();

  ~XBRenderer();

  int Init(LPCSTR title);

  int Clear();

  void Flip(BOOL vsync);

  SDL_Renderer *GetRenderer() { return renderer_; }

  int GetWidth() const { return width_; }

  int GetHeight() const { return height_; }

  int SetDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);

  int FillRect(const SDL_Rect &rect);

  void DrawTexture(SDL_Texture *tex, SDL_Rect &src, SDL_Rect &dst);

  void DrawTexture(SDL_Texture *tex, SDL_Rect &dst);

  void DrawTexture(SDL_Texture *tex, int x, int y);

  void DrawBackgroundTexture(SDL_Texture *tex);

  void BlitSurface(SDL_Surface *bg, SDL_Surface *fg, int offset);

  inline SDL_Texture *LoadImage(LPCSTR path) {
    return IMG_LoadTexture(renderer_, path);
  }

 private:
  void UpdateFromCurrentVideoMode();

 private:
  SDL_Renderer *renderer_ = nullptr;
  SDL_Window *window_ = nullptr;
  Uint32 render_flags_ = 0;
  Uint32 window_flags_ = 0;

  int height_ = 0;
  int width_ = 0;
  int overscan_comp_x_ = 0;
  int overscan_comp_y_ = 0;
  size_t menu_item_count_ = 0;
  size_t lower_half_ = 0;
  size_t upper_half_ = 0;
};

#endif  // _XBRENDERER_H__
