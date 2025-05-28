#pragma once

#include <DxLib.h>
#include <string>

// �}�N����`
#define WINDOW_WIDTH_HD			(1280)		// HD�T�C�Y��
#define WINDOW_HEIGHT_HD		(720)		// HD�T�C�Y����
#define WINDOW_WIDTH_FHD		(1920)		// FHD�T�C�Y��
#define WINDOW_HEIGHT_FHD		(1080)		// FHD�T�C�Y����
#define WINDOW_WIDTH_SVGA		(800)		// SVGA�T�C�Y��
#define WINDOW_HEIGHT_SVGA		(600)		// SVGA�T�C�Y����

#define FPS_60					(60)		// FPS

#define PLAYER_MOVEMENT			(5.0f)	//	�v���C���[�̈ړ���
#define PLAYER_BULLET_MAX		(10000)	// �e�̍ő吔
#define PLAYER_BULLET_CIRCLE_SHOT (64 - 1)	// ����V���b�g�̒e��
#define PLAYER_BULLET_ANIMATION_MAX	(4)	// �e�̃A�j���[�V��������
#define PLAYER_BULLET_MOVEMENT	(10.0f)

#define ENEMY_BULLET_MAX		(100)
#define	ENEMY_BULLET_ANIMATION_MAX	(4)
#define ENEMY_BULLET_SPEED		(2.0f)
#define ENEMY_MAX				(256)	// ��ʏ�ɏo�Ă���G�̍ő吔
#define ENEMY_SPEED				(3)

#define ONE_WAY_BULLETS			(1)
#define THREE_WAY_BULLETS		(3)
#define FOUR_WAY_BULLETS		(4)

#define DEGREE_CIRCLE			(360.0f)
#define RADIAN_CIRCLE			(4*DX_PI_F)

//	�����t���}�N����`
#define Deg2Rad(x) ( x / 180.0f * DX_PI_F )
#define Rad2Deg(x) ( x / DX_PI_F * 180.0f )


// �萔��`
// �F
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

// �v���g�^�C�v��`
void Init();
void Update();
void CollisionCheck();
void Render();

/// <summary>
/// ���̓L�[�񋓌^
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
/// �w�i���C���[�񋓌^
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
* C����Ȃ̂ō\���̂̏��Ԃ���邱�Ɓi�R�[�h�̌�ɕt���������\���̂͑O�̍\���̂���Q�Ƃł��Ȃ����߁j
*/
/// <summary>
/// �L�[���͂���舵���\����
/// </summary>
struct Input {
	struct KeyState {
		bool old;
		bool down;
		bool on;
		bool up;

		/// <summary>
		/// �l�̏�����
		/// </summary>
		void Init() {
			old = false;
			down = false;
			on = false;
			up = false;
		}
	};
	// �L���ȃL�[�̕�����KeyState��p�ӂ���
	struct KeyState KeyState[InputMax];

	/// <summary>
	/// �X�V�֐�
	/// </summary>
	void Update() {
		// �L���ȃL�[�S�Ăɑ΂��ď������s��
		for (int i = 0; i < InputMax; i++) {
			bool now = false;
			//main.cpp�Œ�`����uInputAssign�v�̃L�[���蓖�Ă�DxLib�ɉ���
			now = CheckHitKey(InputAssign[i]);

			// �L�[�̏�Ԃ��X�V
			KeyState[i].on = now;							// ��������Ă���
			KeyState[i].down = now && !KeyState[i].old;		// ��������Ă��邪�O�t���[���ł͉�����Ă��Ȃ�
			KeyState[i].up = !now && KeyState[i].old;		// ��������Ă��Ȃ����O�t���[���ł͉�����Ă���

			KeyState[i].old = now;							// ���t���[���̏�Ԃ�ۑ�����
		}
	}

	// �L�[�̏����擾�ł���悤�ɂ���
	// ���t���[���ŏ��߂ĉ����ꂽ��
	bool IsDown(MyInput _input) {
		return KeyState[_input].down;
	}
	// �L�[���p�����ĉ�����Ă��邩
	bool IsOn(MyInput _input) {
		return KeyState[_input].on;
	}
	// ���t���[���ŃL�[�������ꂽ��
	bool IsUp(MyInput _input) {
		return KeyState[_input].up;
	}
};

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

	/// <summary>
	/// �������֐�
	/// </summary>
	/// <param name="_image">�摜�n���h��</param>
	/// <param name="_visible">�\���t���O</param>
	/// <param name="_x">X���W</param>
	/// <param name="_y">Y���W</param>
	/// <param name="_radius">���蔼�a</param>
	void Init(int _image, bool _visible = true, float _x = 0.0f, float _y = 0.0f, float _radius = 0.0f) {
		// �\���t���O�̏�����
		isVisible = _visible;
		// �摜�n���h���̏�����
		image = _image;
		// ���W�̏�����
		x = _x;
		y = _y;

		// �摜�T�C�Y�̏�����
		GetGraphSize(image, &width, &height);
		// �������W�̏������i�w�肳�ꂽ���W�{�C���[�W�̃T�C�Y�̔����i�����j
		cx = _x + width * 0.5f;
		cy = _y + height * 0.5f;
		// ���a�̏�����
		radius = _radius;
	}
};

