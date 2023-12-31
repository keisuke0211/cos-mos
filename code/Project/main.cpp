//========================================
// 
// メイン処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "main.h"
#include "manager.h"

//========================================
// WinMain関数
// Author:RIKU NISHIMURA
//========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR IpCmdLine, int nCmdShow) {
	int nEndCode = 0;

	while (RNSystem::MainLoop(hInstance, "data\\RNSettings.txt", "data\\RNOptionsData.txt", PRIORITY_MAX, RNSystem::MODE::EXECUTION)) {

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
		case RNSystem::SIGNAL::DRAW:
			Manager::Draw();
			break;
		}
	}

	return nEndCode;
}
