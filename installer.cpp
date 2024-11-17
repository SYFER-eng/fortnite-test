#include <cstdlib>
#include <string>
#include <iostream>

void install_dependencies() {
    // Install Visual Studio Build Tools
    system("curl -O https://aka.ms/vs/17/release/vs_buildtools.exe");
    system("vs_buildtools.exe --quiet --wait --norestart --nocache "
        "--installPath C:\\BuildTools "
        "--add Microsoft.VisualStudio.Workload.VCTools "
        "--add Microsoft.VisualStudio.Component.Windows10SDK");

    // Install DirectX SDK
    system("curl -O https://download.microsoft.com/download/A/E/7/AE743F1F-632B-4809-87A9-AA1BB3458E31/DXSDK_Jun10.exe");
    system("DXSDK_Jun10.exe /U");

    // Install CMake
    system("curl -O https://github.com/Kitware/CMake/releases/download/v3.24.0/cmake-3.24.0-windows-x86_64.msi");
    system("msiexec /i cmake-3.24.0-windows-x86_64.msi /quiet /norestart");
}
