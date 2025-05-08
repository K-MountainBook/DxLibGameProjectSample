#include "DxLib.h"
#include "Definition.h"

const int InputAssign[InputMax] = {
	KEY_INPUT_W,
	KEY_INPUT_A,
	KEY_INPUT_S,
	KEY_INPUT_D,

	KEY_INPUT_SPACE,
};

int PlayerSpriteHandle[5];
int PlayerBulletAnimation[PLAYER_BULLET_ANIMATION_MAX];

int BackGroundImages[BACK_GROUND_IMAGE_MAX];
BackGround BackGrounds[BACK_GROUND_IMAGE_MAX];

Input input;
Player player;
Enemy enemy;

int EnemySpriteHandle[5];
int EnemyBulletAnimations[ENEMY_BULLET_ANIMATION_MAX];

int scr = 0;

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	SetGraphMode(WINDOW_WIDTH_SVGA, WINDOW_HEIGHT_SVGA, 32, FPS_60);
	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	// ��ʂ�`�悷�����o�b�N�o�b�t�@�ɐݒ肷�遨�o�b�N�o�b�t�@�ɑS�ď����o���Ă���t�����g�o�b�t�@�Ƀf�[�^�𑗐M���邽��
	SetDrawScreen(DX_SCREEN_BACK);

	// �t���[���������p
	int time;

	// �v���C���[�摜�̓ǂݍ��݁i�ꉞ�����j
	LoadDivGraph("Res/Image/Player_1.png", 5, 5, 1, 32, 32, PlayerSpriteHandle);
	// �G�l�~�[�摜�̓ǂݍ��݁i�ꉞ�����j
	LoadDivGraph("Res/Image/Enemy.png", 10, 10, 1, 32, 32, EnemySpriteHandle);
	// �v���C���[�e�̉摜�̓ǂݍ��݁i�����j
	LoadDivGraph("Res/Image/Bullet_1.png", PLAYER_BULLET_ANIMATION_MAX, 4, 1, 32, 32, PlayerBulletAnimation);
	// �G�e�̉摜�̓ǂݍ��݁i�����j
	LoadDivGraph("Res/Image/Bullet_3.png", ENEMY_BULLET_ANIMATION_MAX, 4, 1, 16, 16, EnemyBulletAnimations);

	// �w�i�摜�̓ǂݍ��݁i1���G�Ȃ̂Ŗ������ǂݍ��ށj
	BackGroundImages[BACK_GROUND_1] = LoadGraph("Res/Image/Background_back.png");
	BackGroundImages[BACK_GROUND_2] = LoadGraph("Res/Image/Background_middle.png");
	BackGroundImages[BACK_GROUND_3] = LoadGraph("Res/Image/Background_front.png");

	// �v���C���[�̏�����
	player.Init(PlayerSpriteHandle[2], true, 200, 200, 16.0f);
	// �G�l�~�[�̏�����
	enemy.Init(EnemySpriteHandle[0], true, WINDOW_WIDTH_SVGA / 2 - 16, 30.0f, 16.0f);
	// �w�i�摜�̏�����
	BackGrounds[BACK_GROUND_1].Init(BackGroundImages[BACK_GROUND_1], -1.0f);
	BackGrounds[BACK_GROUND_2].Init(BackGroundImages[BACK_GROUND_2], -5.0f);
	BackGrounds[BACK_GROUND_3].Init(BackGroundImages[BACK_GROUND_3], -20.0f);
	// �v���C���[�e�̏�����
	for (int i = 0; i < PLAYER_BULLET_MAX; i++) {
		player.bullets[i].Init(PlayerBulletAnimation, false, 0, 0, 32.0f, PLAYER_BULLET_ANIMATION_MAX);
	}
	// ���C�����[�v
	while (1) {

		// �~���b�P�ʂ̌��ݎ����𓾂�
		time = GetNowCount();

		// �X�V
		Update();

		// �`��
		Render();

		// �o�b�N�o�b�t�@�̏������݂��I�������t�����g�o�b�t�@�փf�[�^��]������B
		ScreenFlip();

		// ���̃��[�v�̏������w��b�ԃt���[���̏������Ԃ�葁���I������ꍇ�͏�����҂�
		while (1) {
			if (GetNowCount() - time >= 1000 / FPS_60) {
				break;
			}
		}

		// ����{�^�����������Ȃǂ̃E�B���h�E���b�Z�[�W����M�����烋�[�v�𔲂���
		if (ProcessMessage() == -1) {
			break;
		}

	}

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}

/// <summary>
/// �t���[���̃A�b�v�f�[�g����
/// </summary>
void Update() {

	scr++;

	// ���͂��X�V����i���̃t���[���łǂ̃L�[��������Ă��邩�����X�V����B�j
	input.Update();
	// �v���C���[�̏����X�V����i��̃L�[���𗘗p����Ȃǂ��Ă��̃t���[���Ńv���C���[���ǂ��ɕ`�悷�邩�m�肷��j
	player.Update();
	// �G�l�~�[�̏����X�V����B
	enemy.Update(player.gameObject.x, player.gameObject.y);
	// �w�i�̕`��ʒu���X�V����i�����Ă���悤�Ɍ����邽�߁A�w�i�摜�̈ʒu�𓮂����j
	for (int i = 0; i < BACK_GROUND_IMAGE_MAX; i++) {
		BackGrounds[i].Update(scr);
	}
}

/// <summary>
/// �t���[���̃����_�����O����
/// Update()�֐��Ō��肳�ꂽ�l�����Ƃɂ��̈ʒu�ɕ`����s��
/// </summary>
void Render() {

	// �����_�����O����O�ɑO�t���[���̉摜������
	ClearDrawScreen();

	// �S�Ă̔w�i�摜�ɑ΂��ĕ`����s��
	for (int i = 0; i < BACK_GROUND_IMAGE_MAX; i++) {
		BackGrounds[i].Render();
	}
	// �v���C���[�̕`����s��
	player.Render();
	//�G�l�~�[�̕`����s��
	enemy.Render();
}
