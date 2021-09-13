#ifndef _XB_APPLICATION_H__
#define _XB_APPLICATION_H__

#include <hal/video.h>
#include <windows.h>

#include <map>
#include <memory>
#include <string>

#include "XBGamepad.h"
#include "XBRenderer.h"

class CXBApplication {
 public:
  static const int AnyHeight = 0x7FFF;
  static const int AnyWidth = 0x7FFF;
  typedef enum {
    BPP_15 = 15,
    BPP_16 = 16,
    BPP_32 = 32,
  } ColorDepth;

 public:
  HRESULT Create();
  [[noreturn]] INT Run();
  VOID Destroy();

  static BOOL SetBestVideoMode(ColorDepth bpp = BPP_32,
                               int max_height = AnyHeight,
                               int max_width = AnyWidth);

 protected:
  virtual HRESULT Initialize() { return S_OK; }
  virtual HRESULT FrameMove() { return S_OK; }
  virtual HRESULT Render() { return S_OK; }
  virtual HRESULT Cleanup() { return S_OK; }

 private:
  void OnControllerAdded_(const SDL_ControllerDeviceEvent &event);
  void OnControllerRemapped_(const SDL_ControllerDeviceEvent &event);
  void OnControllerRemoved_(const SDL_ControllerDeviceEvent &event);

 protected:
  BOOL vsync_{TRUE};
  std::string application_name_;
  XBRenderer renderer_;

  // Region of the screen that is actually viewable.
  SDL_Rect safe_area_{0, 0, 0, 0};

  std::map<SDL_JoystickID, std::shared_ptr<CXBGamepad>> gamepads_;
};

#endif
