#pragma once
#include <vector>
#include "i2c_driver.hpp"
#include  <memory>
/**
 * @brief SSD1306 OLED display driver (I2C version)
 * 
 * Provides low-level access to all SSD1306 commands and initialization routines.
 * Each command is documented with parameter details from the official datasheet.
 */
class SSD1306 {
public:

    enum class ControlByte: uint8_t  { 
        COMMAND = 0x00,
        DATA = 0x40
    };
    /// Default I2C address (commonly 0x3C or 0x3D)
    /**
     * @brief Enumeration of all SSD1306 command codes.
     * 
     * Each constant corresponds to a specific SSD1306 command.
     * Many commands require one or more parameter bytes to follow.
     */
    enum class Command : uint8_t {

        // ===== Power / Display control =====
        DISPLAY_OFF              = 0xAE, ///< Display OFF (sleep mode)
                                         ///< *No parameters.*

        DISPLAY_ON               = 0xAF, ///< Display ON (normal mode)
                                         ///< *No parameters.*

        ENTIRE_DISPLAY_RESUME    = 0xA4, ///< Resume display from RAM content
                                         ///< *No parameters.*

        ENTIRE_DISPLAY_ON        = 0xA5, ///< Force all pixels ON (regardless of RAM)
                                         ///< *No parameters.*

        NORMAL_DISPLAY           = 0xA6, ///< Set normal display (pixel=1 → lit)
                                         ///< *No parameters.*

        INVERT_DISPLAY           = 0xA7, ///< Invert display (pixel=0 → lit)
                                         ///< *No parameters.*

        // ===== Addressing & memory control =====
        MEMORY_MODE              = 0x20, ///< Set memory addressing mode
                                         ///< Parameters: 1 byte
                                         ///< - 0x00: Horizontal
                                         ///< - 0x01: Vertical
                                         ///< - 0x02: Page addressing

        SET_COLUMN_ADDR          = 0x21, ///< Set column address range
                                         ///< Parameters: 2 bytes
                                         ///< - start address (0–127)
                                         ///< - end address (0–127)

        SET_PAGE_ADDR            = 0x22, ///< Set page address range
                                         ///< Parameters: 2 bytes
                                         ///< - start page (0–7)
                                         ///< - end page (0–7)

        // ===== Hardware configuration =====
        SET_START_LINE           = 0x40, ///< Set display start line
                                         ///< Command = 0x40 | (0–63)
                                         ///< Defines the RAM row displayed at the top of the screen.

        SET_CONTRAST             = 0x81, ///< Set contrast control
                                         ///< Parameters: 1 byte (0x00–0xFF)
                                         ///< Higher = brighter.

        SET_MULTIPLEX            = 0xA8, ///< Set multiplex ratio
                                         ///< Parameters: 1 byte (0x0F–0x3F)
                                         ///< Defines number of active display lines minus 1.

        SET_DISPLAY_OFFSET       = 0xD3, ///< Set vertical display offset
                                         ///< Parameters: 1 byte (0x00–0x3F)
                                         ///< Shifts display vertically.

        SET_COM_PINS             = 0xDA, ///< Set COM pins hardware configuration
                                         ///< Parameters: 1 byte
                                         ///< - 0x02: for 128x32
                                         ///< - 0x12: for 128x64

        SET_DISPLAY_CLOCK_DIV    = 0xD5, ///< Set display clock divide ratio and oscillator frequency
                                         ///< Parameters: 1 byte
                                         ///< - [3:0]: divide ratio (1–16)
                                         ///< - [7:4]: oscillator frequency

        SET_PRECHARGE            = 0xD9, ///< Set pre-charge period
                                         ///< Parameters: 1 byte
                                         ///< - [3:0]: phase 1 period (1–15)
                                         ///< - [7:4]: phase 2 period (1–15)

        SET_VCOMH_DESELECT       = 0xDB, ///< Set VCOMH deselect level
                                         ///< Parameters: 1 byte
                                         ///< - 0x00: ~0.65×VCC
                                         ///< - 0x20: ~0.77×VCC
                                         ///< - 0x30: ~0.83×VCC

        CHARGE_PUMP              = 0x8D, ///< Enable/disable internal charge pump
                                         ///< Parameters: 1 byte
                                         ///< - 0x10: disable
                                         ///< - 0x14: enable (for internal voltage regulator)

        // ===== Segment remap & scan direction =====
        SEG_REMAP_NORMAL         = 0xA0, ///< Map column 0 to SEG0
                                         ///< *No parameters.*

        SEG_REMAP_REVERSE        = 0xA1, ///< Map column 127 to SEG0 (mirror horizontally)
                                         ///< *No parameters.*

        COM_SCAN_NORMAL          = 0xC0, ///< Scan COM from 0→63 (normal)
                                         ///< *No parameters.*

        COM_SCAN_REMAP           = 0xC8, ///< Scan COM from 63→0 (vertical flip)
                                         ///< *No parameters.*

