
#include "DxLib.h"

/*
#include "stdlib.h"
#include "stdio.h"

#include "def.h"
#include "function.h"
*/

#define PLAYER_MOVE 6		//プレイヤーの移動速度			 X
#define PLAYER_SLOW_MOVE 2	//プレイヤーの低速移動速度

#define UP_LIMIT	20		
#define RIGHT_LIMIT 360		//上下左右の移動できる場所
#define DOWN_LIMIT	460
#define LEFT_LIMIT	20		

#define BULLET_UP_LIMIT		-50	//	この範囲を超えたら、
#define BULLET_RIGHT_LIMIT	450	//	弾を消去する
#define BULLET_DOWN_LIMIT	500
#define BULLET_LEFT_LIMIT	-50

#define ROOT2 1.41421356f 	//プレイヤーの斜め移動速度調整に使用
#define P_SHOT_MOVE_SPEED 20

#define EB_SINPLE_SIZE 10	//	敵シンプル弾の弾サイズ
#define EB_SMALL_SIZE 3		//	敵の小弾のサイズ
#define EB_FIRE_SIZE 6		//	炎弾のサイズ
#define PLAYER_SIZE 3		//	プレイヤーの当たり判定
#define PLAYER_GRAZE_SIZE 20 //	プレイヤーのグレイズ判定
#define ITEM_SIZE 10			//	敵から落ちるアイテムのサイズ
#define BOM_POWER 5		//	ボムの１フレーム当たりのダメージ量
#define BOM_SIZE 50			//	ボムの当たり判定のサイズ

#define PLAYER_DOT_IMAGE "player.png"	//プレイヤーのドット絵の名前
#define PLAYER_RMOVE "graphic\\girl_R.png"	//	右移動時の画像
#define PLAYER_LMOVE "graphic\\girl_L.png"	//	左移動時の画像
#define PLAYER_SHOT "graphic\\girl_S.png"	//	ショット時の画像
#define PLAYER_SHOT_RMOVE "graphic\\girl_SR.png"	//	右移動ショット時の画像
#define PLAYER_SHOT_LMOVE "graphic\\girl_SL.png"	//	左移動ショット時の画像
#define P_SLOW_GRAPHIC "P_SLOW.bmp"		//	プレイヤーがゆっくり移動の時に当たり判定の目安を表示
#define PLAYER_BULLET_GRAPHIC "PBullet2.png"	//	プレイヤー弾の画像の名前
#define PLAYER_BOM "Bom3.png"		//	プレイヤーのボムの名前
#define PLAYER_BOM_EFFECT "Bom_PG.png"	//	ボム無敵時に表示する魔方陣

#define CRASH_PLAYER "player.png"	//	自機撃墜時の画像

#define BACKGROUND_GRAPHIC1 "BackGround.jpg"	//	ループ背景画像の名前
#define BACKGROUND_GRAPHIC2 "graphic\\background2.png"	//	２面のループ画像
#define BACKGROUND_Y_SIZE 512	//	ループ画像のYサイズ
#define BACKGROUND_SPEED 2		//	ループ画像のスクロールスピード
#define PANEL	"panel.png"		//	右のパネル画像名
#define PLAYER_ICON	"Player_icon.png"	//	右パネルの残機表示
#define BOM_ICON "Bom_icon.png"			//	右パネルの残ボム表示
#define TEN_ITEM "graphic\\ten.png"				//	点アイテム
#define LIFEUP_ITEM "graphic\\LifeupItem.png"	//	１ＵＰアイテム
#define BOM_ITEM "graphic\\BomItem.png"		//	ボムアイテム
#define GAME_OVER "graphic\\GameOver.png"	//	ゲームオーバー画面
#define STAGE_CLEAR "graphic\\StageClear.png"	//	クリア画面
#define TITLE "graphic\\title.png"	//	タイトル画面
#define TITLE_CARSOR "graphic\\titleCarsor.png"	//	タイトル画面のカーソル

