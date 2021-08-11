#pragma once

#ifndef DDS_SHIM_HPP_INCLUDED
#define DDS_SHIM_HPP_INCLUDED

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN

#ifndef NOMINMAX
#define NOMINMAX
#endif // NOMINMAX

#ifndef STRICT
#define STRICT
#endif // STRICT

#include <windows.h>
#include <ddraw.h>

#define DDS_BEGIN_NS namespace ddw {
#define DDS_END_NS }

#endif // DDS_SHIM_HPP_INCLUDED
