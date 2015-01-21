
#define FUNCTION	// function.cpp�ł��邱�Ƃ�����

#include <math.h>
#include <iostream>

#include "def.h"
#include "function.h"

using namespace std;

float rad90 = DegToRad * 90 ;
float rad180 = DegToRad * 180 ;
float rad270 = DegToRad * 270 ;
float rad360 = DegToRad * 360 ;

//	A����B�ւ̊p�x���v�Z����B
float GetAngle(float Xa , float Ya , float Xb , float Yb){
	return atan2f(Yb - Ya , Xb - Xa ) ;
}

//	�p�x�ƈړ���������AX������Y�������v�Z����B
void GetVector(float move , float RadAngle , float *X , float *Y)
{
	*X = move * cos(RadAngle) ;
	*Y = move * sin(RadAngle) ;
}

//	�����蔻�������Bsize�͓����蔻��̃T�C�Y
//	�߂�l��TRUE�����������Ƃ��AFALSE���O�ꂽ�Ƃ�
bool isHit(float xA , float yA , float sizeA , float xB , float yB , float sizeB)
{
	//	���������Ƃ�
	if((xB - xA) * (xB - xA) + (yB - yA) * (yB - yA) < (sizeA + sizeB) * (sizeA + sizeB))
		return true ;
	//	�������Ă��Ȃ��Ƃ�
	else
		return false ;
}

