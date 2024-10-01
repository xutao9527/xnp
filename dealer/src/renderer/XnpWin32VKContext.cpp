#include "XnpWin32VKContext.h"

std::once_flag XnpWin32VKContext::initFlag;
std::atomic<HWND> XnpWin32VKContext::hwnd{ nullptr };