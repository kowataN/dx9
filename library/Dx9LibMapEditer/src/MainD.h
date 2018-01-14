/******************************************************************************/
//	�}�b�v�G�f�B�^�[
//		�w�b�_�t�@�C��
/******************************************************************************/

//----------------------------------------------------------------------------//
//	�f�t�@�C��
//----------------------------------------------------------------------------//

/////////////////////////////////////
//	�ԍ�

//	�e�N�X�`��
#define	TEX_SYSTEM	0		//	�V�X�e��
#define	TEX_BMP		5		//	bmp�t�@�C��

//	�p�^�[��
#define	PAT_MENU	0		//	���j���[
#define	PAT_FRM		1		//	�t���[��
#define	PAT_CURSOR	5		//	�J�[�\��
#define	PAT_BMP		10		//	bmp�t�@�C��
#define	PAT_CHIP	12		//	�`�b�v
#define	PAT_MARU	15		//	��
#define	PAT_BATU	16		//	�~
#define	PAT_BRUSH	18		//	�u���V
#define	PAT_MAP		20		//	�}�b�v

//	�|���S��
#define	POL_MENU	0		//	���j���[
#define	POL_FRM		1		//	�t���[��
#define	POL_CURSOR	5		//	�J�[�\��
#define	POL_SEL		6		//	�J�[�\���i���j���[�j
#define	POL_BG		10		//	�w�i
#define	POL_BMP		15		//	bmp�t�@�C��
#define	POL_BRUSH	20		//	�u���V
#define	POL_GRID	30		//	�O���b�h
#define	POL_HIT		20		//	���E�~
#define	POL_MAP		125		//	�}�b�v

//	�e�L�X�g
enum
{
	TXT_MOUSE = 0,
	TXT_FILE_NAME,
	TXT_WIDTH,
	TXT_HEIGHT,
};

/////////////////////////////////////
//	���̑�

//	�`�b�v�T�C�Y
#define	CHIP_SIZE		16
#define	CHIP_W_NUM_MAX	6
#define	CHIP_H_NUM_MAX	12
#define	CHIP_NUM_MAX	(CHIP_W_NUM_MAX*CHIP_H_NUM_MAX)

//	�}�b�v�̍ő吔(�`�b�v��)
#define	MAP_W_MAX		300
#define	MAP_H_MAX		300

//	�ǂݍ���bmp�T�C�Y
#define	BMP_SIZE		384

//	bmp�p�^�[���T�C�Y
#define	BMP_PAT_SIZE_W	(CHIP_SIZE*CHIP_W_NUM_MAX)	//	96
#define	BMP_PAT_SIZE_H	(CHIP_SIZE*CHIP_H_NUM_MAX)	//	192

//	�y�[�W�ő吔
#define	PAGE_MAX		8

//	�`�b�v�E�C���h�E�T�C�Y
#define	CHIP_WINDOW_X	CHIP_SIZE
#define	CHIP_WINDOW_Y	164
#define	CHIP_WINDOW_W	112
#define	CHIP_WINDOW_H	355

#define	LAYER(n)		(n*10)

enum
{
	LAYER_1 = 0,	//	0	���C���[1
	LAYER_2,		//	1	���C���[2
	LAYER_3,		//	2	���C���[3
	LAYER_HIT,		//	3	���蔻��
	LAYER_MAX		//	4	�ő吔
};

#define	MAP_INDEX_LAYER1	0
#define	MAP_INDEX_LAYER2	10
#define	MAP_INDEX_LAYER3	20
#define	MAP_INDEX_HIT		30

//	�f�o�b�O�X�C�b�`
//#define	__DEBUG__
#ifdef	__DEBUG__
#define	DB_MS			100
#endif

//----------------------------------------------------------------------------//
//	�O���[�o���ϐ�
//----------------------------------------------------------------------------//

//	�_�C�A���O�p
char	Dialog[][128]=
{
	"�}�b�v�t�@�C��(*.map)\0*.map\0\0",
	"�摜�t�@�C��(*.bmp *.img)\0*.bmp\0*.img\0",
	"*.map",
	"*.bmp *.img",
	"���[�h����t�@�C�������w�肵�Ă��������B",
	"�Z�[�u����t�@�C�������w�肵�Ă�������",
};

