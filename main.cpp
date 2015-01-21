// あとで、main.hを作って、定義関係はそのファイルに入れ、条件コンパイルする
#define MAIN

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <Windows.h> // これがないと後のkinectヘッダがインクルードできないから注意
#include <math.h>
#include <fstream>
#include <shlwapi.h>
#include <tchar.h>

//	これがないとPathIsDirectoryが使えない
#pragma comment(lib, "shlwapi.lib")

#pragma comment(lib, "kinect10.lib")

#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>
#include <NuiSkeleton.h>

//	Kinect関係のヘッダファイル
//#include <MSR_NuiApi.h>
//#include <MSR_NuiImageCamera.h>
//#include <MSR_NuiProps.h>
//#include <MSR_NuiSkeleton.h>
//#include <NuiImageBuffer.h>



//	リーク検出
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "def.h"
#include "function.h"
#include "objClass.h"	//	シューティングゲームのデータまとめ
#include "main.h"

//	3Dモデルの座標を取得、変更
//----------------------------
//	モデルの座標を移動
//--------------------------
void ModelPosSet(int ModelHr, VECTOR vector)
{
	MV1SetPosition(ModelHr, vector) ;
	return;
}

void ModelPosAdd(int ModelHr, VECTOR vector)
{
	VECTOR vPos = MV1GetPosition(ModelHr);
	vPos = VAdd(vPos, vector) ;
	MV1SetPosition(ModelHr, vPos) ;
	return;
}

void ModelRotationAdd(int ModelHr, VECTOR vector)
{
	VECTOR v = MV1GetRotationXYZ(ModelHr);
	v = VAdd(v, vector) ;
	MV1SetRotationXYZ( ModelHr, v);
}
//------------------------------------
//	スケルトンデータ取得関数
//---------------------------------------
//	スケルトンデータからX座標とY座標のデータを変換してから取り出す
void GetSkeletonXY(NUI_SKELETON_FRAME frame, float *X, float *Y, int skeletonPos)
{
	for(int i = 0; i < 6; i++){
		if(frame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKING_STATE::NUI_SKELETON_TRACKED)
		{
			//	kinectから拾ったデータはメートル単位だから、それを関数で座標にする
			Vector4 v4 = frame.SkeletonData[i].SkeletonPositions[skeletonPos] ;
			//NuiTransformSkeletonToDepthImageF(v4, X, Y);
			
		}
	}
}

//	スケルトンデータからVector4をそのまま返す
void GetSkeletonV4(NUI_SKELETON_FRAME frame, Vector4 *XYZ, int skeletonPos)
{
	for(int i = 0; i < 6; i++){
		if(frame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKING_STATE::NUI_SKELETON_TRACKED)
		{
			//	Vector4の値をkinectセンサーからコピーする
			*XYZ = frame.SkeletonData[i].SkeletonPositions[skeletonPos] ;
		}
	}
}

//	スケルトンデータからVECTORの型に直して返す
void GetSkeletonV(NUI_SKELETON_FRAME frame, VECTOR *XYZ, int skeletonPos)
{
	SkeletonTracking = false ;
	for(int i = 0; i < 6; i++){
		if(frame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKING_STATE::NUI_SKELETON_TRACKED)
		{
			//	Vector4の値をkinectセンサーからコピーする
			XYZ->x = frame.SkeletonData[i].SkeletonPositions[skeletonPos].x;
			XYZ->y = frame.SkeletonData[i].SkeletonPositions[skeletonPos].y ;
			XYZ->z = frame.SkeletonData[i].SkeletonPositions[skeletonPos].z ;
			SkeletonTracking = true ;	//	プレイヤー認識、フラグを立てておく
		}
	}
}

//-------------------------------------------------------------------
//	グラフィックハンドルを生成する。
void CreateGraphicHandle(int *grHandle, const NUI_IMAGE_FRAME *ImageFrame , BASEIMAGE *BaseImage){
	// 画像データの取得
	NUI_LOCKED_RECT rect;
	ImageFrame->pFrameTexture->LockRect( 0, &rect, 0, 0 );   // IDirect3DTexture9 のLockメソッドと同じ？

	// Dxlibで使用する画像データの構造体設定
	BaseImage->GraphData = rect.pBits;
	BaseImage->Pitch = rect.Pitch;  // ピッチは横１ラインあたりのByte数

	ImageFrame->pFrameTexture->UnlockRect(0);
	// 最初の場合はグラフィックハンドルの作成と映像の転送を一度に行う
	*grHandle = CreateGraphFromBaseImage( BaseImage ) ;
}



//	kinectカメラのデータをグラフィックハンドルに転送する
void LoadImageForKinectCameraRGB(int *grHandle, const NUI_IMAGE_FRAME *ImageFrame , BASEIMAGE *BaseImage){
	return;

	// 画像データの取得
	NUI_LOCKED_RECT rect;
	ImageFrame->pFrameTexture->LockRect( 0, &rect, 0, 0 );   // IDirect3DTexture9 のLockメソッドと同じ？

	// Dxlibで使用する画像データの構造体設定
	BaseImage->GraphData = rect.pBits;
	BaseImage->Pitch = rect.Pitch;  // ピッチは横１ラインあたりのByte数



	// ２回目以降はグラフィックハンドルへ映像を転送
	ReCreateGraphFromBaseImage( BaseImage, *grHandle ) ;
}

void CreateGraphicHandle(int *GrHandle, HANDLE StreamHandle, BASEIMAGE *BaseImage){
	HRESULT hr;
	// カメラデータの取得
	const NUI_IMAGE_FRAME *ImageFrame;
	hr = NuiImageStreamGetNextFrame( StreamHandle, 0, &ImageFrame );
	
	// 画像データの取得
	NUI_LOCKED_RECT rect;
	ImageFrame->pFrameTexture->LockRect( 0, &rect, 0, 0 );   // IDirect3DTexture9 のLockメソッドと同じ？

	// Dxlibで使用する画像データの構造体設定
	BaseImage->GraphData = rect.pBits;
	BaseImage->Pitch = rect.Pitch;  // ピッチは横１ラインあたりのByte数

	// カメラデータの解放
	hr = NuiImageStreamReleaseFrame( StreamHandle, ImageFrame );
	//  一時的にコメントアウト
	//ImageFrame->pFrameTexture->UnlockRect(0);

	// 最初の場合はグラフィックハンドルの作成と映像の転送を一度に行う
	*GrHandle = CreateGraphFromBaseImage( BaseImage ) ;
}



//	kinectカメラのデータをグラフィックハンドルに転送する
void LoadImageForKinectCameraRGB(int *grHandle, HANDLE StreamHandle , BASEIMAGE *BaseImage){
	// カメラデータの取得
	const NUI_IMAGE_FRAME *ImageFrame;
	
	NuiImageStreamGetNextFrame( StreamHandle, 0, &ImageFrame );

	// 画像データの取得
	NUI_LOCKED_RECT rect;
	ImageFrame->pFrameTexture->LockRect( 0, &rect, 0, 0 );   // IDirect3DTexture9 のLockメソッドと同じ？

	// Dxlibで使用する画像データの構造体設定
	BaseImage->GraphData = rect.pBits;
	BaseImage->Pitch = rect.Pitch;  // ピッチは横１ラインあたりのByte数

	
	// カメラデータの解放
	NuiImageStreamReleaseFrame( StreamHandle, ImageFrame );
	//  一時的にコメントアウト
	//ImageFrame->pFrameTexture->UnlockRect(0);
	
	// ２回目以降はグラフィックハンドルへ映像を転送
	ReCreateGraphFromBaseImage( BaseImage, *grHandle ) ;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------







//---------------------------------------------------
//	アルファ値を使ってビルボードを描画する
void DrawAlphaBill(VECTOR pos, int Alpha, int grHandle, float size = 1.0f){
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, Alpha) ;
	DrawBillboard3D( pos, 0.5f, 0.5f, size, 0, grHandle, TRUE );
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 ) ;
}
//---------------------------------------------------
//	VECTORの値を全て２乗にする
int VSquare(VECTOR square){
	return ( pow(square.x,2) + pow(square.y,2) + pow(square.z,2) ) ;
}

//------------------------------------------------------
//	スコアを計算する
int ScoreCalc(VECTOR velocity){
	if(velocity.y < 0)	velocity.y = 0;	
	return (abs(velocity.z * 100 * (velocity.y / 10 + 1))) ;	//	スコアを計算
}


//----------------------------------
//	とりあえずオブジェクトを表示するクラス
//----------------------------------
class Object : EffectBase{
public:
	int grHandle ;
	Object(char* fileName, VECTOR pos, VECTOR angle, float scale);
	virtual void Draw() ;
};

Object::Object(char* fileName, VECTOR pos, VECTOR angle = VGet(0,0,0), float scale = 1.0f){
	grHandle = MV1LoadModel( fileName ) ;
	MV1SetPosition( grHandle, pos) ;
	MV1SetRotationXYZ( grHandle, angle) ;
	MV1SetScale( grHandle, VGet(scale, scale, scale)) ;
}
void Object::Draw(){
	MV1DrawModel(grHandle) ;
}

//-----------------------------------------
//	火花エフェクトを表示するクラス
//----------------------------------------
class SparkEffect : EffectBase{
public:
	static int grHandle[10];
	int frame ;
	VECTOR pos ;
	SparkEffect(VECTOR pos);
	virtual void Move();
	virtual void Draw();
};
int SparkEffect::grHandle[] = {0,0,0,0,0,0,0,0,0,0} ;
SparkEffect::SparkEffect(VECTOR pos){
	this->pos = pos;
	frame = 0;
}
void SparkEffect::Move(){
	frame++;
	if(frame == 10)	//	10フレームまで終了したら削除する
		alive = false;	
}
void SparkEffect::Draw(){
	DrawBillboard3D( pos, 0.5f, 0.5f, 2.0f, 0, grHandle[frame], TRUE );
}

//-----------------------------------------------
//	セリフエフェクトを表示するクラス（ビルボードで表示）
//-----------------------------------------------
class SerifEffect : EffectBase{
public:
	static int grHandle [5] ;
	int nowAlpha ;
	int serifNum ;	//	使用するセリフ。ランダムに選ぶ
	VECTOR pos ;	//	表示する位置
	SerifEffect( VECTOR pos , int serifNum);
	virtual void Move() ;
	virtual void Draw() ;
};
int SerifEffect::grHandle[] = {0,0,0,0,0} ;
SerifEffect::SerifEffect( VECTOR pos , int serifNum = -1){
	if(serifNum == -1) this->serifNum = GetRand(3) ;
	else this->serifNum = serifNum ;
	this->pos = VAdd(pos, VGet(0,0,-2.0f)) ;
	nowAlpha = 255 ;
}
void SerifEffect::Move(){
	if(nowAlpha >= 4)nowAlpha -= 4 ;
	else {nowAlpha = 0; alive = false;}
}
void SerifEffect::Draw(){
	DrawAlphaBill(pos, nowAlpha, grHandle[serifNum], 2.0f) ;
}

//-----------------------------------------------
//	スコアエフェクト（ドラム缶を殴った際に数字を表示）
//-----------------------------------------------
class ScoreNumEffect : EffectBase{
public:
	int nowAlpha ;
	int scoreNum ;	//	表示する数字
	VECTOR pos ;	//	表示する位置
	ScoreNumEffect( VECTOR pos , int score);
	virtual void Move() ;
	virtual void Draw() ;
};
ScoreNumEffect::ScoreNumEffect( VECTOR pos , int score){
	scoreNum = score ;
	pos = ConvWorldPosToScreenPos(pos) ;
	this->pos.x = pos.x ;
	this->pos.y = pos.y  + 100;
	nowAlpha = 255 ;
}
void ScoreNumEffect::Move(){
	if(nowAlpha >= 5)nowAlpha -= 5 ;
	else {nowAlpha = 0; alive = false;}
}
void ScoreNumEffect::Draw(){
	SetFontSize(50) ;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, nowAlpha) ;
	DrawFormatString(pos.x, pos.y, GetColor(255,255,255), "+%d", scoreNum) ;
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, NULL) ;
}



//----------------------------------------
//	カメラを揺らすクラス
//----------------------------------------
class CameraShakeEffect : EffectBase{
public:
	VECTOR shake ;
	int limitFrame ;	//	消えるまでの時間
	int nowFrame ;		//	現在のフレーム数
	CameraShakeEffect(VECTOR shake, int repeatNum);
	virtual void Move() ;
};
CameraShakeEffect::CameraShakeEffect(VECTOR shake, int repeatNum){	//	揺らすVECTOR、揺らす回数
	nowFrame = 0;
	this->shake = shake ;
	limitFrame = repeatNum * 2 ;
}
void CameraShakeEffect::Move(){
	VECTOR cameraPos = GetCameraPosition()  ;
	VECTOR cameraTarget = GetCameraTarget() ;
	int i = pow(-1.0f ,(nowFrame % 2)) ;	//	偶数は1、奇数は-1
	cameraTarget = VAdd( cameraTarget, VScale(shake, i)) ;
	SetCameraPositionAndTargetAndUpVec(
		cameraPos, cameraTarget, VGet(0, 1.0f ,0)) ;

	nowFrame++ ;
	if(limitFrame <= nowFrame)
		alive = false ;
}

