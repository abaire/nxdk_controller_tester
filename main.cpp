#include <hal/video.h>
#include <hal/xbox.h>
#include <windows.h>
#include <xboxkrnl/xboxkrnl.h>

#include "ControllerTester.h"
#include "DebugOutput.h"

VOID __cdecl main() {
  ControllerTester app;

  // TODO: Use Dashboard resolution if available.
  app.SetBestVideoMode(CXBApplication::BPP_16, 720);

  if (FAILED(app.Create())) {
    XLaunchXBE(nullptr);
    return;  // Should never reach here.
  }

  app.Run();

  // Reboot
  XLaunchXBE(nullptr);
}