#pragma once

#include <DxLib.h>
#include <string>

// マクロ定義
#define WINDOW_WIDTH_HD			(1280)
#define WINDOW_HEIGHT_HD		(720)
#define WINDOW_WIDTH_FHD		(1920)
#define WINDOW_HEIGHT_FHD		(1080)
#define WINDOW_WIDTH_SVGA		(800)
#define WINDOW_HEIGHT_SVGA		(600)

#define FPS_60					(60)

#define PLAYER_MOVEMENT			(5.0f)	//	プレイヤーの移動量
#define PLAYER_BULLET_MAX		(10000)	// 弾の最大数
#define PLAYER_BULLET_ANIMATION_MAX	(4)	// 弾のアニメーション枚数
#define PLAYER_BULLET_MOVEMENT	(10.0f)

#define ENEMY_BULLET_MAX		(100)
#define	ENEMY_BULLET_ANIMATION_MAX	(4)
#define ENEMY_BULLET_SPEED		(2.0f)
#define ENEMY_MAX				(3)

#define ONE_WAY_BULLETS			(1)
#define THREE_WAY_BULLETS		(3)
#define FOUR_WAY_BULLETS		(4)

// 定数定義
// 色
const unsigned int red = GetColor(255, 0, 0);
const unsigned int green = GetColor(0, 255, 0);
const unsigned int blue = GetColor(0, 0, 255);
const unsigned int magenta = GetColor(255, 0, 255);
const unsigned int cyan = GetColor(0, 255, 255);
const unsigned int yellow = GetColor(255, 255, 0);
const unsigned int white = GetColor(255, 255, 255);
const unsigned int black = GetColor(0, 0, 0);

extern const int InputAssign[];
extern struct Input input;

extern int PlayerImage;
extern int PlayerBulletImage;
extern int PlayerBulletAnimation[];

extern int EnemyImage;
extern int EnemyBulletImage;
extern int EnemyBulletAnimations[];

// プロトタイプ定義
void Init();
void Update();
void Render();

/// <summary>
/// 入力キー列挙型
/// </summary>
enum MyInput {
	W,
	A,
	S,
	D,

	SPACE,

	ESC,

	InputMax,
};

/// <summary>
/// 背景レイヤー列挙型
/// </summary>
enum BackGroundLayer {
	BACK_GROUND_1,
	BACK_GROUND_2,
	BACK_GROUND_3,

	BACK_GROUND_IMAGE_MAX,
};


/*
* C言語なので構造体の順番を守ること（コードの後に付け足した構造体は前の構造体から参照できないため）
*/
/// <summary>
/// キー入力を取り扱う構造体
/// </summary>
struct Input {
	struct KeyState {
		bool old;
		bool down;
		bool on;
		bool up;

		/// <summary>
		/// 値の初期化
		/// </summary>
		void Init() {
			old = false;
			down = false;
			on = false;
			up = false;
		}
	};
	// 有効なキーの分だけKeyStateを用意する
	struct KeyState KeyState[InputMax];

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() {
		// 有効なキー全てに対して処理を行う
		for (int i = 0; i < InputMax; i++) {
			bool now = false;
			//main.cppで定義する「InputAssign」のキー割り当てはDxLibに沿う
			now = CheckHitKey(InputAssign[i]);

			// キーの状態を更新
			KeyState[i].on = now;							// 今押されている
			KeyState[i].down = now && !KeyState[i].old;		// 今押されているが前フレームでは押されていない
			KeyState[i].up = !now && KeyState[i].old;		// 今押されていないが前フレームでは押されていた

			KeyState[i].old = now;							// 現フレームの状態を保存する
		}
	}

	// キーの情報を取得できるようにする
	// 現フレームで初めて押されたか
	bool IsDown(MyInput _input) {
		return KeyState[_input].down;
	}
	// キーが継続して押されているか
	bool IsOn(MyInput _input) {
		return KeyState[_input].on;
	}
	// 現フレームでキーが離されたか
	bool IsUp(MyInput _input) {
		return KeyState[_input].up;
	}
};

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

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="_image">画像ハンドル</param>
	/// <param name="_visible">表示フラグ</param>
	/// <param name="_x">X座標</param>
	/// <param name="_y">Y座標</param>
	/// <param name="_radius">判定半径</param>
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

/// <summary>
/// 背景用の構造体
/// </summary>
struct BackGround {
	GameObject gameObject;
	float scrollSpeed;
	float currentScroll;

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="_image">画像ハンドル</param>
	/// <param name="_speed">スクロールスピード</param>
	void Init(int _image, float _speed) {
		gameObject.Init(_image);
		scrollSpeed = _speed;
		currentScroll = 0.0f;
	}

