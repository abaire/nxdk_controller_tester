#include "XBGamepad.h"

CXBGamepad::CXBGamepad(SDL_GameController *handle)
    : state_changed_(FALSE), handle_(handle) {}

CXBGamepad::~CXBGamepad() {
  if (handle_) {
    SDL_GameControllerClose(handle_);
  }
}

void CXBGamepad::OnControllerAxisEvent(const SDL_ControllerAxisEvent &event) {
  auto deadzone = deadzones_[event.axis];
  auto value = event.axis;
  if (value > -deadzone && value < deadzone) {
    value = 0;
  }

  state_changed_ |= (axes_[event.axis] != value);
  axes_[event.axis] = value;
}

void CXBGamepad::OnControllerButtonEvent(
    const SDL_ControllerButtonEvent &event) {
  state_changed_ |= (buttons_[event.button] != event.state);
  buttons_[event.button] = event.state;
}
