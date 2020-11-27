#pragma once

#ifndef DDS_DDSINTERNAL_HPP_INCLUDED
#define DDS_DDSINTERNAL_HPP_INCLUDED

#include "../include/ddshim/ddshim.hpp"
#include <wrl/client.h>

#include <d3d11.h>
#include <d3d11_1.h>
#include <dxgidebug.h>
#include <directxmath.h>

#include <commctrl.h>
#include <comdef.h>

#include <memory>
#include <stdexcept>

#include "deviceresources.hpp"

// Include libraries used in the platform specific part of the code base
#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#define DDS_BEGIN_ANON_NS namespace {
#define DDS_END_ANON_NS }

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

SIZE getClientSize(HWND hwnd);

HRESULT throwIfFailed(HRESULT hr, const char* file, int line) noexcept(false);
[[noreturn]] void throwUnconditional(HRESULT hr, const char* file, int line);
HRESULT toHRESULT(std::exception_ptr ep) noexcept;

DDS_END_NS

#define DDS_THROW_IF_FAILED(hr) throwIfFailed(hr, __FILE__, __LINE__);
#define DDS_THROW(hr) throwUnconditional(hr, __FILE__, __LINE__);

#endif // DDS_DDSINTERNAL_HPP_INCLUDED
