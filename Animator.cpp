#include "Animator.h"
#include "Main.h"
#include <vector>

void Animator::setChar( parametros &personaje )
{
	modelo = personaje;
	points.push_back( modelo.PosicionObj );
}

void Animator::addPoint( CVector punto )
{
	points.push_back( punto );
}

void Animator::changeSpeed( float speed )
{
	modelo.VelocidadObj = speed;
}

void Animator::startAnim()
{
	// para cada punto al que se debe de mover este personaje
	for( int i = 0; i < points.size(); i++ )
	{
		if( ( modelo.PosicionObj.x <= points[ i ].x + 1.0f && modelo.PosicionObj.x >= points[ i ].x - 1.0f ) &&
			( modelo.PosicionObj.y <= points[ i ].y + 1.0f && modelo.PosicionObj.y >= points[ i ].y - 1.0f ) &&
			( modelo.PosicionObj.z <= points[ i ].z + 1.0f && modelo.PosicionObj.z >= points[ i ].z - 1.0f ) )
		{
			continue;
		}
		else
		{
			moveToPoint( points[ i ] );
			i--;
		}
	}
}

void Animator::moveToPoint( CVector punto )
{
	modelo.PosicionObj = modelo.PosicionObj + Normaliza( punto ) * modelo.VelocidadObj;
}