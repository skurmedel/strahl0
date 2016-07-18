#ifndef DIELECTRIC_HPP
#define DIELECTRIC_HPP

#include "vec3.hpp"
#include "ray.hpp"
#include "material.hpp"

class dielectric: public material
{
public:
    dielectric(ray::t_type rindex): ref_index(rindex) {}

    virtual bool scatter(
        ray const &r_in, 
        hit_record const &rec, 
        color &out_atten, 
        ray &out_scattered) const
    {
        vec3 outward_normal;
        vec3 reflected = reflect(r_in.direction, rec.N);

        // Refractive index for the material at the side the ray light comes
        // from, divided by the refractive index of the other material/surface.
        // The empty space is usually given ref. index 1.0.
        ray::t_type ni_over_nt;
        out_atten = vec3(1.0, 1.0, 1.0);
        vec3 refracted;

        /*
         Cosine angle between incoming ray normal and the surface boundary
         normal. Note that, since our ray points TOWARDS the surface, and many
         formulas and laws has this vector pointing away from the surface, 
         towards the incoming light. So in our case, r_in.direction is usually
         180 degrees off to what the formulas expect.
        */ 
        float cosine;
        float reflect_prob;

        if (dot(r_in.direction, rec.N) > 0)
        {
            // We are coming from inside our object, so we flip
            // the normal since it will most likely point towards 
            // the outside of the surface.
            outward_normal = -rec.N;
            // Since we assume the other "material", the empty space
            // has refractive index 1.0, we get just ref_index / 1.0.
            ni_over_nt = ref_index;
            
            // The code diverges somewhat from the book here, see why here:
            // http://psgraphics.blogspot.se/2016/03/my-buggy-implimentation-of-schlick.html

            // dot between to vectors: 
            //   |a| |b| * cos(t)      = dot(a,b)
            //   dot(a, b) / (|a| |b|) = cos(t)
            //   If a = N, and |a| = 1, this follows with b = r_in.direction:
            cosine = dot(r_in.direction, rec.N) / r_in.direction.len();
            /*
             As Peter Shirley says:
             "For dielectrics, a key point is that this theta is the bigger 
             of the two regardless of which direction the ray is traveling."

             We need to use the larger angle in Snell's law, which is going 
             to be on the side with the lowest refractive index.
            
             Since we are "inside" the material with a refractive index
             larger or equal to 1.0, we pick the angle for the side with
             refractive index one.  

             We call the smaller angle t2 and the larger t1.

             Thus we have n2 * sin(t2) = 1.0 * sin(t1)

             We have cos(t2), but we want sin(t2). So we do:

                n2 * sqrt(1 - cos^2(t2)) = sin(t1)
             
             Then we have:

                sin(t1) = n2 * sqrt(1 - cos^2(t2)) 
                cos(t1) = sqrt(1 - n2^2 (1 - cos^2(t2)))
            
             Here we have used the trigonometric one (as we call it in
             Sweden) twice (cos^2(t) + sin^2(t) = 1) and that 
             (ab)^2 = a^2 b^2.

             Since r_in points toward the surface.
            */ 
            cosine = sqrt(1.0f - ref_index * ref_index * (1.0f - cosine * cosine));
        } 
        else 
        {
            outward_normal = rec.N;
            ni_over_nt = 1.0 / ref_index;
            /*
             This cosine needs to be negated since our ray points towards the 
             surface. But the cosine between the normal and some vector  v 
             differs from the cosine between the normal and some vector -v
             only by sign. It is easy to see if you draw a normal straight up,
             from the origin, then draw a straight line through the origin.
            */
            cosine = -dot(r_in.direction, rec.N) / r_in.direction.len();            
        }
        if (refract(r_in.direction, outward_normal, ni_over_nt, refracted))
        {
            reflect_prob = schlick(cosine, ref_index);
        }
        else
        {
            reflect_prob = 1.0f;
        }

        out_scattered = (random1d() < reflect_prob) ?
              ray(rec.P, reflected)
            : ray(rec.P, refracted);
        return true;
    }

    ray::t_type ref_index;
};

#endif