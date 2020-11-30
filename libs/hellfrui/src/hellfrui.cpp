
#include "../include/hellfrui/hellfrui.h"

// clang-format off
#define HUI_BEGIN_ANON_NS namespace {
#define HUI_END_ANON_NS }
// clang-format on

HUI_BEGIN_ANON_NS

using UiDestroyFn = void (__stdcall *)();

using UiTitleDialogFn = BOOL (__stdcall *)(int a1);

using UiSetSpawnedFn = void (__stdcall *)(BOOL bSpawned);

using UiInitializeFn = void (__stdcall *)();

using UiCopyProtErrorFn = BOOL (__stdcall *)(int* pdwResult);

using UiAppActivateFn = void (__stdcall *)(BOOL bActive);

using UiValidPlayerNameFn = BOOL (__fastcall *)(const char* name); /* check __stdcall */

using UiSelHeroMultDialogFn = BOOL (__stdcall *)(
    BOOL(__stdcall* fninfo)(BOOL(__stdcall* fninfofunc)(_uiheroinfo*)),
    BOOL(__stdcall* fncreate)(_uiheroinfo*),
    BOOL(__stdcall* fnremove)(_uiheroinfo*),
    BOOL(__stdcall* fnstats)(unsigned int, _uidefaultstats*),
    int* dlgresult,
    BOOL* hero_is_created,
    char* name);

using UiSelHeroSingDialogFn = BOOL (__stdcall *)(
    BOOL(__stdcall* fninfo)(BOOL(__stdcall* fninfofunc)(_uiheroinfo*)),
    BOOL(__stdcall* fncreate)(_uiheroinfo*),
    BOOL(__stdcall* fnremove)(_uiheroinfo*),
    BOOL(__stdcall* fnstats)(unsigned int, _uidefaultstats*),
    int* dlgresult,
    char* name,
    int* difficulty,
    BOOLEAN a8,
    BOOLEAN a9);

using UiCreditsDialogFn = BOOL (__stdcall *)(int a1);

using UiMainMenuDialogFn = BOOL (__stdcall *)(const char* name, int* pdwResult, BOOLEAN a3, void(__stdcall* fnSound)(const char* file), int attractTimeOut);

using UiProgressDialogFn = BOOL (__stdcall *)(HWND window, const char* msg, int enable, int (*fnfunc)(), int rate);

using UiProfileCallbackFn = void (__cdecl *)();

using UiProfileDrawFn = void (__cdecl *)();

using UiCategoryCallbackFn = BOOL (__stdcall *)(int a1, int a2, int a3, int a4, int a5, DWORD* a6, DWORD* a7);

using UiGetDataCallbackFn = BOOL (__stdcall *)(int game_type, int data_code, void* a3, int a4, int a5);

using UiAuthCallbackFn = BOOL (__stdcall *)(int a1, char* a2, char* a3, char a4, char* a5, LPSTR lpBuffer, int cchBufferMax);

using UiSoundCallbackFn = BOOL (__stdcall *)(int a1, int type, int a3);

using UiMessageBoxCallbackFn = void (__stdcall *)(HWND hWnd, char* lpText, LPCSTR lpCaption, UINT uType);

using UiDrawDescCallbackFn = BOOL (__stdcall *)(int game_type, COLORREF color, LPCSTR lpString, char* a4, int a5, UINT align, time_t a7, HDC* a8);

using UiCreateGameCallbackFn = BOOL (__stdcall *)(int a1, int a2, int a3, int a4, int a5, int a6);

using UiArtCallbackFn = BOOL (__stdcall *)(int game_type, unsigned int art_code, PALETTEENTRY* pPalette, BYTE* pBuffer, DWORD dwBuffersize, DWORD* pdwWidth, DWORD* pdwHeight, DWORD* pdwBpp);

using UiSelectGameFn = int (__stdcall *)(int a1, _SNETPROGRAMDATA* client_info, _SNETPLAYERDATA* user_info, _SNETUIDATA* ui_info, _SNETVERSIONDATA* file_info, int* a6);

using UiSelectProviderFn = int (__stdcall *)(int a1, _SNETPROGRAMDATA* client_info, _SNETPLAYERDATA* user_info, _SNETUIDATA* ui_info, _SNETVERSIONDATA* file_info, int* type);

using UiCreatePlayerDescriptionFn = BOOL (__stdcall *)(_uiheroinfo* info, DWORD mode, char* desc);

using UiSetupPlayerInfoFn = void (__stdcall *)(char* infostr, _uiheroinfo* pInfo, DWORD type);

using UiCreateGameCriteriaFn = void (__stdcall *)(_uiheroinfo* pInfo, char* str);

using UiGetDefaultStatsFn = BOOL (__stdcall *)(int pclass, _uidefaultstats* pStats);

using UiBetaDisclaimerFn = BOOL (__stdcall *)(int a1);

class DllLoader
{
public:
    DllLoader()
        : dll_{LoadLibraryW(L"hellfrui.dll")}
    {
    }

    ~DllLoader()
    {
        FreeLibrary(dll_);
    }

protected:
    HMODULE dll_;

