//========================================
// 
// ���p�`����N���X�̒�`
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "camera.h"

//----------------------------------------
// �N���X��`
//----------------------------------------
// ���p�`����N���X
class CDrawInfoBase {
public:
	//===== [[[ �񋓌^��` ]]
	// ���
	enum class TYPE {
		NONE,
		POLYGON2D,
		POLYGON3D,
		MODEL,
	};

	//===== [[[ �֐��錾 ]]]
	CDrawInfoBase();
	virtual ~CDrawInfoBase();
	virtual void Draw(Device& device, const Matrix& viewMtx) = 0;

	//===== [[[ �ϐ���` ]]]
	TYPE  m_type;
	short m_priority;
	short m_clippingID;
};
