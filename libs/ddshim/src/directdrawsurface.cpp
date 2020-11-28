
#include "directdrawsurface.hpp"
#include "directdraw.hpp"

using Microsoft::WRL::ComPtr;

DDS_BEGIN_NS

DirectDrawSurface::DirectDrawSurface(Microsoft::WRL::ComPtr<DirectDraw> dd, LPDDSURFACEDESC surfaceDesc, const VirtualDisplayMode& vdm)
    : vdm_{vdm}
    , dd_{dd}
{
    if ((surfaceDesc->dwFlags & DDSD_CAPS) != DDSD_CAPS)
    {
        DDS_THROW(DDERR_INVALIDPARAMS);
    }
    surfaceDesc_.ddsCaps.dwCaps = DDSCAPS_3DDEVICE | DDSCAPS_SYSTEMMEMORY | surfaceDesc->dwFlags;

    createTexturesAndView();
    dd_->getDeviceResources()->addDeviceEventSink(this, &DirectDrawSurface::onDeviceLost, &DirectDrawSurface::onDeviceRestored);
}

DirectDrawSurface::~DirectDrawSurface()
{
    dd_->getDeviceResources()->removeDeviceEventSink(this);
}

DirectDrawSurface::MappedSubresource::MappedSubresource()
{
    reset();
}

long DirectDrawSurface::MappedSubresource::decRef()
{
    return ++refCount;
}

long DirectDrawSurface::MappedSubresource::incRef()
{
    return --refCount;
}

bool DirectDrawSurface::MappedSubresource::reset()
{
    if (refCount != 0)
    {
        return false;
    }
    memset(static_cast<D3D11_MAPPED_SUBRESOURCE*>(this), 0, sizeof(D3D11_MAPPED_SUBRESOURCE));
    return true;
}