/// <summary>
/// �w�i�p�̍\����
/// </summary>
struct BackGround {
	GameObject gameObject;
	float scrollSpeed;
	float currentScroll;

	/// <summary>
	/// �������֐�
	/// </summary>
	/// <param name="_image">�摜�n���h��</param>
	/// <param name="_speed">�X�N���[���X�s�[�h</param>
	void Init(int _image, float _speed) {
		gameObject.Init(_image);
		scrollSpeed = _speed;
		currentScroll = 0.0f;
	}

	/// <summary>
	/// �X�V�֐�
	/// </summary>
	/// <param name="_scroll">�X�N���[����</param>
	void Update(int _scroll) {
		if (!gameObject.isVisible) {
			return;
		}

		// �X�N���[���ʂ��摜�̍����Ŋ���]������߂�
		currentScroll = (int)(_scroll * scrollSpeed) % gameObject.height;
		// �]�肪�摜�̍����𒴉߂�����X�N���[���ʂ�0�ɖ߂�
		if (currentScroll > gameObject.height) {
			currentScroll = 0.0f;
		}
	}

	/// <summary>
	/// �`��֐�
	/// </summary>
	void Render() {
		if (!gameObject.isVisible) {
			return;
		}

		// (0,0)�̍��W�ɉ摜��\������B�������X�N���[�����ď�ɂ��ꂽ���̂�\��
		DrawRectGraph(
			0, 0,									// �`�悷��u��ʁv�̍��W
			0, currentScroll,						// �`�悷��u�摜�v�̍���̍��W
			gameObject.width, gameObject.height,
			gameObject.image,
			true
		);
		// ���DrawGraph�ł���Ĕw�i���Ȃ��Ȃ��������ɉ摜�̏㕔����`�悷��
		DrawRectGraph(
			0, 0,	// �`�悷��u��ʁv�̍��W
			0, WINDOW_HEIGHT_SVGA + currentScroll,									// �`�悷��u�摜�v�̍���̍��W
			gameObject.width, gameObject.height,
			gameObject.image,
			true
		);

	}

};

/// <summary>
/// �e�p�̍\���́i�G�����o���ŗ��p�\�j
/// �e�𗘗p����L�����N�^�̍\���̂ɕϐ��Ƃ��Ē�`����B
/// </summary>
struct Bullet {
	GameObject gameObject;
	float moveX, moveY;
	int animations[PLAYER_BULLET_ANIMATION_MAX];
	int animationCounter;
	int currentAnimation;
	int animationCount;

