#ifndef NXDK_CONTROLLER_TESTER_CONTROLLERTESTER_H
#define NXDK_CONTROLLER_TESTER_CONTROLLERTESTER_H

#include "XBApplication.h"
#include "XBFont.h"

class ControllerTester : public CXBApplication {
 protected:
  HRESULT Initialize() override;
  HRESULT FrameMove() override;
  HRESULT Render() override;
  HRESULT Cleanup() override;

 private:
  CXBFont font_{};
};

#endif  // NXDK_CONTROLLER_TESTER_CONTROLLERTESTER_H
