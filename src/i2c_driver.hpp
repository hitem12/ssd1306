#pragma once

#include <string_view> 
extern "C" {
    // #if defined(__linux__)
        #include <sys/ioctl.h>
        #include <linux/i2c-dev.h>
        #include <i2c/smbus.h>
        #include <fcntl.h>
        #include <unistd.h>
        #include <cstdint>
    // #elif _WIN32
    // #endif
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
        void send(const std::span<uint8_t> data) { 
            int file;
            file = open(_filename.data(), O_RDWR);
            if(file < 0) {
                std::cerr << "Access to device error " << std::to_string(file) << std::endl; 
                return;   //error
            }
            if (ioctl(file, I2C_SLAVE, _addr) < 0) {
                std::cerr << "Setup error" << std::endl;
                /* ERROR HANDLING; you can check errno to see what went wrong */
                return;
            }
            std::cout << "send data:";
            for(auto d : data) {
                std::cout << std::hex << "0x" << std::setfill('0') << std::setw(2)  << (int)d << " ";
            }
            std::cout << std::endl;
            auto res = write(file, std::data(data), data.size());
            if(res != data.size()) {
                std::cerr << "Write data error " << std::to_string(res) << std::endl; 
                return;
            }
            if(close(file) < 0) {
                std::cout << "close device error" << std::endl;
            } 
       }

};