    template<typename T>
    T getProcAddress(const char* name)
    {
        return reinterpret_cast<T>(GetProcAddress(dll_, name));
    }
};


class UI : public DllLoader
{
public:
    UiDestroyFn destroy = getProcAddress<UiDestroyFn>("UiDestroy");
    UiTitleDialogFn titleDialog = getProcAddress<UiTitleDialogFn>("UiTitleDialog");
    UiSetSpawnedFn setSpawned = getProcAddress<UiSetSpawnedFn>("UiSetSpawned");
    UiInitializeFn initialize = getProcAddress<UiInitializeFn>("UiInitialize");
    UiCopyProtErrorFn copyProtError = getProcAddress<UiCopyProtErrorFn>("UiCopyProtError");
    UiAppActivateFn appActivate = getProcAddress<UiAppActivateFn>("UiAppActivate");
    UiValidPlayerNameFn validPlayerName = getProcAddress<UiValidPlayerNameFn>("UiValidPlayerName");
    UiSelHeroMultDialogFn selHeroMultDialog = getProcAddress<UiSelHeroMultDialogFn>("UiSelHeroMultDialog");
    UiSelHeroSingDialogFn selHeroSingDialog = getProcAddress<UiSelHeroSingDialogFn>("UiSelHeroSingDialog");
    UiCreditsDialogFn creditsDialog = getProcAddress<UiCreditsDialogFn>("UiCreditsDialog");
    UiMainMenuDialogFn mainMenuDialog = getProcAddress<UiMainMenuDialogFn>("UiMainMenuDialog");
    UiProgressDialogFn progressDialog = getProcAddress<UiProgressDialogFn>("UiProgressDialog");
    UiProfileCallbackFn profileCallback = getProcAddress<UiProfileCallbackFn>("UiProfileCallback");
    UiProfileDrawFn profileDraw = getProcAddress<UiProfileDrawFn>("UiProfileDraw");
    UiCategoryCallbackFn categoryCallback = getProcAddress<UiCategoryCallbackFn>("UiCategoryCallback");
    UiGetDataCallbackFn getDataCallback = getProcAddress<UiGetDataCallbackFn>("UiGetDataCallback");
    UiAuthCallbackFn authCallback = getProcAddress<UiAuthCallbackFn>("UiAuthCallback");
    UiSoundCallbackFn soundCallback = getProcAddress<UiSoundCallbackFn>("UiSoundCallback");
    UiMessageBoxCallbackFn messageBoxCallback = getProcAddress<UiMessageBoxCallbackFn>("UiMessageBoxCallback");
    UiDrawDescCallbackFn drawDescCallback = getProcAddress<UiDrawDescCallbackFn>("UiDrawDescCallback");
    UiCreateGameCallbackFn createGameCallback = getProcAddress<UiCreateGameCallbackFn>("UiCreateGameCallback");
    UiArtCallbackFn artCallback = getProcAddress<UiArtCallbackFn>("UiArtCallback");
    UiSelectGameFn selectGame = getProcAddress<UiSelectGameFn>("UiSelectGame");
    UiSelectProviderFn selectProvider = getProcAddress<UiSelectProviderFn>("UiSelectProvider");
    UiCreatePlayerDescriptionFn createPlayerDescription = getProcAddress<UiCreatePlayerDescriptionFn>("UiCreatePlayerDescription");
    UiSetupPlayerInfoFn setupPlayerInfo = getProcAddress<UiSetupPlayerInfoFn>("UiSetupPlayerInfo");
    UiCreateGameCriteriaFn createGameCriteria = getProcAddress<UiCreateGameCriteriaFn>("UiCreateGameCriteria");
    UiGetDefaultStatsFn getDefaultStats = getProcAddress<UiGetDefaultStatsFn>("UiGetDefaultStats");
    UiBetaDisclaimerFn betaDisclaimer = getProcAddress<UiBetaDisclaimerFn>("UiBetaDisclaimer");
} ui;

HUI_END_ANON_NS

void __stdcall UiDestroy()
{
    ui.destroy();
}

BOOL __stdcall UiTitleDialog(int a1)
{
    return ui.titleDialog(a1);
}

void __stdcall UiSetSpawned(BOOL bSpawned)
{
    ui.setSpawned(bSpawned);
}

void __stdcall UiInitialize()
{
    ui.initialize();
}

BOOL __stdcall UiCopyProtError(int* pdwResult)
{
    return ui.copyProtError(pdwResult);
}

void __stdcall UiAppActivate(BOOL bActive)
{
    ui.appActivate(bActive);
}

BOOL __fastcall UiValidPlayerName(const char* name) /* check __stdcall */
{
    return ui.validPlayerName(name);
}

BOOL __stdcall UiSelHeroMultDialog(
    BOOL(__stdcall* fninfo)(BOOL(__stdcall* fninfofunc)(_uiheroinfo*)),
    BOOL(__stdcall* fncreate)(_uiheroinfo*),
    BOOL(__stdcall* fnremove)(_uiheroinfo*),
    BOOL(__stdcall* fnstats)(unsigned int, _uidefaultstats*),
    int* dlgresult,
    BOOL* hero_is_created,
    char* name)
{
    return ui.selHeroMultDialog(fninfo, fncreate, fnremove, fnstats, dlgresult, hero_is_created, name);
}

