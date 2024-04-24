//
// Created by Martin Lejko on 31/01/2024.
//

#include <iostream>
#include <cstring>
#include <random>
#include "tgaimage.h"
#include "logging.h"

TGAImage::TGAImage(const int w, const int h, const int bpp) : w(w), h(h), bpp(bpp), data(w*h*bpp, 0) {}

bool TGAImage::read_tga_file(const std::string& filename) {
    std::ifstream in;
    in.open(filename, std::ios::binary);
    if (!in.is_open()) {
        logger->error("Failed to open file {}", filename);
        return false;
    }
    TGAHeader header;
    in.read(reinterpret_cast<char *>(&header), sizeof(header));
    if (!in.good()) {
        logger->error("An error occured while reading the header");
        return false;
    }
    w   = header.width;
    h   = header.height;
    bpp = header.bitsperpixel>>3;
    if (w<=0 || h<=0 || (bpp!=GRAYSCALE && bpp!=RGB && bpp!=RGBA)) {
        logger->error("Bad bpp (or width/height) value");
        return false;
    }
    size_t nbytes = bpp*w*h;
    data = std::vector<std::uint8_t>(nbytes, 0);
    if (3==header.datatypecode || 2==header.datatypecode) {
        in.read(reinterpret_cast<char *>(data.data()), nbytes);
        if (!in.good()) {
            logger->error("An error occured while reading the data");
            return false;
        }
    } else if (10==header.datatypecode||11==header.datatypecode) {
        if (!load_rle_data(in)) {
            logger->error("An error occured while reading the data");
            return false;
        }
    } else {
        logger->error("unknown file format {}", (int)header.datatypecode);
        return false;
    }
    if (!(header.imagedescriptor & 0x20))
        flip_vertically();
    if (header.imagedescriptor & 0x10)
        flip_horizontally();
    logger->debug("Image with size {}x{} loaded successfully.", w, h);
    return true;
}

bool TGAImage::load_rle_data(std::ifstream &in) {
    size_t pixelcount = w*h;
    size_t currentpixel = 0;
    size_t currentbyte  = 0;
    TGAColor colorbuffer;
    do {
        std::uint8_t chunkheader = 0;
        chunkheader = in.get();
        if (!in.good()) {
            logger->error("An error occured while reading the header");
            return false;
        }
        if (chunkheader<128) {
            chunkheader++;
            for (int i=0; i<chunkheader; i++) {
                in.read(reinterpret_cast<char *>(colorbuffer.bgra), bpp);
                if (!in.good()) {
                    logger->error("An error occured while reading the header");
                    return false;
                }
                for (int t=0; t<bpp; t++)
                    data[currentbyte++] = colorbuffer.bgra[t];
                currentpixel++;
                if (currentpixel>pixelcount) {
                    logger->error("Too many pixels read");
                    return false;
                }
            }
        } else {
            chunkheader -= 127;
            in.read(reinterpret_cast<char *>(colorbuffer.bgra), bpp);
            if (!in.good()) {
                logger->error("An error occured while reading the header");
                return false;
            }
            for (int i=0; i<chunkheader; i++) {
                for (int t=0; t<bpp; t++)
                    data[currentbyte++] = colorbuffer.bgra[t];
                currentpixel++;
                if (currentpixel>pixelcount) {
                    logger->error("Too many pixels read");
                    return false;
                }
            }
        }
    } while (currentpixel < pixelcount);
    return true;
}

