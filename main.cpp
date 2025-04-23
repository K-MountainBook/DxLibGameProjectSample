#include "DxLib.h"
#include "Definition.h"

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{


	SetGraphMode(WINDOW_WIDTH_HD, WINDOW_HEIGHT_HD, 32, FPS_60);
	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	// ��ʂ�`�悷�����o�b�N�o�b�t�@�ɐݒ肷�遨�o�b�N�o�b�t�@�ɑS�ď����o���Ă���t�����g�o�b�t�@�Ƀf�[�^�𑗐M���邽��
	SetDrawScreen(DX_SCREEN_BACK);

	// �t���[���������p
	int time;

	// ���C�����[�v
	while (1) {

		// �~���b�P�ʂ̌��ݎ����𓾂�
		time = GetNowCount();


		// �o�b�N�o�b�t�@�̏������݂��I�������t�����g�o�b�t�@�փf�[�^��]������B
		ScreenFlip();

		// ���̃��[�v�̏������w��b�ԃt���[���̏������Ԃ�葁���I������ꍇ�͏�����҂�
		while (1) {
			if (GetNowCount() - time >= 1000 / FPS_60) {
				break;
			}
		}

	}

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}