#define ENEMY_MIDLE_BULLET "enemy_midle_bullet.png"	//	敵の弾の名前
#define ENEMY_SMALL_BULLET "E_smallBullet.png"		//	敵米粒弾の名前
#define ENEMY_FIRE_BULLET "fireBullet60.png"		//	炎弾の名前
#define STRAIGHT_ENEMY	   "straight_enemy.png"	//	直線移動する敵の名前
#define SPIN_ENEMY "graphic\\spinEnemy.png"	//	回転弾幕の敵の名前
#define NWAY_ENEMY "straight_enemy.png"	//	nway弾幕の敵の名前
#define ENEMY_BOSS_FIRE "graphic\\Boss.png"		//	１面のボス
#define HP_GRAPH "graphic\\HpGage.png"	//	敵のHPゲージ
#define H_NWAY_ENEMY "graphic\\spinEnemy.png"	//	2面で登場する自機狙いnway弾幕の敵

#define PB_EFFECT_NAME "PB_Effect.png"	//	自機の弾が着弾したエフェクト
#define GRAZE_EFFECT_NAME "graphic\\graze.png"	//	自機がグレイズしたときのエフェクト
#define EFFECT_SPEED_RATE 0.1	//	エフェクトが大きくなる速さ
#define E_DEL_NAME "E_DeleteEffect.png"	//	敵を撃墜させたときに出すエフェクト

#define PB_SOUND	"sound\\PlayerBullet.wav"	//	プレイヤーが弾を撃つ音
#define PBC_SOUND	"sound\\PlayerBulletCollision.wav"	//	プレイヤーの弾が着弾した音
#define PLAYER_BOM_SOUND "sound\\PlayerBom.wav"	//	ボムを撃ったときの音
#define PLAYER_DEATH "sound\\PlayerDeath.wav"	//	プレイヤーが死んだ時の音
#define ED_SOUND	"sound\\EnemyDelete.wav"	//	敵撃墜音
#define EB_SOUND	"sound\\EnemyBullet.wav"	//	敵が弾を撃つ音
#define MAIN1_BGM "BGM\\main1.mp3"	//	１面のBGM
#define BOSS1_BGM "BGM\\boss1.wav"	//  １面のボス
#define MAIN2_BGM "BGM\\main2.mp3"	//	２面のBGM
#define BOSS2_BGM "BGM\\boss2.mp3"	//	３面のボス

#ifdef OBJCLASS
//----------------------------------------------------------------------------------
float PlayerX = 0;	//	プレイヤーの場所を他クラスから参照するための変数。
float PlayerY = 0;

bool GameOverFlag = false;	//	ゲームオーバーになったらtrue
bool GameClearFlag = false;	//	ゲームクリアしたらtrue
bool StageClearFlag = false;	//	ステージをクリアしたらtrue
int player_icon;	//	右パネルの残機表示に使用
int bom_icon;
int HpHandle;	//	敵BossのHp表示用
int Score = 0;	//	表示するスコア

//	サウンド関係
int PB_sound ;		//	プレイヤーが弾を撃つときに出す音。
int PBC_sound ;		//	プレイヤーの弾が着弾したときに出す音。
int ED_sound ;		//	敵を倒したときに出す音。
int EB_sound ;		//	敵が弾を撃つときに出す音
int mainBGM ;		//	メインBGM

#else
//----------------------------------------------------------------------------------

extern float PlayerX;	//	プレイヤーの場所を他クラスから参照するための変数。
extern float PlayerY;

extern bool GameOverFlag;	//	ゲームオーバーになったらtrue
extern bool GameClearFlag;	//	ゲームクリアしたらtrue
extern bool StageClearFlag;	//	ステージをクリアしたらtrue
extern int player_icon;	//	右パネルの残機表示に使用
extern int bom_icon;
extern int HpHandle;	//	敵BossのHp表示用
extern int Score;	//	表示するスコア

