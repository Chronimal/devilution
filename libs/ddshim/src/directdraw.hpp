#pragma once

#ifndef DDS_DIRECTDRAW_HPP_INCLUDED
#define DDS_DIRECTDRAW_HPP_INCLUDED

#include "ddsinternal.hpp"
#include "d3dkmt.hpp"

DDS_BEGIN_NS

class DirectDraw : public IDirectDraw
{
public:
    DirectDraw(const DirectDraw&) = delete;
    DirectDraw& operator=(const DirectDraw&) = delete;

    DirectDraw(GUID* guid);
    virtual ~DirectDraw();

    std::shared_ptr<DeviceResources> getDeviceResources() const noexcept;
    void render(ID3D11ShaderResourceView* canvasView, ID3D11ShaderResourceView* paletteView);

    /*** IUnknown methods ***/
    HRESULT __stdcall QueryInterface(REFIID riid, LPVOID* ppvObj) noexcept override;
    ULONG __stdcall AddRef() noexcept override;
    ULONG __stdcall Release() noexcept override;

private:
    struct D3DKMT_HANDLETraits
    {
        using Type = D3DKMT_HANDLE;

        inline static bool Close(Type h) noexcept
        {
            D3DKMT_CLOSEADAPTER ca{.hAdapter = h};
            return D3DKMTCloseAdapter(&ca) >= 0;
        }

        inline static Type GetInvalidValue() noexcept
        {
            return static_cast<Type>(0);
        }
    };
    using D3DKMTHandle = Microsoft::WRL::Wrappers::HandleT<D3DKMT_HANDLETraits>;

    std::shared_ptr<DeviceResources> deviceResources_;
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer_;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout_;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader_;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader_;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState_;

    UINT stride_{};
    UINT offset_{};
    UINT numVerts_{};

    ULONG refCount_{1};
    VirtualDisplayMode vdm_{640, 480, 8};
    D3DKMTHandle adapter_;
    UINT vidPnSourceId_{};
    bool isSubclassed_{};
    bool inSizeMove_{};

    /*** IDirectDraw methods ***/
    HRESULT __stdcall Compact() noexcept override;
    HRESULT __stdcall CreateClipper(DWORD dwFlags, LPDIRECTDRAWCLIPPER* lplpDDClipper, IUnknown* pUnkOuter) noexcept override;
    HRESULT __stdcall CreatePalette(DWORD dwFlags, LPPALETTEENTRY lpColorTable, LPDIRECTDRAWPALETTE* lplpDDPalette, IUnknown*) noexcept override;
    HRESULT __stdcall CreateSurface(LPDDSURFACEDESC lpDDSurfaceDesc, LPDIRECTDRAWSURFACE* lplpDDSurface, IUnknown* pUnkOuter) noexcept override;
    HRESULT __stdcall DuplicateSurface(LPDIRECTDRAWSURFACE lpDDSurface, LPDIRECTDRAWSURFACE* lplpDupDDSurface) noexcept override;
    HRESULT __stdcall EnumDisplayModes(DWORD dwFlags, LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext, LPDDENUMMODESCALLBACK lpEnumCallback) noexcept override;
    HRESULT __stdcall EnumSurfaces(DWORD dwFlags, LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext, LPDDENUMSURFACESCALLBACK lpEnumCallback) noexcept override;
    HRESULT __stdcall FlipToGDISurface() noexcept override;
    HRESULT __stdcall GetCaps(LPDDCAPS lpDDDriverCaps, LPDDCAPS lpDDHELCaps) noexcept override;
    HRESULT __stdcall GetDisplayMode(LPDDSURFACEDESC lpDDSurfaceDesc) noexcept override;
    HRESULT __stdcall GetFourCCCodes(LPDWORD lpNumCodes, LPDWORD lpCodes) noexcept override;
    HRESULT __stdcall GetGDISurface(LPDIRECTDRAWSURFACE* lplpGDIDDSSurface) noexcept override;
    HRESULT __stdcall GetMonitorFrequency(LPDWORD lpdwFrequency) noexcept override;
    HRESULT __stdcall GetScanLine(LPDWORD lpdwScanLine) noexcept override;
    HRESULT __stdcall GetVerticalBlankStatus(LPBOOL lpbIsInVB) noexcept override;
    HRESULT __stdcall Initialize(GUID* lpGUID) noexcept override;
    HRESULT __stdcall RestoreDisplayMode() noexcept override;
    HRESULT __stdcall SetCooperativeLevel(HWND hWnd, DWORD dwFlags) noexcept override;
    HRESULT __stdcall SetDisplayMode(DWORD dwWidth, DWORD dwHeight, DWORD dwBpp) noexcept override;
    HRESULT __stdcall WaitForVerticalBlank(DWORD dwFlags, HANDLE hEvent) noexcept override;

    void createDeviceDependentResources();
    void createWindowSizeDependentResources();

    void onDeviceLost();
    void onDeviceRestored();
    void onWindowSizeChanged(int width, int height);
    void onWindowMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    void onGetMinMaxInfo(HWND hwnd, WPARAM wParam, LPARAM lParam);
    void onSysKeyDown(HWND hwnd, WPARAM wParam, LPARAM lParam);
    void onSize(HWND hwnd, WPARAM wParam, LPARAM lParam);
    void onEnterSizeMove(HWND hwnd, WPARAM wParam, LPARAM lParam);
    void onExitSizeMove(HWND hwnd, WPARAM wParam, LPARAM lParam);

    static LRESULT CALLBACK subclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR subclassID, DWORD_PTR userData);
    void unsubclassWindow(HWND hwnd) noexcept;
    void subclassWindow(HWND hwnd);
};

DDS_END_NS

#endif // DDS_DIRECTDRAW_HPP_INCLUDED