//----------------------------------------
//	プレイヤーの手
//---------------------------------------
//	右手
class RightHand : PB_Base{
public:
	int grHandle ; //	３Dモデルのハンドル
	RightHand();
	virtual void Move() ;
	virtual void Draw() ;
};
RightHand::RightHand()
{
	this->skeletonPos = NUI_SKELETON_POSITION_HAND_RIGHT ;
	//grHandle = MV1LoadModel("");
}
void RightHand::Move()
{
	if(kinectUpdateFlag)	//	kinectが更新されたときだけ前データを更新する。
	{
		VECTOR centerV;	//	体の中心座標
		revV = nowV ;	//	前データを保存

		GetSkeletonV(playerSkeleton, &nowV,  skeletonPos);	//	ボーンのベクトルをメートル単位で取得
		GetSkeletonV(playerSkeleton, &centerV,  NUI_SKELETON_POSITION_HIP_CENTER);	//	体の中心座標の取得
		//nowV.z = centerV.z - nowV.z ;
		//	2011/08/16黒子
		//nowV.z += PLAYER_Z_POSITION ;	//	プレイヤーの立ち位置を前に移動させる
		//nowV.y += PLAYER_Y_POSITION ;	//	プレイヤーの立ち位置を前に移動させる

		nowV.z = VScale(nowV, -1.0f).z + PLAYER_Z_POSITION ;
		nowV.y += PLAYER_Y_POSITION ;
		//	X方向に大きく動けるようにする
		VECTOR relativeSkeleton ;	//	腰からの相対座標
		relativeSkeleton = VSub(nowV, centerV) ;	//	腰からの相対座標取得
		centerV.x *= PLAYER_X_MOVE_SCALE;	//	プレイヤーがX方向に移動するときの倍率
		nowV.x = centerV.x + relativeSkeleton.x ;	//	X座標だけ大きく移動させる	
	}
}


	

void RightHand::Draw()
{
	DrawString(700,30,"右手",GetColor(255,255,255));
	DrawFormatString(700,50,GetColor(255,255,255),"座標X = %00f", posX);
	DrawFormatString(700,70,GetColor(255,255,255),"座標Y = %00f", posY);
	DrawFormatString(700, 90, GetColor(255,255,255), "ワールド座標X = %00f", nowV.x);
	DrawFormatString(700, 110, GetColor(255,255,255), "ワールド座標Y = %00f", nowV.y);
	DrawFormatString(700, 130, GetColor(255,255,255), "ワールド座標Z = %00f", nowV.z);
	DrawFormatString(700, 150, GetColor(255,255,255), "相対座標Z = %00f", centerV.z - nowV.z);
	//	右手の位置を円で描画
	/*
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawCircle(posX, posY, 20, GetColor(255,0,0));
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	*/
	//	手の位置に球体を描画
	DrawSphere3D(nowV, HAND_SIZE/3*2, POLYGON_NUM, GetColor(255,0,0), GetColor(255,0,0), TRUE);
}
///////////////////////////////////////////
/////////////左手//////////////////////////
class LeftHand : PB_Base{
public:
	int grHandle ; //	３Dモデルのハンドル
	LeftHand();
	virtual void Move() ;
	virtual void Draw() ;
};
LeftHand::LeftHand()
{
	this->skeletonPos = NUI_SKELETON_POSITION_HAND_LEFT ;
	grHandle = MV1LoadModel("");
	x = -10;
	y = -10;
}
void LeftHand::Move()
{
	revV = nowV ;	//	前データを保存
	GetSkeletonXY(playerSkeleton, &posX, &posY, skeletonPos);	//	XYのボーン位置取得
	posX *= 8; // 座標変換
	posY *= 8; // 座標変換
	GetSkeletonV(playerSkeleton, &nowV,  skeletonPos);	//	ボーンのベクトルをメートル単位で取得
	GetSkeletonV(playerSkeleton, &centerV,  NUI_SKELETON_POSITION_HIP_CENTER);	//	体の中心座標の取得
}
void LeftHand::Draw()
{
	DrawString(0,30,"左手",GetColor(255,255,255));

	DrawFormatString(0,50,GetColor(255,255,255),"座標X = %00f", posX);

	DrawFormatString(0,70,GetColor(255,255,255),"座標Y = %00f", posY);
	DrawFormatString(0, 90, GetColor(255,255,255), "距離X = %00f", nowV.x);
	DrawFormatString(0, 110, GetColor(255,255,255), "距離Y = %00f", nowV.y);
	DrawFormatString(0, 130, GetColor(255,255,255), "距離Z = %00f", nowV.z);
	DrawFormatString(0, 150, GetColor(255,255,255), "相対座標Z = %00f", centerV.z - nowV.z);
	//	左手の位置を円で描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawCircle(posX, posY, 20, GetColor(0,0,255));
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
//-----------------------------------------------
//	アタックパーツクラス（当たり判定をする体のパーツ）
//-----------------------------------------------
class AttackParts : PB_Base{
public:
	int grHandle ; //	３Dモデルのハンドル
	AttackParts(int skeletonPos);
	virtual void Move() ;
	virtual void Draw() ;
};
AttackParts::AttackParts(int skeletonPos)
{
	this->skeletonPos = skeletonPos ;
	//grHandle = MV1LoadModel("");
}
void AttackParts::Move()
{
	if(kinectUpdateFlag)	//	kinectが更新されたときだけ前データを更新する。
	{
		VECTOR centerV;	//	体の中心座標
		revV = nowV ;	//	前データを保存

		GetSkeletonV(playerSkeleton, &nowV,  skeletonPos);	//	ボーンのベクトルをメートル単位で取得
		GetSkeletonV(playerSkeleton, &centerV,  NUI_SKELETON_POSITION_HIP_CENTER);	//	体の中心座標の取得
		//nowV.z = centerV.z - nowV.z ;
		//	2011/08/16黒子
		//nowV.z += PLAYER_Z_POSITION ;	//	プレイヤーの立ち位置を前に移動させる
		//nowV.y += PLAYER_Y_POSITION ;	//	プレイヤーの立ち位置を前に移動させる

		nowV.z = VScale(nowV, -1.0f).z + PLAYER_Z_POSITION ;
		centerV.z = VScale(centerV, -1.0f).z + PLAYER_Z_POSITION ;
		nowV.y += PLAYER_Y_POSITION ;
		//	X方向に大きく動けるようにする
		VECTOR relativeSkeleton ;	//	腰からの相対座標
		relativeSkeleton = VSub(nowV, centerV) ;	//	腰からの相対座標取得
		centerV.x *= PLAYER_X_MOVE_SCALE;	//	プレイヤーがX方向に移動するときの倍率
		centerV.z *= PLAYER_Z_MOVE_SCALE;	//	プレイヤーがZ方向に移動するときの倍率
		centerV.y *= PLAYER_Y_MOVE_SCALE;	//	プレイヤーがY方向に移動するときの倍率
		nowV.x = centerV.x + relativeSkeleton.x ;	//	X座標だけ大きく移動させる	
		nowV.z = centerV.z + relativeSkeleton.z ;	//	Z座標だけ大きく移動させる
		nowV.y = centerV.y + relativeSkeleton.y ;	//	Z座標だけ大きく移動させる
	}
}


	

void AttackParts::Draw()
{
	/*
	DrawString(700,30,"右手",GetColor(255,255,255));
	DrawFormatString(700,50,GetColor(255,255,255),"座標X = %00f", posX);
	DrawFormatString(700,70,GetColor(255,255,255),"座標Y = %00f", posY);
	DrawFormatString(700, 90, GetColor(255,255,255), "ワールド座標X = %00f", nowV.x);
	DrawFormatString(700, 110, GetColor(255,255,255), "ワールド座標Y = %00f", nowV.y);
	DrawFormatString(700, 130, GetColor(255,255,255), "ワールド座標Z = %00f", nowV.z);
	DrawFormatString(700, 150, GetColor(255,255,255), "相対座標Z = %00f", centerV.z - nowV.z);
	//	右手の位置を円で描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawCircle(posX, posY, 20, GetColor(255,0,0));
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	*/
	//	手の位置に球体を描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, PLAYER_ALPHA + 80) ;
	DrawSphere3D(nowV, HAND_SIZE/3*2, POLYGON_NUM, GetColor(255,0,0), GetColor(255,0,0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, NULL) ;
}


//--------------------------------
//	プレイヤークラス
//--------------------------------
class KinectPlayer : public MoverBase {	//	大幅追加。自分を描画する
public:
	VECTOR	skeletonPos[NUI_SKELETON_POSITION_COUNT];	//	全てのスケルトンデータ(VECTOR形式)
	static KinectPlayer *p ;	//	他クラスでプレイヤーを参照するときに使用
	int deathHandle;	//	墜落音
	int bomSound;		//	ボムの音
	int noHitTime ;		//	無敵時間
	int flashTime ;		//	点滅時間。自機が被弾したあとに使用。
	bool alive ;
	float hitSize ;		//	当たり判定の大きさ
	int stock ;			//	残機数
	int bom ;			//	残りボム数
	int bomEffect;		//	ボムの無敵時間の目安を示す魔方陣
	KinectPlayer(char graphicFileName[]);
	KinectPlayer(){};
	void virtual Draw();
	void virtual Move();
	void Shot();
};
//	静的メンバ変数の初期化
KinectPlayer *KinectPlayer::p = NULL ;
//	プレイヤークラスのコンストラクタ
KinectPlayer::KinectPlayer(char graphicFileName[30]){
	p = this ;
	x = 340;
	y = 400;
	noHitTime = 0 ;
	alive = true ;
	stock = 3;
	bom = 2;
	hitSize = PLAYER_SIZE;
	bomEffect = 0;
	flashTime = 0;
}

//	プレイヤーの描画
void KinectPlayer::Draw(){
	const int PColor = GetColor(0,0,255);	//	描画する色
	int i;	//	for文用
	
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, PLAYER_ALPHA ) ;	//	アルファ値を設定
	for(i = 0; i < 2; i++)
		DrawCapsule3D(skeletonPos[i], skeletonPos[i+1], HAND_SIZE/2, 20, PColor, PColor, TRUE) ;
	DrawCapsule3D(skeletonPos[2], skeletonPos[4], HAND_SIZE/2, 20, PColor, PColor, TRUE) ;	//	左肩を描画
	for(i = 4; i < 7; i++)
		DrawCapsule3D(skeletonPos[i], skeletonPos[i+1], HAND_SIZE/2, 20, PColor, PColor, TRUE) ;//	左肩から先を描画
	DrawCapsule3D(skeletonPos[2], skeletonPos[8], HAND_SIZE/2, 20, PColor, PColor, TRUE) ;	//	右肩を描画
	for(i = 8; i < 11; i++)
		DrawCapsule3D(skeletonPos[i], skeletonPos[i+1], HAND_SIZE/2, 20, PColor, PColor, TRUE) ;//	右肩から先を描画
	DrawCapsule3D(skeletonPos[0], skeletonPos[12], HAND_SIZE/2, 20, PColor, PColor, TRUE) ;	//	左尻を描画
	for(i = 12; i < 15; i++)
		DrawCapsule3D(skeletonPos[i], skeletonPos[i+1], HAND_SIZE/2, 20, PColor, PColor, TRUE) ;//	左尻から先を描画
	DrawCapsule3D(skeletonPos[0], skeletonPos[16], HAND_SIZE/2, 20, PColor, PColor, TRUE) ;	//	右尻を描画
	for(i = 16; i < 19; i++)
		DrawCapsule3D(skeletonPos[i], skeletonPos[i+1], HAND_SIZE/2, 20, PColor, PColor, TRUE) ;	//	右尻を描画	
	DrawSphere3D(skeletonPos[NUI_SKELETON_POSITION_HEAD], HAND_SIZE/3*2, 20, PColor, PColor, TRUE) ;	//	頭を描画
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, NULL ) ;	//	アルファ値を元に戻す
	return ;
}

void KinectPlayer::Shot(){

}

//	プレイヤーの移動
void KinectPlayer::Move(){
	if(kinectUpdateFlag){
		for(int i = 0; i < 20; i++){
			GetSkeletonV(playerSkeleton, &skeletonPos[i], i) ;
			skeletonPos[i].z = VScale(skeletonPos[i], -1.0f).z + PLAYER_Z_POSITION ;
			skeletonPos[i].y += PLAYER_Y_POSITION ;
		}
		skeletonPos[NUI_SKELETON_POSITION_HIP_CENTER].y -= PLAYER_Y_POSITION ;
		//	XYZ方向に大きく動けるようにする
		VECTOR relativeSkeleton[NUI_SKELETON_POSITION_COUNT];	//	腰からの相対座標
		for(int i = 0; i < 20; i++){
			relativeSkeleton[i] = VSub(skeletonPos[i], skeletonPos[NUI_SKELETON_POSITION_HIP_CENTER]) ;	//	腰からの相対座標取得
		}
		skeletonPos[NUI_SKELETON_POSITION_HIP_CENTER].x *= PLAYER_X_MOVE_SCALE;	//	プレイヤーがX方向に移動するときの倍率
		skeletonPos[NUI_SKELETON_POSITION_HIP_CENTER].z *= PLAYER_Z_MOVE_SCALE;	//	プレイヤーがZ方向に移動するときの倍率
		skeletonPos[NUI_SKELETON_POSITION_HIP_CENTER].y *= PLAYER_Y_MOVE_SCALE;	//	プレイヤーがZ方向に移動するときの倍率
		for(int i = 0; i < 20; i++){
			skeletonPos[i].x = skeletonPos[NUI_SKELETON_POSITION_HIP_CENTER].x + relativeSkeleton[i].x ;	//	X座標だけ大きく移動させる	
			skeletonPos[i].z = skeletonPos[NUI_SKELETON_POSITION_HIP_CENTER].z + relativeSkeleton[i].z ;	//	Z座標を大きく動かす
			skeletonPos[i].y = skeletonPos[NUI_SKELETON_POSITION_HIP_CENTER].y + relativeSkeleton[i].y ;	//	Y座標を大きく動かす
		}

	}


	Shot() ;	//	自機弾を発射する
	return  ;
}
//----------------------------------------
//	ドラム缶のベースクラス（ドラム缶は継承する）
//----------------------------------------



