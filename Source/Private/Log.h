#pragma once

#include <vector>
#include <tchar.h>
#include <stdarg.h>

using namespace std;

#define DEFAULT_LOG_FORMAT_T(format) _T("[file : %s], [line : %d], [function : %s] : {"##format"}\n")
#define LOG(format, ...) Logger::GetInstance()->Log(DEFAULT_LOG_FORMAT_T(##format), _T(__FILE__), __LINE__, _T(__FUNCTION__), __VA_ARGS__)


/*
* �α׸� ����ϴ� Ŭ�����Դϴ�.
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
		TODO : �α� ������ �ʱ� ����� ������ �� �ֵ��� ����ڿ���
		�Լ��� �����ؾ� �մϴ�. 
		���� : ������ �ʱ� ����� ���� ���� �ʾƵ�, ���� �۵��ؾ� �ϸ�,
		�̹� �αװ� �ۼ��Ǳ� ������ �������� ���� �Ұ��� �ؾ� �մϴ�.
	*/

	// ���� ũ�Ⱑ ������ ��� �ڵ����� ������ ũ��
	size_t mBufferSpareSize = 256;

	TCHAR* mLogBuffer = nullptr;
	size_t mLogBufferSize = 1024;
	size_t mLogBufferUsedSize = 0;	// NULL ���ڸ� ������ ���ڿ��� ����

	/*
	* ���ݱ��� ��ϵ� �α׸� ��ȯ�մϴ�.
	*/
	size_t GetLog(TCHAR** buffer);

	void Log(const TCHAR* format, ...);
};