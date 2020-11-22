#pragma once

#ifndef DDW_DDWRAPPER_HPP_INCLUDED
#define DDW_DDWRAPPER_HPP_INCLUDED

#include <windows.h>
#include <ddraw.h>

#pragma comment(lib, "ddraw.lib")

#define DDW_BEGIN_NS namespace ddw {
#define DDW_END_NS }

DDW_BEGIN_NS

HRESULT directDrawCreate(GUID* guid, LPDIRECTDRAW* ddInterface, IUnknown* unkOuter) noexcept;

DDW_END_NS

#endif // DDW_DDWRAPPER_HPP_INCLUDED
