
#include "directdrawpalette.hpp"
#include "directdraw.hpp"

using Microsoft::WRL::ComPtr;

DDS_BEGIN_NS

DirectDrawPalette::ScopedMappedSubresource::ScopedMappedSubresource(ID3D11DeviceContext1* deviceContext, ID3D11Texture1D* staging)
    : deviceContext_{deviceContext}
    , staging_{staging}
{
    DDS_THROW_IF_FAILED(deviceContext_->Map(staging_, 0, D3D11_MAP_WRITE, 0, this));
}

DirectDrawPalette::ScopedMappedSubresource::~ScopedMappedSubresource()
{
    deviceContext_->Unmap(staging_, 0);
}

DirectDrawPalette::DirectDrawPalette(Microsoft::WRL::ComPtr<DirectDraw> dd, DWORD flags, LPPALETTEENTRY entries)
    : dd_{dd}
{
    createTexturesAndView();
    if (entries != nullptr)
    {
        setStageEntries(0, 256, entries);
    }
    else
    {
        PALETTEENTRY emptyPalette[256]{};
        setStageEntries(0, 256, emptyPalette);
    }
    dd_->getDeviceResources()->addDeviceEventSink(this, &DirectDrawPalette::onDeviceLost, &DirectDrawPalette::onDeviceRestored);
}

ComPtr<ID3D11ShaderResourceView> DirectDrawPalette::getPaletteView() const noexcept
{
    return paletteView_;
}

/*** IUnknown methods ***/
HRESULT DirectDrawPalette::QueryInterface(REFIID riid, LPVOID* ppvObj)
{
    if (riid == IID_IDirectDrawPalette)
    {
        *ppvObj = this;
        AddRef();
        return S_OK;
    }
    else
    {
        _ASSERT(false);
        return E_NOINTERFACE;
    }
}

ULONG DirectDrawPalette::AddRef()
{
    return InterlockedIncrement(&refCount_);
}

ULONG DirectDrawPalette::Release()
{
    auto refCount = InterlockedDecrement(&refCount_);
    if (refCount == 0)
    {
        std::destroy_at(this);
    }
    return refCount;
}

/*** IDirectDrawPalette methods ***/
HRESULT DirectDrawPalette::GetCaps(LPDWORD)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDrawPalette::GetEntries(DWORD flags, DWORD start, DWORD count, LPPALETTEENTRY entries)
{
    memcpy(entries, &paletteBackup_[start], count * sizeof(PALETTEENTRY));
    return DD_OK;
}

HRESULT DirectDrawPalette::Initialize(LPDIRECTDRAW, DWORD, LPPALETTEENTRY)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDrawPalette::SetEntries(DWORD flags, DWORD start, DWORD count, LPPALETTEENTRY entries)
{
    try
    {
        setStageEntries(start, count, entries);
    }
    catch (...)
    {
        return toHRESULT(std::current_exception());
    }
    return DD_OK;
}

void DirectDrawPalette::setStageEntries(DWORD start, DWORD count, LPPALETTEENTRY entries)
{
    memcpy(paletteBackup_, &entries[start], count * sizeof(PALETTEENTRY));
    auto deviceContext = dd_->getDeviceResources()->getDeviceContext();
    {
        ScopedMappedSubresource mapped(deviceContext, staging_.Get());

        _ASSERT(mapped.RowPitch == 1024);
        memcpy(mapped.pData, &entries[start], count * sizeof(PALETTEENTRY));
    }
    deviceContext->CopyResource(palette_.Get(), staging_.Get());
}

void DirectDrawPalette::createTexturesAndView()
{
    D3D11_TEXTURE1D_DESC textureDesc{};
    textureDesc.Width = 256;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Usage = D3D11_USAGE_STAGING;
    textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;

    auto deviceResources = dd_->getDeviceResources();
    auto device = deviceResources->getDevice();
    DDS_THROW_IF_FAILED(device->CreateTexture1D(&textureDesc, nullptr, staging_.ReleaseAndGetAddressOf()));

    textureDesc.Usage = D3D11_USAGE_DYNAMIC;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    DDS_THROW_IF_FAILED(device->CreateTexture1D(&textureDesc, nullptr, palette_.ReleaseAndGetAddressOf()));
    DDS_THROW_IF_FAILED(device->CreateShaderResourceView(palette_.Get(), nullptr, paletteView_.ReleaseAndGetAddressOf()));
}

void DirectDrawPalette::onDeviceRestored()
{
    createTexturesAndView();
    setStageEntries(0, ARRAYSIZE(paletteBackup_), paletteBackup_);
}

void DirectDrawPalette::onDeviceLost()
{
    palette_.Reset();
    paletteView_.Reset();
}

DDS_END_NS