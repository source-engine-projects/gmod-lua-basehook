#include "pch.h"

ILuaShared* LuaShared = NULL;
ILuaInterface* whocheats = NULL;

HMODULE module = NULL;


void addHook(std::string name, std::string identifier, CLuaFunction function)
{
	whocheats->PushSpecial(SPECIAL_GLOB);
	whocheats->GetField(-1, "hook");
	whocheats->GetField(-1, "Add");
	whocheats->PushString(name.c_str(), name.length());
	whocheats->PushString(identifier.c_str(), identifier.length());
	whocheats->PushCFunction(function);
	whocheats->Call(3, 0);
	whocheats->Pop();
	whocheats->Pop();
}

void removeHook(std::string name, std::string identifier)
{
	whocheats->PushSpecial(SPECIAL_GLOB);
	whocheats->GetField(-1, "hook");
	whocheats->GetField(-1, "Remove");
	whocheats->PushString(name.c_str(), name.length());
	whocheats->PushString(identifier.c_str(), identifier.length());
	whocheats->Call(2, 0);
	whocheats->Pop();
	whocheats->Pop();
}

int Main(lua_State* state)
{
	removeHook("Think", "whocheats.initializing");

	char s_luashared0[] = "[whocheats] LuaShared: 0x%X";
	char s_luashared1[2000];
	sprintf_s(s_luashared1, s_luashared0, LuaShared);
	MessageBox(NULL, s_luashared1, NULL, MB_OK);

	char s_luastate0[] = "[whocheats] LuaState : 0x%X";
	char s_luastate1[2000];
	sprintf_s(s_luastate1, s_luastate0, whocheats);
	MessageBox(NULL, s_luastate1, NULL, MB_OK);

	whocheats->RunString("RunString", "", "print('fuckyou')", true, true);


	// Place code above ^
	return 0;
}

int DllInit()
{
	HMODULE LuaShared_modhandle = GetModuleHandle("lua_shared.dll");
	if (LuaShared_modhandle != NULL)
	{
		typedef void* (*CreateInterfaceFn)(const char* name, int* returncode);
		CreateInterfaceFn LuaShared_createinter = (CreateInterfaceFn)GetProcAddress(LuaShared_modhandle, "CreateInterface");
		if (LuaShared_createinter != NULL)
		{
			LuaShared = (ILuaShared*)LuaShared_createinter(LUASHARED_INTERFACE_VERSION, NULL);
		}
	}
	if (LuaShared == NULL)
	{
		MessageBox(NULL, "[whocheats] LuaShared not initialized", NULL, MB_OK);
		return 1;
	}
	whocheats = LuaShared->GetLuaInterface(LuaInterfaceType::LUAINTERFACE_CLIENT);
	if (whocheats == NULL) {
		MessageBox(NULL, "[whocheats] Lua State not initialized", NULL, MB_OK);
		return 1;
	}
	addHook("Think", "whocheats.initializing", Main);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		module = hModule;
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)DllInit, NULL, NULL, NULL);
	}
	else if (reason == DLL_PROCESS_DETACH)
	{
		whocheats = NULL;
		LuaShared = NULL;
		module = NULL;
	}
	return TRUE;
}