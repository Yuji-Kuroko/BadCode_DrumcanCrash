// ���ꂪ��`����Ă��邩�ɂ���āA*.h�t�@�C���̒��ŏ����R���p�C������
#define OBJCLASS

#include <math.h>

#include "stdlib.h"
#include "stdio.h"
#include "DxLib.h"

#include "def.h"
#include "function.h"
#include "objClass.h"

//	�G�l�~�[�f�[�^��ǂݍ��ށB����ɂ��錾�������Ă���
void LoadEnemyData(int stage) ;

//	�R���p�C���̌x�����o���Ȃ��悤��float�̊֐����I�[�o�[���[�h����
void DrawGraph(float x, float y, int grHandle, int TransFlag = TRUE)
{
	DrawGraph((int)x, (int)y, grHandle, TRUE);
}
void DrawRotaGraph(float x, float y, float ExRate, float Angle, int grHandle, 
	int TransFlag = TRUE, int TurnFlag = FALSE)
{
	DrawRotaGraph((int)x, (int)y, (double)ExRate, (double)Angle, grHandle, TransFlag, TurnFlag);
}


//	���S����_�Ƃ��ĕ`�悷��
void DrawGraphCenter( float x , float y ,int GraphicHundle)
{
	DrawRotaGraph( (int)x, (int)y, 1.0, 0, GraphicHundle, TRUE ) ;
}

//	�A���t�@�u�����h���g�p���ĕ`�悷��
void DrawExAlpha( float X , float Y ,int alpha, float ExRate, float angle, int grHandle)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha) ;
	DrawRotaGraph( X, Y, ExRate, angle, grHandle,TRUE) ;
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255) ;
}
//	�A���t�@�u�����h���g�p���ĕ`�悷��
void DrawAlpha( float X , float Y ,int alpha,int grHandle)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha) ;
	DrawGraph(X, Y, grHandle, TRUE) ;
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255) ;
} 
//	���Z�u�����h���g�p���ĕ`�悷��
void DrawSub( float X, float Y, int sub, int grHandle)
{
	SetDrawBlendMode( DX_BLENDMODE_SUB, sub );
	DrawGraph(X, Y, grHandle) ;
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 ) ;
}


//	�e��`�悷��Ƃ��͂X�O�x��]������
void DrawBullet(float x, float y, float ExRate, float Angle, int GrHandle)
{
	Angle += DegToRad * 90;
	DrawRotaGraph(x, y, ExRate, Angle, GrHandle, TRUE);
	return ;
}



//	���X�g�\�����g���ꍇ�Ɍp������N���X
class MoverBase{	
public:
	float x , y ;	//	���W
	virtual void Move() {} ;
	virtual void Draw() {} ;
	MoverBase(){} ;
	virtual ~MoverBase(){} ;
};


//////////////////////////////
/////�w�i�摜�N���X///////////
//////////////////////////
class BackGround{
public:
	int alpha ; //	�Q�T�S�ȉ��̏ꍇ�A�A���t�@�u�����h�`��
	float yA , yB;
	float speed ;
	int ySize ;
	int graphicHandle;
	void Move() ;
	void Draw() ;
	BackGround(char *graficName, float speed) ;

};

BackGround::BackGround(char *graficName, float in_speed){
	graphicHandle = LoadGraph(graficName) ;	//	�O���t�B�b�N�����[�h����
	int xSize; // ���̊֐��̂��߂̃_�~�[�錾
	GetGraphSize(graphicHandle, &xSize, &ySize) ;
	yA = 0 ;
	yB = -(float)ySize ;
	speed = in_speed ;
	alpha = 255;
}

void BackGround::Move(){
	yA += speed ;
	yB += speed ;
	if(yA >= ySize)
		yA -= ySize * 2;
	if(yB >= ySize)
		yB -= ySize * 2 ;
}

void BackGround::Draw(){
	if(alpha == 255){
		DrawGraph( 0, (int)yA, graphicHandle, TRUE) ;
		DrawGraph( 0, (int)yB, graphicHandle, TRUE) ;
	}else{
		DrawAlpha( 0, yA, alpha, graphicHandle) ;
		DrawAlpha( 0, yB, alpha, graphicHandle) ;
	}
	
}

/////////////////////////////
//	�Q�[���I�[�o�[�N���X/////
class GameOver : public MoverBase{
public:
	int grHandle;
	virtual void Move();
	virtual void Draw();
	GameOver(char grName[50]);
};
GameOver::GameOver(char grName[50]){
	grHandle = LoadGraph(grName);
	x = 0;
	y = -480;
}
void GameOver::Move(){
	if(y != 0)
		y += 4;
}
void GameOver::Draw(){
	DrawGraph(x, y, grHandle, TRUE);
}

//////////////////////////////////
////�X�e�[�W�N���A�N���X/////////
class StageClear : public MoverBase{
public:
	int grHandle;
	virtual void Move();
	virtual void Draw();
	StageClear(char grName[50]);
};
StageClear::StageClear(char grName[50]){
	grHandle = LoadGraph(grName);
	x = 0;
	y = -480;
}
void StageClear::Move(){
	if(y != 0)
		y += 5;
}
void StageClear::Draw(){
	DrawGraph(x, y, grHandle, TRUE);
}


///////////////////////////
///�G�t�F�N�g�N���X////////
/////////////////////////
//	�G�t�F�N�g�̊��N���X�i�K���p���j
class EffectBase : public MoverBase{
public:
	static EffectBase *first_p ;
	bool alive  ;	//	�����t���O
	float angle ;	//	�G�t�F�N�g�̊p�x�i���x�񂵂ĕ`�悷��j
	float sizeRate;	//	���݂̃G�t�F�N�g�̑傫���B�P�ɂȂ�����delete
	EffectBase *next_p , *rev_p ;	//	���X�g�\���ɂ��邽�߁A�O��̃|�C���^
	EffectBase() ;		//	���X�g�̐ڑ�
	EffectBase(bool initFlg) ;		//	���X�g�̍ŏ��ł̂ݎg�p�i�_�~�[���X�g�j
	~EffectBase() ;	//	�O�ナ�X�g�̐ڑ�
	virtual void Move(){} ;
	virtual void Draw(){} ;
};
//	�ÓI�����o�ϐ��̏�����
EffectBase *EffectBase::first_p = NULL ;

//	���X�g�̐擪���L��
EffectBase::EffectBase(){
	next_p = first_p ;
	rev_p  = first_p->rev_p ;
	first_p->rev_p->next_p = this ; 
	first_p->rev_p = this ;
	alive = true ;
	sizeRate = 0 ;
}
EffectBase::EffectBase(bool startInitFlg ){
	first_p = this ;
	next_p  = this ;
	rev_p   = this ;
	alive   = true ;
}
EffectBase::~EffectBase(){
	next_p->rev_p = rev_p ;
	rev_p->next_p = next_p	;	//	�O�ナ�X�g���O��Ȃ��悤�ɏ���	
}

//	���@�e�̒��e�G�t�F�N�g
class PB_Effect : public EffectBase{
public:
	static int grHandle ;
	PB_Effect(int in_x, int in_y) ;
	virtual void Move() ;
	virtual void Draw() ;
};
//	�ÓI�ϐ�������
int PB_Effect::grHandle = NULL ;
PB_Effect::PB_Effect(int in_x, int in_y){
	x = in_x;
	y = in_y;
}
void PB_Effect::Move(){
	sizeRate += EFFECT_SPEED_RATE ;
	if(sizeRate > 1)
		alive = false ;
}
void PB_Effect::Draw(){
	DrawRotaGraph(x, y, sizeRate, 0, grHandle, TRUE) ;
}

