#include "OpenGLWidget.h"

std::once_flag OpenGLWidget::initFlag;
std::atomic<int> OpenGLWidget::ref_count(0);

