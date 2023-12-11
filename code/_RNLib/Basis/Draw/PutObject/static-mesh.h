//========================================
// 
// �X�^�e�B�b�N���b�V���̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// �N���X��`
//****************************************
// �X�^�e�B�b�N���b�V���N���X
class CStaticMesh {
public:
	//========== [[[ �֐��錾 ]]]
	CStaticMesh           ();
	~CStaticMesh          ();
	void   Init           (const UShort& priorityMax);
	void   Uninit         (void);
	void   Update         (void);
	void   Draw           (Device& device, const UShort& priority, const short& cameraID, const bool& isCameraClipping, const bool& isOnScreen);
	void   Release        (void);
	void   Delete         (void);
	void   SetMaterialMesh(const UShort& priority, const Pos3D& pos, const Rot3D& rot, const Scale3D& scale, const short& modelIdx, const short& texIdx, const Color& col, const bool& isOnScreen = false);
	void   SetMaterialMesh(const UShort& priority, const Pos3D& pos, const Rot3D& rot,                       const short& modelIdx, const short& texIdx, const Color& col, const bool& isOnScreen = false);
	void   SetMaterialMesh(const UShort& priority, const Matrix& mtx,                                        const short& modelIdx, const short& texIdx, const Color& col, const bool& isOnScreen = false);
	UShort GetMeshNum     (void);

private:
	//========== [[[ �񋓌^��` ]]]
	// ���b�V���N���X
	class CMesh {
	public:
		// [[[ �֐��錾 ]]]
		CMesh();
		~CMesh();
		void Draw(Device& device);
		bool SetMesh(const Matrix& mtx, const short& modelIdx, const Color& col);

		// [[[ �ϐ��錾 ]]]
		short        m_texIdx;
		short        m_clippingID;
		bool         m_isOnScreen;
		VertexBuffer m_vtxBuff;
		UInt         m_vtxNum;
		IndexBuffer  m_idxBuff;
		UInt         m_idxNum;
	};

	//========== [[[ �ϐ��錾 ]]]
	CMesh*** m_meshes;
	UShort*  m_meshNums;
};