//========================================
// 
// csvファイル入出力処理
// 
//========================================
// *** csv_file.h ***
//========================================
#ifndef _CSV_FILE_H_	// このマクロ定義がされていなかった時
#define _CSV_FILE_H_	// 二重インクルード防止のマクロを定義する

#pragma once
#include <iostream>	// 外部の装置と入出力
#include <fstream>	// ファイル の入出力
#include <vector>
#include <string>
#include <sstream>	// カンマで区切るために必要

using namespace std;

//****************************************
// クラス
//****************************************
class CSVFILE
{
public:
	
	// ***** 列挙型 *****

	// 端数処理
	enum FORMAT
	{
		FORMAT_ROUNF,	// 四捨五入
		FORMAT_CEIL,	// 切り上げ
		FORMAT_FLOOR,	// 切り捨て
		FORMAT_MAX,
	};

	// ***** コンストラクタ・デストラクタ *****
	CSVFILE();
	virtual~CSVFILE();

	// ***** 関数 *****
	/* 読み込み			*/void FileLood(string filepath, bool bHeader, bool bIndex, char delim);
	/* 書き出し			*/void FileSave(string filepath, char delim);
	/* コンソール出力	*/void FileShow(void);

	// -- 端数処理 -------------------------------------------------------------------
	/* float	*/float Format(float val, float nLen, FORMAT format);
	/* double	*/double Format(double val, float nLen, FORMAT format);

	// -- 変換 -------------------------------------------------------------------
	/* 文字列 → int		*/bool ToValue(int &val, const string &str);
	/* 文字列 → float		*/bool ToValue(float &val, const string &str);
	/* 文字列 → double		*/bool ToValue(double &val, const string &str);
	/* 文字列 → char		*/bool ToValue(char* &val, const string &str);

	/* int		→ 文字列	*/string CastCell(int &val) { return to_string(val); }
	/* float	→ 文字列	*/string CastCell(float &val){ return to_string(Format(val, 2, FORMAT_FLOOR)); }	
	/* double	→ 文字列	*/string CastCell(double &val) { return to_string(val); }
	/* char		→ 文字列	*/string CastCell(char &val) { return &val; }
	/* char*	→ 文字列	*/string CastCell(char* &val) { return * &val; }

	// -- 設定 -----------------------------------------
	/* ヘッダー		*/void SetHeader(string str, char delim);
	/* インデックス	*/void SetIndex(string str, char delim);

	// -- 設定・追加 ------------------------------------------
	/* 要素	*/void SetCell(string str, int row, int line);

	// -- 取得 ------------------------------------------
	/* 行数	*/int GetRowSize(void) { return cell.size(); }
	/* 列数	*/int GetLineSize(int row) { return cell.at(row).size(); }
	/* 要素 */string GetData(int row, int line) { return cell.at(row).at(line); }

private:

	// ***** 変数 *****
	string filepath;	// ファイルパス
	bool bHeader;		// ヘッダーの有無
	bool bIndex;		// インデックスの有無
	char delim;			// 区切り文字

	/* 保管 */
	vector<string> header;			// ヘッダー名
	vector<string> index;			// インデックス名
	vector<vector<string>> cell;	// 要素
};

#endif