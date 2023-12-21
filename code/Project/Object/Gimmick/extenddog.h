//========================================
// 
// �L�т錢�̏���
// Author:KOMURO HIROMU
// Arrange:HIRASAWA SHION
// 
//========================================
#pragma once

#include "../stage-object.h"

//****************************************
// �N���X��`
//****************************************
// ���[�h�̏��\����
class CExtenddog : public CStageObject {
public:
	static const int MAX_COUNT = 40;	// �ő�J�E���g��
	static const int HIP_POS = 7;		// �K�̈ʒu

	//========== [[[ �񋓌^��` ]]]
	enum class STATE {
		NONE = 0,  //����Ă��Ȃ�
		RETURN,    //�߂�
		MAX,
	};

	//========== [[[ �֐��錾 ]]]
	CExtenddog();
	virtual      ~CExtenddog();
	virtual void Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void) { ; }

	// -- �ݒ� ---------------------------------------------
	void  SetState(STATE state)     { m_state = state; }      //��ސݒ�
	void  SetScale(Pos3D scale)     { m_scale = scale; }      //�g�k�ݒ�
	void  SetElasticity(bool Elast) { m_bElasticity = Elast; }//�L�k�ݒ�
	void  SetHead(Pos3D pos)        { m_HeadPos = pos; }      //���̈ʒu�ݒ�
	void  SetHip(Pos3D pos)         { m_HipPos = pos; }       //���K�̈ʒu�ݒ�
	void  SetHeadHeightMin(CInt heightMin) { m_nHeightMin = heightMin; } //���̍����i�Œ�
	void  SetHeadHeightMax(CInt heightMax) { m_nHeightMax = heightMax; } //���̍����i�ō�
	void  SetReturn(bool bReturn)   { m_bInversion = bReturn; }//���]�̐ݒ�

	// -- �擾 ---------------------------------------------
	STATE GetState(void)      { return m_state; }      //���
	STATE GetStateOld(void)	  { return m_stateOld; }   //�ߋ��̎��
	Pos3D GetHeadPos(void)    { return m_HeadPos; }    //���ʒu
	Pos3D GetBodyPos(void)    { return m_BodyPos; }    //�̈ʒu
	Pos3D GetBodyPosOld(void) { return m_BodyPosOld; } //�̈ʒu�i�O��
	Pos3D GetHipPos(void)     { return m_HipPos; }     //���K�ʒu
	bool  IsInversion(void)   { return m_bInversion; } //���]���Ă邩�ǂ���
	float GetColliHeight(void); //�����蔻��p�̍����擾
	int   GetHipRot(void); //���K�̕������擾

private:
	//���E���K�p�T�C�Y
	static const float HEAD_HIP_SIZE;

	//========== [[[ �񋓌^��` ]]]
	enum class Parts
	{
		Hip = 0, // ���K
		Body,    // ��
		Head,    // ��
		Max
	};
	//========== [[[ �֐��錾 ]]]
	void UpdateState_None(void);
	void UpdateState_Return(void);

	//========== [[[ �ϐ��錾 ]]]
	int   m_modelIdx[(int)Parts::Max]; // ���f���ԍ�
	static const char *MODEL_PATH[(int)Parts::Max];//���f���p�X
	STATE m_state;       // ���
	STATE m_stateOld;	 // �ߋ��̎��
	Pos3D m_scale;       // �傫��
	bool  m_bElasticity; // �G���ďk�ނ��L�т邩
	bool  m_bInversion;  // ���]
	int   m_nCntShrink;  // �k�ރJ�E���g
	int   m_nHeightMin;  // �L�т鍂���i�Œ�
	int   m_nHeightMax;  // �L�т鍂���i�ō�

	Pos3D m_HeadPos;     // ���̈ʒu
	Pos3D m_BodyPos;     // �̂̈ʒu
	Pos3D m_BodyPosOld;  // �̂̉ߋ��ʒu
	Pos3D m_HipPos;      // �K�̈ʒu

	Pos3D m_StartBodyPos;// �̂̏����ʒu
	float m_StartHeight; // �����̏����ʒu
};