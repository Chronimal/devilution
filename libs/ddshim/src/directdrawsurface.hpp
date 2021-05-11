#pragma once

#ifndef DDS_DIRECTDRAWSURFACE_HPP_INCLUDED
#define DDS_DIRECTDRAWSURFACE_HPP_INCLUDED

#include "ddsinternal.hpp"
#include "directdrawpalette.hpp"

DDS_BEGIN_NS

class DirectDrawSurface : public IDirectDrawSurface
{
public:
    DirectDrawSurface(const DirectDrawSurface&) = delete;
    DirectDrawSurface& operator=(const DirectDrawSurface&) = delete;

    DirectDrawSurface(Microsoft::WRL::ComPtr<DirectDraw> dd, LPDDSURFACEDESC surfaceDesc, const VirtualDisplayMode& vdm);
    virtual ~DirectDrawSurface();

    /*** IUnknown methods ***/
    HRESULT __stdcall QueryInterface(REFIID riid, LPVOID* ppvObj) noexcept override;
    ULONG __stdcall AddRef() noexcept override;
    ULONG __stdcall Release() noexcept override;

private:
    class MappedSubresource : public D3D11_MAPPED_SUBRESOURCE
    {
    public:
        MappedSubresource();
        long incRef();
        long decRef();
        bool reset();

    private:
        long refCount{};
    };

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> canvasView_;
    Microsoft::WRL::ComPtr<DirectDrawPalette> ddPalette_;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> staging_;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> canvas_;
    Microsoft::WRL::ComPtr<DirectDraw> dd_;
    MappedSubresource mappedSubresource_;
    DDSURFACEDESC surfaceDesc_{};
    ULONG refCount_{1};

    VirtualDisplayMode vdm_;
    bool surfaceHasBeenLost_{};

    /*** IDirectDrawSurface methods ***/
    HRESULT __stdcall AddAttachedSurface(LPDIRECTDRAWSURFACE) noexcept override;
    HRESULT __stdcall AddOverlayDirtyRect(LPRECT) noexcept override;
    HRESULT __stdcall Blt(LPRECT, LPDIRECTDRAWSURFACE, LPRECT, DWORD, LPDDBLTFX) noexcept override;
    HRESULT __stdcall BltBatch(LPDDBLTBATCH, DWORD, DWORD) noexcept override;
    HRESULT __stdcall BltFast(DWORD, DWORD, LPDIRECTDRAWSURFACE, LPRECT, DWORD) noexcept override;
    HRESULT __stdcall DeleteAttachedSurface(DWORD, LPDIRECTDRAWSURFACE) noexcept override;
    HRESULT __stdcall EnumAttachedSurfaces(LPVOID, LPDDENUMSURFACESCALLBACK) noexcept override;
    HRESULT __stdcall EnumOverlayZOrders(DWORD, LPVOID, LPDDENUMSURFACESCALLBACK) noexcept override;
    HRESULT __stdcall Flip(LPDIRECTDRAWSURFACE, DWORD) noexcept override;
    HRESULT __stdcall GetAttachedSurface(LPDDSCAPS, LPDIRECTDRAWSURFACE*) noexcept override;
    HRESULT __stdcall GetBltStatus(DWORD) noexcept override;
    HRESULT __stdcall GetCaps(LPDDSCAPS caps) noexcept override;
    HRESULT __stdcall GetClipper(LPDIRECTDRAWCLIPPER*) noexcept override;
    HRESULT __stdcall GetColorKey(DWORD, LPDDCOLORKEY) noexcept override;
    HRESULT __stdcall GetDC(HDC*) noexcept override;
    HRESULT __stdcall GetFlipStatus(DWORD) noexcept override;
    HRESULT __stdcall GetOverlayPosition(LPLONG, LPLONG) noexcept override;
    HRESULT __stdcall GetPalette(LPDIRECTDRAWPALETTE*) noexcept override;
    HRESULT __stdcall GetPixelFormat(LPDDPIXELFORMAT) noexcept override;
    HRESULT __stdcall GetSurfaceDesc(LPDDSURFACEDESC) noexcept override;
    HRESULT __stdcall Initialize(LPDIRECTDRAW, LPDDSURFACEDESC) noexcept override;
    HRESULT __stdcall IsLost() noexcept override;
    HRESULT __stdcall Lock(LPRECT, LPDDSURFACEDESC, DWORD, HANDLE) noexcept override;
    HRESULT __stdcall ReleaseDC(HDC) noexcept override;
    HRESULT __stdcall Restore() noexcept override;
    HRESULT __stdcall SetClipper(LPDIRECTDRAWCLIPPER) noexcept override;
    HRESULT __stdcall SetColorKey(DWORD, LPDDCOLORKEY) noexcept override;
    HRESULT __stdcall SetOverlayPosition(LONG, LONG) noexcept override;
    HRESULT __stdcall SetPalette(LPDIRECTDRAWPALETTE) noexcept override;
    HRESULT __stdcall Unlock(LPVOID) noexcept override;
    HRESULT __stdcall UpdateOverlay(LPRECT, LPDIRECTDRAWSURFACE, LPRECT, DWORD, LPDDOVERLAYFX) noexcept override;
    HRESULT __stdcall UpdateOverlayDisplay(DWORD) noexcept override;
    HRESULT __stdcall UpdateOverlayZOrder(DWORD, LPDIRECTDRAWSURFACE) noexcept override;

    void createTexturesAndView();
    void onDeviceRestored();
    void onDeviceLost();
};

DDS_END_NS

#endif // DDS_DIRECTDRAWSURFACE_HPP_INCLUDED
