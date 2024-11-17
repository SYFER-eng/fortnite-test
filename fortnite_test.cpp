#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <d3d9.h>
#include <d3dx9.h>
#include <cstdint>

// Fortnite-specific offsets
namespace offsets {
    constexpr auto uworld = 0x11223344;  // Example offset
    constexpr auto game_instance = 0x1A0;
    constexpr auto local_players = 0x38;
    constexpr auto player_controller = 0x30;
    constexpr auto player_camera = 0x340;
    constexpr auto root_component = 0x190;
    constexpr auto actor_array = 0xA0;
    constexpr auto actor_count = 0xB8;
}

struct FVector {
    float x, y, z;
};

struct FMatrix {
    float m[4][4];
};

class Memory {
private:
    HANDLE handle;
    uint64_t base_address;

public:
    bool attach() {
        DWORD pid = 0;
        HWND hwnd = FindWindowA("UnrealWindow", "Fortnite  ");
        GetWindowThreadProcessId(hwnd, &pid);
        handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
        base_address = get_base_address(pid);
        return handle != nullptr;
    }

    template<typename T>
    T read(uint64_t address) {
        T buffer;
        ReadProcessMemory(handle, (LPCVOID)address, &buffer, sizeof(T), nullptr);
        return buffer;
    }

private:
    uint64_t get_base_address(DWORD pid) {
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
        MODULEENTRY32W module_entry;
        module_entry.dwSize = sizeof(module_entry);

        if (Module32FirstW(snapshot, &module_entry)) {
            CloseHandle(snapshot);
            return (uint64_t)module_entry.modBaseAddr;
        }
        return 0;
    }
};

class Overlay {
private:
    LPDIRECT3D9 d3d;
    LPDIRECT3DDEVICE9 device;
    D3DPRESENT_PARAMETERS params;
    ID3DXFont* font;

public:
    bool init() {
        d3d = Direct3DCreate9(D3D_SDK_VERSION);
        ZeroMemory(&params, sizeof(params));

        params.Windowed = TRUE;
        params.SwapEffect = D3DSWAPEFFECT_DISCARD;
        params.hDeviceWindow = GetForegroundWindow();
        params.BackBufferFormat = D3DFMT_A8R8G8B8;
        params.BackBufferWidth = 1920;
        params.BackBufferHeight = 1080;
        params.EnableAutoDepthStencil = TRUE;
        params.AutoDepthStencilFormat = D3DFMT_D16;

        if (FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, params.hDeviceWindow,
            D3DCREATE_SOFTWARE_VERTEXPROCESSING, &params, &device)))
            return false;

        D3DXCreateFont(device, 14, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
            L"Arial", &font);

        return true;
    }

    void begin_scene() {
        device->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
        device->BeginScene();
    }

    void end_scene() {
        device->EndScene();
        device->Present(nullptr, nullptr, nullptr, nullptr);
    }

    void draw_text(const char* text, int x, int y, D3DCOLOR color) {
        RECT rect = { x, y, x + 120, y + 16 };
        font->DrawTextA(nullptr, text, -1, &rect, DT_NOCLIP, color);
    }

    void draw_line(float x1, float y1, float x2, float y2, D3DCOLOR color) {
        D3DXVECTOR2 lines[2] = { D3DXVECTOR2(x1, y1), D3DXVECTOR2(x2, y2) };
        ID3DXLine* line_drawer;
        D3DXCreateLine(device, &line_drawer);
        line_drawer->Draw(lines, 2, color);
        line_drawer->Release();
    }
};

