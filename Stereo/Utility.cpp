#include "math.h"

#include "Utility.h"

/// <summary>
/// HSV色彩空间转换为RGB色彩空间
/// </summary>
/// <param name="H">色调</param>
/// <param name="S">饱和度</param>
/// <param name="V">明度</param>
/// <param name="R">红色</param>
/// <param name="G">绿色</param>
/// <param name="B">蓝色</param>
void HSV_to_RGB(double H, double S, double V, double& R, double& G, double& B)
{
	double nMax, nMin;
	double fDet;
	//
	while (H < 0.0) H += 360.0;
	while (H >= 360.0) H -= 360.0;
	H /= 60.0;
	if (V < 0.0) V = 0.0;
	if (V > 1.0) V = 1.0;
	V *= 255.0;
	if (S < 0.0) S = 0.0;
	if (S > 1.0) S = 1.0;
	//
	if (V == 0.0) {
		R = G = B = 0;
	}
	else {
		fDet = S * V;
		nMax = (V);
		nMin = (V - fDet);
		if (H <= 1.0) { //R>=G>=B, H=(G-B)/fDet
			R = nMax;
			B = nMin;
			G = (H * fDet + B);
		}
		else if (H <= 2.0) { //G>=R>=B, H=2+(B-R)/fDet
			G = nMax;
			B = nMin;
			R = ((2.0 - H) * fDet + B);
		}
		else if (H <= 3.0) { //G>=B>=R, H=2+(B-R)/fDet
			G = nMax;
			R = nMin;
			B = ((H - 2.0) * fDet + R);
		}
		else if (H <= 4.0) { //B>=G>=R, H=4+(R-G)/fDet
			B = nMax;
			R = nMin;
			G = ((4.0 - H) * fDet + R);
		}
		else if (H <= 5.0) { //B>=R>=G, H=4+(R-G)/fDet
			B = nMax;
			G = nMin;
			R = ((H - 4.0) * fDet + G);
		}
		else { // if(H<6.0) //R>=B>=G, H=(G-B)/fDet+6
			R = nMax;
			G = nMin;
			B = ((6.0 - H) * fDet + G);
		}
	}
}

void SetBaseGrayPaletteD11(RGBQUAD* pGrayPaletteD11)
{
	int i;
	double R, G, B;
	//
	for (i = 0; i < 2048; i++) {
		HSV_to_RGB((2047.0 - i) / 8, 1.0, 1.0, R, G, B);
		pGrayPaletteD11[i].rgbBlue = (BYTE)B;
		pGrayPaletteD11[i].rgbGreen = (BYTE)B;
		pGrayPaletteD11[i].rgbRed = (BYTE)B;
		pGrayPaletteD11[i].rgbReserved = 0;
	}
	{
		i = 0;
		pGrayPaletteD11[i].rgbBlue = (BYTE)0;
		pGrayPaletteD11[i].rgbGreen = (BYTE)0;
		pGrayPaletteD11[i].rgbRed = (BYTE)0;
		pGrayPaletteD11[i].rgbReserved = 0;
	}
	{
		i = 2047;
		pGrayPaletteD11[i].rgbBlue = (BYTE)255;
		pGrayPaletteD11[i].rgbGreen = (BYTE)255;
		pGrayPaletteD11[i].rgbRed = (BYTE)255;
		pGrayPaletteD11[i].rgbReserved = 0;
	}
}

void SetBaseGrayPaletteZ14(RGBQUAD* pGrayPaletteZ14)
{
	int i;
	double R, G, B;
	double fx, fy;
	//
	double fCV = 180;
	int nCenter = 1500;
	double r1 = 0.35;
	double r2 = 0.55;
	//
	for (i = 1; i < 16384; i++) {
		if (i == nCenter) {
			fy = fCV;
		}
		else if (i < nCenter) {
			fx = (double)(nCenter - i) / nCenter;
			fy = fCV - pow(fx, r1) * fCV;
		}
		else {
			fx = (double)(i - nCenter) / (16384 - nCenter);
			fy = fCV + pow(fx, r2) * (256 - fCV);
		}
		HSV_to_RGB(fy, 1.0, 1.0, R, G, B);
		pGrayPaletteZ14[i].rgbBlue = (BYTE)B;
		pGrayPaletteZ14[i].rgbGreen = (BYTE)B;
		pGrayPaletteZ14[i].rgbRed = (BYTE)B;
		pGrayPaletteZ14[i].rgbReserved = 0;
	}
	{
		i = 0;
		pGrayPaletteZ14[i].rgbBlue = (BYTE)0;
		pGrayPaletteZ14[i].rgbGreen = (BYTE)0;
		pGrayPaletteZ14[i].rgbRed = (BYTE)0;
		pGrayPaletteZ14[i].rgbReserved = 0;
	}
	{
		i = 16383;
		pGrayPaletteZ14[i].rgbBlue = (BYTE)255;
		pGrayPaletteZ14[i].rgbGreen = (BYTE)255;
		pGrayPaletteZ14[i].rgbRed = (BYTE)255;
		pGrayPaletteZ14[i].rgbReserved = 0;
	}
}

