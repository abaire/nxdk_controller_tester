#include "ControllerTester.h"

HRESULT ControllerTester::Initialize() {
  HRESULT status = font_.Create(&renderer_, R"(A:\resources\fonts\X360.ttf)");
  assert(SUCCEEDED(status) && "Failed to load font_.");

  return S_OK;
}

HRESULT ControllerTester::FrameMove() {
  for (auto &it : gamepads_) {
    auto &gamepad = *it.second;
    if (gamepad.Poll()) {
      int player_index = gamepad.GetPlayerIndex();
      DbgPrint("Input from player %d", player_index);
    }
  }

  return S_OK;
}
HRESULT ControllerTester::Render() {
  renderer_.SetDrawColor(0x3C, 0x00, 0x3C);
  renderer_.Clear();

  int half_width = safe_area_.w >> 1;
  int half_height = safe_area_.h >> 1;

  SDL_Rect render_boxes[4] = {
      {safe_area_.x, safe_area_.y, half_width, half_height},
      {safe_area_.x + half_width, safe_area_.y, half_width, half_height},
      {safe_area_.x, safe_area_.y + half_height, half_width, half_height},
      {safe_area_.x + half_width, safe_area_.y + half_height, half_width,
       half_height}};

  for (auto &it : gamepads_) {
    auto &gamepad = *it.second;
    int player_index = gamepad.GetPlayerIndex();
    if (player_index > 3) {
      DbgPrint("Found unexpected player index %d", player_index);
      font_.DrawFmt(safe_area_.x, safe_area_.y + safe_area_.h,
                    "Found unexpected player index %d", player_index);
      continue;
    }

    const SDL_Rect &box = render_boxes[player_index];
    int y = box.y;
    SDL_Rect line = font_.DrawFmt(box.x, y, "Gamepad %d", player_index + 1);
    y += line.h;

    line = font_.DrawFmt(box.x, y, "LStick: X: %d  Y: %d",
                         gamepad.GetAxis(SDL_CONTROLLER_AXIS_LEFTX),
                         gamepad.GetAxis(SDL_CONTROLLER_AXIS_LEFTY));
    y += line.h;

    line = font_.DrawFmt(box.x, y, "RStick: X: %d  Y: %d",
                         gamepad.GetAxis(SDL_CONTROLLER_AXIS_RIGHTX),
                         gamepad.GetAxis(SDL_CONTROLLER_AXIS_RIGHTY));
    y += line.h;

    line = font_.DrawFmt(box.x, y, "Triggers: L: %d  R: %d",
                         gamepad.GetAxis(SDL_CONTROLLER_AXIS_TRIGGERLEFT),
                         gamepad.GetAxis(SDL_CONTROLLER_AXIS_TRIGGERRIGHT));
    y += line.h;

    line = font_.DrawFmt(box.x, y, "DPAD: U: %d  D: %d  L: %d  R: %d",
                         gamepad.GetButton(SDL_CONTROLLER_BUTTON_DPAD_UP),
                         gamepad.GetButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN),
                         gamepad.GetButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT),
                         gamepad.GetButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT));
    y += line.h;

    line = font_.DrawFmt(box.x, y, "A: %d  B: %d  X: %d  Y: %d",
                         gamepad.GetButton(SDL_CONTROLLER_BUTTON_A),
                         gamepad.GetButton(SDL_CONTROLLER_BUTTON_B),
                         gamepad.GetButton(SDL_CONTROLLER_BUTTON_X),
                         gamepad.GetButton(SDL_CONTROLLER_BUTTON_Y));
    y += line.h;

    line = font_.DrawFmt(box.x, y, "White: %d  Black: %d",
                         gamepad.GetButton(SDL_CONTROLLER_BUTTON_LEFTSHOULDER),
                         gamepad.GetButton(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER));
    y += line.h;

    line = font_.DrawFmt(box.x, y, "Back: %d  Start: %d",
                         gamepad.GetButton(SDL_CONTROLLER_BUTTON_BACK),
                         gamepad.GetButton(SDL_CONTROLLER_BUTTON_START));
    y += line.h;

  }

  return S_OK;
}

HRESULT ControllerTester::Cleanup() {
  HRESULT status = font_.Destroy();
  return status;
}