//	�G���j�G�t�F�N�g
class E_DeleteEffect : public EffectBase{
public:
	static int grHandle ;
	E_DeleteEffect(float in_x, float in_y );
	virtual void Move() ;
	virtual void Draw() ;
};
int E_DeleteEffect::grHandle = NULL ;
E_DeleteEffect::E_DeleteEffect(float in_x, float in_y){
	x = (int)in_x ;
	y = (int)in_y ;
	angle = GetRand(359) * DegToRad ;
	PlaySoundMem(ED_sound, DX_PLAYTYPE_BACK) ;	//	�G���j�����o��
}
void E_DeleteEffect::Move(){
	sizeRate += 0.05 ;
	if(sizeRate > 1)
		alive = false ;
}
void E_DeleteEffect::Draw(){
	DrawRotaGraph(x, y, sizeRate, angle, grHandle, TRUE) ;
}

//	���@�O���C�Y�̃G�t�F�N�g
class GrazeEffect : public EffectBase{
public:
	static int grHandle ;
	GrazeEffect(int in_X, int in_y);
	virtual void Move() ;
	virtual void Draw() ;
	float moveX ;
	float moveY ;
	int cnt ;
};
int GrazeEffect::grHandle = NULL;
GrazeEffect::GrazeEffect(int in_x, int in_y){
	x = in_x ;
	y = in_y ;
	angle = GetRand(359) * DegToRad ;
	GetVector(3, angle, &moveX, &moveY); 
	cnt = 0;
}
void GrazeEffect::Move(){
	x += moveX * abs(cnt - 45) / 45 ;
	y += moveY * abs(cnt - 45) / 45 ;
	cnt++;
	//	cnt��0�ɂȂ�����폜����
	if(cnt == 45)
		alive = false;
}
void GrazeEffect::Draw(){
	DrawRotaGraph(x, y, 1.0f, angle, grHandle, TRUE);
}


////////////////////////////
///�v���C���[�e�̃N���X/////
////////////////////////////


//	�v���C���[�e�̊��N���X
class PB_Base : public MoverBase{
public:
	static PB_Base *first_p ;
	float moveX , moveY ;
	
	int skeletonPos ;	//	�X�P���g�����ʏ��
	VECTOR centerV ;	//	�̂̒��S����ۑ�
	VECTOR revV ; //	�O�ɂƂ����X�P���g������ۑ�
	VECTOR nowV ; //	���݂̃X�P���g������ۑ�
	float posX, posY ;	//	�X�N���[�����W

	float speed ;	//	�X�s�[�h
	float angle ;	//	�p�x
	float size  ;	//	�T�C�Y
	bool alive  ;	//	�����t���O
	float power ;	//	�e�̃_���[�W��
	PB_Base *next_p , *rev_p ;	//	���X�g�\���ɂ��邽�߁A�O��̃|�C���^
	PB_Base() ;		//	���X�g�̐ڑ�
	PB_Base(bool initFlg) ;		//	���X�g�̍ŏ��ł̂ݎg�p�i�_�~�[���X�g�j
	~PB_Base() ;	//	�O�ナ�X�g�̐ڑ�
	virtual void Move(){} ;
	virtual void Draw(){} ;
};

//	�ÓI�����o�ϐ��̏�����
PB_Base *PB_Base::first_p = NULL ;


//	���X�g�̐擪���L��
PB_Base::PB_Base(){
	next_p = first_p->next_p ;
	rev_p  = first_p ;
	first_p->next_p->rev_p = this ; 
	first_p->next_p = this ;
	alive = true ;
	//PlaySoundMem(PB_sound, DX_PLAYTYPE_BACK) ;	//	���@�e���ˉ����o��
}

PB_Base::PB_Base(bool startInitFlg ){
	first_p = this ;
	next_p  = this ;
	rev_p   = this ;
	alive   = true ;
}

PB_Base::~PB_Base(){
	next_p->rev_p = rev_p ;
	rev_p->next_p = next_p	;	//	�O�ナ�X�g���O��Ȃ��悤�ɏ���	
}






//-----------------------------
//	�v���C���[�̒����e
class PB_Straight : public PB_Base {
public:
	static int graphicHandle ;
	PB_Straight(float posX , float posY, float move, float angle , float Power) ;	
	void Move() ;
	void Draw() ;
};

int PB_Straight::graphicHandle = NULL ;

void PB_Straight::Move(){
	x += moveX ;
	y += moveY ;
	if	(x < BULLET_LEFT_LIMIT || x > BULLET_RIGHT_LIMIT ||
		y < BULLET_UP_LIMIT	 || y > BULLET_DOWN_LIMIT)
		alive = false;
}

PB_Straight::PB_Straight(float posX, float posY , float pspeed ,float RadAngle , float Power){
	x = posX ;
	y = posY ;
	angle = RadAngle ;
	size = 5 ;
	alive = true ;
	power = Power;
	GetVector( pspeed, RadAngle, &moveX , &moveY );	//	�p�x�ƈړ����x����X��Y�������v�Z���đ��
}

//	�`�悷��
void PB_Straight::Draw(){
	DrawBullet(x , y , 1.0 , angle, graphicHandle) ;
}	



//-------------------------------
////�v���C���[�̃{���e�N���X
//-------------------------------
//	�v���C���[�{���̊��N���X�i�K���p���j
class PlayerBomBase : public MoverBase{
public:
	static PlayerBomBase *first_p ;
	bool alive  ;	//	�����t���O
	float angle ;	//	�G�t�F�N�g�̊p�x�i���x�񂵂ĕ`�悷��j
	float size ;	//	�e�̃T�C�Y
	float moveX , moveY;	//	X,Y���̈ړ����x
	float power ;	//	�P�t���[��������̃_���[�W��
	PlayerBomBase *next_p , *rev_p ;	//	���X�g�\���ɂ��邽�߁A�O��̃|�C���^
	PlayerBomBase() ;		//	���X�g�̐ڑ�
	PlayerBomBase(bool initFlg) ;		//	���X�g�̍ŏ��ł̂ݎg�p�i�_�~�[���X�g�j
	~PlayerBomBase() ;	//	�O�ナ�X�g�̐ڑ�
	virtual void Move(){} ;
	virtual void Draw(){} ;
};
//	�ÓI�����o�ϐ��̏�����
PlayerBomBase *PlayerBomBase::first_p = NULL ;
//	���X�g�̐擪���L��
PlayerBomBase::PlayerBomBase(){
	next_p = first_p->next_p ;
	rev_p  = first_p ;
	first_p->next_p->rev_p = this ; 
	first_p->next_p = this ;
	alive = true ;
	angle = 0 ;
	size = BOM_SIZE ;
	power = BOM_POWER ;
}
PlayerBomBase::PlayerBomBase(bool startInitFlg ){
	first_p = this ;
	next_p  = this ;
	rev_p   = this ;
	alive   = true ;
}
PlayerBomBase::~PlayerBomBase(){
	next_p->rev_p = rev_p ;
	rev_p->next_p = next_p	;	//	�O�ナ�X�g���O��Ȃ��悤�ɏ���	
}

