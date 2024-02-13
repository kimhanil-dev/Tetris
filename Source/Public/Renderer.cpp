
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

	for (const auto& renderJob : mRenderQueue)
	{
		for (int index = 0; index < (renderJob.mIndexCount - 1); ++index)
		{
			PointF p1, p2;

			p1.X = renderJob.mVertices[renderJob.mIndices[index]].x + renderJob.mVertices[renderJob.mIndices[index]].z;
			p1.Y = renderJob.mVertices[renderJob.mIndices[index]].y + renderJob.mVertices[renderJob.mIndices[index]].z;

			p2.X = renderJob.mVertices[renderJob.mIndices[index + 1]].x + renderJob.mVertices[renderJob.mIndices[index + 1]].z;
			p2.Y = renderJob.mVertices[renderJob.mIndices[index + 1]].y + renderJob.mVertices[renderJob.mIndices[index + 1]].z;


			mGraphics->DrawLine(mPen,p1,p2);
		}
	}
}

void Renderer::PushVertexBuffer(const Vertex* vertices, const int* indices, const int indexCount)
{
	mRenderQueue.push_back({vertices, indices, indexCount});
}
