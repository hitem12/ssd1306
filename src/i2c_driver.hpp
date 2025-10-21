#pragma once

#include <ioctl>
#include <string_view> 
#include <linux/i2c-dev.h>

class i2c_driver {
    uint8_t _addr = 0x3c;
    constexpr std::string_view _filename = "/dev/i2c-1";

    public:
        void send(const uint8_t * data, size_t len) {
            // snprintf(_filename.data(), _filename.size(), "/dev/i2c-%d", adapter_nr);
            int file_status = open(_filename.data(), O_RDWR);
            if(file_status < 0) {
                return;   //error
            }
            if (ioctl(file, I2C_SLAVE, _addr) < 0) {
                /* ERROR HANDLING; you can check errno to see what went wrong */
                return;
            }
        }
    private:
}