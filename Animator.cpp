#include "Animator.h"

Animator::Animator()
{
	i = 0;
}

void Animator::setChar( parametros* personaje )
{
	modelo = personaje;
	points.push_back( modelo->PosicionObj );
}

void Animator::addPoint( CVector punto )
{
	points.push_back( punto );
}

void Animator::changeSpeed( float speed )
{
	modelo->VelocidadObj = speed;
}

void Animator::startAnim()
{

	if( i + 1 < points.size() )
	{
		if( ( modelo->PosicionObj.x <= points[ i + 1 ].x + 1.0f && modelo->PosicionObj.x >= points[ i + 1 ].x - 1.0f ) &&
			( modelo->PosicionObj.y <= points[ i + 1 ].y + 1.0f && modelo->PosicionObj.y >= points[ i + 1 ].y - 1.0f ) &&
			( modelo->PosicionObj.z <= points[ i + 1 ].z + 1.0f && modelo->PosicionObj.z >= points[ i + 1 ].z - 1.0f ) )
		{
				i++;
		}
		else
		{
			moveToPoint( points[ i + 1 ] );
		}
	}
	else
		i -= 2;
}

void Animator::moveToPoint( CVector punto )
{
	CVector temp = punto - modelo->PosicionObj;
	temp = Normaliza( temp );
	modelo->PosicionObj = modelo->PosicionObj + temp * modelo->VelocidadObj;
}