	/// <summary>
	/// �������֐�
	/// </summary>
	/// <param name="_images">�摜�n���h��</param>
	/// <param name="_visible">�\���t���O</param>
	/// <param name="_x">X���W</param>
	/// <param name="_y">Y���W</param>
	/// <param name="_radius">���蔼�a</param>
	/// <param name="_animationCount">�摜�̃A�j���[�V������</param>
	void Init(int _images[], bool _visible = true, float _x = 0.0f, float _y = 0.0f, float _radius = 0.0f, int _animationCount = -1) {
		gameObject.Init(_images[0], _visible, _x, _y, _radius);
		animationCount = _animationCount;

		// �A�j���[�V�����摜�̓ǂݍ���
		for (int i = 0; i < animationCount; i++) {
			animations[i] = _images[i];
		}

		// �e��ϐ��̏�����
		animationCounter = 0;
		currentAnimation = 0;
		moveX = moveY = 0;
	}

	/// <summary>
	/// �X�V�֐�
	/// </summary>
	void Update() {
		if (!gameObject.isVisible) {
			return;
		}


		// �ړ�������
		gameObject.x += moveX;
		gameObject.y += moveY;

		// �ړ��ƂƂ��ɒ��S���W���X�V����B
		gameObject.cx = gameObject.x + gameObject.width * 0.5f;
		gameObject.cy = gameObject.y + gameObject.height * 0.5f;

		// ��ʊO�֏o���ꍇ�A��\���ɂ���B
		if ((gameObject.x + gameObject.width < -32) ||
			(WINDOW_WIDTH_SVGA + 32 < gameObject.x) ||
			(gameObject.y + gameObject.height < -32) ||
			(WINDOW_HEIGHT_SVGA + 32 < gameObject.y)) {
			gameObject.isVisible = false;
		}

		animationCounter++;

		// �A�j���[�V�����J�E���^�����ʂ𒴂����ꍇ�Ɏ��̃A�j���[�V�����ɑJ�ڂ���悤�ɂ���
		if (animationCounter % 10 == 0) {
			animationCounter = 0;
			currentAnimation++;

			// �A�j���[�V�������Ō�܂ōs������A�͂��߂ɖ߂�
			if (currentAnimation >= animationCount) {
				currentAnimation = 0;
			}
		}
	}

	/// <summary>
	/// �`��֐�
	/// </summary>
	void Render() {
		if (!gameObject.isVisible) {
			return;
		}

#if _DEBUG
		DrawCircle(gameObject.cx, gameObject.cy, gameObject.radius, white, 1);
#endif
		// Update�֐��Ŏw�肵���A�j���[�V������`�悷��
		DrawGraph(gameObject.x, gameObject.y, animations[currentAnimation], true);
	}
};

/// <summary>
/// �v���C���[�̍\����
/// </summary>
struct Player {
	GameObject gameObject;
	float moveX, moveY;

	Bullet bullets[PLAYER_BULLET_MAX];	// ��ʓ��ɑ��݂ł���e�̍\����
	int shotInterval;				// �J�E���g�p�ϐ�
	const int SHOT_INTERVAL = 1;	// ���ˊԊu�萔

	/// <summary>
	/// �������֐�
	/// </summary>
	/// <param name="_image">�摜�n���h��</param>
	/// <param name="_visible">�\���t���O</param>
	/// <param name="_x">X���W</param>
	/// <param name="_y">Y���W</param>
	/// <param name="_radius">���蔼�a</param>
	void Init(int _image, bool _visible = true, float _x = 0.0f, float _y = 0.0f, float _radius = 0.0f) {
		gameObject.Init(_image, _visible, _x, _y, _radius);
		moveX = moveY = 0.0f;

		shotInterval = SHOT_INTERVAL;
	}

