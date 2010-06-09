#include "Bullet.h"

Bullet::Bullet( CVector origen )
{
	bala.Pos = origen;
}

void Bullet::changePara( float vel, float radius, CVector destino )
{
	speed = vel;
	// Cambia el radio de la esfera
	bala.radio = radius;
	sTarget = destino;
}

void Bullet::drawBullet()
{
	GLUquadricObj	*q;

	q = gluNewQuadric();

	gluQuadricDrawStyle(q, GLU_LINE);

	glDisable(GL_LIGHTING);

	glColor3f(1.0f, 1.0f, 1.0f);

	glPushMatrix();
		glTranslatef( bala.Pos.x, bala.Pos.y, bala.Pos.z);
		glRotatef(90.0f,1.0f,0.0f,0.0f);
		gluSphere(q, bala.radio, 16, 8);
	glPopMatrix();

}

bool Bullet::moveToTarget()
{
	if( ( bala.Pos.x <= sTarget.x + 1.0f && bala.Pos.x >= sTarget.x - 1.0f ) &&
		( bala.Pos.x <= sTarget.y + 1.0f && bala.Pos.y >= sTarget.y - 1.0f ) &&
		( bala.Pos.x <= sTarget.z + 1.0f && bala.Pos.z >= sTarget.z - 1.0f ) )
	{
		bala.radio = 0.0f;
		return true;
	}
	else
	{
		// Get a vector from the current point to the next point
		CVector temp = sTarget - bala.Pos;
		// Normalize it
		temp = Normaliza( temp );
		// Move towards the next point according to our speed
		bala.Pos = bala.Pos + temp * speed;
	}
}