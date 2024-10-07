#pragma once

#include "XnpWin32VKContext.h"

class DbgRenderer : public XnpWin32VKContext
{

public:
    void Init() override;
    DbgRenderer(HWND win_hwnd, std::wstring title, int width, int height)
            : XnpWin32VKContext(win_hwnd, std::move(title), width, height)
    {}
};
