// ���ƂŁAmain.h������āA��`�֌W�͂��̃t�@�C���ɓ���A�����R���p�C������
#define MAIN

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <Windows.h> // ���ꂪ�Ȃ��ƌ��kinect�w�b�_���C���N���[�h�ł��Ȃ����璍��
#include <math.h>
#include <fstream>
#include <shlwapi.h>
#include <tchar.h>

//	���ꂪ�Ȃ���PathIsDirectory���g���Ȃ�
#pragma comment(lib, "shlwapi.lib")

#pragma comment(lib, "kinect10.lib")

#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>
#include <NuiSkeleton.h>

//	Kinect�֌W�̃w�b�_�t�@�C��
//#include <MSR_NuiApi.h>
//#include <MSR_NuiImageCamera.h>
//#include <MSR_NuiProps.h>
//#include <MSR_NuiSkeleton.h>
//#include <NuiImageBuffer.h>



//	���[�N���o
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "def.h"
#include "function.h"
#include "objClass.h"	//	�V���[�e�B���O�Q�[���̃f�[�^�܂Ƃ�
#include "main.h"

//	3D���f���̍��W���擾�A�ύX
//----------------------------
//	���f���̍��W���ړ�
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
//	�X�P���g���f�[�^�擾�֐�
//---------------------------------------
//	�X�P���g���f�[�^����X���W��Y���W�̃f�[�^��ϊ����Ă�����o��
void GetSkeletonXY(NUI_SKELETON_FRAME frame, float *X, float *Y, int skeletonPos)
{
	for(int i = 0; i < 6; i++){
		if(frame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKING_STATE::NUI_SKELETON_TRACKED)
		{
			//	kinect����E�����f�[�^�̓��[�g���P�ʂ�����A������֐��ō��W�ɂ���
			Vector4 v4 = frame.SkeletonData[i].SkeletonPositions[skeletonPos] ;
			//NuiTransformSkeletonToDepthImageF(v4, X, Y);
			
		}
	}
}

//	�X�P���g���f�[�^����Vector4�����̂܂ܕԂ�
void GetSkeletonV4(NUI_SKELETON_FRAME frame, Vector4 *XYZ, int skeletonPos)
{
	for(int i = 0; i < 6; i++){
		if(frame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKING_STATE::NUI_SKELETON_TRACKED)
		{
			//	Vector4�̒l��kinect�Z���T�[����R�s�[����
			*XYZ = frame.SkeletonData[i].SkeletonPositions[skeletonPos] ;
		}
	}
}

//	�X�P���g���f�[�^����VECTOR�̌^�ɒ����ĕԂ�
void GetSkeletonV(NUI_SKELETON_FRAME frame, VECTOR *XYZ, int skeletonPos)
{
	SkeletonTracking = false ;
	for(int i = 0; i < 6; i++){
		if(frame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKING_STATE::NUI_SKELETON_TRACKED)
		{
			//	Vector4�̒l��kinect�Z���T�[����R�s�[����
			XYZ->x = frame.SkeletonData[i].SkeletonPositions[skeletonPos].x;
			XYZ->y = frame.SkeletonData[i].SkeletonPositions[skeletonPos].y ;
			XYZ->z = frame.SkeletonData[i].SkeletonPositions[skeletonPos].z ;
			SkeletonTracking = true ;	//	�v���C���[�F���A�t���O�𗧂ĂĂ���
		}
	}
}

//-------------------------------------------------------------------
//	�O���t�B�b�N�n���h���𐶐�����B
void CreateGraphicHandle(int *grHandle, const NUI_IMAGE_FRAME *ImageFrame , BASEIMAGE *BaseImage){
	// �摜�f�[�^�̎擾
	NUI_LOCKED_RECT rect;
	ImageFrame->pFrameTexture->LockRect( 0, &rect, 0, 0 );   // IDirect3DTexture9 ��Lock���\�b�h�Ɠ����H

	// Dxlib�Ŏg�p����摜�f�[�^�̍\���̐ݒ�
	BaseImage->GraphData = rect.pBits;
	BaseImage->Pitch = rect.Pitch;  // �s�b�`�͉��P���C���������Byte��

	ImageFrame->pFrameTexture->UnlockRect(0);
	// �ŏ��̏ꍇ�̓O���t�B�b�N�n���h���̍쐬�Ɖf���̓]������x�ɍs��
	*grHandle = CreateGraphFromBaseImage( BaseImage ) ;
}



//	kinect�J�����̃f�[�^���O���t�B�b�N�n���h���ɓ]������
void LoadImageForKinectCameraRGB(int *grHandle, const NUI_IMAGE_FRAME *ImageFrame , BASEIMAGE *BaseImage){
	return;

	// �摜�f�[�^�̎擾
	NUI_LOCKED_RECT rect;
	ImageFrame->pFrameTexture->LockRect( 0, &rect, 0, 0 );   // IDirect3DTexture9 ��Lock���\�b�h�Ɠ����H

	// Dxlib�Ŏg�p����摜�f�[�^�̍\���̐ݒ�
	BaseImage->GraphData = rect.pBits;
	BaseImage->Pitch = rect.Pitch;  // �s�b�`�͉��P���C���������Byte��



	// �Q��ڈȍ~�̓O���t�B�b�N�n���h���։f����]��
	ReCreateGraphFromBaseImage( BaseImage, *grHandle ) ;
}

void CreateGraphicHandle(int *GrHandle, HANDLE StreamHandle, BASEIMAGE *BaseImage){
	HRESULT hr;
	// �J�����f�[�^�̎擾
	const NUI_IMAGE_FRAME *ImageFrame;
	hr = NuiImageStreamGetNextFrame( StreamHandle, 0, &ImageFrame );
	
	// �摜�f�[�^�̎擾
	NUI_LOCKED_RECT rect;
	ImageFrame->pFrameTexture->LockRect( 0, &rect, 0, 0 );   // IDirect3DTexture9 ��Lock���\�b�h�Ɠ����H

	// Dxlib�Ŏg�p����摜�f�[�^�̍\���̐ݒ�
	BaseImage->GraphData = rect.pBits;
	BaseImage->Pitch = rect.Pitch;  // �s�b�`�͉��P���C���������Byte��

	// �J�����f�[�^�̉��
	hr = NuiImageStreamReleaseFrame( StreamHandle, ImageFrame );
	//  �ꎞ�I�ɃR�����g�A�E�g
	//ImageFrame->pFrameTexture->UnlockRect(0);

	// �ŏ��̏ꍇ�̓O���t�B�b�N�n���h���̍쐬�Ɖf���̓]������x�ɍs��
	*GrHandle = CreateGraphFromBaseImage( BaseImage ) ;
}



//	kinect�J�����̃f�[�^���O���t�B�b�N�n���h���ɓ]������
void LoadImageForKinectCameraRGB(int *grHandle, HANDLE StreamHandle , BASEIMAGE *BaseImage){
	// �J�����f�[�^�̎擾
	const NUI_IMAGE_FRAME *ImageFrame;
	
	NuiImageStreamGetNextFrame( StreamHandle, 0, &ImageFrame );

	// �摜�f�[�^�̎擾
	NUI_LOCKED_RECT rect;
	ImageFrame->pFrameTexture->LockRect( 0, &rect, 0, 0 );   // IDirect3DTexture9 ��Lock���\�b�h�Ɠ����H

	// Dxlib�Ŏg�p����摜�f�[�^�̍\���̐ݒ�
	BaseImage->GraphData = rect.pBits;
	BaseImage->Pitch = rect.Pitch;  // �s�b�`�͉��P���C���������Byte��

	
	// �J�����f�[�^�̉��
	NuiImageStreamReleaseFrame( StreamHandle, ImageFrame );
	//  �ꎞ�I�ɃR�����g�A�E�g
	//ImageFrame->pFrameTexture->UnlockRect(0);
	
	// �Q��ڈȍ~�̓O���t�B�b�N�n���h���։f����]��
	ReCreateGraphFromBaseImage( BaseImage, *grHandle ) ;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------







//---------------------------------------------------
//	�A���t�@�l���g���ăr���{�[�h��`�悷��
void DrawAlphaBill(VECTOR pos, int Alpha, int grHandle, float size = 1.0f){
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, Alpha) ;
	DrawBillboard3D( pos, 0.5f, 0.5f, size, 0, grHandle, TRUE );
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 ) ;
}
//---------------------------------------------------
//	VECTOR�̒l��S�ĂQ��ɂ���
int VSquare(VECTOR square){
	return ( pow(square.x,2) + pow(square.y,2) + pow(square.z,2) ) ;
}

//------------------------------------------------------
//	�X�R�A���v�Z����
int ScoreCalc(VECTOR velocity){
	if(velocity.y < 0)	velocity.y = 0;	
	return (abs(velocity.z * 100 * (velocity.y / 10 + 1))) ;	//	�X�R�A���v�Z
}


//----------------------------------
//	�Ƃ肠�����I�u�W�F�N�g��\������N���X
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
//	�ΉԃG�t�F�N�g��\������N���X
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
	if(frame == 10)	//	10�t���[���܂ŏI��������폜����
		alive = false;	
}
void SparkEffect::Draw(){
	DrawBillboard3D( pos, 0.5f, 0.5f, 2.0f, 0, grHandle[frame], TRUE );
}

