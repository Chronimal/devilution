#pragma once

#ifndef DDS_DIRECTDRAWPALETTE_HPP_INCLUDED
#define DDS_DIRECTDRAWPALETTE_HPP_INCLUDED

#include "ddsinternal.hpp"

DDS_BEGIN_NS

class DirectDrawPalette : public IDirectDrawPalette
{
public:
    DirectDrawPalette(const DirectDrawPalette&) = delete;
    DirectDrawPalette& operator=(const DirectDrawPalette&) = delete;

    DirectDrawPalette(Microsoft::WRL::ComPtr<DirectDraw> dd, DWORD flags, LPPALETTEENTRY entries);
    virtual ~DirectDrawPalette();

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> getPaletteView() const noexcept;

    /*** IUnknown methods ***/
    HRESULT __stdcall QueryInterface(REFIID riid, LPVOID* ppvObj) override;
    ULONG __stdcall AddRef() override;
    ULONG __stdcall Release() override;

private:
    class ScopedMappedSubresource : public D3D11_MAPPED_SUBRESOURCE
    {
    public:
        ScopedMappedSubresource(const ScopedMappedSubresource&) = delete;
        ScopedMappedSubresource& operator=(const ScopedMappedSubresource&) = delete;
        ScopedMappedSubresource(ID3D11DeviceContext1* deviceContext, ID3D11Texture1D* staging);
        ~ScopedMappedSubresource();

    private:
        ID3D11DeviceContext1* deviceContext_;
        ID3D11Texture1D* staging_;
    };

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> paletteView_;
    Microsoft::WRL::ComPtr<ID3D11Texture1D> palette_;
    Microsoft::WRL::ComPtr<ID3D11Texture1D> staging_;
    Microsoft::WRL::ComPtr<DirectDraw> dd_;
    PALETTEENTRY paletteBackup_[256];
    ULONG refCount_{1};

    /*** IDirectDrawPalette methods ***/
    HRESULT __stdcall GetCaps(LPDWORD) override;
    HRESULT __stdcall GetEntries(DWORD flags, DWORD start, DWORD count, LPPALETTEENTRY entries) override;
    HRESULT __stdcall Initialize(LPDIRECTDRAW, DWORD, LPPALETTEENTRY) override;
    HRESULT __stdcall SetEntries(DWORD flags, DWORD start, DWORD count, LPPALETTEENTRY entries) override;

    void setStageEntries(DWORD start, DWORD count, LPPALETTEENTRY entries);
    void createTexturesAndView();
    void onDeviceRestored();
    void onDeviceLost();
};

DDS_END_NS

#endif // DDS_DIRECTDRAWPALETTE_HPP_INCLUDED