//	���`�{��
class P_StarBom : public PlayerBomBase{
public:
	static int grHandle ;
	P_StarBom(float in_x, float in_y, float degAngle, float in_speed);
	virtual void Move(); 
	virtual void Draw();
};
int P_StarBom::grHandle = NULL ;
//	�R���X�g���N�^
P_StarBom::P_StarBom(float in_x, float in_y, float degAngle, float in_speed){
	x = in_x;
	y = in_y;
	GetVector(in_speed, degAngle * DegToRad, &moveX, &moveY);
}
void P_StarBom::Move(){
	x += moveX ;
	y += moveY ;
	angle += 7 * DegToRad;	//	�P�t���[����7�x��]������
	if	(x < BULLET_LEFT_LIMIT || x > BULLET_RIGHT_LIMIT ||
		y < BULLET_UP_LIMIT	 || y > BULLET_DOWN_LIMIT)
		alive = false;
}
void P_StarBom::Draw(){
	DrawRotaGraph(x, y, 1.0f, angle, grHandle, TRUE) ; 
}


/////////////////////////////
//////�v���C���[�N���X////////
/////////////////////////////
class Player : public MoverBase {
private:
	int graphicHandle[2][3];	//	���@�̉摜[shotFlag][tiltFlag]
	int slowHandle ;	//	�����蔻��̖ڈ��̉摜
	int shotFlag ;
	int tiltFlag ;
public:
	static int BE_grHandle;	//	�{�����G���ɏo���G�t�F�N�g
	static Player *p ;	//	���N���X�Ńv���C���[���Q�Ƃ���Ƃ��Ɏg�p
	int deathHandle;	//	�ė���
	int bomSound;		//	�{���̉�
	int noHitTime ;		//	���G����
	int flashTime ;		//	�_�Ŏ��ԁB���@����e�������ƂɎg�p�B
	bool alive ;
	int hitAliveTime ;	//	��炢�{����t����
	float slowSizeRate ;//	�X���E���ɕ\�������摜�̃T�C�Y�𐧌䂷��̂Ɏg�p
	float hitSize ;		//	�����蔻��̑傫��
	float grazeSize;	//	�O���C�Y����̑傫��
	int stock ;			//	�c�@��
	int bom ;			//	�c��{����
	int power;			//	�p���[
	int interval ;		//	�V���b�g�̊Ԋu
	int bomEffect;		//	�{���̖��G���Ԃ̖ڈ������������w
	float BE_spin ;		//	�����w���񂷂��߂̐���ϐ�
	Player(char graphicFileName[]);
	Player(){};
	void virtual Draw();
	void virtual Move();
	void Shot();
};
//	�ÓI�����o�ϐ��̏�����
Player *Player::p = NULL ;
int Player::BE_grHandle = NULL;
//	�v���C���[�N���X�̃R���X�g���N�^
Player::Player(char graphicFileName[30]){
	graphicHandle[0][1] = LoadGraph(graphicFileName);	//	�v���C���[�O���t�B�b�N�ǂݍ���
	graphicHandle[0][2] = LoadGraph(PLAYER_RMOVE);
	graphicHandle[0][0] = LoadGraph(PLAYER_LMOVE);
	graphicHandle[1][1] = LoadGraph(PLAYER_SHOT);
	graphicHandle[1][2] = LoadGraph(PLAYER_SHOT_RMOVE);
	graphicHandle[1][0] = LoadGraph(PLAYER_SHOT_LMOVE);
	slowHandle = LoadGraph(P_SLOW_GRAPHIC) ;	//	�����蔻��̖ڈ��̓ǂݍ���
	deathHandle = LoadSoundMem(PLAYER_DEATH) ;		//	���@�ė����̓ǂݍ���
	bomSound = LoadSoundMem(PLAYER_BOM_SOUND) ;	//	�{�����ˉ�
	p = this ;
	x = 340;
	y = 400;
	noHitTime = 0 ;
	alive = true ;
	stock = 3;
	bom = 2;
	power = 0;
	hitSize = PLAYER_SIZE;
	grazeSize = PLAYER_GRAZE_SIZE;
	hitAliveTime = 0;
	bomEffect = 0;
	shotFlag = 0;
	tiltFlag = 1;
	flashTime = 0;
}

//	�v���C���[�̕`��
void Player::Draw(){
	if(bomEffect > 0){
		BE_spin += DegToRad * 7;
		bomEffect--;
		DrawRotaGraph(x, y, (float)bomEffect/300.0f, BE_spin, BE_grHandle, TRUE) ;
	}
	if((flashTime % 10) < 5)	//	���@���ė��������Ƃ̕��A�G�t�F�N�g
		DrawGraphCenter(x, y, graphicHandle[shotFlag][tiltFlag]);	//	���@��`��
	if(flashTime > 0)
		flashTime--;
	if(CheckHitKey(KEY_INPUT_RSHIFT) || CheckHitKey(KEY_INPUT_LSHIFT)){
		if(slowSizeRate < 1 )
			slowSizeRate += 0.05;	//	�X���E�ŕ\�������摜�����X�ɑ傫������B
		DrawRotaGraph(x, y, slowSizeRate, 0, slowHandle, TRUE) ;
	}
	else
		slowSizeRate = 0;
	tiltFlag = 1;
	shotFlag = 0;
	return ;
}

void Player::Shot(){
	interval = (interval + 1) % 5 ;
	if(CheckHitKey(KEY_INPUT_Z))
		shotFlag = 1;	//	�`�悷��Ƃ��̓V���b�g��Ԃ̉摜
	if(CheckHitKey(KEY_INPUT_Z) && interval == 0 && (CheckHitKey(KEY_INPUT_RSHIFT) || CheckHitKey(KEY_INPUT_LSHIFT))){
		new PB_Straight( x + 8, y, P_SHOT_MOVE_SPEED, DegToRad * 270, 5 );
		new PB_Straight( x - 8, y, P_SHOT_MOVE_SPEED, DegToRad * 270, 5 );
		new PB_Straight( x + 20, y, P_SHOT_MOVE_SPEED, DegToRad * 270, 5 );
		new PB_Straight( x - 20, y, P_SHOT_MOVE_SPEED, DegToRad * 270, 5 );
	}
	else if(CheckHitKey(KEY_INPUT_Z) && interval == 0){
		new PB_Straight( x, y, P_SHOT_MOVE_SPEED, DegToRad * 270, 4 );
		new PB_Straight( x, y, P_SHOT_MOVE_SPEED, DegToRad * 260, 4 );
		new PB_Straight( x, y, P_SHOT_MOVE_SPEED, DegToRad * 250, 4 );
		new PB_Straight( x, y, P_SHOT_MOVE_SPEED, DegToRad * 280, 4 );
		new PB_Straight( x, y, P_SHOT_MOVE_SPEED, DegToRad * 290, 4 );
	}
	if (CheckHitKey(KEY_INPUT_X) && noHitTime < 30 && bom > 0){
		for(int i = 0; i < 8; i++)
			new P_StarBom( x, y,  45 * i, 1.5) ;	//	�W�����ɐ��^�{���𔭎�
		new P_StarBom(x, y, 270 - 22, 1.5) ;	//	�O���Q�����ɐ��^�{������
		new P_StarBom(x, y, 270 + 22, 1.5) ;	//	�O�������Z�����^�{������
		noHitTime = 300 ;	//	�T�b�Ԗ��G
		bomEffect = 300 ;
		BE_spin = 0;
		PlaySoundMem(bomSound, DX_PLAYTYPE_BACK) ;
		bom--;
	}
	return ;
}