	/// <summary>
	/// 更新関数
	/// </summary>
	/// <param name="_scroll">スクロール量</param>
	void Update(float _scroll) {
		if (!gameObject.isVisible) {
			return;
		}

		// スクロール量を画像の高さで割り余りを求める
		currentScroll = (int)(_scroll * scrollSpeed) % gameObject.height;
		// 余りが画像の高さを超過したらスクロール量を0に戻す
		if (currentScroll > gameObject.height) {
			currentScroll = 0.0f;
		}
	}

	/// <summary>
	/// 描画関数
	/// </summary>
	void Render() {
		if (!gameObject.isVisible) {
			return;
		}

		// (0,0)の座標に画像を表示する。ただしスクロールして上にずれたものを表示
		DrawRectGraph(
			0, 0,									// 描画する「画面」の座標
			0, currentScroll,						// 描画する「画像」の左上の座標
			gameObject.width, gameObject.height,
			gameObject.image,
			true
		);
		// 上のDrawGraphでずれて背景がなくなった部分に画像の上部分を描画する
		DrawRectGraph(
			0, 0,									// 描画する「画面」の座標
			0, gameObject.height + currentScroll,	// 描画する「画像」の左上の座標
			gameObject.width, gameObject.height,
			gameObject.image,
			true
		);

	}

};

/// <summary>
/// 弾用の構造体（敵味方双方で利用可能）
/// 弾を利用するキャラクタの構造体に変数として定義する。
/// </summary>
struct Bullet {
	GameObject gameObject;
	float moveX, moveY;
	int animations[PLAYER_BULLET_ANIMATION_MAX];
	int animationCounter;
	int currentAnimation;
	int animationCount;

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="_images">画像ハンドル</param>
	/// <param name="_visible">表示フラグ</param>
	/// <param name="_x">X座標</param>
	/// <param name="_y">Y座標</param>
	/// <param name="_radius">判定半径</param>
	/// <param name="_animationCount">画像のアニメーション数</param>
	void Init(int _images[], bool _visible = true, float _x = 0.0f, float _y = 0.0f, float _radius = 0.0f, int _animationCount = -1) {
		gameObject.Init(_images[0], _visible, _x, _y, _radius);
		animationCount = _animationCount;

		// アニメーション画像の読み込み
		for (int i = 0; i < animationCount; i++) {
			animations[i] = _images[i];
		}

		// 各種変数の初期化
		animationCounter = 0;
		currentAnimation = 0;
		moveX = moveY = 0;
	}

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() {
		if (!gameObject.isVisible) {
			return;
		}

		animationCounter++;

		// アニメーションカウンタが一定量を超えた場合に次のアニメーションに遷移するようにする
		if (animationCounter % 10 == 0) {
			animationCounter = 0;
			currentAnimation++;

			// アニメーションが最後まで行ったら、はじめに戻す
			if (currentAnimation >= animationCount) {
				currentAnimation = 0;
			}
		}

		// 移動させる
		gameObject.x += moveX;
		gameObject.y += moveY;

		// 移動とともに中心座標も更新する。
		gameObject.cx = gameObject.x + gameObject.width * 0.5f;
		gameObject.cy = gameObject.y + gameObject.height * 0.5f;

		// 画面外へ出た場合、非表示にする。
		if ((gameObject.x + gameObject.width < 0) ||
			(WINDOW_WIDTH_SVGA < gameObject.x) ||
			(gameObject.y + gameObject.height < 0) ||
			(WINDOW_HEIGHT_SVGA < gameObject.y)) {
			gameObject.isVisible = false;
		}
	}

	/// <summary>
	/// 描画関数
	/// </summary>
	void Render() {
		if (!gameObject.isVisible) {
			return;
		}

		// Update関数で指定したアニメーションを描画する
		DrawGraph(gameObject.x, gameObject.y, animations[currentAnimation], true);
	}
};

/// <summary>
/// プレイヤーの構造体
/// </summary>
struct Player {
	GameObject gameObject;
	float moveX, moveY;

	Bullet bullets[PLAYER_BULLET_MAX];	// 画面内に存在できる弾の構造体
	int shotInterval;				// カウント用変数
	const int SHOT_INTERVAL = 10;	// 発射間隔定数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="_image">画像ハンドル</param>
	/// <param name="_visible">表示フラグ</param>
	/// <param name="_x">X座標</param>
	/// <param name="_y">Y座標</param>
	/// <param name="_radius">判定半径</param>
	void Init(int _image, bool _visible = true, float _x = 0.0f, float _y = 0.0f, float _radius = 0.0f) {
		gameObject.Init(_image, _visible, _x, _y, _radius);
		moveX = moveY = 0.0f;

		shotInterval = SHOT_INTERVAL;
	}

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() {

		// 発射した弾の更新処理
		for (int i = 0; i < PLAYER_BULLET_MAX; i++) {
			bullets[i].Update();
		}

		if (!gameObject.isVisible) {
			return;
		}

		// 移動処理
		Move();

		// 弾の発射処理
		Shoot();
	}