	/// <summary>
	/// �X�V�֐�
	/// </summary>
	void Update() {

		// ���˂����e�̍X�V����
		for (int i = 0; i < PLAYER_BULLET_MAX; i++) {
			bullets[i].Update();
		}

		if (!gameObject.isVisible) {
			return;
		}

		// �ړ�����
		Move();

		// �e�̔��ˏ���
		Shoot();
		CircleShoot();
	}

	/// <summary>
	/// �`��֐�
	/// </summary>
	void Render() {

		// ���˂����e�̕`�揈��
		// Player����\���̍ۂł��e�����͓����悤�ɂ������̂ŁA��\�����̏������O�ɋL�q
		for (int i = 0; i < PLAYER_BULLET_MAX; i++) {
			bullets[i].Render();
		}

		if (!gameObject.isVisible) {
			return;
		}

#if _DEBUG
		DrawCircle(gameObject.cx, gameObject.cy, gameObject.radius, white, 1);
#endif#endif
		// ���@�̕\��
		DrawGraph(gameObject.x, gameObject.y, gameObject.image, TRUE);
	}

	/// <summary>
	/// �ړ��֐�
	/// </summary>
	void Move() {
		moveX = moveY = 0.0f;

		// �ړ��̏���
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

		// ��ʒ[�̏ꍇ����ȏ㓮���Ȃ��悤�ɐ��䂷��
		if (gameObject.x + moveX < 0) {		// ��
			gameObject.x = 0;
			moveX = 0;
		}
		if (gameObject.x + moveX > WINDOW_WIDTH_SVGA - gameObject.width) {		// ��
			gameObject.x = WINDOW_WIDTH_SVGA - gameObject.width;
			moveX = 0;
		}
		if (gameObject.y + moveY < 0) {		// ��
			gameObject.y = 0;
			moveY = 0;
		}
		if (gameObject.y + moveY > WINDOW_HEIGHT_SVGA - gameObject.height) {		// ��
			gameObject.y = WINDOW_HEIGHT_SVGA - gameObject.height;
			moveY = 0;
		}

		// �ŏI�I�Ȉړ����W�𔽉f������
		gameObject.x += moveX;
		gameObject.y += moveY;

		// ���S���W���X�V����
		gameObject.cx = gameObject.x + gameObject.width * 0.5f;		// ���݂̍��W�ɃX�v���C�g�̕��̔����𑫂�
		gameObject.cy = gameObject.y + gameObject.height * 0.5f;		// ���݂̍��W�ɃX�v���C�g�̕��̔����𑫂�

	}

	/// <summary>
	/// �ˌ��֐�
	/// </summary>
	void Shoot() {
		// �C���^�[�o���̍X�V
		if (shotInterval < SHOT_INTERVAL) {
			shotInterval++;
		}

		// �C���^�[�o���𒴂��Ă���ꍇ�X�y�[�X�������ƒe�����˂���
		if (input.IsOn(SPACE) && shotInterval >= SHOT_INTERVAL) {
			for (int i = 0; i < PLAYER_BULLET_MAX; i++) {
				// ��ʓ��Ŋ��ɕ\������Ă���e�ɂ��Ă͔��ˏ����͍s��Ȃ�
				if (bullets[i].gameObject.isVisible) {
					continue;
				}

				// �C���^�[�o����Reset
				shotInterval = 0;
				// ���˂���e�̏��������s��
				bullets[i].Init(PlayerBulletAnimation, true, gameObject.x, gameObject.y, gameObject.radius, PLAYER_BULLET_ANIMATION_MAX);

				// ��Ɍ������l�ړ��ʂ�ݒ肷��
				bullets[i].moveX = 0;
				bullets[i].moveY = -PLAYER_BULLET_MOVEMENT;
				break;
			}
		}

	}

