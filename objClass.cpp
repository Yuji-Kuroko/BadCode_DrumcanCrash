// これが定義されているかによって、*.hファイルの中で条件コンパイルする
#define OBJCLASS

#include <math.h>

#include "stdlib.h"
#include "stdio.h"
#include "DxLib.h"

#include "def.h"
#include "function.h"
#include "objClass.h"

//	エネミーデータを読み込む。事情により宣言だけしておく
void LoadEnemyData(int stage) ;

//	コンパイルの警告を出さないようにfloatの関数をオーバーロードする
void DrawGraph(float x, float y, int grHandle, int TransFlag = TRUE)
{
	DrawGraph((int)x, (int)y, grHandle, TRUE);
}
void DrawRotaGraph(float x, float y, float ExRate, float Angle, int grHandle, 
	int TransFlag = TRUE, int TurnFlag = FALSE)
{
	DrawRotaGraph((int)x, (int)y, (double)ExRate, (double)Angle, grHandle, TransFlag, TurnFlag);
}


//	中心を基点として描画する
void DrawGraphCenter( float x , float y ,int GraphicHundle)
{
	DrawRotaGraph( (int)x, (int)y, 1.0, 0, GraphicHundle, TRUE ) ;
}

//	アルファブレンドを使用して描画する
void DrawExAlpha( float X , float Y ,int alpha, float ExRate, float angle, int grHandle)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha) ;
	DrawRotaGraph( X, Y, ExRate, angle, grHandle,TRUE) ;
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255) ;
}
//	アルファブレンドを使用して描画する
void DrawAlpha( float X , float Y ,int alpha,int grHandle)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha) ;
	DrawGraph(X, Y, grHandle, TRUE) ;
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255) ;
} 
//	減算ブレンドを使用して描画する
void DrawSub( float X, float Y, int sub, int grHandle)
{
	SetDrawBlendMode( DX_BLENDMODE_SUB, sub );
	DrawGraph(X, Y, grHandle) ;
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 ) ;
}


//	弾を描画するときは９０度回転させる
void DrawBullet(float x, float y, float ExRate, float Angle, int GrHandle)
{
	Angle += DegToRad * 90;
	DrawRotaGraph(x, y, ExRate, Angle, GrHandle, TRUE);
	return ;
}



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

BackGround::BackGround(char *graficName, float in_speed){
	graphicHandle = LoadGraph(graficName) ;	//	グラフィックをロードする
	int xSize; // 下の関数のためのダミー宣言
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
//	ゲームオーバークラス/////
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
////ステージクリアクラス/////////
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
//	静的メンバ変数の初期化
EffectBase *EffectBase::first_p = NULL ;

//	リストの先頭を記憶
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
	rev_p->next_p = next_p	;	//	前後リストが外れないように処理	
}

//	自機弾の着弾エフェクト
class PB_Effect : public EffectBase{
public:
	static int grHandle ;
	PB_Effect(int in_x, int in_y) ;
	virtual void Move() ;
	virtual void Draw() ;
};
//	静的変数初期化
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

//	敵撃破エフェクト
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
	PlaySoundMem(ED_sound, DX_PLAYTYPE_BACK) ;	//	敵撃破音を出す
}
void E_DeleteEffect::Move(){
	sizeRate += 0.05 ;
	if(sizeRate > 1)
		alive = false ;
}
void E_DeleteEffect::Draw(){
	DrawRotaGraph(x, y, sizeRate, angle, grHandle, TRUE) ;
}

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
	//	cntが0になったら削除する
	if(cnt == 45)
		alive = false;
}
void GrazeEffect::Draw(){
	DrawRotaGraph(x, y, 1.0f, angle, grHandle, TRUE);
}


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

//	静的メンバ変数の初期化
PB_Base *PB_Base::first_p = NULL ;


//	リストの先頭を記憶
PB_Base::PB_Base(){
	next_p = first_p->next_p ;
	rev_p  = first_p ;
	first_p->next_p->rev_p = this ; 
	first_p->next_p = this ;
	alive = true ;
	//PlaySoundMem(PB_sound, DX_PLAYTYPE_BACK) ;	//	自機弾発射音を出す
}

PB_Base::PB_Base(bool startInitFlg ){
	first_p = this ;
	next_p  = this ;
	rev_p   = this ;
	alive   = true ;
}

PB_Base::~PB_Base(){
	next_p->rev_p = rev_p ;
	rev_p->next_p = next_p	;	//	前後リストが外れないように処理	
}






