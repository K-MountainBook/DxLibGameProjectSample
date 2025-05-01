#include "DxLib.h"
#include "Definition.h"

int PlayerSpriteHandle[5];

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	SetGraphMode(WINDOW_WIDTH_HD, WINDOW_HEIGHT_HD, 32, FPS_60);
	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	// 画面を描画する先をバックバッファに設定する→バックバッファに全て書き出してからフロントバッファにデータを送信するため
	SetDrawScreen(DX_SCREEN_BACK);

	// フレーム数調整用
	int time;


	LoadDivGraph("Res/Image/Player_1.png", 5, 5, 1, 32, 32, PlayerSpriteHandle);

	// メインループ
	while (1) {

		// ミリ秒単位の現在時刻を得る
		time = GetNowCount();

		Update();

		Render();

		// バックバッファの書き込みが終わったらフロントバッファへデータを転送する。
		ScreenFlip();

		// このループの処理が指定秒間フレームの処理時間より早く終わった場合は処理を待つ
		while (1) {
			if (GetNowCount() - time >= 1000 / FPS_60) {
				break;
			}
		}

		if (ProcessMessage() == -1) {
			break;
		}

	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}

void Update() {

}

void Render() {
	DrawGraph(0, 0, PlayerSpriteHandle[2], false);
}