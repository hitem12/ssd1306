        #include "i2c_driver.hpp"
        
        void i2c_driver_implemetation::send(const std::span<uint8_t> data) { 
            int file;
            file = open(_filename.data(), O_RDWR);
            if(file < 0) {
                std::cerr << "Access to device error " << std::to_string(file) << std::endl; 
                return;   
            }
            if (ioctl(file, I2C_SLAVE, _addr) < 0) {
                std::cerr << "Setup error" << std::endl;
                /* ERROR HANDLING; you can check errno to see what went wrong */
                return;
            }
            // std::cout << "send " << data.size() << " bytes" << std::endl;
            auto res = write(file, std::data(data), data.size());
            if(res != data.size()) {
                std::cerr << "Write data error " << std::to_string(res) << std::endl; 
                return;
            }
            if(close(file) < 0) {
                std::cout << "close device error" << std::endl;
            } 
       }