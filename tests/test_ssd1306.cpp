#include <gtest/gtest.h>
#include "file_to_ssd.hpp"


TEST(from_image, 8x8_bitmap) {
    imageData imgData {
        8*8,
        8, // width
        8, // height
        {
            0b11111111,
            0b10000001,
            0b10100101,
            0b10000001,
            0b10100101,
            0b10011001,
            0b10000001,
            0b11111111
        } // pixels
    };
    std::vector<uint8_t> bitmap;
    file_to_ssd::convert_to_ssd_format(imgData, bitmap);

    const uint8_t expected[9] = {
        0b01000000,
        0b11111111,
        0b10000001,
        0b10101001,
        0b10000101,
        0b10000101,
        0b10101001,
        0b10000001,
        0b11111111
    };
   
    for (size_t i = 0; i < bitmap.size(); ++i) {
        EXPECT_EQ(static_cast<std::bitset<8>>(bitmap[i]), static_cast<std::bitset<8>>(expected[i]));
    }
}
TEST(from_image, 4x4_bitmap) {
    /*
    1110 
    1111
    1000 
    0001   
     |
    \ /
    1110 0000
    1100 0000
    1100 0000
    0101 0000
    */
    imageData imgData {
        8*8,
        8, // width
        8, // height
        {
            0b11101111,
            0b10000001,


        } // pixels
    };
    std::vector<uint8_t> bitmap;
    file_to_ssd::convert_to_ssd_format(imgData, bitmap);

    const uint8_t expected[5] = {
        0b01000000,
        0b11100000,
        0b11000000,
        0b11000000,
        0b01010000
    };
   EXPECT_EQ(bitmap.size(), sizeof(expected));
    for (size_t i = 0; i < bitmap.size(); ++i) {
        EXPECT_EQ(static_cast<std::bitset<8>>(bitmap[i]), static_cast<std::bitset<8>>(expected[i]));
    }
}
TEST(from_image, 16x8_bitmap) {
    imageData imgData {
        8*8,
        16, // width
        8, // height
        {
            0b11111111,
            0b10000001,
            0b10100101,
            0b10000001,
            0b10100101,
            0b10011001,
            0b10000001,
            0b11111111,
            0b11111111,
            0b10000001,
            0b10100101,
            0b10000001,
            0b10100101,
            0b10011001,
            0b10000001,
            0b11111111
        } // pixels
    };
    std::vector<uint8_t> bitmap;
    file_to_ssd::convert_to_ssd_format(imgData, bitmap);

    const uint8_t expected[17] = {
        0b01000000,
        0b11111111,
        0b10000001,
        0b10101001,
        0b10000101,
        0b10000101,
        0b10101001,
        0b10000001,
        0b11111111,
        0b11111111,
        0b10000001,
        0b10101001,
        0b10000101,
        0b10000101,
        0b10101001,
        0b10000001,
        0b11111111
    };
   
    for (size_t i = 0; i < bitmap.size(); ++i) {
        EXPECT_EQ(static_cast<std::bitset<8>>(bitmap[i]), static_cast<std::bitset<8>>(expected[i]));
    }
}
TEST(from_image, 8x16_bitmap) {
    imageData imgData {
        8*8,
        8, // width
        16, // height
        {
            0b01111111,
            0b10000001,
            0b10100101,
            0b10000001,
            0b10100101,
            0b10011001,
            0b10000001,
            0b11111111,
            0b11111111,
            0b10000001,
            0b10100101,
            0b10000001,
            0b10100101,
            0b10011001,
            0b10000001,
            0b11111111
        } // pixels
    };
    std::vector<uint8_t> bitmap;
    file_to_ssd::convert_to_ssd_format(imgData, bitmap);

    const uint8_t expected[17] = {
        0b01000000,
        0b01111111,
        0b10000001,
        0b10101001,
        0b10000101,
        0b10000101,
        0b10101001,
        0b10000001,
        0b11111111,
        0b11111111,
        0b10000001,
        0b10101001,
        0b10000101,
        0b10000101,
        0b10101001,
        0b10000001,
        0b11111111
    };
   
    for (size_t i = 0; i < bitmap.size(); ++i) {
        EXPECT_EQ(static_cast<std::bitset<8>>(bitmap[i]), static_cast<std::bitset<8>>(expected[i]));
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}