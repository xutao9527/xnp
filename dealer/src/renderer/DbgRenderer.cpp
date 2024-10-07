//
// Created by Administrator on 2024/10/4.
//

#include "DbgRenderer.h"

void DbgRenderer::Init()
{
    key_down_callback = &Shell::ProcessKeyDownShortcuts;
    context = Rml::CreateContext(Shell::ConvertToString(window_title), Rml::Vector2i(window_width, window_height),&render_interface);
    Rml::ElementDocument *document = context->LoadDocument("assets/demo.rml");
    document->Show();
    Rml::Debugger::Initialise(context);
}

