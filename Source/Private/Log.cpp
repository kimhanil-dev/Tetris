#include "Log.h"

Logger::Logger()
{
	mLogBuffer = new TCHAR[mLogBufferSize];
}

Logger::~Logger()
{
	if (mLogBuffer != nullptr)
	{
		mLogBufferSize = 0;
		delete[] mLogBuffer;
	}
}

void Logger::AddCallback(LogCallback callback)
{
	mLogCallbacks.push_back(callback);
}

size_t Logger::GetLog(TCHAR** buffer)
{
	if (mLogBuffer == nullptr)
	{
		return 0;
	}

	size_t size = _tcslen(mLogBuffer);
	if (size == 0)
	{
		return 0;
	}

	*buffer = new TCHAR[size + 1];
	_tcscpy_s(*buffer, size + 1, mLogBuffer);

	return size;
}

void Logger::Log(const TCHAR* format, ...)
{
	va_list args;
	va_start(args, format);

	// NULL�� ������ ���ڿ� ���̸� ����մϴ�.
	size_t size = _vsctprintf(format, args);
	// ���Ӱ� �ʿ��� ������ ������ ����մϴ�.    NULL�� ���� ����
	size_t needSize = mLogBufferUsedSize + size + 1;

	if (mLogBufferSize <= (needSize))
	{
		/*
			���۰� ���� �ؾ��� ũ�⸦ mLogBufferSize�� ����� ����մϴ�.
			ũ�⸦ �Ź� �����ϴ� ���� �ƴ϶� �� �� �����Ҷ� ������ ũ��� �����Ӱ� ��������
			������ ����Ű���� �մϴ�.
		*/
		size_t overflow = (needSize - mLogBufferSize);
		size_t incresmentCount = (overflow / mBufferSpareSize) + 1;	// '1'�� �ּ��� �ѹ��� �����ؾ� �ϱ� ������ �����ݴϴ�.

		TCHAR* temp = mLogBuffer;

		mLogBufferSize += mBufferSpareSize * incresmentCount;
		mLogBuffer = new TCHAR[mLogBufferSize];

		_tcscpy_s(mLogBuffer, mLogBufferSize, temp);

		delete[] temp;
	}

	// ��ϵ� �α��� ���� ���ο� �α׸� �߰��մϴ�.			
	TCHAR* catStart = mLogBuffer + mLogBufferUsedSize;
	_vstprintf_s(catStart, mLogBufferSize - mLogBufferUsedSize, format, args); // NULL�� �߰��Ǵ� �κ�

	mLogBufferUsedSize = mLogBufferUsedSize + size;
	
	va_end(args);

	// �αװ� �ۼ��Ǿ����� �����մϴ�.
	for (auto callback : mLogCallbacks)
	{
		callback(catStart);
	}
}