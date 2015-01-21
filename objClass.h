
#include "DxLib.h"

/*
#include "stdlib.h"
#include "stdio.h"

#include "def.h"
#include "function.h"
*/

#define PLAYER_MOVE 6		//�v���C���[�̈ړ����x			 X
#define PLAYER_SLOW_MOVE 2	//�v���C���[�̒ᑬ�ړ����x

#define UP_LIMIT	20		
#define RIGHT_LIMIT 360		//�㉺���E�̈ړ��ł���ꏊ
#define DOWN_LIMIT	460
#define LEFT_LIMIT	20		

#define BULLET_UP_LIMIT		-50	//	���͈̔͂𒴂�����A
#define BULLET_RIGHT_LIMIT	450	//	�e����������
#define BULLET_DOWN_LIMIT	500
#define BULLET_LEFT_LIMIT	-50

#define ROOT2 1.41421356f 	//�v���C���[�̎΂߈ړ����x�����Ɏg�p
#define P_SHOT_MOVE_SPEED 20

#define EB_SINPLE_SIZE 10	//	�G�V���v���e�̒e�T�C�Y
#define EB_SMALL_SIZE 3		//	�G�̏��e�̃T�C�Y
#define EB_FIRE_SIZE 6		//	���e�̃T�C�Y
#define PLAYER_SIZE 3		//	�v���C���[�̓����蔻��
#define PLAYER_GRAZE_SIZE 20 //	�v���C���[�̃O���C�Y����
#define ITEM_SIZE 10			//	�G���痎����A�C�e���̃T�C�Y
#define BOM_POWER 5		//	�{���̂P�t���[��������̃_���[�W��
#define BOM_SIZE 50			//	�{���̓����蔻��̃T�C�Y

#define PLAYER_DOT_IMAGE "player.png"	//�v���C���[�̃h�b�g�G�̖��O
#define PLAYER_RMOVE "graphic\\girl_R.png"	//	�E�ړ����̉摜
#define PLAYER_LMOVE "graphic\\girl_L.png"	//	���ړ����̉摜
#define PLAYER_SHOT "graphic\\girl_S.png"	//	�V���b�g���̉摜
#define PLAYER_SHOT_RMOVE "graphic\\girl_SR.png"	//	�E�ړ��V���b�g���̉摜
#define PLAYER_SHOT_LMOVE "graphic\\girl_SL.png"	//	���ړ��V���b�g���̉摜
#define P_SLOW_GRAPHIC "P_SLOW.bmp"		//	�v���C���[���������ړ��̎��ɓ����蔻��̖ڈ���\��
#define PLAYER_BULLET_GRAPHIC "PBullet2.png"	//	�v���C���[�e�̉摜�̖��O
#define PLAYER_BOM "Bom3.png"		//	�v���C���[�̃{���̖��O
#define PLAYER_BOM_EFFECT "Bom_PG.png"	//	�{�����G���ɕ\�����閂���w

#define CRASH_PLAYER "player.png"	//	���@���Ď��̉摜

#define BACKGROUND_GRAPHIC1 "BackGround.jpg"	//	���[�v�w�i�摜�̖��O
#define BACKGROUND_GRAPHIC2 "graphic\\background2.png"	//	�Q�ʂ̃��[�v�摜
#define BACKGROUND_Y_SIZE 512	//	���[�v�摜��Y�T�C�Y
#define BACKGROUND_SPEED 2		//	���[�v�摜�̃X�N���[���X�s�[�h
#define PANEL	"panel.png"		//	�E�̃p�l���摜��
#define PLAYER_ICON	"Player_icon.png"	//	�E�p�l���̎c�@�\��
#define BOM_ICON "Bom_icon.png"			//	�E�p�l���̎c�{���\��
#define TEN_ITEM "graphic\\ten.png"				//	�_�A�C�e��
#define LIFEUP_ITEM "graphic\\LifeupItem.png"	//	�P�t�o�A�C�e��
#define BOM_ITEM "graphic\\BomItem.png"		//	�{���A�C�e��
#define GAME_OVER "graphic\\GameOver.png"	//	�Q�[���I�[�o�[���
#define STAGE_CLEAR "graphic\\StageClear.png"	//	�N���A���
#define TITLE "graphic\\title.png"	//	�^�C�g�����
#define TITLE_CARSOR "graphic\\titleCarsor.png"	//	�^�C�g����ʂ̃J�[�\��

