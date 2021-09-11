#ifndef _XB_APPLICATION_H__
#define _XB_APPLICATION_H__

#include <windows.h>

#include <memory>
#include <string>
#include <vector>

#include "XBRenderer.h"

class CXBApplication {
 public:
  HRESULT Create();
  [[noreturn]] INT Run();
  VOID Destroy();

 protected:
  virtual HRESULT Initialize() { return S_OK; }
  virtual HRESULT FrameMove() { return S_OK; }
  virtual HRESULT Render() { return S_OK; }
  virtual HRESULT Cleanup() { return S_OK; }

 private:
  void OnControllerAdded_(const SDL_Event &event);
  void OnControllerRemapped_(const SDL_Event &event);
  void OnControllerRemoved_(const SDL_Event &event);

 protected:
  std::string application_name_;
  XBRenderer renderer_;

  std::vector<SDL_GameController *> gamepads_;
};

#endif
