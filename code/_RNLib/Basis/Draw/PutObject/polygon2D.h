//========================================
// 
// ポリゴン2Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "polygon.h"
#include "../regist-info.h"
#include "../draw-info.h"
#include "../../Draw/camera.h"

//****************************************
// クラス定義
//****************************************
// ポリゴン2Dクラス
class CPolygon2D {
public:
	//========== [[[ クラス定義 ]]]
	// 描画情報
	class CDrawInfo : public CDrawInfoBase {
	public:
		// [[[ 関数宣言 ]]]
		            CDrawInfo             ();
		            ~CDrawInfo            ();
		void        Draw                  (Device& device, const Matrix& viewMtx);
		static void InitCreateVertexBuffer(void);
		static void CreateVertexBuffer    (const UShort& num);
		static void ReleaseVertexBuffer   (void);

		// [[[ 変数宣言 ]]]
		short                    m_idx;
		void*                    m_tex;
		Polygon2DAnd3D::TEX_TYPE m_texType;
		bool                     m_isZTest;
		float                    m_distance;
		Vertex2D                 m_vtxs[4];

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
		CPolygon2D::CDrawInfo* ConvToDrawInfo(void);
		// <<< 基本情報設定 >>>
		CRegistInfo* SetIdx        (const short& idx);
		CRegistInfo* SetPos        (const Pos3D& pos);
		CRegistInfo* SetAngle      (const Angle& angle);
		// <<< 頂点位置情報設定 >>>
		CRegistInfo* SetVtxPos     (const Pos2D pos0, const Pos2D pos1, const Pos2D pos2, const Pos2D pos3);
		CRegistInfo* SetSize       (const float& width, const float& height);
		// <<< 色情報設定 >>>
		CRegistInfo* SetCol        (const Color& col);
		// <<< テクスチャ設定 >>>
		CRegistInfo* SetTex        (const short& texIdx, const UShort& ptn = 0, const UShort& ptnX = 1, const UShort& ptnY = 1, const Pos2D& ptnPos = INITPOS2D);
		CRegistInfo* SetTex        (CCamera* camera,     const UShort& ptn = 0, const UShort& ptnX = 1, const UShort& ptnY = 1, const Pos2D& ptnPos = INITPOS2D);
		CRegistInfo* SetTexUV      (const short& texIdx, const Pos2D& pos0 = Pos2D(0.0f, 0.0f), const Pos2D& pos1 = Pos2D(1.0f, 0.0f), const Pos2D& pos2 = Pos2D(0.0f, 1.0f), const Pos2D& pos3 = Pos2D(1.0f, 1.0f));
		CRegistInfo* SetTexUV      (CCamera* camera,     const Pos2D& pos0 = Pos2D(0.0f, 0.0f), const Pos2D& pos1 = Pos2D(1.0f, 0.0f), const Pos2D& pos2 = Pos2D(0.0f, 1.0f), const Pos2D& pos3 = Pos2D(1.0f, 1.0f));
		CRegistInfo* SetTexMirrorX (const bool& isMirror);
		// <<< 描画情報設定 >>>
		CRegistInfo* SetZTest      (const bool& isZTest);

	private:
		// <<< 基本情報 >>>
		short m_idx;
		Pos3D m_pos;

		// <<< 頂点情報 >>>
		enum class SET_VTX_POS_INFO_TYPE { NONE, NORMAL, SIZE, MAX, };
		struct SetVtxPosInfo {
			Pos2D vtxPoses[4];
		};
		struct SetSizeInfo {
			float width;
			float height;
			Angle angle;
		};
		void* m_setVtxPosInfo;
		SET_VTX_POS_INFO_TYPE m_setVtxPosInfoType;

		// <<< 色設定 >>>
		Color m_col;

		// <<< テクスチャ設定 >>>
		Polygon2DAnd3D::CSetTexInfoSum m_setTexInfoSum;

		// <<< 描画情報設定 >>>
		bool m_isZtest;
	};

	//========== [[[ 関数宣言 ]]]
	static void SetVtxPos        (Vertex2D* vtxs, const Pos2D& pos, const Angle& angle, const float& width, const float& height);
	static void SetVtxPos        (Vertex2D* vtxs, const Pos2D& pos0, const Pos2D& pos1, const Pos2D& pos2, const Pos2D& pos3);
	static void SetVtxPos_TopLeft(Vertex2D* vtxs, const Pos2D& pos, const float& width, const float& height);
	static void ApplyResolution  (Vertex2D* vtxs);
	static void SetVtxRHW        (Vertex2D* vtxs);
	static void SetVtxCol        (Vertex2D* vtxs, const Color& col);
	static void SetVtxTex        (Vertex2D* vtxs);
	static void SetVtxTex_Cut    (Vertex2D* vtxs, const Pos2D& cutPos, const float& width, const float& height);
	static void SetVtxTex_Cut    (Vertex2D* vtxs, const Pos2D& cutPos, const float& size);
	CPolygon2D();
	~CPolygon2D();
	void Init  (void);
	void Uninit(void);
	void Update(void);
	CRegistInfo* Put(const UShort& priority, const Pos3D& pos, const Angle& angle, const bool& isOnScreen = false);
	CRegistInfo* Put(const UShort& priority, const Pos2D& pos, const Angle& angle, const bool& isOnScreen = false);
	CRegistInfo* Put(const UShort& priority, const bool& isOnScreen = false);
};