//#pragma once
//
//#include <Windows.h>
//#include <math.h>
//#include <tchar.h>
//
//HDC   g_hMemDC = NULL;
//HDC   g_hBackDC = NULL;
//HBITMAP   g_hBackBMP = NULL;
//SIZE g_ClientSize;
//
//float x = 10.f;                     //	원의 x 좌표
//float y = 500.f;                    //	원의 y 좌표
//float diameter = 20.f;              //	원의 지름
//
//float Velocity = 300.f;				//	점프하는 힘
//float Gravity = 400.f;				//	낙하힘으로 상수값
//BOOL bJumpKeyPressed = FALSE;
//float JumpHeight = 0.f;
//
//void Draw(HDC _hDC)
//{
//	Ellipse(_hDC, (int)x, (int)(y + JumpHeight), (int)(x + diameter), (int)(y + JumpHeight + diameter));
//	TCHAR test[256] = { 0 };
//	_stprintf_s(test, L"Up Power : %.1f  y좌표 : %.1f", Velocity, (double)y + (double)JumpHeight);
//	TextOut(_hDC, 300, 350, test, lstrlen(test));
//}
//
//void Jump(void)
//{
//	if (!bJumpKeyPressed)		return;
//
//	//	착지하면 변수들 모두 초기화
//	if (Velocity <= -300.f)
//	{
//		Velocity = 300.f;
//		bJumpKeyPressed = FALSE;
//		JumpHeight = 0;
//	}
//
//	JumpHeight -= Velocity * 0.0004f;
//	Velocity -= Gravity * 0.0004f;
//}
//
//void Action(void)
//{
//	if (GetAsyncKeyState(VK_RIGHT))
//		x += 10;
//	if (GetAsyncKeyState(VK_LEFT))
//		x -= 10;
//	if (GetAsyncKeyState(VK_DOWN))
//		y += 0.1f;
//	if (GetAsyncKeyState(VK_UP))
//		y -= 0.1f;
//	if (GetAsyncKeyState(VK_SPACE))
//	{
//		bJumpKeyPressed = TRUE;
//	}
//}
//
//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR strCmdLine, int nShowCmd) {
//	WNDCLASSEX wcex;
//	memset(&wcex, 0, sizeof(wcex));
//	wcex.cbSize = sizeof(wcex);
//	wcex.style = CS_HREDRAW | CS_VREDRAW;
//	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
//	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
//	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
//	wcex.hInstance = hInstance;
//	wcex.cbClsExtra = 0;
//	wcex.cbWndExtra = 0;
//	wcex.hIconSm = NULL;
//	wcex.lpszMenuName = NULL;
//	wcex.lpfnWndProc = WndProc;
//	wcex.lpszClassName = L"Jump";
//	RegisterClassEx(&wcex);
//	HWND hWnd = CreateWindow(L"Jump", L"Jump", WS_SYSMENU,
//		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
//	if (hWnd == NULL) {
//		MessageBox(NULL, L"윈도우 생성 실패", L"에러", MB_ICONERROR | MB_OK);
//		return -1;
//	}
//	ShowWindow(hWnd, SW_SHOW);
//
//	MSG msg;
//	memset(&msg, NULL, sizeof(msg));
//
//	while (msg.message != WM_QUIT)
//	{
//
//		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
//		{
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		}
//
//		InvalidateRect(hWnd, NULL, FALSE);
//	}
//	return 0;
//}
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	switch (uMsg)
//	{
//	case WM_CREATE:
//	{
//		RECT rc;
//		GetClientRect(hWnd, &rc);
//		g_ClientSize.cx = rc.right - rc.left;
//		g_ClientSize.cy = rc.bottom - rc.top;
//		HDC hdC = GetDC(hWnd);
//		g_hMemDC = CreateCompatibleDC(hdC);
//		g_hBackDC = CreateCompatibleDC(hdC);
//		g_hBackBMP = CreateCompatibleBitmap(hdC, g_ClientSize.cx, g_ClientSize.cy);
//		SelectObject(g_hMemDC, g_hBackBMP);
//		break;
//	}
//	case WM_PAINT:
//	{
//		PAINTSTRUCT ps;
//		HDC hDC = BeginPaint(hWnd, &ps);
//		BitBlt(hDC, 0, 0, g_ClientSize.cx, g_ClientSize.cy, g_hMemDC, 0, 0, SRCCOPY);
//		Draw(g_hMemDC);
//		Action();
//		Jump();
//		EndPaint(hWnd, &ps);
//		break;
//	}
//	case WM_DESTROY:
//	{
//		DeleteDC(g_hBackDC);
//		DeleteObject(g_hBackBMP);
//		DeleteDC(g_hMemDC);
//		PostQuitMessage(0);
//		break;
//	}
//	}
//
//	return DefWindowProc(hWnd, uMsg, wParam, lParam);
//}