/*** IUnknown methods ***/
HRESULT DirectDrawSurface::QueryInterface(REFIID riid, LPVOID* ppvObj)
{
    if (riid == IID_IDirectDrawSurface)
    {
        *ppvObj = this;
        AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}

ULONG DirectDrawSurface::AddRef()
{
    return InterlockedIncrement(&refCount_);
}

ULONG DirectDrawSurface::Release()
{
    auto refCount = InterlockedDecrement(&refCount_);
    if (refCount == 0)
    {
        std::destroy_at(this);
    }
    return refCount;
}

/*** IDirectDrawSurface methods ***/
HRESULT DirectDrawSurface::AddAttachedSurface(LPDIRECTDRAWSURFACE)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDrawSurface::AddOverlayDirtyRect(LPRECT)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDrawSurface::Blt(LPRECT, LPDIRECTDRAWSURFACE, LPRECT, DWORD, LPDDBLTFX)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDrawSurface::BltBatch(LPDDBLTBATCH, DWORD, DWORD)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDrawSurface::BltFast(DWORD, DWORD, LPDIRECTDRAWSURFACE, LPRECT, DWORD)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDrawSurface::DeleteAttachedSurface(DWORD, LPDIRECTDRAWSURFACE)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDrawSurface::EnumAttachedSurfaces(LPVOID, LPDDENUMSURFACESCALLBACK)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDrawSurface::EnumOverlayZOrders(DWORD, LPVOID, LPDDENUMSURFACESCALLBACK)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDrawSurface::Flip(LPDIRECTDRAWSURFACE, DWORD)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDrawSurface::GetAttachedSurface(LPDDSCAPS, LPDIRECTDRAWSURFACE FAR*)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDrawSurface::GetBltStatus(DWORD)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDrawSurface::GetCaps(LPDDSCAPS caps)
{
    memcpy(caps, &surfaceDesc_.ddsCaps, sizeof(surfaceDesc_.ddsCaps));
    return DD_OK;
}

HRESULT DirectDrawSurface::GetClipper(LPDIRECTDRAWCLIPPER FAR*)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDrawSurface::GetColorKey(DWORD, LPDDCOLORKEY)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDrawSurface::GetDC(HDC FAR*)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDrawSurface::GetFlipStatus(DWORD)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDrawSurface::GetOverlayPosition(LPLONG, LPLONG)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDrawSurface::GetPalette(LPDIRECTDRAWPALETTE FAR*)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDrawSurface::GetPixelFormat(LPDDPIXELFORMAT)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDrawSurface::GetSurfaceDesc(LPDDSURFACEDESC)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDrawSurface::Initialize(LPDIRECTDRAW, LPDDSURFACEDESC)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDrawSurface::IsLost()
{
    return DD_OK;
}

HRESULT DirectDrawSurface::Lock(LPRECT dstRect, LPDDSURFACEDESC surfaceDesc, DWORD flags, HANDLE hEvent)
{
    try
    {
        if (!mappedSubresource_.reset())
        {
            DDS_THROW(DDERR_LOCKEDSURFACES);
        }
        auto deviceContext = dd_->getDeviceResources()->getDeviceContext();
        DDS_THROW_IF_FAILED(deviceContext->Map(staging_.Get(), 0, D3D11_MAP_WRITE, 0, &mappedSubresource_));

        // Set video memory and pitch
        memcpy(surfaceDesc, &surfaceDesc_, sizeof(surfaceDesc_));
        surfaceDesc->dwFlags |= DDSD_LPSURFACE | DDSD_PITCH;
        surfaceDesc->lpSurface = mappedSubresource_.pData;
        surfaceDesc->lPitch = mappedSubresource_.RowPitch;

        mappedSubresource_.incRef();
    }
    catch (...)
    {
        return toHRESULT(std::current_exception());
    }
    return DD_OK;
}

HRESULT DirectDrawSurface::Unlock(LPVOID surfaceData)
{
    try
    {
        if (mappedSubresource_.decRef() != 0)
        {
            DDS_THROW(DDERR_NOTLOCKED);
        }
        auto deviceContext = dd_->getDeviceResources()->getDeviceContext();
        deviceContext->Unmap(staging_.Get(), 0);
        mappedSubresource_.reset();

        deviceContext->CopyResource(canvas_.Get(), staging_.Get());
        auto paletteView = ddPalette_->getPaletteView();
        auto canvasView = canvasView_;

        dd_->render(canvasView.Get(), paletteView.Get());
    }
    catch (...)
    {
        return toHRESULT(std::current_exception());
    }
    return DD_OK;
}

HRESULT DirectDrawSurface::ReleaseDC(HDC)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDrawSurface::Restore()
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDrawSurface::SetClipper(LPDIRECTDRAWCLIPPER)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDrawSurface::SetColorKey(DWORD, LPDDCOLORKEY)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDrawSurface::SetOverlayPosition(LONG, LONG)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDrawSurface::SetPalette(LPDIRECTDRAWPALETTE lpDDPalette)
{
    ComPtr<DirectDrawPalette> palette(static_cast<DirectDrawPalette*>(lpDDPalette));
    ddPalette_.Swap(palette);
    return DD_OK;
}

HRESULT DirectDrawSurface::UpdateOverlay(LPRECT, LPDIRECTDRAWSURFACE, LPRECT, DWORD, LPDDOVERLAYFX)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDrawSurface::UpdateOverlayDisplay(DWORD)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

HRESULT DirectDrawSurface::UpdateOverlayZOrder(DWORD, LPDIRECTDRAWSURFACE)
{
    _ASSERT(false);
    return E_NOTIMPL;
}

void DirectDrawSurface::createTexturesAndView()
{
    D3D11_TEXTURE2D_DESC textureDesc{};
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_STAGING;
    textureDesc.Width = vdm_.width;
    textureDesc.Format = DXGI_FORMAT_R8_UINT;
    textureDesc.Height = vdm_.height;
    textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    auto device = dd_->getDeviceResources()->getDevice();
    DDS_THROW_IF_FAILED(device->CreateTexture2D(&textureDesc, nullptr, staging_.ReleaseAndGetAddressOf()));

    textureDesc.Usage = D3D11_USAGE_DYNAMIC;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    DDS_THROW_IF_FAILED(device->CreateTexture2D(&textureDesc, nullptr, canvas_.ReleaseAndGetAddressOf()));
    DDS_THROW_IF_FAILED(device->CreateShaderResourceView(canvas_.Get(), nullptr, canvasView_.ReleaseAndGetAddressOf()));
}

void DirectDrawSurface::onDeviceRestored()
{
    createTexturesAndView();
}

void DirectDrawSurface::onDeviceLost()
{
    canvas_.Reset();
    canvasView_.Reset();
}

DDS_END_NS