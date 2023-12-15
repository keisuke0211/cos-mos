//========================================
// 
// 当たり判定の処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../RNmain.h"

//****************************************
// クラス定義
//****************************************
// 当たり判定クラス
class CHitTest {
public:
	//========== [[[ 関数宣言 ]]]
	CHitTest   ();
	~CHitTest  ();
	void Init  (void);
	void Uninit(void);
	void Update(void);
	// [[[ XZ平面における当たり判定 ]]]
	class XZ {
	public:
		static bool OverlapPointToSquare(const Pos3D& basePos, const Pos3D& targetPos, const Scale2D& targetScale);
	};
	// [[[ 3軸空間における当たり判定 ]]]
	class XYZ {
	public:
		static bool InPointToCameraView(const D3DXVECTOR3& point, const D3DXVECTOR3& cameraPosition, const D3DXVECTOR3& cameraLookAt, float cameraWidth, float cameraHeight, float fov);
	};
};