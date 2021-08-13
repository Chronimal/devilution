#pragma once

#ifndef DDS_DDSINTERNAL_HPP_INCLUDED
#define DDS_DDSINTERNAL_HPP_INCLUDED

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN

#ifndef NOMINMAX
#define NOMINMAX
#endif // NOMINMAX

#ifndef STRICT
#define STRICT
#endif // STRICT

#include <windows.h>
#include <ddraw.h>

#include <wrl/wrappers/corewrappers.h>
#include <wrl/client.h>

#include <d3d11.h>
#include <d3d11_1.h>
#include <dxgidebug.h>
#include <directxmath.h>

#include <winternl.h>
#include <commctrl.h>
#include <comdef.h>

#include <memory>
#include <stdexcept>

// Include libraries used in the platform specific part of the code base
#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "Version.lib")
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

// clang-format off
#define DDS_BEGIN_NS namespace ddw {
#define DDS_END_NS }

#define DDS_BEGIN_ANON_NS namespace {
#define DDS_END_ANON_NS }
// clang-format on

#include "deviceresources.hpp"

DDS_BEGIN_NS

class DirectDraw;
class DirectDrawPalette;
class DirectDrawSurface;

// Helper class for COM exceptions
class ComRuntimeError : public std::runtime_error
{
public:
    ComRuntimeError(HRESULT hr, const char* file, int line);
    HRESULT getResult() const noexcept;

private:
    HRESULT hr_;
    static const char* formatError(HRESULT hr) noexcept;
};

struct VirtualDisplayMode
{
    VirtualDisplayMode(UINT width, UINT height, UINT bpp) noexcept;
    VirtualDisplayMode() = default;

    UINT width{};
    UINT height{};
    UINT bpp{};
};

SIZE getClientSize(HWND hwnd);

HRESULT throwIfFailed(HRESULT hr, const char* file, int line) noexcept(false);
[[noreturn]] void throwUnconditional(HRESULT hr, const char* file, int line);
HRESULT toHRESULT(std::exception_ptr ep) noexcept;

DDS_END_NS

#define DDS_THROW_IF_FAILED(hr) throwIfFailed(hr, __FILE__, __LINE__);
#define DDS_THROW(hr) throwUnconditional(hr, __FILE__, __LINE__);

#endif // DDS_DDSINTERNAL_HPP_INCLUDED