//-------------------------------
//	普通の緑ドラム缶
//------------------------------
class NormalDrum : EnemyBase{
public:
	static int loadOnlyHr[][4] ;	//	３Dモデルのコピー読み込み用
	static int collisionSE;		//	手に触れた時のSE
	static int boundSE;			//	バウンドした際のSE
	static int shotSE ;			//	オブジェクトを生成した時の音（発射音）
	//int grHandle ;	//	３Dモデルのハンドル。描画等はこちらを使用。
	int timer ;	//	生存タイマー。一定以上になったらこのドラム缶を削除
	
	//VECTOR MV ;	//	派生元に追加。物体の移動速度
	VECTOR spin ;	//	回転速度
	//int boundNum ;	//	バウンドの回数

	//float realTimer ;	//	秒換算のタイマー	ミリ秒まで正確。
	//VECTOR initPos;	// 初期位置		派生元クラスに追加
	//VECTOR initVelocity;	// 初速度	派生元クラスに追加

	NormalDrum(unsigned long long in_waitTime, VECTOR initPos, VECTOR initVelocity);
	virtual ~NormalDrum();
	virtual void Move() ;
	virtual void Draw() ;
	virtual void PlayShotSE() ;
	virtual void PlayCollisionSE() ;
	
}; 
int NormalDrum::loadOnlyHr[][4] = {{0, 0, 0, 0},{0, 0, 0, 0},{0,0,0,0},{0,0,0,0}};
int NormalDrum::collisionSE = NULL;
int NormalDrum::boundSE = NULL;
int NormalDrum::shotSE = NULL;
//	引数の構成	出現時間（現在は無視、後で使う）,　初期位置,　移動速度
NormalDrum::NormalDrum(unsigned long long in_waitTime, VECTOR initPos, VECTOR initVelocity)
{
	waitTime = in_waitTime ;
	timer = 0;
	realTimer = 0;
	int drumColor = GetRand(3);		//	ランダムにドラムの色を入れる
	grHandle[0] = MV1DuplicateModel(loadOnlyHr[drumColor][0]) ;	//	予め読み込んであるモデルをコピー
	grHandle[1] = MV1DuplicateModel(loadOnlyHr[drumColor][1]) ;	//	凹んだドラムを読み込み
	grHandle[2] = MV1DuplicateModel(loadOnlyHr[drumColor][2]) ;	//	凹んだドラムを読み込み
	grHandle[3] = MV1DuplicateModel(loadOnlyHr[drumColor][3]) ;	//	凹んだドラムを読み込み

	MV1SetPosition( grHandle[0], initPos);

	MV = initVelocity ;

	NormalDrum::initPos = initPos;	// 初期位置
	NormalDrum::initVelocity = initVelocity;	// 初速度

	MV1SetScale(grHandle[0], VGet(0.8f, 0.8f, 0.8f)) ;
	//MV1SetScale(grHandle[0], VGet(0.08f, 0.08f, 0.08f)) ;
	MV1SetRotationXYZ(grHandle[0], VGet(0,0, DegToRad * 90));
	spin = VGet(0, 0, DegToRad * ((float)GetRand(8) + 2.0f));
	ModelRotationAdd(grHandle[0], VGet(DegToRad * (float)GetRand(50),0,0));
	MV1SetupCollInfo(grHandle[0]) ;	//	衝突情報を構築
	Activation();
}
void NormalDrum::PlayShotSE(){	//	Activation関数内で呼ばれる
	PlaySoundMem( shotSE, DX_PLAYTYPE_BACK ) ;	//	生成音を再生（撃ち出し音）
}
void NormalDrum::PlayCollisionSE(){
	PlaySoundMem( collisionSE, DX_PLAYTYPE_BACK ) ;	//	パンチの音を生成

}
NormalDrum::~NormalDrum()
{
	MV1TerminateCollInfo( grHandle[0], -1) ;
	for(int i = 0; i < 4; i++){
		MV1DeleteModel( grHandle[i] );
	}
}
void NormalDrum::Move()
{

	//ModelPosAdd(grHandle[0], MV) ;	//	移動速度を現在の位置に足す

	VECTOR velocity =  VAdd(VScale(VGet(0.0f,-GRAVITY,0.0f),realTimer),initVelocity); // 加速度×時間＝その時刻での速度

	VECTOR acc =  VGet(0.0f,-0.5f*GRAVITY*realTimer*realTimer,0.0f);
	VECTOR pos = VAdd(VAdd(initPos,VScale(initVelocity,realTimer)),acc);
	
	//	簡易バウンド処理
	if(pos.y < 0.5f && velocity.y < 0 ){
		if(velocity.y < -2 && boundNum == 0){
			new CameraShakeEffect(VGet(0,0.05f,0), 1) ;	//	クロコが追加。カメラを揺らすエフェクト
			PlaySoundMem( boundSE, DX_PLAYTYPE_BACK );	//	大きくバウンドするときのみSEを出す
		}
		if(abs(velocity.y) < 0.2f)  initVelocity.y = 0;
		else initVelocity.y = -velocity.y * RESTITUTE;	//	上に跳ねる高さの減少
		
		initVelocity.x = velocity.x *  RESTITUTE ;	//	速度の減少
		initVelocity.z = velocity.z *  RESTITUTE ;	
		initPos = MV1GetPosition(grHandle[0]) ;
		spin = VScale(spin, 0.5f) ;
		realTimer = 0;
		//	火花を生成
		//new SparkEffect( VAdd(pos, VGet(0, 0.3f, -1.0f)) ) ;

		// カメラの振動　20110823 masuda
		// 物理的に発生するのは振動（もとに戻る）だから、正確な表現ではないが、便宜上テスト
		/*
		float x0 =  ((GetRand(100)-50)/100.0f)*abs(velocity.x)*CAMERA_SHAKE_K;
		float y0 =  ((GetRand(100)-50)/100.0f)*abs(velocity.y)*CAMERA_SHAKE_K;
		float z0 =  ((GetRand(100)-50)/100.0f)*abs(velocity.z)*CAMERA_SHAKE_K;
		*/
		//CameraShake = VGet(x0,y0,z0);
		boundNum++;	//	バウンドの回数を数える
	}
	//	左右の壁の跳ね返り処理
	if((pos.x < (LEFT_WALL_X + 0.5f)) || (pos.x > (RIGHT_WALL_X - 0.5f))){
		initVelocity.x = -(initVelocity.x * 2.0f / 3.0f);
		initVelocity.y = initVelocity.y + acc.y;
		initPos = MV1GetPosition(grHandle[0]) ;
		realTimer = 0;
		new SparkEffect( VAdd(pos, VGet(0, 0.0f, -0.5f)) ) ;
	}

	MV1SetPosition(grHandle[0], pos) ;

	ModelRotationAdd(grHandle[0], spin);	//	モデルを回転させる
	if(hitFlag == false)
		MV1RefreshCollInfo(grHandle[0]); //	衝突情報を更新
	else{	//	凹んだドラム缶を普通のドラム缶と同期させる
		MV1SetPosition(grHandle[state], MV1GetPosition( grHandle[0] ));
		MV1SetRotationXYZ( grHandle[state], MV1GetRotationXYZ( grHandle[0] ));
	}
	timer++;
	realTimer += (float)IntervalTime / 1000.0f;	//	ミリ秒を秒単位に直して加算 
	if(timer == 360)	//	６秒経過したらドラム缶を削除する
		alive = false;
}
void NormalDrum::Draw()
{
	MV1DrawModel( grHandle[state] ) ;	//	モデルを描画
	//	影の描画
	VECTOR shadowPos = MV1GetPosition(grHandle[0]);
	shadowPos.y = 0.05f;
	MV1SetPosition( circleShadow, shadowPos );
	MV1DrawModel( circleShadow) ;
}
//-----------------------------------------------------------
//	赤ドラムクラス（殴るか触るとダメージ）
//-----------------------------------------------------------
class DamageDrum : EnemyBase{
public:
	static int loadOnlyHr[] ;	//	３Dモデルのコピー読み込み用
	//int grHandle ;	//	３Dモデルのハンドル。描画等はこちらを使用。
	int timer ;	//	生存タイマー。一定以上になったらこのドラム缶を削除
	
	//VECTOR MV ;	//	派生元に追加。物体の移動速度
	VECTOR spin ;	//	回転速度

	//float realTimer ;	//	秒換算のタイマー	ミリ秒まで正確。
	//VECTOR initPos;	// 初期位置		派生元クラスに追加
	//VECTOR initVelocity;	// 初速度	派生元クラスに追加

	DamageDrum(unsigned long long in_waitTime, VECTOR initPos, VECTOR initVelocity);
	~DamageDrum();
	virtual void Move() ;
	virtual void Draw() ;
};
int DamageDrum::loadOnlyHr[] = {0, 0};
//	引数の構成	出現時間（現在は無視、後で使う）,　初期位置,　移動速度
DamageDrum::DamageDrum(unsigned long long in_waitTime, VECTOR initPos, VECTOR initVelocity)
{
	waitTime = in_waitTime ;
	timer = 0;
	realTimer = 0;
	grHandle[0] = MV1DuplicateModel(loadOnlyHr[0]) ;	//	予め読み込んであるモデルをコピー
	grHandle[1] = MV1DuplicateModel(loadOnlyHr[1]) ;	//	凹んだドラムを読み込み
	MV1SetPosition( grHandle[0], initPos);
	damage = 1 ;	//	接触したら１ダメージ
	MV = initVelocity ;

	DamageDrum::initPos = initPos;	// 初期位置
	DamageDrum::initVelocity = initVelocity;	// 初速度

	MV1SetScale(grHandle[0], VGet(0.8f, 0.8f, 0.8f)) ;
	//MV1SetScale(grHandle[0], VGet(0.08f, 0.08f, 0.08f)) ;
	MV1SetRotationXYZ(grHandle[0], VGet(0,0, DegToRad * 90));
	spin = VGet(0, 0, DegToRad * ((float)GetRand(8) + 2.0f));
	ModelRotationAdd(grHandle[0], VGet(DegToRad * (float)GetRand(50),0,0));
	MV1SetupCollInfo(grHandle[0]) ;	//	衝突情報を構築
	Activation();
}
DamageDrum::~DamageDrum()
{
	MV1DeleteModel( grHandle[0] );	//	モデルの読み込みデータを破棄する
	MV1DeleteModel( grHandle[1] );
}
void DamageDrum::Move()
{
	//ModelPosAdd(grHandle[0], MV) ;	//	移動速度を現在の位置に足す

	VECTOR velocity =  VAdd(VScale(VGet(0.0f,-GRAVITY,0.0f),realTimer),initVelocity); // 加速度×時間＝その時刻での速度

	VECTOR acc =  VGet(0.0f,-0.5f*GRAVITY*realTimer*realTimer,0.0f);
	VECTOR pos = VAdd(VAdd(initPos,VScale(initVelocity,realTimer)),acc);
	
	//	簡易バウンド処理
	if(pos.y < 0.5f && velocity.y < 0){
		initVelocity.y = -velocity.y * RESTITUTE;	//	上に跳ねる高さの減少
		initVelocity.x = velocity.x * RESTITUTE ;	//	速度の減少
		initVelocity.z = velocity.z * RESTITUTE ;	
		initPos = MV1GetPosition(grHandle[0]) ;
		spin = VScale(spin, 0.5f) ;
		realTimer = 0;
	}

	MV1SetPosition(grHandle[0], pos) ;

	ModelRotationAdd(grHandle[0], spin);	//	モデルを回転させる
	if(hitFlag == false)
		MV1RefreshCollInfo(grHandle[0]); //	衝突情報を更新
	else{	//	凹んだドラム缶を普通のドラム缶と同期させる
		MV1SetPosition(grHandle[1], MV1GetPosition( grHandle[0] ));
		MV1SetRotationXYZ( grHandle[1], MV1GetRotationXYZ( grHandle[0] ));
	}
	timer++;
	realTimer += (float)IntervalTime / 1000.0f;	//	ミリ秒を秒単位に直して加算 
	if(timer == 360)	//	６秒経過したらドラム缶を削除する
		alive = false;
}
void DamageDrum::Draw()
{
	MV1DrawModel( grHandle[hitFlag] ) ;	//	モデルを描画
	//	影の描画
	/*
	VECTOR shadowPos = MV1GetPosition(grHandle[0]);
	shadowPos.y = 0.05f;
	DrawBillboard3D( shadowPos, 0.5f, 0.5f, 1.0f, 90*DegToRad, circleShadow, TRUE) ;
	*/
}


//////////////////////////////////////
///////イテレータクラス///////////////
//////////////////////////////////////

