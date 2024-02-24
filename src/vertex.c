#include "vertex.h"

void gsout_add(GSOut* dest, const GSOut* v, const GSOut* w)
{
  vec4_add(&dest->pos, &v->pos, &w->pos);
}

void gsout_sub(GSOut* dest, const GSOut* v, const GSOut* w)
{
  vec4_sub(&dest->pos, &v->pos, &w->pos);
}

void gsout_mul(GSOut* dest, const GSOut* v, float c)
{
  vec4_mul(&dest->pos, &v->pos, c);
}

void gsout_mul_add(GSOut* dest, const GSOut* v, const GSOut* w, float c)
{
  vec4_mul_add(&dest->pos, &v->pos, &w->pos, c);
}

void gsout_interpolate(GSOut* dest, const GSOut* v, const GSOut* w, float alpha)
{
  vec4_interpolate(&dest->pos, &v->pos, &w->pos, alpha);
}