//	サウンド関係
extern int PB_sound ;		//	プレイヤーが弾を撃つときに出す音。
extern int PBC_sound ;		//	プレイヤーの弾が着弾したときに出す音。
extern int ED_sound ;		//	敵を倒したときに出す音。
extern int EB_sound ;		//	敵が弾を撃つときに出す音
extern int mainBGM ;		//	メインBGM

//	エネミーデータを読み込む。事情により宣言だけしておく
void LoadEnemyData(int stage) ;

//	コンパイルの警告を出さないようにfloatの関数をオーバーロードする
void DrawGraph(float x, float y, int grHandle, int TransFlag = TRUE);

void DrawRotaGraph(float x, float y, float ExRate, float Angle, int grHandle, 
	int TransFlag = TRUE, int TurnFlag = FALSE);

//	中心を基点として描画する
void DrawGraphCenter( float x , float y ,int GraphicHundle);

//	アルファブレンドを使用して描画する
void DrawExAlpha( float X , float Y ,int alpha, float ExRate, float angle, int grHandle);

//	アルファブレンドを使用して描画する
void DrawAlpha( float X , float Y ,int alpha,int grHandle);

//	減算ブレンドを使用して描画する
void DrawSub( float X, float Y, int sub, int grHandle);

//	弾を描画するときは９０度回転させる
void DrawBullet(float x, float y, float ExRate, float Angle, int GrHandle);

//	リスト構造を使う場合に継承するクラス
class MoverBase{	
public:
	float x , y ;	//	座標
	virtual void Move() {} ;
	virtual void Draw() {} ;
	MoverBase(){} ;
	virtual ~MoverBase(){} ;
};

//////////////////////////////
/////背景画像クラス///////////
//////////////////////////

class BackGround{
public:
	int alpha ; //	２５４以下の場合、アルファブレンド描画
	float yA , yB;
	float speed ;
	int ySize ;
	int graphicHandle;
	void Move() ;
	void Draw() ;
	BackGround(char *graficName, float speed) ;

};

/////////////////////////////
//	ゲームオーバークラス/////

class GameOver : public MoverBase{
public:
	int grHandle;
	virtual void Move();
	virtual void Draw();
	GameOver(char grName[50]);
};

//////////////////////////////////
////ステージクリアクラス/////////
class StageClear : public MoverBase{
public:
	int grHandle;
	virtual void Move();
	virtual void Draw();
	StageClear(char grName[50]);
};

///////////////////////////
///エフェクトクラス////////
/////////////////////////
//	エフェクトの基底クラス（必ず継承）
class EffectBase : public MoverBase{
public:
	static EffectBase *first_p ;
	bool alive  ;	//	生存フラグ
	float angle ;	//	エフェクトの角度（毎度回して描画する）
	float sizeRate;	//	現在のエフェクトの大きさ。１になったらdelete
	EffectBase *next_p , *rev_p ;	//	リスト構造にするため、前後のポインタ
	EffectBase() ;		//	リストの接続
	EffectBase(bool initFlg) ;		//	リストの最初でのみ使用（ダミーリスト）
	~EffectBase() ;	//	前後リストの接続
	virtual void Move(){} ;
	virtual void Draw(){} ;
};

//	自機弾の着弾エフェクト
class PB_Effect : public EffectBase{
public:
	static int grHandle ;
	PB_Effect(int in_x, int in_y) ;
	virtual void Move() ;
	virtual void Draw() ;
};

//	敵撃破エフェクト
class E_DeleteEffect : public EffectBase{
public:
	static int grHandle ;
	E_DeleteEffect(float in_x, float in_y );
	virtual void Move() ;
	virtual void Draw() ;
};

//	自機グレイズのエフェクト
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
///プレイヤー弾のクラス/////
////////////////////////////


