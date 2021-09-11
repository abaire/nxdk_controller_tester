#ifndef NXDK_CONTROLLER_TESTER_XBGAMEPAD_H
#define NXDK_CONTROLLER_TESTER_XBGAMEPAD_H

#include <SDL.h>
#include <windows.h>

class CXBGamepad {
 public:
  explicit CXBGamepad(SDL_GameController *handle);

  ~CXBGamepad();

  [[nodiscard]] inline BOOL is_changed() const { return state_changed_; }

  void OnControllerAxisEvent(const SDL_ControllerAxisEvent &event);
  void OnControllerButtonEvent(const SDL_ControllerButtonEvent &event);

 private:
  BOOL state_changed_;
  SDL_GameController *handle_;

  Sint16 deadzones_[SDL_CONTROLLER_AXIS_MAX]{0};
  Sint16 axes_[SDL_CONTROLLER_AXIS_MAX]{0};
  Uint8 buttons_[SDL_CONTROLLER_BUTTON_MAX]{0};
};

#endif  // NXDK_CONTROLLER_TESTER_XBGAMEPAD_H
