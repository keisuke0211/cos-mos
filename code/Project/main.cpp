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

	while (RNSystem::MainLoop(hInstance, "Data\\RNSettings.txt", false)) {

		switch (RNSystem::GetSignal()) {
		case RNSystem::SIGNAL::INIT:
			Manager::Init(CMode::TYPE::TITLE);
			break;
		case RNSystem::SIGNAL::UNINIT:
			Manager::Uninit();
			break;
		case RNSystem::SIGNAL::UPDATE:
			Manager::Update();
			break;
		}
	}

	return nEndCode;
}