//-----------------------------
//	プレイヤーの直線弾
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
	GetVector( pspeed, RadAngle, &moveX , &moveY );	//	角度と移動速度からXとY成分を計算して代入
}

//	描画する
void PB_Straight::Draw(){
	DrawBullet(x , y , 1.0 , angle, graphicHandle) ;
}	



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
//	静的メンバ変数の初期化
PlayerBomBase *PlayerBomBase::first_p = NULL ;
//	リストの先頭を記憶
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
	rev_p->next_p = next_p	;	//	前後リストが外れないように処理	
}

//	星形ボム
class P_StarBom : public PlayerBomBase{
public:
	static int grHandle ;
	P_StarBom(float in_x, float in_y, float degAngle, float in_speed);
	virtual void Move(); 
	virtual void Draw();
};
int P_StarBom::grHandle = NULL ;
//	コンストラクタ
P_StarBom::P_StarBom(float in_x, float in_y, float degAngle, float in_speed){
	x = in_x;
	y = in_y;
	GetVector(in_speed, degAngle * DegToRad, &moveX, &moveY);
}
void P_StarBom::Move(){
	x += moveX ;
	y += moveY ;
	angle += 7 * DegToRad;	//	１フレームで7度回転させる
	if	(x < BULLET_LEFT_LIMIT || x > BULLET_RIGHT_LIMIT ||
		y < BULLET_UP_LIMIT	 || y > BULLET_DOWN_LIMIT)
		alive = false;
}
void P_StarBom::Draw(){
	DrawRotaGraph(x, y, 1.0f, angle, grHandle, TRUE) ; 
}


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
//	静的メンバ変数の初期化
Player *Player::p = NULL ;
int Player::BE_grHandle = NULL;
//	プレイヤークラスのコンストラクタ
Player::Player(char graphicFileName[30]){
	graphicHandle[0][1] = LoadGraph(graphicFileName);	//	プレイヤーグラフィック読み込み
	graphicHandle[0][2] = LoadGraph(PLAYER_RMOVE);
	graphicHandle[0][0] = LoadGraph(PLAYER_LMOVE);
	graphicHandle[1][1] = LoadGraph(PLAYER_SHOT);
	graphicHandle[1][2] = LoadGraph(PLAYER_SHOT_RMOVE);
	graphicHandle[1][0] = LoadGraph(PLAYER_SHOT_LMOVE);
	slowHandle = LoadGraph(P_SLOW_GRAPHIC) ;	//	当たり判定の目安の読み込み
	deathHandle = LoadSoundMem(PLAYER_DEATH) ;		//	自機墜落音の読み込み
	bomSound = LoadSoundMem(PLAYER_BOM_SOUND) ;	//	ボム発射音
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

//	プレイヤーの描画
void Player::Draw(){
	if(bomEffect > 0){
		BE_spin += DegToRad * 7;
		bomEffect--;
		DrawRotaGraph(x, y, (float)bomEffect/300.0f, BE_spin, BE_grHandle, TRUE) ;
	}
	if((flashTime % 10) < 5)	//	自機が墜落したあとの復帰エフェクト
		DrawGraphCenter(x, y, graphicHandle[shotFlag][tiltFlag]);	//	自機を描画
	if(flashTime > 0)
		flashTime--;
	if(CheckHitKey(KEY_INPUT_RSHIFT) || CheckHitKey(KEY_INPUT_LSHIFT)){
		if(slowSizeRate < 1 )
			slowSizeRate += 0.05;	//	スロウで表示される画像を徐々に大きくする。
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
		shotFlag = 1;	//	描画するときはショット状態の画像
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
			new P_StarBom( x, y,  45 * i, 1.5) ;	//	８方向に星型ボムを発射
		new P_StarBom(x, y, 270 - 22, 1.5) ;	//	前方２方向に星型ボム発射
		new P_StarBom(x, y, 270 + 22, 1.5) ;	//	前方だけ濃い星型ボム完成
		noHitTime = 300 ;	//	５秒間無敵
		bomEffect = 300 ;
		BE_spin = 0;
		PlaySoundMem(bomSound, DX_PLAYTYPE_BACK) ;
		bom--;
	}
	return ;
}



//	プレイヤーの移動
void Player::Move(){
	int shiftKey ; 
	shiftKey = CheckHitKey(KEY_INPUT_LSHIFT) || CheckHitKey(KEY_INPUT_RSHIFT) ;
	float tilt = 1 ;
	//斜めに移動しようとしていたら、移動力を√２にする
	if((CheckHitKey(KEY_INPUT_UP) && CheckHitKey(KEY_INPUT_RIGHT))  ||
	   (CheckHitKey(KEY_INPUT_RIGHT) && CheckHitKey(KEY_INPUT_DOWN)) ||
	   (CheckHitKey(KEY_INPUT_DOWN) && CheckHitKey(KEY_INPUT_LEFT))  ||
	   (CheckHitKey(KEY_INPUT_LEFT) && CheckHitKey(KEY_INPUT_UP)))
		tilt = ROOT2 ;
	//	無敵時間を減らす
	if(noHitTime > 0)
		noHitTime--;
	 //	プレイヤーの移動判定
	if(CheckHitKey(KEY_INPUT_UP))	//上移動
	{
		y -= (PLAYER_MOVE - (PLAYER_MOVE * shiftKey) + (shiftKey * PLAYER_SLOW_MOVE)) / tilt ;
		if(y < UP_LIMIT){
			y = UP_LIMIT ;
		}
	}
	if(CheckHitKey(KEY_INPUT_DOWN))	//下移動
	{
		y += (PLAYER_MOVE - (PLAYER_MOVE * shiftKey) + (shiftKey * PLAYER_SLOW_MOVE)) / tilt ;
		if(y > DOWN_LIMIT){
			y = DOWN_LIMIT ;
		}
	}
	if(CheckHitKey(KEY_INPUT_RIGHT))	//右移動
	{
		tiltFlag = 2;	//	右に傾いた自機を表示
		x += (PLAYER_MOVE - (PLAYER_MOVE * shiftKey) + (shiftKey * PLAYER_SLOW_MOVE)) / tilt ;
		if(x > RIGHT_LIMIT){
			x = RIGHT_LIMIT ;
		}
	}
	if(CheckHitKey(KEY_INPUT_LEFT))	//左移動
	{
		tiltFlag = 0;	//	左に傾いた自機を表示
		x -= (PLAYER_MOVE - (PLAYER_MOVE * shiftKey) + (shiftKey * PLAYER_SLOW_MOVE)) / tilt ;
		if(x < LEFT_LIMIT) {
			x = LEFT_LIMIT ;
		}
	} 
	Shot() ;	//	自機弾を発射する
	PlayerX = x;	//	他クラスから参照するためにグローバル変数にコピー
	PlayerY = y;
	return  ;
}

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
		if(p->stock == 0){	//	残機が０だったらゲームオーバー
			spiningSize = 1;
			GameOverFlag = true ;
		}
		else{				//	残機があったら初期化処理
			p->stock -= 1;
			p->alive = true ;
			p->bom = 2;
			p->power = 0;
			p->noHitTime = 180;	//	３秒間無敵
			p->flashTime = 180;	//	３秒間点滅
		}
	}
}
void CrashPlayer::Draw(){
	DrawRotaGraph(p->x, p->y, spiningSize, spin, grHandle, TRUE);
}

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
//	静的メンバ変数の初期化
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
//	リストの先頭（ダミー）
ItemBase::ItemBase(bool startInitFlg ){
	first_p = this ;
	next_p  = this ;
	rev_p   = this ;
	alive   = true ;
}
//	基本的に継承先ではこのコンストラクタを使う
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
	angle = (GetRand(170) + 190) * DegToRad;//	190度から350度の	
	GetVector(5, angle, &moveX, &moveY);	//	上昇時の向き設定
}
ItemBase::~ItemBase(){
	next_p->rev_p = rev_p ;
	rev_p->next_p = next_p	;	//	前後リストが外れないように処理	
}
void ItemBase::Move(){
	//	画面下を超えたら削除
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
		if(upCnt < 10){	//	上昇時の動作
			x += moveX ; 
			y += moveY ;
		}else{	//	上昇が終了し、下降時の動作
			y += 2 ;
		}
		//	プレイヤーが上にいるか、プレイヤーに近づいたらアイテムがプレイヤーを追尾する（アイテム蒐集）
		if(PlayerY < 200 || isHit(PlayerX, PlayerY, PLAYER_SIZE, x, y, size * 3)){
			trackingFlag = true;	
		}
	}
}


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
int ScoreItem::grHandle = NULL;
ScoreItem::ScoreItem(int in_x, int in_y, int in_score) : ItemBase(in_x, in_y){
	score = in_score;
}
void ScoreItem::Get(){
	Score += score;		//	ゲーム中のスコアに加算
}
void ScoreItem::Draw(){
	DrawGraphCenter(x, y, grHandle);
}
/////////////////////////////////////
////////ボムアイテム/////////////////
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
////////１ＵＰアイテム///////////////
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

