#pragma once


#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <vector>

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

using namespace std;

struct Vertex
{
	float x, y, z;
	float r, g, b, a;
};

struct RenderJob
{
	RenderJob(const Vertex* vertices, const int vertexCount, const int* indices, const int indexCount)
		:mIndexCount(indexCount)
	{
		mVertices = new Vertex[vertexCount];
		mIndices = new int[indexCount];
		memcpy((void*)mVertices, vertices, sizeof(Vertex) * vertexCount);
		memcpy((void*)mIndices, indices, sizeof(int) * indexCount);
	}

	~RenderJob()
	{
		if(mVertices != nullptr)
		{
			delete[] mVertices;
			mVertices = nullptr;
		}

		if (mIndices != nullptr)
		{
			delete[] mIndices;
			mIndices = nullptr;
		}
		
	}

	const Vertex*	mVertices	= nullptr;
	const int*		mIndices	= nullptr;
	const int 		mIndexCount	= 0;		
};

class Renderer
{
public:
	Renderer();
	~Renderer();

public:
	bool Initialize(HWND hwnd);
	void Release();
	void OnPaint(HDC hdc);

	void PushVertexBuffer(const Vertex* vertices, const int vertexCount, const int* indices, const int indexCount);
protected:
	GdiplusStartupInput mGdiplusStartupInput;
	ULONG_PTR			mGdiplusToken				= NULL;
	Gdiplus::Graphics*	mGraphics					= nullptr;
	Gdiplus::Pen*		mPen						= nullptr;

	bool bIsInitialized = false;
	bool bIsReleased	= false;

	vector<RenderJob> mRenderQueue;
};