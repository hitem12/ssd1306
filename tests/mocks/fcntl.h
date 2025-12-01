#include <gmock.dir/gmock.h>
MOCK_METHOD(int, write, (int file, const void *buf, size_t count), (override));
MOCK_METHOD(int, close, (int file), (override));
MOCK_METHOD(int, open,(int file), (override)));
