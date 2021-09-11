#include "XBApplication.h"

#include <SDL.h>
#include <nxdk/mount.h>
#include <nxdk/path.h>

#include "DebugOutput.h"

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

  renderer_.Init(application_name_.c_str());

  int init_status = SDL_Init(SDL_INIT_GAMECONTROLLER);
  if (init_status) {
    DbgPrint("Failed to initialize GAMECONTROLLER: %s", SDL_GetError());
    return E_FAIL;
  }

  HRESULT status = Initialize();
  if (!SUCCEEDED(status)) {
    return status;
  }

  return S_OK;
}

[[noreturn]] INT CXBApplication::Run() {
  while (true) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_CONTROLLERDEVICEADDED:
          OnControllerAdded_(event);
          break;

        case SDL_CONTROLLERDEVICEREMAPPED:
          OnControllerRemapped_(event);
          break;

        case SDL_CONTROLLERDEVICEREMOVED:
          OnControllerRemoved_(event);
          break;

        case SDL_CONTROLLERAXISMOTION:
        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP:
          // TODO: Handle.
          break;

        case SDL_WINDOWEVENT:
          // Ignore.
          // Fallthrough

        case SDL_JOYDEVICEADDED:
          // Ignore - handled by SDL_CONTROLLERDEVICEADDED.
          // Fallthrough

        case SDL_JOYDEVICEREMOVED:
          // Ignore - handled by SDL_CONTROLLERDEVICEREMOVED.
          // Fallthrough

        case SDL_JOYAXISMOTION:
        case SDL_JOYBALLMOTION:
        case SDL_JOYHATMOTION:
          // Ignore - handled by SDL_CONTROLLERAXISMOTION.
          // Fallthrough

        case SDL_JOYBUTTONDOWN:
          // Ignore - handled by SDL_CONTROLLERBUTTONDOWN.
          // Fallthrough

        case SDL_JOYBUTTONUP:
          // Ignore - handled by SDL_CONTROLLERBUTTONUP.
          break;

        default:
          DbgPrint("Ignoring SDL event of type %d [0x%x]", event.type,
                   event.type);
      }
    }

    // Parse input

    FrameMove();
    Render();

    renderer_.Flip();
  }
}

VOID CXBApplication::Destroy() {
  Cleanup();

  // Destroy input devices
  for (auto &it : gamepads_) {
    SDL_GameControllerClose(it);
  }
  gamepads_.clear();
}

void CXBApplication::OnControllerAdded_(const SDL_Event &event) {
  SDL_GameController *controller = SDL_GameControllerOpen(event.cdevice.which);
  if (!controller) {
    ErrorPrintSDLError("Failed to handle controller add event.");
    return;
  }

  gamepads_.push_back(controller);
}

void CXBApplication::OnControllerRemapped_(const SDL_Event &event) {
  DbgPrint("Ignoring SDL_CONTROLLERDEVICEREMAPPED event for device %d",
           event.cdevice.which);
}

void CXBApplication::OnControllerRemoved_(const SDL_Event &event) {
  SDL_GameController *controller =
      SDL_GameControllerFromInstanceID(event.cdevice.which);

  auto _new_end = std::remove(gamepads_.begin(), gamepads_.end(), controller);
  SDL_GameControllerClose(controller);
}
