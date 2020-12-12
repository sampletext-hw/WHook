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
	hMouseHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInst, 0);
}

EXPORT void CALLBACK UnKeyBoardHook(void)
{
	UnhookWindowsHookEx(hKeyboardHook);
}

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code < 0)
	{
		CallNextHookEx(hKeyboardHook, code, wParam, lParam);
		return 0;
	}
	if (wParam == WM_KEYDOWN)
	{
		PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)(lParam);
		LPPOINT pt = new POINT;
		GetCursorPos(pt);
		int x = pt->x;
		int y = pt->y;
		//switch (wParam) {
		switch (p->vkCode)
		{
		case VK_LEFT:
			/* ������������ ������� LEFT ARROW (������� �����). */
			SetCursorPos(x - 10, y);
			break;

		case VK_RIGHT:
			/* ������������ ������� RIGHT ARROW (������� ������). */
			SetCursorPos(x + 10, y);
			break;

		case VK_UP:
			/* ������������ ������� UP ARROW (������� �����). */
			SetCursorPos(x, y - 10);
			break;

		case VK_DOWN:
			/* ������������ ������� DOWN ARROW (������� ����). */

			SetCursorPos(x, y + 10);
			break;
		default:
			break;
		}

		return CallNextHookEx(NULL, code, wParam, lParam);
	}
}

LRESULT CALLBACK MouseProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code < 0)
	{
		CallNextHookEx(NULL, code, wParam, lParam);
		return 0;
	}
	if (wParam == WM_MOUSEMOVE)
	{
		PMSLLHOOKSTRUCT p = (PMSLLHOOKSTRUCT)lParam;
		// � ���� ���� ����� ����� ���������� ����� ��������� ������

		//CPoint pt; �������� ��  p->pt.x


		if (p->pt.y < 200)
		{
			SetCursorPos(p->pt.x, 200);
		}
		if (p->pt.y > 700)
		{
			SetCursorPos(p->pt.x, 700);
		}
		if (p->pt.x < 400)
		{
			SetCursorPos(400, p->pt.y);
		}
		if (p->pt.x > 1200)
		{
			SetCursorPos(1200, p->pt.y);
		}
		return 0;
	}
	return CallNextHookEx(NULL, code, wParam, lParam);
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	hInst = hinstDLL;
	return TRUE;
}
