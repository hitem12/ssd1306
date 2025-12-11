#pragma once 
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <bitset>
#include <optional>
/* windows style struct*/
#pragma pack(push, 1)
typedef struct tagBITMAPFILEHEADER {
  uint16_t   bfType;
  uint32_t  bfSize;
  uint16_t   bfReserved1;
  uint16_t   bfReserved2;
  uint32_t  bfOffBits;
} BITMAPFILEHEADER, *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
  unsigned long biSize;
  long   biWidth;
  long   biHeight;
   unsigned short  biPlanes;
   unsigned short  biBitCount;
  unsigned long biCompression;
  unsigned long biSizeImage;
  long   biXPelsPerMeter;
  long   biYPelsPerMeter;
  unsigned long biClrUsed;
  unsigned long biClrImportant;
} BITMAPINFOHEADER, *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;
#pragma pack(pop)

struct imageData { 
    size_t size;
    size_t width;
    size_t height;
    std::vector<uint8_t> pixels;
};  
class file_to_ssd {
public:
    file_to_ssd() = default;
    ~file_to_ssd() = default;
    file_to_ssd(const file_to_ssd&) = delete;
    file_to_ssd& operator=(const file_to_ssd&) = delete;

    static int DrawFromBMPFile(const std::filesystem::path& filename, std::vector<uint8_t>& out)  {
            out.clear();
            imageData imgData;
            auto result = read_bitmap(filename, imgData);
            if(result != 0) {
                std::cout << "Error reading bitmap file" << std::endl;
                return -1;
            }
            if(imgData.width == 0 || imgData.height == 0) {

                std::cout << "Error reading bitmap file" << std::endl;
                return -1;
            }
            convert_to_ssd_format(imgData, out);
            return 0;
        }
    static int convert_to_ssd_format(const imageData& imgData, std::vector<uint8_t>& out) {
        //allocate output buffer
        out.resize(imgData.size+1);
        out[0] = 0x40; //(uint8_t)ControlByte::DATA;
        for(size_t w = 0; w < imgData.width; w++) {
            for (size_t h = 0; h < imgData.height; h+=8) {
                uint8_t byte {0x00};
                for (size_t bit = 0; bit < 8; bit++) {
                    size_t pixel_index = w/8 + (h  + bit ) * imgData.size/8/8;
                    if(pixel_index < imgData.pixels.size()) {
                        std::bitset<8> pixel = imgData.pixels[pixel_index];
                        if(pixel[7-w%8] ==  false) { 
                            byte |= (1 << (bit));
                        }
                    }
                    else {
                        h-=8;
                        bit--;
                        if(h >= imgData.height) {
                            break;
                        }
                    }
                }
                size_t out_index = w + (h/8) * 128;
                out[out_index + 1] = byte;
            }
        }
        return 0;
    }
    static int read_bitmap(const std::filesystem::path& filename, imageData& imgData) {
        std::ifstream file(filename, std::ios::binary);
        if(!file) {
            std::cout << "failed to open bitmap" << std::endl;
            return  -1;
        }
        BITMAPFILEHEADER bmpHeader; // Header
        BITMAPINFOHEADER bmpInfo; // Info 

        file.read(reinterpret_cast<char*>(&bmpHeader), sizeof(bmpHeader));
        file.read(reinterpret_cast<char*>(&bmpInfo), sizeof(bmpInfo));

        if(bmpHeader.bfType != 0x4D42)
        {
            std::cout << "File \"" << filename << "\" isn't a bitmap file\n";
            return  -1;
        }
        if(bmpInfo.biBitCount != 1) {
            std::cout << "Bit per pixel diffrent then 1 is not supported" << std::to_string(bmpInfo.biBitCount) << std::endl;
            return  -1;
        }
        // imageData imgData;
        // First allocate pixel memory
        // std::vector<uint8_t> pixels(bmpInfo.biSizeImage);
        imgData.size = static_cast<size_t>( bmpInfo.biSizeImage);
        imgData.width = static_cast<size_t>(bmpInfo.biWidth);
        imgData.height = static_cast<size_t>(bmpInfo.biHeight);
        imgData.pixels.resize(bmpInfo.biSizeImage);
        file.seekg(bmpHeader.bfOffBits, file.beg);
        // Go to where image data starts, then read in image data
        file.read(reinterpret_cast<char*>(imgData.pixels.data()), bmpInfo.biSizeImage);
        file.close();
        return 0;
    }
};