//	プレイヤー弾の基底クラス
class PB_Base : public MoverBase{
public:
	static PB_Base *first_p ;
	float moveX , moveY ;
	
	int skeletonPos ;	//	スケルトン部位情報
	VECTOR centerV ;	//	体の中心情報を保存
	VECTOR revV ; //	前にとったスケルトン情報を保存
	VECTOR nowV ; //	現在のスケルトン情報を保存
	float posX, posY ;	//	スクリーン座標

	float speed ;	//	スピード
	float angle ;	//	角度
	float size  ;	//	サイズ
	bool alive  ;	//	生存フラグ
	float power ;	//	弾のダメージ量
	PB_Base *next_p , *rev_p ;	//	リスト構造にするため、前後のポインタ
	PB_Base() ;		//	リストの接続
	PB_Base(bool initFlg) ;		//	リストの最初でのみ使用（ダミーリスト）
	~PB_Base() ;	//	前後リストの接続
	virtual void Move(){} ;
	virtual void Draw(){} ;
};

//-----------------------------
//	プレイヤーの直線弾
class PB_Straight : public PB_Base {
public:
	static int graphicHandle ;
	PB_Straight(float posX , float posY, float move, float angle , float Power) ;	
	void Move() ;
	void Draw() ;
};


//-------------------------------
////プレイヤーのボム弾クラス
//-------------------------------
//	プレイヤーボムの基底クラス（必ず継承）
class PlayerBomBase : public MoverBase{
public:
	static PlayerBomBase *first_p ;
	bool alive  ;	//	生存フラグ
	float angle ;	//	エフェクトの角度（毎度回して描画する）
	float size ;	//	弾のサイズ
	float moveX , moveY;	//	X,Y軸の移動速度
	float power ;	//	１フレームあたりのダメージ量
	PlayerBomBase *next_p , *rev_p ;	//	リスト構造にするため、前後のポインタ
	PlayerBomBase() ;		//	リストの接続
	PlayerBomBase(bool initFlg) ;		//	リストの最初でのみ使用（ダミーリスト）
	~PlayerBomBase() ;	//	前後リストの接続
	virtual void Move(){} ;
	virtual void Draw(){} ;
};

//	星形ボム
class P_StarBom : public PlayerBomBase{
public:
	static int grHandle ;
	P_StarBom(float in_x, float in_y, float degAngle, float in_speed);
	virtual void Move(); 
	virtual void Draw();
};

/////////////////////////////
//////プレイヤークラス////////
/////////////////////////////
class Player : public MoverBase {
private:
	int graphicHandle[2][3];	//	自機の画像[shotFlag][tiltFlag]
	int slowHandle ;	//	当たり判定の目安の画像
	int shotFlag ;
	int tiltFlag ;
public:
	static int BE_grHandle;	//	ボム無敵中に出すエフェクト
	static Player *p ;	//	他クラスでプレイヤーを参照するときに使用
	int deathHandle;	//	墜落音
	int bomSound;		//	ボムの音
	int noHitTime ;		//	無敵時間
	int flashTime ;		//	点滅時間。自機が被弾したあとに使用。
	bool alive ;
	int hitAliveTime ;	//	喰らいボム受付時間
	float slowSizeRate ;//	スロウ時に表示される画像のサイズを制御するのに使用
	float hitSize ;		//	当たり判定の大きさ
	float grazeSize;	//	グレイズ判定の大きさ
	int stock ;			//	残機数
	int bom ;			//	残りボム数
	int power;			//	パワー
	int interval ;		//	ショットの間隔
	int bomEffect;		//	ボムの無敵時間の目安を示す魔方陣
	float BE_spin ;		//	魔方陣を回すための制御変数
	Player(char graphicFileName[]);
	Player(){};
	void virtual Draw();
	void virtual Move();
	void Shot();
};

