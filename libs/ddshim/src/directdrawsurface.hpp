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
    HRESULT __stdcall QueryInterface(REFIID riid, LPVOID* ppvObj) override;
    ULONG __stdcall AddRef() override;
    ULONG __stdcall Release() override;

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
    HRESULT __stdcall AddAttachedSurface(LPDIRECTDRAWSURFACE) override;
    HRESULT __stdcall AddOverlayDirtyRect(LPRECT) override;
    HRESULT __stdcall Blt(LPRECT, LPDIRECTDRAWSURFACE, LPRECT, DWORD, LPDDBLTFX) override;
    HRESULT __stdcall BltBatch(LPDDBLTBATCH, DWORD, DWORD) override;
    HRESULT __stdcall BltFast(DWORD, DWORD, LPDIRECTDRAWSURFACE, LPRECT, DWORD) override;
    HRESULT __stdcall DeleteAttachedSurface(DWORD, LPDIRECTDRAWSURFACE) override;
    HRESULT __stdcall EnumAttachedSurfaces(LPVOID, LPDDENUMSURFACESCALLBACK) override;
    HRESULT __stdcall EnumOverlayZOrders(DWORD, LPVOID, LPDDENUMSURFACESCALLBACK) override;
    HRESULT __stdcall Flip(LPDIRECTDRAWSURFACE, DWORD) override;
    HRESULT __stdcall GetAttachedSurface(LPDDSCAPS, LPDIRECTDRAWSURFACE*) override;
    HRESULT __stdcall GetBltStatus(DWORD) override;
    HRESULT __stdcall GetCaps(LPDDSCAPS caps) override;
    HRESULT __stdcall GetClipper(LPDIRECTDRAWCLIPPER*) override;
    HRESULT __stdcall GetColorKey(DWORD, LPDDCOLORKEY) override;
    HRESULT __stdcall GetDC(HDC*) override;
    HRESULT __stdcall GetFlipStatus(DWORD) override;
    HRESULT __stdcall GetOverlayPosition(LPLONG, LPLONG) override;
    HRESULT __stdcall GetPalette(LPDIRECTDRAWPALETTE*) override;
    HRESULT __stdcall GetPixelFormat(LPDDPIXELFORMAT) override;
    HRESULT __stdcall GetSurfaceDesc(LPDDSURFACEDESC) override;
    HRESULT __stdcall Initialize(LPDIRECTDRAW, LPDDSURFACEDESC) override;
    HRESULT __stdcall IsLost() override;
    HRESULT __stdcall Lock(LPRECT, LPDDSURFACEDESC, DWORD, HANDLE) override;
    HRESULT __stdcall ReleaseDC(HDC) override;
    HRESULT __stdcall Restore() override;
    HRESULT __stdcall SetClipper(LPDIRECTDRAWCLIPPER) override;
    HRESULT __stdcall SetColorKey(DWORD, LPDDCOLORKEY) override;
    HRESULT __stdcall SetOverlayPosition(LONG, LONG) override;
    HRESULT __stdcall SetPalette(LPDIRECTDRAWPALETTE) override;
    HRESULT __stdcall Unlock(LPVOID) override;
    HRESULT __stdcall UpdateOverlay(LPRECT, LPDIRECTDRAWSURFACE, LPRECT, DWORD, LPDDOVERLAYFX) override;
    HRESULT __stdcall UpdateOverlayDisplay(DWORD) override;
    HRESULT __stdcall UpdateOverlayZOrder(DWORD, LPDIRECTDRAWSURFACE) override;

    void createTexturesAndView();
    void onDeviceRestored();
    void onDeviceLost();
};

DDS_END_NS

#endif // DDS_DIRECTDRAWSURFACE_HPP_INCLUDED
