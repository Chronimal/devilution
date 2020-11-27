
#include "directdrawsurface.hpp"
#include "directdrawpalette.hpp"
#include "directdraw.hpp"

using Microsoft::WRL::ComPtr;
using namespace DirectX;

DDS_BEGIN_NS

struct Viewport : public D3D11_VIEWPORT
{
    Viewport(const XMFLOAT2& size) noexcept
    {
        TopLeftX = 0.0f;
        TopLeftY = 0.0f;
        Width = size.x;
        Height = size.y;
        MinDepth = 0.0f;
        MaxDepth = 1.0f;
    }
};

DDS_BEGIN_ANON_NS

#include "pixelshader.h"
#include "vertexshader.h"

DDS_END_ANON_NS

DirectDraw::DirectDraw(GUID* /* guid */, IUnknown*)
    : deviceResources_{std::make_shared<DeviceResources>()}
{
}

DirectDraw::~DirectDraw()
{
    unsubclassWindow(deviceResources_->getHWND());
}

std::shared_ptr<DeviceResources> DirectDraw::getDeviceResources() const noexcept
{
    return deviceResources_;
}

void DirectDraw::render(ID3D11ShaderResourceView* canvasView, ID3D11ShaderResourceView* paletteView)
{
    static constexpr FLOAT backgroundColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    auto frameBufferView = deviceResources_->getRenderTargetView();
    auto deviceContext = deviceResources_->getDeviceContext();
    auto deviceResources = deviceResources_.get();

    deviceContext->ClearRenderTargetView(frameBufferView, backgroundColor);

    Viewport viewport(deviceResources->getViewportSize());
    deviceContext->RSSetViewports(1, &viewport);

    auto renderTargetView = deviceResources->getRenderTargetView();
    deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);

    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    deviceContext->IASetInputLayout(inputLayout_.Get());

    deviceContext->VSSetShader(vertexShader_.Get(), nullptr, 0);
    deviceContext->PSSetShader(pixelShader_.Get(), nullptr, 0);

    ID3D11ShaderResourceView* psResources[]{ canvasView, paletteView };
    deviceContext->PSSetShaderResources(0, 2, psResources);

    auto samplerState = samplerState_.Get();
    deviceContext->PSSetSamplers(0, 1, &samplerState);

    auto vertexBuffer = vertexBuffer_.Get();
    deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride_, &offset_);

    deviceContext->Draw(numVerts_, 0);
    deviceResources->present();
}

