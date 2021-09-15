#include "XBApplication.h"

#include <SDL.h>
#include <nxdk/mount.h>
#include <nxdk/path.h>

#include "DebugOutput.h"

// Mounts the path containing this xbe as "A:".
static BOOL MountDiskA() {
  if (nxIsDriveMounted('A')) {
    PRINTMSG(("A: already mounted!"));
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
      PRINTMSG(("Failed to mount A:"));
      return E_FAIL;
    }
  }

  renderer_.Init(application_name_.c_str());

  float overscan_x = static_cast<float>(renderer_.GetWidth()) * 0.05f;
  float overscan_y = static_cast<float>(renderer_.GetHeight()) * 0.05f;

  safe_area_.x = static_cast<int>(overscan_x);
  safe_area_.y = static_cast<int>(overscan_y);
  safe_area_.w = renderer_.GetWidth() - safe_area_.x;
  safe_area_.h = renderer_.GetHeight() - safe_area_.y;

  int init_status = SDL_Init(SDL_INIT_GAMECONTROLLER);
  if (init_status) {
    PRINTMSG(("Failed to initialize GAMECONTROLLER: %s", SDL_GetError()));
    return E_FAIL;
  }

  HRESULT status = Initialize();
  if (!SUCCEEDED(status)) {
    return status;
  }

  return S_OK;
}

VOID CXBApplication::Destroy() {
  Cleanup();

  // Destroy input devices
  for (auto &gamepad : gamepads_) {
    gamepad.UnlinkDevice();
  }

  SDL_Quit();
}

[[noreturn]] INT CXBApplication::Run() {
  while (true) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_CONTROLLERDEVICEADDED:
          OnControllerAdded_(event.cdevice);
          break;

        case SDL_CONTROLLERDEVICEREMAPPED:
          OnControllerRemapped_(event.cdevice);
          break;

        case SDL_CONTROLLERDEVICEREMOVED:
          OnControllerRemoved_(event.cdevice);
          break;

        case SDL_CONTROLLERAXISMOTION: {
          for (auto &gamepad : gamepads_) {
            if (gamepad.GetID() == event.caxis.which) {
              gamepad.OnControllerAxisEvent(event.caxis);
            }
            break;
          }
          assert(!"Gamepad event from unknown device");
        } break;

        case SDL_CONTROLLERBUTTONDOWN:
          // Fallthrough
        case SDL_CONTROLLERBUTTONUP: {
          for (auto &gamepad : gamepads_) {
            if (gamepad.GetID() == event.cbutton.which) {
              gamepad.OnControllerButtonEvent(event.cbutton);
            }
            break;
          }
          assert(!"Gamepad event from unknown device");
        } break;

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
          PRINTMSG(
              ("Ignoring SDL event of type %d [0x%x]", event.type, event.type));
      }
    }

    FrameMove();

    Render();

    renderer_.Flip(vsync_);
  }
}

void CXBApplication::OnControllerAdded_(
    const SDL_ControllerDeviceEvent &event) {
  SDL_GameController *controller = SDL_GameControllerOpen(event.which);
  if (!controller) {
    ErrorPrintSDLError("Failed to handle controller add event.");
    return;
  }

  auto player_index = SDL_GameControllerGetPlayerIndex(controller);

  if (player_index < 0 || player_index >= MAX_NUM_GAMEPADS) {
    ErrorPrintSDLError("Player index %d is out of range (max %d).",
                       player_index, MAX_NUM_GAMEPADS);
    return;
  }

  gamepads_[player_index].LinkDevice(controller);
}

void CXBApplication::OnControllerRemapped_(
    const SDL_ControllerDeviceEvent &event) {
  PRINTMSG(("Ignoring SDL_CONTROLLERDEVICEREMAPPED event for device %d",
            event.which));
}

void CXBApplication::OnControllerRemoved_(
    const SDL_ControllerDeviceEvent &event) {
  SDL_GameController *controller =
      SDL_GameControllerFromInstanceID(event.which);

  for (auto &gamepad : gamepads_) {
    if (gamepad.GetID() == event.which) {
      gamepad.UnlinkDevice();
      return;
    }
  }

  // Handle case where the controller was never mapped for some reason.
  SDL_GameControllerClose(controller);
}

BOOL CXBApplication::SetBestVideoMode(CXBApplication::ColorDepth bpp,
                                      int max_height, int max_width) {
  void *vmode_context = nullptr;
  VIDEO_MODE vmode;
  VIDEO_MODE best_mode;
  best_mode.height = -1;

  while (XVideoListModes(&vmode, bpp, REFRESH_DEFAULT, &vmode_context)) {
    if (vmode.height > max_height || vmode.width > max_width) {
      continue;
    }

    if (vmode.height > best_mode.height ||
        (vmode.width > best_mode.width && vmode.height == best_mode.height)) {
      best_mode = vmode;
    }
  }

  if (best_mode.height <= 0) {
    return FALSE;
  }

  BOOL status = XVideoSetMode(best_mode.width, best_mode.height, best_mode.bpp,
                              best_mode.refresh);

  return status;
}