class ListIterator{
public:
	ListIterator() ;
	~ListIterator() ;
	ItemBase *IB ;	//	アイテム
	KinectPlayer *player ;//	プレイヤー
	CrashPlayer *crashP;	//	プレイヤーが撃墜
	PB_Base *PB ;	//	プレイヤーの弾
	BackGround *BG ;//	背景
	EnemyBase *E_Active ;	//	画面上の敵
	EnemyBase *E_Wait ;		//	待ち状態の敵
	EnemyBulletBase *EB ;	//	敵の弾
	EffectBase *Effect ;	//	エフェクト
	PlayerBomBase *PBom;	//	プレイヤーボム
	unsigned long long time ;	//	経過時間
	int kinectTime;	//	これが1の時にkinectのデータを取得する
	int panel ;	//	パネルのグラフィックハンドル
	unsigned long long grazeNum;	//	グレイズを数える
	int moveStageCnt ;	//	MoveStage関数の制御変数
	int nowStage ;	//	現在のステージ（1:ノーマルステージ１	11:	ハードステージ１

	//	テスト用追加
	int drumTimer ;
	BASEIMAGE ScreenShot[120];

	//	ポーズ関係
	int pauseAlpha ;		//	ポーズのアルファ値制御変数
	int pauseAdd ;			//	ポーズのアルファ値増減
	int pauseGr ;			//	ポーズのグラフィックハンドル
	systemTime *sysTime ; 
	int score ;				//	スコア

	//	タイムアウト関数関係
	int ranking[RANKING_MAX + 1];
	int rankPosY[11];
	bool timeoutInitialized ;
	float timeoutX, timeoutY;
	int timeoutGrHandle;
	int rankTimer ;	//	ランキング表示で使う制御変数
	int playerRank ;

	//	カラーイメージ（RGBカメラ）関係
	BASEIMAGE BaseImage[60] ;	//	画像を取得する際に使用
	int	ImageGrHandle[60] ;		//	カラーイメージハンドル
	int displayPos ;			//	現在の表示位置。フリック操作でスクリーンショット画面に移動
	int displayMove ;			//	画面の移動速度
	float displayMoveCouse;			//	画面の移動方向
	VECTOR revRHand ;				//	前フレームの右手の位置
	bool lookonRHand;			//	右手が前にあるか(trueのときは前にある)



	void Move() ;	//	敵、弾、自分等動かす
	void Draw() ;	//	敵、弾、自分等描画する
	void DeleteEnemyBullet() ;	//	敵弾を全て消去する
	void HitJadge() ;	//	当たり判定を行う
	void CreateEnemy();	//	待ち状態の敵を出現させる
	void MoveStage() ;	//	ステージ移動
	void Pause() ;	//	一時停止。プレイヤーが認識されていないとき。
	void Timeout() ;	//	時間切れ。
	void InitRanking() ;	//	すべてのランキングを0で初期化する
	void SaveScreenShot() ;
	void DeleteDrumcan() ;
};

ListIterator::ListIterator(){
	drumTimer = 0;
	kinectTime = 0;
	player = new KinectPlayer(PLAYER_DOT_IMAGE);
	PB = new PB_Base((bool)true);
	BG = new BackGround(BACKGROUND_GRAPHIC1,BACKGROUND_SPEED) ;
	E_Active = new EnemyBase((bool)true) ;
	E_Wait = new EnemyBase((bool)false) ;
	EB = new EnemyBulletBase((bool)true) ;
	crashP = new CrashPlayer(CRASH_PLAYER) ;
	Effect = new EffectBase((bool)true) ;
	PBom = new PlayerBomBase((bool)true) ;
	IB = new ItemBase((bool)true);
	time = 0;
	panel = LoadGraph(PANEL);	//	右パネルのロード
	grazeNum = 0;
	moveStageCnt = 0;
	pauseAlpha = 0 ;
	pauseAdd = 0 ;
	pauseGr = LoadGraph("graphic\\pause.png") ;
	sysTime = new systemTime(60);
	score = 0;
	timeoutInitialized = false;
	timeoutGrHandle = LoadGraph("graphic\\TimeOut.png");
	lookonRHand = false ;
	InitRanking();	//	ランキングを全て0で初期化
	for(int i = 0; i < 10; i++)	//	ランキングの初期化
		ranking[i] = (10-i)*5000;
	for(int i = 0; i < 60; i++){
		BaseImage[i].Width  = 640 ;
		BaseImage[i].Height = 480 ;
		CreateARGB8ColorData( &BaseImage[i].ColorData );              // キネクトからのデータは8bit*4なんでCreateFullColorDataじゃ駄目っぽい
        BaseImage[i].MipMapCount = 0 ;	// ミップマップではないので０
		WaitForSingleObject(ColorImageEv, INFINITE) ;
		CreateGraphicHandle(&ImageGrHandle[i], ColorImageStream, &BaseImage[i]);	//	グラフィックハンドルを生成する
	}
}
ListIterator::~ListIterator(){	//	newで確保したリストを解放
	delete player;
	delete crashP;
	while(PB->next_p != PB->first_p)
		delete PB->next_p;
	while(EB->next_p != EB->first_p)
		delete EB->next_p;
	while(E_Active->next_p != E_Active->activeFirst_p)
		delete E_Active->next_p;
	while(E_Wait->next_p != E_Wait->waitFirst_p)
		delete E_Wait->next_p;
	while(Effect->next_p != Effect->first_p)
		delete Effect->next_p;
	while(PBom->next_p != PBom->first_p)
		delete PBom->next_p;
	while(IB->next_p != IB->first_p)
		delete IB->next_p;
}
void ListIterator::InitRanking(){
	for(int i = 0; i < RANKING_MAX; i++)
		ranking[i] = 0;
}

void ListIterator::DeleteEnemyBullet(){
	PlayerBomBase *PBom_iter = PBom->next_p;
	for(EnemyBulletBase *EB_iter = EB->next_p; EB_iter!=EB_iter->first_p; EB_iter=EB_iter->next_p){
		new PB_Effect(EB_iter->x, EB_iter->y) ;	//	自機弾の着弾エフェクトを生成
		EB_iter->alive = false;	//	敵弾の生存フラグをfalseにする
	}
}

void ListIterator::DeleteDrumcan(){
	//	ドラム缶の消去処理
	EnemyBase *Enow_p = E_Active->next_p ;
	while(Enow_p != Enow_p->activeFirst_p){
		Enow_p = Enow_p->next_p ;
		delete Enow_p->rev_p;	
	}

}

void ListIterator::HitJadge(){
	//	自機と敵弾の当たり判定
	EnemyBulletBase *EB_iter ;
	if(player->alive==true && player->noHitTime==0){	
		for(EB_iter = EB->next_p; EB_iter!=EB_iter->first_p; EB_iter=EB_iter->next_p){
			/*
			//	自機墜落判定
			if(isHit(player->x, player->y, player->hitSize,
			EB_iter->x, EB_iter->y, EB_iter->size)){
			player->alive = false;
			PlaySoundMem(player->deathHandle, DX_PLAYTYPE_BACK);
			}
			//グレイズ判定
			else if(EB_iter->grazed == false && isHit(player->x, player->y, player->grazeSize,
			EB_iter->x, EB_iter->y, EB_iter->size)){
			grazeNum++;
			EB_iter->grazed = true;
			new GrazeEffect(player->x, player->y);	//	グレイズエフェクトを出す
			}
			*/
		}
	}
	//	自機と敵の当たり判定
	if(player->alive==true && player->noHitTime==0){	
		for(EnemyBase *E_iter = E_Active->next_p; E_iter!=E_iter->activeFirst_p; E_iter=E_iter->next_p){
			if(isHit(player->x, player->y, player->hitSize,
				E_iter->x, E_iter->y, E_iter->size / 3)){
					player->alive = false;
					PlaySoundMem(player->deathHandle, DX_PLAYTYPE_BACK);
			}
		}
	}

	//	敵と自機弾の当たり判定
	//	追加あり
	PB_Base *PB_iter = PB->next_p;
	EnemyBase *E_iter = E_Active->next_p;
	for(; E_iter!=E_iter->activeFirst_p; E_iter=E_iter->next_p){
		for(PB_iter = PB->next_p; PB_iter!=PB_iter->first_p; PB_iter=PB_iter->next_p){

			MV1_COLL_RESULT_POLY_DIM mv1_coll = MV1CollCheck_Sphere(E_iter->grHandle[0], -1, PB_iter->nowV, HAND_SIZE);
			if(E_iter->hitFlag == false && mv1_coll.HitNum > 0){
				E_iter->PlayCollisionSE();
				E_iter->initPos = MV1GetPosition( E_iter->grHandle[0] );	//	初期位置を現在位置に変更
				E_iter->initVelocity = VScale(VSub(PB_iter->nowV, PB_iter->revV), DRUM_COLLISION_SCALE) ;	//	手の動きをそのまま初速度に設定
				E_iter->initVelocity = VScale(E_iter->initVelocity, 60.0f);	//	手の動きを秒速に修正
				E_iter->realTimer = 0;	//	重力計算で使うタイマーを初期化する
				E_iter->hitFlag = true ;	//	2度手に触れないようにする
				VECTOR V = VSub(PB_iter->nowV, PB_iter->revV);	//	力の強さを計算
				float VT = (V.x * V.x) + (V.y * V.y) + (V.z * V.z) ;
				int i;
				for(i = 0; i < 4 ; i++){	//	ドラム缶のへこみ量を判定、for文だけでやったらバグったので修正
					if(drumcanStateBasis[i] > VT)
						E_iter->state = i;
				}
				KinectPlayer::p->stock -= E_iter->damage ;
				if(VSquare(E_iter->initVelocity) > 0.2f)new SerifEffect(E_iter->initPos,0) ;	//	ドゴ！！等を生成
				else new SerifEffect(E_iter->initPos, 4);	//	イテを生成
				int score = ScoreCalc(E_iter->initVelocity) ;
				this->score += score ;
				new ScoreNumEffect(PB_iter->nowV, score) ;
				//new PB_Effect(PB_iter->x, PB_iter->y) ;	//	自機弾の着弾エフェクトを生成
				//E_iter->alive = false;	//	敵の生存フラグをfalseにする
				//new E_DeleteEffect(E_iter->x, E_iter->y) ;	//	敵撃破エフェクトを生成

			}
			MV1CollResultPolyDimTerminate( mv1_coll );
		}
	}
	//	ボムと敵弾の当たり判定
	PlayerBomBase *PBom_iter = PBom->next_p;
	for(; PBom_iter!=PBom_iter->first_p; PBom_iter=PBom_iter->next_p){
		for(EB_iter = EB->next_p; EB_iter!=EB_iter->first_p; EB_iter=EB_iter->next_p){
			if(isHit(PBom_iter->x, PBom_iter->y, PBom_iter->size,
				EB_iter->x, EB_iter->y, EB_iter->size)){
					new PB_Effect(EB_iter->x, EB_iter->y) ;	//	自機弾の着弾エフェクトを生成
					EB_iter->alive = false;	//	敵弾の生存フラグをfalseにする

			}
		}
	}
	//	ボムと敵の当たり判定
	E_iter = E_Active->next_p;
	for(; E_iter!=E_iter->activeFirst_p; E_iter=E_iter->next_p){
		for(PBom_iter=PBom->next_p ; PBom_iter!=PBom_iter->first_p; PBom_iter=PBom_iter->next_p){
			if(isHit(PBom_iter->x, PBom_iter->y, PBom_iter->size,
				E_iter->x, E_iter->y, E_iter->size)){
					E_iter->hp -= PBom_iter->power;	//	ボムのパワー分だけ敵のHPを減らす
					new PB_Effect(PBom_iter->x, PBom_iter->y) ;	//	自機弾の着弾エフェクトを生成
					if(E_iter->hp < 0){
						E_iter->alive = false;	//	敵の生存フラグをfalseにする
						new E_DeleteEffect(E_iter->x, E_iter->y) ;	//	敵撃破エフェクトを生成
					}
			}
		}
	}
	//	アイテムを拾ったとき
	if(player->alive == true){	
		for(ItemBase *IB_iter = IB->next_p; IB_iter!=IB_iter->first_p; IB_iter=IB_iter->next_p){
			if(isHit(player->x, player->y, player->hitSize,
				IB_iter->x, IB_iter->y, IB_iter->size)){
					IB_iter->Get();
					IB_iter->alive = false;	//	アイテムの生存フラグをfalseにして削除する
					//	PlaySoundMem(player->deathHandle, DX_PLAYTYPE_BACK);
			}
		}
	}

}


