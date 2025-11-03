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

    // for(uint8_t i= 0; i < 1; i++) {
    //     oled.test_screen(i);
    //     std::cout << "test "  << std::endl;
    //     std::this_thread::sleep_for(1000ms);
    // }
    std::cout << "full_display with: " << std::hex << 0xaF << std::endl;
    oled.fill_display(0x01);
    std::this_thread::sleep_for(2000ms);
    // std::cout << "full_display with: " << std::hex << 0x17 << std::endl;
    // oled.fill_display(0xaa);
    // std::this_thread::sleep_for(2000ms);
    // std::cout << "full_display with: " << std::hex << 0x55 << std::endl;
    // oled.fill_display(0x55);
    // std::this_thread::sleep_for(2000ms);
    std::vector<uint8_t> drawBuffer;
    auto result = file_to_ssd::DrawFromBMPFile("test.bmp", drawBuffer);
    if(result != 0) {
        std::cout << "Error loading bitmap file" << std::endl;
        return -1;
    }
    oled.DrawInVerticalMode(drawBuffer);
    return 0;
}