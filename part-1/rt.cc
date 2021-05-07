// Raxel Ortiz
// CPSC 120-05
// 2021-05-06
// raxelortiz7@csu.fullerton.edu
// @raxelortiz7
//
// Lab 12-01
//
// This program generates a bunch of smooth balls!
//

#include <chrono>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>

#include "image.h"
#include "ray.h"
#include "sphere.h"
#include "utility.h"
#include "vec3.h"

using namespace std;

/// RayColor computes the color the ray \p r sees through the camera lens.
/// Given a Ray \p r, calculate the color that is visible to the ray.
/// \param r A ray eminating from the camera through the scene
/// \param world The world - the scene that will be rendered
/// \returns The color visible from that ray
Color RayColor(const Ray& r, const std::vector<std::shared_ptr<Hittable>>& world) {
  HitRecord rec;
  Color c;
  HitRecord tmp_rec;
        bool hit_anything = false;
        double t_min = 0.0;
        double closest_so_far = kInfinity;
        for (const auto& object : world) {
          if (object->hit(r, t_min, closest_so_far, tmp_rec)) {
            hit_anything = true;
            closest_so_far = tmp_rec.t;
            rec = tmp_rec;
          }
        }
  // check to see if the ray intersects with the world
  /*if (world.hit(r, 0, kInfinity, rec)) {
    // This is the base color of the sphere. You may set this color
    // to whatever you like.
    Color sphere_color{1, 1, 0};
    const double kAmbientReflection = 0.3;
    const double kDiffuseReflection = 0.7;
    const double kSpecularReflection = 0.5;
    const double kSpecularShininess = 32.0;
    // If the ray intersects an object in the world (the sphere), then
    // set the color to be the
    // c = 0.5 * (rec.normal + sphere);
    Vec3 light{1, 1, 0.25};
    Vec3 to_light_vector = UnitVector(light - rec.p);
    Vec3 unit_normal = UnitVector(rec.normal);
    Vec3 to_viewer = UnitVector(-rec.p);
    Vec3 reflection = Reflect(to_light_vector, unit_normal);

    Color phong_ambient = kAmbientReflection * sphere_color;
    Color phong_diffuse =
        kDiffuseReflection * std::max(Dot(to_light_vector, unit_normal), 0.0) * sphere_color;
    Color phong_specular =
        kSpecularReflection *
        std::pow(std::max(Dot(reflection, to_viewer), 0.0), kSpecularShininess) * sphere_color;

    Color phong = phong_ambient + phong_diffuse + phong_specular;
    c = Clamp(phong, 0, 1);
  } else {
    // Color sky_top{.5, .4, 0};
    // Color sky_bottom{.2, .1, .5};
    Color sky_top{0.4980392156862745, 0.7450980392156863, 0.9215686274509803};
    Color sky_bottom{1, 1, 1};
    Vec3 unit_direction = UnitVector(r.direction());
    double t = 0.5 * (unit_direction.y() + 1.0);
    c = (1.0 - t) * sky_bottom + t * sky_top;
  }*/
  if (hit_anything) {
         c = rec.material->reflect_color(r, rec);
  } else {
        Color sky_top{0.4980392156862745, 0.7450980392156863, 0.9215686274509803};
        Color sky_bottom{1, 1, 1};
        Vec3 unit_direction = UnitVector(r.direction());
        double t = 0.5 * (unit_direction.y() + 1.0);
       c = (1.0 - t) * sky_bottom + t * sky_top;
    }
  return c;
}

/// ErrorMessage prints out \p message first and then prints the standard
/// message
/// \code
/// "There was an error. Exiting.\n".
/// \endcode
///
/// \param message The programmer defined string that specifies the current
/// error.
void ErrorMessage(const string& message) {
  cout << message << "\n";
  cout << "There was an error. Exiting.\n";
}

