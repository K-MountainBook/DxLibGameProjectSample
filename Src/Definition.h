#pragma once

#include <DxLib.h>
#include <string>

// マクロ定義
#define WINDOW_WIDTH_HD			(1280)		// HDサイズ幅
#define WINDOW_HEIGHT_HD		(720)		// HDサイズ高さ
#define WINDOW_WIDTH_FHD		(1920)		// FHDサイズ幅
#define WINDOW_HEIGHT_FHD		(1080)		// FHDサイズ高さ
#define WINDOW_WIDTH_SVGA		(800)		// SVGAサイズ幅
#define WINDOW_HEIGHT_SVGA		(600)		// SVGAサイズ高さ

#define FPS_60					(60)		// FPS

#define PLAYER_MOVEMENT			(5.0f)	//	プレイヤーの移動量
#define PLAYER_BULLET_MAX		(256)	// 弾の最大数
#define PLAYER_BULLET_CIRCLE_SHOT (64 - 1)	// 周回ショットの弾数
#define PLAYER_BULLET_ANIMATION_MAX	(4)	// 弾のアニメーション枚数
#define PLAYER_BULLET_MOVEMENT	(10.0f)

#define ENEMY_BULLET_MAX		(100)
#define	ENEMY_BULLET_ANIMATION_MAX	(4)
#define ENEMY_BULLET_SPEED		(2.0f)
#define ENEMY_MAX				(256)	// 画面上に出てくる敵の最大数
#define ENEMY_SPEED				(3)

#define RAPID_FIRE_COUNT		(3)
#define RAPID_FIRE_COOLDOWN		(10)

#define EXPLOSION_MAX			(255)
#define EXPLOSION_ANIMATION_MAX	(4)

#define ONE_WAY_BULLETS			(1)
#define THREE_WAY_BULLETS		(3)
#define FOUR_WAY_BULLETS		(4)

#define DEGREE_CIRCLE			(360.0f)
#define RADIAN_CIRCLE			(4*DX_PI_F)

//	引数付きマクロ定義
#define Deg2Rad(x) ( x / 180.0f * DX_PI_F )
#define Rad2Deg(x) ( x / DX_PI_F * 180.0f )
#define WaveMoveX(x)  ( cosf(x * 0.1) * 10)


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
extern int EnemySpriteHandle[];
extern int EnemyBulletImage;
extern int EnemyBulletAnimations[];

//	エネミーの弾のアニメーション画像
extern int EnemyBulletAnimations[];

extern int bgm;

extern int se1;
extern int se2;

// プロトタイプ定義
void Init();
void Update();
void CollisionCheck();
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
	Z,

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


enum GameSceneType {
	Title,
	InGame,
	Result,

	SceneMax
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
		// 座標の初期化
		x = _x;
		y = _y;

