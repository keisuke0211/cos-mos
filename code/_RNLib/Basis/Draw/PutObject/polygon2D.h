//========================================
// 
// �|���S��2D�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "polygon.h"
#include "../regist-info.h"
#include "../draw-info.h"
#include "../../Draw/camera.h"
#include "../draw-state.h"

//****************************************
// �N���X��`
//****************************************
// �|���S��2D�N���X
class _RNC_Polygon2D {
public:
	//========== [[[ �N���X��` ]]]
	// �`����
	class CDrawInfo : public CDrawInfoBase {
	public:
		// [[[ �֐��錾 ]]]
		static void InitCreateVertexBuffer(void);
		static void CreateVertexBuffer    (const UShort& num);
		static void ReleaseVertexBuffer   (void);
		CDrawInfo ();
		~CDrawInfo();

		// [[[ �ϐ��錾 ]]]
		static VertexBuffer                ms_vtxBuff;
		static UShort                      ms_allocPower;
		static UShort                      ms_allocNum;
		static UShort                      ms_idxCount;
		short                              m_idx;
		void*                              m_tex;
		Polygon2DAnd3D::TEX_TYPE           m_texType;
		_RNC_DrawState::INTERPOLATION_MODE m_interpolationMode;
		Vertex2D                           m_vtxs[4];
	};

	// �o�^���
	class CRegistInfo : public CRegistInfoBase {
	public:
		// [[[ �֐��錾 ]]]
		CRegistInfo();
		~CRegistInfo();
		CRegistInfo* SetVtxPos           (const Pos2D pos0, const Pos2D pos1, const Pos2D pos2, const Pos2D pos3);
		CRegistInfo* SetSize             (const float& width, const float& height);
		CRegistInfo* SetSize             (const Size2D& size);
		CRegistInfo* SetCol              (const Color& col);
		CRegistInfo* SetVtxCol           (const Color col0, const Color col1, const Color col2, const Color col3);
		CRegistInfo* SetTex              (const short& texIdx);
		CRegistInfo* SetTex              (CCamera* camera);
		CRegistInfo* SetTex              (const short& texIdx, const UShort& ptn, const UShort& ptnX, const UShort& ptnY, const Pos2D& ptnPos = INITPOS2D);
		CRegistInfo* SetTex              (CCamera* camera,     const UShort& ptn, const UShort& ptnX, const UShort& ptnY, const Pos2D& ptnPos = INITPOS2D);
		CRegistInfo* SetTex              (const short& texIdx, const Pos2D& pos0, const Pos2D& pos1, const Pos2D& pos2, const Pos2D& pos3);
		CRegistInfo* SetTex              (CCamera* camera,     const Pos2D& pos0, const Pos2D& pos1, const Pos2D& pos2, const Pos2D& pos3);
		CRegistInfo* SetTexMirrorX       (const bool& isMirrorX);
		CRegistInfo* SetTexMirrorY       (const bool& isMirrorY);
		CRegistInfo* SetInterpolationMode(const INTERPOLATION_MODE& interpolationMode);

	private:
		// [[[ �F�B�錾 ]]]
		friend class _RNC_Polygon2D;
		friend class _RNC_DrawMgr;

		// [[[ �֐��錾 ]]]
		void                       ClearParameter(void);
		_RNC_Polygon2D::CDrawInfo* ConvToDrawInfo(void);
		CRegistInfo*               SetIdx        (const short& idx);
		CRegistInfo*               SetPos        (const Pos2D& pos);
		CRegistInfo*               SetAngle      (const Angle& angle);

		// <<< ��{��� >>>
		short                          m_idx;
		Pos2D                          m_pos;

		// <<< ���_��� >>>
		enum class SET_VTX_POS_INFO_TYPE { NONE, NORMAL, SIZE, MAX, };
		struct SetVtxPosInfo {
			Pos2D vtxPoses[4];
		};
		struct SetSizeInfo {
			float width;
			float height;
			Angle angle;
		};
		void*                          m_setVtxPosInfo;
		SET_VTX_POS_INFO_TYPE          m_setVtxPosInfoType;

		// <<< �����ڏ�� >>>
		Color                          m_vtxCols[4];
		Polygon2DAnd3D::CSetTexInfoSum m_setTexInfoSum;

		// <<< �`����ݒ� >>>
		_RNC_DrawState::INTERPOLATION_MODE m_interpolationMode;
	};

	//========== [[[ �֐��錾 ]]]
	_RNC_Polygon2D                ();
	~_RNC_Polygon2D               ();
	void         Init             (void);
	void         Uninit           (void);
	void         Update           (void);
	CRegistInfo* Put              (const UShort& priority, const Pos2D& pos, const Angle& angle, const bool& isOnScreen = false);
	CRegistInfo* Put              (const UShort& priority, const Pos3D& pos, const Angle& angle, const bool& isOnScreen = false);
	CRegistInfo* Put              (const UShort& priority, const bool& isOnScreen = false);
	void         SetVtxPos        (Vertex2D* vtxs, const Pos2D& pos, const Angle& angle, const float& width, const float& height);
	void         SetVtxPos        (Vertex2D* vtxs, const Pos2D& pos0, const Pos2D& pos1, const Pos2D& pos2, const Pos2D& pos3);
	void         SetVtxPos_TopLeft(Vertex2D* vtxs, const Pos2D& pos, const float& width, const float& height);
	void         ApplyResolution  (Vertex2D* vtxs);
	void         SetVtxRHW        (Vertex2D* vtxs);
	void         SetVtxCol        (Vertex2D* vtxs, const Color& col);
	void         SetVtxTex        (Vertex2D* vtxs);
	void         SetVtxTex_Cut    (Vertex2D* vtxs, const Pos2D& cutPos, const float& width, const float& height);
	void         SetVtxTex_Cut    (Vertex2D* vtxs, const Pos2D& cutPos, const float& size);
};