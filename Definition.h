#pragma once


#include <DxLib.h>
#include <string>

#define WINDOW_WIDTH_HD		(1280)
#define WINDOW_HEIGHT_HD		(720)
#define WINDOW_WIDTH_FHD	(1920)
#define WINDOW_HEIGHT_FHD	(1080)

#define FPS_60				(60)

//	色
const unsigned int red = GetColor(255, 0, 0);
const unsigned int green = GetColor(0, 255, 0);
const unsigned int blue = GetColor(0, 0, 255);
const unsigned int magenta = GetColor(255, 0, 255);
const unsigned int cyan = GetColor(0, 255, 255);
const unsigned int yellow = GetColor(255, 255, 0);
const unsigned int white = GetColor(255, 255, 255);
const unsigned int black = GetColor(0, 0, 0);

/// <summary>
/// ゲームオブジェクト構造体（これをベースとする）
/// </summary>
struct GameObject {
	bool isVisible;					// 表示・非表示
	int image;						// 画像ハンドラ
	int width, height;				// サイズ
	float x, y;						// 座標
	float cx, cy;					// 中心座標
	float radius;					// 半径

	void Init(int _image, bool _visible = true, float _x = 0.0f, float _y = 0.0f, float _radius = 0.0f) {
		// 表示フラグの初期化
		isVisible = _visible;
		// 画像ハンドルの初期化
		image = _image;
		// 画像サイズの初期化
		GetGraphSize(image, &width, &height);
		// 座標の初期化
		x = _x;
		y = _y;
		// 中央座標の初期化（指定された座標＋イメージのサイズの半分（中央）
		cx = _x + width * 0.5f;
		cx = _y + height * 0.5f;
		// 半径の初期化
		radius = _radius;
	}
};

struct Player {
	GameObject gameObject;
	float moveX, moveY;

	void Init(int _image, bool _visible = true, float _x = 0.0f, float _y = 0.0f, float _radius = 0.0f) {

	}
};
