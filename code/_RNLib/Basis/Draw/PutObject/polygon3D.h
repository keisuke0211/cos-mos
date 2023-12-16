//========================================
// 
// �|���S��3D�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "polygon.h"
#include "../draw-info.h"
#include "../draw-state.h"
#include "../../Draw/camera.h"
#include "../draw-state.h"

//****************************************
// �N���X��`
//****************************************
// �|���S��3D�N���X
class _RNC_Polygon3D {
public:
	//========== [[[ �N���X��` ]]]
	// �`����
	class CDrawInfo : public CDrawInfoBase {
	public:
		// [[[ �֐��錾 ]]]
		CDrawInfo();
		~CDrawInfo();
		static void InitCreateVertexBuffer(void);
		static void CreateVertexBuffer(const UShort& num);
		static void ReleaseVertexBuffer(void);

		// [[[ �ϐ��錾 ]]]
		static VertexBuffer                ms_vtxBuff;
		static UShort                      ms_allocPower;
		static UShort                      ms_allocNum;
		static UShort                      ms_idxCount;
		short                              m_idx;
		Matrix                             m_mtx;
		void*                              m_tex;
		Polygon2DAnd3D::TEX_TYPE           m_texType;
		bool                               m_isZTest;
		bool                               m_isLighting;
		bool                               m_isBillboard;
		_RNC_DrawState::CULLING_MODE       m_cullingMode;
		_RNC_DrawState::ALPHA_BLEND_MODE   m_alphaBlendMode;
		_RNC_DrawState::INTERPOLATION_MODE m_interpolationMode;
		float                              m_distance;
		Vertex3D                           m_vtxs[4];
	};

	// �o�^���
	class CRegistInfo : public CRegistInfoBase {
	public:
		// [[[ �֐��錾 ]]]
		CRegistInfo                                    ();
		~CRegistInfo                                   ();
		void                       ClearParameter      (void);
		_RNC_Polygon3D::CDrawInfo* ConvToDrawInfo      (void);
		CRegistInfo*               SetIdx              (const short& idx);
		CRegistInfo*               SetMtx              (const Matrix& mtx);
		CRegistInfo*               SetClippingCamera   (CCamera& camera); 
		CRegistInfo*               SetClippingCamera   (const short& ID);
		CRegistInfo*               SetVtxPos           (const Pos3D pos0, const Pos3D pos1, const Pos3D pos2, const Pos3D pos3);
		CRegistInfo*               SetSize             (const float& width, const float& height);
		CRegistInfo*               SetSize             (const Size2D& size);
		CRegistInfo*               SetVtxNor           (const Vector3D nor0, const Vector3D nor1, const Vector3D nor2, const Vector3D nor3);
		CRegistInfo*               SetCol              (const Color& col);
		CRegistInfo*               SetVtxCol           (const Color col0, const Color col1, const Color col2, const Color col3);
		CRegistInfo*               SetTex              (const short& texIdx, const UShort& ptn = 0, const UShort& ptnX = 1, const UShort& ptnY = 1, const Pos2D& ptnPos = INITPOS2D);
		CRegistInfo*               SetTex              (CCamera* camera, const UShort& ptn = 0, const UShort& ptnX = 1, const UShort& ptnY = 1, const Pos2D& ptnPos = INITPOS2D);
		CRegistInfo*               SetTexUV            (const short& texIdx, const Pos2D& pos0, const Pos2D& pos1, const Pos2D& pos2, const Pos2D& pos3);
		CRegistInfo*               SetTexUV            (CCamera* camera, const Pos2D& pos0, const Pos2D& pos1, const Pos2D& pos2, const Pos2D& pos3);
		CRegistInfo*               SetTexMirrorX       (const bool& isMirror);
		CRegistInfo*               SetZTest            (const bool& isZTest);
		CRegistInfo*               SetLighting         (const bool& isLighting);
		CRegistInfo*               SetBillboard        (const bool& isBillboard);
		CRegistInfo*               SetCullingMode      (const _RNC_DrawState::CULLING_MODE& cullingMode);
		CRegistInfo*               SetAlphaBlendMode   (const _RNC_DrawState::ALPHA_BLEND_MODE& alphaBlendMode);
		CRegistInfo*               SetInterpolationMode(const _RNC_DrawState::INTERPOLATION_MODE& interpolationMode);

	private:
		// <<< ��{��� >>>
		short  m_idx;
		Matrix m_mtx;

		// <<< ���_�ʒu��� >>>
		enum class SET_VTX_POS_INFO_TYPE { NONE, NORMAL, SIZE, MAX, };
		struct SetVtxPosInfo {
			Pos3D vtxPoses[4];
		};
		struct SetSizeInfo {
			float width;
			float height;
		};
		void*                 m_setVtxPosInfo;
		SET_VTX_POS_INFO_TYPE m_setVtxPosInfoType;

		// <<< ���_�@����� >>>
		struct SetVtxNorInfo {
			Pos3D vtxNors[4];
		};
		SetVtxNorInfo* m_setVtxNorInfo;

		// <<< �����ڏ�� >>>
		Color                          m_vtxCols[4];
		Polygon2DAnd3D::CSetTexInfoSum m_setTexInfoSum;

		// <<< �`����ݒ� >>>
		bool                               m_isZtest;
		bool                               m_isLighting;
		bool                               m_isBillboard;
		_RNC_DrawState::CULLING_MODE       m_cullingMode;
		_RNC_DrawState::ALPHA_BLEND_MODE   m_alphaBlendMode;
		_RNC_DrawState::INTERPOLATION_MODE m_interpolationMode;
	};

	//========== [[[ �֐��錾 ]]]
	_RNC_Polygon3D     ();
	~_RNC_Polygon3D    ();
	void         Init  (void);
	void         Uninit(void);
	void         Update(void);
	CRegistInfo* Put   (const UShort& priority, const Matrix& mtx, const bool& isOnScreen = false);
	CRegistInfo* Put   (const UShort& priority, const Pos3D& pos, const Rot3D& rot, const bool& isOnScreen = false);
};