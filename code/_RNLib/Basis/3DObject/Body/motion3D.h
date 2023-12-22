//========================================
// 
// ���[�V����3D�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "setup3D.h"
#include "../../Mechanical/regist.h"
#include "../../Calculation/ease.h"

//****************************************
// �N���X��`
//****************************************
class _RNC_Motion3D : public CRegist {
public:
	//========== [[[ �񋓌^��` ]]]
	// �R�}���h
	enum class COMMAND {
		NONE = -1,
		MOVE,		// �ړ�
		SPIN,		// ��]
		SCALING,	// �g�k
		MAX,
	};

	//========== [[[ �\���̒�` ]]]
	// �R�}���h�\����
	struct CommandData_Move {
		UShort          time;
		Pos3D           targetPos;
		_RNC_Ease::TYPE easeType;
	};
	struct CommandData_Spin {
		UShort          time;
		Rot3D           targetRot;
		_RNC_Ease::TYPE easeType;
	};
	struct CommandData_Scaling {
		UShort          time;
		Scale3D         targetScale;
		_RNC_Ease::TYPE easeType;
	};
	struct CommandData {
		short   time    = 0;
		COMMAND command = COMMAND::NONE;
		void*   data    = NULL;
	};

	// �{�[�����[�V�����f�[�^�\����
	struct BoneMotionData {
		bool         isMove         = false;
		bool         isSpin         = false;
		bool         isScale        = false;
		UShort       commandDataNum = 0;
		CommandData* commandDatas   = NULL;
	};

	// �f�[�^�\����
	class CData {
	public:
		// [[[ �֐��錾 ]]]
		CData       ();
		~CData      ();
		void Release(void);

		// [[[ �ϐ��錾 ]]]
		short           loopTime        = 0;
		bool            isLoop          = false;
		UShort          boneNum         = 0;
		BoneMotionData* boneMotionDatas = NULL;
	};

	//========== [[[ �֐��錾 ]]]
	_RNC_Motion3D ();
	~_RNC_Motion3D();
	short  Load   (const char* loadPath, short idx = NONEDATA);
	CData& GetData(const short& idx) { return idx == EDITDATA ? *m_editData : (idx == NONEDATA ? *(CData*)(nullptr) : *m_datas[idx]); }

private:
	//========== [[[ �F�B�錾 ]]]
	friend class C3DObject;
	friend class CSetUp3DEditor;

	//========== [[[ �֐��錾 ]]]
	void Init         (void);
	void Uninit       (void);
	void Update       (void);
	bool LoadEditData (const char* loadPath);
	void SaveEditData (const char* savePath);
	bool ExecutionLoad(const char* loadPath, CData& data);
	void InitMemory   (const UShort& num);

	//========== [[[ �ϐ��錾 ]]]
	CData** m_datas;
	CData*  m_editData;
};