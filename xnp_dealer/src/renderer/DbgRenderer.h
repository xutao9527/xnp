#pragma once

#include "XnpWin32VKContext.h"
#include <iostream>
#include <QDebug>

class DbgRenderer : public XnpWin32VKContext
{

public:
    void Init() override;
    DbgRenderer(HWND win_hwnd, std::string title, int width, int height)
            : XnpWin32VKContext(win_hwnd, std::move(title), width, height)
    {
        qDebug() << "DbgRenderer";
    }

    ~DbgRenderer() override {
        qDebug() << "~DbgRenderer";
    }
};
