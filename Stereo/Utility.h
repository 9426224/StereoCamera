#include "windows.h"


void HSV_to_RGB(double H, double S, double V, double& R, double& G, double& B);

void SetBaseGrayPaletteD11(RGBQUAD* pGrayPaletteD11);
void SetBaseGrayPaletteZ14(RGBQUAD* pGrayPaletteZ14);

void UpdateD11DisplayImage_DIB24(RGBQUAD* pColorPaletteD11, BYTE* pDepthD11, BYTE* pDepthDIB24, int cx, int cy);
void UpdateZ14DisplayImage_DIB24(RGBQUAD* pColorPaletteZ14, BYTE* pDepthZ14, BYTE* pDepthDIB24, int cx, int cy);

void DmColorMode11(RGBQUAD* pallete, int mode);
void DmColorMode14(RGBQUAD* pallete, int mode);