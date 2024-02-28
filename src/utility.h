#ifndef UTILITY_H_
#define UTILITY_H_

#include <tgmath.h>

// `M_PI` is not guaranteed to be defined by the C standard.
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static inline float degrees_to_radians(float degrees)
{
  return degrees * (M_PI / 180.0f);
}

static inline float radians_to_degrees(float radians)
{
  return radians * (180.0f / M_PI);
}

static inline float clamp(float value, float low, float high)
{
  return fmax(fmin(value, high), low);
}

static inline double wrap_angle(double angle)
{
  const double rem = fmod(angle, 2 * M_PI);
  return angle >= 0 ? rem : rem + 2 * M_PI;
}

char* strsep(char** stringp, const char* delim);

#endif
