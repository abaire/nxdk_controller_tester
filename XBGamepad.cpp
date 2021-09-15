#include "XBGamepad.h"

#define SetAxisChanged(device_status, axis) \
  (device_status |= (1 << (static_cast<DWORD>(axis) + 16)))

#define SetButtonChanged(device_status, button) \
  (device_status |= (1 << static_cast<DWORD>(button)))

void CXBGamepad::LinkDevice(SDL_GameController *handle) {
  id_ = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(handle));
  handle_ = handle;
  connected_ = TRUE;

  axes_[SDL_CONTROLLER_AXIS_LEFTX] =
      SDL_GameControllerGetAxis(handle_, SDL_CONTROLLER_AXIS_LEFTX);
  axes_[SDL_CONTROLLER_AXIS_LEFTY] =
      SDL_GameControllerGetAxis(handle_, SDL_CONTROLLER_AXIS_LEFTY);
  axes_[SDL_CONTROLLER_AXIS_RIGHTX] =
      SDL_GameControllerGetAxis(handle_, SDL_CONTROLLER_AXIS_RIGHTX);
  axes_[SDL_CONTROLLER_AXIS_RIGHTY] =
      SDL_GameControllerGetAxis(handle_, SDL_CONTROLLER_AXIS_RIGHTY);

  axes_[SDL_CONTROLLER_AXIS_TRIGGERLEFT] =
      SDL_GameControllerGetAxis(handle_, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
  axes_[SDL_CONTROLLER_AXIS_TRIGGERRIGHT] =
      SDL_GameControllerGetAxis(handle_, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

  buttons_[SDL_CONTROLLER_BUTTON_DPAD_UP] =
      SDL_GameControllerGetButton(handle_, SDL_CONTROLLER_BUTTON_DPAD_UP);
  buttons_[SDL_CONTROLLER_BUTTON_DPAD_DOWN] =
      SDL_GameControllerGetButton(handle_, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
  buttons_[SDL_CONTROLLER_BUTTON_DPAD_LEFT] =
      SDL_GameControllerGetButton(handle_, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
  buttons_[SDL_CONTROLLER_BUTTON_DPAD_RIGHT] =
      SDL_GameControllerGetButton(handle_, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);

  buttons_[SDL_CONTROLLER_BUTTON_A] =
      SDL_GameControllerGetButton(handle_, SDL_CONTROLLER_BUTTON_A);
  buttons_[SDL_CONTROLLER_BUTTON_B] =
      SDL_GameControllerGetButton(handle_, SDL_CONTROLLER_BUTTON_B);
  buttons_[SDL_CONTROLLER_BUTTON_X] =
      SDL_GameControllerGetButton(handle_, SDL_CONTROLLER_BUTTON_X);
  buttons_[SDL_CONTROLLER_BUTTON_Y] =
      SDL_GameControllerGetButton(handle_, SDL_CONTROLLER_BUTTON_Y);

  buttons_[SDL_CONTROLLER_BUTTON_WHITE] =
      SDL_GameControllerGetButton(handle_, SDL_CONTROLLER_BUTTON_WHITE);
  buttons_[SDL_CONTROLLER_BUTTON_BLACK] =
      SDL_GameControllerGetButton(handle_, SDL_CONTROLLER_BUTTON_BLACK);
  buttons_[SDL_CONTROLLER_BUTTON_LEFTSTICK] =
      SDL_GameControllerGetButton(handle_, SDL_CONTROLLER_BUTTON_LEFTSTICK);
  buttons_[SDL_CONTROLLER_BUTTON_RIGHTSTICK] =
      SDL_GameControllerGetButton(handle_, SDL_CONTROLLER_BUTTON_RIGHTSTICK);

  buttons_[SDL_CONTROLLER_BUTTON_BACK] =
      SDL_GameControllerGetButton(handle_, SDL_CONTROLLER_BUTTON_BACK);
  buttons_[SDL_CONTROLLER_BUTTON_START] =
      SDL_GameControllerGetButton(handle_, SDL_CONTROLLER_BUTTON_START);

  state_changed_ = 0xFFFFFFFF;
}

void CXBGamepad::UnlinkDevice() {
  connected_ = FALSE;
  state_changed_ = 0;

  if (handle_) {
    SDL_GameControllerClose(handle_);
    handle_ = nullptr;
  }

  id_ = -1;  // Flagged in SDL_joystick.h as invalid ID.
}

Uint32 CXBGamepad::Poll() {
  if (!connected_) {
    return 0;
  }

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
