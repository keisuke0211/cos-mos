//========================================
// 
// 入力クラスの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//****************************************
// マクロ定義
//****************************************
#define VIBRATION_SUBTRACT (0.02f)

//================================================================================
//----------|---------------------------------------------------------------------
//==========| 入力クラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// 定数定義
//****************************************
// マウスのマスク
const int CInput::MOUSE_MASK[(int)MOUSEBUTTON::MAX] = {
	VK_LBUTTON,
	VK_RBUTTON,
};

// ボタンのマスク
const int CInput::BUTTON_MASK[(int)BUTTON::MAX] = {
	XINPUT_GAMEPAD_DPAD_UP,
	XINPUT_GAMEPAD_DPAD_DOWN,
	XINPUT_GAMEPAD_DPAD_LEFT,
	XINPUT_GAMEPAD_DPAD_RIGHT,
	XINPUT_GAMEPAD_START,
	XINPUT_GAMEPAD_BACK,
	XINPUT_GAMEPAD_LEFT_THUMB,
	XINPUT_GAMEPAD_RIGHT_THUMB,
	XINPUT_GAMEPAD_LEFT_SHOULDER,
	XINPUT_GAMEPAD_RIGHT_SHOULDER,
	-1,
	-1,
	XINPUT_GAMEPAD_A,
	XINPUT_GAMEPAD_B,
	XINPUT_GAMEPAD_X,
	XINPUT_GAMEPAD_Y,
};

//========================================
// コンストラクタ
//========================================
CInput::CInput() {

}

//========================================
// デストラクタ
//========================================
CInput::~CInput() {

}

//========================================
// 初期化処理
//========================================
void CInput::Init(HINSTANCE& instanceHandle) {

	m_joyPads = NULL;
	m_joyPadNum = 0;
	m_inputDevice = NULL;
	m_keyboardDevice = NULL;
	m_activeDevice = ACTIVE_DEVICE::KEYBOARD;
	m_isFixedCursor = false;

	// 入力情報をクリア
	ClearInputInfo();

	// マウスカーソルを表示
	// (※最初からtrueだと、後から要求を無視される為一度falseにしてからtrueにする)
	ShowCursor(false);
	ShowCursor(true);

	// XInputを開く
	XInputEnable(true);

	// DirectInputオブジェクトの生成
	DirectInput8Create(instanceHandle, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_inputDevice, NULL);

	// 入力デバイス(キーボード)の生成
	m_inputDevice->CreateDevice(GUID_SysKeyboard, &m_keyboardDevice, NULL);

	// データフォーマットを設定
	m_keyboardDevice->SetDataFormat(&c_dfDIKeyboard);

	// 協調フォーマット
	m_keyboardDevice->SetCooperativeLevel(RNLib::Window().GetHandle(), (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));

	// キーボードへのアクセス権を獲得
	m_keyboardDevice->Acquire();
}

//========================================
// 終了処理
//========================================
void CInput::Uninit(void) {

	// XInputを閉じる
	XInputEnable(false);

	// 入力デバイス(キーボード)の破棄
	if (m_keyboardDevice != NULL) {
		m_keyboardDevice->Unacquire();	// キーボードへのアクセス権を放棄
		m_keyboardDevice->Release();
		m_keyboardDevice = NULL;
	}

	// DirectInputオブジェクトの破棄
	if (m_inputDevice != NULL) {
		m_inputDevice->Release();
		m_inputDevice = NULL;
	}

	// ジョイパッド解放
	CMemory::Release(&m_joyPads);
}

//========================================
// 更新処理
//========================================
void CInput::Update(void) {

	UpdateKeyboard();
	UpdateCursor();
	UpdateMouseButton();
	UpdateJoyPad();

	// ホイールの回転をリセットする
	SetWheelSpin(CInput::WHEELSPIN::NONE);
}

//========================================
// 入力情報クリア処理
//========================================
void CInput::ClearInputInfo(void) {

	for (int cntKey = 0; cntKey < NUM_KEY_MAX; cntKey++)
		m_keyInputs[cntKey] = {};

	for (int cntMouseButton = 0; cntMouseButton < (int)MOUSEBUTTON::MAX; cntMouseButton++)
		m_mouseButtonInputs[cntMouseButton] = {};

	m_cursorInfo = {};
	m_wheelSpin  = WHEELSPIN::NONE;
}

