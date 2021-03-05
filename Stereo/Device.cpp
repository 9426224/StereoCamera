#include "Device.h"

Device::~Device()
{
    if (pHandleEtronDI != nullptr)
    {
        EtronDI_CloseDevice(pHandleEtronDI, &pDevSelInfo);

        EtronDI_Release(&pHandleEtronDI);

        pHandleEtronDI = nullptr;
    }
}

bool Device::Init()
{
    if (!GetEtronDIDevice())
    {
        return false;
    }

    EtronDI_Init(&pHandleEtronDI, false);

    EtronDI_GetDeviceResolutionList(pHandleEtronDI, &pDevSelInfo, 64, pStreamColorInfo, 64, pStreamDepthInfo);

    EtronDI_SetDepthDataType(pHandleEtronDI, &pDevSelInfo, ETronDI_DEPTH_DATA_11_BITS);

    if (EtronDI_OpenDevice2(pHandleEtronDI, &pDevSelInfo,
                            pStreamColorInfo[0].nWidth, pStreamColorInfo[0].nHeight,
                            pStreamColorInfo[0].bFormatMJPG, pStreamDepthInfo[0].nWidth,
                            pStreamDepthInfo[0].nHeight, DEPTH_IMG_NON_TRANSFER,
                            false, NULL, &pFPS, IMAGE_SN_NONSYNC) != ETronDI_OK)
    {
        return false;
    }

    return true;
}

bool Device::GetEtronDIDevice()
{
    if (EtronDI_Init(&pHandleEtronDI, false) < 0)
    {
        EtronDI_Release(&pHandleEtronDI);
        return false;
    }

    if (pHandleEtronDI != nullptr)
    {
        int deviceCount = EtronDI_GetDeviceNumber(pHandleEtronDI);

        if (deviceCount > 0)
        {
            devInfo.clear();
            for (int i = 0; i < deviceCount; i++)
            {
                DEVSELINFO devSelInfo;
                devSelInfo.index = i;
                DEVINFORMATION devInformation;
                EtronDI_GetDeviceInfo(pHandleEtronDI, &devSelInfo, &devInformation);
                devInfo.push_back(devInformation);
            }
            EtronDI_Release(&pHandleEtronDI);
        }
    }

    pDevSelInfo.index = devInfo.size() - 1;

    return true;
}