//	被弾したときに表示する自機

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
/////アイテムクラス/////////////////
/////////////////////////////////////
//	アイテムの基底クラス（必ず継承）
class ItemBase : public MoverBase{
public:
	static ItemBase *first_p ;
	bool trackingFlag ;	//	trueのときは、自機に集まる
	bool alive ;	//	生存フラグ
	int upCnt ;		//　出現直後は上昇する。その上昇時間を数える。
	float angle ;	//	進行方向
	float moveX, moveY ;	//	上昇時に進むX成分Y成分	
	float size ;	//	あたり判定の大きさ
	ItemBase *next_p , *rev_p ;	//	リスト構造にするための前後のポインタ
	ItemBase() ;	//	リストの接続
	ItemBase(bool initFlag) ; //	リストの先頭をつくる（ダミーリスト）
	ItemBase(int in_x, int in_y);
	int score;	//	スコアアイテムでのみ使用
	~ItemBase() ;	//	前後リストの接続
	virtual void Move() ;
	virtual void Draw(){} ;
	virtual void Get(){} ;	//	アイテムを取得した時の処理
};

//////////////////////////////////////
//	スコアアイテム//////////////////
//	スコアアイテムのベース（継承して）
class ScoreItem	: public ItemBase{
public:
	static int grHandle   ;
	virtual void Draw() ;
	virtual void Get();
	ScoreItem(int in_x, int in_y, int in_score);
};

/////////////////////////////////////
////////ボムアイテム/////////////////
class BomItem : public ItemBase{
public:
	static int grHandle ;
	virtual void Draw() ;
	virtual void Get() ;
	BomItem(int in_x, int in_y) : ItemBase(in_x, in_y){} ;
};

/////////////////////////////////////
////////１ＵＰアイテム///////////////
class LifeupItem : public ItemBase{
public:
	static int grHandle ;
	virtual void Draw() ;
	virtual void Get() ;
	LifeupItem(int in_x, int in_y) : ItemBase(in_x, in_y){} ;
};

///////////////////////////////////
////敵弾クラス///////////////////
//////////////////////////////

//	敵弾の基底クラス
class EnemyBulletBase{
public:
	static EnemyBulletBase *first_p ;
	float x , y ;	//	座標
	float moveX, moveY ;
	float speed ;	//	弾のスピード　
	float angle ;	//	角度
	float size ;	//	弾の大きさ
	bool alive ;
	bool grazed ;	//	グレイズフラグ、trueのときはグレイズしない。
	EnemyBulletBase *next_p , *rev_p ;	//	リスト構造にするため、前後のポインタ
	EnemyBulletBase() ;
	EnemyBulletBase(bool initFlg) ;
	~EnemyBulletBase() ;
	virtual void Move(){};
	virtual void Draw(){};
};


////////////////////////////
//	直線、サイズ中の弾	/////
class EB_Sinple : EnemyBulletBase{
public:
	static int grHandle ;
	virtual void Move() ;
	virtual void Draw() ;
	EB_Sinple(float in_x, float in_y, float in_moveX, float in_moveY) ;
};

////////////////////////////
//	直線、サイズ小の米粒弾	/////
class EB_Small : EnemyBulletBase{
public:
	static int grHandle ;
	virtual void Move() ;
	virtual void Draw() ;
	EB_Small(float in_x, float in_y, float in_moveX, float in_moveY) ;
};
///////////////////////
//火の玉（画面上部に到達すると垂直に落ちる）
class EB_Fire : public EnemyBulletBase{
public:
	static int grHandle ;
	virtual void Move() ;
	virtual void Draw() ;
	EB_Fire(float in_x, float in_y, float in_moveX, float in_moveY) ;
};

class EB_Fire_P : EB_Fire{
	//	継承元クラスのコンストラクタを使う
public:
	int moveCnt;	//	非自機狙いで移動した時間
	EB_Fire_P(float in_x, float in_y, float in_moveX, float in_moveY) :
		EB_Fire(in_x, in_y, in_moveX, in_moveY){moveCnt = 0; size = EB_FIRE_SIZE/3*2;}
	virtual void Move();
	virtual void Draw();
};

