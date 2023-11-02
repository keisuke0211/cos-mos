//========================================
// 
// メイン処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "main.h"

//========================================
// WinMain関数
// Author:RIKU NISHIMURA
//========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR IpCmdLine, int nCmdShow) {
	int nEndCode = 0;

	while (RNLib::MainLoop(
			&nEndCode,
			hInstance,
			"DigBug",	// クラス名
			"DigBug",	// ウィンドウ名
			1280,	// 幅
			720,	// 高さ
			false,	// ImGui使用
			2.0f	// 解像度倍率
			)) {

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
