//========================================
// 
// �����蔻��̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../RNmain.h"

//****************************************
// �N���X��`
//****************************************
// �����蔻��N���X
class CHitTest {
public:
	//========== [[[ �֐��錾 ]]]
	CHitTest   ();
	~CHitTest  ();
	void Init  (void);
	void Uninit(void);
	void Update(void);
	// [[[ XZ���ʂɂ����铖���蔻�� ]]]
	class XZ {
	public:
		static bool OverlapPointToSquare(const Pos3D& basePos, const Pos3D& targetPos, const Scale2D& targetScale);
	};
	// [[[ 3����Ԃɂ����铖���蔻�� ]]]
	class XYZ {
	public:
		static bool InPointToCameraView(const D3DXVECTOR3& point, const D3DXVECTOR3& cameraPosition, const D3DXVECTOR3& cameraLookAt, float cameraWidth, float cameraHeight, float fov);
	};
};