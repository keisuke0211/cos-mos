//========================================
// 
// マテリアルメッシュの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// クラス定義
//****************************************
// マテリアルメッシュクラス
class CMatMesh {
public:
	//========== [[[ 関数宣言 ]]]
	CMatMesh         ();
	~CMatMesh        ();
	void   Init      (const UShort& priorityMax);
	void   Uninit    (void);
	void   Update    (void);
	void   Draw      (Device& device, const UShort& priority, const short& cameraID, const bool& isCameraClipping, const bool& isOnScreen);
	void   Release   (void);
	void   Delete    (void);
	void   SetMesh   (const UShort& priority, const Matrix& mtx, const short& modelIdx, const short& texIdx, const Color& col, const bool& isOnScreen);
	UShort GetMeshNum(void);

private:
	//========== [[[ 列挙型定義 ]]]
	// メッシュクラス
	class CMesh {
	public:
		// [[[ 関数宣言 ]]]
		CMesh();
		~CMesh();
		void Draw(Device& device);
		bool SetMesh(const Matrix& mtx, const short& modelIdx, const Color& col);

		// [[[ 変数宣言 ]]]
		short        m_texIdx;
		short        m_clippingID;
		bool         m_isOnScreen;
		VertexBuffer m_vtxBuff;
		UInt         m_vtxNum;
		IndexBuffer  m_idxBuff;
		UInt         m_idxNum;
	};

	//========== [[[ 変数宣言 ]]]
	CMesh*** m_meshes;
	UShort*  m_meshNums;
};