void ListIterator::Move(){
	/*
	//	kinectのデータは2フレームに１回取得する	kinect:約30fps	ゲーム:60fpsのため
	kinectTime = (kinectTime + 1) % 2 ;
	if(kinectTime == 1){
	::WaitForSingleObject(SkeletonEv , INFINITE);	//	ボーン情報が更新されるまで待つ
	NuiSkeletonGetNextFrame(0, &playerSkeleton) ;	//	ボーン情報を取得
	}
	*/
	//	kinectのデータが更新された時だけデータを取得する
	if(::WaitForSingleObject(SkeletonEv , 0) == WAIT_OBJECT_0){
		kinectUpdateFlag = true ;
		NuiSkeletonGetNextFrame(0, &playerSkeleton) ;	//	ボーン情報を取得
		NuiTransformSmooth(&playerSkeleton, NULL) ;		//	取得したボーン情報にフィルタを掛ける
	}else kinectUpdateFlag = false ;
	
	/*
	//	カメラ情報の解放、及び再取得
	if(::WaitForSingleObject(ColorImageEv , 0) == WAIT_OBJECT_0){
		NuiImageStreamReleaseFrame( ColorImageStream, ImageFrame );
		ImageFrame->pFrameTexture->UnlockRect(0);
		NuiImageStreamGetNextFrame( ColorImageStream, 0, &ImageFrame ) ;
	}
	int igh = sysTime->remainingTime / 1000 ; 
	LoadImageForKinectCameraRGB( &ImageGrHandle[igh], ImageFrame, &BaseImage[igh] );	//	一秒ごとにカラーイメージを保存
	*/
	
	
	//	カメラ情報の解放、及び再取得(こっち使う）
	if(::WaitForSingleObject(ColorImageEv, 0) == WAIT_OBJECT_0){
		int igh = sysTime->remainingTime / 1000 ; 
		LoadImageForKinectCameraRGB( &ImageGrHandle[igh], ColorImageStream, &BaseImage[igh] );	//	一秒ごとにカラーイメージを保存
	}
	
	/*
	if(drumTimer % 60 == 0){
		ScreenShot[ScreenShotCnt % 120] = BaseImage[sysTime->remainingTime/1000];
		char Path[50];
		sprintf(Path, "ScreenShot_Buffa\\%d.bmp", ScreenShotCnt);
		//SaveBaseImageToPng(Path, &ScreenShot[ScreenShotCnt % 120], 1);
		//SaveBaseImageToPng(Path, &BaseImage[sysTime->remainingTime/1000], 1) ;
		//SaveBaseImageToJpeg(Path, &BaseImage[sysTime->remainingTime/1000], 20, TRUE);
		//SaveBaseImageToBmp(Path, &BaseImage[sysTime->remainingTime/1000]);
		SaveBaseImageToBmp(Path, &ScreenShot[ScreenShotCnt % 120]);
		//SaveBaseImageToJpeg(Path, &ScreenShot[ScreenShotCnt % 120], 30, TRUE);
		ScreenShotCnt++;
	}
	*/
	HitJadge() ;	//	当たり判定をする

	sysTime->Move();

	//	経過時間を取得、更新
	NowTime = GetNowCount();
	IntervalTime = NowTime - RevTime ;	//	IntervalTimeは経過時間を保存しておく場所
	RevTime = NowTime ;

	int i ;
	//BG->Move() ;		//	背景のスクロール処理
	if(player->alive == true)
		player->Move() ;	//	自機の移動処理
	else if(player->alive == false){
		crashP->Move() ;
		DeleteEnemyBullet() ;
	}
	
	PB_Base *PBnow_p = PB->next_p ;
	//	自機弾の移動処理
	while(PBnow_p != PB->first_p){
		PBnow_p->Move() ;
		if(PBnow_p->alive == 0)
		{
			PBnow_p = PBnow_p->next_p ;
			delete PBnow_p->rev_p;
		}
		else{
			PBnow_p = PBnow_p->next_p ;
		}
	}
	/*
	//	敵弾の移動処理
	EnemyBulletBase *EBnow_p = EB->next_p ;
	while(EBnow_p != EB->first_p){
		EBnow_p->Move() ;
		if(EBnow_p->alive == false)
		{
			EBnow_p = EBnow_p->next_p ;
			delete EBnow_p->rev_p;
		}
		else{
			EBnow_p = EBnow_p->next_p ;
		}
	}
	*/
	//	敵の移動処理
	EnemyBase *Enow_p = E_Active->next_p ;
	while(Enow_p != Enow_p->activeFirst_p){
		Enow_p->Move() ;
		if(Enow_p->alive == 0)
		{
			Enow_p = Enow_p->next_p ;
			delete Enow_p->rev_p;
		}
		else{
			Enow_p = Enow_p->next_p ;
		}
	}
	//	エフェクトの拡大処理
	EffectBase *EffectNow_p = Effect->next_p ;
	while(EffectNow_p != Effect->first_p){
		EffectNow_p->Move() ;
		if(EffectNow_p->alive == 0)
		{
			EffectNow_p = EffectNow_p->next_p ;
			delete EffectNow_p->rev_p;
		}
		else{
			EffectNow_p = EffectNow_p->next_p ;
		}
	}
	/*
	//	プレイヤーボムを移動させる
	PlayerBomBase *PBomNow_p = PBom->next_p ;
	while(PBomNow_p != PBom->first_p){
		PBomNow_p->Move() ;
		if(PBomNow_p->alive == 0)
		{
			PBomNow_p = PBomNow_p->next_p ;
			delete PBomNow_p->rev_p;
		}
		else{
			PBomNow_p = PBomNow_p->next_p ;
		}
	}
	//	アイテムの移動処理
	ItemBase *IBnow_p = IB->next_p ;
	while(IBnow_p != IB->first_p){
		IBnow_p->Move() ;
		if(IBnow_p->alive == false)
		{
			IBnow_p = IBnow_p->next_p ;
			delete IBnow_p->rev_p;
		}
		else{
			IBnow_p = IBnow_p->next_p ;
		}
	}
	*/
	//	テスト環境、ドラム缶を出現させる
	if(CheckHitKey(KEY_INPUT_RIGHT) && !rKey){
		CameraPosition.x += 0.1f; // カメラを右に masuda
		rKey = true;
	}else if(!CheckHitKey(KEY_INPUT_RIGHT))rKey = false;
	if(CheckHitKey(KEY_INPUT_LEFT) && !lKey){
		CameraPosition.x -= 0.1f;  // masuda
		lKey = true;
	}else if(!CheckHitKey(KEY_INPUT_LEFT))lKey = false;
	if(CheckHitKey(KEY_INPUT_UP) && !uKey){
		CameraPosition.y += 0.1f; // masuda カメラを上に
		uKey = true;
	}else if(!CheckHitKey(KEY_INPUT_UP))uKey = false;
	if(CheckHitKey(KEY_INPUT_DOWN) && !dKey){
		CameraPosition.y -= 0.1f; // masuda
		dKey = true;
	}else if(!CheckHitKey(KEY_INPUT_DOWN))dKey = false;
	//	とりあえず、速度はダミーデータが入ってます

	if (rKey || lKey || uKey || dKey) {
		SetCameraPositionAndTargetAndUpVec( 
		VAdd(CameraPosition,CameraShake), // カメラの位置　初期位置は、高さ１メートル、原点より５メートル後ろ
		VGet( XORG, YORG, ZORG ),	//　原点をみている(ここにkinectプレーやがいるはず)
		VGet( 0.0f, 1.0f, 0.0f ) ) ; // カメラの傾き
	}

	drumTimer = (drumTimer + 1) % 1200 ;
	//drumTimer++;
	//	普通のドラム缶出現

	int span =  sysTime->remainingTime * (120 - 35)/(60000 - 0) + 35;
	//if (span < 35) span=35;
	//span = 10;
	if(drumTimer % span == 0) { 
			// x方向初速度0,ｙ方向初速度11m/秒,z方向初速度-3.9m/秒
		float x0 =   0.0f + ((GetRand(100)-50)/100.0f)*1.5f;
		float y0 =  11.0f + ((GetRand(100)-50)/100.0f)*0.5f;
		float z0 =  -5.5f + ((GetRand(100)-50)/100.0f)*1.0f;
		new NormalDrum(time, VGet(XORG, YORG, 12.0f),VGet(x0,y0,z0)); // masuda
	}
		/*
	if(drumTimer % 120 == 0) { 
	
		// x方向初速度0,ｙ方向初速度11m/秒,z方向初速度-3.9m/秒
		float x0 =   0.0f + ((GetRand(100)-50)/100.0f)*1.5f;
		float y0 =  11.0f + ((GetRand(100)-50)/100.0f)*0.5f;
		float z0 =  -5.5f + ((GetRand(100)-50)/100.0f)*1.0f;
		new NormalDrum(time, VGet(XORG, YORG, 12.0f),VGet(x0,y0,z0)); // masuda
	}
	//	残り２０秒以下になったら、ドラム缶の出現間隔を早める
	if((drumTimer+60) % 120 == 0 && sysTime->remainingTime < 20000){
		// x方向初速度0,ｙ方向初速度11m/秒,z方向初速度-3.9m/秒
		float x0 =   0.0f + ((GetRand(100)-50)/100.0f)*1.5f;
		float y0 =  11.0f + ((GetRand(100)-50)/100.0f)*0.5f;
		float z0 =  -5.5f + ((GetRand(100)-50)/100.0f)*1.0f;
		new NormalDrum(time, VGet(XORG, YORG, 12.0f),VGet(x0,y0,z0)); // masuda
	}
	//	残り１０秒以下でドラム缶の出現数が２倍になる（同時に２個出現する）
	if((drumTimer) % 60 == 0 && sysTime->remainingTime < 10000){
		// x方向初速度0,ｙ方向初速度11m/秒,z方向初速度-3.9m/秒
		float x0 =   0.0f + ((GetRand(100)-50)/100.0f)*1.5f;
		float y0 =  11.0f + ((GetRand(100)-50)/100.0f)*0.5f;
		float z0 =  -5.5f + ((GetRand(100)-50)/100.0f)*1.0f;
		new NormalDrum(time, VGet(XORG, YORG, 12.0f),VGet(x0,y0,z0)); // masuda
	}
	//	ダメージドラム出現
	if(drumTimer == 0) {
		float x0 =   0.0f + ((GetRand(100)-50)/100.0f)*2.0f;
		float y0 =  11.0f + ((GetRand(100)-50)/100.0f)*0.5f;
		float z0 =  -5.5f + ((GetRand(100)-50)/100.0f)*1.0f;
		new NormalDrum(time, VGet(XORG, YORG, 12.0f),VGet(x0,y0,z0)); // masuda
	}
	*/
	CreateEnemy() ;	//	敵を出現させる
}

void ListIterator::Draw(){
	int i, j ;
	//BG->Draw() ;		//	背景の描画処理

	if(player->alive == true)
		player->Draw() ;	//	自機の描画処理
	else if(player->alive == false)
		crashP->Draw() ;
	/*
	ItemBase *IBnow_p = IB->next_p ;
	//	アイテムの描画処理
	while(IBnow_p != IB->first_p){
		IBnow_p->Draw() ;
		IBnow_p = IBnow_p->next_p ;
	}
	*/
	PB_Base *PBnow_p = PB->next_p ;
	//	自機弾の描画処理
	while(PBnow_p != PB->first_p){
		PBnow_p->Draw() ;
		PBnow_p = PBnow_p->next_p ;
	}
	//	敵の描画処理
	EnemyBase *Enow_p = E_Active->next_p ;
	while(Enow_p != E_Active->activeFirst_p){
		Enow_p->Draw() ;
		Enow_p = Enow_p->next_p ;
	}
	/*
	EnemyBulletBase *EBnow_p = EB->next_p ;
	//	敵弾の描画処理
	while(EBnow_p != EB->first_p){
		EBnow_p->Draw() ;
		EBnow_p = EBnow_p->next_p ;
	}
	//	プレイヤーボムの描画処理
	PlayerBomBase *PBomNow_p = PBom->next_p ;
	while(PBomNow_p != PBom->first_p){
		PBomNow_p->Draw() ;
		PBomNow_p = PBomNow_p->next_p ;
	}
	*/
	//	エフェクトの描画処理
	EffectBase *EffectNow_p = Effect->next_p ;
	while(EffectNow_p != Effect->first_p){
		EffectNow_p->Draw() ;
		EffectNow_p = EffectNow_p->next_p ;
	}
	/*
	//	残りライフ表示
	DrawString(0, 0, "残りライフ：",GetColor(255,255,255), GetColor(255,255,255));
	for(i = 0; i < player->stock; i++)	//	残機アイコンの表示
		DrawGraph(50 + i * 30, 10, player_icon, TRUE);
		*/

	
	sysTime->Draw(300, 0);	//	残り時間を表示
	SetFontSize(50) ;
	DrawFormatString(830, 0, GetColor(255,255,255), "%06d", score) ;

	int secondTime = (sysTime->remainingTime + 100) / 1000 ;	//	0.1秒前のデータを見て描画する（カクつき防止）
	DrawExtendGraph(800, 200, 1000, 350, ImageGrHandle[secondTime], FALSE) ;
	/*
	DrawGraph(0, 0, panel, TRUE);	//	右パネルを描画
	for(i = 0; i < player->stock; i++)	//	残機アイコンの表示
	DrawGraph(480 + i * 30, 110, player_icon, TRUE);
	for(i = 0; i < player->bom; i++)	//	残ボムアイコンの表示
	DrawGraph(480 + i * 30, 195, bom_icon, TRUE);
	*/
	/*
	SetFontSize(25);
	//	グレイズ数の表示
	char grazeNumS [20];
	sprintf(grazeNumS, "%010d", grazeNum) ;	//	sprintf関数は出力先が文字列になっただけ
	DrawString(420, 320, "Graze",GetColor(0,0,0), GetColor(255,255,255)) ;	//	「Graze」を描画 
	DrawString(480, 360, grazeNumS , GetColor(0,0,0) , GetColor(255,255,255));	//	グレイズ数の描画
	//	スコアの表示
	char scoreNumS [20];
	sprintf(scoreNumS, "%010d", Score) ;
	DrawString (420, 400, "Score", GetColor(0,0,0), GetColor(255,255,255)) ;	//	「Score」を描画
	DrawString (480, 440, scoreNumS , GetColor(0,0,0), GetColor(255,255,255));	//	スコアを描画
	*/
}
//	決められた時間になったら敵を出す
void ListIterator::CreateEnemy(){
	time++ ;
	//	ボスのみになっていたら、待ち時間なしでボスを出す。
	//	詳細条件	：約２秒ごとに待ち状態の敵がボスのみ、フィールドに敵がいないか調べる。
	//	trueだったら、ボスの出現時間を現在の時間に書き換える（約1秒遅延させて出現）
	if((time % 120) == 0 && (E_Wait->next_p->next_p == E_Wait) &&
		(E_Active->next_p == E_Active)){
			//time = E_Wait->next_p->waitTime - 60;
			E_Wait->next_p->waitTime = time + 100;
	}
	//	待ち状態の敵をチェック、条件にあっていたら画面に出現させる
	if(E_Wait->waitFirst_p != E_Wait->next_p){
		EnemyBase *EBp = E_Wait ;
		for(;;){
			if(EBp->next_p->waitTime == time){
				EBp->next_p->Activation() ;
			}
			else
				return ;
		}
	}
	return ;
}
void ListIterator::MoveStage()
{
	moveStageCnt++ ;
	if(moveStageCnt < 240){
		SetFontSize(50) ;
		DrawString(50, 100, "Stage Clear!!", GetColor(0,50,50), GetColor(255,255,255));
		SetFontSize(20) ;
		DrawString(200, 300, "To Next Stage...", GetColor(100,100,100), GetColor(255,255,255)) ;
	}
	else if(moveStageCnt < 495){
		BG->alpha-- ;	//	徐々に背景を暗くする
	}
	//	次ステージの背景を読み込む
	else if(moveStageCnt == 495){
		DeleteGraph(BG->graphicHandle) ;
		switch(nowStage){
		case 1:
		case 11:
			nowStage++;	//	現在のステージを次のステージに移行させる
			BG = new BackGround(BACKGROUND_GRAPHIC2, BACKGROUND_SPEED) ;
			BG->alpha = 0;
			break ;
		default :
			break ;
		}
	}
	//	黒色から徐々にステージを明るくする
	else if(moveStageCnt < 750){
		BG->alpha++ ;
	}
	//	ステージの初期化処理＆敵データ読み込み
	else{
		time = 0;
		StageClearFlag = false ;
		//LoadEnemyData(nowStage) ;
	}
}