//-----------------------------------------------
//	�Z���t�G�t�F�N�g��\������N���X�i�r���{�[�h�ŕ\���j
//-----------------------------------------------
class SerifEffect : EffectBase{
public:
	static int grHandle [5] ;
	int nowAlpha ;
	int serifNum ;	//	�g�p����Z���t�B�����_���ɑI��
	VECTOR pos ;	//	�\������ʒu
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
//	�X�R�A�G�t�F�N�g�i�h�����ʂ��������ۂɐ�����\���j
//-----------------------------------------------
class ScoreNumEffect : EffectBase{
public:
	int nowAlpha ;
	int scoreNum ;	//	�\�����鐔��
	VECTOR pos ;	//	�\������ʒu
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
//	�J������h�炷�N���X
//----------------------------------------
class CameraShakeEffect : EffectBase{
public:
	VECTOR shake ;
	int limitFrame ;	//	������܂ł̎���
	int nowFrame ;		//	���݂̃t���[����
	CameraShakeEffect(VECTOR shake, int repeatNum);
	virtual void Move() ;
};
CameraShakeEffect::CameraShakeEffect(VECTOR shake, int repeatNum){	//	�h�炷VECTOR�A�h�炷��
	nowFrame = 0;
	this->shake = shake ;
	limitFrame = repeatNum * 2 ;
}
void CameraShakeEffect::Move(){
	VECTOR cameraPos = GetCameraPosition()  ;
	VECTOR cameraTarget = GetCameraTarget() ;
	int i = pow(-1.0f ,(nowFrame % 2)) ;	//	������1�A���-1
	cameraTarget = VAdd( cameraTarget, VScale(shake, i)) ;
	SetCameraPositionAndTargetAndUpVec(
		cameraPos, cameraTarget, VGet(0, 1.0f ,0)) ;

	nowFrame++ ;
	if(limitFrame <= nowFrame)
		alive = false ;
}

//----------------------------------------
//	�v���C���[�̎�
//---------------------------------------
//	�E��
class RightHand : PB_Base{
public:
	int grHandle ; //	�RD���f���̃n���h��
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
	if(kinectUpdateFlag)	//	kinect���X�V���ꂽ�Ƃ������O�f�[�^���X�V����B
	{
		VECTOR centerV;	//	�̂̒��S���W
		revV = nowV ;	//	�O�f�[�^��ۑ�

		GetSkeletonV(playerSkeleton, &nowV,  skeletonPos);	//	�{�[���̃x�N�g�������[�g���P�ʂŎ擾
		GetSkeletonV(playerSkeleton, &centerV,  NUI_SKELETON_POSITION_HIP_CENTER);	//	�̂̒��S���W�̎擾
		//nowV.z = centerV.z - nowV.z ;
		//	2011/08/16���q
		//nowV.z += PLAYER_Z_POSITION ;	//	�v���C���[�̗����ʒu��O�Ɉړ�������
		//nowV.y += PLAYER_Y_POSITION ;	//	�v���C���[�̗����ʒu��O�Ɉړ�������

		nowV.z = VScale(nowV, -1.0f).z + PLAYER_Z_POSITION ;
		nowV.y += PLAYER_Y_POSITION ;
		//	X�����ɑ傫��������悤�ɂ���
		VECTOR relativeSkeleton ;	//	������̑��΍��W
		relativeSkeleton = VSub(nowV, centerV) ;	//	������̑��΍��W�擾
		centerV.x *= PLAYER_X_MOVE_SCALE;	//	�v���C���[��X�����Ɉړ�����Ƃ��̔{��
		nowV.x = centerV.x + relativeSkeleton.x ;	//	X���W�����傫���ړ�������	
	}
}


	

void RightHand::Draw()
{
	DrawString(700,30,"�E��",GetColor(255,255,255));
	DrawFormatString(700,50,GetColor(255,255,255),"���WX = %00f", posX);
	DrawFormatString(700,70,GetColor(255,255,255),"���WY = %00f", posY);
	DrawFormatString(700, 90, GetColor(255,255,255), "���[���h���WX = %00f", nowV.x);
	DrawFormatString(700, 110, GetColor(255,255,255), "���[���h���WY = %00f", nowV.y);
	DrawFormatString(700, 130, GetColor(255,255,255), "���[���h���WZ = %00f", nowV.z);
	DrawFormatString(700, 150, GetColor(255,255,255), "���΍��WZ = %00f", centerV.z - nowV.z);
	//	�E��̈ʒu���~�ŕ`��
	/*
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawCircle(posX, posY, 20, GetColor(255,0,0));
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	*/
	//	��̈ʒu�ɋ��̂�`��
	DrawSphere3D(nowV, HAND_SIZE/3*2, POLYGON_NUM, GetColor(255,0,0), GetColor(255,0,0), TRUE);
}
///////////////////////////////////////////
/////////////����//////////////////////////
class LeftHand : PB_Base{
public:
	int grHandle ; //	�RD���f���̃n���h��
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
	revV = nowV ;	//	�O�f�[�^��ۑ�
	GetSkeletonXY(playerSkeleton, &posX, &posY, skeletonPos);	//	XY�̃{�[���ʒu�擾
	posX *= 8; // ���W�ϊ�
	posY *= 8; // ���W�ϊ�
	GetSkeletonV(playerSkeleton, &nowV,  skeletonPos);	//	�{�[���̃x�N�g�������[�g���P�ʂŎ擾
	GetSkeletonV(playerSkeleton, &centerV,  NUI_SKELETON_POSITION_HIP_CENTER);	//	�̂̒��S���W�̎擾
}
void LeftHand::Draw()
{
	DrawString(0,30,"����",GetColor(255,255,255));

	DrawFormatString(0,50,GetColor(255,255,255),"���WX = %00f", posX);

	DrawFormatString(0,70,GetColor(255,255,255),"���WY = %00f", posY);
	DrawFormatString(0, 90, GetColor(255,255,255), "����X = %00f", nowV.x);
	DrawFormatString(0, 110, GetColor(255,255,255), "����Y = %00f", nowV.y);
	DrawFormatString(0, 130, GetColor(255,255,255), "����Z = %00f", nowV.z);
	DrawFormatString(0, 150, GetColor(255,255,255), "���΍��WZ = %00f", centerV.z - nowV.z);
	//	����̈ʒu���~�ŕ`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawCircle(posX, posY, 20, GetColor(0,0,255));
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
//-----------------------------------------------
//	�A�^�b�N�p�[�c�N���X�i�����蔻�������̂̃p�[�c�j
//-----------------------------------------------
class AttackParts : PB_Base{
public:
	int grHandle ; //	�RD���f���̃n���h��
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
	if(kinectUpdateFlag)	//	kinect���X�V���ꂽ�Ƃ������O�f�[�^���X�V����B
	{
		VECTOR centerV;	//	�̂̒��S���W
		revV = nowV ;	//	�O�f�[�^��ۑ�

		GetSkeletonV(playerSkeleton, &nowV,  skeletonPos);	//	�{�[���̃x�N�g�������[�g���P�ʂŎ擾
		GetSkeletonV(playerSkeleton, &centerV,  NUI_SKELETON_POSITION_HIP_CENTER);	//	�̂̒��S���W�̎擾
		//nowV.z = centerV.z - nowV.z ;
		//	2011/08/16���q
		//nowV.z += PLAYER_Z_POSITION ;	//	�v���C���[�̗����ʒu��O�Ɉړ�������
		//nowV.y += PLAYER_Y_POSITION ;	//	�v���C���[�̗����ʒu��O�Ɉړ�������

		nowV.z = VScale(nowV, -1.0f).z + PLAYER_Z_POSITION ;
		centerV.z = VScale(centerV, -1.0f).z + PLAYER_Z_POSITION ;
		nowV.y += PLAYER_Y_POSITION ;
		//	X�����ɑ傫��������悤�ɂ���
		VECTOR relativeSkeleton ;	//	������̑��΍��W
		relativeSkeleton = VSub(nowV, centerV) ;	//	������̑��΍��W�擾
		centerV.x *= PLAYER_X_MOVE_SCALE;	//	�v���C���[��X�����Ɉړ�����Ƃ��̔{��
		centerV.z *= PLAYER_Z_MOVE_SCALE;	//	�v���C���[��Z�����Ɉړ�����Ƃ��̔{��
		centerV.y *= PLAYER_Y_MOVE_SCALE;	//	�v���C���[��Y�����Ɉړ�����Ƃ��̔{��
		nowV.x = centerV.x + relativeSkeleton.x ;	//	X���W�����傫���ړ�������	
		nowV.z = centerV.z + relativeSkeleton.z ;	//	Z���W�����傫���ړ�������
		nowV.y = centerV.y + relativeSkeleton.y ;	//	Z���W�����傫���ړ�������
	}
}


	

void AttackParts::Draw()
{
	/*
	DrawString(700,30,"�E��",GetColor(255,255,255));
	DrawFormatString(700,50,GetColor(255,255,255),"���WX = %00f", posX);
	DrawFormatString(700,70,GetColor(255,255,255),"���WY = %00f", posY);
	DrawFormatString(700, 90, GetColor(255,255,255), "���[���h���WX = %00f", nowV.x);
	DrawFormatString(700, 110, GetColor(255,255,255), "���[���h���WY = %00f", nowV.y);
	DrawFormatString(700, 130, GetColor(255,255,255), "���[���h���WZ = %00f", nowV.z);
	DrawFormatString(700, 150, GetColor(255,255,255), "���΍��WZ = %00f", centerV.z - nowV.z);
	//	�E��̈ʒu���~�ŕ`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawCircle(posX, posY, 20, GetColor(255,0,0));
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	*/
	//	��̈ʒu�ɋ��̂�`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, PLAYER_ALPHA + 80) ;
	DrawSphere3D(nowV, HAND_SIZE/3*2, POLYGON_NUM, GetColor(255,0,0), GetColor(255,0,0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, NULL) ;
}


//--------------------------------
//	�v���C���[�N���X
//--------------------------------
class KinectPlayer : public MoverBase {	//	�啝�ǉ��B������`�悷��
public:
	VECTOR	skeletonPos[NUI_SKELETON_POSITION_COUNT];	//	�S�ẴX�P���g���f�[�^(VECTOR�`��)
	static KinectPlayer *p ;	//	���N���X�Ńv���C���[���Q�Ƃ���Ƃ��Ɏg�p
	int deathHandle;	//	�ė���
	int bomSound;		//	�{���̉�
	int noHitTime ;		//	���G����
	int flashTime ;		//	�_�Ŏ��ԁB���@����e�������ƂɎg�p�B
	bool alive ;
	float hitSize ;		//	�����蔻��̑傫��
	int stock ;			//	�c�@��
	int bom ;			//	�c��{����
	int bomEffect;		//	�{���̖��G���Ԃ̖ڈ������������w
	KinectPlayer(char graphicFileName[]);
	KinectPlayer(){};
	void virtual Draw();
	void virtual Move();
	void Shot();
};
//	�ÓI�����o�ϐ��̏�����
KinectPlayer *KinectPlayer::p = NULL ;
//	�v���C���[�N���X�̃R���X�g���N�^
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

//	�v���C���[�̕`��
void KinectPlayer::Draw(){
	const int PColor = GetColor(0,0,255);	//	�`�悷��F
	int i;	//	for���p
	
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, PLAYER_ALPHA ) ;	//	�A���t�@�l��ݒ�
	for(i = 0; i < 2; i++)
		DrawCapsule3D(skeletonPos[i], skeletonPos[i+1], HAND_SIZE/2, 20, PColor, PColor, TRUE) ;
	DrawCapsule3D(skeletonPos[2], skeletonPos[4], HAND_SIZE/2, 20, PColor, PColor, TRUE) ;	//	������`��
	for(i = 4; i < 7; i++)
		DrawCapsule3D(skeletonPos[i], skeletonPos[i+1], HAND_SIZE/2, 20, PColor, PColor, TRUE) ;//	����������`��
	DrawCapsule3D(skeletonPos[2], skeletonPos[8], HAND_SIZE/2, 20, PColor, PColor, TRUE) ;	//	�E����`��
	for(i = 8; i < 11; i++)
		DrawCapsule3D(skeletonPos[i], skeletonPos[i+1], HAND_SIZE/2, 20, PColor, PColor, TRUE) ;//	�E��������`��
	DrawCapsule3D(skeletonPos[0], skeletonPos[12], HAND_SIZE/2, 20, PColor, PColor, TRUE) ;	//	���K��`��
	for(i = 12; i < 15; i++)
		DrawCapsule3D(skeletonPos[i], skeletonPos[i+1], HAND_SIZE/2, 20, PColor, PColor, TRUE) ;//	���K������`��
	DrawCapsule3D(skeletonPos[0], skeletonPos[16], HAND_SIZE/2, 20, PColor, PColor, TRUE) ;	//	�E�K��`��
	for(i = 16; i < 19; i++)
		DrawCapsule3D(skeletonPos[i], skeletonPos[i+1], HAND_SIZE/2, 20, PColor, PColor, TRUE) ;	//	�E�K��`��	
	DrawSphere3D(skeletonPos[NUI_SKELETON_POSITION_HEAD], HAND_SIZE/3*2, 20, PColor, PColor, TRUE) ;	//	����`��
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, NULL ) ;	//	�A���t�@�l�����ɖ߂�
	return ;
}

void KinectPlayer::Shot(){

}

//	�v���C���[�̈ړ�
void KinectPlayer::Move(){
	if(kinectUpdateFlag){
		for(int i = 0; i < 20; i++){
			GetSkeletonV(playerSkeleton, &skeletonPos[i], i) ;
			skeletonPos[i].z = VScale(skeletonPos[i], -1.0f).z + PLAYER_Z_POSITION ;
			skeletonPos[i].y += PLAYER_Y_POSITION ;
		}
		skeletonPos[NUI_SKELETON_POSITION_HIP_CENTER].y -= PLAYER_Y_POSITION ;
		//	XYZ�����ɑ傫��������悤�ɂ���
		VECTOR relativeSkeleton[NUI_SKELETON_POSITION_COUNT];	//	������̑��΍��W
		for(int i = 0; i < 20; i++){
			relativeSkeleton[i] = VSub(skeletonPos[i], skeletonPos[NUI_SKELETON_POSITION_HIP_CENTER]) ;	//	������̑��΍��W�擾
		}
		skeletonPos[NUI_SKELETON_POSITION_HIP_CENTER].x *= PLAYER_X_MOVE_SCALE;	//	�v���C���[��X�����Ɉړ�����Ƃ��̔{��
		skeletonPos[NUI_SKELETON_POSITION_HIP_CENTER].z *= PLAYER_Z_MOVE_SCALE;	//	�v���C���[��Z�����Ɉړ�����Ƃ��̔{��
		skeletonPos[NUI_SKELETON_POSITION_HIP_CENTER].y *= PLAYER_Y_MOVE_SCALE;	//	�v���C���[��Z�����Ɉړ�����Ƃ��̔{��
		for(int i = 0; i < 20; i++){
			skeletonPos[i].x = skeletonPos[NUI_SKELETON_POSITION_HIP_CENTER].x + relativeSkeleton[i].x ;	//	X���W�����傫���ړ�������	
			skeletonPos[i].z = skeletonPos[NUI_SKELETON_POSITION_HIP_CENTER].z + relativeSkeleton[i].z ;	//	Z���W��傫��������
			skeletonPos[i].y = skeletonPos[NUI_SKELETON_POSITION_HIP_CENTER].y + relativeSkeleton[i].y ;	//	Y���W��傫��������
		}

	}


	Shot() ;	//	���@�e�𔭎˂���
	return  ;
}
//----------------------------------------
//	�h�����ʂ̃x�[�X�N���X�i�h�����ʂ͌p������j
//----------------------------------------



//-------------------------------
//	���ʂ̗΃h������
//------------------------------
class NormalDrum : EnemyBase{
public:
	static int loadOnlyHr[][4] ;	//	�RD���f���̃R�s�[�ǂݍ��ݗp
	static int collisionSE;		//	��ɐG�ꂽ����SE
	static int boundSE;			//	�o�E���h�����ۂ�SE
	static int shotSE ;			//	�I�u�W�F�N�g�𐶐��������̉��i���ˉ��j
	//int grHandle ;	//	�RD���f���̃n���h���B�`�擙�͂�������g�p�B
	int timer ;	//	�����^�C�}�[�B���ȏ�ɂȂ����炱�̃h�����ʂ��폜
	
