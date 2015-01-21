#ifdef FUNCTION
// function.cppからインクルードされた場合

#else
// function.cpp以外からインクルードされた場合

//	AからBへの角度を計算する。
extern float GetAngle(float Xa , float Ya , float Xb , float Yb);

//	角度と移動距離から、X成分とY成分を計算する。
extern void GetVector(float move , float RadAngle , float *X , float *Y);

//	当たり判定をする。sizeは当たり判定のサイズ
//	戻り値はTRUEが当たったとき、FALSEが外れたとき
extern bool isHit(float xA , float yA , float sizeA , float xB , float yB , float sizeB);

#endif