//========================================
// キーボードの更新処理
//========================================
void CInput::UpdateKeyboard(void) {

	// キーボードの入力情報
	BYTE keyInputs[NUM_KEY_MAX];

	// 入力デバイスからデータを取得
	if (SUCCEEDED(m_keyboardDevice->GetDeviceState(sizeof(keyInputs), &keyInputs[0]))) {
		for (int cntKey = 0; cntKey < NUM_KEY_MAX; cntKey++) {
			// キーボードのトリガー情報を保存
			m_keyInputs[cntKey].trigger = (m_keyInputs[cntKey].press ^ keyInputs[cntKey]) & keyInputs[cntKey];

			// キーボードのリリース情報を保存
			m_keyInputs[cntKey].release = (m_keyInputs[cntKey].press ^ keyInputs[cntKey]) & ~keyInputs[cntKey];

			// キーボードのプレス情報を保存
			m_keyInputs[cntKey].press = keyInputs[cntKey];

			// 入力が行われた時、動的なデバイスをキーボードにする
			if (keyInputs[cntKey])
				m_activeDevice = ACTIVE_DEVICE::KEYBOARD;
		}
	}
	else
	{// 入力デバイスからデータを取得できなかった時、
		m_keyboardDevice->Acquire();	// キーボードへのアクセス権を取得
	}
}

//========================================
// マウスの更新処理
//========================================
void CInput::UpdateMouseButton(void) {

	// 画面にフォーカスが当たっているかどうか調べる
	bool isWindowFocused = RNLib::Window().FindFocused(RNLib::Window().GetHandle());

	for (int cntMouseButton = 0; cntMouseButton < (int)MOUSEBUTTON::MAX; cntMouseButton++) {

		// マウスの入力情報
		BYTE mouseState = isWindowFocused ? (GetKeyState(MOUSE_MASK[cntMouseButton]) & (0x80) ? 1 : 0) : 0;

		// マウスのトリガー情報を保存
		m_mouseButtonInputs[cntMouseButton].trigger = (m_mouseButtonInputs[cntMouseButton].press ^ mouseState) & mouseState;

		// マウスのリリース情報を保存
		m_mouseButtonInputs[cntMouseButton].release = (m_mouseButtonInputs[cntMouseButton].press ^ mouseState) & ~mouseState;

		// マウスのプレス情報を保存
		m_mouseButtonInputs[cntMouseButton].press = mouseState;

		// 入力が行われた時、動的なデバイスをキーボードにする
		if (mouseState)
			m_activeDevice = ACTIVE_DEVICE::KEYBOARD;
	}
}

//========================================
// カーソルの更新処理
//========================================
void CInput::UpdateCursor(void) {

	// 過去の位置として保存
	Pos2D oldCursorPos = m_cursorInfo.pos;

	// クライアント領域のサイズを取得
	RECT rc;
	GetClientRect(RNLib::Window().GetHandle(), &rc);

	{// カーソルのウィンドウ上の位置を取得
		// カーソルの現在位置を取得
		POINT cursorPos;
		GetCursorPos(&cursorPos);

		// ウィンドウの位置を取得
		Pos3D windowPos = RNLib::Window().GetPos();

		m_cursorInfo.pos = Pos2D(
			(cursorPos.x - windowPos.x) * (RNLib::Window().GetWidth() / (float)rc.right),
			(cursorPos.y - windowPos.y) * (RNLib::Window().GetHeight() / (float)rc.bottom));
	}

	// カーソルの移動量を設定
	m_cursorInfo.move = m_cursorInfo.pos - oldCursorPos;

	// カーソルの固定
	if (m_isFixedCursor) {

		// マウス座標の設定
		SetCursorPos(rc.right * 0.5f, rc.bottom * 0.5f);

		// カーソル位置を保存
		Pos2D centerPos = RNLib::Window().GetCenterPos();
		m_cursorInfo.pos.x = centerPos.x;
		m_cursorInfo.pos.y = centerPos.y;
	}
}

//========================================
// ジョイパッド数設定処理
//========================================
void CInput::SetJoyPadNum(const UShort& num) {

	// メモリ再確保
	CMemory::ReAlloc(&m_joyPads, m_joyPadNum, num);

	// 数を代入
	m_joyPadNum = num;

	// 番号を代入していく
	for (int cntJoyPad = 0; cntJoyPad < m_joyPadNum; cntJoyPad++)
		m_joyPads[cntJoyPad].SetIdx(cntJoyPad);
}

