///////////////////////////////////////////////////////////////////////////////////////////
//
//������-Բ����任
//
///////////////////////////////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <nmmintrin.h>

///////////////////////////////////////////////////////////////////////////////////////////
//
//��Բ
//
///////////////////////////////////////////////////////////////////////////////////////////
void RmwDrawCircle(BYTE *pGryImg, int width, int height, int x0, int y0, int r, int color)
{
	int theta, x, y;

	for (theta = 0; theta<360; theta++) //����Ϊ1��
	{
		x = x0+(int)(r*cos(theta*3.1415926/180));
		y = y0+(int)(r*sin(theta*3.1415926/180));
		if ((x>=0)&&(x<width)&&
			(y>=0)&&(y<height)
			)
		{
			*(pGryImg+y*width+x) = color;
		}
	}
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////
//
//ԲHT�任
//
///////////////////////////////////////////////////////////////////////////////////////////
int RmwHoughCircle( int *pCount, //�ⲿ����õļ����ռ�,��Сwidth*height
	                int width, int height,  //ͼ��Ĵ�С
	                int r, //Բ�İ뾶 
	                int *x, int *y, int N, //��Ե��
	                int *x0,int *y0 //Բ��λ��(�޶�Բ��λ����ͼ���ڲ�)
                  )
{  	//��֪�뾶rʱ��Hough�任
	int *pCur;
	int theta, cosV, sinV;
	int i, tstY0, tstX0, maxCount;

 	// step.1-------------����任---------------------------//
	memset(pCount, 0, sizeof(int)*width*height);
	for (theta = 0; theta<360; theta++) //����Ϊ1��
	{
		cosV = (int)(cos(theta*3.1415926/180)*2048); //�Ŵ�2048��
		sinV = (int)(sin(theta*3.1415926/180)*2048);
		for (i = 0; i<N; i++)
		{
			tstX0 = x[i]-((r*cosV)>>11); //��С2048��,����Ϊ1
			if ((tstX0<0)||(tstX0>width-1)) continue; //tstX0��Ч
			tstY0 = y[i]-((r*sinV)>>11); //��С2048��,����Ϊ1
			if ((tstY0<0)||(tstY0>height-1)) continue; //tstY0��Ч
			pCount[tstY0*width+tstX0] += 1; //����
		}
	}
	// step.2-------------Ѱ�����ֵλ��----------------------//
	*x0 = *y0 = maxCount = 0;
	for (tstY0 = 0, pCur = pCount; tstY0<height; tstY0++)
	{
		for (tstX0 = 0; tstX0<width; tstX0++, pCur++)
		{
			if (*pCur>maxCount)
			{
				maxCount = *pCur;
				*x0 = tstX0;
				*y0 = tstY0;
			}
		}
	}
	// step.3-------------����------------------------------//
	return maxCount; //���ڼ�������ԭ��,��ֵ�������ڳ����ڽ��Բ�ϵı�Ե����
}
///////////////////////////////////////////////////////////////////////////////////////////
//
//���η���Բ��
//
///////////////////////////////////////////////////////////////////////////////////////////
int RmwCircleX0(BYTE *pBinImg, int width, int height, int *x0)
{
	int *pCount, x, y, x1, x2, maxCount;
	BYTE *pRow;

 	// step.1---------//
	pCount = new int[width]; //
	if (!pCount) return 0;
	// step.2------//
	memset(pCount, 0, sizeof(int)*width);
	for (y = 0, pRow = pBinImg; y<height; y++, pRow += width)
	{
		//�����ĺڵ�
		for (x1 = 0; x1<width; x1++)
		{
			if (!pRow[x1]) break;  // 边缘为黑色，0
		}
		//���Ҳ�ĺڵ�
		for (x2 = width-1; x2>=0; x2--)
		{
			if (!pRow[x2]) break;
		}
		//�е����
		if (x2-x1>0)
		{
			pCount[(x2+x1+1)>>1]++;
 		}
	}
	// step.3------寻找最大值位置//
	*x0 = 0;
	maxCount = 0;
	for (x = 0; x<width; x++)
	{
		if (pCount[x]>maxCount) { maxCount = pCount[x]; *x0 = x; }
	}
	// step.3-------//
	delete pCount; //
	return maxCount;
}

int RmwCircleY0(BYTE *pBinImg, int width, int height, int *y0)
{
	int *pCount, x, y, y1, y2, maxCount;
	BYTE *pRow;

 	// step.1-------------����������ռ�---------------------//
	pCount = new int[height]; //����pCount�ڸú���������
	if (!pCount) return 0;
	// step.2-------------����λ�ü���-----------------------//
	memset(pCount, 0, sizeof(int)*width);
	//����
	for (x = 0; x<width; x++)
	{
		//������ĺڵ�
		for (y1 = 0, pRow = pBinImg+x; y1<height; y1++, pRow += width)
		{
			if (!(*pRow)) break;
		}
		//������ĺڵ�
		for (y2 = height-1, pRow = pBinImg+y2*width+x; y2>=0; y2--, pRow -= width)
		{
			if (!(*pRow)) break;
		}
		//�е����
		if (y2-y1>0)
		{
			pCount[(y2+y1+1)>>1]++;
 		}
	}
	// step.3-------------Ѱ�����ֵλ��----------------------//
	*y0 = 0;
	maxCount = 0;
	for (y = 0; y<height; y++)
	{
		if (pCount[y]>maxCount) { maxCount = pCount[y]; *y0 = y; }
	}
	// step.3-------------����------------------------------//
	delete pCount;//�ͷ��Լ�������ڴ�,�ú����ڶ�̬������ͷŻ�����ڴ���Ƭ
	return maxCount;
}
