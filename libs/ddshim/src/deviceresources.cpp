
#include "deviceresources.hpp"
#include <algorithm>

using Microsoft::WRL::ComPtr;
using namespace DirectX;

DDS_BEGIN_NS

void DeviceResources::DeviceEventDelegate::remove(void* instance)
{
    std::erase_if(deviceEventSinks_, [&](auto& p) { return p->isInstance(instance); });
}

void DeviceResources::DeviceEventDelegate::deviceLost()
{
    for (auto& p : deviceEventSinks_)
    {
        p->deviceLost();
    }
}

void DeviceResources::DeviceEventDelegate::deviceRestored()
{
    for (auto& p : deviceEventSinks_)
    {
        p->deviceRestored();
    }
}

DeviceResources::DeviceResources()
{
    createDeviceResources();
}

void DeviceResources::setHWND(HWND hwnd) noexcept
{
    _ASSERT(hwnd_ == nullptr && IsWindow(hwnd));
    if (hwnd != hwnd_)
    {
        hwnd_ = hwnd;
        createWindowSizeDependentResources(getClientSize(hwnd_));
    }
}

HWND DeviceResources::getHWND() const noexcept
{
    return hwnd_;
}

void DeviceResources::windowSizeChanged(int width, int height)
{
    createWindowSizeDependentResources({width, height});
}

void DeviceResources::windowSizeChanged(const SIZE& size)
{
    createWindowSizeDependentResources(size);
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

const XMFLOAT2 DeviceResources::getViewportSize() const noexcept
{
    return {static_cast<FLOAT>(windowSize_.cx), static_cast<FLOAT>(windowSize_.cy)};
}

void DeviceResources::createDeviceResources()
{
    // clang-format off
    ComPtr<ID3D11Device> d3dBaseDevice;
    ComPtr<ID3D11DeviceContext> d3dBaseDeviceContext;
    UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
    static constexpr D3D_FEATURE_LEVEL featureLevels[]
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
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

void DeviceResources::createWindowSizeDependentResources(const SIZE& windowSize)
{
    windowSize_ = windowSize;

    d3dDeviceContext_->OMSetRenderTargets(0, 0, 0);
    d3dRenderTargetView_.Reset();
    d3dDeviceContext_->Flush();

    if (d3dSwapChain_) // If the swap chain already exists, resize it
    {
        // clang-format off
        auto hr = d3dSwapChain_->ResizeBuffers(2,
                                               windowSize_.cx,
                                               windowSize_.cy,
                                               dxgiFormat_,
                                               0);
        // clang-format on
        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
            // If the device was removed for any reason, a new device and swap chain will need to be created.
            // Everything is set up now. Do not continue execution of this method. deviceLost() will reenter this method
            // and correctly set up the new device.
            return deviceLost();
        }
    }
    else // otherwise create it
    {
        ComPtr<IDXGIDevice1> dxgiDevice;
        DDS_THROW_IF_FAILED(d3dDevice_.As(&dxgiDevice));

        ComPtr<IDXGIAdapter> dxgiAdapter;
        DDS_THROW_IF_FAILED(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

        DXGI_ADAPTER_DESC adapterDesc;
        dxgiAdapter->GetDesc(&adapterDesc);

        ComPtr<IDXGIFactory2> dxgiFactory;
        DDS_THROW_IF_FAILED(dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)dxgiFactory.GetAddressOf()));

        DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
        swapChainDesc.Width = windowSize_.cx;
        swapChainDesc.Height = windowSize_.cy;
        swapChainDesc.Format = dxgiFormat_;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 2;
        swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
        swapChainDesc.Flags = 0;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapChainFullscreenDesc = {};
        swapChainFullscreenDesc.Windowed = TRUE;

        // clang-format off
(dxgiFactory->CreateSwapChainForHwnd(d3dDevice_.Get(),
                                                                hwnd_,
                                                                &swapChainDesc,
                                                                &swapChainFullscreenDesc,
                                                                nullptr,
                                                                d3dSwapChain_.ReleaseAndGetAddressOf()));
        // clang-format on
    }

    // Create frame buffer render target
    ComPtr<ID3D11Texture2D> d3dFrameBuffer;
    DDS_THROW_IF_FAILED(d3dSwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(d3dFrameBuffer.GetAddressOf())));
    DDS_THROW_IF_FAILED(d3dDevice_->CreateRenderTargetView(d3dFrameBuffer.Get(), 0, d3dRenderTargetView_.ReleaseAndGetAddressOf()));
}

void DeviceResources::present()
{
    auto hr = d3dSwapChain_->Present(1, 0);
    // Discard the contents of the render target.
    // This is a valid operation only when the existing contents will be entirely
    // overwritten. If dirty or scroll rects are used, this call should be removed.
    d3dDeviceContext_->DiscardView(d3dRenderTargetView_.Get());
    // If the device was removed either by a disconnection or a driver upgrade, we
    // must recreate all device resources.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        return deviceLost();
    }
    DDS_THROW_IF_FAILED(hr);
}

void DeviceResources::deviceLost()
{
    deviceEventDelegate_.deviceLost();

    d3dRenderTargetView_.Reset();
    d3dDeviceContext_.Reset();
    d3dSwapChain_.Reset();
    d3dDevice_.Reset();

    createDeviceResources();
    createWindowSizeDependentResources(getClientSize(hwnd_));

    deviceEventDelegate_.deviceRestored();
}

DDS_END_NS
