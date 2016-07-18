#include "image_texture.hpp"

#include "external.hpp"

image::unique_ptr image::load(char const *fname)
{
    int w, h, chans;
    std::unique_ptr<unsigned char> rgb(stbi_load(fname, &w, &h, &chans, 3));
    if (!rgb)
        return nullptr;
    unique_ptr img(new (std::nothrow) image(rgb, w, h));
    if (!img)
        return nullptr;
    return img;
}

