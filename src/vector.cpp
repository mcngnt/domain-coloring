#include "vector.h"

Vector vec_v(float x, float y)
{
	Vector myVec;
	myVec.x = x;
	myVec.y = y;
	return myVec;
}

Vector print_v(Vector vec)
{
	printf("(%f, %f)\n", vec.x, vec.y);
	return vec;
}

Vector add_vec_v(Vector vec1, Vector vec2)
{
	return vec_v(vec1.x + vec2.x, vec1.y + vec2.y);
}

Vector invert_v(Vector vec)
{
	return vec_v(-vec.x, -vec.y);
}

Vector sub_vec_v(Vector vec1, Vector vec2)
{
	return add_vec_v(vec1, invert_v(vec2));
}

Vector add_scal_v(Vector vec, float scalar)
{
	return vec_v(vec.x + scalar, vec.y + scalar);
}

Vector sub_scalar_v(Vector vec, float scalar)
{
	return add_scal_v(vec, -scalar);
}

float get_norm_v(Vector vec)
{
	return sqrt(vec.x*vec.x + vec.y*vec.y);
}

float dist_v(Vector vec1, Vector vec2)
{
	Vector diffVec = sub_vec_v(vec1, vec2);
	return get_norm_v(diffVec);
}

Vector divide_scal_v(Vector vec, float scalar)
{
	if(scalar == 0)
	{
		return vec;
	}
	return vec_v(vec.x / scalar, vec.y / scalar);
}

Vector divide_vec_v(Vector vecNum, Vector vecDenom)
{
	Vector result;
	if (vecDenom.x == 0)
	{
		result.x = vecNum.x;
	}
	else
	{
		result.x = vecNum.x / vecDenom.x;
	}
	if (vecDenom.y == 0)
	{
		result.y = vecNum.y;
	}
	else
	{
		result.y = vecNum.y / vecDenom.y;
	}
	return result;
}

int equal_v(Vector vec1, Vector vec2)
{
	return vec1.x == vec2.x && vec1.y == vec2.y;
}

Vector abs_v(Vector vec)
{
	return vec_v(abs(vec.x), abs(vec.y));
}

float dot_v(Vector vec1, Vector vec2)
{
	return vec1.x*vec2.x + vec1.y*vec2.y;
}

Vector normalize_v(Vector vec)
{
	return divide_scal_v(vec, get_norm_v(vec));
}

Vector mult_scalar_v(Vector vec, float scalar)
{
	return vec_v(vec.x * scalar, vec.y * scalar);
}

Vector max_v(Vector vec, float scalMax)
{
	return vec_v(fmax(vec.x, scalMax), fmax(vec.y, scalMax));
}

Vector min_v(Vector vec, float scalMin)
{
	return vec_v(fmin(vec.x, scalMin), fmin(vec.y, scalMin));
}

float get_angle_v(Vector vec)
{
	return atan2(vec.y,vec.x);
}

Vector vec_from_angle_v(float angle)
{
	Vector vec;
	vec.x = cos(angle - PI/2);
	vec.y = sin(angle - PI/2);
	return vec;
}

Vector random_vec_v(int minX, int maxX, int minY, int maxY)
{
	Vector vec;
	vec.x = (float)((rand() % (maxX + abs(minX))) - minX);
	vec.y = (float)((rand() % (maxY + abs(minY))) - minY);
	return vec;
}

sf::Vector2f vec_to_sfvec2_v(Vector vec)
{
	return sf::Vector2f(vec.x, vec.y);
}

Vector sfvec2_to_vec_v(sf::Vector2f vec)
{
	return vec_v(vec.x, vec.y);
}

Vector mult_comp_c(Vector a, Vector b)
{
	return vec_v(a.x*b.x-a.y*b.y, a.x*b.y+a.y*b.x);
}

Vector conjugate_c(Vector z)
{
	return vec_v(z.x, -z.y);
}
Vector divide_comp_c(Vector a, Vector b)
{
	return vec_v(((a.x*b.x+a.y*b.y)/(b.x*b.x+b.y*b.y)),((a.y*b.x-a.x*b.y)/(b.x*b.x+b.y*b.y)));
}
Vector power_comp_c(Vector a, int p)
{
  	Vector result = a;
  	for (int i = 0; i < p-1; ++i)
  	{
    	result = mult_comp_c(result,a);
	}
	return result;
}

// float get_arg_c(Vector z)
// {
//     float angle = z.x == 0.0 ? abs(z.y)*1.57f : atan(abs(z.y), z.x);
//     if (z.y < 0.f)
//     {
//       return (6.28f-angle)/6.28f;
//     }
//     return angle/6.28f;
// }