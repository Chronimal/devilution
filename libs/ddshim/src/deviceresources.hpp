#pragma once

#include "ddsinternal.hpp"
#include <vector>

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

    template<typename Instance, typename MfnLost, typename MfnRestored>
    void addDeviceEventSink(Instance instance, MfnLost lost, MfnRestored restored)
    {
        deviceEventDelegate_.add(instance, lost, restored);
    }

    template<typename Instance>
    void removeDeviceEventSink(Instance instance)
    {
        deviceEventDelegate_.remove(static_cast<void*>(instance));
    }

private:
    class DeviceEventDelegate
    {
    public:
        template<typename Instance, typename MfnLost, typename MfnRestored>
        void add(Instance instance, MfnLost lost, MfnRestored restored)
        {
            class DeviceEventSinks : public IDeviceEventSinks
            {
            public:
                DeviceEventSinks(Instance instance, MfnLost lost, MfnRestored restored)
                    : instance_{instance}
                    , restored_{restored}
                    , lost_{lost}
                {
                }

                void deviceLost() override
                {
                    std::invoke(lost_, instance_);
                }

                void deviceRestored() override
                {
                    std::invoke(restored_, instance_);
                }

                bool isInstance(void* instance) override
                {
                    return static_cast<void*>(instance_) == instance;
                }

            private:
                Instance instance_;
                MfnRestored restored_;
                MfnLost lost_;
            };
            deviceEventSinks_.push_back(std::make_unique<DeviceEventSinks>(instance, lost, restored));
        }

        void remove(void* instance);

        void deviceLost();
        void deviceRestored();

    private:
        struct IDeviceEventSinks
        {
            virtual void deviceLost() = 0;
            virtual void deviceRestored() = 0;
            virtual bool isInstance(void* instance) = 0;
            ~IDeviceEventSinks() = default;
        };
        std::vector<std::unique_ptr<IDeviceEventSinks>> deviceEventSinks_;
    };


    HWND hwnd_{};
    SIZE windowSize_{};
    DeviceEventDelegate deviceEventDelegate_;

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
