
#include <assert.h>

#include "Renderer.h"
#include "Log.h"

Renderer::Renderer()
{
	
}

Renderer::~Renderer()
{
	if (bIsInitialized && !bIsReleased)
	{
		Release();
	}
}

bool Renderer::Initialize(HWND hwnd)
{
	if (bIsInitialized)
	{
		LOG(_T("Renderer is already initialized."));
		return false;
	}

	GdiplusStartup(&mGdiplusToken, &mGdiplusStartupInput, NULL);
	mGraphics	= new Gdiplus::Graphics(hwnd);
	mPen		= new Gdiplus::Pen(Gdiplus::Color(255, 0, 0, 0));

	LOG(_T("Renderer initialized"));

	return true;
}

void Renderer::Release()
{
	if (!bIsInitialized)
	{
		return;
	}

	delete mGraphics;
	delete mPen;
	GdiplusShutdown(mGdiplusToken);

	bIsReleased = true;

	LOG(_T("Renderer released"));
}

void Renderer::OnPaint(HDC hdc)
{
	if (mGraphics == nullptr)
	{
		return;
	}

	for (int i = 0; i < mVertexBuffers.size(); i++)
	{
		Vertex* vertices	= mVertexBuffers[i];
		int numVertices		= mVertexBufferSizes[i];

		for (int j = 0; j < (numVertices - 1); j++)
		{
			mGraphics->DrawLine(mPen, vertices[j].x, vertices[j].y, vertices[j + 1].x, vertices[j + 1].y);
		}
	}
}

void Renderer::PushVertexBuffer(Vertex* vertices, int* indices, int indexCount)
{
	mRenderBuffer.push_back(RenderBuffer(vertices, indices, indexCount));
}