	// �~�`�ɋ��𔭎˂���
	void CircleShoot() {
		if (shotInterval < SHOT_INTERVAL) {
			shotInterval++;
		}

		// Z�L�[���������ꍇ�ɔ��˂���
		if (input.IsOn(Z) && shotInterval >= SHOT_INTERVAL) {
			// �o���b�g��S�ă`�F�b�N����
			for (int i = 0; i < PLAYER_BULLET_MAX; i++) {
				// ���łɕ\������Ă���i���ˍς݁j�̃o���b�g�ɂ��Ă͏������s��Ȃ��B
				if (bullets[i].gameObject.isVisible) {
					continue;
				}
				// �o���b�g�̔��ː����K��l�𒴉߂��Ă���ꍇ���[�v���I������
				if (i > PLAYER_BULLET_CIRCLE_SHOT) {
					break;
				}

				// �V���ɕ\������o���b�g����������C���^�[�o�������Z�b�g����
				shotInterval = 0;

				// �o���b�g�̏��������s��
				bullets[i].Init(PlayerBulletAnimation, true, gameObject.x, gameObject.y, gameObject.radius, PLAYER_BULLET_ANIMATION_MAX);


				// ���ˊp�����W�A���ŋ��߁A���[�v���Ɋp�x��ύX����B
				// �p�x�������ʂ�float�ɂ��Ȃ��Ɛ����ł����A���Ă��Ȃ��̂Œ��ӂ��邱��
				float angle = Deg2Rad(360.0f / 64) * i;

				// cos��X���ɁAsin��Y���̈ړ��ɐݒ肷��
				bullets[i].moveX = cosf(angle) * PLAYER_BULLET_MOVEMENT;
				bullets[i].moveY = sinf(angle) * PLAYER_BULLET_MOVEMENT;

			}
		}
	}
};

/// <summary>
/// �G�\����
/// </summary>
struct Enemy {
	GameObject gameObject;
	float moveX, moveY;

	int sinMoveDefault;

	Bullet bullets[ENEMY_BULLET_MAX];
	float IntervalCnt;
	const float SHOT_INTEVAL = 50.0f;

	/// <summary>
	/// �X�V�֐�
	/// </summary>
	/// <param name="_image">�摜�n���h��</param>
	/// <param name="_visible">�\���t���O</param>
	/// <param name="_x">X���W</param>
	/// <param name="_y">Y���W</param>
	/// <param name="_radius">���a</param>
	void Init(int _image, bool _visible = true, float _x = 0.0f, float _y = 0.0f, float _radius = 0.0f) {
		gameObject.Init(_image, _visible, _x, _y, _radius);
		moveX = moveY = 0.0f;

		sinMoveDefault = 0;
		IntervalCnt = SHOT_INTEVAL;
	}

	/// <summary>
	/// �e�̔��ː�������Ŏw�肷��
	/// </summary>
	/// <param name="_targetX">���ː��X���W</param>
	/// <param name="_targetY">���ː��Y���W</param>
	void Update(float _targetX, float _targetY) {
		if (IsAllOut()) {
			return;
		}

		// �e�̍X�V
		for (int i = 0; i < ENEMY_BULLET_MAX; i++) {
			bullets[i].Update();
		}

		// Enemy����\���Ȃ�΍X�V���Ȃ�
		if (!gameObject.isVisible) {
			return;
		}

		// �ړ��֐�
		Move();
		// �e����
		Shoot(_targetX, _targetY, THREE_WAY_BULLETS, 60);
	}

	/// <summary>
	/// �`��֐�
	/// </summary>
	void Render() {
		// ���g�Ɋ֘A����I�u�W�F�N�g���ׂĂ���\���̏ꍇ��
		// �����_�����O�������s��Ȃ�
		if (IsAllOut()) {
			return;
		}

		// �e�̍X�V
		for (int i = 0; i < ENEMY_BULLET_MAX; i++) {
			bullets[i].Render();
		}

		// ��\���Ȃ�΍X�V���Ȃ��i�G�{�́j
		if (!gameObject.isVisible) {
			return;
		}
		// float angle = atan2(playerY - gameObject.y, playerX - gameObject.x) - DX_PI / 2;

#if _DEBUG
		DrawCircle(gameObject.cx, gameObject.cy, gameObject.radius, white, 1);
#endif#endif

		DrawGraph(gameObject.x, gameObject.y, gameObject.image, true);
		// RotaGraph�̕`����W�͒��S���W�ł�
		//DrawRotaGraph(gameObject.cx, gameObject.cy, 1, 0, gameObject.image, true);
	}

