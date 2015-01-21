
#define MODEL_DIR "model\\"

#define RESTITUTE 0.4f // 反発係数
#define GRAVITY 9.8f	// 重力加速度 9.8m/s^2 masuda

#define XORG 0.0f
#define YORG 0.0f
#define ZORG 0.0f

#define POLYGON_NUM 20
#define PLAYER_Z_POSITION 2.0f	//	プレイヤーがいる位置
#define PLAYER_Y_POSITION 1.0f	//	プレイヤーがいる位置のオフセットy masuda
#define PLAYER_ALPHA 128		//	プレイヤーのアルファ値

#define PLAYER_X_MOVE_SCALE	3.0f//	プレイヤーがX方向に移動するときの倍率
#define PLAYER_Y_MOVE_SCALE 3.0F//	プレイヤーがY方向に移動するときの倍率
#define PLAYER_Z_MOVE_SCALE 3.0f//	プレイヤーがZ方向に移動するときの倍率

#define RIGHT_WALL_X	8.0f	//	右の壁の位置
#define LEFT_WALL_X		-5.0F	//	左の壁の位置

#define WINDOWSIZE_X 1024
#define WINDOWSIZE_Y 768
#define HAND_SIZE (0.4f)

#define DRUM_COLLISION_SCALE 1.4f	//	ドラム缶と手が接触した際の力の倍率
#define CAMERA_SHAKE_K	0.05f	// カメラが振動するときの乗数

#define RANKING_MAX 1000	//	ランキングの最大数
#define RANKING_SCORE_POSITION_X	320	//	ランキングスコアの左端の位置

#define SCREENSHOT_NUM 60	//	６０秒間に保存するスクリーンショットの枚数

#ifdef MAIN
//-----------------------------------------------------
VECTOR CameraPosition;	// カメラの位置 masuda
VECTOR CameraShake;	// カメラの位置の揺れ masuda

//	kinectのスケルトン情報が更新されたか確認するためのハンドル
HANDLE SkeletonEv ;
NUI_SKELETON_FRAME playerSkeleton ; //	スケルトン情報が格納される構造体
//	kinectのカラーカメラ情報が更新されたか確認するためのハンドル
HANDLE ColorImageEv ;
const NUI_IMAGE_FRAME *ImageFrame = NULL ;
HANDLE ColorImageStream ;	//	RGBカメラのハンドル


bool rKey, lKey, uKey, dKey;
bool kinectUpdateFlag ;	//	kinect情報が更新されたらtrue
bool SkeletonTracking = false;	//	プレイヤーが認識されているか。trueで認識済み

int RevTime = 0;			//	時間を測るため、前時間を保存
int NowTime = 0;				//	現在時間を保存。ListIteratorクラスの中で更新。
int IntervalTime = 0;			//	上の２つを使って経過時間を入れる

int circleShadow;	//	丸影のハンドル

//	力を判定、どのぐらい凹むか判定をする
float drumcanStateBasis[] = {0.5f, 1.0f, 5.0f, 999999999999.0f};	//	ドラム缶が凹む基準

int skyHandle;	// 空のハンドル 20110904 masuda

int ScreenShotCnt = 0;	//	テスト用

#else
//-----------------------------------------------------

extern VECTOR CameraPosition;	// カメラの位置 masuda
extern VECTOR CameraShake;	// カメラの位置の揺れ masuda

//	kinectのスケルトン情報が更新されたか確認するためのハンドル
extern HANDLE SkeletonEv ;
extern NUI_SKELETON_FRAME playerSkeleton ; //	スケルトン情報が格納される構造体
//	kinectのカラーカメラ情報が更新されたか確認するためのハンドル
extern HANDLE ColorImageEv ;
extern const NUI_IMAGE_FRAME *ImageFrame = NULL ;
extern HANDLE ColorImageStream ;	//	RGBカメラのハンドル

extern bool rKey, lKey, uKey, dKey;
extern bool kinectUpdateFlag ;	//	kinect情報が更新されたらtrue
extern bool SkeletonTracking;	//	プレイヤーが認識されているか。trueで認識済み

extern int RevTime;			//	時間を測るため、前時間を保存
extern int NowTime;				//	現在時間を保存。ListIteratorクラスの中で更新。
extern int IntervalTime;			//	上の２つを使って経過時間を入れる

extern int circleShadow;	//	丸影のハンドル

//	力を判定、どのぐらい凹むか判定をする
extern float drumcanStateBasis[];	//	ドラム缶が凹む基準

extern int skyHandle;	// 空のハンドル 20110904 masuda

extern int ScreenShotCnt;	//	テスト用

#endif