	//VECTOR MV ;	//	�h�����ɒǉ��B���̂̈ړ����x
	VECTOR spin ;	//	��]���x
	//int boundNum ;	//	�o�E���h�̉�

	//float realTimer ;	//	�b���Z�̃^�C�}�[	�~���b�܂Ő��m�B
	//VECTOR initPos;	// �����ʒu		�h�����N���X�ɒǉ�
	//VECTOR initVelocity;	// �����x	�h�����N���X�ɒǉ�

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
//	�����̍\��	�o�����ԁi���݂͖����A��Ŏg���j,�@�����ʒu,�@�ړ����x
NormalDrum::NormalDrum(unsigned long long in_waitTime, VECTOR initPos, VECTOR initVelocity)
{
	waitTime = in_waitTime ;
	timer = 0;
	realTimer = 0;
	int drumColor = GetRand(3);		//	�����_���Ƀh�����̐F������
	grHandle[0] = MV1DuplicateModel(loadOnlyHr[drumColor][0]) ;	//	�\�ߓǂݍ���ł��郂�f�����R�s�[
	grHandle[1] = MV1DuplicateModel(loadOnlyHr[drumColor][1]) ;	//	���񂾃h������ǂݍ���
	grHandle[2] = MV1DuplicateModel(loadOnlyHr[drumColor][2]) ;	//	���񂾃h������ǂݍ���
	grHandle[3] = MV1DuplicateModel(loadOnlyHr[drumColor][3]) ;	//	���񂾃h������ǂݍ���

	MV1SetPosition( grHandle[0], initPos);

	MV = initVelocity ;

	NormalDrum::initPos = initPos;	// �����ʒu
	NormalDrum::initVelocity = initVelocity;	// �����x

	MV1SetScale(grHandle[0], VGet(0.8f, 0.8f, 0.8f)) ;
	//MV1SetScale(grHandle[0], VGet(0.08f, 0.08f, 0.08f)) ;
	MV1SetRotationXYZ(grHandle[0], VGet(0,0, DegToRad * 90));
	spin = VGet(0, 0, DegToRad * ((float)GetRand(8) + 2.0f));
	ModelRotationAdd(grHandle[0], VGet(DegToRad * (float)GetRand(50),0,0));
	MV1SetupCollInfo(grHandle[0]) ;	//	�Փˏ����\�z
	Activation();
}
void NormalDrum::PlayShotSE(){	//	Activation�֐����ŌĂ΂��
	PlaySoundMem( shotSE, DX_PLAYTYPE_BACK ) ;	//	���������Đ��i�����o�����j
}
void NormalDrum::PlayCollisionSE(){
	PlaySoundMem( collisionSE, DX_PLAYTYPE_BACK ) ;	//	�p���`�̉��𐶐�

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

	//ModelPosAdd(grHandle[0], MV) ;	//	�ړ����x�����݂̈ʒu�ɑ���

	VECTOR velocity =  VAdd(VScale(VGet(0.0f,-GRAVITY,0.0f),realTimer),initVelocity); // �����x�~���ԁ����̎����ł̑��x

	VECTOR acc =  VGet(0.0f,-0.5f*GRAVITY*realTimer*realTimer,0.0f);
	VECTOR pos = VAdd(VAdd(initPos,VScale(initVelocity,realTimer)),acc);
	
	//	�ȈՃo�E���h����
	if(pos.y < 0.5f && velocity.y < 0 ){
		if(velocity.y < -2 && boundNum == 0){
			new CameraShakeEffect(VGet(0,0.05f,0), 1) ;	//	�N���R���ǉ��B�J������h�炷�G�t�F�N�g
			PlaySoundMem( boundSE, DX_PLAYTYPE_BACK );	//	�傫���o�E���h����Ƃ��̂�SE���o��
		}
		if(abs(velocity.y) < 0.2f)  initVelocity.y = 0;
		else initVelocity.y = -velocity.y * RESTITUTE;	//	��ɒ��˂鍂���̌���
		
		initVelocity.x = velocity.x *  RESTITUTE ;	//	���x�̌���
		initVelocity.z = velocity.z *  RESTITUTE ;	
		initPos = MV1GetPosition(grHandle[0]) ;
		spin = VScale(spin, 0.5f) ;
		realTimer = 0;
		//	�ΉԂ𐶐�
		//new SparkEffect( VAdd(pos, VGet(0, 0.3f, -1.0f)) ) ;

		// �J�����̐U���@20110823 masuda
		// �����I�ɔ�������̂͐U���i���Ƃɖ߂�j������A���m�ȕ\���ł͂Ȃ����A�֋X��e�X�g
		/*
		float x0 =  ((GetRand(100)-50)/100.0f)*abs(velocity.x)*CAMERA_SHAKE_K;
		float y0 =  ((GetRand(100)-50)/100.0f)*abs(velocity.y)*CAMERA_SHAKE_K;
		float z0 =  ((GetRand(100)-50)/100.0f)*abs(velocity.z)*CAMERA_SHAKE_K;
		*/
		//CameraShake = VGet(x0,y0,z0);
		boundNum++;	//	�o�E���h�̉񐔂𐔂���
	}
	//	���E�̕ǂ̒��˕Ԃ菈��
	if((pos.x < (LEFT_WALL_X + 0.5f)) || (pos.x > (RIGHT_WALL_X - 0.5f))){
		initVelocity.x = -(initVelocity.x * 2.0f / 3.0f);
		initVelocity.y = initVelocity.y + acc.y;
		initPos = MV1GetPosition(grHandle[0]) ;
		realTimer = 0;
		new SparkEffect( VAdd(pos, VGet(0, 0.0f, -0.5f)) ) ;
	}

	MV1SetPosition(grHandle[0], pos) ;

	ModelRotationAdd(grHandle[0], spin);	//	���f������]������
	if(hitFlag == false)
		MV1RefreshCollInfo(grHandle[0]); //	�Փˏ����X�V
	else{	//	���񂾃h�����ʂ𕁒ʂ̃h�����ʂƓ���������
		MV1SetPosition(grHandle[state], MV1GetPosition( grHandle[0] ));
		MV1SetRotationXYZ( grHandle[state], MV1GetRotationXYZ( grHandle[0] ));
	}
	timer++;
	realTimer += (float)IntervalTime / 1000.0f;	//	�~���b��b�P�ʂɒ����ĉ��Z 
	if(timer == 360)	//	�U�b�o�߂�����h�����ʂ��폜����
		alive = false;
}
void NormalDrum::Draw()
{
	MV1DrawModel( grHandle[state] ) ;	//	���f����`��
	//	�e�̕`��
	VECTOR shadowPos = MV1GetPosition(grHandle[0]);
	shadowPos.y = 0.05f;
	MV1SetPosition( circleShadow, shadowPos );
	MV1DrawModel( circleShadow) ;
}
//-----------------------------------------------------------
//	�ԃh�����N���X�i���邩�G��ƃ_���[�W�j
//-----------------------------------------------------------
class DamageDrum : EnemyBase{
public:
	static int loadOnlyHr[] ;	//	�RD���f���̃R�s�[�ǂݍ��ݗp
	//int grHandle ;	//	�RD���f���̃n���h���B�`�擙�͂�������g�p�B
	int timer ;	//	�����^�C�}�[�B���ȏ�ɂȂ����炱�̃h�����ʂ��폜
	
	//VECTOR MV ;	//	�h�����ɒǉ��B���̂̈ړ����x
	VECTOR spin ;	//	��]���x

	//float realTimer ;	//	�b���Z�̃^�C�}�[	�~���b�܂Ő��m�B
	//VECTOR initPos;	// �����ʒu		�h�����N���X�ɒǉ�
	//VECTOR initVelocity;	// �����x	�h�����N���X�ɒǉ�

