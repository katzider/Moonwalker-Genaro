#include "Animator.h"

Animator::Animator()
{
	// Just set the index to zero
	i = 0;
}

void Animator::setChar( parametros* personaje )
{
	// Set the model parameters we are using
	modelo = personaje;
	// Add the first point as the current position of this object
	points.push_back( modelo->PosicionObj );
}

void Animator::addPoint( CVector punto )
{
	// Add a point to move to
	points.push_back( punto );
}

void Animator::changeSpeed( float speed )
{
	// Easy way to set the speed of the current model we are using
	modelo->VelocidadObj = speed;
}

void Animator::startAnim()
{
	// As long as there are valid points to get to...
	if( i + 1 < points.size() )
	{
		// Check whether we are in range of the next point
		if( ( modelo->PosicionObj.x <= points[ i + 1 ].x + 1.0f && modelo->PosicionObj.x >= points[ i + 1 ].x - 1.0f ) &&
			( modelo->PosicionObj.y <= points[ i + 1 ].y + 1.0f && modelo->PosicionObj.y >= points[ i + 1 ].y - 1.0f ) &&
			( modelo->PosicionObj.z <= points[ i + 1 ].z + 1.0f && modelo->PosicionObj.z >= points[ i + 1 ].z - 1.0f ) )
		{
			// If we are, move on to the next stage Parappa
			i++;
		}
		else
		{
			// If we are not, move a little bit into the next point
			moveToPoint( points[ i + 1 ] );
		}
	}
	else
	{
		// Duck and turn, and also start from the first point
		i = -1;
	}
}

void Animator::moveToPoint( CVector punto )
{
	// Get a vector from the current point to the next point
	CVector temp = punto - modelo->PosicionObj;
	// Normalize it
	temp = Normaliza( temp );
	// Move towards the next point according to our speed
	modelo->PosicionObj = modelo->PosicionObj + temp * modelo->VelocidadObj;
}