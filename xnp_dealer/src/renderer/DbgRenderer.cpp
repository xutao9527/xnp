#include "DbgRenderer.h"

void DbgRenderer::Init()
{
    context = Rml::CreateContext(window_title, Rml::Vector2i(window_width, window_height),&render_interface);
    Rml::ElementDocument *document = context->LoadDocument("resources/layout/login.rml");
    Rml::Debugger::Initialise(context);
    document->Show();
}