void ListIterator::Pause(){
	Draw() ;	//	背景を描画
	DrawAlpha(0, 0, pauseAlpha, pauseGr) ;	//	ポーズ画面を描画
	const int alphaSpeed = 2 ;
	if(pauseAlpha == 0 && pauseAdd == 0 && SkeletonTracking == false){	//	通常状態からポーズへ移行した際の一周目
		pauseAdd = alphaSpeed;
	}else if(pauseAdd > 0){	//	徐々にポーズ画面を出している途中
		pauseAlpha += pauseAdd ;
		if(::WaitForSingleObject(SkeletonEv , 0) == WAIT_OBJECT_0){
			NuiSkeletonGetNextFrame(0, &playerSkeleton) ;	//	ボーン情報を取得
			for(int i = 0; i < 6; i++){
				if(playerSkeleton.SkeletonData[i].eTrackingState != NUI_SKELETON_NOT_TRACKED)
					pauseAdd = -alphaSpeed ;	//	ポーズを解除するか判定
			}
		}
		if(pauseAlpha > 255){	//	ポーズ画面が完全に出現 
			pauseAlpha = 255 ;
			pauseAdd = 0;
		}
	}else if(pauseAdd < 0){	//	徐々にポーズを解除中
		pauseAlpha += pauseAdd ;
		if(pauseAlpha < 0){	//	ポーズ画面を完全に解除
			pauseAlpha = 0;
			pauseAdd = 0;
			SkeletonTracking = true ;
			sysTime->revRetime();	//	残り時間計測のリフレッシュ
			DeleteDrumcan();
		}
	}else if(pauseAlpha == 255){	//	ポーズ中
		if(::WaitForSingleObject(SkeletonEv , 0) == WAIT_OBJECT_0){
			NuiSkeletonGetNextFrame(0, &playerSkeleton) ;	//	ボーン情報を取得
			for(int i = 0; i < 6; i++){
				if(playerSkeleton.SkeletonData[i].eTrackingState != NUI_SKELETON_NOT_TRACKED)
					pauseAdd = -alphaSpeed ;	//	ポーズを解除するか判定
			}
		}
	}
}

//	時間切れ
void ListIterator::Timeout(){
	if(timeoutInitialized == false){
		playerRank = RANKING_MAX - 1;
		for(int i = 0; i < RANKING_MAX; i++){	//	プレイヤーの順位を調べる
			if(ranking[i] < score){
				playerRank = i;
				break;
			}
		}
		for(int i = 9; i >= playerRank; i--){	//	プレイヤーの順位から下をずらして挿入
			ranking[i] = ranking[i - 1];
		}
		ranking[playerRank] = score ;

		for(int i = 0; i < 10; i++)	//	yの初期値を入れる
			rankPosY[i] = -900 + i * 60 ;
		timeoutX = 0;	timeoutY = -WINDOWSIZE_Y;
		rankTimer = 0;	//	ランキングの表示に使う制御変数の初期化
		timeoutInitialized = true;
		displayMove = 0;
		displayPos = 0;
		lookonRHand = false;
	}
	if(timeoutY < 0){	//	タイムアップという板を移動させる
		if(timeoutY > -10)timeoutY += 10;
		else timeoutY = 0;

		DrawGraph(timeoutX, timeoutY, timeoutGrHandle, TRUE) ;
		DrawString(400, timeoutY + 400, "タイムアップ！！", GetColor(255,255,255));
	}
	else if(rankTimer < 120){	//	待ち時間
		rankTimer++;

		DrawGraph(timeoutX, timeoutX, timeoutGrHandle, TRUE) ;
		DrawString(400, timeoutY + 400, "タイムアップ！！", GetColor(255,255,255));
	}
	else if(rankTimer < 360){	//	４秒間(240フレーム)の間にランキングを上からスライドインさせる
		DrawGraph(timeoutX, timeoutX, timeoutGrHandle, TRUE) ;	
		for(int i = 0; i < 10; i++){
			rankPosY[i] += WINDOWSIZE_Y / 180;
			SetFontSize(40);
			DrawFormatString(RANKING_SCORE_POSITION_X, rankPosY[i], GetColor(255,255,255), "%4d位　　%06d", i+1, ranking[i]) ;
		}
		rankTimer++;
	}
	else if(rankTimer < 500){//	ランキング表示をする
		rankTimer++;
		DrawGraph(timeoutX, timeoutX, timeoutGrHandle, TRUE) ;
		SetFontSize(40);
		for(int i = 0; i < 10; i++)
			DrawFormatString(RANKING_SCORE_POSITION_X, rankPosY[i], GetColor(255,255,255), "%4d位　　%06d", i+1, ranking[i]) ;
		if(playerRank >= 10){	//	プレイヤーの順位が11位以下だったら、11位の位置に表示
			if(rankTimer % 20 < 10)	DrawFormatString(RANKING_SCORE_POSITION_X, 650, GetColor(255,20,20), "%4d位　　%06d", playerRank +1, ranking[playerRank]) ;
			else					DrawFormatString(RANKING_SCORE_POSITION_X, 650, GetColor(255,255,255), "%4d位　　%06d", playerRank +1, ranking[playerRank]) ;
		}
		else if(rankTimer % 20 < 10)	//	自分のランクを点滅表示をする
			DrawFormatString(RANKING_SCORE_POSITION_X, rankPosY[playerRank], GetColor(255,20,20), "%4d位　　%06d", playerRank +1, ranking[playerRank]) ;
	}
	else{	//	ランキングの表示が終わった場合	
		rankTimer++;
		DrawGraph(timeoutX, timeoutX, timeoutGrHandle, TRUE) ;
		SetFontSize(40);
		for(int i = 0; i < 10; i++)
			DrawFormatString(displayPos +RANKING_SCORE_POSITION_X, rankPosY[i], GetColor(255,255,255), "%4d位　　%06d", i+1, ranking[i]) ;
		if(playerRank >= 10){	//	プレイヤーの順位が11位以下だったら、11位の位置に表示
			if(rankTimer % 20 < 10)	DrawFormatString(displayPos +RANKING_SCORE_POSITION_X, 650, GetColor(255,20,20), "%4d位　　%06d", playerRank +1, ranking[playerRank]) ;
			else					DrawFormatString(displayPos +RANKING_SCORE_POSITION_X, 650, GetColor(255,255,255), "%4d位　　%06d", playerRank +1, ranking[playerRank]) ;
		}
		else if(rankTimer % 20 < 10)	//	自分のランクを点滅表示をする
			DrawFormatString(displayPos +RANKING_SCORE_POSITION_X, rankPosY[playerRank], GetColor(255,20,20), "%4d位　　%06d", playerRank +1, ranking[playerRank]) ;
		SetFontSize(30);
		DrawString(displayPos +150, 700, "再プレイするには、両手を頭より上に上げてください。", GetColor(255,255,255));
		//	kinectのデータが更新された時だけデータを取得する
		if(::WaitForSingleObject(SkeletonEv , 0) == WAIT_OBJECT_0){
			kinectUpdateFlag = true ;
			NuiSkeletonGetNextFrame(0, &playerSkeleton) ;	//	ボーン情報を取得
			NuiTransformSmooth(&playerSkeleton, NULL) ;		//	取得したボーン情報にフィルタを掛ける
		}else kinectUpdateFlag = false ;
		//	スクリーンショットの描画
		DrawExtendGraph(displayPos -1000, 100, displayPos -600, 400, ImageGrHandle[10], FALSE);
		DrawExtendGraph(displayPos -500, 100, displayPos -100, 400, ImageGrHandle[30], FALSE);
		DrawExtendGraph(displayPos -1000, 450, displayPos -600, 750, ImageGrHandle[40], FALSE);
		DrawExtendGraph(displayPos -500, 450, displayPos -100, 750, ImageGrHandle[50], FALSE);

		VECTOR head, RHand, LHand, center;
		GetSkeletonV(playerSkeleton, &head, NUI_SKELETON_POSITION_HEAD);
		GetSkeletonV(playerSkeleton, &RHand, NUI_SKELETON_POSITION_HAND_RIGHT);
		GetSkeletonV(playerSkeleton, &LHand, NUI_SKELETON_POSITION_HAND_LEFT);
		//	両手を上げていたら
		if(head.y < RHand.y && head.y < LHand.y){
			rankTimer = 0;
			timeoutInitialized = false;
			sysTime->initTime(60);
			score = 0;
			ScreenShotCnt = 0;
			DeleteDrumcan();
		}
		GetSkeletonV(playerSkeleton, &center, NUI_SKELETON_POSITION_SPINE) ;
		//	右手を前に出したら
		if((center.z-RHand.z) > 0.3 && (center.z-LHand.z) > 0.3 &&!lookonRHand){	//	右手を初めて前に出したら
			revRHand = RHand ;
			lookonRHand = true ;
			displayMoveCouse = 0;
		}else if((center.z-RHand.z) > 0.3 && lookonRHand){	//	右手を前に出して動かしたら
			displayMove = (RHand.x - revRHand.x) * 500;
			displayMoveCouse += (RHand.x - revRHand.x);
			revRHand = RHand ;
		}else{	//	手を後ろに戻したら、右手の捕捉を解除する
			if(displayMoveCouse > 0) displayMove = 30;
			else if(displayMoveCouse < 0)displayMove = -30;
			displayMoveCouse = 0;
			lookonRHand = false ;		
		}
		displayPos += displayMove;	//	表示画面を動かす
		//	画面の位置を規定の位置に戻す
		if(displayPos > WINDOWSIZE_X) displayPos = WINDOWSIZE_X;
		else if(displayPos < 0)displayPos = 0 ;

		//	ctrl + S でセーブ画面以降
		if((CheckHitKey(KEY_INPUT_RCONTROL) || CheckHitKey(KEY_INPUT_LCONTROL)) && CheckHitKey(KEY_INPUT_S))
			SaveScreenShot();
	}
}

