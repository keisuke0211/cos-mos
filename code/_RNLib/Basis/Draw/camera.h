//========================================
// 
// カメラの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// クラス定義
//****************************************
// カメラクラス
class CCamera {
public:
	//========== [[[ 列挙型定義 ]]]
	// 状態
	enum class STATE {
		NONE,
		GRAB_AIR_MOUSE,		// マウスで宙を掴む
		RECIPROCATE_SPIN,	// 往復回転
		MAX,
	};

	// 振動の強さ
	enum class VIB_POWER { LOW, MID, HIGH, MAX, };

	//========== [[[ 関数宣言 ]]]
	CCamera();
	~CCamera();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void SetTexture(LPDIRECT3DDEVICE9 device) { device->SetTexture(0, m_texturesMT[0]); }
	// 設定系
	static void StartRenderingScreen(LPDIRECT3DDEVICE9 device);
	void StartRendering (LPDIRECT3DDEVICE9 device);
	void EndRendering   (LPDIRECT3DDEVICE9 device);
	void SetGeometryInfo(const D3DXVECTOR3 posV, const D3DXVECTOR3 posR);
	void SetVib         (const VIB_POWER vibPower);
	void SetRadianGoal  (const float fGoal) { m_radianGoal = fGoal; }
	void SetFixed       (const bool bFixed) { m_isFixed = bFixed; };
	void PivotToPosV    (void)              { if (m_state == STATE::NONE) m_isPivotToPosV = true; }
	void PivotToPosR    (void)              { if (m_state == STATE::NONE) m_isPivotToPosV = false; }
	// 取得
	D3DXVECTOR3 GetPosV(void) { return m_posV; }
	D3DXVECTOR3 GetPosR(void) { return m_posR; }
	D3DXVECTOR3 GetRot (void) { return m_rot; }
	D3DXVECTOR3 GetVec (void) { return m_posR - m_posV; }
	D3DXVECTOR3 GetNor (void) { 
		D3DXVECTOR3 vec = GetVec();
		return *D3DXVec3Normalize(&vec, &vec);
	}
	// 状態系
	void RemoveState    (void) { SetState(STATE::NONE); }
	void GrabAirMouse   (const float moveForce, const float spinForce, const float zoomForce);
	void ReciprocateSpin(const D3DXVECTOR3 posR, const float dist, const D3DXVECTOR3 rotA, const D3DXVECTOR3 rotB, const int time);
	// 操作系
	void ParallelMove(const D3DXVECTOR3 move) { m_posV += move; m_posR += move; }
	void Spin        (const D3DXVECTOR3 add)  { m_spin += add;   }

private:
	//========== [[[ 構造体宣言 ]]]
	struct GrabAirMouseInfo {
		float moveForce;
		float spinForce;
		float zoomForce;
	};
	struct ReciprocateSpinInfo {
		D3DXVECTOR3 rotA = INITD3DXVECTOR3;
		D3DXVECTOR3 rotB = INITD3DXVECTOR3;
		int time  = 0;
		int count = 0;
	};

	//========== [[[ 関数宣言 ]]]
	static void SetUpProjectionMatrix(LPDIRECT3DDEVICE9 device, const float radian);
	void ProcessState(const PROCESS process);
	void SetState(const STATE state);

	//========== [[[ 変数の宣言 ]]]
	D3DXVECTOR3 m_posV;		// 視点
	D3DXVECTOR3 m_posR;		// 注視点
	D3DXVECTOR3 m_posVib;	// 振動位置
	// 運動系
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_spin;
	float       m_dist;
	float       m_radian;
	float       m_radianGoal;
	float       m_vibForce;
	// 状態系
	bool        m_isPivotToPosV;
	bool        m_isFixed;
	STATE       m_state;
	void*       m_stateInfo;
	// マルチターゲットレンダリング
	LPDIRECT3DTEXTURE9      m_texturesMT[2];	// レンダリングターゲット用テクスチャ
	LPDIRECT3DSURFACE9      m_rendersMT[2];		// テクスチャレンダリング用インターフェース
	LPDIRECT3DSURFACE9      m_ZBuffMT;			// テクスチャレンダリング用Zバッファ
	D3DVIEWPORT9            m_viewportMT;		// テクスチャレンダリング用ビューポート
	LPDIRECT3DVERTEXBUFFER9 m_vtxBuffMT;		// フィードバック用ポリゴンの頂点バッファ
};