//========================================
// コントローラーの更新処理
//========================================
void CInput::UpdateJoyPad(void) {

	for (int cntJoyPad = 0; cntJoyPad < m_joyPadNum; cntJoyPad++)
		m_joyPads[cntJoyPad].Update();
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| ジョイパッドクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CInput::CJoyPad::CJoyPad() {

	m_idx                 = 0;
	for (int cntButton = 0; cntButton < (int)BUTTON::MAX; cntButton++)
		m_buttonInputs[cntButton] = {};
	for (int cntStick = 0; cntStick < (int)STICK::MAX; cntStick++)
		m_sticInputs[cntStick] = {};
	m_vibrationCounter    = 0;
	m_vibrationCounterMax = 0;
	m_vibration           = 0.0f;
	memset(&m_xInputState, 0, sizeof(XINPUT_STATE));
}

//========================================
// デストラクタ
//========================================
CInput::CJoyPad::~CJoyPad() {

}

//========================================
// 更新処理
//========================================
void CInput::CJoyPad::Update(void) {

	// XInputの状態を取得
	XInputGetState(m_idx, &m_xInputState);

	UpdateButton();
	UpdateStick();
	UpdateVibration();
}

//========================================
// ボタンの更新処理
//========================================
void CInput::CJoyPad::UpdateButton(void) {

	for (int cntButton = 0; cntButton < (int)BUTTON::MAX; cntButton++) {

		// ボタンの入力情報
		BYTE button = 0;

		//----------------------------------------
		// 入力判定
		//----------------------------------------
		// [[[ 左トリガーの時 ]]]
		if (cntButton == (int)BUTTON::LEFT_TRIGGER)
			button = (BYTE)(m_xInputState.Gamepad.bLeftTrigger > 0);	// 押されてるか
		// [[[ 右トリガーの時 ]]]
		else if (cntButton == (int)BUTTON::RIGHT_TRIGGER)
			button = (BYTE)(m_xInputState.Gamepad.bRightTrigger > 0);	// 押されてるか
		// [[[ それ以外のボタンで押されている時 ]]]
		else if (m_xInputState.Gamepad.wButtons & BUTTON_MASK[cntButton])
			button = 1;

		// ボタンのトリガー情報を保存
		m_buttonInputs[cntButton].trigger = (m_buttonInputs[cntButton].press ^ button) & button;

		// ボタンのリリース情報を保存
		m_buttonInputs[cntButton].release = (m_buttonInputs[cntButton].press ^ button) & ~button;

		// ボタンのプレス情報を保存
		m_buttonInputs[cntButton].press = button;

		// 入力が行われた時、動的なデバイスをジョイパッドにする(※0番目限定)
		if (m_idx == 0)
			if (button != 0)
				RNLib::Input().SetActiveDevice(ACTIVE_DEVICE::JOYPAD);
	}
}

//========================================
// スティックの更新処理
//========================================
void CInput::CJoyPad::UpdateStick(void) {

	// スティックの入力情報
	BYTE angleInputs[(int)INPUT_ANGLE::MAX];

	for (int cntStick = 0; cntStick < (int)STICK::MAX; cntStick++) {
		float X = 0.0f;	// スティックのX軸
		float Y = 0.0f;	// スティックのY軸

		// 種類に応じたスティックの軸の値を取得
		switch ((STICK)cntStick) {
		case STICK::LEFT:
			X = m_xInputState.Gamepad.sThumbLX;
			Y = m_xInputState.Gamepad.sThumbLY;
			break;
		case STICK::RIGHT:
			X = m_xInputState.Gamepad.sThumbRX;
			Y = m_xInputState.Gamepad.sThumbRY;
			break;
		}

		// 角度を取得
		m_sticInputs[cntStick].tiltAngle = CGeometry::FindAngleXY(Pos3D(X, Y, 0.0f), Pos3D(0.0f, 0.0f, 0.0f)) * -1;

		// スティックの倒し具合を取得
		m_sticInputs[cntStick].tiltRate = fabsf(X);
		if (m_sticInputs[cntStick].tiltRate < fabsf(Y)) {
			m_sticInputs[cntStick].tiltRate = fabsf(Y);
		}
		m_sticInputs[cntStick].tiltRate /= 32768.0f;

		// 方向入力フラグを初期化
		for (int cntAngle = 0; cntAngle < (int)INPUT_ANGLE::MAX; cntAngle++) {
			angleInputs[cntAngle] = false;
		}

		if (m_sticInputs[cntStick].tiltRate > 0)
		{// スティックが倒されている時、
			// 角度が四分割で上に位置する
			if (   (m_sticInputs[cntStick].tiltAngle < D3DX_PI_LEFT_UP)
				|| (m_sticInputs[cntStick].tiltAngle > D3DX_PI_RIGHT_UP))
				angleInputs[(int)INPUT_ANGLE::UP] = true;

			// 角度が四分割で下に位置する
			else if ((m_sticInputs[cntStick].tiltAngle > D3DX_PI_LEFT_DOWN)
				&&   (m_sticInputs[cntStick].tiltAngle < D3DX_PI_RIGHT_DOWN))
				angleInputs[(int)INPUT_ANGLE::DOWN] = true;

			// 角度が四分割で左に位置する
			else if ((m_sticInputs[cntStick].tiltAngle > D3DX_PI_LEFT_UP)
				&&   (m_sticInputs[cntStick].tiltAngle < D3DX_PI_LEFT_DOWN))
				angleInputs[(int)INPUT_ANGLE::LEFT] = true;

			// 角度が四分割で右に位置する
			else if ((m_sticInputs[cntStick].tiltAngle < D3DX_PI_RIGHT_UP)
				&&   (m_sticInputs[cntStick].tiltAngle > D3DX_PI_RIGHT_DOWN))
				angleInputs[(int)INPUT_ANGLE::RIGHT] = true;

			// 動的なデバイスをジョイパッドにする(※0番目限定)
			if (m_idx == 0)
				RNLib::Input().SetActiveDevice(ACTIVE_DEVICE::JOYPAD);
		}

		for (int cntAngle = 0; cntAngle < (int)INPUT_ANGLE::MAX; cntAngle++) {

			// スティックのトリガー情報を保存
			m_sticInputs[cntStick].angleInputs[cntAngle].trigger = (m_sticInputs[cntStick].angleInputs[cntAngle].press ^ angleInputs[cntAngle]) & angleInputs[cntAngle];

			// スティックのリリース情報を保存
			m_sticInputs[cntStick].angleInputs[cntAngle].release = (m_sticInputs[cntStick].angleInputs[cntAngle].press ^ angleInputs[cntAngle]) & ~angleInputs[cntAngle];

			// スティックのプレス情報を保存
			m_sticInputs[cntStick].angleInputs[cntAngle].press = angleInputs[cntAngle];
		}
	}
}

//========================================
// 振動の更新処理
//========================================
void CInput::CJoyPad::UpdateVibration(void) {

	if (m_vibration > 0.0f)
	{// 振動倍率が0を上回っている時、
		// 振動倍率を減算
		m_vibration -= VIBRATION_SUBTRACT;

		// 振動倍率を制御
		if (m_vibration < 0.0f)
			m_vibration = 0.0f;

		{// 振動フラグが真の時、
			XINPUT_VIBRATION vibration;	// XINPUT_VIBRATION vibrationを作成

			//vibrationメンバーwLeftMotorSpeedに値を代入 モータースピード100%
			vibration.wLeftMotorSpeed = 65535 * m_vibration;

			//vibrationメンバーwRightMotorSpeedに値を代入 モータースピード0%
			vibration.wRightMotorSpeed = 65535 * m_vibration;

			//ジョイスティックにバイブレーターデータを送信
			XInputSetState(m_idx, &vibration);
		}
	}
}

//========================================
// コントローラーの振動設定処理
//========================================
void CInput::CJoyPad::SetVibration(const float& vibration) {

	// 自身がジョイパッド0番でアクティブでない時、処理を終了
	if (m_idx == 0)
		if (RNLib::Input().GetActiveDevice() != ACTIVE_DEVICE::JOYPAD)
			return;

	// 振動倍率が元の振動倍率を上回っている時、振動倍率を代入
	if (m_vibration < vibration) {
		m_vibration = vibration;

		if (m_vibration > 1.0f)
			m_vibration = 1.0f;
	}
}