//	�v���C���[�̈ړ�
void Player::Move(){
	int shiftKey ; 
	shiftKey = CheckHitKey(KEY_INPUT_LSHIFT) || CheckHitKey(KEY_INPUT_RSHIFT) ;
	float tilt = 1 ;
	//�΂߂Ɉړ����悤�Ƃ��Ă�����A�ړ��͂���Q�ɂ���
	if((CheckHitKey(KEY_INPUT_UP) && CheckHitKey(KEY_INPUT_RIGHT))  ||
	   (CheckHitKey(KEY_INPUT_RIGHT) && CheckHitKey(KEY_INPUT_DOWN)) ||
	   (CheckHitKey(KEY_INPUT_DOWN) && CheckHitKey(KEY_INPUT_LEFT))  ||
	   (CheckHitKey(KEY_INPUT_LEFT) && CheckHitKey(KEY_INPUT_UP)))
		tilt = ROOT2 ;
	//	���G���Ԃ����炷
	if(noHitTime > 0)
		noHitTime--;
	 //	�v���C���[�̈ړ�����
	if(CheckHitKey(KEY_INPUT_UP))	//��ړ�
	{
		y -= (PLAYER_MOVE - (PLAYER_MOVE * shiftKey) + (shiftKey * PLAYER_SLOW_MOVE)) / tilt ;
		if(y < UP_LIMIT){
			y = UP_LIMIT ;
		}
	}
	if(CheckHitKey(KEY_INPUT_DOWN))	//���ړ�
	{
		y += (PLAYER_MOVE - (PLAYER_MOVE * shiftKey) + (shiftKey * PLAYER_SLOW_MOVE)) / tilt ;
		if(y > DOWN_LIMIT){
			y = DOWN_LIMIT ;
		}
	}
	if(CheckHitKey(KEY_INPUT_RIGHT))	//�E�ړ�
	{
		tiltFlag = 2;	//	�E�ɌX�������@��\��
		x += (PLAYER_MOVE - (PLAYER_MOVE * shiftKey) + (shiftKey * PLAYER_SLOW_MOVE)) / tilt ;
		if(x > RIGHT_LIMIT){
			x = RIGHT_LIMIT ;
		}
	}
	if(CheckHitKey(KEY_INPUT_LEFT))	//���ړ�
	{
		tiltFlag = 0;	//	���ɌX�������@��\��
		x -= (PLAYER_MOVE - (PLAYER_MOVE * shiftKey) + (shiftKey * PLAYER_SLOW_MOVE)) / tilt ;
		if(x < LEFT_LIMIT) {
			x = LEFT_LIMIT ;
		}
	} 
	Shot() ;	//	���@�e�𔭎˂���
	PlayerX = x;	//	���N���X����Q�Ƃ��邽�߂ɃO���[�o���ϐ��ɃR�s�[
	PlayerY = y;
	return  ;
}

//	��e�����Ƃ��ɕ\�����鎩�@

class CrashPlayer : public Player {
public:
	int grHandle ;
	float spin ;
	float spiningSize ;
	virtual void Move();
	virtual void Draw();
	CrashPlayer(char grName[]);
};

CrashPlayer::CrashPlayer(char grName[30]){
	grHandle = LoadGraph(grName) ;
	spin = 0;
	spiningSize = 1;
}
void CrashPlayer::Move(){
	spiningSize -= 0.02;
	spin = spin - 10 * DegToRad ; 
	if(spiningSize <= 0){
		spiningSize = 1;
		if(p->stock == 0){	//	�c�@���O��������Q�[���I�[�o�[
			spiningSize = 1;
			GameOverFlag = true ;
		}
		else{				//	�c�@���������珉��������
			p->stock -= 1;
			p->alive = true ;
			p->bom = 2;
			p->power = 0;
			p->noHitTime = 180;	//	�R�b�Ԗ��G
			p->flashTime = 180;	//	�R�b�ԓ_��
		}
	}
}
void CrashPlayer::Draw(){
	DrawRotaGraph(p->x, p->y, spiningSize, spin, grHandle, TRUE);
}

//////////////////////////////////////
/////�A�C�e���N���X/////////////////
/////////////////////////////////////
//	�A�C�e���̊��N���X�i�K���p���j
class ItemBase : public MoverBase{
public:
	static ItemBase *first_p ;
	bool trackingFlag ;	//	true�̂Ƃ��́A���@�ɏW�܂�
	bool alive ;	//	�����t���O
	int upCnt ;		//�@�o������͏㏸����B���̏㏸���Ԃ𐔂���B
	float angle ;	//	�i�s����
	float moveX, moveY ;	//	�㏸���ɐi��X����Y����	
	float size ;	//	�����蔻��̑傫��
	ItemBase *next_p , *rev_p ;	//	���X�g�\���ɂ��邽�߂̑O��̃|�C���^
	ItemBase() ;	//	���X�g�̐ڑ�
	ItemBase(bool initFlag) ; //	���X�g�̐擪������i�_�~�[���X�g�j
	ItemBase(int in_x, int in_y);
	int score;	//	�X�R�A�A�C�e���ł̂ݎg�p
	~ItemBase() ;	//	�O�ナ�X�g�̐ڑ�
	virtual void Move() ;
	virtual void Draw(){} ;
	virtual void Get(){} ;	//	�A�C�e�����擾�������̏���
};
//	�ÓI�����o�ϐ��̏�����
ItemBase *ItemBase::first_p = NULL ;

ItemBase::ItemBase(){
	next_p = first_p->next_p ;
	rev_p  = first_p ;
	first_p->next_p->rev_p = this ; 
	first_p->next_p = this ;
	alive = true ;
	size = ITEM_SIZE ;
	trackingFlag = false ;
}
//	���X�g�̐擪�i�_�~�[�j
ItemBase::ItemBase(bool startInitFlg ){
	first_p = this ;
	next_p  = this ;
	rev_p   = this ;
	alive   = true ;
}
//	��{�I�Ɍp����ł͂��̃R���X�g���N�^���g��
ItemBase::ItemBase(int in_x, int in_y){
	next_p = first_p->next_p ;
	rev_p  = first_p ;
	first_p->next_p->rev_p = this ; 
	first_p->next_p = this ;
	alive = true ;
	size = ITEM_SIZE ;
	trackingFlag = false ;
	x = in_x;
	y = in_y;
	upCnt = 0;
	angle = (GetRand(170) + 190) * DegToRad;//	190�x����350�x��	
	GetVector(5, angle, &moveX, &moveY);	//	�㏸���̌����ݒ�
}
ItemBase::~ItemBase(){
	next_p->rev_p = rev_p ;
	rev_p->next_p = next_p	;	//	�O�ナ�X�g���O��Ȃ��悤�ɏ���	
}
void ItemBase::Move(){
	//	��ʉ��𒴂�����폜
	if(DOWN_LIMIT + 50 < y)
		alive = false;
	if(trackingFlag){
		float vX, vY;
		angle =	GetAngle(x, y, PlayerX, PlayerY);
		GetVector(5, angle, &vX, &vY);
		x += vX;
		y += vY;
	}
	else{
		upCnt++;
		if(upCnt < 10){	//	�㏸���̓���
			x += moveX ; 
			y += moveY ;
		}else{	//	�㏸���I�����A���~���̓���
			y += 2 ;
		}
		//	�v���C���[����ɂ��邩�A�v���C���[�ɋ߂Â�����A�C�e�����v���C���[��ǔ�����i�A�C�e���N�W�j
		if(PlayerY < 200 || isHit(PlayerX, PlayerY, PLAYER_SIZE, x, y, size * 3)){
			trackingFlag = true;	
		}
	}
}


