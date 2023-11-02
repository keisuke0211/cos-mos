//========================================
// 
// セットアップ3Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNLib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]セットアップ3Dクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CSetUp3D::CSetUp3D() {

	m_datas = NULL;
}

//========================================
// デストラクタ
//========================================
CSetUp3D::~CSetUp3D() {

	// データの解放
	RNLib::Memory().Release<CData>(&m_datas);
}

//========================================
// 読み込み処理
//========================================
short CSetUp3D::Load(const char* loadPath, short idx) {

	const unsigned short numOld(m_num);

	if (CRegist::Load(loadPath, idx))
	{// 読み込み成功
		// データのメモリ再確保
		RNLib::Memory().ReAlloc(&m_datas, numOld, m_num);

		// データの破棄(番号指定の場合)
		if (idx != NONEDATA)
			m_datas[idx].Release();

		// ファイルを開く
		if (RNLib::File().OpenLoadFile(loadPath)) {
			while (RNLib::File().SearchLoop("END")) {
				if (RNLib::File().CheckIdentifier("boneDataSum{")) {
					BoneData*& boneDatas = m_datas[idx].m_boneDatas;
					
					// ボーンデータ群を解放
					RNLib::Memory().Release(&boneDatas);

					// ボーンデータ数読み込み
					RNLib::File().Scan(CFile::SCAN::INT, &m_datas[idx].m_boneDataNum);

					// ボーンデータ群のメモリ確保
					RNLib::Memory().Alloc(&boneDatas, m_datas[idx].m_boneDataNum);

					// ボーンデータ群の読み込み
					int cntBoneData(0);
					while (RNLib::File().SearchLoop("}")) {
						if (RNLib::File().CheckIdentifier("boneData{")) {

							// エラーメッセージ
							if (cntBoneData >= m_datas[idx].m_boneDataNum) {
								RNLib::Window().Message_ERROR(CreateText("ボーンデータの数が指定数をオーバーしています。\n%s", loadPath));
								break;
							}

							// ボーンデータ初期化
							BoneData& boneData(boneDatas[cntBoneData]);
							boneData = {};

							// 部品情報の読み込みを開始
							while (RNLib::File().SearchLoop("}")) {
								RNLib::File().Scan(CFile::SCAN::INT, &boneData.idx, "idx");
								RNLib::File().Scan(CFile::SCAN::MODELIDX, &boneData.modelIdx, "modelIdx");
								RNLib::File().Scan(CFile::SCAN::INT, &boneData.parentIdx, "parentIdx");
								RNLib::File().Scan(CFile::SCAN::POS3D, &boneData.relativePos, "relativePos");
								RNLib::File().Scan(CFile::SCAN::POS3D, &boneData.relativeRot, "relativeRot");
							}

							// カウントを加算
							cntBoneData++;
						}
					}// ボーンデータ群の読み込み終了…

					// エラーメッセージ
					if (cntBoneData < m_datas[idx].m_boneDataNum) {
						RNLib::Window().Message_ERROR(CreateText("ボーンデータの数が指定数に対して不足しています。\n%s", loadPath));
					}
					
					// ボーンデータ1つ1つの親番号をカウントに変換していく
					for (int cntBoneData = 0; cntBoneData < m_datas[idx].m_boneDataNum; cntBoneData++) {
						BoneData& boneData(boneDatas[cntBoneData]);

						// 親がDATANONEの時、折り返す
						if (boneData.parentIdx == NONEDATA)
							continue;

						// 親番号と一致したボーンデータを親とし、
						// ボーンデータの親番号をカウントに変換する
						int cntBoneData2;
						for (cntBoneData2 = 0; cntBoneData2 < m_datas[idx].m_boneDataNum; cntBoneData2++) {
							if (boneData.parentIdx == boneDatas[cntBoneData2].idx) {
								boneData.parentIdx = cntBoneData2;
								break;
							}
						}

						// 親が見つからなかった時、親番号をDATANONEにする
						if (cntBoneData2 == m_datas[idx].m_boneDataNum)
							boneData.parentIdx = NONEDATA;
					}
				}
			}

			// ファイルを閉じる
			RNLib::File().CloseFile();
		}
		else 
		{// 読み込み失敗
			// エラーメッセージ
			RNLib::Window().Message_ERROR(CreateText("セットアップ3Dデータファイルが存在しません。\n%s", loadPath));

			return NONEDATA;
		}
	}

	return idx;
}

//========================================
// 書き出し処理
//========================================
void CSetUp3D::Save(const char* path, short idx) {

}

//========================================
// メモリを指定数に初期化
//========================================
void CSetUp3D::InitMemory(const unsigned short& num) {
	CRegist::InitMemory(num);

	// データのメモリ確保
	RNLib::Memory().Alloc(&m_datas, num);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]データクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CSetUp3D::CData::CData() {

	m_boneDatas   = NULL;
	m_boneDataNum = 0;
}

//========================================
// デストラクタ
//========================================
CSetUp3D::CData::~CData() {

	// 解放処理
	Release();
}

//========================================
// 解放処理
//========================================
void CSetUp3D::CData::Release(void) {

	RNLib::Memory().Release(&m_boneDatas);
}