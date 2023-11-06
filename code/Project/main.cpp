//========================================
// 
// ÉÅÉCÉìèàóù
// Author:RIKU NISHIMURA
// 
//========================================
#include "main.h"

//========================================
// WinMainä÷êî
// Author:RIKU NISHIMURA
//========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR IpCmdLine, int nCmdShow) {
	int nEndCode = 0;

	while (RNLib::MainLoop(hInstance, "Data\\RNSettings.txt")) {

		switch (RNLib::GetSignal()) {
		case RNLib::SIGNAL::INIT:
			Manager::Init(CMode::TYPE::TITLE);
			break;
		case RNLib::SIGNAL::UNINIT:
			Manager::Uninit();
			break;
		case RNLib::SIGNAL::UPDATE:
			Manager::Update();
			break;
		case RNLib::SIGNAL::LOAD:
			Manager::Load();
			break;
		case RNLib::SIGNAL::SAVE:
			Manager::Save();
			break;
		}
	}

	return nEndCode;
}
