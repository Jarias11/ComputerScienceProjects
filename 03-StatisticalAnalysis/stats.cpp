
#include <iostream>
#include <cstdlib>
#include <string>
#include "assert.h"
#include "stats.h"

using namespace main_savitch_2C;
using namespace std;


namespace main_savitch_2C
{
statistician::statistician()
{
	count = 0;
	total = 0;
	tinyest = 0;
	largest = 0;
	}

void statistician::next(double r)
{
	total += r;

	if (length() > 0)
	{
		if (r < tinyest)
		{
			tinyest = r;
		}
		if (r > largest)
		{
			largest = r;
		}
	}
	else if (length() == 0)
	{
		tinyest = r;
		largest = r;
	}
	
	count++;
}

void statistician::reset() 
{
	count = 0;
	total = 0;
	tinyest = 0;
	largest = 0;

}

double statistician::mean() const
{
	assert(length() > 0);

	return(total / count);
}

double statistician::minimum() const
{
	assert(length() > 0);

		return tinyest;
}

double statistician::maximum() const
{
	assert(length() > 0);

	return largest;
}

statistician main_savitch_2C::operator +(const statistician& s1, const statistician& s2)
{
	statistician s3;

	if (s1.length() == 0)
	{
		s3.total = s2.total;
		s3.count = s2.count;
		s3.tinyest = s2.tinyest;
		s3.largest = s2.largest;
	}
	
	else if (s2.length() == 0)
	{
		s3.total = s1.total;
		s3.count = s1.count;
		s3.tinyest = s1.tinyest;
		s3.largest = s1.largest;
	}
	
	else
	{
		s3.total = s1.total + s2.total;
		s3.count = s1.count + s2.count;

		if (s1.tinyest < s2.tinyest)
		{
			s3.tinyest = s1.tinyest;
		}
		else
		{
			s3.tinyest = s2.tinyest;
		}

		if (s1.largest > s2.largest)
		{
			s3.largest = s1.largest;
		}
		else
		{
			s3.largest = s2.largest;
		}
	}

	return s3;
}


statistician main_savitch_2C::operator *(double scale, const statistician& s)
{
	statistician s1;

	
	if (s.length() != 0)
	{
		s1.total = scale * s.total;
		s1.count = s.count;

		
		if (scale > 0)
		{
			s1.tinyest = scale * s.tinyest;
			s1.largest = scale * s.largest;
		}
		
		else
		{
			s1.tinyest = scale * s.largest;
			s1.largest = scale * s.tinyest;
		}
	}

	return s1;
}


bool main_savitch_2C::operator ==(const statistician& s1, const statistician& s2)
{
	if ((s1.length() == 0 && s2.length() == 0) || (s1.length() == s2.length()
		&& s1.sum() == s2.sum() && s1.mean() == s2.mean()
		&& s1.minimum() == s2.minimum() && s1.maximum() == s2.maximum()))
	{
		return true; 
	}
	return false; 
}
}