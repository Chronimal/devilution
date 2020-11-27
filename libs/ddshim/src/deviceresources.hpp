#pragma once

#include "ddsinternal.hpp"

DDS_BEGIN_NS

class DeviceResources
{
public:
    DeviceResources();
    DeviceResources(const DeviceResources&) = delete;
    DeviceResources& operator=(const DeviceResources&) = delete;

    HWND getHWND() const noexcept;
    void setHWND(HWND hwnd) noexcept;
    void windowSizeChanged(const SIZE& size);
    void windowSizeChanged(int width, int height);

    ID3D11Device1* getDevice() const noexcept;
    IDXGISwapChain1* getSwapChain() const noexcept;
    ID3D11DeviceContext1* getDeviceContext() const noexcept;
    ID3D11RenderTargetView* getRenderTargetView() const noexcept;

    const DirectX::XMFLOAT2 getViewportSize() const noexcept;

    void present();

private:
    HWND hwnd_{};
    SIZE windowSize_{};

    Microsoft::WRL::ComPtr<ID3D11Device1> d3dDevice_;
    Microsoft::WRL::ComPtr<IDXGISwapChain1> d3dSwapChain_;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1> d3dDeviceContext_;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> d3dRenderTargetView_;

    static constexpr DXGI_FORMAT dxgiFormat_ = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;

    void deviceLost();
    void createDeviceResources();
    void createWindowSizeDependentResources(const SIZE& windowSize);
};

DDS_END_NS
