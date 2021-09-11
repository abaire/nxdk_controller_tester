#ifndef _XB_APPLICATION_H__
#define _XB_APPLICATION_H__

#include <string>
#include <windows.h>

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

 protected:
  std::string application_name_;
  XBRenderer renderer_;
};

#endif