/////////////////////////////////////////
/////////敵クラス//////////////////////
////////////////////////////////////////
//	敵の基底クラス
class EnemyBase : public MoverBase{
public:
	static EnemyBase *activeFirst_p ;	//	アクティブリストのダミーへのポインタ
	static EnemyBase *waitFirst_p ;		//	待ち状態リストのダミーへのポインタ
	EnemyBase *next_p , *rev_p ;	//	リスト構造にするため、前後のポインタ
	
	//	追加
	int grHandle[4] ;
	VECTOR MV ;	//	移動力
	bool hitFlag ;
	VECTOR initPos;	// 初期位置
	VECTOR initVelocity;	// 初速度
	float realTimer ;	//	秒換算のタイマー	ミリ秒まで正確。
	int damage;	//	ダメージ量。１以上で触るとダメージあり。
	int boundNum ;	//	バウンドの回数
	int state ;		//	ドラム缶の凹み状態（０が無傷、４が超凹み）

	int score ;	//	落とすスコアの得点
	int scoreNum ;	//	落とすスコアの数
	int bomNum ;	//	落とすボムの数
	int lifeupNum ;	//	落とす１ＵＰの数

	float moveX , moveY ;	//	XとYの移動距離
	float speed ;	//	スピード
	float angle ;	//	角度
	float size  ;	//	サイズ
	float hp ;		//	体力
	int	  interval ;//	ショットを撃つ間隔
	int	  nowInterval;	//	ショット間隔を制御
	float bulletSpeed ; //	弾の速さ
	unsigned long long waitTime ;	//	出現待ち時間
	bool alive  ;	//	生存フラグ

	EnemyBase() ;		//	リストの接続
	EnemyBase(bool initFlg) ;		//	リストの最初でのみ使用（ダミーリスト）
	virtual ~EnemyBase() ;	//	前後リストの接続
	virtual void Move(){} ;
	virtual void Draw(){} ;
	virtual void DamageEffect(){} ;
	virtual void PlayShotSE(){} ;
	virtual void PlayCollisionSE(){} ;
	void Activation() ;		//	待ち状態リストからアクティブリストへ移動
};

////////////////////////////////////
////直進して、一定間隔で自機狙いの弾を打つ敵////
class E_mStraight_bStraight : public EnemyBase{
public:
	static int GrHandle ;
	E_mStraight_bStraight( unsigned long long in_waitTime, float initX, float initY, float moveSpeed, float degAngle, float in_bulletSpeed, float interval );
	virtual void Move() ;
	virtual void Draw() ;
	virtual void Shot() ;
};

////////////////////////////////////
////回転全方位弾を出す敵////
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
//	下方向にnway弾を発射する敵（角度は固定）
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
//	ここからは2面で使う
//	直線に動き、自機狙いのnway弾を撃つ敵
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
////敵ボスの基底クラス//////////////////
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
	static int HP;	//	ボスのHP共有用
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
//◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
//----------------------------------------------------------------------
//----------------------------------------------------------------------

//	ListIteratorで使う変数をまとめた型
class systemTime{
public:
	int nowTime, revTime;	//	時間を測るために用意
	int remainingTime;		//	残り時間。整数でミリ秒単位。
	
	void initTime(int setTime) ;	//	基本は６０を入れる
	void Move() ;					//	時間を測る。１フレームごとに一回呼ぶこと
	void Draw(int posX, int posY) ;	//	残り時間を描画する
	void revRetime();				//	前時間を更新する。ポーズ等で一時停止したら、復帰直後に呼ぶこと
	systemTime(int setTime) ;	//	残り時間を入れる。
};

//----------------------------------------------------------------------------------
#endif