		// 画像サイズの初期化
		GetGraphSize(image, &width, &height);
		// 中央座標の初期化（指定された座標＋イメージのサイズの半分（中央）
		cx = _x + width * 0.5f;
		cy = _y + height * 0.5f;
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
	void Update(int _scroll) {
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
			0, 0,										// 描画する「画面」の座標
			0, WINDOW_HEIGHT_SVGA + currentScroll,		// 描画する「画像」の左上の座標
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


		// 移動させる
		gameObject.x += moveX;
		gameObject.y += moveY;

		// 移動とともに中心座標も更新する。
		gameObject.cx = gameObject.x + gameObject.width * 0.5f;
		gameObject.cy = gameObject.y + gameObject.height * 0.5f;

		// 画面外へ出た場合、非表示にする。
		// 横
		if ((gameObject.x + gameObject.width < -32) ||
			(WINDOW_WIDTH_SVGA + 32 < gameObject.x) ||
		// 縦
			(gameObject.y + gameObject.height < -32) ||
			(WINDOW_HEIGHT_SVGA + 32 < gameObject.y)) {
			gameObject.isVisible = false;
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
	}

	/// <summary>
	/// 描画関数
	/// </summary>
	void Render() {
		if (!gameObject.isVisible) {
			return;
		}

#if _DEBUG
		DrawCircle(gameObject.cx, gameObject.cy, gameObject.radius, white, 1);
#endif
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

		// 表示されていないオブジェクトについては処理を行わない
		if (!gameObject.isVisible) {
			return;
		}

		// 移動処理
		Move();

		// 弾の発射処理
		Shoot();
		// 円形発射の関数
		CircleShoot();
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

#if _DEBUG
		DrawCircle(gameObject.cx, gameObject.cy, gameObject.radius, white, 1);
#endif
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

				// 上に向かう様移動量を設定する
				bullets[i].moveX = 0;
				bullets[i].moveY = -PLAYER_BULLET_MOVEMENT;
				break;
			}
		}

	}

	// 円形に球を発射する
	void CircleShoot() {
		if (shotInterval < SHOT_INTERVAL) {
			shotInterval++;
		}

		// Zキーを押した場合に発射する
		if (input.IsOn(Z) && shotInterval >= SHOT_INTERVAL) {
			// バレットを全てチェックする
			for (int i = 0; i < PLAYER_BULLET_MAX; i++) {
				// すでに表示されている（発射済み）のバレットについては処理を行わない。
				if (bullets[i].gameObject.isVisible) {
					continue;
				}
				// バレットの発射数が規定値を超過している場合ループを終了する
				if (i > PLAYER_BULLET_CIRCLE_SHOT) {
					break;
				}

				// 新たに表示するバレットがあったらインターバルをリセットする
				shotInterval = 0;

				// バレットの初期化を行う
				bullets[i].Init(PlayerBulletAnimation, true, gameObject.x, gameObject.y, gameObject.radius, PLAYER_BULLET_ANIMATION_MAX);


				// 発射角をラジアンで求め、ループ毎に角度を変更する。
				// 角度か分割量をfloatにしないと整数でしか帰ってこないので注意すること
				float angle = Deg2Rad(360.0f / 64) * i;

				// cosをX軸に、sinをY軸の移動に設定する
				bullets[i].moveX = cosf(angle) * PLAYER_BULLET_MOVEMENT;
				bullets[i].moveY = sinf(angle) * PLAYER_BULLET_MOVEMENT;

			}
		}
	}
};

/// <summary>
/// 敵構造体
/// </summary>
struct Enemy {


	enum moveType {
		STRAIGHT,
		WAVE,
		STOPANDBACK,
	};

	//enum STOPANDBACK_moveMode {
	//	foward,
	//	stop,
	//	back
	//};

	GameObject gameObject;
	float moveX, moveY;

	int sinMoveDefault;

	Bullet bullets[ENEMY_BULLET_MAX];
	float IntervalCnt;
	const float SHOT_INTEVAL = 50.0f;

	int rapidFireCnt = 0;
	int rapidFireInterval = 5;

	moveType mType;

	int src = 0;		// 時刻経過用変数

	/// <summary>
	/// 更新関数
	/// </summary>
	/// <param name="_image">画像ハンドル</param>
	/// <param name="_visible">表示フラグ</param>
	/// <param name="_x">X座標</param>
	/// <param name="_y">Y座標</param>
	/// <param name="_radius">半径</param>
	void Init(int _image, bool _visible = true, float _x = 0.0f, float _y = 0.0f, float _radius = 0.0f, moveType movetype = Enemy::moveType::STRAIGHT) {
		gameObject.Init(_image, _visible, _x, _y, _radius);
		moveX = moveY = 0.0f;

		sinMoveDefault = 0;
		IntervalCnt = SHOT_INTEVAL;
		mType = movetype;
		src = 0;
	}

	/// <summary>
	/// 弾の発射先もここで指定する
	/// </summary>
	/// <param name="_targetX">発射先のX座標</param>
	/// <param name="_targetY">発射先のY座標</param>
	void Update(float _targetX, float _targetY) {
		if (IsAllOut()) {
			return;
		}

		// 弾の更新
		for (int i = 0; i < ENEMY_BULLET_MAX; i++) {
			bullets[i].Update();
		}

		// Enemyが非表示ならば更新しない
		if (!gameObject.isVisible) {
			return;
		}

		// 移動関数
		switch (mType)
		{
		case Enemy::STRAIGHT:
			MoveStraight();
			break;
		case Enemy::WAVE:
			MoveWave();
			break;
		case Enemy::STOPANDBACK:
			MoveStopAndBack();
			break;
		default:
			break;
		}
		
		// 弾発射
		//Shoot(_targetX, _targetY, THREE_WAY_BULLETS, 60);
		// 一定数連射する関数
		 rapidFire(_targetX, _targetY, RAPID_FIRE_COUNT, 60);

		 src++;
	}

