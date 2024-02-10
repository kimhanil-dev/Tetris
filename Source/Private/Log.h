#pragma once

#include <vector>
#include <tchar.h>
#include <stdarg.h>

using namespace std;

#define DEFAULT_LOG_FORMAT_T(format) _T("[file : %s], [line : %d], [function : %s] : {"##format"}\n")
#define LOG(format, ...) Logger::GetInstance()->Log(DEFAULT_LOG_FORMAT_T(##format), _T(__FILE__), __LINE__, _T(__FUNCTION__), __VA_ARGS__)


/*
* 로그를 기록하는 클래스입니다.
*/

class Logger
{
private:
	Logger();
	~Logger();

public:

	static Logger* GetInstance()
	{
		static Logger instance;
		return &instance;
	}

	/*
		TODO : 로그 버퍼의 초기 사이즈를 설정할 수 있도록 사용자에게
		함수를 제공해야 합니다. 
		조건 : 별도로 초기 사이즈를 지정 하지 않아도, 정상 작동해야 하며,
		이미 로그가 작성되기 시작한 시점에는 변경 불가능 해야 합니다.
	*/

	// 버퍼 크기가 부족할 경우 자동으로 증가할 크기
	size_t mBufferSpareSize = 256;

	TCHAR* mLogBuffer = nullptr;
	size_t mLogBufferSize = 1024;
	size_t mLogBufferUsedSize = 0;	// NULL 문자를 제외한 문자열의 길이

	/*
	* 지금까지 기록된 로그를 반환합니다.
	*/
	size_t GetLog(TCHAR** buffer);

	void Log(const TCHAR* format, ...);
};