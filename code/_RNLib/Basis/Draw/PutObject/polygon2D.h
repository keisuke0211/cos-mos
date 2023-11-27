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

//****************************************
// �N���X��`
//****************************************
// �|���S��2D�N���X
class CPolygon2D {
public:
	//========== [[[ �N���X��` ]]]
	// �`����
	class CDrawInfo : public CDrawInfoBase {
	public:
		// [[[ �֐��錾 ]]]
		            CDrawInfo             ();
		            ~CDrawInfo            ();
		void        Draw                  (Device& device, const Matrix& viewMtx);
		static void InitCreateVertexBuffer(void);
		static void CreateVertexBuffer    (const UShort& num);
		static void ReleaseVertexBuffer   (void);

		// [[[ �ϐ��錾 ]]]
		short                    m_idx;
		void*                    m_tex;
		Polygon2DAnd3D::TEX_TYPE m_texType;
		bool                     m_isZTest;
		float                    m_distance;
		Vertex2D                 m_vtxs[4];

		// [[[ �ÓI�ϐ��錾 ]]]
		static VertexBuffer m_vtxBuff;
		static UShort       m_allocPower;
		static UShort       m_allocNum;
		static UShort       m_idxCount;
	};

	// �o�^���
	class CRegistInfo : public CRegistInfoBase {
	public:
		// [[[ �֐��錾 ]]]
		CRegistInfo();
		~CRegistInfo();
		void ClearParameter(void);
		CPolygon2D::CDrawInfo* ConvToDrawInfo(void);
		// <<< ��{���ݒ� >>>
		CRegistInfo* SetIdx        (const short& idx);
		CRegistInfo* SetPos        (const Pos3D& pos);
		CRegistInfo* SetAngle      (const Angle& angle);
		// <<< ���_�ʒu���ݒ� >>>
		CRegistInfo* SetVtxPos     (const Pos2D pos0, const Pos2D pos1, const Pos2D pos2, const Pos2D pos3);
		CRegistInfo* SetSize       (const float& width, const float& height);
		// <<< �F���ݒ� >>>
		CRegistInfo* SetCol        (const Color& col);
		// <<< �e�N�X�`���ݒ� >>>
		CRegistInfo* SetTex        (const short& texIdx, const UShort& ptn = 0, const UShort& ptnX = 1, const UShort& ptnY = 1, const Pos2D& ptnPos = INITPOS2D);
		CRegistInfo* SetTex        (CCamera* camera,     const UShort& ptn = 0, const UShort& ptnX = 1, const UShort& ptnY = 1, const Pos2D& ptnPos = INITPOS2D);
		CRegistInfo* SetTexUV      (const short& texIdx, const Pos2D& pos0 = Pos2D(0.0f, 0.0f), const Pos2D& pos1 = Pos2D(1.0f, 0.0f), const Pos2D& pos2 = Pos2D(0.0f, 1.0f), const Pos2D& pos3 = Pos2D(1.0f, 1.0f));
		CRegistInfo* SetTexUV      (CCamera* camera,     const Pos2D& pos0 = Pos2D(0.0f, 0.0f), const Pos2D& pos1 = Pos2D(1.0f, 0.0f), const Pos2D& pos2 = Pos2D(0.0f, 1.0f), const Pos2D& pos3 = Pos2D(1.0f, 1.0f));
		CRegistInfo* SetTexMirrorX (const bool& isMirror);
		// <<< �`����ݒ� >>>
		CRegistInfo* SetZTest      (const bool& isZTest);

	private:
		// <<< ��{��� >>>
		short m_idx;
		Pos3D m_pos;

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
		void* m_setVtxPosInfo;
		SET_VTX_POS_INFO_TYPE m_setVtxPosInfoType;

		// <<< �F�ݒ� >>>
		Color m_col;

		// <<< �e�N�X�`���ݒ� >>>
		Polygon2DAnd3D::CSetTexInfoSum m_setTexInfoSum;

		// <<< �`����ݒ� >>>
		bool m_isZtest;
	};

	//========== [[[ �֐��錾 ]]]
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