	/// <summary>
	/// 描画関数
	/// </summary>
	void Render(float _targetX, float _targetY) {
		// 自身に関連するオブジェクトすべてが非表示の場合は
		// レンダリング処理を行わない
		if (IsAllOut()) {
			return;
		}

		// 弾の更新
		for (int i = 0; i < ENEMY_BULLET_MAX; i++) {
			bullets[i].Render();
		}

		// 非表示ならば更新しない（敵本体）
		if (!gameObject.isVisible) {
			return;
		}
		// float angle = atan2(playerY - gameObject.y, playerX - gameObject.x) - DX_PI / 2;

#if _DEBUG
		DrawCircle(gameObject.cx, gameObject.cy, gameObject.radius, white, 1);
#endif#endif

		if (mType != Enemy::STOPANDBACK) {
			DrawGraph(gameObject.x, gameObject.y, gameObject.image, true);
		} else {
		// RotaGraphの描画座標は中心座標です
			// ATANを用いて敵の角度（ラジアン）を計算
			// targetX,targetYは目標の中央座標
			float angle = atan2(_targetY - gameObject.cy, _targetX - gameObject.cx) - DX_PI / 2;
			DrawRotaGraph(gameObject.cx, gameObject.cy, 1, angle, gameObject.image, true);
		}
	}

	/// <summary>
	/// 移動関数
	/// </summary>
	void MoveStraight() {

		gameObject.x += moveX;
		gameObject.y += moveY;

		gameObject.cx = gameObject.x + gameObject.width * 0.5f;
		gameObject.cy = gameObject.y + gameObject.height * 0.5f;


		// 非表示にする処理が必要
		// 通常画面の端からエネミーの画像の幅/高さ分超えると非表示にするのが一般的
		if ((gameObject.x + gameObject.width < 0) ||				//	左端
			(WINDOW_WIDTH_SVGA < gameObject.x) ||			//	右端
			(gameObject.y + gameObject.height < 0) ||				//	上端
			(WINDOW_HEIGHT_SVGA < gameObject.y)) {			//	下端
			gameObject.isVisible = false;
		}

	}

	// 移動関数（周期）
	void MoveWave() {
		
		// cos、sinカーブは1～0～-1を取る
		gameObject.x += cosf(src / 10.0f) * 10;
		gameObject.y += ENEMY_SPEED;

		gameObject.cx = gameObject.x + gameObject.width * 0.5f;
		gameObject.cy = gameObject.y + gameObject.height * 0.5f;

		
		// 非表示にする処理が必要
		// 通常画面の端からエネミーの画像の幅/高さ分超えると非表示にするのが一般的
		if ((gameObject.x + gameObject.width + WINDOW_WIDTH_SVGA < 0) ||				//	左端
			(WINDOW_WIDTH_SVGA * 2 < gameObject.x) ||			//	右端
			(gameObject.y + gameObject.height < 0) ||				//	上端
			(WINDOW_HEIGHT_SVGA < gameObject.y)) {			//	下端
			gameObject.isVisible = false;
		}
	}

	// 移動後一定時間滞留したあとに引き返す
	void MoveStopAndBack() {


		if (src <= 100) {
			rapidFireInterval = 0;
			gameObject.x += moveX;
			gameObject.y += moveY;
		}
		else if (src > 200) {

			rapidFireInterval = 0;
			gameObject.x -= moveX;
			gameObject.y -= moveY;
		}
		gameObject.cx = gameObject.x + gameObject.width * 0.5f;
		gameObject.cy = gameObject.y + gameObject.height * 0.5f;
		src++;

		// 非表示にする処理が必要
		// 通常画面の端からエネミーの画像の幅/高さ分超えると非表示にするのが一般的
		if ((gameObject.x + gameObject.width + WINDOW_WIDTH_SVGA < 0) ||				//	左端
			(WINDOW_WIDTH_SVGA * 2 < gameObject.x) ||			//	右端
			(gameObject.y + gameObject.height < 0) ||				//	上端
			(WINDOW_HEIGHT_SVGA < gameObject.y)) {			//	下端
			gameObject.isVisible = false;
		}
	}

