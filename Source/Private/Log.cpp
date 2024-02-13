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

	// NULL을 제외한 문자열 길이를 계산합니다.
	size_t size = _vsctprintf(format, args);
	// 새롭게 필요할 버퍼의 공간을 계산합니다.    NULL을 위한 공간
	size_t needSize = mLogBufferUsedSize + size + 1;

	if (mLogBufferSize <= (needSize))
	{
		/*
			버퍼가 증가 해야할 크기를 mLogBufferSize의 배수로 계산합니다.
			크기를 매번 증가하는 것이 아니라 한 번 증가할때 일정한 크기로 여유롭게 증가시켜
			성능을 향상시키고자 합니다.
		*/
		size_t overflow = (needSize - mLogBufferSize);
		size_t incresmentCount = (overflow / mBufferSpareSize) + 1;	// '1'은 최소한 한번은 증가해야 하기 때문에 더해줍니다.

		TCHAR* temp = mLogBuffer;

		mLogBufferSize += mBufferSpareSize * incresmentCount;
		mLogBuffer = new TCHAR[mLogBufferSize];

		_tcscpy_s(mLogBuffer, mLogBufferSize, temp);

		delete[] temp;
	}

	// 기록된 로그의 끝에 새로운 로그를 추가합니다.			
	TCHAR* catStart = mLogBuffer + mLogBufferUsedSize;
	_vstprintf_s(catStart, mLogBufferSize - mLogBufferUsedSize, format, args); // NULL이 추가되는 부분

	mLogBufferUsedSize = mLogBufferUsedSize + size;
	
	va_end(args);

	// 로그가 작성되었음을 전파합니다.
	for (auto callback : mLogCallbacks)
	{
		callback(catStart);
	}
}