#include <hal/video.h>
#include <windows.h>
#include <xboxkrnl/xboxkrnl.h>

#include "ControllerTester.h"

VOID __cdecl main() {
  // TODO: Use Dashboard resolution if available.
  XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

  ControllerTester app;
  if (FAILED(app.Create())) {
    return;
  }

  app.Run();
}