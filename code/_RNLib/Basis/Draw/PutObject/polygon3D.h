//========================================
// 
// ポリゴン3Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "polygon.h"
#include "../draw-info.h"
#include "../draw-state.h"
#include "../../Draw/camera.h"

//****************************************
// クラス定義
//****************************************
// ポリゴン3Dクラス
class _RNC_Polygon3D {
public:
	//========== [[[ クラス定義 ]]]
	// 描画情報
	class CDrawInfo : public CDrawInfoBase {
	public:
		// [[[ 関数宣言 ]]]
		CDrawInfo();
		~CDrawInfo();
		static void InitCreateVertexBuffer(void);
		static void CreateVertexBuffer(const UShort& num);
		static void ReleaseVertexBuffer(void);

		// [[[ 変数宣言 ]]]
		static VertexBuffer      ms_vtxBuff;
		static UShort            ms_allocPower;
		static UShort            ms_allocNum;
		static UShort            ms_idxCount;
		short                    m_idx;
		Matrix                   m_mtx;
		void*                    m_tex;
		Polygon2DAnd3D::TEX_TYPE m_texType;
		bool                     m_isZTest;
		bool                     m_isLighting;
		bool                     m_isBillboard;
		CULLING_MODE             m_cullingMode;
		ALPHA_BLEND_MODE         m_alphaBlendMode;
		INTERPOLATION_MODE       m_interpolationMode;
		float                    m_distance;
		Vertex3D                 m_vtxs[4];
	};

	// 登録情報
	class CRegistInfo : public CRegistInfoBase {
	public:
		// [[[ 関数宣言 ]]]
		CRegistInfo                      ();
		~CRegistInfo                     ();
		CRegistInfo* SetClippingCamera   (CCamera& camera); 
		CRegistInfo* SetClippingCamera   (const short& ID);
		CRegistInfo* SetVtxPos           (const Pos3D pos0, const Pos3D pos1, const Pos3D pos2, const Pos3D pos3);
		CRegistInfo* SetSize             (const float& width, const float& height);
		CRegistInfo* SetSize             (const Size2D& size);
		CRegistInfo* SetVtxNor           (const Vector3D nor0, const Vector3D nor1, const Vector3D nor2, const Vector3D nor3);
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
		CRegistInfo* SetZTest            (const bool& isZTest);
		CRegistInfo* SetLighting         (const bool& isLighting);
		CRegistInfo* SetBillboard        (const bool& isBillboard);
		CRegistInfo* SetCullingMode      (const CULLING_MODE& cullingMode);
		CRegistInfo* SetAlphaBlendMode   (const ALPHA_BLEND_MODE& alphaBlendMode);
		CRegistInfo* SetInterpolationMode(const INTERPOLATION_MODE& interpolationMode);

	private:
		// [[[ 友達宣言 ]]]
		friend class _RNC_DrawMgr;

		// [[[ 関数宣言 ]]]
		void                       ClearParameter(void);
		_RNC_Polygon3D::CDrawInfo* ConvToDrawInfo(void);
		CRegistInfo*               SetIdx        (const short& idx);
		CRegistInfo*               SetMtx        (const Matrix& mtx);

		// <<< 基本情報 >>>
		short  m_idx;
		Matrix m_mtx;

		// <<< 頂点位置情報 >>>
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

		// <<< 頂点法線情報 >>>
		struct SetVtxNorInfo {
			Pos3D vtxNors[4];
		};
		SetVtxNorInfo* m_setVtxNorInfo;

		// <<< 見た目情報 >>>
		Color                          m_vtxCols[4];
		Polygon2DAnd3D::CSetTexInfoSum m_setTexInfoSum;

		// <<< 描画情報設定 >>>
		bool               m_isZtest;
		bool               m_isLighting;
		bool               m_isBillboard;
		CULLING_MODE       m_cullingMode;
		ALPHA_BLEND_MODE   m_alphaBlendMode;
		INTERPOLATION_MODE m_interpolationMode;
	};

	//========== [[[ 関数宣言 ]]]
	_RNC_Polygon3D     ();
	~_RNC_Polygon3D    ();
	void         Init  (void);
	void         Uninit(void);
	void         Update(void);
	CRegistInfo* Put   (const UShort& priority, const Matrix& mtx, const bool& isOnScreen = false);
	CRegistInfo* Put   (const UShort& priority, const Pos3D& pos, const Rot3D& rot, const bool& isOnScreen = false);
};