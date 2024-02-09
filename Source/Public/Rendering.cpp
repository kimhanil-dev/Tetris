
#include "Rendering.h"
#include <assert.h>

bool Renderer::Initialize(HWND hwnd)
{
	if (bIsInitialized)
	{
		
		return false;
	}

	GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);
	m_pGraphics = new Gdiplus::Graphics(hwnd);
	m_pPen		= new Gdiplus::Pen(Gdiplus::Color(255, 0, 0, 0));
}

void Renderer::Release()
{
	delete m_pGraphics;
	delete m_pPen;
	GdiplusShutdown(m_gdiplusToken);
}

void Renderer::OnPaint(HDC hdc)
{
	if (m_pGraphics == nullptr)
	{

	}
}

void Renderer::DrawVertices(Vertex* vertices, int numVertices)
{
}
