#include <hal/video.h>
#include <windows.h>
#include <xboxkrnl/xboxkrnl.h>

#include "ControllerTester.h"
#include "DebugOutput.h"

VOID __cdecl main() {
  DWORD launchDataType = LaunchDataPage->Header.dwLaunchDataType;
  PRINTMSG(("LaunchD data type: 0x%X", launchDataType));

  //  LaunchDataPage->LaunchData
//  XGetLaunchInfo
  // TODO: Use Dashboard resolution if available.

  ControllerTester app;
  app.SetBestVideoMode(CXBApplication::BPP_16, 720);
  if (FAILED(app.Create())) {
    return;
  }

  app.Run();
}