//////////////////////////////////////
//	�X�R�A�A�C�e��//////////////////
//	�X�R�A�A�C�e���̃x�[�X�i�p�����āj
class ScoreItem	: public ItemBase{
public:
	static int grHandle   ;
	virtual void Draw() ;
	virtual void Get();
	ScoreItem(int in_x, int in_y, int in_score);
};
int ScoreItem::grHandle = NULL;
ScoreItem::ScoreItem(int in_x, int in_y, int in_score) : ItemBase(in_x, in_y){
	score = in_score;
}
void ScoreItem::Get(){
	Score += score;		//	�Q�[�����̃X�R�A�ɉ��Z
}
void ScoreItem::Draw(){
	DrawGraphCenter(x, y, grHandle);
}
/////////////////////////////////////
////////�{���A�C�e��/////////////////
class BomItem : public ItemBase{
public:
	static int grHandle ;
	virtual void Draw() ;
	virtual void Get() ;
	BomItem(int in_x, int in_y) : ItemBase(in_x, in_y){} ;
};
int BomItem::grHandle = NULL ;
void BomItem::Draw(){
	DrawGraphCenter(x, y, grHandle) ;
}
void BomItem::Get(){
	Player::p->bom++ ;
}
/////////////////////////////////////
////////�P�t�o�A�C�e��///////////////
class LifeupItem : public ItemBase{
public:
	static int grHandle ;
	virtual void Draw() ;
	virtual void Get() ;
	LifeupItem(int in_x, int in_y) : ItemBase(in_x, in_y){} ;
};
int LifeupItem::grHandle = NULL ;
void LifeupItem::Draw(){
	DrawGraphCenter(x, y, grHandle);
}
void LifeupItem::Get(){
	Player::p->stock++;
}


///////////////////////////////////
////�G�e�N���X///////////////////
//////////////////////////////

//	�G�e�̊��N���X
class EnemyBulletBase{
public:
	static EnemyBulletBase *first_p ;
	float x , y ;	//	���W
	float moveX, moveY ;
	float speed ;	//	�e�̃X�s�[�h�@
	float angle ;	//	�p�x
	float size ;	//	�e�̑傫��
	bool alive ;
	bool grazed ;	//	�O���C�Y�t���O�Atrue�̂Ƃ��̓O���C�Y���Ȃ��B
	EnemyBulletBase *next_p , *rev_p ;	//	���X�g�\���ɂ��邽�߁A�O��̃|�C���^
	EnemyBulletBase() ;
	EnemyBulletBase(bool initFlg) ;
	~EnemyBulletBase() ;
	virtual void Move(){};
	virtual void Draw(){};
};

//	�ÓI�����o�ϐ��̏�����
EnemyBulletBase *EnemyBulletBase::first_p = NULL ;
//	�G�e���X�g�ɓo�^
EnemyBulletBase::EnemyBulletBase(){
	next_p = first_p->next_p ;
	rev_p  = first_p ;
	first_p->next_p->rev_p = this ; 
	first_p->next_p = this ;
	alive = true ;
	PlaySoundMem(EB_sound, DX_PLAYTYPE_BACK) ;	//	�G�e�̔��ˉ����o��	
	grazed = false;
}
//	�G�e���X�g�̐擪���L��
EnemyBulletBase::EnemyBulletBase(bool initFlg){
	first_p = this ;
	rev_p = this ;
	next_p = this ;
}
EnemyBulletBase::~EnemyBulletBase(){
	rev_p->next_p = next_p	;	//	�O�ナ�X�g���O��Ȃ��悤�ɏ���	
	next_p->rev_p = rev_p	;	//
}



////////////////////////////
//	�����A�T�C�Y���̒e	/////
class EB_Sinple : EnemyBulletBase{
public:
	static int grHandle ;
	virtual void Move() ;
	virtual void Draw() ;
	EB_Sinple(float in_x, float in_y, float in_moveX, float in_moveY) ;
};
int EB_Sinple::grHandle = NULL ;

EB_Sinple::EB_Sinple(float in_x, float in_y, float in_moveX, float in_moveY){
	x = in_x ;
	y = in_y ;
	moveX = in_moveX ;
	moveY = in_moveY ;
	size = EB_SINPLE_SIZE ;
}

void EB_Sinple::Move(){
	x += moveX;
	y += moveY;
	if	(x < BULLET_LEFT_LIMIT || x > BULLET_RIGHT_LIMIT ||
		y < BULLET_UP_LIMIT	 || y > BULLET_DOWN_LIMIT)
		alive = false;
}

void EB_Sinple::Draw(){
	DrawBullet(x, y, 1.0, angle, grHandle) ;
}
////////////////////////////
//	�����A�T�C�Y���̕ė��e	/////
class EB_Small : EnemyBulletBase{
public:
	static int grHandle ;
	virtual void Move() ;
	virtual void Draw() ;
	EB_Small(float in_x, float in_y, float in_moveX, float in_moveY) ;
};
int EB_Small::grHandle = NULL ;

EB_Small::EB_Small(float in_x, float in_y, float in_moveX, float in_moveY){
	x = in_x ;
	y = in_y ;
	moveX = in_moveX ;
	moveY = in_moveY ;
	angle = GetAngle(x, y, x + moveX, y+moveY);
	size = EB_SMALL_SIZE ;
}

void EB_Small::Move(){
	x += moveX;
	y += moveY;
	if	(x < BULLET_LEFT_LIMIT || x > BULLET_RIGHT_LIMIT ||
		y < BULLET_UP_LIMIT	 || y > BULLET_DOWN_LIMIT)
		alive = false;
}

void EB_Small::Draw(){
	DrawBullet(x, y, 1.0, angle, grHandle) ;
}

///////////////////////
//�΂̋ʁi��ʏ㕔�ɓ��B����Ɛ����ɗ�����j
class EB_Fire : public EnemyBulletBase{
public:
	static int grHandle ;
	virtual void Move() ;
	virtual void Draw() ;
	EB_Fire(float in_x, float in_y, float in_moveX, float in_moveY) ;
};
int EB_Fire::grHandle = NULL;
EB_Fire::EB_Fire(float in_x, float in_y, float in_moveX, float in_moveY){
	x = in_x ;
	y = in_y ;
	moveX = in_moveX ;
	moveY = in_moveY ;
	size = EB_FIRE_SIZE ;
}
void EB_Fire::Move(){
	x += moveX;
	y += moveY;
	if	(x < BULLET_LEFT_LIMIT || x > BULLET_RIGHT_LIMIT ||
		y < BULLET_UP_LIMIT	 || y > BULLET_DOWN_LIMIT)
		alive = false;
	if( y < 5){
		moveX = 0;
		moveY = (GetRand(40)+1) / 10;
	}
}
void EB_Fire::Draw(){
	DrawRotaGraph(x, y, 1.0f, 0, grHandle, TRUE);
}

class EB_Fire_P : EB_Fire{
	//	�p�����N���X�̃R���X�g���N�^���g��
public:
	int moveCnt;	//	�񎩋@�_���ňړ���������
	EB_Fire_P(float in_x, float in_y, float in_moveX, float in_moveY) :
		EB_Fire(in_x, in_y, in_moveX, in_moveY){moveCnt = 0; size = EB_FIRE_SIZE/3*2;}
	virtual void Move();
	virtual void Draw();
};
void EB_Fire_P::Move(){
	int MOVE_TIME = 120;	//	�Œ�p�x�Ŕ��ł��鎞��
	moveCnt++;	//	�Œ�p�x�œ����Ă鎞�Ԃ��J�E���g
	if(moveCnt < MOVE_TIME){	//	�������Ă����āA�~�܂����玩�@�_���e�ɕύX
		x += moveX * abs(moveCnt-MOVE_TIME) / MOVE_TIME ;
		y += moveY * abs(moveCnt-MOVE_TIME) / MOVE_TIME ;
		if	(x < BULLET_LEFT_LIMIT-300 || x > BULLET_RIGHT_LIMIT+300 ||
			y < BULLET_UP_LIMIT-400 || y > BULLET_DOWN_LIMIT)
			alive = false;
	}else if(moveCnt == MOVE_TIME){	//	���@�_���e�ɐݒ肷��
		float range = sqrt(moveX*moveX+moveY*moveY);
		float pAngle = GetAngle(x, y, Player::p->x, Player::p->y);	//	���@�ւ̊p�x���擾
		GetVector(range, pAngle, &moveX, &moveY);
	}else{	//	���@�_���e��Ԃ̒e�ړ�
		x += moveX;
		y += moveY;
	}
}
void EB_Fire_P::Draw(){
	DrawRotaGraph(x, y, (float)(1.0/3.0*2.0), 0, grHandle, TRUE);

}

