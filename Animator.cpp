#include "Animator.h"
#include "Bullet.h"

Animator::Animator()
{
	// Just set the index to zero
	i = 0;
	// Set the shooting timer to its default value
	delay = 115 + rand() % ((200 + 1) - 115 );
}

void Animator::setChar( parametros* personaje )
{
	// Set the model parameters we are using
	modelo = personaje;
	// Add the first point as the current position of this object
	points.push_back( modelo->PosicionObj );
	// Set the origin of the ray as this model's position
	rayo.origen = modelo->PosicionObj;
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

/* The function used to start the waypoints stored, if you wanna test it, I think you'll find 
 * the things this teaches you, are sure to beat you, but nevertheless, you'll get a lesson
 * from teacher now.
 */
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
	// Update the ray's origin point with the current model's point
	rayo.origen = modelo->PosicionObj;
}

// This function kicks and punch, it's all in the mind
void Animator::moveToPoint( CVector punto )
{
	// Get a vector from the current point to the next point
	CVector temp = punto - modelo->PosicionObj;
	// Normalize it
	temp = Normaliza( temp );
	// Move towards the next point according to our speed
	modelo->PosicionObj = modelo->PosicionObj + temp * modelo->VelocidadObj;
}
// This function sets where the enemy should be facing and waiting for MJ
void Animator::setTarget( CVector destino )
{
	rayo.dir = destino;
}
// This function draws the enemies vision rays
void Animator::drawRay()
{
	glDisable( GL_LIGHTING );
	glBegin( GL_LINES );
		glColor3f( 0.0f, 1.0f, 0.0f );
		glVertex3f( rayo.origen.x, rayo.origen.y, rayo.origen.z );
		glVertex3f( rayo.dir.x, rayo.dir.y, rayo.dir.z );
		glColor3f( 0.0f, 0.0f, 0.0f );
	glEnd();
	glEnable( GL_LIGHTING );
}

// Reload one bullet
int Animator::Reload()
{
	--delay;
	return delay;
}
// Fire one bullet
CVector Animator::attack()
{
	// create one bullet
	CVector bull = modelo->PosicionObj;
	// reset the current delay 
	delay = 115 + rand() % ((200 + 1) - 115 );
	return bull;
}
int Animator::getDelay()
{
	return delay;
}

CVector Animator::getPos()
{
	return modelo->PosicionObj;
}