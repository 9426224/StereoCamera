#include "eSPDI.h"

#include <vector>
#include <string>

class Device
{
public:
    Device() : pHandleEtronDI(nullptr) {}
    ~Device();
    bool Init();

    ETRONDI_STREAM_INFO pStreamColorInfo[64], pStreamDepthInfo[64];
    void *pHandleEtronDI;
    DEVSELINFO pDevSelInfo;

    int depthResolution = 0;
    int colorResolution = 0;
    int depthOption = 11;

private:
    bool GetEtronDIDevice();

    std::vector<DEVINFORMATION> devInfo;

    int pFPS = 30;
};