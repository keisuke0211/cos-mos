//========================================
// 
// 入力クラスの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// クラス定義
//****************************************
// 入力クラス
class CInput {
public:
	//========== [[[ 定数定義 ]]]
	static const int NUM_KEY_MAX = 256;

	//========== [[[ 列挙型の定義 ]]]
	enum class ACTIVE_DEVICE { KEYBOARD, JOYPAD, MAX, };
	enum class INPUT_ANGLE   { UP, DOWN, LEFT, RIGHT, MAX, };
	enum class MOUSEBUTTON   { LEFT, RIGHT, MAX, };
	enum class BUTTON        { UP, DOWN, LEFT, RIGHT, START, BACK, LEFT_THUMB, RIGHT_THUMB, LEFT_SHOULDER, RIGHT_SHOULDER, LEFT_TRIGGER, RIGHT_TRIGGER, A, B, X, Y, MAX, };
	enum class STICK         { LEFT, RIGHT, MAX, };
	enum class WHEELSPIN     { NONE, FRONT, BACK, };

	//========== [[[ クラス定義 ]]]
	// ジョイパッドクラス
	class CJoyPad {
	public:
		CJoyPad();
		~CJoyPad();
		void  Update              (void);
		void  SetIdx              (const UShort& idx) { m_idx = idx; }
		void  SetVibration        (const float& vibration);
		bool  GetButtonPress      (const BUTTON& btn) { return m_buttonInputs[(int)btn].press   != 0; }
		bool  GetButtonTrigger    (const BUTTON& btn) { return m_buttonInputs[(int)btn].trigger != 0; }
		bool  GetButtonRelease    (const BUTTON& btn) { return m_buttonInputs[(int)btn].release != 0; }
		float GetStickTiltRate    (const STICK& stick) { return m_sticInputs[(int)stick].tiltRate; }
		float GetStickTiltAngle   (const STICK& stick) { return m_sticInputs[(int)stick].tiltAngle; }
		bool  GetStickAnglePress  (const STICK& stick, const INPUT_ANGLE& angle) { return m_sticInputs[(int)stick].angleInputs[(int)angle].press   != 0; }
		bool  GetStickAngleTrigger(const STICK& stick, const INPUT_ANGLE& angle) { return m_sticInputs[(int)stick].angleInputs[(int)angle].trigger != 0; }
		bool  GetStickAngleRelease(const STICK& stick, const INPUT_ANGLE& angle) { return m_sticInputs[(int)stick].angleInputs[(int)angle].release != 0; }

	private:
		// [[[ 構造体定義 ]]]
		struct Input {
			BYTE press = 0;
			BYTE trigger = 0;
			BYTE release = 0;
		};
		struct StickInput {
			float tiltRate;
			float tiltAngle;
			Input angleInputs[(int)INPUT_ANGLE::MAX];
		};

		// [[[ 関数宣言 ]]]
		void UpdateButton(void);
		void UpdateStick(void);
		void UpdateVibration(void);

		// [[[ 変数宣言 ]]]
		UShort       m_idx;
		Input        m_buttonInputs[(int)BUTTON::MAX];
		StickInput   m_sticInputs[(int)STICK::MAX];
		int          m_vibrationCounter;
		int          m_vibrationCounterMax;
		float        m_vibration;
		XINPUT_STATE m_xInputState;
	};