        // ===== Scrolling control =====
        RIGHT_SCROLL             = 0x26, ///< Continuous horizontal scroll (right)
                                         ///< Parameters: 6 bytes
                                         ///< [0] dummy (0x00)
                                         ///< [1] start page (0–7)
                                         ///< [2] time interval (0–7, frame delay)
                                         ///< [3] end page (0–7)
                                         ///< [4] dummy (0x00)
                                         ///< [5] dummy (0xFF)

        LEFT_SCROLL              = 0x27, ///< Continuous horizontal scroll (left)
                                         ///< Same format as RIGHT_SCROLL.

        VERT_RIGHT_SCROLL        = 0x29, ///< Vertical + right scroll
                                         ///< Parameters: 5 bytes
                                         ///< [0] dummy (0x00)
                                         ///< [1] start page (0–7)
                                         ///< [2] time interval (0–7)
                                         ///< [3] end page (0–7)
                                         ///< [4] vertical offset (0–63)

        VERT_LEFT_SCROLL         = 0x2A, ///< Vertical + left scroll
                                         ///< Same format as VERT_RIGHT_SCROLL.

        SET_SCROLL_AREA          = 0xA3, ///< Set vertical scroll area
                                         ///< Parameters: 2 bytes
                                         ///< [0] top fixed area (0–63)
                                         ///< [1] scroll area (1–64)

        ACTIVATE_SCROLL          = 0x2F, ///< Start any defined scroll
                                         ///< *No parameters.*

        DEACTIVATE_SCROLL        = 0x2E, ///< Stop active scroll
                                         ///< *No parameters.*

        // ===== Miscellaneous =====
        NOP                      = 0xE3  ///< No operation
                                         ///< *No parameters.*
    };

private:
    std::shared_ptr<bus_driver> _bus; 
public:
    SSD1306() = delete;
    SSD1306(std::shared_ptr<bus_driver> bus) {
        _bus = bus;
    }
    /**
     * @brief Send display RAM data (pixel buffer).
     * Control byte = 0x40.
     */
    void sendData(const std::vector<uint8_t> &data) const {
        std::vector<uint8_t> instruction = {(uint8_t)ControlByte::DATA};
        instruction.insert(instruction.end(), data.begin(), data.end());
        _bus->send(instruction);
    }

    /**
     * @brief Standard initialization sequence for 128x64 OLEDs.
     */
    void init_128x64() const {
        std::vector<uint8_t> init_seq = {
            (uint8_t)ControlByte::COMMAND,
            (uint8_t)Command::DISPLAY_OFF,
            (uint8_t)Command::SET_DISPLAY_CLOCK_DIV, 0x80,
            (uint8_t)Command::SET_MULTIPLEX, 0x3F,
            (uint8_t)Command::SET_DISPLAY_OFFSET, 0x00,
            (uint8_t)Command::SET_START_LINE, 0x00,
            (uint8_t)Command::CHARGE_PUMP, 0x14,
            (uint8_t)Command::MEMORY_MODE, 0x00,
            (uint8_t)Command::SEG_REMAP_REVERSE,
            (uint8_t)Command::COM_SCAN_REMAP,
            (uint8_t)Command::SET_COM_PINS, 0x12,
            (uint8_t)Command::SET_CONTRAST, 0x7F,
            (uint8_t)Command::SET_PRECHARGE, 0xF1,
            (uint8_t)Command::SET_VCOMH_DESELECT, 0x40,
            (uint8_t)Command::ENTIRE_DISPLAY_RESUME,
            (uint8_t)Command::NORMAL_DISPLAY,
            (uint8_t)Command::DEACTIVATE_SCROLL,
            (uint8_t)Command::DISPLAY_ON
        };
        _bus->send(init_seq);
    }

    /**
     * @brief Set display contrast.
     * @param level Value between 0x00 (dim) and 0xFF (bright).
     */
    void setContrast(uint8_t level) const {
        _bus->send(std::vector<uint8_t>{(uint8_t)ControlByte::COMMAND,(uint8_t)Command::SET_CONTRAST, level});
    }

    /**
     * @brief Turn display ON or OFF.
     */
    void setDisplayOn(bool on) const {
        if(on) {
            _bus->send(std::vector<uint8_t>{(uint8_t)ControlByte::COMMAND, (uint8_t)Command::DISPLAY_ON});
        } else {
            _bus->send(std::vector<uint8_t>{(uint8_t)ControlByte::COMMAND, (uint8_t)Command::DISPLAY_OFF});
        }
    }

    /**
     * @brief Enable or disable inverted display mode.
     */
    void invertDisplay(bool invert) const {
        if(invert) {
            _bus->send(std::vector<uint8_t>{(uint8_t)ControlByte::COMMAND, (uint8_t)Command::INVERT_DISPLAY});
        } else {
            _bus->send(std::vector<uint8_t>{(uint8_t)ControlByte::COMMAND, (uint8_t)Command::NORMAL_DISPLAY});
        }

    }
};
