#pragma once

extern Core::App* Core::CreateApp();

#ifdef _DEBUG
	
int main() {
	auto app = Core::CreateApp();
	app->Init();
	app->Run();
	app->Shutdown();
	delete app;
}

#else
#include <Windows.h>

int WINAPI WinMain(_In_ HINSTANCE hInstace, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	auto app = Core::CreateApp();
	app->Init();
	app->Run();
	app->Shutdown();
	delete app;
	return 0;
}

#endif