//	�p�^�[�����W
short	PatDat[][4] =
{
	0,0,128,416,	//	���j���[�P
	128,0,16,16,	//	�J�[�\��
	128,16,40,16,	//	�X�C�b�`�g
	128,32,16,16,	//	��
	144,32,16,16,	//	�~
	160,0,16,16,	//	�u���V
	192,0,128,208,	//	���j���[�Q
};

//	�|���S���\�����W
double	PolDisp[][3] =
{
	0,0,0.6,	//	���j���[
	0,0,0.5,	//	�J�[�\��
	20,368,0.8,	//	�X�C�b�`�g
	16,164,0.8,	//	�J�[�\���i���j���[�j
};

//	�f�[�^�̓����蔻��(MapEdit)
#define	DAT_HIT_MAX	13	//	�f�[�^�̍ő吔
short	DatHit[DAT_HIT_MAX][4] =
{
	8,32,32,16,		//	save
	48,32,32,16,	//	load
	88,32,32,16,	//	bmp

	100,104,12,8,	//	����
	100,112,12,8,	//	����

	100,124,12,8,	//	������
	100,132,12,8,	//	������

	20,368,40,16,	//	���C���[�P�X�C�b�`
	68,368,40,16,	//	���C���[�Q�X�C�b�`
	20,388,40,16,	//	���C���[�R�X�C�b�`
	68,388,40,16,	//	�����蔻��X�C�b�`

	12,152,8,8,		//	��
	108,152,8,8,	//	��
};

//	�f�[�^�̓����蔻��(MapAnmEdit)
short	AnmHit[][4] =
{
	272,32,32,16,	//	load
	272,128,12,8,	//	�A�j����
	272,136,12,8,	//	�A�j����
};

//	�t�H���g�̕\�����W
#define	FONT_MAX	4	//	�t�H���g�̍ő吔
short	Font[FONT_MAX][2] =
{
	40,8,	//	�}�E�X���W
	12,80,	//	�t�@�C����
	72,106,	//	����
	72,126,	//	��
};

//	�`�b�v�p�^�[���f�[�^
short	ChipPatDat[][2] =
{
	0,0,		//	0
	0,192,		//	1
	96,0,		//	2
	96,192,		//	3
	192,0,		//	4
	192,192,	//	5
	288,0,		//	6
	288,192,	//	7
};

//----------------------------------------------------------------------------//
//	�\����
//----------------------------------------------------------------------------//

//	�t���O�p�\����
typedef	struct
{
	long	Page;		//	�y�[�W
	long	Layer;		//	���C���[�i0:���C���[1�A1:���C���[2�A2:���C���[3�A3:�����蔻��j
	long	Chip;		//	�`�b�v�ԍ�
	long	ChipBk;		//	�`�b�v�ԍ��i�o�b�N�A�b�v�j
	UChar	HitMenu;	//	���j���[���̓����蔻��i0:�Ȃ��A1:������j
	UChar	HitChipWin;	//	�`�b�v�E�C���h�E�Ƃ̓����蔻��i0:�Ȃ��A1:������j
	UChar	DispMap;	//	�}�b�v�\���t���O�i0:�ω��Ȃ��A1:�ω�����j
	UChar	LoadBmp;	//	�摜�t�@�C���ǂݍ��݃t���O
	UChar	DispGrid;	//	�O���b�h�\���t���O�i0:��\���A1:�\���j
	UChar	Brush;		//	�u���V�i0:�m�[�}���A1:�h��Ԃ��j
}FLAG_TBL;

//	�}�b�v�\����
typedef	struct
{
	long	Use;
	long	Width;
	long	Height;	//	�}�b�v�̃T�C�Y
	struct	LAYER_TBL
	{
		long	Chip[MAP_H_MAX][MAP_W_MAX];	//	�p�^�[���ԍ�
	}Layer[LAYER_MAX];
}MAP_TBL;