bool TGAImage::write_tga_file(const std::string& filename, const bool vflip, const bool rle) const {
    constexpr std::uint8_t developer_area_ref[4] = {0, 0, 0, 0};
    constexpr std::uint8_t extension_area_ref[4] = {0, 0, 0, 0};
    constexpr std::uint8_t footer[18] = {'T','R','U','E','V','I','S','I','O','N','-','X','F','I','L','E','.','\0'};
    std::ofstream out;
    out.open(filename, std::ios::binary);
    if (!out.is_open()) {
        logger->error("Failed to open file {}", filename);
        return false;
    }
    TGAHeader header = {};
    header.bitsperpixel = bpp<<3;
    header.width  = w;
    header.height = h;
    header.datatypecode = (bpp==GRAYSCALE?(rle?11:3):(rle?10:2));
    header.imagedescriptor = vflip ? 0x00 : 0x20; // top-left or bottom-left origin
    out.write(reinterpret_cast<const char *>(&header), sizeof(header));
    if (!out.good()) {
        logger->error("An error occured while writing the header");
        return false;
    }
    if (!rle) {
        out.write(reinterpret_cast<const char *>(data.data()), w*h*bpp);
        if (!out.good()) {
            logger->error("An error occured while writing the data");
            return false;
        }
    } else if (!unload_rle_data(out)) {
        logger->error("An error occured while writing the data");
        return false;
    }
    out.write(reinterpret_cast<const char *>(developer_area_ref), sizeof(developer_area_ref));
    if (!out.good()) {
        logger->error("Cannot dump the tga file");
        return false;
    }
    out.write(reinterpret_cast<const char *>(extension_area_ref), sizeof(extension_area_ref));
    if (!out.good()) {
        logger->error("Cannot dump the tga file");
        return false;
    }
    out.write(reinterpret_cast<const char *>(footer), sizeof(footer));
    if (!out.good()) {
        logger->error("Cannot dump the tga file");
        return false;
    }
    return true;
}

bool TGAImage::unload_rle_data(std::ofstream &out) const {
    const std::uint8_t max_chunk_length = 128;
    size_t npixels = w*h;
    size_t curpix = 0;
    while (curpix<npixels) {
        size_t chunkstart = curpix*bpp;
        size_t curbyte = curpix*bpp;
        std::uint8_t run_length = 1;
        bool raw = true;
        while (curpix+run_length<npixels && run_length<max_chunk_length) {
            bool succ_eq = true;
            for (int t=0; succ_eq && t<bpp; t++)
                succ_eq = (data[curbyte+t]==data[curbyte+t+bpp]);
            curbyte += bpp;
            if (1==run_length)
                raw = !succ_eq;
            if (raw && succ_eq) {
                run_length--;
                break;
            }
            if (!raw && !succ_eq)
                break;
            run_length++;
        }
        curpix += run_length;
        out.put(raw?run_length-1:run_length+127);
        if (!out.good()) {
            logger->error("An error occured while writing the data");
            return false;
        }
        out.write(reinterpret_cast<const char *>(data.data()+chunkstart), (raw?run_length*bpp:bpp));
        if (!out.good()) {
            logger->error("An error occured while writing the data");
            return false;
        }
    }
    return true;
}

TGAColor TGAImage::get(const int x, const int y) const {
    if (data.empty() || x<0 || y<0 || x>=w || y>=h)
        return {};
    TGAColor ret = {0, 0, 0, 0, bpp};
    const std::uint8_t *p = data.data()+(x+y*w)*bpp;
    for (int i=bpp; i--; ret.bgra[i] = p[i]);
    return ret;
}

void TGAImage::set(int x, int y, const TGAColor &c) {
    if (data.empty() || x<0 || y<0 || x>=w || y>=h) return;
    memcpy(data.data()+(x+y*w)*bpp, c.bgra, bpp);
}

void TGAImage::flip_horizontally() {
    int half = w>>1;
    for (int i=0; i<half; i++)
        for (int j=0; j<h; j++)
            for (int b=0; b<bpp; b++)
                std::swap(data[(i+j*w)*bpp+b], data[(w-1-i+j*w)*bpp+b]);
}

void TGAImage::flip_vertically() {
    int half = h>>1;
    for (int i=0; i<w; i++)
        for (int j=0; j<half; j++)
            for (int b=0; b<bpp; b++)
                std::swap(data[(i+j*w)*bpp+b], data[(i+(h-1-j)*w)*bpp+b]);
}

int TGAImage::width() const {
    return w;
}

int TGAImage::height() const {
    return h;
}

TGAColor genereateColor() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<std::uint8_t> dist(0, 255);

    // Generate random values for each color channel
    std::uint8_t r = dist(gen);
    std::uint8_t g = dist(gen);
    std::uint8_t b = dist(gen);

    return {r, g, b, 255};
}

