#ifndef NXDK_CONTROLLER_TESTER_CONTROLLERTESTER_H
#define NXDK_CONTROLLER_TESTER_CONTROLLERTESTER_H

#include "XBApplication.h"

class ControllerTester : public CXBApplication {
 protected:
  HRESULT Initialize() override;
  HRESULT FrameMove() override;
  HRESULT Render() override;
  HRESULT Cleanup() override;
};

#endif  // NXDK_CONTROLLER_TESTER_CONTROLLERTESTER_H
