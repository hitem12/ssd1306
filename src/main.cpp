#include "i2c_driver.hpp"
#include "ssd1306_driver.hpp"
#include <thread>
#include <chrono>
#include "file_to_ssd.hpp"
int main () {
    using namespace std::chrono_literals;
    auto bus_driver = std::make_shared<i2c_driver_implemetation>();
    auto oled =  SSD1306(bus_driver);
    oled.init_128x64();

    std::cout << "Fill display with 0x55" << std::endl;
    oled.fill_display(0x55);
    std::this_thread::sleep_for(2000ms);

    std::cout << "Show test.bmp" << std::endl;
    std::vector<uint8_t> drawBuffer;
    auto result = file_to_ssd::DrawFromBMPFile("test.bmp", drawBuffer);
    if(result != 0) {
        std::cout << "Error loading bitmap file" << std::endl;
        return -1;
    }
    oled.DrawINHorizontalMode(drawBuffer);

    return 0;
}