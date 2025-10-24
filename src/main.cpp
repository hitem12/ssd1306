#include "i2c_driver.hpp"
#include "ssd1306_driver.hpp"
#include <thread>
#include <chrono>

int main () {
    using namespace std::chrono_literals;
    auto bus_driver = std::make_shared<i2c_driver_implemetation>();
    auto oled =  SSD1306(bus_driver);
    oled.init_128x64();
    for (auto i : {0,1,2,3,4,5,6,7,8,9}) {
        std::cout << "Invert! " << std::to_string(i) << std::endl;
        oled.invertDisplay(true);
        std::this_thread::sleep_for(2000ms);
        std::cout << "Normal! " << std::to_string(i) << std::endl;
        oled.invertDisplay(false);
        std::this_thread::sleep_for(2000ms);

    } 
    return 0;
}