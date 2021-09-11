#include "XBGamepad.h"

CXBGamepad::CXBGamepad(SDL_GameController *handle)
    : state_changed_(FALSE), handle_(handle) {}

CXBGamepad::~CXBGamepad() {
  if (handle_) {
    SDL_GameControllerClose(handle_);
  }
}

CXBGamepad::DeviceStatus CXBGamepad::Poll() {
  auto changed = state_changed_;
  state_changed_ = 0;
  return changed;
}

void CXBGamepad::SetDeadzone(SDL_GameControllerAxis axis, Sint16 value) {
  deadzones_[axis] = value;
}

void CXBGamepad::OnControllerAxisEvent(const SDL_ControllerAxisEvent &event) {
  auto deadzone = deadzones_[event.axis];
  auto value = event.value;
  if (value > -deadzone && value < deadzone) {
    value = 0;
  }

  if (axes_[event.axis] == value) {
    return;
  }

  SetAxisChanged(state_changed_, event.axis);
  axes_[event.axis] = value;
}

void CXBGamepad::OnControllerButtonEvent(
    const SDL_ControllerButtonEvent &event) {

  if (buttons_[event.button] != event.state) {
    SetButtonChanged(state_changed_, event.button);
  }
  buttons_[event.button] = event.state;
}
