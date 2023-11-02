//========================================
// 
// ポリゴン2Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

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
		CDrawInfo();
		~CDrawInfo();
		void Draw(Device& device, const Matrix& viewMtx);
		static void InitCreateVertexBuffer(void);
		static void CreateVertexBuffer(const unsigned short& num);
		static void ReleaseVertexBuffer(void);

		// [[[ 変数宣言 ]]]
		short    m_idx;
		short    m_texIdx;
		CCamera* m_texCamera;
		bool     m_isZTest;
		float    m_distance;
		Vertex2D m_vtxs[4];

		// [[[ 静的変数宣言 ]]]
		static LPDIRECT3DVERTEXBUFFER9 m_vtxBuff;
		static unsigned short          m_allocPower;
		static unsigned short          m_allocNum;
		static unsigned short          m_idxCount;
	};

	// 登録情報
	class CRegistInfo {
	public:
		// [[[ 関数宣言 ]]]
		CRegistInfo();
		~CRegistInfo();
		void ClearParameter(void);
		CPolygon2D::CDrawInfo* ConvToDrawInfo(void);
		CRegistInfo* SetIdx              (const short& idx);
		CRegistInfo* SetPos              (const Pos3D& pos);
		CRegistInfo* SetAngle            (const Angle& angle);
		CRegistInfo* SetCol              (const Color& col);
		CRegistInfo* SetTex              (const short& texIdx, const unsigned short& ptn = 0, const unsigned short& ptnX = 1, const unsigned short& ptnY = 1, const Pos2D& ptnPos = INITPOS2D);
		CRegistInfo* SetTex_Camera       (CCamera* camera);
		CRegistInfo* SetSize             (const float& width, const float& height);
		CRegistInfo* SetSize_TexBaseScale(const float& scaleX, const float& scaleY);
		CRegistInfo* ExtendFixedTexX     (const float& rateX);
		CRegistInfo* ExtendFixedTexY     (const float& rateY);
		CRegistInfo* SetZTest            (const bool& isZTest);
		CRegistInfo* SetTexMirrorX       (const bool& isMirror);
		CRegistInfo* SetPriority         (const short& priority);

	private:
		// [[[ 変数宣言 ]]]
		short          m_idx;
		float          m_scaleX;
		float          m_scaleY;
		bool           m_isFactScale;
		Pos3D          m_pos;
		Angle          m_angle;
		Color          m_col;
		short          m_texIdx;
		CCamera*       m_texCamera;
		unsigned short m_ptn;
		unsigned short m_ptnX;
		unsigned short m_ptnY;
		float          m_ptnScaleX;
		float          m_ptnScaleY;
		Pos2D          m_ptnPos;
		bool           m_isZtest;
		bool           m_isTexMirrorX;
		short          m_priority;
	};

	//========== [[[ 関数宣言 ]]]
	CRegistInfo* Put(const Pos3D& pos, const Angle& angle, const bool& isOnScreen = false);
	// 頂点情報設定系
	static void SetVtxPos        (Vertex2D* vtxs, const Pos3D& pos, const Angle& angle, const float& width, const float& height);
	static void SetVtxPos        (Vertex2D* vtxs, const Pos3D& pos0, const Pos3D& pos1, const Pos3D& pos2, const Pos3D& pos3);
	static void SetVtxPos_TopLeft(Vertex2D* vtxs, const Pos3D& pos, const float& width, const float& height);
	static void ApplyResolution  (Vertex2D* vtxs);
	static void SetVtxRHW        (Vertex2D* vtxs);
	static void SetVtxCol        (Vertex2D* vtxs, const Color& col);
	static void SetVtxTex_Cut    (Vertex2D* vtxs, const Pos2D& cutPos, const float& width, const float& height);
	static void SetVtxTex_Cut    (Vertex2D* vtxs, const Pos2D& cutPos, const float& size);
};