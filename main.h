
#define MODEL_DIR "model\\"

#define RESTITUTE 0.4f // �����W��
#define GRAVITY 9.8f	// �d�͉����x 9.8m/s^2 masuda

#define XORG 0.0f
#define YORG 0.0f
#define ZORG 0.0f

#define POLYGON_NUM 20
#define PLAYER_Z_POSITION 2.0f	//	�v���C���[������ʒu
#define PLAYER_Y_POSITION 1.0f	//	�v���C���[������ʒu�̃I�t�Z�b�gy masuda
#define PLAYER_ALPHA 128		//	�v���C���[�̃A���t�@�l

#define PLAYER_X_MOVE_SCALE	3.0f//	�v���C���[��X�����Ɉړ�����Ƃ��̔{��
#define PLAYER_Y_MOVE_SCALE 3.0F//	�v���C���[��Y�����Ɉړ�����Ƃ��̔{��
#define PLAYER_Z_MOVE_SCALE 3.0f//	�v���C���[��Z�����Ɉړ�����Ƃ��̔{��

#define RIGHT_WALL_X	8.0f	//	�E�̕ǂ̈ʒu
#define LEFT_WALL_X		-5.0F	//	���̕ǂ̈ʒu

#define WINDOWSIZE_X 1024
#define WINDOWSIZE_Y 768
#define HAND_SIZE (0.4f)

#define DRUM_COLLISION_SCALE 1.4f	//	�h�����ʂƎ肪�ڐG�����ۂ̗͂̔{��
#define CAMERA_SHAKE_K	0.05f	// �J�������U������Ƃ��̏搔

#define RANKING_MAX 1000	//	�����L���O�̍ő吔
#define RANKING_SCORE_POSITION_X	320	//	�����L���O�X�R�A�̍��[�̈ʒu

#define SCREENSHOT_NUM 60	//	�U�O�b�Ԃɕۑ�����X�N���[���V���b�g�̖���

#ifdef MAIN
//-----------------------------------------------------
VECTOR CameraPosition;	// �J�����̈ʒu masuda
VECTOR CameraShake;	// �J�����̈ʒu�̗h�� masuda

//	kinect�̃X�P���g����񂪍X�V���ꂽ���m�F���邽�߂̃n���h��
HANDLE SkeletonEv ;
NUI_SKELETON_FRAME playerSkeleton ; //	�X�P���g����񂪊i�[�����\����
//	kinect�̃J���[�J������񂪍X�V���ꂽ���m�F���邽�߂̃n���h��
HANDLE ColorImageEv ;
const NUI_IMAGE_FRAME *ImageFrame = NULL ;
HANDLE ColorImageStream ;	//	RGB�J�����̃n���h��


bool rKey, lKey, uKey, dKey;
bool kinectUpdateFlag ;	//	kinect��񂪍X�V���ꂽ��true
bool SkeletonTracking = false;	//	�v���C���[���F������Ă��邩�Btrue�ŔF���ς�

int RevTime = 0;			//	���Ԃ𑪂邽�߁A�O���Ԃ�ۑ�
int NowTime = 0;				//	���ݎ��Ԃ�ۑ��BListIterator�N���X�̒��ōX�V�B
int IntervalTime = 0;			//	��̂Q���g���Čo�ߎ��Ԃ�����

int circleShadow;	//	�ۉe�̃n���h��

//	�͂𔻒�A�ǂ̂��炢���ނ����������
float drumcanStateBasis[] = {0.5f, 1.0f, 5.0f, 999999999999.0f};	//	�h�����ʂ����ފ

int skyHandle;	// ��̃n���h�� 20110904 masuda

int ScreenShotCnt = 0;	//	�e�X�g�p

#else
//-----------------------------------------------------

extern VECTOR CameraPosition;	// �J�����̈ʒu masuda
extern VECTOR CameraShake;	// �J�����̈ʒu�̗h�� masuda

//	kinect�̃X�P���g����񂪍X�V���ꂽ���m�F���邽�߂̃n���h��
extern HANDLE SkeletonEv ;
extern NUI_SKELETON_FRAME playerSkeleton ; //	�X�P���g����񂪊i�[�����\����
//	kinect�̃J���[�J������񂪍X�V���ꂽ���m�F���邽�߂̃n���h��
extern HANDLE ColorImageEv ;
extern const NUI_IMAGE_FRAME *ImageFrame = NULL ;
extern HANDLE ColorImageStream ;	//	RGB�J�����̃n���h��

extern bool rKey, lKey, uKey, dKey;
extern bool kinectUpdateFlag ;	//	kinect��񂪍X�V���ꂽ��true
extern bool SkeletonTracking;	//	�v���C���[���F������Ă��邩�Btrue�ŔF���ς�

extern int RevTime;			//	���Ԃ𑪂邽�߁A�O���Ԃ�ۑ�
extern int NowTime;				//	���ݎ��Ԃ�ۑ��BListIterator�N���X�̒��ōX�V�B
extern int IntervalTime;			//	��̂Q���g���Čo�ߎ��Ԃ�����

extern int circleShadow;	//	�ۉe�̃n���h��

//	�͂𔻒�A�ǂ̂��炢���ނ����������
extern float drumcanStateBasis[];	//	�h�����ʂ����ފ

extern int skyHandle;	// ��̃n���h�� 20110904 masuda

extern int ScreenShotCnt;	//	�e�X�g�p

#endif