	DamageDrum(unsigned long long in_waitTime, VECTOR initPos, VECTOR initVelocity);
	~DamageDrum();
	virtual void Move() ;
	virtual void Draw() ;
};
int DamageDrum::loadOnlyHr[] = {0, 0};
//	�����̍\��	�o�����ԁi���݂͖����A��Ŏg���j,�@�����ʒu,�@�ړ����x
DamageDrum::DamageDrum(unsigned long long in_waitTime, VECTOR initPos, VECTOR initVelocity)
{
	waitTime = in_waitTime ;
	timer = 0;
	realTimer = 0;
	grHandle[0] = MV1DuplicateModel(loadOnlyHr[0]) ;	//	�\�ߓǂݍ���ł��郂�f�����R�s�[
	grHandle[1] = MV1DuplicateModel(loadOnlyHr[1]) ;	//	���񂾃h������ǂݍ���
	MV1SetPosition( grHandle[0], initPos);
	damage = 1 ;	//	�ڐG������P�_���[�W
	MV = initVelocity ;

	DamageDrum::initPos = initPos;	// �����ʒu
	DamageDrum::initVelocity = initVelocity;	// �����x

	MV1SetScale(grHandle[0], VGet(0.8f, 0.8f, 0.8f)) ;
	//MV1SetScale(grHandle[0], VGet(0.08f, 0.08f, 0.08f)) ;
	MV1SetRotationXYZ(grHandle[0], VGet(0,0, DegToRad * 90));
	spin = VGet(0, 0, DegToRad * ((float)GetRand(8) + 2.0f));
	ModelRotationAdd(grHandle[0], VGet(DegToRad * (float)GetRand(50),0,0));
	MV1SetupCollInfo(grHandle[0]) ;	//	�Փˏ����\�z
	Activation();
}
DamageDrum::~DamageDrum()
{
	MV1DeleteModel( grHandle[0] );	//	���f���̓ǂݍ��݃f�[�^��j������
	MV1DeleteModel( grHandle[1] );
}
void DamageDrum::Move()
{
	//ModelPosAdd(grHandle[0], MV) ;	//	�ړ����x�����݂̈ʒu�ɑ���

	VECTOR velocity =  VAdd(VScale(VGet(0.0f,-GRAVITY,0.0f),realTimer),initVelocity); // �����x�~���ԁ����̎����ł̑��x

	VECTOR acc =  VGet(0.0f,-0.5f*GRAVITY*realTimer*realTimer,0.0f);
	VECTOR pos = VAdd(VAdd(initPos,VScale(initVelocity,realTimer)),acc);
	
	//	�ȈՃo�E���h����
	if(pos.y < 0.5f && velocity.y < 0){
		initVelocity.y = -velocity.y * RESTITUTE;	//	��ɒ��˂鍂���̌���
		initVelocity.x = velocity.x * RESTITUTE ;	//	���x�̌���
		initVelocity.z = velocity.z * RESTITUTE ;	
		initPos = MV1GetPosition(grHandle[0]) ;
		spin = VScale(spin, 0.5f) ;
		realTimer = 0;
	}

	MV1SetPosition(grHandle[0], pos) ;

	ModelRotationAdd(grHandle[0], spin);	//	���f������]������
	if(hitFlag == false)
		MV1RefreshCollInfo(grHandle[0]); //	�Փˏ����X�V
	else{	//	���񂾃h�����ʂ𕁒ʂ̃h�����ʂƓ���������
		MV1SetPosition(grHandle[1], MV1GetPosition( grHandle[0] ));
		MV1SetRotationXYZ( grHandle[1], MV1GetRotationXYZ( grHandle[0] ));
	}
	timer++;
	realTimer += (float)IntervalTime / 1000.0f;	//	�~���b��b�P�ʂɒ����ĉ��Z 
	if(timer == 360)	//	�U�b�o�߂�����h�����ʂ��폜����
		alive = false;
}
void DamageDrum::Draw()
{
	MV1DrawModel( grHandle[hitFlag] ) ;	//	���f����`��
	//	�e�̕`��
	/*
	VECTOR shadowPos = MV1GetPosition(grHandle[0]);
	shadowPos.y = 0.05f;
	DrawBillboard3D( shadowPos, 0.5f, 0.5f, 1.0f, 90*DegToRad, circleShadow, TRUE) ;
	*/
}


//////////////////////////////////////
///////�C�e���[�^�N���X///////////////
//////////////////////////////////////

class ListIterator{
public:
	ListIterator() ;
	~ListIterator() ;
	ItemBase *IB ;	//	�A�C�e��
	KinectPlayer *player ;//	�v���C���[
	CrashPlayer *crashP;	//	�v���C���[������
	PB_Base *PB ;	//	�v���C���[�̒e
	BackGround *BG ;//	�w�i
	EnemyBase *E_Active ;	//	��ʏ�̓G
	EnemyBase *E_Wait ;		//	�҂���Ԃ̓G
	EnemyBulletBase *EB ;	//	�G�̒e
	EffectBase *Effect ;	//	�G�t�F�N�g
	PlayerBomBase *PBom;	//	�v���C���[�{��
	unsigned long long time ;	//	�o�ߎ���
	int kinectTime;	//	���ꂪ1�̎���kinect�̃f�[�^���擾����
	int panel ;	//	�p�l���̃O���t�B�b�N�n���h��
	unsigned long long grazeNum;	//	�O���C�Y�𐔂���
	int moveStageCnt ;	//	MoveStage�֐��̐���ϐ�
	int nowStage ;	//	���݂̃X�e�[�W�i1:�m�[�}���X�e�[�W�P	11:	�n�[�h�X�e�[�W�P

	//	�e�X�g�p�ǉ�
	int drumTimer ;
	BASEIMAGE ScreenShot[120];

	//	�|�[�Y�֌W
	int pauseAlpha ;		//	�|�[�Y�̃A���t�@�l����ϐ�
	int pauseAdd ;			//	�|�[�Y�̃A���t�@�l����
	int pauseGr ;			//	�|�[�Y�̃O���t�B�b�N�n���h��
	systemTime *sysTime ; 
	int score ;				//	�X�R�A

	//	�^�C���A�E�g�֐��֌W
	int ranking[RANKING_MAX + 1];
	int rankPosY[11];
	bool timeoutInitialized ;
	float timeoutX, timeoutY;
	int timeoutGrHandle;
	int rankTimer ;	//	�����L���O�\���Ŏg������ϐ�
	int playerRank ;

	//	�J���[�C���[�W�iRGB�J�����j�֌W
	BASEIMAGE BaseImage[60] ;	//	�摜���擾����ۂɎg�p
	int	ImageGrHandle[60] ;		//	�J���[�C���[�W�n���h��
	int displayPos ;			//	���݂̕\���ʒu�B�t���b�N����ŃX�N���[���V���b�g��ʂɈړ�
	int displayMove ;			//	��ʂ̈ړ����x
	float displayMoveCouse;			//	��ʂ̈ړ�����
	VECTOR revRHand ;				//	�O�t���[���̉E��̈ʒu
	bool lookonRHand;			//	�E�肪�O�ɂ��邩(true�̂Ƃ��͑O�ɂ���)



