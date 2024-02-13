#pragma once


#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <vector>

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

using namespace std;

class Vertex
{
public:
	float x, y, z;
	float r, g, b, a;
};

class Render
{
	const Vertex*	mVertices;
	const int*		mIndices;
	const int 		mIndexCount;		
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

	void PushVertexBuffer(Vertex* vertices, int* indices, int indexCount);
protected:
	GdiplusStartupInput mGdiplusStartupInput;
	ULONG_PTR			mGdiplusToken				= NULL;
	Gdiplus::Graphics*	mGraphics					= nullptr;
	Gdiplus::Pen*		mPen						= nullptr;

	bool bIsInitialized = false;
	bool bIsReleased	= false;

	vector<RenderBuffer> mRenderBuffer;
};