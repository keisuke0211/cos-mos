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
class CPolygon3D {
public:
	//========== [[[ クラス定義 ]]]
	// 描画情報
	class CDrawInfo : public CDrawInfoBase {
	public:
		// [[[ 関数宣言 ]]]
		CDrawInfo();
		~CDrawInfo();
		void Draw(Device& device, const Matrix& viewMtx);
		static void InitCreateVertexBuffer(void);
		static void CreateVertexBuffer(const UShort& num);
		static void ReleaseVertexBuffer(void);

		// [[[ 変数宣言 ]]]
		short                        m_idx;
		Matrix                       m_mtx;
		void*                        m_tex;
		Polygon2DAnd3D::TEX_TYPE     m_texType;
		bool                         m_isZTest;
		bool                         m_isLighting;
		bool                         m_isBillboard;
		CDrawState::CULLING_MODE     m_cullingMode;
		CDrawState::ALPHA_BLEND_MODE m_alphaBlendMode;
		float                        m_distance;
		Vertex3D                     m_vtxs[4];

		// [[[ 静的変数宣言 ]]]
		static VertexBuffer m_vtxBuff;
		static UShort       m_allocPower;
		static UShort       m_allocNum;
		static UShort       m_idxCount;
	};

	// 登録情報
	class CRegistInfo : public CRegistInfoBase {
	public:
		// [[[ 関数宣言 ]]]
		CRegistInfo();
		~CRegistInfo();
		void ClearParameter(void);
		CPolygon3D::CDrawInfo* ConvToDrawInfo(void);
		// <<< 基本情報設定 >>>
		CRegistInfo* SetIdx              (const short& idx);
		CRegistInfo* SetMtx              (const Matrix& mtx);
		// <<< 頂点位置情報設定 >>>
		CRegistInfo* SetVtxPos           (const Pos3D pos0, const Pos3D pos1, const Pos3D pos2, const Pos3D pos3);
		CRegistInfo* SetSize             (const float& width, const float& height);
		CRegistInfo* SetSize             (const Size2D& size);
		// <<< 頂点法線情報設定 >>>
		CRegistInfo* SetVtxNor           (const Vector3D nor0, const Vector3D nor1, const Vector3D nor2, const Vector3D nor3);
		// <<< 色情報設定 >>>
		CRegistInfo* SetCol              (const Color& col);
		CRegistInfo* SetVtxCol              (const Color col0, const Color col1, const Color col2, const Color col3);
		// <<< テクスチャ情報設定 >>>
		CRegistInfo* SetTex              (const short& texIdx, const UShort& ptn = 0, const UShort& ptnX = 1, const UShort& ptnY = 1, const Pos2D& ptnPos = INITPOS2D);
		CRegistInfo* SetTex              (CCamera* camera, const UShort& ptn = 0, const UShort& ptnX = 1, const UShort& ptnY = 1, const Pos2D& ptnPos = INITPOS2D);
		CRegistInfo* SetTexUV            (const short& texIdx, const Pos2D& pos0, const Pos2D& pos1, const Pos2D& pos2, const Pos2D& pos3);
		CRegistInfo* SetTexUV            (CCamera* camera, const Pos2D& pos0, const Pos2D& pos1, const Pos2D& pos2, const Pos2D& pos3);
		CRegistInfo* SetTexMirrorX       (const bool& isMirror);
		// <<< 描画情報設定 >>>
		CRegistInfo* SetZTest            (const bool& isZTest);
		CRegistInfo* SetLighting         (const bool& isLighting);
		CRegistInfo* SetBillboard        (const bool& isBillboard);
		CRegistInfo* SetCullingMode      (const CDrawState::CULLING_MODE& cullingMode);
		CRegistInfo* SetAlphaBlendMode   (const CDrawState::ALPHA_BLEND_MODE& alphaBlendMode);

	private:
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

		// <<< 色情報設定 >>>
		Color m_vtxCols[4];

		// <<< テクスチャ設定 >>>
		Polygon2DAnd3D::CSetTexInfoSum m_setTexInfoSum;

		// <<< 描画情報設定 >>>
		bool                         m_isZtest;
		bool                         m_isLighting;
		bool                         m_isBillboard;
		CDrawState::CULLING_MODE     m_cullingMode;
		CDrawState::ALPHA_BLEND_MODE m_alphaBlendMode;
	};

	//========== [[[ 関数宣言 ]]]
	CPolygon3D();
	~CPolygon3D();
	void Init(void);
	void Uninit(void);
	void Update(void);
	CRegistInfo* Put(const UShort& priority, const Matrix& mtx, const bool& isOnScreen = false);
	CRegistInfo* Put(const UShort& priority, const Pos3D& pos, const Rot3D& rot, const bool& isOnScreen = false);
};