	//========== [[[ 関数宣言 ]]]
	CInput();
	~CInput();
	void          Init                (HINSTANCE& instanceHandle);
	void          Uninit              (void);
	void          Update              (void);
	void          ClearInputInfo      (void);
	void          SetActiveDevice     (const ACTIVE_DEVICE& device)                                 { m_activeDevice = device; }
	ACTIVE_DEVICE GetActiveDevice     (void)                                                        { return m_activeDevice; }
	bool          GetPress            (const int& key, const BUTTON& button, const UShort& idx = 0) { return GetKeyPress(key) || GetButtonPress(button, idx); }
	bool          GetTrigger          (const int& key, const BUTTON& button, const UShort& idx = 0) { return GetKeyTrigger(key) || GetButtonTrigger(button, idx); }
	bool          GetRelease          (const int& key, const BUTTON& button, const UShort& idx = 0) { return GetKeyRelease(key) || GetButtonRelease(button, idx); }
	bool          GetKeyPress         (const int& key)                                              { return m_keyInputs[key].press != 0; }
	bool          GetKeyTrigger       (const int& key)                                              { return m_keyInputs[key].trigger != 0; }
	bool          GetKeyRelease       (const int& key)                                              { return m_keyInputs[key].release != 0; }
	bool          GetMousePress       (const MOUSEBUTTON& btn)                                      { return m_mouseButtonInputs[(int)btn].press != 0; }
	bool          GetMouseTrigger     (const MOUSEBUTTON& btn)                                      { return m_mouseButtonInputs[(int)btn].trigger != 0; }
	bool          GetMouseRelease     (const MOUSEBUTTON& btn)                                      { return m_mouseButtonInputs[(int)btn].release != 0; }
	void          FixedCursor         (const bool& isFixed)                                         { m_isFixedCursor = isFixed; ShowCursor(!isFixed); }
	Pos2D         GetCursorPosOnWindow(void)                                                        { return m_cursorInfo.pos; }
	Vector2D      GetCursorMove       (void)                                                        { return m_cursorInfo.move; }
	WHEELSPIN     GetWheelSpin        (void)                                                        { return m_wheelSpin; }
	void          SetWheelSpin        (WHEELSPIN wheelSpin)                                         { m_wheelSpin = wheelSpin; }
	void          SetJoyPadNum        (const UShort& num);
	CJoyPad&      GetJoyPad           (const UShort& idx)                                                   { return m_joyPads[idx]; }
	void          SetVibration        (const float& vibration,                       const UShort& idx = 0) { if    (CheckJoyPadConnected(idx))  m_joyPads[idx].SetVibration        (vibration);            }
	bool          GetButtonPress      (const BUTTON& btn,                            const UShort& idx = 0) { return CheckJoyPadConnected(idx) ? m_joyPads[idx].GetButtonPress      (btn)          : false; }
	bool          GetButtonTrigger    (const BUTTON& btn,                            const UShort& idx = 0) { return CheckJoyPadConnected(idx) ? m_joyPads[idx].GetButtonTrigger    (btn)          : false; }
	bool          GetButtonRelease    (const BUTTON& btn,                            const UShort& idx = 0) { return CheckJoyPadConnected(idx) ? m_joyPads[idx].GetButtonRelease    (btn)          : false; }
	float         GetStickTiltRate    (const STICK& stick,                           const UShort& idx = 0) { return CheckJoyPadConnected(idx) ? m_joyPads[idx].GetStickTiltRate    (stick)        : 0.0f;  }
	float         GetStickTiltAngle   (const STICK& stick,                           const UShort& idx = 0) { return CheckJoyPadConnected(idx) ? m_joyPads[idx].GetStickTiltAngle   (stick)        : 0.0f;  }
	bool          GetStickAnglePress  (const STICK& stick, const INPUT_ANGLE& angle, const UShort& idx = 0) { return CheckJoyPadConnected(idx) ? m_joyPads[idx].GetStickAnglePress  (stick, angle) : false; }
	bool          GetStickAngleTrigger(const STICK& stick, const INPUT_ANGLE& angle, const UShort& idx = 0) { return CheckJoyPadConnected(idx) ? m_joyPads[idx].GetStickAngleTrigger(stick, angle) : false; }
	bool          GetStickAngleRelease(const STICK& stick, const INPUT_ANGLE& angle, const UShort& idx = 0) { return CheckJoyPadConnected(idx) ? m_joyPads[idx].GetStickAngleRelease(stick, angle) : false; }

private:
	//========== [[[ 定数宣言 ]]]
	static const int MOUSE_MASK[(int)MOUSEBUTTON::MAX];
	static const int BUTTON_MASK[(int)BUTTON::MAX];

	//========== [[[ 構造体定義 ]]]
	struct Input {
		BYTE press   = 0;
		BYTE trigger = 0;
		BYTE release = 0;
	};
	struct CursorInfo {
		Pos2D    pos  = INITPOS2D;
		Vector2D move = INITVECTOR2D;
	};

	//========== [[[ 関数宣言 ]]]
	void UpdateKeyboard(void);
	void UpdateMouseButton(void);
	void UpdateCursor(void);
	void UpdateJoyPad(void);
	bool CheckJoyPadConnected(const UShort& idx) { return (idx < m_joyPadNum); }

	//========== [[[ 変数宣言 ]]]
	Input                m_keyInputs[NUM_KEY_MAX];
	Input                m_mouseButtonInputs[(int)MOUSEBUTTON::MAX];
	CursorInfo           m_cursorInfo;
	WHEELSPIN            m_wheelSpin;
	CJoyPad*             m_joyPads;
	UShort               m_joyPadNum;
	LPDIRECTINPUT8       m_inputDevice;
	LPDIRECTINPUTDEVICE8 m_keyboardDevice;
	ACTIVE_DEVICE        m_activeDevice;
	bool                 m_isFixedCursor;
};