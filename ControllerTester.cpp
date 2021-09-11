#include "ControllerTester.h"

HRESULT ControllerTester::Initialize() { return CXBApplication::Initialize(); }
HRESULT ControllerTester::FrameMove() { return CXBApplication::FrameMove(); }
HRESULT ControllerTester::Render() { return CXBApplication::Render(); }
HRESULT ControllerTester::Cleanup() { return CXBApplication::Cleanup(); }
