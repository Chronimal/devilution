
#include "deviceresources.hpp"

using Microsoft::WRL::ComPtr;
using namespace DirectX;

DDS_BEGIN_NS

DeviceResources::DeviceResources()
{
    createDeviceResources();
}

void DeviceResources::setHWND(HWND hwnd) noexcept
{
    _ASSERT(hwnd_ == nullptr && IsWindow(hwnd));
    hwnd_ = hwnd;
}

HWND DeviceResources::getHWND() const noexcept
{
    return hwnd_;
}

ID3D11Device1* DeviceResources::getDevice() const noexcept
{
    return d3dDevice_.Get();
}

IDXGISwapChain1* DeviceResources::getSwapChain() const noexcept
{
    return d3dSwapChain_.Get();
}

ID3D11DeviceContext1* DeviceResources::getDeviceContext() const noexcept
{
    return d3dDeviceContext_.Get();
}

ID3D11RenderTargetView* DeviceResources::getRenderTargetView() const noexcept
{
    return d3dRenderTargetView_.Get();
}

const XMFLOAT2& DeviceResources::getViewportSize() const noexcept
{
    return viewportSize_;
}

void DeviceResources::createDeviceResources()
{
    // clang-format off
    ComPtr<ID3D11Device> d3dBaseDevice;
    ComPtr<ID3D11DeviceContext> d3dBaseDeviceContext;
    UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
    static constexpr D3D_FEATURE_LEVEL featureLevels[]
    {
        D3D_FEATURE_LEVEL_11_0
    };

#if defined(DEBUG_BUILD)
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    DDS_THROW_IF_FAILED(D3D11CreateDevice(0,
                                          D3D_DRIVER_TYPE_HARDWARE,
                                          0,
                                          creationFlags,
                                          featureLevels,
                                          ARRAYSIZE(featureLevels),
                                          D3D11_SDK_VERSION,
                                          d3dBaseDevice.GetAddressOf(),
                                          0,
                                          d3dBaseDeviceContext.GetAddressOf()));
    // clang-format on

#ifdef _DEBUG
    ComPtr<ID3D11Debug> d3dDebug;
    if (SUCCEEDED(d3dBaseDevice.As(&d3dDebug)))
    {
        ComPtr<ID3D11InfoQueue> d3dInfoQueue;
        if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
        {
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
        }
    }
#endif

    // .As() does ReleaseAndGetAddressOf() internally
    DDS_THROW_IF_FAILED(d3dBaseDevice.As(&d3dDevice_));
    DDS_THROW_IF_FAILED(d3dBaseDeviceContext.As(&d3dDeviceContext_));
}

void DeviceResources::createWindowSizeDependentResources()
{
    // Get view size
    RECT clientRect{};
    if (!GetClientRect(hwnd_, &clientRect))
    {
        DDS_THROW_IF_FAILED(HRESULT_FROM_WIN32(GetLastError()));
    }
    viewportSize_.x = static_cast<FLOAT>(clientRect.right - clientRect.left);
    viewportSize_.y = static_cast<FLOAT>(clientRect.bottom - clientRect.top);

    d3dDeviceContext_->OMSetRenderTargets(0, 0, 0);

    // Create Swap Chain
    ComPtr<IDXGIDevice1> dxgiDevice;
    DDS_THROW_IF_FAILED(d3dDevice_.As(&dxgiDevice));

    ComPtr<IDXGIAdapter> dxgiAdapter;
    DDS_THROW_IF_FAILED(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

    DXGI_ADAPTER_DESC adapterDesc;
    dxgiAdapter->GetDesc(&adapterDesc);

    ComPtr<IDXGIFactory2> dxgiFactory;
    DDS_THROW_IF_FAILED(dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)dxgiFactory.GetAddressOf()));

    DXGI_SWAP_CHAIN_DESC1 d3dSwapChainDesc{};
    d3dSwapChainDesc.Width = static_cast<UINT>(viewportSize_.x);
    d3dSwapChainDesc.Height = static_cast<UINT>(viewportSize_.y);
    d3dSwapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
    d3dSwapChainDesc.SampleDesc.Count = 1;
    d3dSwapChainDesc.SampleDesc.Quality = 0;
    d3dSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    d3dSwapChainDesc.BufferCount = 2;
    d3dSwapChainDesc.Scaling = DXGI_SCALING_STRETCH;
    d3dSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    d3dSwapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    d3dSwapChainDesc.Flags = 0;

    DDS_THROW_IF_FAILED(dxgiFactory->CreateSwapChainForHwnd(d3dDevice_.Get(), hwnd_, &d3dSwapChainDesc, 0, 0, d3dSwapChain_.ReleaseAndGetAddressOf()));

    // Create frame buffer render target
    ComPtr<ID3D11Texture2D> d3dFrameBuffer;
    DDS_THROW_IF_FAILED(d3dSwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(d3dFrameBuffer.GetAddressOf())));
    DDS_THROW_IF_FAILED(d3dDevice_->CreateRenderTargetView(d3dFrameBuffer.Get(), 0, d3dRenderTargetView_.ReleaseAndGetAddressOf()));
}

DDS_END_NS
