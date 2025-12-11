#include <CLI/CLI.hpp>

#include "i2c_driver.hpp"
#include "ssd1306_driver.hpp"
#include <thread>
#include <chrono>
#include "file_to_ssd.hpp"
#include <filesystem>
#include <regex>


// struct TimeValidator : public CLI::TypeValidator<std::chrono::milliseconds> {
//     public:
//     TimeValidator() {
//         name_ = "time_duration";
//         // description_ = "Validates time duration strings (e.g., 500ms, 2s, 1m, 1h)";
//         func_ = [](const std::string &str) -> std::chrono::milliseconds {
//             static const std::regex r(R"((\d+)(ms|s|m|h)?)");
//             std::smatch match;
            
//             if (!std::regex_match(str, match, r)) { 
//                 throw CLI::ValidationError("Invalid time format: " + str);
//             }

//             int value = std::stoi(match[1]);
//             std::string unit = match[2];

//             if (unit == "ms" || unit == "")
//                 return std::chrono::milliseconds(value);
//             if (unit == "s")
//                 return std::chrono::seconds(value);
//             if (unit == "m")
//                 return std::chrono::minutes(value);
//             if (unit == "h")
//                 return std::chrono::hours(value);

//             throw CLI::ValidationError("Unknown time unit");
//         };
//     }
//     auto operator()(const std::string &str) {
//         return func_(str);
//     }
// };
// const static TimeValidator time_validator;

int main (int argc, char** argv) {
    CLI::App app;
    using namespace std::chrono_literals;
    std::string i2c_path = "/dev/i2c-1";
    uint8_t i2c_address = 0x3c;
    app.add_option("-p,--i2c-path", i2c_path, "I2C device path (default: /dev/i2c-1)")->check(CLI::ExistingFile);
    app.add_option("-a,--i2c-addr", i2c_address, "I2C device address (default: 0x3c)")->check(CLI::Range(0x03, 0x77));

    CLI::App* test = app.add_subcommand("test", "Run in test mode");
    std::chrono::milliseconds program_duration = 20000ms;
    std::chrono::milliseconds delay_time = 1000ms;
    test->add_option("-d,--delay", delay_time, "Delay time between display updates in milliseconds");
    test->add_option("-t,--time", program_duration, "Total duration of the program in milliseconds");
   
    std::filesystem::path bmp_file = "test.bmp";

    CLI::App* bmp_display = app.add_subcommand("bmp_display", "Display BMP images");
    bmp_display->add_option("-f,--file", bmp_file, "BMP file to display")->check(CLI::ExistingFile);

    std::filesystem::path bmp_directory = ".";
    CLI::App* presentation = app.add_subcommand("presentation", "Run presentation mode (display all BMP files in current directory)");
    presentation->add_option("-d,--delay", delay_time, "Delay time between images in milliseconds");
    presentation->add_option("-D, --dictionary", bmp_directory, "Directory containing BMP files")->check(CLI::ExistingDirectory); 
    CLI11_PARSE(app, argc, argv);


    auto bus_driver = std::make_shared<i2c_driver_implemetation>(i2c_path, i2c_address);
    auto oled =  SSD1306(bus_driver);
    oled.init_128x64();
    if(test->parsed()) {
        auto start_time = std::chrono::steady_clock::now();
        while(start_time + program_duration > std::chrono::steady_clock::now()) {
            oled.fill_display_vertical(0xF0);
            std::this_thread::sleep_for(delay_time);
            oled.fill_display_vertical(0x0F);
            std::this_thread::sleep_for(delay_time);
            oled.fill_display_horizontal(0x88);
            std::this_thread::sleep_for(delay_time);
            oled.fill_display_horizontal(0x11);
            std::this_thread::sleep_for(delay_time);
        }
        return 0;
    }

    if(bmp_display->parsed()) {
        std::cout <<"Show " << bmp_file << std::endl;
        std::vector<uint8_t> drawBuffer;
        auto result = file_to_ssd::DrawFromBMPFile(bmp_file, drawBuffer);
        if(result != 0) {
            std::cout << "Error loading bitmap file" << std::endl;
            return -1;
        }
        oled.RemapDisplay(false);
        oled.DrawINHorizontalMode(drawBuffer);
        return 0;
    }
    if(presentation->parsed()) {
        oled.RemapDisplay(false);
        for(const auto &entry : std::filesystem::directory_iterator(bmp_directory)) {
            if(entry.path().extension() == ".bmp") {
                std::cout << "Drawing file: " << entry.path() << std::endl;
                std::vector<uint8_t> drawBuffer;
                auto result = file_to_ssd::DrawFromBMPFile(entry.path(), drawBuffer);
                if(result != 0) {
                    std::cout << "Error loading bitmap file" << std::endl;
                    continue;
                }
                oled.DrawINHorizontalMode(drawBuffer);
                std::this_thread::sleep_for(delay_time);
            }
        }
        return 0;
    }
    return 0;
}