/*** IUnknown methods ***/
HRESULT DirectDraw::QueryInterface(REFIID riid, LPVOID* ppvObj)
{
    if (riid == IID_IDirectDraw || riid == IID_IDirectDraw2 || riid == IID_IDirectDraw4 || riid == IID_IDirectDraw7)
    {
        *ppvObj = this;
        AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}

ULONG DirectDraw::AddRef()
{
    return InterlockedIncrement(&refCount_);
}

ULONG DirectDraw::Release()
{
    auto refCount = InterlockedDecrement(&refCount_);
    if (refCount == 0)
    {
        std::destroy_at(this);
    }
    return refCount;
}

/*** IDirectDraw methods ***/
HRESULT DirectDraw::Compact()
{
    //return dd_->Compact();
    return E_NOTIMPL;
}

HRESULT DirectDraw::CreateClipper(DWORD dwFlags, LPDIRECTDRAWCLIPPER* lplpDDClipper, IUnknown* pUnkOuter)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDraw::CreatePalette(DWORD dwFlags, LPPALETTEENTRY lpColorTable, LPDIRECTDRAWPALETTE* lplpDDPalette, IUnknown* pUnkOuter)
{
    try
    {
        *lplpDDPalette = std::make_unique<DirectDrawPalette>(this, dwFlags, lpColorTable, pUnkOuter).release();
    }
    catch (...)
    {
        return toHRESULT(std::current_exception());
    }
    return DD_OK;
}

HRESULT DirectDraw::CreateSurface(LPDDSURFACEDESC lpDDSurfaceDesc, LPDIRECTDRAWSURFACE* lplpDDSurface, IUnknown* pUnkOuter)
{
    try
    {
        *lplpDDSurface = std::make_unique<DirectDrawSurface>(this, lpDDSurfaceDesc, pUnkOuter).release();
    }
    catch (...)
    {
        return toHRESULT(std::current_exception());
    }
    return DD_OK;
}

HRESULT DirectDraw::DuplicateSurface(LPDIRECTDRAWSURFACE lpDDSurface, LPDIRECTDRAWSURFACE* lplpDupDDSurface)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDraw::EnumDisplayModes(DWORD dwFlags, LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext, LPDDENUMMODESCALLBACK lpEnumCallback)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDraw::EnumSurfaces(DWORD dwFlags, LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext, LPDDENUMSURFACESCALLBACK lpEnumCallback)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDraw::FlipToGDISurface()
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDraw::GetCaps(LPDDCAPS lpDDDriverCaps, LPDDCAPS lpDDHELCaps)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDraw::GetDisplayMode(LPDDSURFACEDESC lpDDSurfaceDesc)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDraw::GetFourCCCodes(LPDWORD lpNumCodes, LPDWORD lpCodes)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDraw::GetGDISurface(LPDIRECTDRAWSURFACE* lplpGDIDDSSurface)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDraw::GetMonitorFrequency(LPDWORD lpdwFrequency)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDraw::GetScanLine(LPDWORD lpdwScanLine)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDraw::GetVerticalBlankStatus(LPBOOL lpbIsInVB)
{
    // TODO: This needs some work
    *lpbIsInVB = FALSE;
    return DD_OK;
}

HRESULT DirectDraw::Initialize(GUID* lpGUID)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDraw::RestoreDisplayMode()
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDraw::SetCooperativeLevel(HWND hWnd, DWORD dwFlags)
{
    try
    {
        subclassWindow(hWnd);
        deviceResources_->setHWND(hWnd);
        createDeviceDependentResources();
        createWindowSizeDependentResources();
    }
    catch (...)
    {
        unsubclassWindow(hWnd);
        return toHRESULT(std::current_exception());
    }
    return DD_OK;
}

HRESULT DirectDraw::SetDisplayMode(DWORD dwWidth, DWORD dwHeight, DWORD dwBpp)
{
    return DD_OK;
}

HRESULT DirectDraw::WaitForVerticalBlank(DWORD dwFlags, HANDLE hEvent)
{
    // TODO: This needs some work
    Sleep(1000 / 60);
    return S_OK;
}

// --- Help methods ---

void DirectDraw::createDeviceDependentResources()
{
    auto device = deviceResources_->getDevice();

    DDS_THROW_IF_FAILED(device->CreatePixelShader(g_pixelShader, sizeof(g_pixelShader), nullptr, pixelShader_.ReleaseAndGetAddressOf()));
    DDS_THROW_IF_FAILED(device->CreateVertexShader(g_vertexShader, sizeof(g_vertexShader), nullptr, vertexShader_.ReleaseAndGetAddressOf()));

    // Create Input Layout
    // clang-format off
    static constexpr D3D11_INPUT_ELEMENT_DESC inputElementDesc[]
    {
        {"POS", 0, DXGI_FORMAT_R32G32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    DDS_THROW_IF_FAILED(device->CreateInputLayout(inputElementDesc,
                                                  ARRAYSIZE(inputElementDesc),
                                                  g_vertexShader,
                                                  sizeof(g_vertexShader),
                                                  inputLayout_.ReleaseAndGetAddressOf()));

    static constexpr FLOAT vertexData[]
    {  // x,     y,    u,   v
         -1.f,  1.f, 0.f, 0.f,
          1.f, -1.f, 1.f, 1.f,
         -1.f, -1.f, 0.f, 1.f,
         -1.f,  1.f, 0.f, 0.f,
          1.f,  1.f, 1.f, 0.f,
          1.f, -1.f, 1.f, 1.f
    };
    // clang-format on

    offset_ = 0;
    stride_ = 4 * sizeof(FLOAT);
    numVerts_ = sizeof(vertexData) / stride_;

    D3D11_BUFFER_DESC vertexBufferDesc{};
    vertexBufferDesc.ByteWidth = sizeof(vertexData);
    vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA vertexSubresourceData{vertexData};
    DDS_THROW_IF_FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, vertexBuffer_.ReleaseAndGetAddressOf()));

    // Create Sampler State
    D3D11_SAMPLER_DESC samplerDesc{};
    samplerDesc.BorderColor[0] = 1.0f;
    samplerDesc.BorderColor[1] = 1.0f;
    samplerDesc.BorderColor[2] = 1.0f;
    samplerDesc.BorderColor[3] = 1.0f;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    DDS_THROW_IF_FAILED(device->CreateSamplerState(&samplerDesc, samplerState_.ReleaseAndGetAddressOf()));
}

void DirectDraw::createWindowSizeDependentResources()
{
}

void DirectDraw::onDeviceLost()
{
    inputLayout_.Reset();
    vertexBuffer_.Reset();
    vertexShader_.Reset();
    pixelShader_.Reset();
}

void DirectDraw::onDeviceRestored()
{
    createDeviceDependentResources();
    createWindowSizeDependentResources();
}

void DirectDraw::onWindowSizeChanged(int width, int height)
{
    createWindowSizeDependentResources();
}

void DirectDraw::onWindowMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_GETMINMAXINFO:
            return onGetMinMaxInfo(hwnd, wParam, lParam);
        case WM_SYSKEYDOWN:
            return onSysKeyDown(hwnd, wParam, lParam);
        case WM_SIZE:
            return onSize(hwnd, wParam, lParam);
        case WM_ENTERSIZEMOVE:
            return onEnterSizeMove(hwnd, wParam, lParam);
        case WM_EXITSIZEMOVE:
            return onExitSizeMove(hwnd, wParam, lParam);
    }
}

