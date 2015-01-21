
#define FUNCTION	// function.cppであることを示す

#include <math.h>
#include <iostream>

#include "def.h"
#include "function.h"

using namespace std;

float rad90 = DegToRad * 90 ;
float rad180 = DegToRad * 180 ;
float rad270 = DegToRad * 270 ;
float rad360 = DegToRad * 360 ;

//	AからBへの角度を計算する。
float GetAngle(float Xa , float Ya , float Xb , float Yb){
	return atan2f(Yb - Ya , Xb - Xa ) ;
}

//	角度と移動距離から、X成分とY成分を計算する。
void GetVector(float move , float RadAngle , float *X , float *Y)
{
	*X = move * cos(RadAngle) ;
	*Y = move * sin(RadAngle) ;
}

//	当たり判定をする。sizeは当たり判定のサイズ
//	戻り値はTRUEが当たったとき、FALSEが外れたとき
bool isHit(float xA , float yA , float sizeA , float xB , float yB , float sizeB)
{
	//	当たったとき
	if((xB - xA) * (xB - xA) + (yB - yA) * (yB - yA) < (sizeA + sizeB) * (sizeA + sizeB))
		return true ;
	//	当たっていないとき
	else
		return false ;
}

