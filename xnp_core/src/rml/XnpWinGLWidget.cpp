#include "XnpWinGLWidget.h"

std::once_flag XnpWinGLWidget::init_flag;
std::atomic<bool> XnpWinGLWidget::update_flag(false);
std::atomic<bool> XnpWinGLWidget::force_update_flag(false);
std::atomic<int> XnpWinGLWidget::ref_count(0);
