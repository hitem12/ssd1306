#pragma once 

class file_to_ssd {
public:
    file_to_ssd() = default;
    ~file_to_ssd() = default;
    file_to_ssd(const file_to_ssd&) = delete;
    file_to_ssd& operator=(const file_to_ssd&) = delete;

    static int DrawFromBMPFile(const std::filesystem::path& filename, std::vector<uint8_t>& out)  {
        out.clear();
        std::ifstream file(filename, std::ios::binary);
        if(!file) {
            std::cout << "failed to open bitmap" << std::endl;
            return -1;
        }
        	uint8_t* datBuff[2] = {nullptr, nullptr}; // Header buffers
            BITMAPFILEHEADER* bmpHeader = nullptr; // Header
            BITMAPINFOHEADER* bmpInfo   = nullptr; // Info 
            datBuff[0] = new uint8_t[sizeof(BITMAPFILEHEADER)];
            datBuff[1] = new uint8_t[sizeof(BITMAPINFOHEADER)];

            file.read((char*)datBuff[0], sizeof(BITMAPFILEHEADER));
            file.read((char*)datBuff[1], sizeof(BITMAPINFOHEADER));

            	// Construct the values from the buffers
            bmpHeader = (BITMAPFILEHEADER*) datBuff[0];
            bmpInfo   = (BITMAPINFOHEADER*) datBuff[1];
            
    
            if(bmpHeader->bfType != 0x4D42)
            {
                std::cout << "File \"" << filename << "\" isn't a bitmap file\n";
                return -1;
            }
            if(bmpInfo->biBitCount != 1) {
                std::cout << "Bit per pixel diffrent then 1 is not supported" << std::to_string(bmpInfo->biBitCount) << std::endl;
                return -1;
            }
            // First allocate pixel memory
            std::vector<uint8_t> pixels(bmpInfo->biSizeImage);
    
            file.seekg(bmpHeader->bfOffBits, file.beg);
            // Go to where image data starts, then read in image data
            
            file.read(reinterpret_cast<char*>(pixels.data()), bmpInfo->biSizeImage);
            file.close();
            //allocate output buffer
            out.resize(bmpInfo->biSizeImage+1);
            out[0] = (uint8_t)ControlByte::DATA;
            if(bmpInfo->biWidth == 128) {
                for(size_t w = 0; w < 128; w++) {
                    for (size_t h = 0; h < 64; h+=8) {
                        uint8_t byte {0x00};
                        for (size_t bit = 0; bit < 8; bit++) {
                            size_t pixel_index = w/8 + (h  + bit ) * 128/8;
                            if(pixel_index < pixels.size()) {
                                std::bitset<8> pixel = pixels[pixel_index];
                                std::cout << pixel << " " << 7-h/8%8 << std::endl;
                                if(pixel[7-h/8%8] ==  false) { // assuming black pixel
                                    byte |= (1 << (7-bit));
                                }
                            }
                            else {
                                std::cout << "Error pixel index out of range: " << pixel_index << " size: " << pixels.size() << " bit: " << bit << std::endl;
                            }
                        }
                        size_t out_index = w + (h/8) * 128;
                        std::cout << "komurka: " << std::dec << out_index << std::hex <<" byte: 0x" << std::setw(2) << static_cast<short>(byte) << std::dec << " w: " << w << " h:" << h <<  std::endl;
                        out[out_index + 1] = byte;
                    }
                }
            }
            std::cout << "Sending data to display" << std::endl;
            // _bus->send({(uint8_t)ControlByte::COMMAND, (uint8_t)Command::MEMORY_MODE, 0x01});
            // set_default_canvas();
            // _bus->send(out);
            // sendData(out);
            delete[] datBuff[0];
            delete[] datBuff[1];
            return 0;
        }
    private:

};