/////////////////////////////////////////
/////////�G�N���X//////////////////////
////////////////////////////////////////
//	�G�̊��N���X
class EnemyBase : public MoverBase{
public:
	static EnemyBase *activeFirst_p ;	//	�A�N�e�B�u���X�g�̃_�~�[�ւ̃|�C���^
	static EnemyBase *waitFirst_p ;		//	�҂���ԃ��X�g�̃_�~�[�ւ̃|�C���^
	EnemyBase *next_p , *rev_p ;	//	���X�g�\���ɂ��邽�߁A�O��̃|�C���^
	
	//	�ǉ�
	int grHandle[4] ;
	VECTOR MV ;	//	�ړ���
	bool hitFlag ;
	VECTOR initPos;	// �����ʒu
	VECTOR initVelocity;	// �����x
	float realTimer ;	//	�b���Z�̃^�C�}�[	�~���b�܂Ő��m�B
	int damage;	//	�_���[�W�ʁB�P�ȏ�ŐG��ƃ_���[�W����B
	int boundNum ;	//	�o�E���h�̉�
	int state ;		//	�h�����ʂ̉��ݏ�ԁi�O�������A�S�������݁j

	int score ;	//	���Ƃ��X�R�A�̓��_
	int scoreNum ;	//	���Ƃ��X�R�A�̐�
	int bomNum ;	//	���Ƃ��{���̐�
	int lifeupNum ;	//	���Ƃ��P�t�o�̐�

	float moveX , moveY ;	//	X��Y�̈ړ�����
	float speed ;	//	�X�s�[�h
	float angle ;	//	�p�x
	float size  ;	//	�T�C�Y
	float hp ;		//	�̗�
	int	  interval ;//	�V���b�g�����Ԋu
	int	  nowInterval;	//	�V���b�g�Ԋu�𐧌�
	float bulletSpeed ; //	�e�̑���
	unsigned long long waitTime ;	//	�o���҂�����
	bool alive  ;	//	�����t���O

	EnemyBase() ;		//	���X�g�̐ڑ�
	EnemyBase(bool initFlg) ;		//	���X�g�̍ŏ��ł̂ݎg�p�i�_�~�[���X�g�j
	virtual ~EnemyBase() ;	//	�O�ナ�X�g�̐ڑ�
	virtual void Move(){} ;
	virtual void Draw(){} ;
	virtual void DamageEffect(){} ;
	virtual void PlayShotSE(){} ;
	virtual void PlayCollisionSE(){} ;
	void Activation() ;		//	�҂���ԃ��X�g����A�N�e�B�u���X�g�ֈړ�
};
//	�ÓI�����o�ϐ��̏�����
EnemyBase *EnemyBase::activeFirst_p = NULL ;
EnemyBase *EnemyBase::waitFirst_p = NULL ;

//	�҂���ԃ��X�g�̍Ō�ɒǉ�
EnemyBase::EnemyBase(){
	hitFlag = false ;
	next_p = waitFirst_p ;
	rev_p  = waitFirst_p->rev_p ;
	waitFirst_p->rev_p->next_p = this ; 
	waitFirst_p->rev_p = this ;
	alive = true ;
	score = 0 ;
	scoreNum = 0 ;
	bomNum = 0 ;
	lifeupNum = 0 ;
	damage = 0;
	boundNum = 0;
	state = 0;
}
//	activeInit��true�Ȃ�A�N�e�B�u���X�g�̃_�~�[���쐬
//	false�Ȃ�҂���ԃ��X�g�̃_�~�[���쐬
EnemyBase::EnemyBase(bool activeInit ){
	if(activeInit){
	activeFirst_p = this ;
	next_p  = this ;
	rev_p   = this ;
	alive   = true ;
	}
	else{
	waitFirst_p = this ;
	next_p  = this ;
	rev_p   = this ;
	alive   = true ;
	}
}
EnemyBase::~EnemyBase(){
	next_p->rev_p = rev_p ;
	rev_p->next_p = next_p	;	//	�O�ナ�X�g���O��Ȃ��悤�ɏ���	
	/*
	//	�v���C���[���G��|������A�C�e�����o��
	if(alive == false){
		for(int i = 0; i < scoreNum; i++)
			new ScoreItem((int)x, (int)y, score);
		for(int i = 0; i < bomNum; i++)
			new BomItem((int)x, (int)y);
		for(int i = 0; i < lifeupNum; i++)
			new LifeupItem((int)x, (int)y);
	}
	*/
	for(int i = 0; i < 4; i++)
		MV1DeleteModel( grHandle[i] );	
}
//	�҂���ԃ��X�g����A�N�e�B�u���X�g�֌q���ւ�
void EnemyBase::Activation(){
	//	�҂���ԃ��X�g�̑O�オ�O��Ȃ��悤�ɏ���
	next_p->rev_p = rev_p ;		//	�҂���ԃ��X�g��
	rev_p->next_p = next_p	;	//	�O�オ�O��Ȃ��悤�ɏ���	
	//	�A�N�e�B�u���X�g�֓o�^
	next_p = activeFirst_p ;
	rev_p  = activeFirst_p->rev_p ;
	activeFirst_p->rev_p->next_p = this ; 
	activeFirst_p->rev_p = this ;
	PlayShotSE() ;	//	�������ꂽ���̉��i��ɔ��ˉ��j
}

////////////////////////////////////
////���i���āA���Ԋu�Ŏ��@�_���̒e��łG////
class E_mStraight_bStraight : public EnemyBase{
public:
	static int GrHandle ;
	E_mStraight_bStraight( unsigned long long in_waitTime, float initX, float initY, float moveSpeed, float degAngle, float in_bulletSpeed, float interval );
	virtual void Move() ;
	virtual void Draw() ;
	virtual void Shot() ;
};
//	�ÓI�ϐ��̏�����
int E_mStraight_bStraight::GrHandle = NULL ;