	/// <summary>
	/// �ړ��֐�
	/// </summary>
	void Move() {

		// TODO:Init������Sin�J�[�u�̈ʒu�𒆐S�ɖ߂���悤�ɂ���
		// sin,cos�ɓn�������̒l���������Ȃ�΂Ȃ�قǔg�̊Ԋu���L���Ȃ�
		// ��������}�C�i�X���A��������v���X���Ɋ��
		// �؂�Ԃ��̎��_�ŎO�p�֐��̓}�C�i�X�l�ɂȂ�
		// �}�C�i�X�ɓ]������̂�sin���Ɣ�����������Acos����1/4�����ɂȂ�
		// �l�̕ψق�0�X�^�[�g�Ȃ�sin�A1�X�^�[�g�Ȃ�cos���g��
		// moveX = cosf(sinMoveDefault * 0.1) * 10;

		// moveY = 1;

		gameObject.x += moveX;
		gameObject.y += moveY;

		gameObject.cx = gameObject.x + gameObject.width * 0.5f;
		gameObject.cy = gameObject.y + gameObject.height * 0.5f;

		// ��\���ɂ��鏈�����K�v
		// �ʏ��ʂ̒[����G�l�~�[�̉摜�̕�/������������Ɣ�\���ɂ���̂���ʓI
		if ((gameObject.x + gameObject.width < 0) ||				//	���[
			(WINDOW_WIDTH_SVGA < gameObject.x) ||			//	�E�[
			(gameObject.y + gameObject.height < 0) ||				//	��[
			(WINDOW_HEIGHT_SVGA < gameObject.y)) {			//	���[
			gameObject.isVisible = false;
		}
		//if (sinMoveDefault < 120) {
		// sinMoveDefault++;
		//}
		//else {
		//	sinMoveDefault = 0;
		//}
	}