void DirectDraw::onGetMinMaxInfo(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    auto info = reinterpret_cast<MINMAXINFO*>(lParam);
    info->ptMinTrackSize.x = 320;
    info->ptMinTrackSize.y = 200;
}

void DirectDraw::onSysKeyDown(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
    {
        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreenDesc;
        auto swapChain = deviceResources_->getSwapChain();
        if (SUCCEEDED(swapChain->GetFullscreenDesc(&fullscreenDesc)))
        {
            swapChain->SetFullscreenState(fullscreenDesc.Windowed, nullptr);
        }
    }
}

void DirectDraw::onSize(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    if (!inSizeMove_)
    {
        deviceResources_->windowSizeChanged(LOWORD(lParam), HIWORD(lParam));
    }
}

void DirectDraw::onEnterSizeMove(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    inSizeMove_ = true;
}

void DirectDraw::onExitSizeMove(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    inSizeMove_ = false;
    deviceResources_->windowSizeChanged(getClientSize(hwnd));
}

LRESULT DirectDraw::subclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR subclassID, DWORD_PTR)
{
    auto self = reinterpret_cast<DirectDraw*>(subclassID);
    self->onWindowMessage(hwnd, msg, wParam, lParam);
    switch (msg)
    {
    case WM_MENUCHAR:
        return MAKELRESULT(0, MNC_CLOSE);
    case WM_DESTROY:
        self->unsubclassWindow(hwnd);
    }
    return ::DefSubclassProc(hwnd, msg, wParam, lParam);
}

void DirectDraw::subclassWindow(HWND hwnd)
{
    if (!isSubclassed_ && IsWindow(hwnd))
    {
        if (!SetWindowSubclass(hwnd, &DirectDraw::subclassProc, reinterpret_cast<UINT_PTR>(this), 0))
        {
            DDS_THROW(HRESULT_FROM_WIN32(GetLastError()));
        }
        isSubclassed_ = true;
    }
}

void DirectDraw::unsubclassWindow(HWND hwnd) noexcept
{
    if (isSubclassed_ && IsWindow(hwnd))
    {
        if (::RemoveWindowSubclass(hwnd, &DirectDraw::subclassProc, reinterpret_cast<UINT_PTR>(this)))
        {
            isSubclassed_ = false;
        }
    }
}

DDS_END_NS