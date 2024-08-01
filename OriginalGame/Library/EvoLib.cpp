#include "EvoLib.h"
#include <cassert>


void EvoLib::Assert::ErrorMessage(const std::string& errorMessage)
{
#if _DEBUG

	// read me枠
	std::string startFrame = "\n///////// README /////////\n\n";
	// read me枠	
	std::string endFrame = "\n\n//////////////////////////////\n";

	// エラーメッセージ
	const std::string errorMsg = startFrame + errorMessage + endFrame;

	//ワイド文字列に変換
	WCHAR* _wtext = new WCHAR[strlen(errorMsg.c_str()) + 1];
	mbstowcs_s(nullptr, _wtext, strlen(errorMsg.c_str()) + 1, errorMsg.c_str(), _TRUNCATE);


	_wassert(_wtext, _CRT_WIDE(__FILE__), (unsigned)(__LINE__));
#endif
}