void UpdateD11DisplayImage_DIB24(RGBQUAD* pColorPaletteD11, BYTE* pDepthD11, BYTE* pDepthDIB24, int cx, int cy)
{
	int x, y, nBPS;
	WORD* pWSL, * pWS;
	BYTE* pDL, * pD;
	RGBQUAD* pClr;
	//
	if ((cx <= 0) || (cy <= 0)) return;
	//
	nBPS = ((cx * 3 + 3) / 4) * 4;
	pWSL = (WORD*)pDepthD11;
	pDL = pDepthDIB24;
	//for (y = 0; y < cy; y++) {
	//	pWS = pWSL;
	//	pD = pDL;
	//	for (x = 0; x < cx; x++) {
	//		pClr = &(pColorPaletteD11[pWS[x]]);
	//		pD[0] = pClr->rgbBlue; //B
	//		pD[1] = pClr->rgbGreen; //G
	//		pD[2] = pClr->rgbRed; //R
	//		pD += 3;
	//	}
	//	pWSL += cx;
	//	pDL += nBPS;
	//}
	for (y = 0; y < cy; y++)
	{
		pWS = pWSL;
		pD = pDL;
		for (x = 0; x < cx; x++)
		{
			if (pWS[x] < 400 || pWS[x]> 1800)
			{
				pD[0] = 0;//B
				pD[1] = 0;//G
				pD[2] = 0;//R
			}
			else
			{
				pD[0] = pWS[x] / 8;//B
				pD[1] = pWS[x] / 8;//G
				pD[2] = pWS[x] / 8;//R
			}
			
			pD += 3;
		}
		pWSL += cx;
		pDL += nBPS;
	}
}

void UpdateZ14DisplayImage_DIB24(RGBQUAD* pColorPaletteZ14, BYTE* pDepthZ14, BYTE* pDepthDIB24, int cx, int cy)
{
	int x, y, nBPS;
	WORD* pWSL, * pWS;
	BYTE* pDL, * pD;
	RGBQUAD* pClr;
	//
	if ((cx <= 0) || (cy <= 0)) return;
	//
	nBPS = ((cx * 3 + 3) / 4) * 4;
	pWSL = (WORD*)pDepthZ14;
	pDL = pDepthDIB24;
	for (y = 0; y < cy; y++) {
		pD = pDL;
		for (x = 0; x < cx; x++) {
			pClr = &(pColorPaletteZ14[pWSL[x]]);
			pD[0] = pClr->rgbBlue; //B
			pD[1] = pClr->rgbGreen; //G
			pD[2] = pClr->rgbRed; //R
			pD += 3;
		}
		pWSL += cx;
		pDL += nBPS;
	}
}

//k= 0~1.00
//maps k to a pixel color RGB
void ColorMap(double k, double& R, double& G, double& B)
{
	double r;

	if (k < 0.0) k = 0.0;
	if (k > 1.0) k = 1.0;
	if (k < 0.1) {
		r = k / 0.1;
		R = G = B = 128.0 + r * 127.0; //128~255
	}
	else if (k < 0.2) {
		k -= .1;
		r = k / 0.1;
		R = 255.0;
		G = B = (1.0 - r) * 255.0;//255~0
	}
	else if (k < 0.35) {
		k -= .2;
		r = k / 0.15;
		B = 0.0; //B
		G = r * 255.0; // 0~255
		R = 255.0; //R
	}
	else if (k < 0.5) {
		k -= 0.35;
		r = k / 0.15;
		B = 0.0;
		G = (1.0 - r / 4.0) * 255.0;  //255~196
		R = (1.0 - r / 2.0) * 255.0; //255~128
	}
	else if (k < 0.6) {
		k -= 0.5;
		r = k / 0.1;
		B = r * 128.0; //B 0~128
		G = 196.0; //Gc
		R = (1.0 - r) * 128.0; //R 128~0
	}
	else if (k < 0.7) {
		k -= 0.6;
		r = k / 0.1;
		B = 128.0 + r * 127.0; //B 128~255
		G = 196.0; //G
		R = 0.0; //R
	}
	else if (k < 0.8) {
		k -= 0.7;
		r = k / 0.1;
		B = 255; //B
		G = (1.0 - r) * 196.0; //G 196~0
		R = 0; //R
	}
	else if (k < 0.9) {
		k -= 0.8;
		r = k / 0.1;
		B = (1.0 - r / 2.0) * 255.0; //B 255~128
		G = 0.0; //G
		R = r * 128.0; //R=0~128
	}
	else {
		k -= .9;
		r = k / .1;
		R = B = (1 - r) * 128; //B 128~0
		G = 0; //G
	}
}

