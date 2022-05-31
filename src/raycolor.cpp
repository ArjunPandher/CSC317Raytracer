#include "raycolor.h"
#include "first_hit.h"
#include "blinn_phong_shading.h"
#include "reflect.h"
#include "limits.h"
#include <iostream>

bool raycolor(
  const Ray & ray, 
  const double min_t,
  const std::vector< std::shared_ptr<Object> > & objects,
  const std::vector< std::shared_ptr<Light> > & lights,
  const int num_recursive_calls,
  Eigen::Vector3d & rgb)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  ////////////////////////////////////////////////////////////////////////////

  int hit_id;
  double t;
  Eigen::Vector3d n;

  if (first_hit(ray, min_t, objects, hit_id, t, n)) {
    // do something
    n.normalize();
    if (num_recursive_calls == 0) {
      rgb += blinn_phong_shading(ray, hit_id, t, n, objects, lights);
    } else {
      rgb += (objects[hit_id]->material->km.array() * blinn_phong_shading(ray, hit_id, t, n, objects, lights).array()).matrix();
    }
    if (num_recursive_calls > 30) {
      return true;
    }
    // compute reflected ray, recursive call
    Ray reflected_ray;
    reflected_ray.origin = (ray.origin + t * ray.direction);
    reflected_ray.direction = (ray.direction - 2*(ray.direction.dot(n))*n);
    return raycolor(reflected_ray, 0.00001, objects, lights, num_recursive_calls + 1, rgb);
  } else {
    // hit background
    Eigen::Vector3d background;
    background << 0, 0, 0;
    rgb += background;
    return false;
  }
}