//	静的メンバ変数の初期化
EnemyBulletBase *EnemyBulletBase::first_p = NULL ;
//	敵弾リストに登録
EnemyBulletBase::EnemyBulletBase(){
	next_p = first_p->next_p ;
	rev_p  = first_p ;
	first_p->next_p->rev_p = this ; 
	first_p->next_p = this ;
	alive = true ;
	PlaySoundMem(EB_sound, DX_PLAYTYPE_BACK) ;	//	敵弾の発射音を出す	
	grazed = false;
}
//	敵弾リストの先頭を記憶
EnemyBulletBase::EnemyBulletBase(bool initFlg){
	first_p = this ;
	rev_p = this ;
	next_p = this ;
}
EnemyBulletBase::~EnemyBulletBase(){
	rev_p->next_p = next_p	;	//	前後リストが外れないように処理	
	next_p->rev_p = rev_p	;	//
}



////////////////////////////
//	直線、サイズ中の弾	/////
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
//	直線、サイズ小の米粒弾	/////
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
//火の玉（画面上部に到達すると垂直に落ちる）
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
	//	継承元クラスのコンストラクタを使う
public:
	int moveCnt;	//	非自機狙いで移動した時間
	EB_Fire_P(float in_x, float in_y, float in_moveX, float in_moveY) :
		EB_Fire(in_x, in_y, in_moveX, in_moveY){moveCnt = 0; size = EB_FIRE_SIZE/3*2;}
	virtual void Move();
	virtual void Draw();
};
void EB_Fire_P::Move(){
	int MOVE_TIME = 120;	//	固定角度で飛んでいる時間
	moveCnt++;	//	固定角度で動いてる時間をカウント
	if(moveCnt < MOVE_TIME){	//	減速していって、止まったら自機狙い弾に変更
		x += moveX * abs(moveCnt-MOVE_TIME) / MOVE_TIME ;
		y += moveY * abs(moveCnt-MOVE_TIME) / MOVE_TIME ;
		if	(x < BULLET_LEFT_LIMIT-300 || x > BULLET_RIGHT_LIMIT+300 ||
			y < BULLET_UP_LIMIT-400 || y > BULLET_DOWN_LIMIT)
			alive = false;
	}else if(moveCnt == MOVE_TIME){	//	自機狙い弾に設定する
		float range = sqrt(moveX*moveX+moveY*moveY);
		float pAngle = GetAngle(x, y, Player::p->x, Player::p->y);	//	自機への角度を取得
		GetVector(range, pAngle, &moveX, &moveY);
	}else{	//	自機狙い弾状態の弾移動
		x += moveX;
		y += moveY;
	}
}
void EB_Fire_P::Draw(){
	DrawRotaGraph(x, y, (float)(1.0/3.0*2.0), 0, grHandle, TRUE);

}

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
//	静的メンバ変数の初期化
EnemyBase *EnemyBase::activeFirst_p = NULL ;
EnemyBase *EnemyBase::waitFirst_p = NULL ;

