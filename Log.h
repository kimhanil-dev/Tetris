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
* ���ݱ��� ��ϵ� �α׸� ��ȯ�մϴ�.
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

	// NULL�� ������ ���ڿ� ���̸� ����մϴ�.
	size_t size = _vsctprintf(format, args);

	if (g_LogBuffer == nullptr)
	{
		g_LogBuffer = new TCHAR[LOG_BUFFER_SIZE];
	}

	if (g_LogBufferSize <= size)
	{
		/*
			�����ؾ� �� ũ�⸦ ���
		*/
		size_t overflow = (size - g_LogBufferSize);
		size_t incresmentCount = (overflow / LOG_BUFFER_INCRESMENT_SIZE) + 1;	// '1'�� �ּ��� �ѹ��� �����ؾ� �ϱ� ������ �����ݴϴ�.

		TCHAR* temp = g_LogBuffer;

		g_LogBufferSize = size + (LOG_BUFFER_INCRESMENT_SIZE * incresmentCount);
		g_LogBuffer = new TCHAR[g_LogBufferSize];

		_tcscpy_s(g_LogBuffer, size, temp);

		delete[] temp;
	}

	// ��ϵ� �α��� ���� ���ο� �α׸� �߰��մϴ�.
	TCHAR* catStart = (g_LogBuffer + size) + 1;
	_vstprintf_s(catStart, g_LogBufferSize - size, format, args);

	va_end(args);
}