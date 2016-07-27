# strahl0
A raytracer designed after Peter Shirleys Raytracer in a Weekend.

## Notes Book 1
This raytracer differs from the one in the book in some ways. I use C++11 in 
the makefile, and in the stochastics.hpp header, really everything else is 
just plain boring C++98.

 - The dielectric material has fixes and A LOT of comments to describe the code
   and why it looks like it does.
 - the quadratic equation for the sphere is a bit different, mostly 
   in aesthetics: x^2 + px + q is the form I prefer.
 - My vec3 is templated.
 - Metal has a refractive index-field, for cheapo grazing angle color effects.
 - There's a plane class (there is one in the second book I think).
 - stochastics.hpp substitutes drand48 in the book for a C++11 std object.
 - hitable_list uses a std::vector.
 - I use a more accurate gamma_correct for SRGB, but I really should use the 
   one that avoids problem close to 0.
 - Some fields and variables have different names here and there.

Note that the DOF chapter (the last) is not implemented.

## Notes Book 2
  - No motion blur support although the interfaces are there.
  - `color` is used in most places where a color is expected as a result,
    `color` is just typedefed to `vec3` though.
  - `xy_rect` etc. all substituted with a single templated `aa_rectangle` class,
    for example: `aa_rectangle<X_AXIS, Y_AXIS>`. This uses C++11 features to 
    work out the complement set of `{axis1, axis2}`.
  - Image textures are implemented, but not noise textures.
  - bvh_node class is a bit different, builds a bvh from a hitable list in a 
    static function instead.
  - Volumes and instances not implemented