void DmColorMode11(RGBQUAD* pallete, int mode)
{
#define CP1	0.75
#define CP2 0.25

	int i;
	double R, G, B;
	int t1, t2;
	switch (mode) {
	case 1: //near
		t1 = 512;
		t2 = 1024;
		break;
	case 2: //midle
		t1 = 200;
		t2 = 512;
		break;
	case 3: //far
		t1 = 5;
		t2 = 256;
		break;
	default: //normal 
		t1 = 256;
		t2 = 512;
		break;
	}
	double m, b; //y=mx+b
				 //0~t1
	m = (CP1 - 1.0) / (double)t1;
	b = 1.0;
	for (i = 0; i < t1; i++) {
		ColorMap(m * (double)i + b, R, G, B);
		pallete[i].rgbBlue = (BYTE)B;
		pallete[i].rgbGreen = (BYTE)G;
		pallete[i].rgbRed = (BYTE)R;
		pallete[i].rgbReserved = 0;
	}
	m = (CP2 - CP1) / (double)(t2 - t1);
	b = CP1 - m * (double)t1;
	for (; i < t2; i++) {
		ColorMap(m * (double)i + b, R, G, B);
		pallete[i].rgbBlue = (BYTE)B;
		pallete[i].rgbGreen = (BYTE)G;
		pallete[i].rgbRed = (BYTE)R;
		pallete[i].rgbReserved = 0;
	}
	m = (0 - CP2) / (double)(2048 - t2);
	b = CP2 - m * (double)t2;
	for (; i < 2048; i++) {
		ColorMap(m * (double)i + b, R, G, B);
		pallete[i].rgbBlue = (BYTE)B;
		pallete[i].rgbGreen = (BYTE)G;
		pallete[i].rgbRed = (BYTE)R;
		pallete[i].rgbReserved = 0;
	}


}

void DmColorMode14(RGBQUAD* pallete, int mode)
{
#define CP1	0.75
#define CP2 0.25
	int length = 16384;
	int i;
	double R, G, B;
	int t1, t2; //focus region, 0.25~0.75 mapping area
	switch (mode) {
	case 1: //near
		t1 = 512 * 8;
		t2 = 1024 * 8;
		break;
	case 2: //midle
		t1 = 200 * 8;
		t2 = 512 * 8;
		break;
	case 3: //far
		t1 = 5 * 8;
		t2 = 256 * 8;
		break;
	default: //normal 
		t1 = 256 * 8;
		t2 = 512 * 8;
		break;
	}
	double m, b; //y=mx+b
				 //0~t1
	m = (CP1 - 1.0) / (double)t1;
	b = 1.0;
	for (i = 0; i < t1; i++) {
		ColorMap(m * (double)i + b, R, G, B);
		pallete[i].rgbBlue = (BYTE)B;
		pallete[i].rgbGreen = (BYTE)G;
		pallete[i].rgbRed = (BYTE)R;
		pallete[i].rgbReserved = 0;
	}
	m = (CP2 - CP1) / (double)(t2 - t1);
	b = CP1 - m * (double)t1;
	for (; i < t2; i++) {
		ColorMap(m * (double)i + b, R, G, B);
		pallete[i].rgbBlue = (BYTE)B;
		pallete[i].rgbGreen = (BYTE)G;
		pallete[i].rgbRed = (BYTE)R;
		pallete[i].rgbReserved = 0;
	}
	m = (0 - CP2) / (double)(2048 - t2);
	b = CP2 - m * (double)t2;
	for (; i < length; i++) {
		ColorMap(m * (double)i + b, R, G, B);
		pallete[i].rgbBlue = (BYTE)B;
		pallete[i].rgbGreen = (BYTE)G;
		pallete[i].rgbRed = (BYTE)R;
		pallete[i].rgbReserved = 0;
	}

}
