#include "DxLib.h"
#include "Definition.h"

const int InputAssign[InputMax] = {
	KEY_INPUT_W,
	KEY_INPUT_A,
	KEY_INPUT_S,
	KEY_INPUT_D,

	KEY_INPUT_SPACE,
	KEY_INPUT_Z,

	KEY_INPUT_ESCAPE,
};

int PlayerSpriteHandle[5];
int PlayerBulletAnimation[PLAYER_BULLET_ANIMATION_MAX];

int BackGroundImages[BACK_GROUND_IMAGE_MAX];

BackGround BackGrounds[BACK_GROUND_IMAGE_MAX];
EnemyWave enemyWaves;

Input input;
Player player;
Enemy enemys[ENEMY_MAX];

int EnemySpriteHandle[5];
int EnemyBulletAnimations[ENEMY_BULLET_ANIMATION_MAX];

int scr;

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
	//for (int i = -1; i < ENEMY_MAX - 1; i++) {
	//	enemys[i + 1].Init(EnemySpriteHandle[0], true, (WINDOW_WIDTH_SVGA / 2) + 200 * i - 16, 30.0f, 16.0f);
	//}
	// 背景画像の初期化
	BackGrounds[BACK_GROUND_1].Init(BackGroundImages[BACK_GROUND_1], -1.0f);
	BackGrounds[BACK_GROUND_2].Init(BackGroundImages[BACK_GROUND_2], -5.0f);
	BackGrounds[BACK_GROUND_3].Init(BackGroundImages[BACK_GROUND_3], -20.0f);
	// プレイヤー弾の初期化
	for (int i = 0; i < PLAYER_BULLET_MAX; i++) {
		player.bullets[i].Init(PlayerBulletAnimation, false, 0, 0, 32.0f, PLAYER_BULLET_ANIMATION_MAX);
	}
	// エネミー弾の初期化
	//for (int j = 0; j < ENEMY_MAX; j++) {
	//	for (int i = 0; i < ENEMY_BULLET_MAX; i++) {
	//		enemys[j].bullets[i].Init(EnemyBulletAnimations, false, 0, 0, 16.0f, ENEMY_BULLET_ANIMATION_MAX);
	//	}
	//}
	enemyWaves.Init();
	enemyWaves.Spawn();

	GameSceneType currentScene = InGame;
	GameSceneType nextScene = InGame;


	// メインの処理内で初期化しないとグローバル関数が初期化されてる保証がない
	scr = 0;

	// メインループ
	while (1) {

		// ミリ秒単位の現在時刻を得る
		time = GetNowCount();

		// ESCキーが押されたら画面を閉じる（メインループを抜ける
		if (input.IsDown(ESC)) {
			break;
		}

		switch (currentScene)
		{
		case Title:
			currentScene = Title;
			break;
		case InGame:

			currentScene = InGame;

			Init();

			// 更新
			Update();

			CollisionCheck();

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
			break;
		case Result:
			currentScene = Result;
			break;
		default:
			break;
		}


		// 閉じるボタンが押されるなどのウィンドウメッセージを受信したらループを抜ける
		if (ProcessMessage() == -1) {
			break;
		}

	}

	//	=================================================================
	//		ゲームの解放処理
	//	=================================================================

	//	画像ハンドルの解放
	// TODO: for文でやるしかないとDXLIBリファレンスには書いてあった
	// delete[] PlayerSpriteHandle;
	// delete[] EnemySpriteHandle;
	//	画像ハンドルの解放
	// delete[] PlayerBulletAnimation;
	// delete[] EnemyBulletAnimations;

	// delete[] BackGroundImages;

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}

// ゲーム初期化部
void Init() {
	// 画面上の敵の表現数をとりあえず3にする
	for (int i = -1; i < ENEMY_MAX - 1; i++) {
		if (!enemys[i + 1].gameObject.isVisible) {
			enemys[i + 1].Init(EnemySpriteHandle[0], true, (WINDOW_WIDTH_SVGA / 2) + 200 * i, 0, 16.0f);
		}
	}
}

/// <summary>
/// フレームのアップデート処理
/// </summary>
void Update() {

	// 背景スライド用のカウンタ
	scr++;

	//// 背景の描画位置を更新する（動いているように見せるため、背景画像の位置を動かす）
	for (int i = 0; i < BACK_GROUND_IMAGE_MAX; i++) {
		BackGrounds[i].Update(scr);
	}

	// 入力を更新する（このフレームでどのキーが押されているか情報を更新する。）
	input.Update();
	// プレイヤーの情報を更新する（上のキー情報を利用するなどしてこのフレームでプレイヤーをどこに描画するか確定する）
	player.Update();
	// エネミーの情報を更新する。
	//for (int i = 0; i < ENEMY_MAX; i++) {
	//	enemys[i].Update(player.gameObject.cx, player.gameObject.cy);
	//}

	enemyWaves.Update(player.gameObject.x, player.gameObject.y);
}


void CollisionCheck() {
	// プレイヤーの弾と敵の判定
	for (int i = 0; i < PLAYER_BULLET_MAX; i++) {
		for (int j = 0; j < ENEMY_MAX; j++) {
			if(CheckHitCircle(&player.bullets[i].gameObject,&enemyWaves.enemies[j].gameObject)){
				player.bullets[i].gameObject.isVisible = false;
				enemyWaves.enemies[j].gameObject.isVisible = false;
			}
		}
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
	//for (int i = 0; i < ENEMY_MAX; i++) {
	//	enemys[i].Render();
	//}
	enemyWaves.Render();

	DrawFormatString(0, 0, red, "%f", enemyWaves.enemies[0].gameObject.y);
	DrawFormatString(0, 20, red, "%f", enemyWaves.enemies[0].gameObject.x);
}