	/// <summary>
	/// �ˌ��֐�
	/// </summary>
	/// <param name="targetX">�ڕW��X���W</param>
	/// <param name="targetY">�ڕW��Y���W</param>
	/// <param name="bulletCnt">�ˌ�����e�̐�</param>
	/// <param name="sshot_interval">�ˌ��̃C���^�[�o��</param>
	void Shoot(float targetX, float targetY, int bulletCnt, float shot_interval) {

		// ���˂���e�̐���
		int bulletsCnt = 0;

		if (IntervalCnt < shot_interval) {
			IntervalCnt++;
		}

		// �G�Ȃ̂ŃC���^�[�o�����I������疳�����Œe��f���o���B
		if (IntervalCnt >= shot_interval) {

			// �C���^�[�o���̃��Z�b�g
			IntervalCnt = 0;

			for (int i = 0; i < ENEMY_BULLET_MAX; i++) {
				// ���ɕ\������Ă���i�܂�͔��˂���Ă���
				// �e�Ɋւ��Ă͏������s��Ȃ��i���̊֐��͔��˗p�̂���
				if (bullets[i].gameObject.isVisible) {
					continue;
				}
				// TODO:�G�̒��S�i���邢�͐�[�j����e���o��悤�ɂ�����
				bullets[i].Init(EnemyBulletAnimations, true, gameObject.cx - bullets[i].gameObject.width / 2, gameObject.cy - bullets[i].gameObject.height / 2, bullets[i].gameObject.radius, ENEMY_BULLET_ANIMATION_MAX);


				// ATAN��p���ēG�̊p�x�i���W�A���j���v�Z
				// targetX,targetY�͖ڕW�̒������W
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
	/// ���g�Ǝ��g��������������ʊO���̃`�F�b�N
	/// </summary>
	/// <returns>��ʊO�̏ꍇTrue</returns>
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
/// �GWave�p
/// </summary>
struct EnemyWave {
	struct SpawnInfo {
		float x, y;		// �o���ʒu
		float moveX, moveY;		// �ړ�����
	};

	// �G�z��
	Enemy enemies[ENEMY_MAX];

	// �G�o���p�^�[��
	SpawnInfo info[3][10] = {
		// wave1
		{
			{100.0f,-32.0f,0,ENEMY_SPEED},
			{400.0f,WINDOW_HEIGHT_SVGA,0,-ENEMY_SPEED},
			{700.0f,-32.0f,0,ENEMY_SPEED}
		},
		// wave2
		{
			{100,-32,0,ENEMY_SPEED},
			{250,-32,0,ENEMY_SPEED},
			{400,-32,0,ENEMY_SPEED},
			{550,-32,0,ENEMY_SPEED},
			{700,-32,0,ENEMY_SPEED},
		},
		// wave3
		{
			{100.0f,-32.0f,0,ENEMY_SPEED},
			{400.0f,-32.0f,0,ENEMY_SPEED * 2},
			{700.0f,-32.0f,0,ENEMY_SPEED}

		}
	};

	/// <summary>
	/// wave�̏���������
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
	/// Wave�̏o��
	/// </summary>
	/// <param name="_type">Wave�ԍ�</param>
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
		}

		// wave�ɏo�Ă���G�̐�
		int index = 0;

		for (int i = 0; i < ENEMY_MAX; i++) {
			//	�G���g���������e���S�Ĕ�\�����ǂ���
			// ��\���łȂ��ꍇ�͂��̓G�̏����������͍s��Ȃ�
			// ���܂�֘A����I�u�W�F�N�g���ׂĂ���\���ɂȂ�Ȃ��Ə��������Ă͂����Ȃ�
			if (!enemies[i].IsAllOut()) {
				continue;
			}

			// �G�̌R���̏��������s��
			// �������̓��e�͏�ɒ�`�����o���p�^�[���z��Ɉˑ�����
			enemies[i].Init(enemies[i].gameObject.image, true, info[_type][index].x, info[_type][index].y, enemies[i].gameObject.radius);
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

	void Update(float _targetX, float _targetY) {
		for (int i = 0; i < ENEMY_MAX; i++) {
			enemies[i].Update(_targetX, _targetY);
		}

		// ���ׂĂ̌R������\���ɂȂ��Ă��邩�m�F��
		// �Ȃ��Ă��Ȃ����Spawn�����͍s��Ȃ��B
		bool visible = false;
		for (int i = 0; i < ENEMY_MAX; i++) {
			if (enemies[i].gameObject.isVisible) {
				visible = true;
				break;
			}
		}
		if (!visible) {
			Spawn(GetRand(2));
		}

	}

	void Render() {
		for (int i = 0; i < ENEMY_MAX; i++) {
			enemies[i].Render();
		}
	}

};

/// <summary>
/// �Q�̃Q�[���I�u�W�F�N�g�̔��a�����ɐڐG�𔻒肷��
/// </summary>
/// <param name="_pObj1"></param>
/// <param name="_pObj2"></param>
/// <returns></returns>
bool CheckHitCircle(GameObject* _pObj1, GameObject* _pObj2) {
	if (!_pObj1->isVisible || !_pObj2->isVisible) {
		return false;
	}
	// �܂��Z���^�[����Ȃ��Ă������Ɣ��a�������Ă�΂����񂶂�Ȃ����ȁE�E�E�E
	float dx = _pObj2->cx - _pObj1->cx;
	float dy = _pObj2->cy - _pObj1->cy;

	float l = sqrtf(dx * dx + dy * dy);

	if (l < _pObj1->radius + _pObj2->radius) {
		return true;
	}

	return false;
}