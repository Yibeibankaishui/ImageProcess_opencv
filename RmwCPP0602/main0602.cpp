#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "bmpFile.h"
#include "RmwALG_HoughCircle.h"

///////////////////////////////////////////////////////////////////////////////////////
//
//  全局变量
//
///////////////////////////////////////////////////////////////////////////////////////
#define  MAX_POINT_NUM   (512*512)
static int gN;
static int gX[MAX_POINT_NUM];
static int gY[MAX_POINT_NUM];
///////////////////////////////////////////////////////////////////////////////////////
//
//圆HT变换
//
///////////////////////////////////////////////////////////////////////////////////////
void testCircleHT()
{
	unsigned char *pGryImg;
	int width, height;
	int R=14; //半径

	// step.1-------------读图像文件------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0602\\Fig0616b.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
 	// step.2-------------收集边缘点------------------------//
 	BYTE *pCur;
	int x, y;
	gN = 0;
	for (y = 0, pCur = pGryImg; y<height; y++)
	{
		for (x = 0; x<width; x++, pCur++)
		{
			if (*pCur)
			{
				*pCur = 0;
				if (gN<MAX_POINT_NUM-1)
				{
					gX[gN] = x;
					gY[gN] = y;
					gN++;
				}
			}
			else *pCur = 200;
		}
	}
 	// step.3-------------霍夫变换--------------------------//
	int resX0, resY0; //圆心位置
	int *pCount = new int[width*height];
	RmwHoughCircle(pCount, width, height, R, gX, gY, gN, &resX0, &resY0);
	//最佳圆
	RmwDrawCircle(pGryImg, width, height, resX0, resY0, R, 255);
	Rmw_Write8BitImg2BmpFileMark(pGryImg, width, height, "..\\RmwCPP0602\\Fig0616d.bmp");
	//圆心计数值
	for (int i = 0; i<width*height; i++) pGryImg[i] = min(255, pCount[i]*2);
	Rmw_Write8BitImg2BmpFile(pGryImg, width, height, "..\\RmwCPP0602\\Fig0616c.bmp");
	// step.4-------------结束-----------------------------//
	delete pGryImg; //释放自己申请的内存
	delete pCount; //释放自己申请的内存
	printf("Press any key to exit!\n");
	getchar();
	return;
}
///////////////////////////////////////////////////////////////////////////////////////
//
//圆分治法
//
///////////////////////////////////////////////////////////////////////////////////////
void testCircleX0Y0()
{
	unsigned char *pGryImg;
	int width, height;
	int i;

	// step.1-------------读图像文件------------------------//
	pGryImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0602\\Fig0617a.bmp", &width, &height);
	if (!pGryImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	// step.2-------------图像处理--------------------------//
	int x0, y0, count;
	count = RmwCircleX0(pGryImg, width, height, &x0);
	count = RmwCircleY0(pGryImg, width, height, &y0);
	// step.3-------------调试------------------------------//
	for (i = 0; i<width*height; i++) pGryImg[i] = min(250, pGryImg[i]);
	for (i = 0; i<height; i++) *(pGryImg+i*width+x0) = 255;
	for (i = 0; i<width; i++) *(pGryImg+y0*width+i) = 255;
	Rmw_Write8BitImg2BmpFileMark(pGryImg, width, height, "..\\RmwCPP0602\\Fig0617a_x0y0.bmp");
	// step.4-------------结束------------------------------//
	delete pGryImg; //释放自己申请的内存
	printf("Press any key to exit!\n");
	getchar();
	return;
}

void main()
{
	testCircleHT();
	testCircleX0Y0();
}