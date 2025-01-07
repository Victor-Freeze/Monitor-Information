#include <iostream>
#include <iomanip>
#include <windows.h>
#include <dxgi.h>
#pragma comment(lib, "dxgi.lib")

void getMonitorInfoDEVMODE() {
    // Get the handle to the primary monitor
    HMONITOR hMonitor = MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY);

    // Retrieve the display device information
    MONITORINFOEX mi;
    mi.cbSize = sizeof(MONITORINFOEX);
    GetMonitorInfo(hMonitor, &mi);

    // Get the device name
    DEVMODE dm;
    dm.dmSize = sizeof(DEVMODE);
    dm.dmDriverExtra = 0;
    EnumDisplaySettings(mi.szDevice, ENUM_CURRENT_SETTINGS, &dm);

    // Display the resolution and refresh rate
    std::cout << "DEVMODE - Current Monitor Resolution: " << dm.dmPelsWidth << " x " << dm.dmPelsHeight << std::endl;
    std::cout << "DEVMODE - Current Refresh Rate: " << dm.dmDisplayFrequency << " Hz" << std::endl;
}

void getMonitorInfoDXGI() {
    IDXGIFactory* pFactory = nullptr;
    if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory))) {
        std::cerr << "Failed to create DXGI Factory" << std::endl;
        return;
    }

    IDXGIAdapter* pAdapter = nullptr;
    if (FAILED(pFactory->EnumAdapters(0, &pAdapter))) {
        std::cerr << "Failed to enumerate adapters" << std::endl;
        pFactory->Release();
        return;
    }

    IDXGIOutput* pOutput = nullptr;
    if (FAILED(pAdapter->EnumOutputs(0, &pOutput))) {
        std::cerr << "Failed to enumerate outputs" << std::endl;
        pAdapter->Release();
        pFactory->Release();
        return;
    }

    DXGI_OUTPUT_DESC desc;
    pOutput->GetDesc(&desc);

    UINT numModes = 0;
    pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numModes, NULL);
    DXGI_MODE_DESC* pModeDescs = new DXGI_MODE_DESC[numModes];
    pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numModes, pModeDescs);

    for (UINT i = 0; i < numModes; ++i) {
        if (pModeDescs[i].Width == desc.DesktopCoordinates.right - desc.DesktopCoordinates.left &&
            pModeDescs[i].Height == desc.DesktopCoordinates.bottom - desc.DesktopCoordinates.top) {
            std::cout << "DXGI - Current Monitor Resolution: " << pModeDescs[i].Width << " x " << pModeDescs[i].Height << std::endl;
            std::cout << "DXGI - Current Refresh Rate: " << std::fixed << std::setprecision(3)
                << static_cast<double>(pModeDescs[i].RefreshRate.Numerator) / pModeDescs[i].RefreshRate.Denominator << " Hz" << std::endl;
            break;
        }
    }

    delete[] pModeDescs;
    pOutput->Release();
    pAdapter->Release();
    pFactory->Release();
}

int main() {
    std::cout << "Using DEVMODE:" << std::endl;
    getMonitorInfoDEVMODE();
    std::cout << std::endl;

    std::cout << "Using DXGI:" << std::endl;
    getMonitorInfoDXGI();

    std::cout << "Press any key to exit..." << std::endl;
    std::cin.get();
    return 0;
}
