

#include "internal.hpp"

DDW_BEGIN_NS

HRESULT directDrawCreate(GUID* guid, LPDIRECTDRAW* ddInterface, IUnknown* unkOuter) noexcept
{
    return DirectDrawCreate(guid, ddInterface, unkOuter);
}

DDW_END_NS