#ifndef __Vector3_H
#define __Vector3_H

#include <math.h>

class Vector3
{
public:
	float x, y, z;

	Vector3( void )
	{
		x = y = z = 0.0;
	};

	Vector3( const float tx, const float ty, const float tz )
	{
		x = tx;
		y = ty;
		z = tz;
	};

//	construtor de copia
	Vector3( const Vector3& v )
	{
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
	};

    void setValor( const float tx, const float ty, const float tz )
	{
		x = tx;
		y = ty;
		z = tz;
	};

    //inicializacao de Vector3
	void setValor(const Vector3 v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	};

//	Soma de Vector3es
	inline Vector3 operator + ( const Vector3 v )
	{
		Vector3 aux( x + v.x, y + v.y, z + v.z );
		return( aux );
	}

//	Subtracao de Vector3es
	inline Vector3 operator - ( const Vector3 v )
	{
		Vector3 aux( x - v.x, y - v.y, z - v.z );
		return( aux );
	}

//	Produto por escalar (float)
	inline Vector3 operator * ( const float s )
	{
		Vector3 aux( x * s, y * s, z * s );
		return( aux );
	}

//	Divisao por escalar
	inline Vector3  operator / ( const float s )
	{
		Vector3 aux( x / s, y / s, z / s );
		return( aux );
	}

//	Produto escalar
	inline float operator * ( const Vector3 v )
	{
		float aux = x * v.x + y * v.y + z * v.z;
		return( aux );
	}

//	Produto Vector3ial
	inline Vector3 operator ^ ( Vector3 v )
	{
		Vector3 aux( y * v.z - z * v.y,
			       z * v.x - x * v.z,
			       x * v.y - y * v.x );
		return( aux );
	}

//  norma do Vector3
	float norma( void )
	{
		return (float)( sqrt( x * x + y * y + z * z ) );
	}

	float distancia( Vector3 v )
	{
		return( (*this - v).norma() );
	}

//	normaliza o Vector3
	Vector3 normalize( void )
	{
		return( (*this) / (this->norma()) );
	}

	//	normaliza o Vector3
	Vector3 extende(float val)
	{
		return ((*this) * (val) );
	}

};

#endif
