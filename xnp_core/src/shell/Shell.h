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

}