	void Move() ;	//	�G�A�e�A������������
	void Draw() ;	//	�G�A�e�A�������`�悷��
	void DeleteEnemyBullet() ;	//	�G�e��S�ď�������
	void HitJadge() ;	//	�����蔻����s��
	void CreateEnemy();	//	�҂���Ԃ̓G���o��������
	void MoveStage() ;	//	�X�e�[�W�ړ�
	void Pause() ;	//	�ꎞ��~�B�v���C���[���F������Ă��Ȃ��Ƃ��B
	void Timeout() ;	//	���Ԑ؂�B
	void InitRanking() ;	//	���ׂẴ����L���O��0�ŏ���������
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
	panel = LoadGraph(PANEL);	//	�E�p�l���̃��[�h
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
	InitRanking();	//	�����L���O��S��0�ŏ�����
	for(int i = 0; i < 10; i++)	//	�����L���O�̏�����
		ranking[i] = (10-i)*5000;
	for(int i = 0; i < 60; i++){
		BaseImage[i].Width  = 640 ;
		BaseImage[i].Height = 480 ;
		CreateARGB8ColorData( &BaseImage[i].ColorData );              // �L�l�N�g����̃f�[�^��8bit*4�Ȃ��CreateFullColorData����ʖڂ��ۂ�
        BaseImage[i].MipMapCount = 0 ;	// �~�b�v�}�b�v�ł͂Ȃ��̂łO
		WaitForSingleObject(ColorImageEv, INFINITE) ;
		CreateGraphicHandle(&ImageGrHandle[i], ColorImageStream, &BaseImage[i]);	//	�O���t�B�b�N�n���h���𐶐�����
	}
}
ListIterator::~ListIterator(){	//	new�Ŋm�ۂ������X�g�����
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
		new PB_Effect(EB_iter->x, EB_iter->y) ;	//	���@�e�̒��e�G�t�F�N�g�𐶐�
		EB_iter->alive = false;	//	�G�e�̐����t���O��false�ɂ���
	}
}

void ListIterator::DeleteDrumcan(){
	//	�h�����ʂ̏�������
	EnemyBase *Enow_p = E_Active->next_p ;
	while(Enow_p != Enow_p->activeFirst_p){
		Enow_p = Enow_p->next_p ;
		delete Enow_p->rev_p;	
	}

}

void ListIterator::HitJadge(){
	//	���@�ƓG�e�̓����蔻��
	EnemyBulletBase *EB_iter ;
	if(player->alive==true && player->noHitTime==0){	
		for(EB_iter = EB->next_p; EB_iter!=EB_iter->first_p; EB_iter=EB_iter->next_p){
			/*
			//	���@�ė�����
			if(isHit(player->x, player->y, player->hitSize,
			EB_iter->x, EB_iter->y, EB_iter->size)){
			player->alive = false;
			PlaySoundMem(player->deathHandle, DX_PLAYTYPE_BACK);
			}
			//�O���C�Y����
			else if(EB_iter->grazed == false && isHit(player->x, player->y, player->grazeSize,
			EB_iter->x, EB_iter->y, EB_iter->size)){
			grazeNum++;
			EB_iter->grazed = true;
			new GrazeEffect(player->x, player->y);	//	�O���C�Y�G�t�F�N�g���o��
			}
			*/
		}
	}
	//	���@�ƓG�̓����蔻��
	if(player->alive==true && player->noHitTime==0){	
		for(EnemyBase *E_iter = E_Active->next_p; E_iter!=E_iter->activeFirst_p; E_iter=E_iter->next_p){
			if(isHit(player->x, player->y, player->hitSize,
				E_iter->x, E_iter->y, E_iter->size / 3)){
					player->alive = false;
					PlaySoundMem(player->deathHandle, DX_PLAYTYPE_BACK);
			}
		}
	}

	//	�G�Ǝ��@�e�̓����蔻��
	//	�ǉ�����
	PB_Base *PB_iter = PB->next_p;
	EnemyBase *E_iter = E_Active->next_p;
	for(; E_iter!=E_iter->activeFirst_p; E_iter=E_iter->next_p){
		for(PB_iter = PB->next_p; PB_iter!=PB_iter->first_p; PB_iter=PB_iter->next_p){

			MV1_COLL_RESULT_POLY_DIM mv1_coll = MV1CollCheck_Sphere(E_iter->grHandle[0], -1, PB_iter->nowV, HAND_SIZE);
			if(E_iter->hitFlag == false && mv1_coll.HitNum > 0){
				E_iter->PlayCollisionSE();
				E_iter->initPos = MV1GetPosition( E_iter->grHandle[0] );	//	�����ʒu�����݈ʒu�ɕύX
				E_iter->initVelocity = VScale(VSub(PB_iter->nowV, PB_iter->revV), DRUM_COLLISION_SCALE) ;	//	��̓��������̂܂܏����x�ɐݒ�
				E_iter->initVelocity = VScale(E_iter->initVelocity, 60.0f);	//	��̓�����b���ɏC��
				E_iter->realTimer = 0;	//	�d�͌v�Z�Ŏg���^�C�}�[������������
				E_iter->hitFlag = true ;	//	2�x��ɐG��Ȃ��悤�ɂ���
				VECTOR V = VSub(PB_iter->nowV, PB_iter->revV);	//	�͂̋������v�Z
				float VT = (V.x * V.x) + (V.y * V.y) + (V.z * V.z) ;
				int i;
				for(i = 0; i < 4 ; i++){	//	�h�����ʂ̂ւ��ݗʂ𔻒�Afor�������ł������o�O�����̂ŏC��
					if(drumcanStateBasis[i] > VT)
						E_iter->state = i;
				}
				KinectPlayer::p->stock -= E_iter->damage ;
				if(VSquare(E_iter->initVelocity) > 0.2f)new SerifEffect(E_iter->initPos,0) ;	//	�h�S�I�I���𐶐�
				else new SerifEffect(E_iter->initPos, 4);	//	�C�e�𐶐�
				int score = ScoreCalc(E_iter->initVelocity) ;
				this->score += score ;
				new ScoreNumEffect(PB_iter->nowV, score) ;
				//new PB_Effect(PB_iter->x, PB_iter->y) ;	//	���@�e�̒��e�G�t�F�N�g�𐶐�
				//E_iter->alive = false;	//	�G�̐����t���O��false�ɂ���
				//new E_DeleteEffect(E_iter->x, E_iter->y) ;	//	�G���j�G�t�F�N�g�𐶐�

			}
			MV1CollResultPolyDimTerminate( mv1_coll );
		}
	}
	//	�{���ƓG�e�̓����蔻��
	PlayerBomBase *PBom_iter = PBom->next_p;
	for(; PBom_iter!=PBom_iter->first_p; PBom_iter=PBom_iter->next_p){
		for(EB_iter = EB->next_p; EB_iter!=EB_iter->first_p; EB_iter=EB_iter->next_p){
			if(isHit(PBom_iter->x, PBom_iter->y, PBom_iter->size,
				EB_iter->x, EB_iter->y, EB_iter->size)){
					new PB_Effect(EB_iter->x, EB_iter->y) ;	//	���@�e�̒��e�G�t�F�N�g�𐶐�
					EB_iter->alive = false;	//	�G�e�̐����t���O��false�ɂ���

			}
		}
	}
	//	�{���ƓG�̓����蔻��
	E_iter = E_Active->next_p;
	for(; E_iter!=E_iter->activeFirst_p; E_iter=E_iter->next_p){
		for(PBom_iter=PBom->next_p ; PBom_iter!=PBom_iter->first_p; PBom_iter=PBom_iter->next_p){
			if(isHit(PBom_iter->x, PBom_iter->y, PBom_iter->size,
				E_iter->x, E_iter->y, E_iter->size)){
					E_iter->hp -= PBom_iter->power;	//	�{���̃p���[�������G��HP�����炷
					new PB_Effect(PBom_iter->x, PBom_iter->y) ;	//	���@�e�̒��e�G�t�F�N�g�𐶐�
					if(E_iter->hp < 0){
						E_iter->alive = false;	//	�G�̐����t���O��false�ɂ���
						new E_DeleteEffect(E_iter->x, E_iter->y) ;	//	�G���j�G�t�F�N�g�𐶐�
					}
			}
		}
	}
	//	�A�C�e�����E�����Ƃ�
	if(player->alive == true){	
		for(ItemBase *IB_iter = IB->next_p; IB_iter!=IB_iter->first_p; IB_iter=IB_iter->next_p){
			if(isHit(player->x, player->y, player->hitSize,
				IB_iter->x, IB_iter->y, IB_iter->size)){
					IB_iter->Get();
					IB_iter->alive = false;	//	�A�C�e���̐����t���O��false�ɂ��č폜����
					//	PlaySoundMem(player->deathHandle, DX_PLAYTYPE_BACK);
			}
		}
	}

}


void ListIterator::Move(){
	/*
	//	kinect�̃f�[�^��2�t���[���ɂP��擾����	kinect:��30fps	�Q�[��:60fps�̂���
	kinectTime = (kinectTime + 1) % 2 ;
	if(kinectTime == 1){
	::WaitForSingleObject(SkeletonEv , INFINITE);	//	�{�[����񂪍X�V�����܂ő҂�
	NuiSkeletonGetNextFrame(0, &playerSkeleton) ;	//	�{�[�������擾
	}
	*/
	//	kinect�̃f�[�^���X�V���ꂽ�������f�[�^���擾����
	if(::WaitForSingleObject(SkeletonEv , 0) == WAIT_OBJECT_0){
		kinectUpdateFlag = true ;
		NuiSkeletonGetNextFrame(0, &playerSkeleton) ;	//	�{�[�������擾
		NuiTransformSmooth(&playerSkeleton, NULL) ;		//	�擾�����{�[�����Ƀt�B���^���|����
	}else kinectUpdateFlag = false ;
	
	/*
	//	�J�������̉���A�y�эĎ擾
	if(::WaitForSingleObject(ColorImageEv , 0) == WAIT_OBJECT_0){
		NuiImageStreamReleaseFrame( ColorImageStream, ImageFrame );
		ImageFrame->pFrameTexture->UnlockRect(0);
		NuiImageStreamGetNextFrame( ColorImageStream, 0, &ImageFrame ) ;
	}
	int igh = sysTime->remainingTime / 1000 ; 
	LoadImageForKinectCameraRGB( &ImageGrHandle[igh], ImageFrame, &BaseImage[igh] );	//	��b���ƂɃJ���[�C���[�W��ۑ�
	*/
	
	
	//	�J�������̉���A�y�эĎ擾(�������g���j
	if(::WaitForSingleObject(ColorImageEv, 0) == WAIT_OBJECT_0){
		int igh = sysTime->remainingTime / 1000 ; 
		LoadImageForKinectCameraRGB( &ImageGrHandle[igh], ColorImageStream, &BaseImage[igh] );	//	��b���ƂɃJ���[�C���[�W��ۑ�
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
	HitJadge() ;	//	�����蔻�������

	sysTime->Move();

	//	�o�ߎ��Ԃ��擾�A�X�V
	NowTime = GetNowCount();
	IntervalTime = NowTime - RevTime ;	//	IntervalTime�͌o�ߎ��Ԃ�ۑ����Ă����ꏊ
	RevTime = NowTime ;

	int i ;
	//BG->Move() ;		//	�w�i�̃X�N���[������
	if(player->alive == true)
		player->Move() ;	//	���@�̈ړ�����
	else if(player->alive == false){
		crashP->Move() ;
		DeleteEnemyBullet() ;
	}
	
	PB_Base *PBnow_p = PB->next_p ;
	//	���@�e�̈ړ�����
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
	//	�G�e�̈ړ�����
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
	//	�G�̈ړ�����
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
	//	�G�t�F�N�g�̊g�又��
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
	//	�v���C���[�{�����ړ�������
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
	//	�A�C�e���̈ړ�����
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
	//	�e�X�g���A�h�����ʂ��o��������
	if(CheckHitKey(KEY_INPUT_RIGHT) && !rKey){
		CameraPosition.x += 0.1f; // �J�������E�� masuda
		rKey = true;
	}else if(!CheckHitKey(KEY_INPUT_RIGHT))rKey = false;
	if(CheckHitKey(KEY_INPUT_LEFT) && !lKey){
		CameraPosition.x -= 0.1f;  // masuda
		lKey = true;
	}else if(!CheckHitKey(KEY_INPUT_LEFT))lKey = false;
	if(CheckHitKey(KEY_INPUT_UP) && !uKey){
		CameraPosition.y += 0.1f; // masuda �J���������
		uKey = true;
	}else if(!CheckHitKey(KEY_INPUT_UP))uKey = false;
	if(CheckHitKey(KEY_INPUT_DOWN) && !dKey){
		CameraPosition.y -= 0.1f; // masuda
		dKey = true;
	}else if(!CheckHitKey(KEY_INPUT_DOWN))dKey = false;
	//	�Ƃ肠�����A���x�̓_�~�[�f�[�^�������Ă܂�

	if (rKey || lKey || uKey || dKey) {
		SetCameraPositionAndTargetAndUpVec( 
		VAdd(CameraPosition,CameraShake), // �J�����̈ʒu�@�����ʒu�́A�����P���[�g���A���_���T���[�g�����
		VGet( XORG, YORG, ZORG ),	//�@���_���݂Ă���(������kinect�v���[�₪����͂�)
		VGet( 0.0f, 1.0f, 0.0f ) ) ; // �J�����̌X��
	}

	drumTimer = (drumTimer + 1) % 1200 ;
	//drumTimer++;
	//	���ʂ̃h�����ʏo��

	int span =  sysTime->remainingTime * (120 - 35)/(60000 - 0) + 35;
	//if (span < 35) span=35;
	//span = 10;
	if(drumTimer % span == 0) { 
			// x���������x0,�����������x11m/�b,z���������x-3.9m/�b
		float x0 =   0.0f + ((GetRand(100)-50)/100.0f)*1.5f;
		float y0 =  11.0f + ((GetRand(100)-50)/100.0f)*0.5f;
		float z0 =  -5.5f + ((GetRand(100)-50)/100.0f)*1.0f;
		new NormalDrum(time, VGet(XORG, YORG, 12.0f),VGet(x0,y0,z0)); // masuda
	}
		/*
	if(drumTimer % 120 == 0) { 
	
		// x���������x0,�����������x11m/�b,z���������x-3.9m/�b
		float x0 =   0.0f + ((GetRand(100)-50)/100.0f)*1.5f;
		float y0 =  11.0f + ((GetRand(100)-50)/100.0f)*0.5f;
		float z0 =  -5.5f + ((GetRand(100)-50)/100.0f)*1.0f;
		new NormalDrum(time, VGet(XORG, YORG, 12.0f),VGet(x0,y0,z0)); // masuda
	}
	//	�c��Q�O�b�ȉ��ɂȂ�����A�h�����ʂ̏o���Ԋu�𑁂߂�
	if((drumTimer+60) % 120 == 0 && sysTime->remainingTime < 20000){
		// x���������x0,�����������x11m/�b,z���������x-3.9m/�b
		float x0 =   0.0f + ((GetRand(100)-50)/100.0f)*1.5f;
		float y0 =  11.0f + ((GetRand(100)-50)/100.0f)*0.5f;
		float z0 =  -5.5f + ((GetRand(100)-50)/100.0f)*1.0f;
		new NormalDrum(time, VGet(XORG, YORG, 12.0f),VGet(x0,y0,z0)); // masuda
	}
	//	�c��P�O�b�ȉ��Ńh�����ʂ̏o�������Q�{�ɂȂ�i�����ɂQ�o������j
	if((drumTimer) % 60 == 0 && sysTime->remainingTime < 10000){
		// x���������x0,�����������x11m/�b,z���������x-3.9m/�b
		float x0 =   0.0f + ((GetRand(100)-50)/100.0f)*1.5f;
		float y0 =  11.0f + ((GetRand(100)-50)/100.0f)*0.5f;
		float z0 =  -5.5f + ((GetRand(100)-50)/100.0f)*1.0f;
		new NormalDrum(time, VGet(XORG, YORG, 12.0f),VGet(x0,y0,z0)); // masuda
	}
	//	�_���[�W�h�����o��
	if(drumTimer == 0) {
		float x0 =   0.0f + ((GetRand(100)-50)/100.0f)*2.0f;
		float y0 =  11.0f + ((GetRand(100)-50)/100.0f)*0.5f;
		float z0 =  -5.5f + ((GetRand(100)-50)/100.0f)*1.0f;
		new NormalDrum(time, VGet(XORG, YORG, 12.0f),VGet(x0,y0,z0)); // masuda
	}
	*/
	CreateEnemy() ;	//	�G���o��������
}

void ListIterator::Draw(){
	int i, j ;
	//BG->Draw() ;		//	�w�i�̕`�揈��

	if(player->alive == true)
		player->Draw() ;	//	���@�̕`�揈��
	else if(player->alive == false)
		crashP->Draw() ;
	/*
	ItemBase *IBnow_p = IB->next_p ;
	//	�A�C�e���̕`�揈��
	while(IBnow_p != IB->first_p){
		IBnow_p->Draw() ;
		IBnow_p = IBnow_p->next_p ;
	}
	*/
	PB_Base *PBnow_p = PB->next_p ;
	//	���@�e�̕`�揈��
	while(PBnow_p != PB->first_p){
		PBnow_p->Draw() ;
		PBnow_p = PBnow_p->next_p ;
	}
	//	�G�̕`�揈��
	EnemyBase *Enow_p = E_Active->next_p ;
	while(Enow_p != E_Active->activeFirst_p){
		Enow_p->Draw() ;
		Enow_p = Enow_p->next_p ;
	}
	/*
	EnemyBulletBase *EBnow_p = EB->next_p ;
	//	�G�e�̕`�揈��
	while(EBnow_p != EB->first_p){
		EBnow_p->Draw() ;
		EBnow_p = EBnow_p->next_p ;
	}
	//	�v���C���[�{���̕`�揈��
	PlayerBomBase *PBomNow_p = PBom->next_p ;
	while(PBomNow_p != PBom->first_p){
		PBomNow_p->Draw() ;
		PBomNow_p = PBomNow_p->next_p ;
	}
	*/
	//	�G�t�F�N�g�̕`�揈��
	EffectBase *EffectNow_p = Effect->next_p ;
	while(EffectNow_p != Effect->first_p){
		EffectNow_p->Draw() ;
		EffectNow_p = EffectNow_p->next_p ;
	}
	/*
	//	�c�胉�C�t�\��
	DrawString(0, 0, "�c�胉�C�t�F",GetColor(255,255,255), GetColor(255,255,255));
	for(i = 0; i < player->stock; i++)	//	�c�@�A�C�R���̕\��
		DrawGraph(50 + i * 30, 10, player_icon, TRUE);
		*/

	
	sysTime->Draw(300, 0);	//	�c�莞�Ԃ�\��
	SetFontSize(50) ;
	DrawFormatString(830, 0, GetColor(255,255,255), "%06d", score) ;

	int secondTime = (sysTime->remainingTime + 100) / 1000 ;	//	0.1�b�O�̃f�[�^�����ĕ`�悷��i�J�N���h�~�j
	DrawExtendGraph(800, 200, 1000, 350, ImageGrHandle[secondTime], FALSE) ;
	/*
	DrawGraph(0, 0, panel, TRUE);	//	�E�p�l����`��
	for(i = 0; i < player->stock; i++)	//	�c�@�A�C�R���̕\��
	DrawGraph(480 + i * 30, 110, player_icon, TRUE);
	for(i = 0; i < player->bom; i++)	//	�c�{���A�C�R���̕\��
	DrawGraph(480 + i * 30, 195, bom_icon, TRUE);
	*/
	/*
	SetFontSize(25);
	//	�O���C�Y���̕\��
	char grazeNumS [20];
	sprintf(grazeNumS, "%010d", grazeNum) ;	//	sprintf�֐��͏o�͐悪������ɂȂ�������
	DrawString(420, 320, "Graze",GetColor(0,0,0), GetColor(255,255,255)) ;	//	�uGraze�v��`�� 
	DrawString(480, 360, grazeNumS , GetColor(0,0,0) , GetColor(255,255,255));	//	�O���C�Y���̕`��
	//	�X�R�A�̕\��
	char scoreNumS [20];
	sprintf(scoreNumS, "%010d", Score) ;
	DrawString (420, 400, "Score", GetColor(0,0,0), GetColor(255,255,255)) ;	//	�uScore�v��`��
	DrawString (480, 440, scoreNumS , GetColor(0,0,0), GetColor(255,255,255));	//	�X�R�A��`��
	*/
}
//	���߂�ꂽ���ԂɂȂ�����G���o��
void ListIterator::CreateEnemy(){
	time++ ;
	//	�{�X�݂̂ɂȂ��Ă�����A�҂����ԂȂ��Ń{�X���o���B
	//	�ڍ׏���	�F��Q�b���Ƃɑ҂���Ԃ̓G���{�X�̂݁A�t�B�[���h�ɓG�����Ȃ������ׂ�B
	//	true��������A�{�X�̏o�����Ԃ����݂̎��Ԃɏ���������i��1�b�x�������ďo���j
	if((time % 120) == 0 && (E_Wait->next_p->next_p == E_Wait) &&
		(E_Active->next_p == E_Active)){
			//time = E_Wait->next_p->waitTime - 60;
			E_Wait->next_p->waitTime = time + 100;
	}
	//	�҂���Ԃ̓G���`�F�b�N�A�����ɂ����Ă������ʂɏo��������
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
		BG->alpha-- ;	//	���X�ɔw�i���Â�����
	}
	//	���X�e�[�W�̔w�i��ǂݍ���
	else if(moveStageCnt == 495){
		DeleteGraph(BG->graphicHandle) ;
		switch(nowStage){
		case 1:
		case 11:
			nowStage++;	//	���݂̃X�e�[�W�����̃X�e�[�W�Ɉڍs������
			BG = new BackGround(BACKGROUND_GRAPHIC2, BACKGROUND_SPEED) ;
			BG->alpha = 0;
			break ;
		default :
			break ;
		}
	}
	//	���F���珙�X�ɃX�e�[�W�𖾂邭����
	else if(moveStageCnt < 750){
		BG->alpha++ ;
	}
	//	�X�e�[�W�̏������������G�f�[�^�ǂݍ���
	else{
		time = 0;
		StageClearFlag = false ;
		//LoadEnemyData(nowStage) ;
	}
}

void ListIterator::Pause(){
	Draw() ;	//	�w�i��`��
	DrawAlpha(0, 0, pauseAlpha, pauseGr) ;	//	�|�[�Y��ʂ�`��
	const int alphaSpeed = 2 ;
	if(pauseAlpha == 0 && pauseAdd == 0 && SkeletonTracking == false){	//	�ʏ��Ԃ���|�[�Y�ֈڍs�����ۂ̈����
		pauseAdd = alphaSpeed;
	}else if(pauseAdd > 0){	//	���X�Ƀ|�[�Y��ʂ��o���Ă���r��
		pauseAlpha += pauseAdd ;
		if(::WaitForSingleObject(SkeletonEv , 0) == WAIT_OBJECT_0){
			NuiSkeletonGetNextFrame(0, &playerSkeleton) ;	//	�{�[�������擾
			for(int i = 0; i < 6; i++){
				if(playerSkeleton.SkeletonData[i].eTrackingState != NUI_SKELETON_NOT_TRACKED)
					pauseAdd = -alphaSpeed ;	//	�|�[�Y���������邩����
			}
		}
		if(pauseAlpha > 255){	//	�|�[�Y��ʂ����S�ɏo�� 
			pauseAlpha = 255 ;
			pauseAdd = 0;
		}
	}else if(pauseAdd < 0){	//	���X�Ƀ|�[�Y��������
		pauseAlpha += pauseAdd ;
		if(pauseAlpha < 0){	//	�|�[�Y��ʂ����S�ɉ���
			pauseAlpha = 0;
			pauseAdd = 0;
			SkeletonTracking = true ;
			sysTime->revRetime();	//	�c�莞�Ԍv���̃��t���b�V��
			DeleteDrumcan();
		}
	}else if(pauseAlpha == 255){	//	�|�[�Y��
		if(::WaitForSingleObject(SkeletonEv , 0) == WAIT_OBJECT_0){
			NuiSkeletonGetNextFrame(0, &playerSkeleton) ;	//	�{�[�������擾
			for(int i = 0; i < 6; i++){
				if(playerSkeleton.SkeletonData[i].eTrackingState != NUI_SKELETON_NOT_TRACKED)
					pauseAdd = -alphaSpeed ;	//	�|�[�Y���������邩����
			}
		}
	}
}

//	���Ԑ؂�
void ListIterator::Timeout(){
	if(timeoutInitialized == false){
		playerRank = RANKING_MAX - 1;
		for(int i = 0; i < RANKING_MAX; i++){	//	�v���C���[�̏��ʂ𒲂ׂ�
			if(ranking[i] < score){
				playerRank = i;
				break;
			}
		}
		for(int i = 9; i >= playerRank; i--){	//	�v���C���[�̏��ʂ��牺�����炵�đ}��
			ranking[i] = ranking[i - 1];
		}
		ranking[playerRank] = score ;

		for(int i = 0; i < 10; i++)	//	y�̏����l������
			rankPosY[i] = -900 + i * 60 ;
		timeoutX = 0;	timeoutY = -WINDOWSIZE_Y;
		rankTimer = 0;	//	�����L���O�̕\���Ɏg������ϐ��̏�����
		timeoutInitialized = true;
		displayMove = 0;
		displayPos = 0;
		lookonRHand = false;
	}
	if(timeoutY < 0){	//	�^�C���A�b�v�Ƃ������ړ�������
		if(timeoutY > -10)timeoutY += 10;
		else timeoutY = 0;

		DrawGraph(timeoutX, timeoutY, timeoutGrHandle, TRUE) ;
		DrawString(400, timeoutY + 400, "�^�C���A�b�v�I�I", GetColor(255,255,255));
	}
	else if(rankTimer < 120){	//	�҂�����
		rankTimer++;

		DrawGraph(timeoutX, timeoutX, timeoutGrHandle, TRUE) ;
		DrawString(400, timeoutY + 400, "�^�C���A�b�v�I�I", GetColor(255,255,255));
	}
	else if(rankTimer < 360){	//	�S�b��(240�t���[��)�̊ԂɃ����L���O���ォ��X���C�h�C��������
		DrawGraph(timeoutX, timeoutX, timeoutGrHandle, TRUE) ;	
		for(int i = 0; i < 10; i++){
			rankPosY[i] += WINDOWSIZE_Y / 180;
			SetFontSize(40);
			DrawFormatString(RANKING_SCORE_POSITION_X, rankPosY[i], GetColor(255,255,255), "%4d�ʁ@�@%06d", i+1, ranking[i]) ;
		}
		rankTimer++;
	}
	else if(rankTimer < 500){//	�����L���O�\��������
		rankTimer++;
		DrawGraph(timeoutX, timeoutX, timeoutGrHandle, TRUE) ;
		SetFontSize(40);
		for(int i = 0; i < 10; i++)
			DrawFormatString(RANKING_SCORE_POSITION_X, rankPosY[i], GetColor(255,255,255), "%4d�ʁ@�@%06d", i+1, ranking[i]) ;
		if(playerRank >= 10){	//	�v���C���[�̏��ʂ�11�ʈȉ���������A11�ʂ̈ʒu�ɕ\��
			if(rankTimer % 20 < 10)	DrawFormatString(RANKING_SCORE_POSITION_X, 650, GetColor(255,20,20), "%4d�ʁ@�@%06d", playerRank +1, ranking[playerRank]) ;
			else					DrawFormatString(RANKING_SCORE_POSITION_X, 650, GetColor(255,255,255), "%4d�ʁ@�@%06d", playerRank +1, ranking[playerRank]) ;
		}
		else if(rankTimer % 20 < 10)	//	�����̃����N��_�ŕ\��������
			DrawFormatString(RANKING_SCORE_POSITION_X, rankPosY[playerRank], GetColor(255,20,20), "%4d�ʁ@�@%06d", playerRank +1, ranking[playerRank]) ;
	}
	else{	//	�����L���O�̕\�����I������ꍇ	
		rankTimer++;
		DrawGraph(timeoutX, timeoutX, timeoutGrHandle, TRUE) ;
		SetFontSize(40);
		for(int i = 0; i < 10; i++)
			DrawFormatString(displayPos +RANKING_SCORE_POSITION_X, rankPosY[i], GetColor(255,255,255), "%4d�ʁ@�@%06d", i+1, ranking[i]) ;
		if(playerRank >= 10){	//	�v���C���[�̏��ʂ�11�ʈȉ���������A11�ʂ̈ʒu�ɕ\��
			if(rankTimer % 20 < 10)	DrawFormatString(displayPos +RANKING_SCORE_POSITION_X, 650, GetColor(255,20,20), "%4d�ʁ@�@%06d", playerRank +1, ranking[playerRank]) ;
			else					DrawFormatString(displayPos +RANKING_SCORE_POSITION_X, 650, GetColor(255,255,255), "%4d�ʁ@�@%06d", playerRank +1, ranking[playerRank]) ;
		}
		else if(rankTimer % 20 < 10)	//	�����̃����N��_�ŕ\��������
			DrawFormatString(displayPos +RANKING_SCORE_POSITION_X, rankPosY[playerRank], GetColor(255,20,20), "%4d�ʁ@�@%06d", playerRank +1, ranking[playerRank]) ;
		SetFontSize(30);
		DrawString(displayPos +150, 700, "�ăv���C����ɂ́A����𓪂���ɏグ�Ă��������B", GetColor(255,255,255));
		//	kinect�̃f�[�^���X�V���ꂽ�������f�[�^���擾����
		if(::WaitForSingleObject(SkeletonEv , 0) == WAIT_OBJECT_0){
			kinectUpdateFlag = true ;
			NuiSkeletonGetNextFrame(0, &playerSkeleton) ;	//	�{�[�������擾
			NuiTransformSmooth(&playerSkeleton, NULL) ;		//	�擾�����{�[�����Ƀt�B���^���|����
		}else kinectUpdateFlag = false ;
		//	�X�N���[���V���b�g�̕`��
		DrawExtendGraph(displayPos -1000, 100, displayPos -600, 400, ImageGrHandle[10], FALSE);
		DrawExtendGraph(displayPos -500, 100, displayPos -100, 400, ImageGrHandle[30], FALSE);
		DrawExtendGraph(displayPos -1000, 450, displayPos -600, 750, ImageGrHandle[40], FALSE);
		DrawExtendGraph(displayPos -500, 450, displayPos -100, 750, ImageGrHandle[50], FALSE);

		VECTOR head, RHand, LHand, center;
		GetSkeletonV(playerSkeleton, &head, NUI_SKELETON_POSITION_HEAD);
		GetSkeletonV(playerSkeleton, &RHand, NUI_SKELETON_POSITION_HAND_RIGHT);
		GetSkeletonV(playerSkeleton, &LHand, NUI_SKELETON_POSITION_HAND_LEFT);
		//	������グ�Ă�����
		if(head.y < RHand.y && head.y < LHand.y){
			rankTimer = 0;
			timeoutInitialized = false;
			sysTime->initTime(60);
			score = 0;
			ScreenShotCnt = 0;
			DeleteDrumcan();
		}
		GetSkeletonV(playerSkeleton, &center, NUI_SKELETON_POSITION_SPINE) ;
		//	�E���O�ɏo������
		if((center.z-RHand.z) > 0.3 && (center.z-LHand.z) > 0.3 &&!lookonRHand){	//	�E������߂đO�ɏo������
			revRHand = RHand ;
			lookonRHand = true ;
			displayMoveCouse = 0;
		}else if((center.z-RHand.z) > 0.3 && lookonRHand){	//	�E���O�ɏo���ē���������
			displayMove = (RHand.x - revRHand.x) * 500;
			displayMoveCouse += (RHand.x - revRHand.x);
			revRHand = RHand ;
		}else{	//	������ɖ߂�����A�E��̕ߑ�����������
			if(displayMoveCouse > 0) displayMove = 30;
			else if(displayMoveCouse < 0)displayMove = -30;
			displayMoveCouse = 0;
			lookonRHand = false ;		
		}
		displayPos += displayMove;	//	�\����ʂ𓮂���
		//	��ʂ̈ʒu���K��̈ʒu�ɖ߂�
		if(displayPos > WINDOWSIZE_X) displayPos = WINDOWSIZE_X;
		else if(displayPos < 0)displayPos = 0 ;

		//	ctrl + S �ŃZ�[�u��ʈȍ~
		if((CheckHitKey(KEY_INPUT_RCONTROL) || CheckHitKey(KEY_INPUT_LCONTROL)) && CheckHitKey(KEY_INPUT_S))
			SaveScreenShot();
	}
}

void ListIterator::SaveScreenShot(){

	//	�w�b�_�[
	unsigned char heder[] = {
		0x42, 0x4D, 0x36, 0x10, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,
		0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 0xE0, 0x01, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x10, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
		unsigned char *BmpData = new unsigned char[3000000];
		for(int i = 0; i < (16*3+6); i++)
			BmpData[i] = heder[i];	//	�w�b�_�[���R�s�[	
		
		int captureGr = MakeGraph( 1024, 768 ) ;
		char folderName[50];
		GetDrawScreenGraph(0, 0, 1024, 768, captureGr) ;
		DrawGraph(0, 0, captureGr, FALSE) ;
		while(!ScreenFlip() && !ClearDrawScreen() && !ProcessMessage() ){
			DrawGraph(0, 0, captureGr, FALSE) ;
			DrawBox(300, 400, 700, 700, GetColor(255,255,255), TRUE) ;
			SetFontSize(40);
			DrawString(320, 400, "�t�H���_������͂��Ă�������(Esc�L�[�������ƕۑ����Ȃ�)", GetColor(0, 0, 0)) ;
			//	���p���������́BEsc�L�[�ŏI������
			if(KeyInputSingleCharString( 340, 460, 40, folderName, TRUE) == 2)	
				return;
			if(!PathIsDirectory(TEXT(folderName))){
				CreateDirectory(TEXT(folderName), NULL) ;
				break;
			}else{
				DrawString(300, 500, "�������O�̃t�H���_�����݂��܂��B�ʂ̖��O�ɂ��Ă��������B", GetColor(255,0,0));
				ScreenFlip();
				WaitKey();
			}
		}
		
		//	�摜�ۑ�����
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
		//	�o�b�t�@���w�肵���t�H���_��jpeg�ϊ����ăR�s�[
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
		//	�o�b�t�@�t�H���_�̉摜���R�s�[

		char soucePath[30];
		char destPath[30];
		for(int i = 0; i < SCREENSHOT_NUM; i++){
			sprintf(soucePath, "ScreenShot_Buffa\\%d.bmp", i);
			sprintf(destPath,  "%s\\%d.bmp", folderName, i)  ;
			CopyFile(soucePath, destPath, FALSE) ;
		}

		DrawString(300, 500, "����ɕۑ����܂���", GetColor(255,255,255));
		ScreenFlip();
		WaitKey();

}


/////////////////////////////////////
///////main�֐�//////////////////////
/////////////////////////////////////
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow )
{
	
	//ChangeWindowMode( TRUE ) ;		//	�E�B���h�E�ŊJ��

	SetGraphMode(WINDOWSIZE_X, WINDOWSIZE_Y, 32);	//	�E�B���h�E�T�C�Y�ݒ�
	
	SetMainWindowText("�h�����ʃN���b�V���I�I");
	if( DxLib_Init() == -1 )		// �c�w���C�u��������������
	{
		return -1 ;			// �G���[���N�����璼���ɏI��
	}
	
	

	SetDrawScreen( DX_SCREEN_BACK ) ; //�`���𗠉�ʂ�
	ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE) ;	//	�A���`�G�C���A�X���G�b�W�t���t�H���g
	//	���f���ȊO�ɂ�Z�o�b�t�@���g��
	SetUseZBuffer3D(TRUE) ;
	SetWriteZBuffer3D(TRUE) ;


