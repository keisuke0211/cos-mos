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
		Pos2D    texPos;
	};

	//========== [[[ �N���X��` ]]]
	// �f�[�^�N���X
	class CData {
	public:
		// [[[ �\���̒�` ]]]
		struct VtxData {
			bool* isMats = NULL;
		};
		struct MatData {
			Color  col    = INITCOLOR;
			ULong  vtxNum = 0;
			ULong* idxes  = NULL;
			ULong  idxNum = 0;
		};

		// [[[ �֐��錾 ]]]
		CData();
		~CData();
		void Release(void);

		// [[[ �ϐ��錾 ]]]
		short*   m_texIdxes;
		Texture* m_texes;
		VtxData* m_vtxDatas;
		ULong    m_vtxNum;
		ULong*   m_idxes;
		ULong    m_idxNum;
		Buffer   m_matBuff;
		MatData* m_matDatas;
		UShort   m_matNum;
		UShort*  m_faceMatIdxes;
		ULong    m_faceNum;
		Mesh     m_mesh;
		Mesh*    m_outLineMeshs;
		float    m_radiusMax;
	};

	// �`����N���X
	class CDrawInfo : public CDrawInfoBase {
	public:
		// [[[ �֐��錾 ]]]
		CDrawInfo();
		~CDrawInfo();

		// [[[ �ϐ��錾 ]]]
		static Material ms_outLineMat;
		Matrix    m_mtx;
		Material* m_mats;
		Texture*  m_texes;
		UShort    m_matNum;
		Mesh      m_mesh;
		Mesh      m_outLineMesh;
		bool      m_isScaling;
		bool      m_isZTest;
		bool      m_isLighting;
		float     m_radiusMax;
	};

	// �o�^���N���X
	class CRegistInfo : public CRegistInfoBase {
	public:
		// [[[ �֐��錾 ]]]
		CRegistInfo();
		~CRegistInfo();
		void ClearParameter(void);
		CDrawInfo* ConvToDrawInfo(Device& device);
		CRegistInfo* SetClippingCamera      (CCamera& camera); 
		CRegistInfo* SetClippingCamera      (const short& ID);
		CRegistInfo* SetMtx                 (const Matrix& mtx);
		CRegistInfo* SetCol                 (const Color& col);
		CRegistInfo* SetModel               (const short& modelIdx);
		CRegistInfo* SetTex                 (const short& texIdx);
		CRegistInfo* SetZTest               (const bool& isZTest);
		CRegistInfo* SetLighting            (const bool& isLighting);
		CRegistInfo* SetOutLineIdx          (const UShort& outLineIdx);
		CRegistInfo* SetBrightnessOfEmissive(const float& brightnessOfEmissive);

	private:
		// [[[ �ϐ��錾 ]]]
		Matrix m_mtx;
		Color  m_col;
		short  m_modelIdx;
		short  m_texIdx;
		bool   m_isZTest;
		bool   m_isLighting;
		short  m_outLineIdx;
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
	void         StoreVtxInfo(const Matrix& modelMtx, const short& modelIdx, Vertex3DInfo** vtxInfos, const short& matIdx = NONEDATA);
	CRegistInfo* Put         (const UShort& priority, const short& modelIdx, const Matrix& mtx,                                        const bool& isOnScreen = false);
	CRegistInfo* Put         (const UShort& priority, const short& modelIdx, const Pos3D& pos, const Rot3D& rot,                       const bool& isOnScreen = false);
	CRegistInfo* Put         (const UShort& priority, const short& modelIdx, const Pos3D& pos, const Rot3D& rot, const Scale3D& scale, const bool& isOnScreen = false);

private:
	//========== [[[ �֐��錾 ]]]
	void ExecutionStoreVtxInfo(Vertex3DInfo* vtx, const Matrix& modelMtx, const ULong& cntVtx, BYTE*& vtxBuff, const DWORD& dwSizeFVF, const DWORD& texCoordOffset);

	//========== [[[ �ϐ��錾 ]]]
	CData** m_datas;
};