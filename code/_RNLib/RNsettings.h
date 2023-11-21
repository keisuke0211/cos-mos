//========================================
// 
// RN���C�u�����̐ݒ�Ǘ�����
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// ���O���
//****************************************
namespace RNSettings {

	//========== [[[ �\���̒�` ]]]
	struct Info {
		// KeyOptions
		char* projectName             = NULL;
		float windowWidth             = 0.0f;
		float windowHeight            = 0.0f;
		bool  isFullScreen            = false;	
		float resolution              = 0.0f;
		// OtherOptions
		float modelOutLineAddDistance = 0.0f;
	};

	//========== [[[ �֐��錾 ]]]
	void Release(void);
	bool LoadAndSave(const char* path);
	Info GetInfo(void);
	void SetFulScreen(bool screen);
}