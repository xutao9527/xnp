//
// Created by Administrator on 2024/10/4.
//

#include "DbgRenderer.h"

void DbgRenderer::Init()
{
    key_down_callback = &Shell::ProcessKeyDownShortcuts;
    context = Rml::CreateContext(window_title, Rml::Vector2i(window_width, window_height),&render_interface);
    Rml::ElementDocument *document = context->LoadDocument("resources/layout/login.rml");
    Rml::Debugger::Initialise(context);
    document->Show();
}