//	待ち状態リストの最後に追加
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
//	activeInitがtrueならアクティブリストのダミーを作成
//	falseなら待ち状態リストのダミーを作成
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
	rev_p->next_p = next_p	;	//	前後リストが外れないように処理	
	/*
	//	プレイヤーが敵を倒したらアイテムを出す
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
//	待ち状態リストからアクティブリストへ繋ぎ替え
void EnemyBase::Activation(){
	//	待ち状態リストの前後が外れないように処理
	next_p->rev_p = rev_p ;		//	待ち状態リストの
	rev_p->next_p = next_p	;	//	前後が外れないように処理	
	//	アクティブリストへ登録
	next_p = activeFirst_p ;
	rev_p  = activeFirst_p->rev_p ;
	activeFirst_p->rev_p->next_p = this ; 
	activeFirst_p->rev_p = this ;
	PlayShotSE() ;	//	生成された時の音（主に発射音）
}

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
//	静的変数の初期化
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
//	静的変数の初期化
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
	else if(nowTime > 720){	//	12秒滞在したら逃げる
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
	else if(nowTime > 420){	//	７秒滞在したら逃げる
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
int E_mStraight_bNway::GrHandle = NULL;
E_mStraight_bNway::E_mStraight_bNway(unsigned long long in_waitTime,float initX, float initY, 
	float moveSpeed, float degAngle, int in_nway,float in_bulletSpeed, float bulletAngleInterval, float in_interval )
{
	this->bulletAngleInterval = bulletAngleInterval ;	//	自機狙いnWayの間隔
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
	if(nway % 2 == 1){	//	偶数弾の場合
		float angle = GetAngle(x, y, Player::p->x, Player::p->y);
		float BMoveX, BMoveY;
		GetVector(bulletSpeed, angle, &BMoveX, &BMoveY); 
		new EB_Small(x, y, BMoveX, BMoveY);	//	自機狙い弾を撃つ
		int n = nway / 2;	//	自機狙いの隣に何発撃つか
		for(int i = 1; n >= i; i++){
			float angleN ;	//	実際に撃つ角度
			angleN = angle - (bulletAngleInterval * DegToRad * (float)i) ; 
			GetVector(bulletSpeed, angleN ,&BMoveX, &BMoveY) ;
			new EB_Small(x, y, BMoveX, BMoveY);
			angleN = angle + (bulletAngleInterval * DegToRad * (float)i) ;
			GetVector(bulletSpeed, angleN ,&BMoveX, &BMoveY) ;
			new EB_Small(x, y, BMoveX, BMoveY);
		}
	}
	else{	//	偶数弾の場合。最低でも2発発射されるはず。
		float angle[2] ;
		float BMoveX, BMoveY;
		angle[0] = GetAngle(x, y, Player::p->x, Player::p->y);
		angle[1] = angle[0] + bulletAngleInterval * DegToRad / 2 ;
		angle[0] = angle[0] - bulletAngleInterval * DegToRad / 2 ;
		GetVector(bulletSpeed, angle[0], &BMoveX, &BMoveY) ;
		new EB_Small(x, y, BMoveX, BMoveY) ;
		GetVector(bulletSpeed, angle[1], &BMoveX, &BMoveY) ;
		new EB_Small(x, y, BMoveX, BMoveY) ;
		int n = (nway / 2) - 1 ;	//	偶数弾の隣に何発撃つか
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
////敵ボスの基底クラス//////////////////
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
	float hpGage = hp / maxHp; //	残りHPの割合
	DrawExtendGraphF(5, 3, 380*hpGage, 8, HpHandle, TRUE);
}

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

	if(y == 1){	//	ボスが出現したらBGMを変える
		DeleteSoundMem(mainBGM);
		mainBGM = LoadSoundMem(BOSS1_BGM);
		PlaySoundMem(mainBGM, DX_PLAYTYPE_LOOP);
	}
	float X, Y;
	nowInterval = (nowInterval + 1) % interval ;
	bInterval = (bInterval + 1) % 240;
	if(y < 100)
		y++;
	if(nowInterval == 0){	//	弾を撃つ間隔
		if(hp < (maxHp / 3) && (bInterval == 0 || bInterval == 30)){	//	HPが３分の１以下、円に展開して自機
			for(int i = 0; i < 40; i++){
				GetVector(3, i*9*DegToRad, &X, &Y);
				new EB_Fire_P(x, y, X, Y);
			}	
		}
		else if(hp < (maxHp / 3 * 2) && bInterval == 0){	//	HPが３分の２以下、自機狙い弾を追加
			float V = GetAngle(x, y, Player::p->x, Player::p->y);
			GetVector(5, V, &X, &Y);
			new EB_Fire(x, y, X, Y);
		}
		//	基本弾幕（HPに関係なく出る）
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
systemTime::systemTime(int setTime){
	initTime(setTime) ;
}
void systemTime::initTime(int setTime){	//	基本は６０を入れる
	remainingTime = setTime * 1000 ;
	revTime = GetNowCount() ;	//	時間計測のため、前時間をセット
}
void systemTime::Move(){
	nowTime = GetNowCount() ;
	remainingTime -= (nowTime - revTime) ;
	revTime = nowTime ;
}
void systemTime::Draw(int posX, int posY){
	int output = remainingTime / 1000;	//	秒単位に直す
	DrawFormatString(posX, posY, GetColor(255,255,255), "残り時間：%d秒", output) ;
}
void systemTime::revRetime(){
	revTime = GetNowCount() ;
}

