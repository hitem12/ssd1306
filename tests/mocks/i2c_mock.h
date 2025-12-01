#include <gmock.dir/gmock.h>

MOCK_METHOD(int, ioctl, (int file, unsigned long request, ...), (override));