	/// <summary>
	/// 描画関数
	/// </summary>
	void Render() {

		// 発射した弾の描画処理
		// Playerが非表示の際でも弾だけは動くようにしたいので、非表示時の処理より前に記述
		for (int i = 0; i < PLAYER_BULLET_MAX; i++) {
			bullets[i].Render();
		}

		if (!gameObject.isVisible) {
			return;
		}
		// 自機の表示
		DrawGraph(gameObject.x, gameObject.y, gameObject.image, TRUE);
	}

	/// <summary>
	/// 移動関数
	/// </summary>
	void Move() {
		moveX = moveY = 0.0f;

		// 移動の処理
		if (input.IsOn(A)) {
			moveX = -PLAYER_MOVEMENT;
		}
		if (input.IsOn(D)) {
			moveX = PLAYER_MOVEMENT;
		}
		if (input.IsOn(W)) {
			moveY = -PLAYER_MOVEMENT;
		}
		if (input.IsOn(S)) {
			moveY = PLAYER_MOVEMENT;
		}

		// 画面端の場合それ以上動かないように制御する
		if (gameObject.x + moveX < 0) {		// 左
			gameObject.x = 0;
			moveX = 0;
		}
		if (gameObject.x + moveX > WINDOW_WIDTH_SVGA - gameObject.width) {		// 左
			gameObject.x = WINDOW_WIDTH_SVGA - gameObject.width;
			moveX = 0;
		}
		if (gameObject.y + moveY < 0) {		// 上
			gameObject.y = 0;
			moveY = 0;
		}
		if (gameObject.y + moveY > WINDOW_HEIGHT_SVGA - gameObject.height) {		// 下
			gameObject.y = WINDOW_HEIGHT_SVGA - gameObject.height;
			moveY = 0;
		}

		// 最終的な移動座標を反映させる
		gameObject.x += moveX;
		gameObject.y += moveY;

		// 中心座標も更新する
		gameObject.cx = gameObject.x + gameObject.width * 0.5f;		// 現在の座標にスプライトの幅の半分を足す
		gameObject.cy = gameObject.y + gameObject.height * 0.5f;		// 現在の座標にスプライトの幅の半分を足す

	}

	/// <summary>
	/// 射撃関数
	/// </summary>
	void Shoot() {
		// インターバルの更新
		if (shotInterval < SHOT_INTERVAL) {
			shotInterval++;
		}

		// インターバルを超えている場合スペースを押すと弾が発射する
		if (input.IsOn(SPACE) && shotInterval >= SHOT_INTERVAL) {
			for (int i = 0; i < PLAYER_BULLET_MAX; i++) {
				// 画面内で既に表示されている弾については発射処理は行わない
				if (bullets[i].gameObject.isVisible) {
					continue;
				}

				// インターバルをReset
				shotInterval = 0;
				// 発射する弾の初期化を行う
				bullets[i].Init(PlayerBulletAnimation, true, gameObject.x, gameObject.y, gameObject.radius, PLAYER_BULLET_ANIMATION_MAX);

				bullets[i].moveX = 0;
				bullets[i].moveY = -PLAYER_BULLET_MOVEMENT;
				break;
			}
		}

	}
};

/// <summary>
/// 敵構造体
/// </summary>
struct Enemy {
	GameObject gameObject;
	float moveX, moveY;
	float playerX, playerY;

	int sinMoveDefault;

	Bullet bullets[ENEMY_BULLET_MAX];
	float IntervalCnt;
	const float SHOT_INTEVAL = 50.0f;

	/// <summary>
	/// 更新関数
	/// </summary>
	/// <param name="_image">画像ハンドル</param>
	/// <param name="_visible">表示フラグ</param>
	/// <param name="_x">X座標</param>
	/// <param name="_y">Y座標</param>
	/// <param name="_radius">半径</param>
	void Init(int _image, bool _visible = true, float _x = 0.0f, float _y = 0.0f, float _radius = 0.0f) {
		gameObject.Init(_image, _visible, _x, _y, _radius);
		moveX = moveY = 0.0f;

		sinMoveDefault = 0;
		IntervalCnt = SHOT_INTEVAL;
	}

	/// <summary>
	/// 弾の発射先もここで指定する
	/// </summary>
	/// <param name="_targetX">発射先のX座標</param>
	/// <param name="_targetY">発射先のY座標</param>
	void Update(float _targetX, float _targetY) {
		for (int i = 0; i < ENEMY_BULLET_MAX; i++) {
			bullets[i].Update();
		}

		if (!gameObject.isVisible) {
			return;
		}

		playerX = _targetX;
		playerY = _targetY;

		// 移動関数
		Move();
		// 弾発射
		Shoot(_targetX, _targetY, ONE_WAY_BULLETS, 60);
	}