	//	kinect�̏�����
	HRESULT hr = NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX |
		NUI_INITIALIZE_FLAG_USES_SKELETON | NUI_INITIALIZE_FLAG_USES_COLOR);
	if(FAILED(hr))
		return -1;
	SkeletonEv = ::CreateEventA(NULL, TRUE, FALSE, NULL);
	hr = NuiSkeletonTrackingEnable(SkeletonEv, 0) ;
	if(FAILED(hr))
		return -1;
	//	RGB�J�����̏�����
	ColorImageEv = ::CreateEventA(NULL, TRUE, FALSE, NULL);
	hr = NuiImageStreamOpen( NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, 0, 2, ColorImageEv, &ColorImageStream );
	if(FAILED(hr))
		return -1;
	//WaitForSingleObject(ColorImageEv, INFINITE) ;	//	�J���[�C���[�W�̍X�V��҂�
	//NuiImageStreamGetNextFrame(ColorImageStream, 0, &ImageFrame);
	/*
		Range = 2000.0f ;
	Atten0 = 0.0f ;
	Atten1 = 0.0006f ;
	Atten2 = 0.0f ;
	*/
	ChangeLightTypePoint( VGet(0, 5.0f, -5.0f), 100.0f, 0.8f, 0.001f, 0.0001f);
	// masuda	
	//ChangeLightTypeDir( VGet( 0.5f, -0.5f, 0.0f  ) ) ;	// �f�B���N�V���i�����C�g�̕���
	SetGlobalAmbientLight( GetColorF( 1.0f, 0.0f, 0.0f, 0.0f ) ) ;	// �O���[�o���A���r�G���g�J���[��ԐF�ɕύX
	CameraPosition = VGet( XORG, YORG+3.0f, -5.0f); // �J�����̈ʒu�@����5���[�g���A���_���T���[�g�����

