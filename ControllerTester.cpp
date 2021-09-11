#include "ControllerTester.h"

HRESULT ControllerTester::Initialize() { return CXBApplication::Initialize(); }
HRESULT ControllerTester::FrameMove() {

  for (auto &it : gamepads_) {
    auto &gamepad = *it.second;
    if (gamepad.Poll()) {
      DbgPrint("Input from player %d", gamepad.GetPlayerIndex());
    }
  }

  return S_OK;
}
HRESULT ControllerTester::Render() {

  renderer_.SetDrawColor(0x3C, 0x00, 0x3C);
  renderer_.Clear();

  return S_OK;
}

HRESULT ControllerTester::Cleanup() { return CXBApplication::Cleanup(); }
