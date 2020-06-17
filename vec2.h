//https://github.com/OneLoneCoder/videos/blob/master/OneLoneCoder_OperatorOverloading.cpp
#include <cmath>


#ifndef VEC2_H
#define VEC2_H

    #if defined(RAYMATH_STANDALONE)
        #include "raymath.h"
    #endif

    #if defined(RAYLIB_H)
        #include "raylib.h"
    #endif



struct vec2{
    float x = 0.0f;
    float y = 0.0f;
    vec2() : x(0.0f), y(0.0f) {}
    vec2(float _x, float _y) : x(_x), y(_y) {}
    vec2(const vec2& v) : x(v.x), y(v.y) {}

    vec2    operator +  (const vec2&  rhs)  {return vec2(this->x + rhs.x, this->y + rhs.y);     }
    vec2    operator -  (const vec2&  rhs)  {return vec2(this->x - rhs.x, this->y - rhs.y);     }
    vec2    operator *  (const float& rhs)  {return vec2(this->x * rhs, this->y * rhs);         }
    vec2    operator *  (const vec2&  rhs)  {return vec2(this->x * rhs.x, this->y * rhs.y);     }
    vec2    operator /  (const float& rhs)  {return vec2(this->x / rhs, this->y / rhs);         }
    vec2    operator /  (const vec2&  rhs)  {return vec2(this->x / rhs.x, this->y / rhs.y);     }
    vec2    operator += (const vec2&  rhs)  {this->x += rhs.x;this->y += rhs.y;return * this;   }
    vec2    operator *= (const float& rhs)  {this->x *= rhs;this->y *= rhs;return * this;       }
    vec2    operator *= (const vec2&  rhs)  {this->x *= rhs.x;this->y *= rhs.y;return * this;   }
    vec2    operator /= (const float& rhs)  {this->x /= rhs;this->y /= rhs;return * this;       }
    vec2    operator /= (const vec2&  rhs)  {this->x /= rhs.x;this->y *= rhs.y;return * this;   }

    float   length      ()                  {return sqrt(x*x + y*y);                            }
    vec2    normalized  ()                  {float r = 1/length();return vec2(x*r, y*r);        }
    float   distance    (const vec2& v)     {return sqrt((v.x-x)*(v.x-x) + (v.y-y)*(v.y-y));    }
    vec2    direction   (const vec2& v)     {return vec2(v.x-x, v.y-y).normalized();            }
    vec2    lerp        (const vec2& v, const float& perc){return vec2(x+(v.x-x)*perc, y+(v.y-y)*perc);}
    vec2    move        (const vec2& vel, const float& delta){return vec2(x+(vel.x)*delta, y+(vel.y)*delta);}
    vec2    tangent     ()                  {return vec2(-y, x);                                }           //Perpendicular vector
    float   dot         (const vec2& v)     {return x*v.x + y*v.y;                              }           //Dot product
    vec2    floor       ()                  {return vec2(::floor(x), ::floor(y));                   }
    vec2    ceil        (const vec2& v)     {return vec2(::ceil(x), ::ceil(y));                     }
    bool    is_equal    (const vec2& v)     {return fabs(x-v.x)<0.0001 && fabs(y-v.y)<0.0001;   }
    vec2    sign        ()                  {return vec2(fsign(x), fsign(y));                     }
    vec2    rotate      (const float& rad)  {return vec2(x*cos(rad), y*sin(rad));               }

    //Framework compatibility: RAYLIB
#if defined RAYLIB_H || defined RAYMATH_STANDALONE
    vec2(const Vector2& v) : x(v.x), y(v.y) {}
    Vector2 to_Vector2  ()                  {return (Vector2){x, y};                            }
#endif

private:
    float fsign(const float& f){
        if (f >= 0){return 1.0f;}
        return -1.0f;
    }
};

#endif