	//---------------
	//	�^�C�g�����
	int startGr = LoadGraph("graphic\\start.png");
	int creditGr = LoadGraph("graphic\\credit.png");
	DrawGraph(0, 0, startGr, TRUE) ;
	ScreenFlip() ;
	ClearDrawScreen() ;
	//	�҂�����
	for(int i = 0; i < 150 && !CheckHitKey(KEY_INPUT_RETURN) && !ProcessMessage(); i++){	//	�G���^�[�L�[�ő҂����Ԃ��X�L�b�v�\�ɂ���
		WaitTimer(10) ;
	}

	//-------------------------
	//	�N���W�b�g��ʂɈڍs
	for(int i = 0; i < 255 && !ProcessMessage() && !ScreenFlip() 
		&&!ClearDrawScreen() ; i += 3 ){
			DrawAlpha(0, 0, i, creditGr) ;
			DrawAlpha(0, 0, 255 - i, startGr) ;
	}

	//------------------------------------------------


		//	�摜�֌W�ǂݍ���
		/*
		Player player(PLAYER_DOT_IMAGE) ;	//�v���C���[�摜�̓ǂݍ���
		PB_Straight::graphicHandle = LoadGraph(PLAYER_BULLET_GRAPHIC) ;	//	�v���C���[�̒e�O���t�B�b�N�ǂݍ���
		P_StarBom::grHandle = LoadGraph(PLAYER_BOM) ;		//	�v���C���[�̃{���O���t�B�b�N
		Player::BE_grHandle = LoadGraph(PLAYER_BOM_EFFECT) ; //	�{�����G���ɕ\�����閂���w
		E_mStraight_bStraight::GrHandle = LoadGraph(STRAIGHT_ENEMY) ; //	�G�O���t�B�b�N�ǂݍ���
		E_mStraight_bSpin::GrHandle = LoadGraph(SPIN_ENEMY) ;	//	��]�e���̓G��Ǎ�
		E_mStraight_bNway_f::GrHandle = LoadGraph(NWAY_ENEMY) ;	//	�Œ�nway�e���̓G��Ǎ�
		EB_Sinple::grHandle = LoadGraph(ENEMY_MIDLE_BULLET)	;		//	�e�O���t�B�b�N�̓ǂݍ���
		EB_Small::grHandle = LoadGraph(ENEMY_SMALL_BULLET) ;		//	�G�ė��e
		EB_Fire::grHandle = LoadGraph(ENEMY_FIRE_BULLET) ;		//	�G���e
		PB_Effect::grHandle = LoadGraph(PB_EFFECT_NAME) ;	//	���@�e�G�t�F�N�g�̓ǂݍ���
		GrazeEffect::grHandle = LoadGraph(GRAZE_EFFECT_NAME) ; // ���@�O���C�Y�G�t�F�N�g�̓ǂݍ���
		E_DeleteEffect::grHandle = LoadGraph(E_DEL_NAME) ;	//	�G���j�G�t�F�N�g�̓ǂݍ���
		
		bom_icon = LoadGraph(BOM_ICON) ;	//	�E�p�l���̎c�{���摜�Ǎ�
		HpHandle = LoadGraph(HP_GRAPH) ;	//	�{�X��HP�Q�[�W��Ǎ�
		ScoreItem::grHandle = LoadGraph(TEN_ITEM) ;	//	���_�A�C�e����Ǎ�
		BomItem::grHandle = LoadGraph(BOM_ITEM) ; // �{���A�C�e���Ǎ�
		LifeupItem::grHandle = LoadGraph(LIFEUP_ITEM) ;	//	�P�t�o�A�C�e���Ǎ�
		E_Boss_Fire::grHandle = LoadGraph(ENEMY_BOSS_FIRE) ;	//	�P�ʂ̃{�X��Ǎ�
		//	�Q�ʂ̓G
		E_mStraight_bNway::GrHandle = LoadGraph(H_NWAY_ENEMY) ;
		*/
	player_icon = LoadGraph(PLAYER_ICON) ;	//	�E�p�l���̎c�@�摜�ǂݍ���
	
