#ifndef UTILITY_H_
#define UTILITY_H_

// XXX: Might want to break this up into several modules.

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

char* strsep(char** stringp, const char* delim);

#endif
