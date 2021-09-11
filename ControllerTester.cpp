#include "ControllerTester.h"

HRESULT ControllerTester::Initialize() { return CXBApplication::Initialize(); }
HRESULT ControllerTester::FrameMove() {

  for (auto &it : gamepads_) {
    auto &gamepad = *it.second;
    if (gamepad.Poll()) {
      DbgPrint("Input from player %d", gamepad.GetPlayerIndex());
    }
  }

  return CXBApplication::FrameMove();
}
HRESULT ControllerTester::Render() { return CXBApplication::Render(); }
HRESULT ControllerTester::Cleanup() { return CXBApplication::Cleanup(); }