#define ENEMY_MIDLE_BULLET "enemy_midle_bullet.png"	//	�G�̒e�̖��O
#define ENEMY_SMALL_BULLET "E_smallBullet.png"		//	�G�ė��e�̖��O
#define ENEMY_FIRE_BULLET "fireBullet60.png"		//	���e�̖��O
#define STRAIGHT_ENEMY	   "straight_enemy.png"	//	�����ړ�����G�̖��O
#define SPIN_ENEMY "graphic\\spinEnemy.png"	//	��]�e���̓G�̖��O
#define NWAY_ENEMY "straight_enemy.png"	//	nway�e���̓G�̖��O
#define ENEMY_BOSS_FIRE "graphic\\Boss.png"		//	�P�ʂ̃{�X
#define HP_GRAPH "graphic\\HpGage.png"	//	�G��HP�Q�[�W
#define H_NWAY_ENEMY "graphic\\spinEnemy.png"	//	2�ʂœo�ꂷ�鎩�@�_��nway�e���̓G

#define PB_EFFECT_NAME "PB_Effect.png"	//	���@�̒e�����e�����G�t�F�N�g
#define GRAZE_EFFECT_NAME "graphic\\graze.png"	//	���@���O���C�Y�����Ƃ��̃G�t�F�N�g
#define EFFECT_SPEED_RATE 0.1	//	�G�t�F�N�g���傫���Ȃ鑬��
#define E_DEL_NAME "E_DeleteEffect.png"	//	�G�����Ă������Ƃ��ɏo���G�t�F�N�g

#define PB_SOUND	"sound\\PlayerBullet.wav"	//	�v���C���[���e������
#define PBC_SOUND	"sound\\PlayerBulletCollision.wav"	//	�v���C���[�̒e�����e������
#define PLAYER_BOM_SOUND "sound\\PlayerBom.wav"	//	�{�����������Ƃ��̉�
#define PLAYER_DEATH "sound\\PlayerDeath.wav"	//	�v���C���[�����񂾎��̉�
#define ED_SOUND	"sound\\EnemyDelete.wav"	//	�G���ĉ�
#define EB_SOUND	"sound\\EnemyBullet.wav"	//	�G���e������
#define MAIN1_BGM "BGM\\main1.mp3"	//	�P�ʂ�BGM
#define BOSS1_BGM "BGM\\boss1.wav"	//  �P�ʂ̃{�X
#define MAIN2_BGM "BGM\\main2.mp3"	//	�Q�ʂ�BGM
#define BOSS2_BGM "BGM\\boss2.mp3"	//	�R�ʂ̃{�X

#ifdef OBJCLASS
//----------------------------------------------------------------------------------
float PlayerX = 0;	//	�v���C���[�̏ꏊ�𑼃N���X����Q�Ƃ��邽�߂̕ϐ��B
float PlayerY = 0;

bool GameOverFlag = false;	//	�Q�[���I�[�o�[�ɂȂ�����true
bool GameClearFlag = false;	//	�Q�[���N���A������true
bool StageClearFlag = false;	//	�X�e�[�W���N���A������true
int player_icon;	//	�E�p�l���̎c�@�\���Ɏg�p
int bom_icon;
int HpHandle;	//	�GBoss��Hp�\���p
int Score = 0;	//	�\������X�R�A

//	�T�E���h�֌W
int PB_sound ;		//	�v���C���[���e�����Ƃ��ɏo�����B
int PBC_sound ;		//	�v���C���[�̒e�����e�����Ƃ��ɏo�����B
int ED_sound ;		//	�G��|�����Ƃ��ɏo�����B
int EB_sound ;		//	�G���e�����Ƃ��ɏo����
int mainBGM ;		//	���C��BGM

#else
//----------------------------------------------------------------------------------

extern float PlayerX;	//	�v���C���[�̏ꏊ�𑼃N���X����Q�Ƃ��邽�߂̕ϐ��B
extern float PlayerY;

extern bool GameOverFlag;	//	�Q�[���I�[�o�[�ɂȂ�����true
extern bool GameClearFlag;	//	�Q�[���N���A������true
extern bool StageClearFlag;	//	�X�e�[�W���N���A������true
extern int player_icon;	//	�E�p�l���̎c�@�\���Ɏg�p
extern int bom_icon;
extern int HpHandle;	//	�GBoss��Hp�\���p
extern int Score;	//	�\������X�R�A

//	�T�E���h�֌W
extern int PB_sound ;		//	�v���C���[���e�����Ƃ��ɏo�����B
extern int PBC_sound ;		//	�v���C���[�̒e�����e�����Ƃ��ɏo�����B
extern int ED_sound ;		//	�G��|�����Ƃ��ɏo�����B
extern int EB_sound ;		//	�G���e�����Ƃ��ɏo����
extern int mainBGM ;		//	���C��BGM

//	�G�l�~�[�f�[�^��ǂݍ��ށB����ɂ��錾�������Ă���
void LoadEnemyData(int stage) ;

