//========================================
// 
// �o�^����
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// �N���X��`
//****************************************
// �o�^�N���X
class CRegist {
public:
	//========== [[[ �֐��錾 ]]]
	CRegist();
	virtual ~CRegist();
	virtual bool Load(const char* loadPath, short& idx);
	virtual void InitMemory(const UShort& num);

protected:
	//========== [[[ �֐��錾 ]]]
	void ReAllocLoadPath(const short& oldNum);

	//========== [[[ �ϐ��錾 ]]]
	char** m_loadPaths;
	UShort m_num;
};