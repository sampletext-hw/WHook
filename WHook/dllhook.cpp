#include "dllhook.h"

#include "atltypes.h"

HHOOK hMouseHook;
HHOOK hKeyboardHook;
HINSTANCE hInst;

EXPORT void CALLBACK SetMouseHook(void)
{
	hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, hInst, 0);
}

EXPORT void CALLBACK UnMouseHook(void)
{
	UnhookWindowsHookEx(hMouseHook);
}

EXPORT void CALLBACK SetKeyBoardHook(void)
{
	hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInst, 0);
}

EXPORT void CALLBACK UnKeyBoardHook(void)
{
	UnhookWindowsHookEx(hKeyboardHook);
}

bool isProcessing = false;

int speedX = 5;
int speedY = 5;

int lastPosX = 0;
int lastPosY = 0;

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code < 0)
	{
		CallNextHookEx(NULL, code, wParam, lParam);
		return 0;
	}
	if (wParam == WM_KEYDOWN)
	{
		PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)(lParam);
		// LPPOINT pt = new POINT;
		// GetCursorPos(pt);
		// int x = pt->x;
		// int y = pt->y;

		//switch (wParam) {
		switch (p->vkCode)
		{
		case VK_LEFT:
			speedX = speedX >= 0 ? -speedX : speedX;
			break;
		case VK_RIGHT:
			speedX = speedX >= 0 ? speedX : -speedX;
			break;
		case VK_UP:
			speedY = speedY >= 0 ? -speedY : speedY;
			break;
		case VK_DOWN:
			speedY = speedY >= 0 ? speedY : -speedY;
			break;
		case VK_SPACE:
			if (isProcessing)
			{
				isProcessing = false;
			}
			else
			{
				// GET SCREEN WIDTH AND HEIGHT
				RECT desktop;
				const HWND hDesktop = GetDesktopWindow();
				GetWindowRect(hDesktop, &desktop);

				int screenWidth = desktop.right;
				int screenHeight = desktop.bottom;
				isProcessing = true;
				while (isProcessing)
				{
					LPPOINT pt = new POINT;
					GetCursorPos(pt);
					int posX = pt->x;
					int posY = pt->y;

					if (posX + speedX >= screenWidth || posX + speedX <= 0)
					{
						speedX *= -1;
					}
					if (posY + speedY >= screenHeight || posY + speedY <= 0)
					{
						speedY *= -1;
					}

					posX += speedX;
					posY += speedY;

					SetCursorPos(posX, posY);
					lastPosX = posX;
					lastPosY = posY;
					Sleep(16);
					CallNextHookEx(NULL, code, wParam, lParam);
				}
			}
			break;
		}

		return CallNextHookEx(NULL, code, wParam, lParam);
	}
	return CallNextHookEx(NULL, code, wParam, lParam);
}

LRESULT CALLBACK MouseProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code < 0)
	{
		return CallNextHookEx(NULL, code, wParam, lParam);
	}

	if (wParam == WM_MOUSEMOVE)
	{
		MOUSEHOOKSTRUCT* p = (MOUSEHOOKSTRUCT*)lParam;
		int x = p->pt.x;
		int y = p->pt.y;

		int dx = x - lastPosX;
		int dy = y - lastPosY;

		if (dx > 0 && speedX < 0 || dx < 0 && speedX > 0)
		{
			speedX = -speedX;
		}

		if (dy > 0 && speedY < 0 || dy < 0 && speedY > 0)
		{
			speedY = -speedY;
		}

		return CallNextHookEx(NULL, code, wParam, lParam);
	}
	else
	{
		return CallNextHookEx(NULL, code, wParam, lParam);
	}
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	hInst = hinstDLL;
	return TRUE;
}