	/// <summary>
	/// 描画関数
	/// </summary>
	void Render() {
		for (int i = 0; i < ENEMY_BULLET_MAX; i++) {
			bullets[i].Render();
		}

		if (!gameObject.isVisible) {
			return;
		}
		float angle = atan2(playerY - gameObject.y, playerX - gameObject.x) - DX_PI / 2;

		// 描画座標は中心座標です
		// DrawGraph(gameObject.x, gameObject.y, gameObject.image, true);
		DrawRotaGraph(gameObject.cx, gameObject.cy, 1, 0, gameObject.image, true);
	}

	/// <summary>
	/// 移動関数
	/// </summary>
	void Move() {

		// TODO:InitしたらSinカーブの位置を中心に戻せるようにする
		// sin,cosに渡す引数の値が小さくなればなるほど波の間隔が広くなる
		// 引いたらマイナス側、足したらプラス側に寄る
		// 切り返しの時点で三角関数はマイナス値になる
		// マイナスに転換するのがsinだと半周期かかる、cosだと1/4周期になる
		// 値の変異が0スタートならsin、1スタートならcosを使う
		moveX = cosf(sinMoveDefault * 0.1) * 10;

		moveY = 1;

		gameObject.x += moveX;
		gameObject.y += moveY;

		gameObject.cx = gameObject.x + gameObject.width * 0.5f;
		gameObject.cy = gameObject.y + gameObject.height * 0.5f;

		// 非表示にする処理が必要
		// 通常画面の端からエネミーの画像の幅/高さ分超えると非表示にするのが一般的
		if (//(gameObject.x + gameObject.width < 0) ||				//	左端
			//(WINDOW_WIDTH_SVGA < gameObject.x) ||			//	右端
			(gameObject.y + gameObject.height < 0) ||				//	上端
			(WINDOW_HEIGHT_SVGA < gameObject.y)) {			//	下端
			gameObject.isVisible = false;
			gameObject.y = 0;
		}
		//if (sinMoveDefault < 120) {
		sinMoveDefault++;
		//}
		//else {
		//	sinMoveDefault = 0;
		//}
	}

	/// <summary>
	/// 射撃関数
	/// </summary>
	/// <param name="targetX">目標のX座標</param>
	/// <param name="targetY">目標のY座標</param>
	/// <param name="bulletCnt">射撃する弾の数</param>
	/// <param name="sshot_interval">射撃のインターバル</param>
	void Shoot(float targetX, float targetY, int bulletCnt, float shot_interval) {

		int bulletsCnt = 0;

		if (IntervalCnt < shot_interval) {
			IntervalCnt++;
		}

		// 敵なのでインターバルが終わったら無条件で弾を吐き出す。
		if (IntervalCnt >= shot_interval) {

			IntervalCnt = 0;

			for (int i = 0; i < ENEMY_BULLET_MAX; i++) {
				// 既に表示されている（つまりは発射されている
				// 弾に関しては処理を行わない（この関数は発射用のため
				if (bullets[i].gameObject.isVisible) {
					continue;
				}
				// TODO:敵の中心（あるいは先端）から弾が出るようにしたい
				// bullets[i].Init(EnemyBulletAnimations, true, gameObject.cx - bullets[i].gameObject.width / 2, gameObject.cy - bullets[i].gameObject.height / 2, gameObject.radius, ENEMY_BULLET_ANIMATION_MAX);
				bullets[i].gameObject.x = gameObject.x - bullets[i].gameObject.width / 2;
				bullets[i].gameObject.y = gameObject.y - bullets[i].gameObject.height / 2;
				bullets[i].gameObject.cx = bullets[i].gameObject.x - bullets[i].gameObject.width / 2;
				bullets[i].gameObject.cy = bullets[i].gameObject.y - bullets[i].gameObject.height / 2;
				bullets[i].gameObject.isVisible = true;


				// ATANを用いて敵の角度（ラジアン）を計算
				// targetX,targetYは目標の中央座標
				float angle = atan2(targetY - gameObject.y, targetX - gameObject.x);

				angle += (DX_PI * 20 / 180) * (bulletsCnt - bulletCnt / 2);

				bullets[i].moveX = cosf(angle) * ENEMY_BULLET_SPEED;
				bullets[i].moveY = sinf(angle) * ENEMY_BULLET_SPEED;

				bulletsCnt++;
				if (bulletsCnt >= bulletCnt) {
					break;
				}
			}

		}
	}

};