BOOL __stdcall UiSelHeroSingDialog(
    BOOL(__stdcall* fninfo)(BOOL(__stdcall* fninfofunc)(_uiheroinfo*)),
    BOOL(__stdcall* fncreate)(_uiheroinfo*),
    BOOL(__stdcall* fnremove)(_uiheroinfo*),
    BOOL(__stdcall* fnstats)(unsigned int, _uidefaultstats*),
    int* dlgresult,
    char* name,
    int* difficulty,
    BOOLEAN a8,
    BOOLEAN a9)
{
    return ui.selHeroSingDialog(fninfo, fncreate, fnremove, fnstats, dlgresult, name, difficulty, a8, a9);
}

BOOL __stdcall UiCreditsDialog(int a1)
{
    return ui.creditsDialog(a1);
}

BOOL __stdcall UiMainMenuDialog(const char* name, int* pdwResult, BOOLEAN a3, void(__stdcall* fnSound)(const char* file), int attractTimeOut)
{
    return ui.mainMenuDialog(name, pdwResult, a3, fnSound, attractTimeOut);
}

BOOL __stdcall UiProgressDialog(HWND window, const char* msg, int enable, int (*fnfunc)(), int rate)
{
    return ui.progressDialog(window, msg, enable, fnfunc, rate);
}

void __cdecl UiProfileCallback()
{
    ui.profileCallback();
}

void __cdecl UiProfileDraw()
{
    ui.profileDraw();
}

BOOL __stdcall UiCategoryCallback(int a1, int a2, int a3, int a4, int a5, DWORD* a6, DWORD* a7)
{
    return ui.categoryCallback(a1, a2, a3, a4, a5, a6, a7);
}

BOOL __stdcall UiGetDataCallback(int game_type, int data_code, void* a3, int a4, int a5)
{
    return ui.getDataCallback(game_type, data_code, a3, a4, a5);
}

BOOL __stdcall UiAuthCallback(int a1, char* a2, char* a3, char a4, char* a5, LPSTR lpBuffer, int cchBufferMax)
{
    return ui.authCallback(a1, a2, a3, a4, a5, lpBuffer, cchBufferMax);
}

BOOL __stdcall UiSoundCallback(int a1, int type, int a3)
{
    return ui.soundCallback(a1, type, a3);
}

void __stdcall UiMessageBoxCallback(HWND hWnd, char* lpText, LPCSTR lpCaption, UINT uType)
{
    ui.messageBoxCallback(hWnd, lpText, lpCaption, uType);
}

BOOL __stdcall UiDrawDescCallback(int game_type, COLORREF color, LPCSTR lpString, char* a4, int a5, UINT align, time_t a7, HDC* a8)
{
    return ui.drawDescCallback(game_type, color, lpString, a4, a5, align, a7, a8);
}

BOOL __stdcall UiCreateGameCallback(int a1, int a2, int a3, int a4, int a5, int a6)
{
    return FALSE;
}

BOOL __stdcall UiArtCallback(int game_type, unsigned int art_code, PALETTEENTRY* pPalette, BYTE* pBuffer, DWORD dwBuffersize, DWORD* pdwWidth, DWORD* pdwHeight, DWORD* pdwBpp)
{
    return ui.artCallback(game_type, art_code, pPalette, pBuffer, dwBuffersize, pdwWidth, pdwHeight, pdwBpp);
}

int __stdcall UiSelectGame(int a1, _SNETPROGRAMDATA* client_info, _SNETPLAYERDATA* user_info, _SNETUIDATA* ui_info, _SNETVERSIONDATA* file_info, int* a6)
{
    return ui.selectGame(a1, client_info, user_info, ui_info, file_info, a6);
}

int __stdcall UiSelectProvider(int a1, _SNETPROGRAMDATA* client_info, _SNETPLAYERDATA* user_info, _SNETUIDATA* ui_info, _SNETVERSIONDATA* file_info, int* type)
{
    return ui.selectProvider(a1, client_info, user_info, ui_info, file_info, type);
}

BOOL __stdcall UiCreatePlayerDescription(_uiheroinfo* info, DWORD mode, char* desc)
{
    return ui.createPlayerDescription(info, mode, desc);
}

void __stdcall UiSetupPlayerInfo(char* infostr, _uiheroinfo* pInfo, DWORD type)
{
    ui.setupPlayerInfo(infostr, pInfo, type);
}

void __stdcall UiCreateGameCriteria(_uiheroinfo* pInfo, char* str)
{
    ui.createGameCriteria(pInfo, str);
}

BOOL __stdcall UiGetDefaultStats(int pclass, _uidefaultstats* pStats)
{
    return ui.getDefaultStats(pclass, pStats);
}

BOOL __stdcall UiBetaDisclaimer(int a1)
{
    return ui.betaDisclaimer(a1);
}