void ListIterator::SaveScreenShot(){

	//	ヘッダー
	unsigned char heder[] = {
		0x42, 0x4D, 0x36, 0x10, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,
		0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 0xE0, 0x01, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x10, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
		unsigned char *BmpData = new unsigned char[3000000];
		for(int i = 0; i < (16*3+6); i++)
			BmpData[i] = heder[i];	//	ヘッダーをコピー	
		
		int captureGr = MakeGraph( 1024, 768 ) ;
		char folderName[50];
		GetDrawScreenGraph(0, 0, 1024, 768, captureGr) ;
		DrawGraph(0, 0, captureGr, FALSE) ;
		while(!ScreenFlip() && !ClearDrawScreen() && !ProcessMessage() ){
			DrawGraph(0, 0, captureGr, FALSE) ;
			DrawBox(300, 400, 700, 700, GetColor(255,255,255), TRUE) ;
			SetFontSize(40);
			DrawString(320, 400, "フォルダ名を入力してください(Escキーを押すと保存しない)", GetColor(0, 0, 0)) ;
			//	半角文字列を入力。Escキーで終了する
			if(KeyInputSingleCharString( 340, 460, 40, folderName, TRUE) == 2)	
				return;
			if(!PathIsDirectory(TEXT(folderName))){
				CreateDirectory(TEXT(folderName), NULL) ;
				break;
			}else{
				DrawString(300, 500, "同じ名前のフォルダが存在します。別の名前にしてください。", GetColor(255,0,0));
				ScreenFlip();
				WaitKey();
			}
		}
		
		//	画像保存処理
		/*
		char Path[50];
		for(int i = 0; i < 60; i++){
			char *souceData = (char*)BaseImage[i].GraphData;

			int e = 54;
			for(int j = 480-1; j >= 0; j--){
				for(int k = 0; k < 640*4; k++){
					if(k % 4 != 3){
						BmpData[e] = souceData[(BaseImage[i].Pitch*j)+(k)];
						e++;
					}
				}
			}

			FILE *fp;
			char Path[50];

			sprintf(Path,"%s\\%d.bmp",folderName,i);
			fp = fopen(Path, "wb");
			fwrite(BmpData, 1, (640*480*3)+(16*3+6), fp) ;
			fclose(fp);

		}
		for(int i = 0; i < 60; i++){
			sprintf(Path, "%s\\DX%d.jpg", folderName, i);
			SaveBaseImageToJpeg(Path, &BaseImage[i], 100, TRUE);
		}
		*/
		//	バッファを指定したフォルダにjpeg変換してコピー
		/*
		FILE *InFp;
		char InPath[50];
		char OutPath[50];
		BASEIMAGE BmpToJpeg = BaseImage[10];
		for(int i = 1; i < 360; i++){
			sprintf(InPath, "ScreenShot_Buffa\\%d.bmp", i);
			sprintf(OutPath, "%s\\DX%d.jpg", folderName, i);
			InFp = fopen(InPath, "rb");
			fread(BaseImage[0].GraphData, 1, (640*480*3)+(16*3+6), InFp);
			SaveBaseImageToJpeg(OutPath, &BaseImage[0], 30, TRUE);
			fclose(InFp);
		}
		*/
		//	バッファフォルダの画像をコピー

		char soucePath[30];
		char destPath[30];
		for(int i = 0; i < SCREENSHOT_NUM; i++){
			sprintf(soucePath, "ScreenShot_Buffa\\%d.bmp", i);
			sprintf(destPath,  "%s\\%d.bmp", folderName, i)  ;
			CopyFile(soucePath, destPath, FALSE) ;
		}

		DrawString(300, 500, "正常に保存しました", GetColor(255,255,255));
		ScreenFlip();
		WaitKey();

}


/////////////////////////////////////
///////main関数//////////////////////
/////////////////////////////////////
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow )
{
	
	//ChangeWindowMode( TRUE ) ;		//	ウィンドウで開く

	SetGraphMode(WINDOWSIZE_X, WINDOWSIZE_Y, 32);	//	ウィンドウサイズ設定
	
	SetMainWindowText("ドラム缶クラッシュ！！");
	if( DxLib_Init() == -1 )		// ＤＸライブラリ初期化処理
	{
		return -1 ;			// エラーが起きたら直ちに終了
	}
	
	

	SetDrawScreen( DX_SCREEN_BACK ) ; //描画先を裏画面へ
	ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE) ;	//	アンチエイリアス＆エッジ付きフォント
	//	モデル以外にもZバッファを使う
	SetUseZBuffer3D(TRUE) ;
	SetWriteZBuffer3D(TRUE) ;


	//	kinectの初期化
	HRESULT hr = NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX |
		NUI_INITIALIZE_FLAG_USES_SKELETON | NUI_INITIALIZE_FLAG_USES_COLOR);
	if(FAILED(hr))
		return -1;
	SkeletonEv = ::CreateEventA(NULL, TRUE, FALSE, NULL);
	hr = NuiSkeletonTrackingEnable(SkeletonEv, 0) ;
	if(FAILED(hr))
		return -1;
	//	RGBカメラの初期化
	ColorImageEv = ::CreateEventA(NULL, TRUE, FALSE, NULL);
	hr = NuiImageStreamOpen( NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, 0, 2, ColorImageEv, &ColorImageStream );
	if(FAILED(hr))
		return -1;
	//WaitForSingleObject(ColorImageEv, INFINITE) ;	//	カラーイメージの更新を待つ
	//NuiImageStreamGetNextFrame(ColorImageStream, 0, &ImageFrame);
	/*
		Range = 2000.0f ;
	Atten0 = 0.0f ;
	Atten1 = 0.0006f ;
	Atten2 = 0.0f ;
	*/
	ChangeLightTypePoint( VGet(0, 5.0f, -5.0f), 100.0f, 0.8f, 0.001f, 0.0001f);
	// masuda	
	//ChangeLightTypeDir( VGet( 0.5f, -0.5f, 0.0f  ) ) ;	// ディレクショナルライトの方向
	SetGlobalAmbientLight( GetColorF( 1.0f, 0.0f, 0.0f, 0.0f ) ) ;	// グローバルアンビエントカラーを赤色に変更
	CameraPosition = VGet( XORG, YORG+3.0f, -5.0f); // カメラの位置　高さ5メートル、原点より５メートル後ろ

	//---------------
	//	タイトル画面
	int startGr = LoadGraph("graphic\\start.png");
	int creditGr = LoadGraph("graphic\\credit.png");
	DrawGraph(0, 0, startGr, TRUE) ;
	ScreenFlip() ;
	ClearDrawScreen() ;
	//	待ち時間
	for(int i = 0; i < 150 && !CheckHitKey(KEY_INPUT_RETURN) && !ProcessMessage(); i++){	//	エンターキーで待ち時間をスキップ可能にする
		WaitTimer(10) ;
	}

	//-------------------------
	//	クレジット画面に移行
	for(int i = 0; i < 255 && !ProcessMessage() && !ScreenFlip() 
		&&!ClearDrawScreen() ; i += 3 ){
			DrawAlpha(0, 0, i, creditGr) ;
			DrawAlpha(0, 0, 255 - i, startGr) ;
	}

	//------------------------------------------------


		//	画像関係読み込み
		/*
		Player player(PLAYER_DOT_IMAGE) ;	//プレイヤー画像の読み込み
		PB_Straight::graphicHandle = LoadGraph(PLAYER_BULLET_GRAPHIC) ;	//	プレイヤーの弾グラフィック読み込み
		P_StarBom::grHandle = LoadGraph(PLAYER_BOM) ;		//	プレイヤーのボムグラフィック
		Player::BE_grHandle = LoadGraph(PLAYER_BOM_EFFECT) ; //	ボム無敵時に表示する魔方陣
		E_mStraight_bStraight::GrHandle = LoadGraph(STRAIGHT_ENEMY) ; //	敵グラフィック読み込み
		E_mStraight_bSpin::GrHandle = LoadGraph(SPIN_ENEMY) ;	//	回転弾幕の敵を読込
		E_mStraight_bNway_f::GrHandle = LoadGraph(NWAY_ENEMY) ;	//	固定nway弾幕の敵を読込
		EB_Sinple::grHandle = LoadGraph(ENEMY_MIDLE_BULLET)	;		//	弾グラフィックの読み込み
		EB_Small::grHandle = LoadGraph(ENEMY_SMALL_BULLET) ;		//	敵米粒弾
		EB_Fire::grHandle = LoadGraph(ENEMY_FIRE_BULLET) ;		//	敵炎弾
		PB_Effect::grHandle = LoadGraph(PB_EFFECT_NAME) ;	//	自機弾エフェクトの読み込み
		GrazeEffect::grHandle = LoadGraph(GRAZE_EFFECT_NAME) ; // 自機グレイズエフェクトの読み込み
		E_DeleteEffect::grHandle = LoadGraph(E_DEL_NAME) ;	//	敵撃破エフェクトの読み込み
		
		bom_icon = LoadGraph(BOM_ICON) ;	//	右パネルの残ボム画像読込
		HpHandle = LoadGraph(HP_GRAPH) ;	//	ボスのHPゲージを読込
		ScoreItem::grHandle = LoadGraph(TEN_ITEM) ;	//	得点アイテムを読込
		BomItem::grHandle = LoadGraph(BOM_ITEM) ; // ボムアイテム読込
		LifeupItem::grHandle = LoadGraph(LIFEUP_ITEM) ;	//	１ＵＰアイテム読込
		E_Boss_Fire::grHandle = LoadGraph(ENEMY_BOSS_FIRE) ;	//	１面のボスを読込
		//	２面の敵
		E_mStraight_bNway::GrHandle = LoadGraph(H_NWAY_ENEMY) ;
		*/
	player_icon = LoadGraph(PLAYER_ICON) ;	//	右パネルの残機画像読み込み
	
	//	ドラム缶読み込み
	NormalDrum::loadOnlyHr[0][0] = MV1LoadModel(MODEL_DIR "drum0_g.x");
	NormalDrum::loadOnlyHr[0][1] = MV1LoadModel(MODEL_DIR "drum1_g.x");
	NormalDrum::loadOnlyHr[0][2] = MV1LoadModel(MODEL_DIR "drum2_g.x");
	NormalDrum::loadOnlyHr[0][3] = MV1LoadModel(MODEL_DIR "drum3_g.x");
	NormalDrum::loadOnlyHr[1][0] = MV1LoadModel(MODEL_DIR "drum0_b.x");
	NormalDrum::loadOnlyHr[1][1] = MV1LoadModel(MODEL_DIR "drum1_b.x");
	NormalDrum::loadOnlyHr[1][2] = MV1LoadModel(MODEL_DIR "drum2_b.x");
	NormalDrum::loadOnlyHr[1][3] = MV1LoadModel(MODEL_DIR "drum3_b.x");
	NormalDrum::loadOnlyHr[2][0] = MV1LoadModel(MODEL_DIR "drum0_y.x");
	NormalDrum::loadOnlyHr[2][1] = MV1LoadModel(MODEL_DIR "drum1_y.x");
	NormalDrum::loadOnlyHr[2][2] = MV1LoadModel(MODEL_DIR "drum2_y.x");
	NormalDrum::loadOnlyHr[2][3] = MV1LoadModel(MODEL_DIR "drum3_y.x");
	NormalDrum::loadOnlyHr[3][0] = MV1LoadModel(MODEL_DIR "drum0_r.x");
	NormalDrum::loadOnlyHr[3][1] = MV1LoadModel(MODEL_DIR "drum1_r.x");
	NormalDrum::loadOnlyHr[3][2] = MV1LoadModel(MODEL_DIR "drum2_r.x");
	NormalDrum::loadOnlyHr[3][3] = MV1LoadModel(MODEL_DIR "drum3_r.x");
	//	赤ドラム缶の読み込み（ダメージ判定あり）
	DamageDrum::loadOnlyHr[0] = MV1LoadModel(MODEL_DIR "drum0_r.x");
	DamageDrum::loadOnlyHr[1] = MV1LoadModel(MODEL_DIR "drum2_r.x");
	//	丸影を読み込み
	circleShadow = MV1LoadModel(MODEL_DIR "shade.x") ;
	int floorId =MV1LoadModel("floor.x"); // y=0にフロアを設定(確認のため)
	//	火花読み込み
	LoadDivGraph("graphic\\spark.png", 10, 5, 2, 192, 142, SparkEffect::grHandle);
	//	セリフエフェクト読み込み
	SerifEffect::grHandle[0] = LoadGraph("graphic\\word.png") ;
	SerifEffect::grHandle[4] = LoadGraph("graphic\\ide.png") ;

	//	効果音読み込み
	NormalDrum::collisionSE = LoadSoundMem("sound\\punch1.mp3") ;
	NormalDrum::boundSE = LoadSoundMem("sound\\fall1.mp3") ;
	NormalDrum::shotSE = LoadSoundMem("sound\\fly1.mp3") ;

	

	