E_mStraight_bStraight::E_mStraight_bStraight( unsigned long long in_waitTime,float initX, float initY, float moveSpeed, float degAngle, float in_bulletSpeed, float in_interval ){
	waitTime = in_waitTime ;
	GetVector( moveSpeed, DegToRad * degAngle, &moveX, &moveY);
	angle = degAngle * DegToRad ; 
	x = initX ;
	y = initY ;
	bulletSpeed = in_bulletSpeed ;
	interval = in_interval ;
	nowInterval = 0 ;
	size = 40 ;
	hp = 30 ;
	score = 20;
	scoreNum = 3;
}
//void E_mStraight_bStraight::Move(){
//	x += moveX ;
//	y += moveY ;
//	nowInterval = (nowInterval + 1) % interval ;
//	if(nowInterval == 0)
//		Shot() ;
//	if	(x < BULLET_LEFT_LIMIT || x > BULLET_RIGHT_LIMIT ||
//		y < BULLET_UP_LIMIT	 || y > BULLET_DOWN_LIMIT)
//		alive = false;
//}
void E_mStraight_bStraight::Move(){
	x += moveX ;
	y += moveY ;
	if(interval > GetRand(1000))
		Shot() ;
	if	(x < BULLET_LEFT_LIMIT || x > BULLET_RIGHT_LIMIT ||
		y < BULLET_UP_LIMIT	 || y > BULLET_DOWN_LIMIT)
		alive = false;
}
void E_mStraight_bStraight::Draw(){
	DrawGraphCenter(x, y, GrHandle) ;
}
void E_mStraight_bStraight::Shot(){
	float sAngle = GetAngle(x, y, Player::p->x, Player::p->y);
	float X, Y ;
	GetVector( bulletSpeed, sAngle, &X, &Y );
	new EB_Sinple(x, y, X, Y) ; 
}

////////////////////////////////////
////��]�S���ʒe���o���G////
class E_mStraight_bSpin : public EnemyBase{
public:
	static int GrHandle ;
	int moveStopTime ;
	float bulletAngle[2] ;
	unsigned int nowTime ;
	E_mStraight_bSpin( unsigned long long in_waitTime, float initX, float initY, float moveSpeed, int moveStopTime,
		float degAngle, float in_bulletSpeed, float interval );
	virtual void Move() ;
	virtual void Draw() ;
	void Shot() ;
};
//	�ÓI�ϐ��̏�����
int E_mStraight_bSpin::GrHandle = NULL ;

E_mStraight_bSpin::E_mStraight_bSpin( unsigned long long in_waitTime,float initX, float initY, 
	float moveSpeed,int in_moveStopTime, float degAngle, float in_bulletSpeed, float in_interval ){
	waitTime = in_waitTime ;
	GetVector( moveSpeed, DegToRad * degAngle, &moveX, &moveY);
	angle = degAngle * DegToRad ; 
	x = initX ;
	y = initY ;
	bulletSpeed = in_bulletSpeed ;
	interval = in_interval ;
	nowInterval = 0 ;
	size = 50 ;
	hp = 800 ;
	moveStopTime = in_moveStopTime ;
	nowTime = 0;
	bulletAngle[0] = 0;
	bulletAngle[1] = 0;
	lifeupNum = 1;
	bomNum = 1;
}
void E_mStraight_bSpin::Move(){
	nowTime++;
	if(moveStopTime > nowTime){
		x += moveX ;
		y += moveY ;
	}
	else if(nowTime > 720){	//	12�b�؍݂����瓦����
		x -= moveX ;
		y -= moveY ;
	}
	nowInterval = (nowInterval + 1) % interval ;
	if(nowInterval == 0)
		Shot() ;
	if	(x < BULLET_LEFT_LIMIT || x > BULLET_RIGHT_LIMIT ||
		y < BULLET_UP_LIMIT	 || y > BULLET_DOWN_LIMIT)
		alive = false;
}
void E_mStraight_bSpin::Draw(){
	DrawGraphCenter(x, y, GrHandle) ;
}
void E_mStraight_bSpin::Shot(){
	bulletAngle[0] += DegToRad * 8 ;
	bulletAngle[1] -= DegToRad * 10 ;
	float X, Y ;
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 4; j++){
			GetVector( bulletSpeed, bulletAngle[i] + (j*90*DegToRad) , &X, &Y );
			new EB_Small(x, y, X, Y) ;
		}
	}
}
////////////////////////////////////////////
//	��������nway�e�𔭎˂���G�i�p�x�͌Œ�j
class E_mStraight_bNway_f : public EnemyBase{
public:
	E_mStraight_bNway_f(unsigned long long in_waitTime,float initX, float initY, 
	float moveSpeed,int in_moveStopTime, float degAngle, int in_way,float in_bulletSpeed, float in_interval );
	static int GrHandle ;
	int moveStopTime ;
	int nowTime ;
	unsigned int nway ;
	virtual void Move() ;
	virtual void Draw() ;
	void Shot() ;
};
int E_mStraight_bNway_f::GrHandle = NULL;

E_mStraight_bNway_f::E_mStraight_bNway_f(unsigned long long in_waitTime,float initX, float initY, 
	float moveSpeed,int in_moveStopTime, float degAngle, int in_nway, float in_bulletSpeed, float in_interval )
{
	nway = in_nway;
	waitTime = in_waitTime ;
	GetVector( moveSpeed, DegToRad * degAngle, &moveX, &moveY);
	angle = degAngle * DegToRad ; 
	x = initX ;
	y = initY ;
	bulletSpeed = in_bulletSpeed ;
	interval = in_interval ;
	nowInterval = 0 ;
	size = 40 ;
	hp = 300 ;
	moveStopTime = in_moveStopTime ;
	nowTime = 0;
}
void E_mStraight_bNway_f::Draw(){
	DrawGraphCenter(x, y, GrHandle) ;
}
void E_mStraight_bNway_f::Move(){
	nowTime++;
	if(moveStopTime > nowTime){
		x += moveX ;
		y += moveY ;
	}
	else if(nowTime > 420){	//	�V�b�؍݂����瓦����
		x -= moveX ;
		y -= moveY ;
	}
	nowInterval = (nowInterval + 1) % interval ;
	if(nowInterval < 20)
		Shot() ;
	if	(x < BULLET_LEFT_LIMIT || x > BULLET_RIGHT_LIMIT ||
		y < BULLET_UP_LIMIT	 || y > BULLET_DOWN_LIMIT)
		alive = false;
}