	/// <summary>
	/// 射撃関数
	/// </summary>
	/// <param name="targetX">目標のX座標</param>
	/// <param name="targetY">目標のY座標</param>
	/// <param name="bulletCnt">射撃する弾の数</param>
	/// <param name="sshot_interval">射撃のインターバル</param>
	void Shoot(float targetX, float targetY, int bulletCnt, float shot_interval) {

		// 発射する弾の数量
		int bulletsCnt = 0;

		if (IntervalCnt < shot_interval) {
			IntervalCnt++;
		}

		// 敵なのでインターバルが終わったら無条件で弾を吐き出す。
		if (IntervalCnt >= shot_interval) {

			// インターバルのリセット
			IntervalCnt = 0;

			for (int i = 0; i < ENEMY_BULLET_MAX; i++) {
				// 既に表示されている（つまりは発射されている
				// 弾に関しては処理を行わない（この関数は発射用のため
				if (bullets[i].gameObject.isVisible) {
					continue;
				}
				// 敵の中心（あるいは先端）から弾が出るように
				bullets[i].Init(EnemyBulletAnimations, true, gameObject.cx - bullets[i].gameObject.width / 2, gameObject.cy - bullets[i].gameObject.height / 2, bullets[i].gameObject.radius, ENEMY_BULLET_ANIMATION_MAX);


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

	/// <summary>
	/// 一定数の弾を一度に連射する
	/// </summary>
	/// <param name="targetX"></param>
	/// <param name="targetY"></param>
	/// <param name="bulletCnt"></param>
	/// <param name="shot_interval"></param>
	void rapidFire(float targetX, float targetY, int bulletCnt, float shot_interval) {

		if (IntervalCnt < shot_interval) {
			IntervalCnt++;
			rapidFireCnt = bulletCnt;
		}

		// 敵なのでインターバルが終わったら無条件で弾を吐き出す。
		if (IntervalCnt >= shot_interval) {

			if (rapidFireInterval > RAPID_FIRE_COOLDOWN) {

				rapidFireInterval = 0;

				for (int i = 0; i < ENEMY_BULLET_MAX; i++) {
					// 既に表示されている（つまりは発射されている
					// 弾に関しては処理を行わない（この関数は発射用のため
					if (bullets[i].gameObject.isVisible) {
						continue;
					}
					// 敵の中心（あるいは先端）から弾が出るように
					bullets[i].Init(EnemyBulletAnimations, true, gameObject.cx - bullets[i].gameObject.width / 2, gameObject.cy - bullets[i].gameObject.height / 2, bullets[i].gameObject.radius, ENEMY_BULLET_ANIMATION_MAX);

					// ATANを用いて敵の角度（ラジアン）を計算
					// targetX,targetYは目標の中央座標
					float angle = atan2(targetY - gameObject.y, targetX - gameObject.x);

					bullets[i].moveX = cosf(angle) * ENEMY_BULLET_SPEED;
					bullets[i].moveY = sinf(angle) * ENEMY_BULLET_SPEED;

					rapidFireCnt--;

					break;

				}
			}
			else {
				rapidFireInterval++;
			}

			if (rapidFireCnt <= 0) {
				IntervalCnt = 0;
			}

		}

	}


	/// <summary>
	/// 自身と自身が撃った球が画面外かのチェック
	/// </summary>
	/// <returns>画面外の場合True</returns>
	bool IsAllOut() {
		if (gameObject.isVisible) {
			return false;
		}

		for (int i = 0; i < ENEMY_BULLET_MAX; i++) {
			if (bullets[i].gameObject.isVisible) {
				return false;
			}
		}
		return true;
	}

};

/// <summary>
/// 敵Wave用
/// </summary>
struct EnemyWave {
	struct SpawnInfo {
		float x, y;		// 出現位置
		float moveX, moveY;		// 移動方向
		Enemy::moveType moveType;
	};

	// 敵配列
	Enemy enemies[ENEMY_MAX];

	// 敵出現テーブル
	SpawnInfo info[5][5] = {
		// wave1
		{
			{100.0f - 16.0f,-32.0f,0,ENEMY_SPEED,Enemy::moveType::STRAIGHT },
			{400.0f - 16.0f,WINDOW_HEIGHT_SVGA,0,-ENEMY_SPEED,Enemy::moveType::STRAIGHT},
			{700.0f - 16.0f,-32.0f,0,ENEMY_SPEED,Enemy::moveType::STRAIGHT}
		},
		// wave2
		{
			{100 - 16.0f,-32,0,ENEMY_SPEED,Enemy::moveType::STRAIGHT},
			{250 - 16.0f,-32,0,ENEMY_SPEED,Enemy::moveType::STRAIGHT},
			{400 - 16.0f,-32,0,ENEMY_SPEED,Enemy::moveType::STRAIGHT},
			{550 - 16.0f,-32,0,ENEMY_SPEED,Enemy::moveType::STRAIGHT},
			{700 - 16.0f,-32,0,ENEMY_SPEED,Enemy::moveType::STRAIGHT},
		},
		// wave3
		{
			{100.0f - 16.0f,-32.0f,0,ENEMY_SPEED,Enemy::moveType::STRAIGHT},
			{400.0f - 16.0f,-32.0f,0,ENEMY_SPEED * 2,Enemy::moveType::STRAIGHT},
			{700.0f - 16.0f,-32.0f,0,ENEMY_SPEED,Enemy::moveType::STRAIGHT}

		},
		// wave4
		{
			{100.0f - 16.0f,-32.0f,0,ENEMY_SPEED,Enemy::moveType::WAVE},
			{400.0f - 16.0f,-32.0f,0,ENEMY_SPEED,Enemy::moveType::WAVE},
			{700.0f - 16.0f,-32.0f,0,ENEMY_SPEED,Enemy::moveType::WAVE}
		},
		// wave5
		{
			{100.0f - 16.0f,-32.0f,0,ENEMY_SPEED,Enemy::moveType::STOPANDBACK},
			{400.0f - 16.0f,-32.0f,0,ENEMY_SPEED,Enemy::moveType::STOPANDBACK},
			{700.0f - 16.0f,-32.0f,0,ENEMY_SPEED,Enemy::moveType::STOPANDBACK}
		},
	};

	/// <summary>
	/// waveの初期化処理
	/// </summary>
	void Init() {
		for (int i = 0; i < ENEMY_MAX; i++) {
			enemies[i].Init(EnemySpriteHandle[0], false, 0, 0, 16.0f);
			for (int j = 0; j < ENEMY_BULLET_MAX; j++) {
				enemies[i].bullets[j].Init(EnemyBulletAnimations, false, 0.0, 0.0, 4);
			}
		}
	}

	/// <summary>
	/// Waveの出撃
	/// </summary>
	/// <param name="_type">Wave番号</param>
	void Spawn(int _type = 0) {
		int num = 0;

		switch (_type) {
		case 0:
			num = 3;
			break;
		case 1:
			num = 5;
			break;
		case 2:
			num = 3;
			break;
		case 3:
			num = 3;
			break;
		case 4:
			num = 3;
			break;
		}

		// waveに出てくる敵の数
		int index = 0;

		for (int i = 0; i < ENEMY_MAX; i++) {
			//	敵自身が撃った弾が全て非表示かどうか
			// 非表示でない場合はその敵の初期化処理は行わない
			// →つまり関連するオブジェクトすべてが非表示にならないと初期化してはいけない
			if (!enemies[i].IsAllOut()) {
				continue;
			}

			// 敵の軍勢の初期化を行う
			// 初期化の内容は上に定義した出現パターン配列に依存する
			enemies[i].Init(enemies[i].gameObject.image, true, info[_type][index].x, info[_type][index].y, enemies[i].gameObject.radius, info[_type][index].moveType);
			enemies[i].moveX = info[_type][index].moveX;
			enemies[i].moveY = info[_type][index].moveY;

			for (int j = 0; j < ENEMY_BULLET_MAX; j++) {
				enemies[i].bullets[j].Init(EnemyBulletAnimations, false, 0, 0, 4, ENEMY_BULLET_ANIMATION_MAX);
			}

			index++;

			// 
			if (index >= num) {
				break;
			}
		}
	}

	/// <summary>
	/// Enemy波状の更新
	/// </summary>
	/// <param name="_targetX"></param>
	/// <param name="_targetY"></param>
	void Update(float _targetX, float _targetY) {
		for (int i = 0; i < ENEMY_MAX; i++) {
			enemies[i].Update(_targetX, _targetY);
		}

		// すべての軍勢が非表示になっているか確認し
		// なっていなければSpawn処理は行わない。
		bool visible = false;
		for (int i = 0; i < ENEMY_MAX; i++) {
			if (enemies[i].gameObject.isVisible) {
				visible = true;
				break;
			}
		}
		if (!visible) {
			Spawn(GetRand(4));
// 			Spawn((Enemy::moveType)4);
		}

	}

	/// <summary>
	///  EnemyWaveの描画
	/// </summary>
	/// <param name="_targetX">プレイヤー位置X</param>
	/// <param name="_targetY">プレイヤー位置Y</param>
	void Render(float _targetX, float _targetY) {
		for (int i = 0; i < ENEMY_MAX; i++) {
			enemies[i].Render(_targetX, _targetY);
		}
	}

};

/// <summary>
/// 爆発アニメーション構造体
/// </summary>
struct Explosion {
	GameObject gameObject;

	int animations[EXPLOSION_ANIMATION_MAX];	// アニメーション画像ハンドル
	int animationCounter;						// 何番目のアニメーションを表示するかのカウント
	int currentAnimation;						// 今表示されているアニメーション番号

	/// <summary>
	/// 爆発アニメーションの初期化
	/// </summary>
	/// <param name="_images"></param>
	/// <param name="_visible"></param>
	/// <param name="_x"></param>
	/// <param name="_y"></param>
	/// <param name="_radius"></param>
	void Init(int _images[EXPLOSION_ANIMATION_MAX], bool _visible = true, float _x = 0.0f, float _y = 0.0f, float _radius = 0.0f) {
		gameObject.Init(_images[0], _visible, _x, _y, _radius);

		// アニメーション画像の初期化
		for (int i = 0; i < EXPLOSION_ANIMATION_MAX; i++) {
			animations[i] = _images[i];
		}

		animationCounter = 0;

		currentAnimation = 0;

		if (_visible) {
			// サウンドの再生
		}
	}

	/// <summary>
	/// 爆発アニメーションの更新
	/// </summary>
	void Update() {
		// 非表示なら実行しない
		if (!gameObject.isVisible) {
			return;
		}

		// フレーム毎にカウンタを足しこむ
		animationCounter++;

		// 一定時間ごとにアニメーションを進めるようにする
		// このプログラムだとカウンタが10進むごとにアニメーションを進める（10フレームで1アニメーション進める）
		if (animationCounter % 10 == 0) {
			// アニメーションを更新したらカウンタを0に戻す
			animationCounter = 0;
			currentAnimation++;

			if (currentAnimation > EXPLOSION_ANIMATION_MAX) {
				gameObject.isVisible = false;
				currentAnimation = 0;
			}
		}
	}
	
	/// <summary>
	/// 爆発アニメーションの描画
	/// </summary>
	void Render() {
		if (!gameObject.isVisible) {
			return;
		}

		DrawGraph(gameObject.x, gameObject.y, animations[currentAnimation], true);
	}

};

/// <summary>
/// ２つのゲームオブジェクトの半径を元に接触を判定する
/// </summary>
/// <param name="_pObj1"></param>
/// <param name="_pObj2"></param>
/// <returns></returns>
bool CheckHitCircle(GameObject* _pObj1, GameObject* _pObj2) {
	if (!_pObj1->isVisible || !_pObj2->isVisible) {
		return false;
	}
	// まあセンターじゃなくても距離と半径が合ってればいいんじゃないかな・・・・
	float dx = _pObj2->cx - _pObj1->cx;
	float dy = _pObj2->cy - _pObj1->cy;

	float l = sqrtf(dx * dx + dy * dy);

	if (l < _pObj1->radius + _pObj2->radius) {
		return true;
	}

	return false;
}