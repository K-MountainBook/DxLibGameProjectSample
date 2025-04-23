#pragma once


#include <DxLib.h>
#include <string>

#define WINDOW_WIDTH_HD		(1280)
#define WINDOW_HEIGHT_HD		(720)
#define WINDOW_WIDTH_FHD	(1920)
#define WINDOW_HEIGHT_FHD	(1080)

#define FPS_60				(60)

//	�F
const unsigned int red = GetColor(255, 0, 0);
const unsigned int green = GetColor(0, 255, 0);
const unsigned int blue = GetColor(0, 0, 255);
const unsigned int magenta = GetColor(255, 0, 255);
const unsigned int cyan = GetColor(0, 255, 255);
const unsigned int yellow = GetColor(255, 255, 0);
const unsigned int white = GetColor(255, 255, 255);
const unsigned int black = GetColor(0, 0, 0);

/// <summary>
/// �Q�[���I�u�W�F�N�g�\���́i������x�[�X�Ƃ���j
/// </summary>
struct GameObject {
	bool isVisible;					// �\���E��\��
	int image;						// �摜�n���h��
	int width, height;				// �T�C�Y
	float x, y;						// ���W
	float cx, cy;					// ���S���W
	float radius;					// ���a

	void Init(int _image, bool _visible = true, float _x = 0.0f, float _y = 0.0f, float _radius = 0.0f) {
		// �\���t���O�̏�����
		isVisible = _visible;
		// �摜�n���h���̏�����
		image = _image;
		// �摜�T�C�Y�̏�����
		GetGraphSize(image, &width, &height);
		// ���W�̏�����
		x = _x;
		y = _y;
		// �������W�̏������i�w�肳�ꂽ���W�{�C���[�W�̃T�C�Y�̔����i�����j
		cx = _x + width * 0.5f;
		cx = _y + height * 0.5f;
		// ���a�̏�����
		radius = _radius;
	}
};

struct Player {
	GameObject gameObject;
	float moveX, moveY;

	void Init(int _image, bool _visible = true, float _x = 0.0f, float _y = 0.0f, float _radius = 0.0f) {

	}
};
