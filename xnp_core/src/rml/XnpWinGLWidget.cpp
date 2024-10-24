#include "XnpWinGLWidget.h"

std::once_flag XnpWinGLWidget::initFlag;
std::atomic<int> XnpWinGLWidget::ref_count(0);