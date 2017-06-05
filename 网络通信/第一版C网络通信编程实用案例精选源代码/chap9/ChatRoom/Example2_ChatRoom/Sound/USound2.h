//---------------------------------------------------------------------------
#ifndef USound2H
#define USound2H
//---------------------------------------------------------------------------
#endif
extern "C" __declspec(dllexport) long __stdcall setIpAddr(char* ip);
extern "C" __declspec(dllexport) void __stdcall setPort(int port);
extern "C" __declspec(dllexport) void __stdcall SoundStart();
extern "C" __declspec(dllexport) void __stdcall SoundStop();

