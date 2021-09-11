#include <nxdk/mount.h>
#include <nxdk/path.h>

#include "XBApplication.h"

// Mounts the path containing this xbe as "A:".
static BOOL MountDiskA() {
  if (nxIsDriveMounted('A')) {
    DbgPrint("A: already mounted!");
    return FALSE;
  }

  CHAR targetPath[MAX_PATH];
  nxGetCurrentXbeNtPath(targetPath);

  LPSTR filenameStr;
  filenameStr = strrchr(targetPath, '\\');
  assert(filenameStr != NULL);
  *(filenameStr + 1) = '\0';

  BOOL status = nxMountDrive('A', targetPath);
  return status;
}


HRESULT CXBApplication::Create() {
  if (!nxIsDriveMounted('A')) {
    if (!MountDiskA()) {
      DbgPrint("Failed to mount A:");
      return E_FAIL;
    }
  }
  // Initialize the renderer
  renderer_.Init(application_name_.c_str());

  // Create input devices

  HRESULT status = Initialize();
  if (!SUCCEEDED(status)) {
    return status;
  }

  return S_OK;
}

[[noreturn]] INT CXBApplication::Run() {
  while (true) {
    // Parse input

    FrameMove();
    Render();

    renderer_.Flip();
  }
}

VOID CXBApplication::Destroy() {
  Cleanup();

  // Destroy input devices
}