void E_mStraight_bNway_f::Shot(){
	float nowAngle = 0;
	float X, Y ;
	float bulletSpace = (180 / (float)nway) * DegToRad;
	for(int i = 0; i < nway; i++){
		GetVector( bulletSpeed, i * bulletSpace , &X, &Y );
		new EB_Small(x, y, X, Y) ;
	}

}
//------------------------
//	���������2�ʂŎg��
//	�����ɓ����A���@�_����nway�e�����G
class E_mStraight_bNway : public EnemyBase{
public:
	E_mStraight_bNway(unsigned long long in_waitTime,float initX, float initY, 
	float moveSpeed, float degAngle, int in_nway,float in_bulletSpeed, float bulletAngleInterval, float in_interval );
	static int GrHandle ;
	float bulletAngleInterval ;
	int nowTime ;
	unsigned int nway ;
	virtual void Move() ;
	virtual void Draw() ;
	void Shot() ;
};
int E_mStraight_bNway::GrHandle = NULL;
E_mStraight_bNway::E_mStraight_bNway(unsigned long long in_waitTime,float initX, float initY, 
	float moveSpeed, float degAngle, int in_nway,float in_bulletSpeed, float bulletAngleInterval, float in_interval )
{
	this->bulletAngleInterval = bulletAngleInterval ;	//	���@�_��nWay�̊Ԋu
	nway = in_nway;
	waitTime = in_waitTime ;
	GetVector( moveSpeed, DegToRad * degAngle, &moveX, &moveY);
	angle = degAngle * DegToRad ; 
	x = initX ;
	y = initY ;
	bulletSpeed = in_bulletSpeed ;
	interval = in_interval ;
	nowInterval = 0 ;
	size = 40 ;
	hp = 100 ;
	nowTime = 0;

}
void E_mStraight_bNway::Move()
{
	x += moveX;
	y += moveY;
	nowInterval = (nowInterval + 1) % interval ;
	if(nowInterval == 0)
		Shot() ;
	if	(x < BULLET_LEFT_LIMIT || x > BULLET_RIGHT_LIMIT ||
		y < BULLET_UP_LIMIT	 || y > BULLET_DOWN_LIMIT)
		alive = false;
}
void E_mStraight_bNway::Draw()
{
	DrawGraphCenter(x, y, GrHandle);
}
void E_mStraight_bNway::Shot()
{
	if(nway % 2 == 1){	//	�����e�̏ꍇ
		float angle = GetAngle(x, y, Player::p->x, Player::p->y);
		float BMoveX, BMoveY;
		GetVector(bulletSpeed, angle, &BMoveX, &BMoveY); 
		new EB_Small(x, y, BMoveX, BMoveY);	//	���@�_���e������
		int n = nway / 2;	//	���@�_���ׂ̗ɉ�������
		for(int i = 1; n >= i; i++){
			float angleN ;	//	���ۂɌ��p�x
			angleN = angle - (bulletAngleInterval * DegToRad * (float)i) ; 
			GetVector(bulletSpeed, angleN ,&BMoveX, &BMoveY) ;
			new EB_Small(x, y, BMoveX, BMoveY);
			angleN = angle + (bulletAngleInterval * DegToRad * (float)i) ;
			GetVector(bulletSpeed, angleN ,&BMoveX, &BMoveY) ;
			new EB_Small(x, y, BMoveX, BMoveY);
		}
	}
	else{	//	�����e�̏ꍇ�B�Œ�ł�2�����˂����͂��B
		float angle[2] ;
		float BMoveX, BMoveY;
		angle[0] = GetAngle(x, y, Player::p->x, Player::p->y);
		angle[1] = angle[0] + bulletAngleInterval * DegToRad / 2 ;
		angle[0] = angle[0] - bulletAngleInterval * DegToRad / 2 ;
		GetVector(bulletSpeed, angle[0], &BMoveX, &BMoveY) ;
		new EB_Small(x, y, BMoveX, BMoveY) ;
		GetVector(bulletSpeed, angle[1], &BMoveX, &BMoveY) ;
		new EB_Small(x, y, BMoveX, BMoveY) ;
		int n = (nway / 2) - 1 ;	//	�����e�ׂ̗ɉ�������
		for(int i = 1; i <= n; i++){
			float angleN ;
			angleN = angle[0] - (bulletAngleInterval * DegToRad * (float)i) ; 
			GetVector(bulletSpeed, angleN ,&BMoveX, &BMoveY) ;
			new EB_Small(x, y, BMoveX, BMoveY);
			angleN = angle[1] + (bulletAngleInterval * DegToRad * (float)i) ;
			GetVector(bulletSpeed, angleN ,&BMoveX, &BMoveY) ;
			new EB_Small(x, y, BMoveX, BMoveY);
		}
	}
}



//////////////////////////////////////////
////�G�{�X�̊��N���X//////////////////
/////////////////////////////////
class E_BossBase : public EnemyBase{
public:
	static int grHandle;
	float maxHp;
	void DrawHp();
	virtual ~E_BossBase(){};
};
int E_BossBase::grHandle = 0;
void E_BossBase::DrawHp(){
	float hpGage = hp / maxHp; //	�c��HP�̊���
	DrawExtendGraphF(5, 3, 380*hpGage, 8, HpHandle, TRUE);
}

class E_Boss_Fire : public E_BossBase{
public:
	static int HP;	//	�{�X��HP���L�p
	E_Boss_Fire(unsigned long long in_waitTime, int in_x, bool hardFlag);
	virtual void Move();
	virtual void Draw();
	virtual ~E_Boss_Fire(){StageClearFlag = true; StopSoundMem(mainBGM);};
	int interval;
	int nowInterval;
	unsigned int bInterval;
};
int E_Boss_Fire::HP = 0;
E_Boss_Fire::E_Boss_Fire(unsigned long long in_waitTime, int in_x, bool hardFlag = false){
	waitTime = in_waitTime;
	x = in_x;
	y = 1;
	size = 80;
	if(hardFlag == false){
		HP = maxHp = hp = 20000;
	}
	else{
		HP = maxHp = hp = 30000;
	}
	interval = 10;
	nowInterval = 0;
	bInterval = 0;
}
void E_Boss_Fire::Move(){
	if(hp < HP)
		HP = hp;
	else
		hp = HP;

	if(y == 1){	//	�{�X���o��������BGM��ς���
		DeleteSoundMem(mainBGM);
		mainBGM = LoadSoundMem(BOSS1_BGM);
		PlaySoundMem(mainBGM, DX_PLAYTYPE_LOOP);
	}
	float X, Y;
	nowInterval = (nowInterval + 1) % interval ;
	bInterval = (bInterval + 1) % 240;
	if(y < 100)
		y++;
	if(nowInterval == 0){	//	�e�����Ԋu
		if(hp < (maxHp / 3) && (bInterval == 0 || bInterval == 30)){	//	HP���R���̂P�ȉ��A�~�ɓW�J���Ď��@
			for(int i = 0; i < 40; i++){
				GetVector(3, i*9*DegToRad, &X, &Y);
				new EB_Fire_P(x, y, X, Y);
			}	
		}
		else if(hp < (maxHp / 3 * 2) && bInterval == 0){	//	HP���R���̂Q�ȉ��A���@�_���e��ǉ�
			float V = GetAngle(x, y, Player::p->x, Player::p->y);
			GetVector(5, V, &X, &Y);
			new EB_Fire(x, y, X, Y);
		}
		//	��{�e���iHP�Ɋ֌W�Ȃ��o��j
		GetVector(3, (210+GetRand(120))*DegToRad, &X, &Y);
		new EB_Fire(x, y, X, Y);
	}
}
void E_Boss_Fire::Draw(){
	DrawRotaGraph(x, y, 1.0f, 0, grHandle, TRUE);
	DrawHp();
}


//----------------------------------------------------------------------
//----------------------------------------------------------------------
//����������������������������������������������������������������������
//----------------------------------------------------------------------
//----------------------------------------------------------------------

//	ListIterator�Ŏg���ϐ����܂Ƃ߂��^
class systemTime{
public:
	int nowTime, revTime;	//	���Ԃ𑪂邽�߂ɗp��
	int remainingTime;		//	�c�莞�ԁB�����Ń~���b�P�ʁB
	
	void initTime(int setTime) ;	//	��{�͂U�O������
	void Move() ;					//	���Ԃ𑪂�B�P�t���[�����ƂɈ��ĂԂ���
	void Draw(int posX, int posY) ;	//	�c�莞�Ԃ�`�悷��
	void revRetime();				//	�O���Ԃ��X�V����B�|�[�Y���ňꎞ��~������A���A����ɌĂԂ���
	systemTime(int setTime) ;	//	�c�莞�Ԃ�����B
};
systemTime::systemTime(int setTime){
	initTime(setTime) ;
}
void systemTime::initTime(int setTime){	//	��{�͂U�O������
	remainingTime = setTime * 1000 ;
	revTime = GetNowCount() ;	//	���Ԍv���̂��߁A�O���Ԃ��Z�b�g
}
void systemTime::Move(){
	nowTime = GetNowCount() ;
	remainingTime -= (nowTime - revTime) ;
	revTime = nowTime ;
}
void systemTime::Draw(int posX, int posY){
	int output = remainingTime / 1000;	//	�b�P�ʂɒ���
	DrawFormatString(posX, posY, GetColor(255,255,255), "�c�莞�ԁF%d�b", output) ;
}
void systemTime::revRetime(){
	revTime = GetNowCount() ;
}

