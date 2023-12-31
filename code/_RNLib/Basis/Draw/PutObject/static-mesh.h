//========================================
// 
// スタティックメッシュの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// クラス定義
//****************************************
// スタティックメッシュクラス
class _RNC_StaticMesh {
public:
	//========== [[[ 関数宣言 ]]]
	_RNC_StaticMesh        ();
	~_RNC_StaticMesh       ();
	void   Init            (const UShort& priorityMax);
	void   Uninit          (void);
	void   Update          (void);
	void   Draw            (Device& device, const UShort& priority, const short& cameraID, const bool& isCameraClipping, const bool& isOnScreen);
	void   Release         (void);
	void   Delete          (const bool& isOnScreen);
	void   SetModel        (const UShort& priority, const Pos3D& pos, const Rot3D& rot, const Scale3D& scale, const short& modelIdx, const Color& col, const bool& isOnScreen = false);
	void   SetModel        (const UShort& priority, const Pos3D& pos, const Rot3D& rot,                       const short& modelIdx, const Color& col, const bool& isOnScreen = false);
	void   SetModel        (const UShort& priority, const Matrix& mtx,                                        const short& modelIdx, const Color& col, const bool& isOnScreen = false);
	void   SetMaterialModel(const UShort& priority, const Pos3D& pos, const Rot3D& rot, const Scale3D& scale, const short& modelIdx, const short& texIdx, const Color& col, const bool& isOnScreen = false);
	void   SetMaterialModel(const UShort& priority, const Pos3D& pos, const Rot3D& rot,                       const short& modelIdx, const short& texIdx, const Color& col, const bool& isOnScreen = false);
	void   SetMaterialModel(const UShort& priority, const Matrix& mtx,                                        const short& modelIdx, const short& texIdx, const Color& col, const bool& isOnScreen = false);
	UShort GetMeshNum      (void);

private:
	//========== [[[ 関数定義 ]]]
	void NewCreateMesh(const UShort& priority, const Matrix& mtx, const short& modelIdx, const short& texIdx, const short& matIdx, const Color& col, const bool& isOnScreen);

	//========== [[[ 列挙型定義 ]]]
	// メッシュクラス
	class CMesh {
	public:
		// [[[ 関数宣言 ]]]
		CMesh        ();
		~CMesh       ();
		void Draw    (Device& device);
		bool SetModel(const Matrix& mtx, const short& modelIdx, const Color& col, const short& matIdx, const short& matNum);

		// [[[ 変数宣言 ]]]
		short        m_texIdx;
		short        m_clippingID;
		VertexBuffer m_vtxBuff;
		ULong        m_vtxNum;
		IndexBuffer  m_idxBuff;
		ULong        m_idxNum;
	};

	//========== [[[ 変数宣言 ]]]
	CMesh*** m_meshes;
	UShort*  m_meshNums;
	CMesh*** m_meshesScreen;
	UShort*  m_meshNumsScreen;
};