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

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	SetGraphMode(WINDOW_WIDTH_SVGA, WINDOW_HEIGHT_SVGA, 32, FPS_60);
	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	// 画面を描画する先をバックバッファに設定する→バックバッファに全て書き出してからフロントバッファにデータを送信するため
	SetDrawScreen(DX_SCREEN_BACK);

	// フレーム数調整用
	int time;

	// プレイヤー画像の読み込み（一応分割）
	LoadDivGraph("Res/Image/Player_1.png", 5, 5, 1, 32, 32, PlayerSpriteHandle);
	// エネミー画像の読み込み（一応分割）
	LoadDivGraph("Res/Image/Enemy.png", 10, 10, 1, 32, 32, EnemySpriteHandle);
	// プレイヤー弾の画像の読み込み（分割）
	LoadDivGraph("Res/Image/Bullet_1.png", PLAYER_BULLET_ANIMATION_MAX, 4, 1, 32, 32, PlayerBulletAnimation);
	// 敵弾の画像の読み込み（分割）
	LoadDivGraph("Res/Image/Bullet_3.png", ENEMY_BULLET_ANIMATION_MAX, 4, 1, 16, 16, EnemyBulletAnimations);

	// 背景画像の読み込み（1枚絵なので枚数分読み込む）
	BackGroundImages[BACK_GROUND_1] = LoadGraph("Res/Image/Background_back.png");
	BackGroundImages[BACK_GROUND_2] = LoadGraph("Res/Image/Background_middle.png");
	BackGroundImages[BACK_GROUND_3] = LoadGraph("Res/Image/Background_front.png");

	// プレイヤーの初期化
	player.Init(PlayerSpriteHandle[2], true, 200, 200, 16.0f);
	// エネミーの初期化
	enemy.Init(EnemySpriteHandle[0], true, WINDOW_WIDTH_SVGA / 2 - 16, 30.0f, 16.0f);
	// 背景画像の初期化
	BackGrounds[BACK_GROUND_1].Init(BackGroundImages[BACK_GROUND_1], -1.0f);
	BackGrounds[BACK_GROUND_2].Init(BackGroundImages[BACK_GROUND_2], -5.0f);
	BackGrounds[BACK_GROUND_3].Init(BackGroundImages[BACK_GROUND_3], -20.0f);
	// プレイヤー弾の初期化
	for (int i = 0; i < PLAYER_BULLET_MAX; i++) {
		player.bullets[i].Init(PlayerBulletAnimation, false, 0, 0, 32.0f, PLAYER_BULLET_ANIMATION_MAX);
	}
	// メインループ
	while (1) {

		// ミリ秒単位の現在時刻を得る
		time = GetNowCount();

		// 更新
		Update();

		// 描画
		Render();

		// バックバッファの書き込みが終わったらフロントバッファへデータを転送する。
		ScreenFlip();

		// このループの処理が指定秒間フレームの処理時間より早く終わった場合は処理を待つ
		while (1) {
			if (GetNowCount() - time >= 1000 / FPS_60) {
				break;
			}
		}

		// 閉じるボタンが押されるなどのウィンドウメッセージを受信したらループを抜ける
		if (ProcessMessage() == -1) {
			break;
		}

	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}

/// <summary>
/// フレームのアップデート処理
/// </summary>
void Update() {

	scr++;

	// 入力を更新する（このフレームでどのキーが押されているか情報を更新する。）
	input.Update();
	// プレイヤーの情報を更新する（上のキー情報を利用するなどしてこのフレームでプレイヤーをどこに描画するか確定する）
	player.Update();
	// エネミーの情報を更新する。
	enemy.Update(player.gameObject.x, player.gameObject.y);
	// 背景の描画位置を更新する（動いているように見せるため、背景画像の位置を動かす）
	for (int i = 0; i < BACK_GROUND_IMAGE_MAX; i++) {
		BackGrounds[i].Update(scr);
	}
}

/// <summary>
/// フレームのレンダリング処理
/// Update()関数で決定された値をもとにその位置に描画を行う
/// </summary>
void Render() {

	// レンダリングする前に前フレームの画像を消す
	ClearDrawScreen();

	// 全ての背景画像に対して描画を行う
	for (int i = 0; i < BACK_GROUND_IMAGE_MAX; i++) {
		BackGrounds[i].Render();
	}
	// プレイヤーの描画を行う
	player.Render();
	//エネミーの描画を行う
	enemy.Render();
}
