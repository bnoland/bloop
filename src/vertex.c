#include "vertex.h"

GSOut gsout_add(const GSOut* v, const GSOut* w)
{
  GSOut result;
  result.pos = vec4_add(&v->pos, &w->pos);
  return result;
}

GSOut gsout_sub(const GSOut* v, const GSOut* w)
{
  GSOut result;
  result.pos = vec4_sub(&v->pos, &w->pos);
  return result;
}

GSOut gsout_mul(const GSOut* v, float c)
{
  GSOut result;
  result.pos = vec4_mul(&v->pos, c);
  return result;
}

GSOut gsout_mul_add(const GSOut* v, const GSOut* w, float c)
{
  GSOut result;
  result.pos = vec4_mul_add(&v->pos, &w->pos, c);
  return result;
}

GSOut gsout_interpolate(const GSOut* v, const GSOut* w, float alpha)
{
  GSOut result;
  result.pos = vec4_interpolate(&v->pos, &w->pos, alpha);
  return result;
}
