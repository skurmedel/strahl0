# strahl0
A raytracer designed after Peter Shirleys Raytracer in a Weekend.

## Notes
This raytracer differs from the one in the book in some ways. I use C++11 in 
this code.

 - hitable_list uses a std::vector.
 - the quadratic equation for the sphere is a bit different, mostly 
   in aesthetics.
 - The dielectric material has fixes and A LOT of comments to describe the code
   and why it looks like it does.
 - My vec3 is templated.
 - There's a plane class (there is one in the second book I think).
 - stochastics.hpp substitutes drand48 in the book for a C++11 std object.
 - I use a more accurate gamma_correct for SRGB, but I really should use the 
   one that avoids problem close to 0.