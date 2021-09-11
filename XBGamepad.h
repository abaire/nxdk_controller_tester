#ifndef NXDK_CONTROLLER_TESTER_XBGAMEPAD_H
#define NXDK_CONTROLLER_TESTER_XBGAMEPAD_H

#include <SDL.h>
#include <windows.h>

// Returns a bitvector of the axes that have changed since the last poll.
#define ChangedAxes(device_status) ((device_status >> 16) & 0xFFFF)

// Returns a bitvector of the buttons that have changed since the last poll.
#define ChangedButtons(device_status) (device_status & 0xFFFF)

#define SetAxisChanged(device_status, axis) \
  (device_status |= (1 << (static_cast<DWORD>(axis) + 16)))
#define GetAxisChanged(device_status, axis) \
  ((device_status & (1 << (static_cast<DWORD>(axis) + 16))) != 0)

#define SetButtonChanged(device_status, button) \
  (device_status |= (1 << static_cast<DWORD>(button)))
#define GetButtonChanged(device_status, button) \
  ((device_status & (1 << static_cast<DWORD>(button))) != 0)

class CXBGamepad {
 public:
  typedef DWORD DeviceStatus;

 public:
  explicit CXBGamepad(SDL_GameController *handle);

  ~CXBGamepad();

  // Returns whether or not the device is changed and clears the changed state.
  // The result is a bitvector containing the buttons and axes, which can be
  // extracted via the ChangedAxes and ChangedButtons macros.
  DeviceStatus Poll();

  [[nodiscard]] inline BOOL IsChanged() const { return state_changed_ != 0; }

  [[nodiscard]] int GetPlayerIndex() const {
    return SDL_GameControllerGetPlayerIndex(handle_);
  }

  void SetDeadzone(SDL_GameControllerAxis axis, Sint16 value);
  [[nodiscard]] inline Sint16 GetDeadzone(SDL_GameControllerAxis axis) const {
    return deadzones_[axis];
  }

  [[nodiscard]] inline Sint16 GetAxis(SDL_GameControllerAxis axis) const {
    return axes_[axis];
  }

  [[nodiscard]] inline Uint8 GetButton(SDL_GameControllerButton button) const {
    return buttons_[button];
  }

  inline BOOL Rumble(Uint16 left_low_freq_rumble, Uint16 right_high_freq_rumble,
                     Uint32 duration_ms) const {
    return SDL_GameControllerRumble(handle_, left_low_freq_rumble,
                                    right_high_freq_rumble, duration_ms) == 0;
  }

  void OnControllerAxisEvent(const SDL_ControllerAxisEvent &event);
  void OnControllerButtonEvent(const SDL_ControllerButtonEvent &event);

 private:
  DeviceStatus state_changed_;
  SDL_GameController *handle_;

  Sint16 deadzones_[SDL_CONTROLLER_AXIS_MAX]{0};
  Sint16 axes_[SDL_CONTROLLER_AXIS_MAX]{0};
  Uint8 buttons_[SDL_CONTROLLER_BUTTON_MAX]{0};
};

#endif  // NXDK_CONTROLLER_TESTER_XBGAMEPAD_H
