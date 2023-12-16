//========================================
// 
// �Z�b�g�A�b�v3D�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../Mechanical/regist.h"
#include "../../../RNmain.h"
#include "../../Draw/color.h"

//****************************************
// �N���X��`
//****************************************
// �Z�b�g�A�b�v3D�N���X
class _RNC_SetUp3D : public CRegist {
public:
	//========== [[[ �\���̒�` ]]]
	// �{�[���f�[�^�\����
	struct Swaying {
		UShort timeMin = 0;
		UShort timeAdd = 0;
		float  distMin = 0.0f;
		float  distAdd = 0.0f;
	};
	struct Follow {
		short    followIdx = NONEDATA;
		Vector3D posRate   = INITVECTOR3D;
		Vector3D rotRate   = INITVECTOR3D;
		Vector3D scaleRate = INITVECTOR3D;
	};
	struct BoneData {
		short    idx         = NONEDATA;
		short    modelIdx    = NONEDATA;
		short    parentIdx   = NONEDATA;
		Pos3D    relativePos = INITPOS3D;
		Pos3D    relativeRot = INITPOS3D;
		Swaying* swaying     = NULL;
		Follow*  follow      = NULL;
	};
	// �t�F�C�X�f�[�^�\����
	struct FaceVtxData {
		UShort boneIdx = NONEDATA;
		UInt   vtxIdx  = NONEDATA;
		Pos2D  texPos  = INITPOS2D;
	};
	struct FaceData {
		FaceVtxData vtxs[4] = {};
		Color       col     = INITCOLOR;
		short       texIdx  = NONEDATA;
	};

	// �f�[�^�\����
	class CData {
	public:
		// [[[ �֐��錾 ]]]
		CData();
		~CData();
		void Release(void);

		// [[[ �ϐ��錾 ]]]
		BoneData* m_boneDatas;
		short     m_boneDataNum;
		FaceData* m_faceDatas;
		short     m_faceDataNum;
	};

	//========== [[[ �֐��錾 ]]]
	_RNC_SetUp3D       ();
	~_RNC_SetUp3D      ();
	void   Init        (void);
	void   Uninit      (void);
	void   Update      (void);
	short  Load        (const char* loadPath, short idx = NONEDATA);
	bool   LoadEditData(const char* loadPath);
	void   SaveEditData(const char* savePath);
	CData& GetData     (const short& idx) { return idx == EDITDATA ? *m_editData : (idx == NONEDATA ? *(CData*)(nullptr) : *m_datas[idx]); }
	void   InitMemory  (const UShort& num);

private:
	//========== [[[ �֐��錾 ]]]
	bool ExecutionLoad(const char* loadPath, CData& data);

	//========== [[[ �ϐ��錾 ]]]
	CData** m_datas;
	CData*  m_editData;
};