#pragma once

#include <string_view> 
extern "C" {
    #include <sys/ioctl.h>
    #include <linux/i2c-dev.h>
    #include <i2c/smbus.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <cstdint>
}
#include <iostream>
#include <vector>
#include <iterator>

class bus_driver {
    public:
        virtual void send(const std::vector<uint8_t> data) = 0;
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
        void send(const std::vector<uint8_t> data) { 
            int file;
            device_open(file);
            if(!i2c_smbus_write_i2c_block_data(file, _addr, (uint8_t)data.size(), std::data(data))) {
                std::cerr << "Write data error" << std::endl; 
                return;
            }
            device_close(file);
        }

    private:
        int device_open(int &file) {
            file = open(_filename.data(), I2C_RDWR);
            if(file < 0) {
                std::cerr << "Write data error" << std::endl; 
                return -10;   //error
            }
            if (ioctl(file, I2C_SLAVE, _addr) < 0) {
                std::cerr << "Setup error" << std::endl;
                /* ERROR HANDLING; you can check errno to see what went wrong */
                return -11;
            }
            return file;
        }
        void device_close(int &file) {
            close(file);
        }
};