
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
		LOG_CALL(_T("Renderer is already initialized."));
		return false;
	}

	GdiplusStartup(&mGdiplusToken, &mGdiplusStartupInput, NULL);
	mGraphics	= new Gdiplus::Graphics(hwnd);
	mPen		= new Gdiplus::Pen(Gdiplus::Color(255, 0, 0, 0));

	LOG_CALL(_T("Renderer initialized\n"));

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

	LOG_CALL(_T("Renderer released\n"));
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
			
			static const int POSITION_OFFSET = 500;

			p1.X = renderJob.mVertices[renderJob.mIndices[index]].x + POSITION_OFFSET;
			p1.Y = renderJob.mVertices[renderJob.mIndices[index]].y + POSITION_OFFSET;

			p2.X = renderJob.mVertices[renderJob.mIndices[index + 1]].x + POSITION_OFFSET;
			p2.Y = renderJob.mVertices[renderJob.mIndices[index + 1]].y + POSITION_OFFSET;


			mGraphics->DrawLine(mPen,p1,p2);
		}
	}

	mRenderQueue.clear();
}

void Renderer::PushVertexBuffer(const Vertex* vertices, const int vertexCount, const int* indices, const int indexCount)
{
	if(vertexCount == 0)
		return;

	mRenderQueue.emplace_back(vertices, vertexCount, indices, indexCount);
}