//	�R���p�C���̌x�����o���Ȃ��悤��float�̊֐����I�[�o�[���[�h����
void DrawGraph(float x, float y, int grHandle, int TransFlag = TRUE);

void DrawRotaGraph(float x, float y, float ExRate, float Angle, int grHandle, 
	int TransFlag = TRUE, int TurnFlag = FALSE);

//	���S����_�Ƃ��ĕ`�悷��
void DrawGraphCenter( float x , float y ,int GraphicHundle);

//	�A���t�@�u�����h���g�p���ĕ`�悷��
void DrawExAlpha( float X , float Y ,int alpha, float ExRate, float angle, int grHandle);

//	�A���t�@�u�����h���g�p���ĕ`�悷��
void DrawAlpha( float X , float Y ,int alpha,int grHandle);

//	���Z�u�����h���g�p���ĕ`�悷��
void DrawSub( float X, float Y, int sub, int grHandle);

//	�e��`�悷��Ƃ��͂X�O�x��]������
void DrawBullet(float x, float y, float ExRate, float Angle, int GrHandle);

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

/////////////////////////////
//	�Q�[���I�[�o�[�N���X/////

class GameOver : public MoverBase{
public:
	int grHandle;
	virtual void Move();
	virtual void Draw();
	GameOver(char grName[50]);
};

//////////////////////////////////
////�X�e�[�W�N���A�N���X/////////
class StageClear : public MoverBase{
public:
	int grHandle;
	virtual void Move();
	virtual void Draw();
	StageClear(char grName[50]);
};

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

//	���@�e�̒��e�G�t�F�N�g
class PB_Effect : public EffectBase{
public:
	static int grHandle ;
	PB_Effect(int in_x, int in_y) ;
	virtual void Move() ;
	virtual void Draw() ;
};

//	�G���j�G�t�F�N�g
class E_DeleteEffect : public EffectBase{
public:
	static int grHandle ;
	E_DeleteEffect(float in_x, float in_y );
	virtual void Move() ;
	virtual void Draw() ;
};

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

//-----------------------------
//	�v���C���[�̒����e
class PB_Straight : public PB_Base {
public:
	static int graphicHandle ;
	PB_Straight(float posX , float posY, float move, float angle , float Power) ;	
	void Move() ;
	void Draw() ;
};


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

//	���`�{��
class P_StarBom : public PlayerBomBase{
public:
	static int grHandle ;
	P_StarBom(float in_x, float in_y, float degAngle, float in_speed);
	virtual void Move(); 
	virtual void Draw();
};

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

/////////////////////////////////////
////////�{���A�C�e��/////////////////
class BomItem : public ItemBase{
public:
	static int grHandle ;
	virtual void Draw() ;
	virtual void Get() ;
	BomItem(int in_x, int in_y) : ItemBase(in_x, in_y){} ;
};

/////////////////////////////////////
////////�P�t�o�A�C�e��///////////////
class LifeupItem : public ItemBase{
public:
	static int grHandle ;
	virtual void Draw() ;
	virtual void Get() ;
	LifeupItem(int in_x, int in_y) : ItemBase(in_x, in_y){} ;
};

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


////////////////////////////
//	�����A�T�C�Y���̒e	/////
class EB_Sinple : EnemyBulletBase{
public:
	static int grHandle ;
	virtual void Move() ;
	virtual void Draw() ;
	EB_Sinple(float in_x, float in_y, float in_moveX, float in_moveY) ;
};

////////////////////////////
//	�����A�T�C�Y���̕ė��e	/////
class EB_Small : EnemyBulletBase{
public:
	static int grHandle ;
	virtual void Move() ;
	virtual void Draw() ;
	EB_Small(float in_x, float in_y, float in_moveX, float in_moveY) ;
};
///////////////////////
//�΂̋ʁi��ʏ㕔�ɓ��B����Ɛ����ɗ�����j
class EB_Fire : public EnemyBulletBase{
public:
	static int grHandle ;
	virtual void Move() ;
	virtual void Draw() ;
	EB_Fire(float in_x, float in_y, float in_moveX, float in_moveY) ;
};

class EB_Fire_P : EB_Fire{
	//	�p�����N���X�̃R���X�g���N�^���g��
public:
	int moveCnt;	//	�񎩋@�_���ňړ���������
	EB_Fire_P(float in_x, float in_y, float in_moveX, float in_moveY) :
		EB_Fire(in_x, in_y, in_moveX, in_moveY){moveCnt = 0; size = EB_FIRE_SIZE/3*2;}
	virtual void Move();
	virtual void Draw();
};

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

//----------------------------------------------------------------------------------
#endif
