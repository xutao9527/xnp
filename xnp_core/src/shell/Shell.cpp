#include "Shell.h"
#include "ShellFileInterface.h"
#include <filesystem>
#include <string>
#include <RmlUi/Core/Core.h>

static Rml::UniquePtr<ShellFileInterface> file_interface;

bool Shell::Initialize()
{
    auto findRootFile = [] {
        std::filesystem::path currentPath = std::filesystem::current_path();
        return currentPath.string();
    };
    std::string root = findRootFile();
    file_interface = Rml::MakeUnique<ShellFileInterface>(root);
    Rml::SetFileInterface(file_interface.get());
    return true;
}

void Shell::Shutdown()
{
    file_interface.reset();
}


void Shell::LoadFonts()
{
    const Rml::String directory = "resources/font/";

    struct FontFace
    {
        const char *filename;
        bool fallback_face;
    };
    FontFace font_faces[] = {
        // {"LatoLatin-Regular.ttf", false},
        // {"LatoLatin-Italic.ttf", false},
        // {"LatoLatin-Bold.ttf", false},
        // {"LatoLatin-BoldItalic.ttf", false},
        // {"NotoEmoji-Regular.ttf", true},
        {"SourceHanSansSC-Regular.ttf", false},
        {"SourceHanSansSC-Bold.ttf",    false},
    };

    for (const FontFace &face: font_faces)
        Rml::LoadFontFace(directory + face.filename, face.fallback_face);
}

