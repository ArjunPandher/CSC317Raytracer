#include "blinn_phong_shading.h"
// Hint:
#include "first_hit.h"
#include <iostream>
#include "math.h"

double max(double x, double y) {
  if (x < y) {
    return y;
  }
  return x;
}

Eigen::Vector3d blinn_phong_shading(
  const Ray & ray,
  const int & hit_id, 
  const double & t,
  const Eigen::Vector3d & n,
  const std::vector< std::shared_ptr<Object> > & objects,
  const std::vector<std::shared_ptr<Light> > & lights)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  ////////////////////////////////////////////////////////////////////////////

  // ambient light
  Eigen::Vector3d rgb = objects[hit_id]->material->ka;
  Eigen::Vector3d ambient_lum;
  ambient_lum << 0.01, 0.01, 0.01;
  rgb = (rgb.array()*ambient_lum.array()).matrix();
  
  for (int i = 0; i < lights.size(); ++i) {
    Eigen::Vector3d query_point = (ray.origin + t * ray.direction);

    Eigen::Vector3d direction;
    double max_t;
    lights[i]->direction(query_point, direction, max_t);

    Ray ray_i;
    ray_i.origin = query_point;
    ray_i.direction = direction;

    int hit_id_i;
    double t_i;
    Eigen::Vector3d n_i;

    if (!first_hit(ray_i, 0.00001, objects, hit_id_i, t_i, n_i)) {
      // YOU NEED TO NORMALIZE V not leaving this as a comment so you cant ignore it

      
      Eigen::Vector3d h = direction.normalized() + (ray.origin + t * ray.direction.normalized()).normalized();
      h.normalize();
      // adding lambertian shading
      rgb += (objects[hit_id]->material->kd.array() * lights[i]->I.array()).matrix() * max(n.dot(direction), 0);
      // adding shiny bits
      rgb += (objects[hit_id]->material->ks.array() * lights[i]->I.array()).matrix() * pow(max(n.dot(h), 0), objects[hit_id]->material->phong_exponent);
    }
  }
  return rgb;
}
