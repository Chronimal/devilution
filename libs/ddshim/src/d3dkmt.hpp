// Definitions and declarations taken from D3dkmthk.h in order
// to avoid to introduce a DDK dependency.

#ifndef DDS_D3DKMT_HPP_INCLUDED
#define DDS_D3DKMT_HPP_INCLUDED

using D3DKMT_HANDLE = UINT;

struct D3DKMT_CLOSEADAPTER
{
    D3DKMT_HANDLE hAdapter; // in: adapter handle
};

struct D3DKMT_GETSCANLINE
{
    D3DKMT_HANDLE hAdapter;  // in: Adapter handle
    UINT VidPnSourceId;      // in: Adapter's VidPN Source ID
    BOOLEAN InVerticalBlank; // out: Within vertical blank
    UINT ScanLine;           // out: Current scan line
};

struct D3DKMT_WAITFORVERTICALBLANKEVENT
{
    D3DKMT_HANDLE hAdapter; // in: adapter handle
    D3DKMT_HANDLE hDevice;  // in: device handle [Optional]
    UINT VidPnSourceId;     // in: adapter's VidPN Source ID
};

struct D3DKMT_OPENADAPTERFROMHDC
{
    HDC hDc;                // in:  DC that maps to a single display
    D3DKMT_HANDLE hAdapter; // out: adapter handle
    LUID AdapterLuid;       // out: adapter LUID
    UINT VidPnSourceId;     // out: VidPN source ID for that particular display
};

EXTERN_C _Check_return_ NTSTATUS APIENTRY D3DKMTWaitForVerticalBlankEvent(_In_ CONST D3DKMT_WAITFORVERTICALBLANKEVENT*);
EXTERN_C _Check_return_ NTSTATUS APIENTRY D3DKMTOpenAdapterFromHdc(_Inout_ D3DKMT_OPENADAPTERFROMHDC*);
EXTERN_C _Check_return_ NTSTATUS APIENTRY D3DKMTCloseAdapter(_In_ CONST D3DKMT_CLOSEADAPTER*);
EXTERN_C _Check_return_ NTSTATUS APIENTRY D3DKMTGetScanLine(_Inout_ D3DKMT_GETSCANLINE*);

#endif // DDS_D3DKMT_HPP_INCLUDED
