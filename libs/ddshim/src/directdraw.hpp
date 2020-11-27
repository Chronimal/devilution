#pragma once

#ifndef DDS_DIRECTDRAW_HPP_INCLUDED
#define DDS_DIRECTDRAW_HPP_INCLUDED

#include "ddsinternal.hpp"

DDS_BEGIN_NS

class DirectDraw : public IDirectDraw
{
public:
    DirectDraw(const DirectDraw&) = delete;
    DirectDraw& operator=(const DirectDraw&) = delete;

    DirectDraw(GUID* guid, IUnknown* unkOuter);
    virtual ~DirectDraw();

    std::shared_ptr<DeviceResources> getDeviceResources() const noexcept;
    void render(ID3D11ShaderResourceView* canvasView, ID3D11ShaderResourceView* paletteView);

    /*** IUnknown methods ***/
    HRESULT __stdcall QueryInterface(REFIID riid, LPVOID* ppvObj) override;
    ULONG __stdcall AddRef() override;
    ULONG __stdcall Release() override;

private:
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
    bool inSizeMove_{};
    bool isSubclassed_{};

    /*** IDirectDraw methods ***/
    HRESULT __stdcall Compact() override;
    HRESULT __stdcall CreateClipper(DWORD dwFlags, LPDIRECTDRAWCLIPPER* lplpDDClipper, IUnknown* pUnkOuter) override;
    HRESULT __stdcall CreatePalette(DWORD dwFlags, LPPALETTEENTRY lpColorTable, LPDIRECTDRAWPALETTE* lplpDDPalette, IUnknown* pUnkOuter) override;
    HRESULT __stdcall CreateSurface(LPDDSURFACEDESC lpDDSurfaceDesc, LPDIRECTDRAWSURFACE* lplpDDSurface, IUnknown* pUnkOuter) override;
    HRESULT __stdcall DuplicateSurface(LPDIRECTDRAWSURFACE lpDDSurface, LPDIRECTDRAWSURFACE* lplpDupDDSurface) override;
    HRESULT __stdcall EnumDisplayModes(DWORD dwFlags, LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext, LPDDENUMMODESCALLBACK lpEnumCallback) override;
    HRESULT __stdcall EnumSurfaces(DWORD dwFlags, LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext, LPDDENUMSURFACESCALLBACK lpEnumCallback) override;
    HRESULT __stdcall FlipToGDISurface() override;
    HRESULT __stdcall GetCaps(LPDDCAPS lpDDDriverCaps, LPDDCAPS lpDDHELCaps) override;
    HRESULT __stdcall GetDisplayMode(LPDDSURFACEDESC lpDDSurfaceDesc) override;
    HRESULT __stdcall GetFourCCCodes(LPDWORD lpNumCodes, LPDWORD lpCodes) override;
    HRESULT __stdcall GetGDISurface(LPDIRECTDRAWSURFACE* lplpGDIDDSSurface) override;
    HRESULT __stdcall GetMonitorFrequency(LPDWORD lpdwFrequency) override;
    HRESULT __stdcall GetScanLine(LPDWORD lpdwScanLine) override;
    HRESULT __stdcall GetVerticalBlankStatus(LPBOOL lpbIsInVB) override;
    HRESULT __stdcall Initialize(GUID* lpGUID) override;
    HRESULT __stdcall RestoreDisplayMode() override;
    HRESULT __stdcall SetCooperativeLevel(HWND hWnd, DWORD dwFlags) override;
    HRESULT __stdcall SetDisplayMode(DWORD dwWidth, DWORD dwHeight, DWORD dwBpp) override;
    HRESULT __stdcall WaitForVerticalBlank(DWORD dwFlags, HANDLE hEvent) override;


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
