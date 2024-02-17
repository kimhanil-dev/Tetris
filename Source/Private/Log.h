#pragma once

#include <vector>
#include <tchar.h>
#include <stdarg.h>

using namespace std;

#define DEFAULT_LOG_FORMAT_T(format) _T("[file : %s], [line : %d], [function : %s] : {") format _T("}\n")
#define LOG_CALL(format, ...) Logger::GetInstance()->Log(DEFAULT_LOG_FORMAT_T(format), _T(__FILE__), __LINE__, _T(__FUNCTION__), ##__VA_ARGS__)
#define LOG(format, ...) Logger::GetInstance()->Log(format, ##__VA_ARGS__)

/*
* �α׸� ����ϴ� Ŭ�����Դϴ�.
*/

class Logger
{
private:
	Logger();
	~Logger();

protected:
	/*
		TODO : �α� ������ �ʱ� ����� ������ �� �ֵ��� ����ڿ���
		�Լ��� �����ؾ� �մϴ�. 
		���� : ������ �ʱ� ����� ���� ���� �ʾƵ�, ���� �۵��ؾ� �ϸ�,
		�̹� �αװ� �ۼ��Ǳ� ������ �������� ���� �Ұ��� �ؾ� �մϴ�.
	*/

	// ���� ũ�Ⱑ ������ ��� �ڵ����� ������ ũ��
	size_t mBufferSpareSize = 256;

	TCHAR* mLogBuffer = nullptr;
	size_t mLogBufferSize = 0;
	size_t mLogBufferUsedSize = 1024;	// NULL ���ڸ� ������ ���ڿ��� ����
	
	/*
		Log�� ������ ������ ȣ��Ǵ� �ݹ� �Լ��Դϴ�.
	*/
	typedef void(*LogCallback)(const TCHAR* log);
	vector<LogCallback> mLogCallbacks;

public:
	
	static Logger* GetInstance()
	{
		static Logger instance;
		return &instance;
	}

	void AddCallback(LogCallback callback);

	/*
	* ���ݱ��� ��ϵ� �α׸� ��ȯ�մϴ�.
	*/
	size_t GetLog(TCHAR** buffer);

	/*
	* �α� �ۼ�
	*/
	void Log(const TCHAR* format, ...);
};