#pragma once
//---------------------------------------
//	�Q�[���S�ʂŎg���ϐ��A�֐��́u�錾�v
//---------------------------------------

// �萔 -----------------------------------------
const float PLAYER_SIZE = 32;
const float BLOCK_SIZE = 32;

const float ENEMY_SIZE = 32;

const float GRAVITY_ACCEL = 0.49f;
const float MAX_SPEED = 6.0f;

const int PLAYER_HP = 30;

const int TAMA_MAX = 3;
// �ϐ�(�g�p�錾)-------------------------------------------
// �L�[���͏��
extern char keyState[256];		// ���݂̃L�[���͏��
extern char preKeyState[256];	// �P�t���[���O�̃L�[���͏��
struct Vector2
{
	float x;
	float y;
};

// �\���� ----------------------------------------
// �I�u�W�F�N�g�f�[�^
struct Object
{
	// ���W
	float posX;
	float posY;
	//�������p�|�W�V����
	float initPosX;
	float initPosY;

	Vector2 vector2;

	float drawPosX;
	float drawPosY;

	// �T�C�Y
	float sizeX;
	float sizeY;

	// ���x
	float velocityX;
	float velocityY;

	//����
	Vector2 direction;

	// �ړ��X�s�[�h
	float moveSpeed;
	//���������
	float accelePower;

	//�摜
	int image;
	int image2;
	int DmgImage;
	int DmgImage2;
	int DmgImage3;
	int EndImage;
	int houseImage;

	//���]�摜
	int Left_image;
	int Left_DmgImage;

	bool DrawFlag;
	bool DamageDrawFlag;

	//�摜���]
	bool InversionFlag;

	//�ω������Ȃ�
	bool NotChangeFlag;

	//�G�l�~�[
	int intervalCount;
	int jumpCount;
	bool isJumpFlag;
	float jumpPower;
	//�ݒu����t���O
	bool isGround;

	//���̓t���O
	bool inputFlag;
	//�����t���O
	bool acceleFlag;

	bool isDamage;

	//�e
	bool ShotFlag;
	bool Left_ShotFlag;

	int count;

	//�_���[�W�t���O
	bool Dmg_Flag;

	//�_���[�W�J�E���g
	int Dmg_count;

	//	HP
	int hitpoint;
	int hpCount;
	bool HitFlag;

};


// �֐��錾 ----------------------------------------
// �Q�[���̏�����
void Initialize();

// �A�N�V�����Q�[���̃��C�����[�v
void ActionMain();

// �X�V
void Update();
void UpdateInput();

// �`��
void Draw();

// ��`�̓����蔻��
bool IsHit(Object obj1, Object obj2);

bool IsHit(float _posX1, float _posY1,
		   float _sizeX1, float _sizeY1,
		   float _posX2, float _posY2,
		   float _sizeX2, float _sizeY2);

//�Q�[�����n�܂��Ă��邩�ǂ���

void HitUp(Object& obj);
void HitDown(Object& obj);


float Start(int obj);