	//	�h�����ʓǂݍ���
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
	//	�ԃh�����ʂ̓ǂݍ��݁i�_���[�W���肠��j
	DamageDrum::loadOnlyHr[0] = MV1LoadModel(MODEL_DIR "drum0_r.x");
	DamageDrum::loadOnlyHr[1] = MV1LoadModel(MODEL_DIR "drum2_r.x");
	//	�ۉe��ǂݍ���
	circleShadow = MV1LoadModel(MODEL_DIR "shade.x") ;
	int floorId =MV1LoadModel("floor.x"); // y=0�Ƀt���A��ݒ�(�m�F�̂���)
	//	�Ήԓǂݍ���
	LoadDivGraph("graphic\\spark.png", 10, 5, 2, 192, 142, SparkEffect::grHandle);
	//	�Z���t�G�t�F�N�g�ǂݍ���
	SerifEffect::grHandle[0] = LoadGraph("graphic\\word.png") ;
	SerifEffect::grHandle[4] = LoadGraph("graphic\\ide.png") ;

	//	���ʉ��ǂݍ���
	NormalDrum::collisionSE = LoadSoundMem("sound\\punch1.mp3") ;
	NormalDrum::boundSE = LoadSoundMem("sound\\fall1.mp3") ;
	NormalDrum::shotSE = LoadSoundMem("sound\\fly1.mp3") ;

	

	
TITLE_STATE:

	ListIterator *iterator = new ListIterator();
	//NuiImageStreamReleaseFrame( ColorImageStream, ImageFrame );
	//ImageFrame->pFrameTexture->UnlockRect(0);

	GameOver *game_over = new GameOver(GAME_OVER);
	StageClear *stage_clear = new StageClear(STAGE_CLEAR);
	int title = LoadGraph(TITLE);	//	�^�C�g����ʓǍ�
	int titleCarsor = LoadGraph(TITLE_CARSOR);	//	�J�[�\���A�C�R���̓Ǎ�
	int gameMode = 0;	//	�Q�[���̓�Փx�I���A�Q�[�����I�����邩�ǂ����B0:�m�[�}��,1:�n�[�h,2:�I��
	bool buttonDown = false;	//	�{�^���������ςȂ��̂Ƃ��̓J�[�\���𓮂����Ȃ�



	//	�����蔻�肪����葫�𐶐�
	//new RightHand();
	new AttackParts(NUI_SKELETON_POSITION_HAND_RIGHT);
	new AttackParts(NUI_SKELETON_POSITION_HAND_LEFT);
	new AttackParts(NUI_SKELETON_POSITION_FOOT_RIGHT);
	new AttackParts(NUI_SKELETON_POSITION_FOOT_LEFT);

	//	�w�i�Ƃ��𐶐�
	new Object(MODEL_DIR "right_wall.x", VGet(RIGHT_WALL_X, 3.0f, 5.0f+6.0f), VGet(0,0,0), 3.0f);
	new Object(MODEL_DIR "left_wall.x",  VGet(LEFT_WALL_X, 4.0f, 7.0f+5.0f), VGet(0,0,0), 3.0f);
	new Object(MODEL_DIR "concrete.x", VGet(0+2.2f, 0.01f, 0+14.0f), VGet(0,0,0), 3.0f);
	skyHandle = (new Object(MODEL_DIR "sky.x", VGet(0, 0, 0), VGet(0,0,0), 1.0f))->grHandle; //  20110904 masuda

	new Object(MODEL_DIR "drum1_r.x", VGet(-1.3f, 1.2f, 10.0f), VGet(0,0,0), 1.0f);
	new Object(MODEL_DIR "drum0_g.x", VGet(0, 1.2f, 9.5f), VGet(0,0,0), 1.0f);
	new Object(MODEL_DIR "drum1_y.x", VGet( 1.3f, 1.2f, 10.0f), VGet(0,0,0), 1.0f);

	new Object(MODEL_DIR "glassland.x", VGet(0, 0, 0)) ;
	//MV1DrawModel(floorId) ;	//	�t���A��`��
	
	// 20110812 masuda
	SetCameraNearFar(1.0f,100.0f); // �J��������݂āA�P���[�g������P�O�O���[�g���O�����\���̈�
	SetCameraPositionAndTargetAndUpVec( 
	// �J�����̐U���@20110823 masuda
	VAdd(CameraPosition,CameraShake), // �J�����̈ʒu�@�����ʒu�́A�����P���[�g���A���_���T���[�g�����
	VGet( XORG, YORG, ZORG ),	//�@���_���݂Ă���(������kinect�v���[�₪����͂�)
	VGet( 0.0f, 1.0f, 0.0f ) ) ; // �J�����̌X��
	
	SetCameraScreenCenter(WINDOWSIZE_X*0.5f , WINDOWSIZE_Y*0.9f); // ���_(0,0,0)�̉�ʏ�̈ʒu��������
	

	





	//	�T�E���h�֌W�ǂݍ���
	mainBGM = LoadSoundMem("sound\\BGM.mp3") ;	//	�P�ʂ�BGM
	PlaySoundMem(mainBGM, DX_PLAYTYPE_LOOP) ;
	
	RevTime = GetNowCount();	//	�O���Ԃ����������Ă���

	VECTOR skyLongtitude_d =  VGet(0.0f,-0.0001f,0.0f);

	//	���C�����[�v
	while(!ProcessMessage() && !ScreenFlip() &&!ClearDrawScreen()
		&& !CheckHitKey(KEY_INPUT_ESCAPE) )
	{
	
		//--------------------------------
		ModelRotationAdd(skyHandle, skyLongtitude_d); // 20110904 masuda


		
		if(CheckHitKey(KEY_INPUT_T)){
			iterator->sysTime->remainingTime = 0;
		}
		



		//	�ʏ�ʂ郋�[�g

		if(GameClearFlag == false && GameOverFlag == false && SkeletonTracking == true && iterator->sysTime->remainingTime > 0){
			iterator->Move();
			iterator->Draw();
		}
		//	�^�C���A�b�v���ɒʂ郋�[�g
		else if(GameClearFlag == false && GameOverFlag == false && SkeletonTracking == true && iterator->sysTime->remainingTime <= 0){
			iterator->Draw();
			iterator->Timeout();
		}
		//	�|�[�Y���ʂ郋�[�g
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
	NuiShutdown();	//	kinect�̌�n��
	DxLib_End();	//	DX���C�u�����̌�n��

	return 0;
}



/*
void LoadEnemyData(int stage){
switch(stage){
case 1:
//	�G��҂���ԃ��X�g�֓o�^
//	������E�ֈړ�����G
new E_mStraight_bStraight( 60, 0, 30, 2, 1, 2, 8);
new E_mStraight_bStraight( 70, 0, 30, 2, 1, 2, 8);
new E_mStraight_bStraight( 80, 0, 30, 2, 1, 2, 8);
new E_mStraight_bStraight( 90, 0, 30, 2, 1, 2, 8);
//	������E�ֈړ�����G
new E_mStraight_bStraight( 200, 0, 70, 2, 1, 2, 8);
new E_mStraight_bStraight( 220, 0, 70, 2, 1, 2, 8);
new E_mStraight_bStraight( 240, 0, 70, 2, 1, 2, 8);
new E_mStraight_bStraight( 260, 0, 70, 2, 1, 2, 8);
//	�E���獶�ֈړ�����G
new E_mStraight_bStraight( 360, 400, 70, 2, 180, 2, 8); 
new E_mStraight_bStraight( 380, 400, 70, 2, 180, 2, 8);
new E_mStraight_bStraight( 400, 400, 70, 2, 180, 2, 8); 
new E_mStraight_bStraight( 420, 400, 70, 2, 180, 2, 8); 
new E_mStraight_bStraight( 440, 400, 70, 2, 180, 2, 8); 
//	�E���獶�ֈړ�����G
new E_mStraight_bStraight( 540, 400, 70, 2, 180, 2, 8);
new E_mStraight_bStraight( 560, 400, 70, 2, 180, 2, 8);
new E_mStraight_bStraight( 580, 400, 70, 2, 180, 2, 8);
new E_mStraight_bStraight( 600, 400, 70, 2, 180, 2, 8);
//	�R�݂����Ȍ`�ŉ��֍~��Ă���G
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
//	�Œ�nway�e���o���Ă���G
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
//	���{�X�I�ȑ̗͂̉�]�e���̓G
new E_mStraight_bSpin	 ( 1600, 200, 0, 2, 50, 90, 2, 8); 
new E_mStraight_bSpin	 ( 2000, 300, 0, 2, 50, 90, 2, 8);
new E_mStraight_bSpin	 ( 2000, 100, 0, 2, 50, 90, 2, 8);
//	�P�ʃ{�X
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
//	�G��҂���ԃ��X�g�֓o�^
//	������E�ֈړ�����G
new E_mStraight_bStraight( 60, 0, 30, 2, 1, 6, 40);
new E_mStraight_bStraight( 70, 0, 30, 2, 1, 6, 40);
new E_mStraight_bStraight( 80, 0, 30, 2, 1, 6, 40);
new E_mStraight_bStraight( 90, 0, 30, 2, 1, 6, 40);
//	������E�ֈړ�����G
new E_mStraight_bStraight( 200, 0, 70, 2, 1, 6, 40);
new E_mStraight_bStraight( 220, 0, 70, 2, 1, 6, 40);
new E_mStraight_bStraight( 240, 0, 70, 2, 1, 6, 40);
new E_mStraight_bStraight( 260, 0, 70, 2, 1, 6, 40);
//	�E���獶�ֈړ�����G
new E_mStraight_bStraight( 360, 400, 70, 2, 180, 6, 40); 
new E_mStraight_bStraight( 380, 400, 70, 2, 180, 6, 40);
new E_mStraight_bStraight( 400, 400, 70, 2, 180, 6, 40); 
new E_mStraight_bStraight( 420, 400, 70, 2, 180, 6, 40); 
new E_mStraight_bStraight( 440, 400, 70, 2, 180, 6, 40); 
//	�E���獶�ֈړ�����G
new E_mStraight_bStraight( 540, 400, 70, 2, 180, 6, 40);
new E_mStraight_bStraight( 560, 400, 70, 2, 180, 6, 40);
new E_mStraight_bStraight( 580, 400, 70, 2, 180, 6, 40);
new E_mStraight_bStraight( 600, 400, 70, 2, 180, 6, 40);
//	�R�݂����Ȍ`�ŉ��֍~��Ă���G
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
//	�Œ�nway�e���o���Ă���G
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
//	���{�X�I�ȑ̗͂̉�]�e���̓G
new E_mStraight_bSpin	 ( 1600, 200, 0, 2, 50, 90, 4, 4); 
new E_mStraight_bSpin	 ( 2000, 300, 0, 2, 50, 90, 4, 4);
new E_mStraight_bSpin	 ( 2000, 100, 0, 2, 50, 90, 4, 4);
//	�P�ʃ{�X
new E_Boss_Fire (2700, 100, true);
new E_Boss_Fire (2700, 300, true);
break;
default:	//	��O�����B�Q�[���������I��������B
return ;
}

}

*/


