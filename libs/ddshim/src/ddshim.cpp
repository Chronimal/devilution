
#include "directdraw.hpp"

DDS_BEGIN_NS

ComRuntimeError::ComRuntimeError(HRESULT hr, const char* file, int line)
    : runtime_error{formatError(hr)}
    , hr_(hr)
{
}

HRESULT ComRuntimeError::getResult() const noexcept
{
    return hr_;
}

const char* ComRuntimeError::formatError(HRESULT hr) noexcept
{
    thread_local static char message[64]{};
    sprintf_s(message, "Failure with HRESULT of %08X", static_cast<unsigned int>(hr));
    return message;
}

VirtualDisplayMode::VirtualDisplayMode(UINT width, UINT height, UINT bpp) noexcept
    : width{width}
    , height{height}
    , bpp{bpp}
{
}

SIZE getClientSize(HWND hwnd)
{
    RECT rect{};
    if (!GetClientRect(hwnd, &rect))
    {
        DDS_THROW_IF_FAILED(HRESULT_FROM_WIN32(GetLastError()));
    }
    return {rect.right - rect.left, rect.bottom - rect.top};
}

HRESULT throwIfFailed(HRESULT hr, const char* file, int line) noexcept(false)
{
    if (FAILED(hr))
    {
        throw ComRuntimeError(hr, file, line);
    }
    return hr;
}

void throwUnconditional(HRESULT hr, const char* file, int line) noexcept(false)
{
    throw ComRuntimeError(hr != DD_OK ? hr : E_UNEXPECTED, file, line);
}

inline HRESULT toHRESULT(std::exception_ptr ep) noexcept
{
    try
    {
        std::rethrow_exception(ep);
    }
    catch (const ComRuntimeError& e)
    {
        return e.getResult();
    }
    catch (...)
    {
        return E_UNEXPECTED;
    }
}

//--- The one and only public interface function --

HRESULT directDrawCreate(GUID* guid, LPDIRECTDRAW* dd, IUnknown*) noexcept
{
    try
    {
        *dd = std::make_unique<DirectDraw>(guid).release();
    }
    catch (...)
    {
        return toHRESULT(std::current_exception());
    }
    return DD_OK;
}

DDS_END_NS