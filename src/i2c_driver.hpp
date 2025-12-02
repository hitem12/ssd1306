#pragma once

#include <string_view> 
extern "C" {
        #include <sys/ioctl.h>
        #include <linux/i2c-dev.h>
        #include <fcntl.h>
}
#include <iostream>
#include <vector>
#include <iterator>

#include <thread>
#include <chrono>
#include <iomanip>
#include <span>

class bus_driver {
    public:
        virtual void send(const std::span<uint8_t> data) = 0;
        virtual ~bus_driver() {};
    protected:
        bus_driver() = default;
    private: 
        bus_driver(const bus_driver&) = delete;
        bus_driver& operator= (const bus_driver&) = delete;
        
};
class i2c_driver_implemetation : public bus_driver {
    uint8_t _addr;
    std::string _filename; 

    public:
        i2c_driver_implemetation(const i2c_driver_implemetation&) = delete;
        i2c_driver_implemetation& operator=(const i2c_driver_implemetation&) = delete;
        i2c_driver_implemetation(std::string_view device_patch = "/dev/i2c-1", uint8_t address = 0x3c) {
            _filename = device_patch;
            _addr = address;
        }
        void send(const std::span<uint8_t> data);
};