/// \remark To install Graphics Magick on an Ubuntu system `sudo apt install
/// graphicsmagick`.
/// \remark To convert from a PPM to a PNG, `gm convert input.ppm output.png`
/// \remark To convert from a PPM to a JPG, `gm convert input.ppm output.png`
int main(int argc, char const* argv[]) {
  if (argc < 2) {
    ErrorMessage("Please provide a path to a file.");
    exit(1);
  }
  string argv_one_output_file_name = string(argv[1]);


  const double kAspectRatio = 16.0 / 9.0;
  
  const int kImageWidth = 800;
  
  const int kImageHeight = int(lround(kImageWidth / kAspectRatio));

  
  
   const int kSamplesPerPixel = 10;

  Image image(argv_one_output_file_name, kImageWidth, kImageHeight);
  if (!image.is_open()) {
    ostringstream message_buffer("Could not open the file ", ios_base::ate);
    
    message_buffer << argv_one_output_file_name << "!";
    
    ErrorMessage(message_buffer.str());
    exit(1);
  }
  cout << "Image: " << image.height() << "x" << image.width() << "\n";

  
  const int kNumSpheres = 150;
  auto world = RandomScene(kNumSpheres);

  
  const double kViewportHeight = 2.0;
  
  const double kViewportWidth = kAspectRatio * kViewportHeight;
  
  const double kFocalLength = 1.0;
  
  const Point3 kOrigin{0, 0, 0};
  
  const Vec3 kHorizontal{kViewportWidth, 0, 0};
  
  const Vec3 kVertical{0, kViewportHeight, 0};
  
  const Vec3 kLowerLeftCorner =
      kOrigin - kHorizontal / 2 - kVertical / 2 - Vec3(0, 0, kFocalLength);

  
  chrono::time_point<chrono::high_resolution_clock> start =
      chrono::high_resolution_clock::now();

  Color pixel_color;  

  for (int row = image.height() - 1; row >= 0; row--) {
    for (int column = 0; column < image.width(); column++) {
      // u is the distance from the left edge of the image to the right
      // edge of the image measured as a percentage.
      // The column divided by the image.width() - 1 is the percentage of
      // the distance from the left side of the image to the right.
      // Consider column = 0 then colum / (image.width() - 1) -> 0.0
      //          column = (image.width() - 1) / 2 then
      //          colum / (image.width() - 1) -> 0.5
      //          column = image.width() - 1 then
      //          colum / (image.width() - 1) -> 1.0
      // The same is true for v
 

       for (int s = 0; s < kSamplesPerPixel; s++) {        
  
      double u = double(column) / double(image.width() - 1);
      double v = double(row) / double(image.height() - 1);
      // Create a ray that starts at the camera's center, the origin, and
      // travels through the pixel center defined by
      // kLowerLeftCorner + u * kHorizontal + v * kVertical - kOrigin
      Ray r{kOrigin,
            kLowerLeftCorner + u * kHorizontal + v * kVertical - kOrigin};
      // cout << "row: " << row << " col: " << column << " u: " << u << " v: "
      // << v << "\n" << r << "\n";
      // Calculate and return the color at the pixel that Ray r
      // points through.
      pixel_color = pixel_color + RayColor(r, world);
      } 
    double scale = 1.0 / double(kSamplesPerPixel);
    double r = Clamp(std::sqrt(pixel_color.r() * scale), 0.0, 1.0);
    double g = Clamp(std::sqrt(pixel_color.g() * scale), 0.0, 1.0);
    double b = Clamp(std::sqrt(pixel_color.b() * scale), 0.0, 1.0);
    pixel_color = Color(r, g, b);
      // Write the color to the image file.
    image.write(pixel_color);
    }
  }
  // Save the ending time
  chrono::time_point<chrono::high_resolution_clock> end =
      chrono::high_resolution_clock::now();

  // Calculate the elapsed time by taking the difference between end
  // and start.
  chrono::duration<double> elapsed_seconds = end - start;
  // Display the elapsed number of seconds.
  cout << "Time elapsed: " << elapsed_seconds.count() << " seconds.\n";

  return 0;
}
