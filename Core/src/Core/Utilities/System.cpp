#include "System.h"

#include <Windows.h>
#include <GLFW/glfw3.h>

double Core::System::GetTime() {
	return glfwGetTime();
}

void Core::System::SetCursor(Core::Cursor cursor) {
	HCURSOR hCursor = LoadCursor(nullptr, IDC_ARROW); 
	switch (cursor) {
		case Cursor::Normal: hCursor = LoadCursor(nullptr, IDC_ARROW); break;
		case Cursor::Help: hCursor = LoadCursor(nullptr, IDC_HELP); break;
		case Cursor::Working: hCursor = LoadCursor(nullptr, IDC_APPSTARTING); break;
		case Cursor::Busy: hCursor = LoadCursor(nullptr, IDC_WAIT); break;
		case Cursor::Precision: hCursor = LoadCursor(nullptr, IDC_CROSS); break;
		case Cursor::Text: hCursor = LoadCursor(nullptr, IDC_IBEAM); break;
		case Cursor::Unavailable: hCursor = LoadCursor(nullptr, IDC_NO); break;
		case Cursor::VResize: hCursor = LoadCursor(nullptr, IDC_SIZENS); break;
		case Cursor::HResize: hCursor = LoadCursor(nullptr, IDC_SIZEWE); break;
		case Cursor::DResize1: hCursor = LoadCursor(nullptr, IDC_SIZENWSE); break;
		case Cursor::DResize2: hCursor = LoadCursor(nullptr, IDC_SIZENESW); break;
		case Cursor::Move: hCursor = LoadCursor(nullptr, IDC_SIZEALL); break;
		case Cursor::AltSelect: hCursor = LoadCursor(nullptr, IDC_UPARROW); break;
		case Cursor::LinkSelect: hCursor = LoadCursor(nullptr, IDC_HAND); break;
	}
	SetCursor(hCursor);
}