TITLE_STATE:

	ListIterator *iterator = new ListIterator();
	//NuiImageStreamReleaseFrame( ColorImageStream, ImageFrame );
	//ImageFrame->pFrameTexture->UnlockRect(0);

	GameOver *game_over = new GameOver(GAME_OVER);
	StageClear *stage_clear = new StageClear(STAGE_CLEAR);
	int title = LoadGraph(TITLE);	//	タイトル画面読込
	int titleCarsor = LoadGraph(TITLE_CARSOR);	//	カーソルアイコンの読込
	int gameMode = 0;	//	ゲームの難易度選択、ゲームを終了するかどうか。0:ノーマル,1:ハード,2:終了
	bool buttonDown = false;	//	ボタン押しっぱなしのときはカーソルを動かさない



	//	当たり判定がある手足を生成
	//new RightHand();
	new AttackParts(NUI_SKELETON_POSITION_HAND_RIGHT);
	new AttackParts(NUI_SKELETON_POSITION_HAND_LEFT);
	new AttackParts(NUI_SKELETON_POSITION_FOOT_RIGHT);
	new AttackParts(NUI_SKELETON_POSITION_FOOT_LEFT);

	//	背景とかを生成
	new Object(MODEL_DIR "right_wall.x", VGet(RIGHT_WALL_X, 3.0f, 5.0f+6.0f), VGet(0,0,0), 3.0f);
	new Object(MODEL_DIR "left_wall.x",  VGet(LEFT_WALL_X, 4.0f, 7.0f+5.0f), VGet(0,0,0), 3.0f);
	new Object(MODEL_DIR "concrete.x", VGet(0+2.2f, 0.01f, 0+14.0f), VGet(0,0,0), 3.0f);
	skyHandle = (new Object(MODEL_DIR "sky.x", VGet(0, 0, 0), VGet(0,0,0), 1.0f))->grHandle; //  20110904 masuda

	new Object(MODEL_DIR "drum1_r.x", VGet(-1.3f, 1.2f, 10.0f), VGet(0,0,0), 1.0f);
	new Object(MODEL_DIR "drum0_g.x", VGet(0, 1.2f, 9.5f), VGet(0,0,0), 1.0f);
	new Object(MODEL_DIR "drum1_y.x", VGet( 1.3f, 1.2f, 10.0f), VGet(0,0,0), 1.0f);

	new Object(MODEL_DIR "glassland.x", VGet(0, 0, 0)) ;
	//MV1DrawModel(floorId) ;	//	フロアを描画
	
	// 20110812 masuda
	SetCameraNearFar(1.0f,100.0f); // カメラからみて、１メートルから１００メートル前方が表示領域
	SetCameraPositionAndTargetAndUpVec( 
	// カメラの振動　20110823 masuda
	VAdd(CameraPosition,CameraShake), // カメラの位置　初期位置は、高さ１メートル、原点より５メートル後ろ
	VGet( XORG, YORG, ZORG ),	//　原点をみている(ここにkinectプレーやがいるはず)
	VGet( 0.0f, 1.0f, 0.0f ) ) ; // カメラの傾き
	
	SetCameraScreenCenter(WINDOWSIZE_X*0.5f , WINDOWSIZE_Y*0.9f); // 原点(0,0,0)の画面上の位置を下げる
	

	





	//	サウンド関係読み込み
	mainBGM = LoadSoundMem("sound\\BGM.mp3") ;	//	１面のBGM
	PlaySoundMem(mainBGM, DX_PLAYTYPE_LOOP) ;
	
	RevTime = GetNowCount();	//	前時間を初期化しておく

	VECTOR skyLongtitude_d =  VGet(0.0f,-0.0001f,0.0f);

	//	メインループ
	while(!ProcessMessage() && !ScreenFlip() &&!ClearDrawScreen()
		&& !CheckHitKey(KEY_INPUT_ESCAPE) )
	{
	
		//--------------------------------
		ModelRotationAdd(skyHandle, skyLongtitude_d); // 20110904 masuda


		
		if(CheckHitKey(KEY_INPUT_T)){
			iterator->sysTime->remainingTime = 0;
		}
		



		//	通常通るルート

		if(GameClearFlag == false && GameOverFlag == false && SkeletonTracking == true && iterator->sysTime->remainingTime > 0){
			iterator->Move();
			iterator->Draw();
		}
		//	タイムアップ時に通るルート
		else if(GameClearFlag == false && GameOverFlag == false && SkeletonTracking == true && iterator->sysTime->remainingTime <= 0){
			iterator->Draw();
			iterator->Timeout();
		}
		//	ポーズ時通るルート
		else if(GameClearFlag == false && GameOverFlag == false && SkeletonTracking == false){
			iterator->Pause();
		}
		else if(GameOverFlag == true){
			iterator->Draw();
			game_over->Move();
			game_over->Draw();
		}
		else if(GameClearFlag == true){
			iterator->Draw();
			stage_clear->Move();
			stage_clear->Draw();
		}

		else if(StageClearFlag == true){
			iterator->DeleteEnemyBullet();
			iterator->Move();
			iterator->Draw();
			iterator->MoveStage() ;
		}

		if(CheckHitKey(KEY_INPUT_F1)){
			delete iterator;
			DeleteSoundMem(mainBGM);
			GameClearFlag = false;
			StageClearFlag = false ;
			GameOverFlag = false;
			goto TITLE_STATE;
		}
	}

	_CrtDumpMemoryLeaks();
	WaitKey();
	NuiShutdown();	//	kinectの後始末
	DxLib_End();	//	DXライブラリの後始末

	return 0;
}



/*
void LoadEnemyData(int stage){
switch(stage){
case 1:
//	敵を待ち状態リストへ登録
//	左から右へ移動する敵
new E_mStraight_bStraight( 60, 0, 30, 2, 1, 2, 8);
new E_mStraight_bStraight( 70, 0, 30, 2, 1, 2, 8);
new E_mStraight_bStraight( 80, 0, 30, 2, 1, 2, 8);
new E_mStraight_bStraight( 90, 0, 30, 2, 1, 2, 8);
//	左から右へ移動する敵
new E_mStraight_bStraight( 200, 0, 70, 2, 1, 2, 8);
new E_mStraight_bStraight( 220, 0, 70, 2, 1, 2, 8);
new E_mStraight_bStraight( 240, 0, 70, 2, 1, 2, 8);
new E_mStraight_bStraight( 260, 0, 70, 2, 1, 2, 8);
//	右から左へ移動する敵
new E_mStraight_bStraight( 360, 400, 70, 2, 180, 2, 8); 
new E_mStraight_bStraight( 380, 400, 70, 2, 180, 2, 8);
new E_mStraight_bStraight( 400, 400, 70, 2, 180, 2, 8); 
new E_mStraight_bStraight( 420, 400, 70, 2, 180, 2, 8); 
new E_mStraight_bStraight( 440, 400, 70, 2, 180, 2, 8); 
//	右から左へ移動する敵
new E_mStraight_bStraight( 540, 400, 70, 2, 180, 2, 8);
new E_mStraight_bStraight( 560, 400, 70, 2, 180, 2, 8);
new E_mStraight_bStraight( 580, 400, 70, 2, 180, 2, 8);
new E_mStraight_bStraight( 600, 400, 70, 2, 180, 2, 8);
//	山みたいな形で下へ降りてくる敵
new E_mStraight_bStraight( 760, 30, 0, 2, 90, 2, 8);
new E_mStraight_bStraight( 760, 350, 0, 2, 90, 2, 8); 
new E_mStraight_bStraight( 780, 50, 0, 2, 90, 2, 8); 
new E_mStraight_bStraight( 780, 320, 0, 2, 90, 2, 8); 
new E_mStraight_bStraight( 800, 80, 0, 2, 90, 2, 8); 
new E_mStraight_bStraight( 800, 290, 0, 2, 90, 2, 8); 
new E_mStraight_bStraight( 820, 110, 0, 2, 90, 2, 8); 
new E_mStraight_bStraight( 820, 260, 0, 2, 90, 2, 8); 
new E_mStraight_bStraight( 840, 140, 0, 2, 90, 2, 8);
new E_mStraight_bStraight( 840, 230, 0, 2, 90, 2, 8);
new E_mStraight_bStraight( 860, 170, 0, 2, 90, 2, 8); 
new E_mStraight_bStraight( 860, 200, 0, 2, 90, 2, 8);
//	固定nway弾を出してくる敵
new E_mStraight_bNway_f(1000, 50, 0, 2, 20, 90, 4, 1.5, 70);
new E_mStraight_bNway_f(1000, 100, 0, 2, 20, 90, 4, 1.5, 70);
new E_mStraight_bNway_f(1000, 150, 0, 2, 20, 90, 4, 1.5, 70);
new E_mStraight_bNway_f(1000, 200, 0, 2, 20, 90, 4, 1.5, 70);
new E_mStraight_bNway_f(1000, 250, 0, 2, 20, 90, 4, 1.5, 70);
new E_mStraight_bStraight( 1000, 50, 0, 1, 90, 1, 5);
new E_mStraight_bStraight( 1000, 350, 0, 1, 90, 1.5, 8);
new E_mStraight_bStraight( 1020, 50, 0, 1, 90, 1, 5);
new E_mStraight_bStraight( 1020, 350, 0, 1, 90, 1, 8);
new E_mStraight_bStraight( 1040, 50, 0, 1, 90, 1, 8);
new E_mStraight_bStraight( 1040, 350, 0, 1, 90, 1, 8);
new E_mStraight_bStraight( 1060, 50, 0, 1, 90, 1, 8);
new E_mStraight_bStraight( 1060, 350, 0, 1, 90, 1, 8);
new E_mStraight_bStraight( 1080, 50, 0, 1, 90, 1, 8);
new E_mStraight_bStraight( 1080, 350, 0, 1, 90, 1, 8);
//	中ボス的な体力の回転弾幕の敵
new E_mStraight_bSpin	 ( 1600, 200, 0, 2, 50, 90, 2, 8); 
new E_mStraight_bSpin	 ( 2000, 300, 0, 2, 50, 90, 2, 8);
new E_mStraight_bSpin	 ( 2000, 100, 0, 2, 50, 90, 2, 8);
//	１面ボス
new E_Boss_Fire (2700,200);
break;
case 2 :
new E_mStraight_bNway(100, 0, 100, 2, 350, 3, 2, 30, 10);
new E_mStraight_bNway(130, 0, 100, 2, 350, 3, 2, 30, 10);
new E_mStraight_bNway(160, 0, 100, 2, 350, 3, 2, 30, 10);
new E_mStraight_bNway(190, 0, 100, 2, 350, 3, 2, 30, 10);
new E_mStraight_bNway(400, 0, 100, 2, 350, 10, 2, 5, 10);
new E_mStraight_bNway(420, 0, 100, 2, 350, 10, 2, 5, 10);
new E_mStraight_bNway(440, 0, 100, 2, 350, 10, 2, 5, 10);
new E_mStraight_bNway(460, 0, 100, 2, 350, 10, 2, 5, 10);
new E_mStraight_bNway(480, 0, 100, 2, 350, 10, 2, 5, 10);
break ;
case 11:
//	敵を待ち状態リストへ登録
//	左から右へ移動する敵
new E_mStraight_bStraight( 60, 0, 30, 2, 1, 6, 40);
new E_mStraight_bStraight( 70, 0, 30, 2, 1, 6, 40);
new E_mStraight_bStraight( 80, 0, 30, 2, 1, 6, 40);
new E_mStraight_bStraight( 90, 0, 30, 2, 1, 6, 40);
//	左から右へ移動する敵
new E_mStraight_bStraight( 200, 0, 70, 2, 1, 6, 40);
new E_mStraight_bStraight( 220, 0, 70, 2, 1, 6, 40);
new E_mStraight_bStraight( 240, 0, 70, 2, 1, 6, 40);
new E_mStraight_bStraight( 260, 0, 70, 2, 1, 6, 40);
//	右から左へ移動する敵
new E_mStraight_bStraight( 360, 400, 70, 2, 180, 6, 40); 
new E_mStraight_bStraight( 380, 400, 70, 2, 180, 6, 40);
new E_mStraight_bStraight( 400, 400, 70, 2, 180, 6, 40); 
new E_mStraight_bStraight( 420, 400, 70, 2, 180, 6, 40); 
new E_mStraight_bStraight( 440, 400, 70, 2, 180, 6, 40); 
//	右から左へ移動する敵
new E_mStraight_bStraight( 540, 400, 70, 2, 180, 6, 40);
new E_mStraight_bStraight( 560, 400, 70, 2, 180, 6, 40);
new E_mStraight_bStraight( 580, 400, 70, 2, 180, 6, 40);
new E_mStraight_bStraight( 600, 400, 70, 2, 180, 6, 40);
//	山みたいな形で下へ降りてくる敵
new E_mStraight_bStraight( 760, 30, 0, 2, 90, 6, 40);
new E_mStraight_bStraight( 760, 350, 0, 2, 90, 6, 40); 
new E_mStraight_bStraight( 780, 50, 0, 2, 90, 6, 40); 
new E_mStraight_bStraight( 780, 320, 0, 2, 90, 6, 40); 
new E_mStraight_bStraight( 800, 80, 0, 2, 90, 6, 40); 
new E_mStraight_bStraight( 800, 290, 0, 2, 90, 6, 40); 
new E_mStraight_bStraight( 820, 110, 0, 2, 90, 6, 40); 
new E_mStraight_bStraight( 820, 260, 0, 2, 90, 6, 40); 
new E_mStraight_bStraight( 840, 140, 0, 2, 90, 6, 40);
new E_mStraight_bStraight( 840, 230, 0, 2, 90, 6, 40);
new E_mStraight_bStraight( 860, 170, 0, 2, 90, 6, 40); 
new E_mStraight_bStraight( 860, 200, 0, 2, 90, 6, 40);
//	固定nway弾を出してくる敵
new E_mStraight_bNway_f(1000, 50, 0, 2, 20, 90, 7, 3.5, 70);
new E_mStraight_bNway_f(1000, 100, 0, 2, 20, 90, 7, 3.5, 70);
new E_mStraight_bNway_f(1000, 150, 0, 2, 20, 90, 7, 3.5, 70);
new E_mStraight_bNway_f(1000, 200, 0, 2, 20, 90, 7, 3.5, 70);
new E_mStraight_bNway_f(1000, 250, 0, 2, 20, 90, 7, 3.5, 70);
new E_mStraight_bStraight( 1000, 50, 0, 1, 90, 2, 15);
new E_mStraight_bStraight( 1000, 350, 0, 1, 90, 1.5, 15);
new E_mStraight_bStraight( 1020, 50, 0, 1, 90, 3, 15);
new E_mStraight_bStraight( 1020, 350, 0, 1, 90, 3, 15);
new E_mStraight_bStraight( 1040, 50, 0, 1, 90, 3, 15);
new E_mStraight_bStraight( 1040, 350, 0, 1, 90, 3, 15);
new E_mStraight_bStraight( 1060, 50, 0, 1, 90, 3, 15);
new E_mStraight_bStraight( 1060, 350, 0, 1, 90, 3, 15);
new E_mStraight_bStraight( 1080, 50, 0, 1, 90, 3, 15);
new E_mStraight_bStraight( 1080, 350, 0, 1, 90, 3, 15);
//	中ボス的な体力の回転弾幕の敵
new E_mStraight_bSpin	 ( 1600, 200, 0, 2, 50, 90, 4, 4); 
new E_mStraight_bSpin	 ( 2000, 300, 0, 2, 50, 90, 4, 4);
new E_mStraight_bSpin	 ( 2000, 100, 0, 2, 50, 90, 4, 4);
//	１面ボス
new E_Boss_Fire (2700, 100, true);
new E_Boss_Fire (2700, 300, true);
break;
default:	//	例外処理。ゲームを強制終了させる。
return ;
}

}

*/


