#pragma once

#include <RmlUi/Core/Types.h>
#include <RmlUi/Core/Input.h>
#include <RmlUi/Core/Log.h>
#include <RmlUi/Core/Platform.h>
#include <string>
#include <locale>
#include <codecvt>

namespace Shell{

    bool Initialize();

    void Shutdown();

    void LoadFonts();

    bool ProcessKeyDownShortcuts(Rml::Context* context, Rml::Input::KeyIdentifier key, int key_modifier, float native_dp_ratio, bool priority);

    std::string ConvertToString(const std::wstring& wstring);

    std::wstring ConvertToWString(const std::string& string);
}
