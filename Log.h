#pragma once

#include <vector>
#include <tchar.h>
#include <stdarg.h>

using namespace std;

#define LOG_BUFFER_SIZE 2048
#define LOG_BUFFER_INCRESMENT_SIZE 128

#define LOG(format, ...) Log("[file : %s], [line : %d], [function : %s], { " #format " }\n", __FILE__, __LINE__, __FUNCTION__, ...))

TCHAR* g_LogBuffer = nullptr;
size_t g_LogBufferSize = LOG_BUFFER_SIZE;

/*
* 지금까지 기록된 로그를 반환합니다.
*/
size_t GetLog(TCHAR** buffer)
{
	if (g_LogBuffer == nullptr)
	{
		return 0;
	}

	size_t size = _tcslen(g_LogBuffer);
	if (size == 0)
	{
		return 0;
	}

	*buffer = new TCHAR[size + 1];
	_tcscpy_s(*buffer, size + 1, g_LogBuffer);

	return size;
}

void Log(const TCHAR* format, ...)
{
	va_list args;
	va_start(args, format);

	// NULL을 제외한 문자열 길이를 계산합니다.
	size_t size = _vsctprintf(format, args);

	if (g_LogBuffer == nullptr)
	{
		g_LogBuffer = new TCHAR[LOG_BUFFER_SIZE];
	}

	if (g_LogBufferSize <= size)
	{
		/*
			증가해야 할 크기를 계산
		*/
		size_t overflow = (size - g_LogBufferSize);
		size_t incresmentCount = (overflow / LOG_BUFFER_INCRESMENT_SIZE) + 1;	// '1'은 최소한 한번은 증가해야 하기 때문에 더해줍니다.

		TCHAR* temp = g_LogBuffer;

		g_LogBufferSize = size + (LOG_BUFFER_INCRESMENT_SIZE * incresmentCount);
		g_LogBuffer = new TCHAR[g_LogBufferSize];

		_tcscpy_s(g_LogBuffer, size, temp);

		delete[] temp;
	}

	// 기록된 로그의 끝에 새로운 로그를 추가합니다.
	TCHAR* catStart = (g_LogBuffer + size) + 1;
	_vstprintf_s(catStart, g_LogBufferSize - size, format, args);

	va_end(args);
}