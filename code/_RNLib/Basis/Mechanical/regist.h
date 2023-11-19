//========================================
// 
// “o˜^ˆ—
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// ƒNƒ‰ƒX’è‹`
//****************************************
// “o˜^ƒNƒ‰ƒX
class CRegist {
public:
	//========== [[[ ŠÖ”éŒ¾ ]]]
	CRegist();
	virtual ~CRegist();
	virtual bool Load(const char* loadPath, short& idx);
	virtual void InitMemory(const UShort& num);
	const char* GetLoadPath(const short& idx) { return idx < 0 ? "NONEDATA" : m_loadPaths[idx]; }

protected:
	//========== [[[ ŠÖ”éŒ¾ ]]]
	void ReAllocLoadPath(const short& oldNum);

	//========== [[[ •Ï”éŒ¾ ]]]
	char** m_loadPaths;
	UShort m_num;
};