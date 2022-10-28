#pragma once
#include <random>
////////////////////////////////////////////////////////////////////////////////
namespace agarai
{
	////////////////////////////////////////////////////////////////////////////////
	struct Coord2d
	{
		float X;
		float Y;

		Coord2d(float x=0, float y=0)
			: X(x), Y(y)
		{
		}

		float distance(const Coord2d& c) const
		{
			return sqrtf( powf(c.X - X, 2) +  powf(c.Y - Y, 2));
		}

		Coord2d operator-(const Coord2d& c)
		{
			return Coord2d(X - c.X, Y - c.Y);
		}

		Coord2d operator+(const Coord2d& c)
		{
			return Coord2d(X + c.X, Y + c.Y);
		}

		Coord2d operator/(float n)
		{
			return Coord2d(X / n, Y / n);
		}

		Coord2d operator*(float n)
		{
			return Coord2d(X * n, Y * n);
		}

		Coord2d operator*=(float n)
		{
			*this = *this * n;
			return *this;
		}

		const Coord2d& operator/=(float n)
		{
			*this = *this / n;
			return *this;
		}

		const Coord2d& operator+=(const Coord2d& c)
		{
			*this = *this + c;
			return *this;
		}
	};
	////////////////////////////////////////////////////////////////////////////////
	struct Dimension2D
	{
		int width;
		int height;
	};
	////////////////////////////////////////////////////////////////////////////////
	struct Rectangle
	{
		float left;
		float right;
		float bottom;
		float top;

		Rectangle(float left=0, float right=0, float bottom=0, float top=0)
		{
			this->left = left;
			this->right  = right;
			this->bottom  = bottom;
			this->top  = top;
		}
	};
	////////////////////////////////////////////////////////////////////////////////
	/**
	* Generates a random number in range [min, max).
	*/
	inline float randf(const float min, const float max)
	{
		return min + (std::rand() % (int)(max - min));
	}
	/**
	* Generates a random number in range [0.0f, 1.0f).
	*/
	inline float randf()
	{
		return ((float)std::rand() / RAND_MAX);
	}
}