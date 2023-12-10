//========================================
// 
// �f���]�[���̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../RNlib.h"
#include "demo_object.h"

//****************************************
// �N���X��`
//****************************************
// �f���]�[���N���X
class CDemoZone : public CDemoObject {
public:
	//========== [[[ �񋓌^��` ]]]
	enum class TYPE2 { DRAW_TEST, CAMERA_TEST, DOLL_TEST, EFFECT_TEST, MATMESH_TEST, MAX,};

	//========== [[[ �֐��錾 ]]]
	static void        SetActive(CDemoZone* demoZone) { ms_active = demoZone; }
	static CDemoZone*& GetActive(void)                { return ms_active; }
	CDemoZone                   (const TYPE2& type2);
	~CDemoZone                  ();
	void               Update   (void);
	const Size2D&      GetScale (void) { return TYPE_DATAS[(int)m_type2].size; }
	const Pos3D&       GetPos   (void) { return TYPE_DATAS[(int)m_type2].pos; }

private:
	//========== [[[ �񋓌^��` ]]]
	enum class PUTOBJECT_TYPE { POLYGON3D, TEXT3D, MODEL, MAX };
	enum class TEXTURE_TYPE   { NONE, NORMAL, CAMERA, MAX };

	//========== [[[ �萔��` ]]]
	static const char* PUTOBJECT_TYPE_NAME[(int)PUTOBJECT_TYPE::MAX];
	static const char* ALIGNMENT_NAME     [(int)CText::ALIGNMENT::MAX];

	//========== [[[ �\���̒�` ]]]
	// ��ޖ��̃f�[�^
	struct TypeData {
		const char*  name;
		const Pos3D  pos;
		const Size2D size;
	};
	// �w�v�b�g�I�u�W�F�N�g�e�X�g�x�̏��
	struct TypeInfo_PutObjectTest_Text3DInfo {
		int              drawNum   = 0;
		CText::ALIGNMENT alignment = CText::ALIGNMENT::CENTER;
	};
	struct TypeInfo_PutObjectTest_Model {
		int   drawNum                = 0;
		Color col                    = INITCOLOR;
		bool  isTex                  = false;
		bool  isZTest                = true;
		bool  isLighting             = true;
		bool  m_outLineIdx           = 0;
		float m_brightnessOfEmissive = 1.0f;
	};
	struct TypeInfo_DrawTest {
		PUTOBJECT_TYPE                    putObjType = PUTOBJECT_TYPE::TEXT3D;
		TypeInfo_PutObjectTest_Text3DInfo text3DInfo = {};
		TypeInfo_PutObjectTest_Model      modelInfo  = {};
	};
	// �w�h�[���x�̏��
	struct TypeInfo_DollTest {
		CDoll3D* doll;
		Pos3D    poses[2];
		Rot3D    rots[2];
	};
	// �w�}�e���A�����b�V���x�̏��
	struct TypeInfo_MatMeshTest {
		int  setNum = 0;
		bool isMesh = false;
	};

	//========== [[[ �֐��錾 ]]]
	void UpdateActive(void);

	//========== [[[ �ϐ��錾 ]]]
	static const TypeData TYPE_DATAS[(int)TYPE2::MAX];
	static CDemoZone* ms_active;
	void* m_typeInfo;
	TYPE2 m_type2;
};