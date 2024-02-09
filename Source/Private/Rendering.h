#pragma once

#include <Windows.h>
#include <gdiplus.h>

#pragma comment (lib,"Gdiplus.lib")

using namespace Gdiplus;


class Vertex
{
public:
	float x, y, z;
	float r, g, b, a;
};

class Renderer
{
public:
	bool Initialize(HWND hwnd);
	void Release();
	void OnPaint(HDC hdc);

protected:
	GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR			m_gdiplusToken;
	Gdiplus::Graphics*	m_pGraphics;
	Gdiplus::Pen*		m_pPen;

	bool bIsInitialized = false;


	void DrawVertices(Vertex* vertices, int numVertices);
};