#ifndef IMAGE_TEXTURE_HPP
#define IMAGE_TEXTURE_HPP

#include "texture.hpp"
#include <memory>

class image
{
public:
    typedef std::unique_ptr<image> unique_ptr;

    static unique_ptr load(char const *fname);

    unsigned char const *pixels() const { return rgbdata.get(); }

    unsigned int width() const { return w; }
    unsigned int height() const { return h; } 

    image(image &i): rgbdata(), w(i.w), h(i.h) { rgbdata.swap(i.rgbdata); }

private:
    image(std::unique_ptr<unsigned char> &data, unsigned int w, unsigned int h):
        rgbdata(), w(w), h(h) { rgbdata.swap(data); }

    std::unique_ptr<unsigned char> rgbdata;
    unsigned int w;
    unsigned int h;
};

class image_texture: public texture 
{
public:
    image_texture(): img() {}
    image_texture(image::unique_ptr i): img() { img.swap(i); }

    virtual color value(float u, float v, vec3 const &P) const
    {
        if (img.get() == nullptr)
            return color(0,0,0);
        unsigned char const *pixels = img->pixels();
        std::size_t x = std::size_t(u * (img->width()  - 1)) % img->width();
        std::size_t y = std::size_t(v * (img->height() - 1)) % img->height();
        
        return color(
            pixels[3 * img->width() * y + 3 * x + 0] / 255.0,
            pixels[3 * img->width() * y + 3 * x + 1] / 255.0,
            pixels[3 * img->width() * y + 3 * x + 2] / 255.0
        );
    }


private:
    image::unique_ptr img;
};


#endif