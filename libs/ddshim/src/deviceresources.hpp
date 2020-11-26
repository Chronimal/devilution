#pragma once

#include "ddsinternal.hpp"

DDS_BEGIN_NS

class DeviceResources
{
public:
    DeviceResources();
    DeviceResources(const DeviceResources&) = delete;
    DeviceResources& operator=(const DeviceResources&) = delete;

    void setHWND(HWND hwnd) noexcept;
    HWND getHWND() const noexcept;

    ID3D11Device1* getDevice() const noexcept;
    IDXGISwapChain1* getSwapChain() const noexcept;
    ID3D11DeviceContext1* getDeviceContext() const noexcept;
    ID3D11RenderTargetView* getRenderTargetView() const noexcept;

    const DirectX::XMFLOAT2& getViewportSize() const noexcept;

    void createDeviceResources();
    void createWindowSizeDependentResources();

private:
    HWND hwnd_{};
    DirectX::XMFLOAT2 viewportSize_{};

    Microsoft::WRL::ComPtr<ID3D11Device1> d3dDevice_;
    Microsoft::WRL::ComPtr<IDXGISwapChain1> d3dSwapChain_;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1> d3dDeviceContext_;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> d3dRenderTargetView_;
};

DDS_END_NS
