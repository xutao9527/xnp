#include "XnpQWidget.h"

std::once_flag XnpQWidget::initFlag;
std::atomic<int> XnpQWidget::ref_count(0);