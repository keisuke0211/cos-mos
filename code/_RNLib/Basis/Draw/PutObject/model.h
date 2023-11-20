//========================================
// 
// ���f���̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../camera.h"
#include "../draw-info.h"
#include "../regist-info.h"
#include "../../Mechanical/regist.h"

//****************************************
// �N���X��`
//****************************************
// ���f���N���X
class CModel : public CRegist {
public:
	//========== [[[ �\���̒�` ]]]
	struct Vertex3DInfo {
		Pos3D    pos;
		Pos3D    worldPos;
		Rot3D    rot;
		Rot3D    worldRot;
		Normal3D nor;
		Normal3D worldNor;
		Pos3D    texPos;
	};

	//========== [[[ �N���X��` ]]]
	// �f�[�^�N���X
	class CData {
	public:
		// [[[ �֐��錾 ]]]
		CData();
		~CData();
		void Release(void);

		// [[[ �ϐ��錾 ]]]
		short* m_texIdxs;
		Mesh   m_mesh;
		Mesh   m_outLineMesh;
		Buffer m_matBuff;
		UShort m_matNum;
		float  m_radiusMax;
	};

	// �`����N���X
	class CDrawInfo : public CDrawInfoBase {
	public:
		// [[[ �֐��錾 ]]]
		CDrawInfo();
		~CDrawInfo();
		void Draw       (Device& device, const Matrix& viewMtx);
		void SetMaterial(Device& device, Material* mat, const Color& col);

		// [[[ �ϐ��錾 ]]]
		static Material ms_outLineMat;
		Matrix m_mtx;
		Color  m_col;
		short  m_modelIdx;
		short  m_texIdx;
		bool   m_isZTest;
		bool   m_isLighting;
		bool   m_isOutLine;
		float  m_brightnessOfEmissive;
		float  m_distance;
	};

	// �o�^���N���X
	class CRegistInfo : public CRegistInfoBase {
	public:
		// [[[ �֐��錾 ]]]
		CRegistInfo();
		~CRegistInfo();
		void ClearParameter(void);
		CDrawInfo* ConvToDrawInfo(void);
		CRegistInfo* SetClippingCamera      (CCamera& camera); 
		CRegistInfo* SetClippingCamera      (const short& ID);
		CRegistInfo* SetMtx                 (const Matrix& mtx);
		CRegistInfo* SetCol                 (const Color& col);
		CRegistInfo* SetModel               (const short& modelIdx);
		CRegistInfo* SetTex                 (const short& texIdx);
		CRegistInfo* SetZTest               (const bool& isZTest);
		CRegistInfo* SetLighting            (const bool& isLighting);
		CRegistInfo* SetOutLine             (const bool& isOutLine);
		CRegistInfo* SetBrightnessOfEmissive(const float& brightnessOfEmissive);

	private:
		// [[[ �ϐ��錾 ]]]
		Matrix m_mtx;
		Color  m_col;
		short  m_modelIdx;
		short  m_texIdx;
		bool   m_isZTest;
		bool   m_isLighting;
		bool   m_isOutLine;
		float  m_brightnessOfEmissive;
	};

	//========== [[[ �֐��錾 ]]]
	CModel();
	~CModel();
	void         Init        (void);
	void         Uninit      (void);
	void         Update      (void);
	void         Release     (void);
	short        Load        (const char* loadPath, short idx = NONEDATA);
	CData&       GetData     (const short& idx) { return *m_datas[idx]; }
	void         StoreVtxInfo(UInt* vtxNum, Vertex3DInfo** vtxInfos, const short& modelIdx, const Matrix& modelMtx);
	CRegistInfo* Put         (const UShort& priority, const short& modelIdx, const Matrix& mtx,                                        const bool& isOnScreen = false);
	CRegistInfo* Put         (const UShort& priority, const short& modelIdx, const Pos3D& pos, const Rot3D& rot,                       const bool& isOnScreen = false);
	CRegistInfo* Put         (const UShort& priority, const short& modelIdx, const Pos3D& pos, const Rot3D& rot, const Scale3D& scale, const bool& isOnScreen = false);

private:
	//========== [[[ �ϐ��錾 ]]]
	CData** m_datas;
};