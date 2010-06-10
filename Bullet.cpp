#include "Bullet.h"

Bullet::Bullet( CVector origen, CVector destino )
{
	bala.Pos = origen;
	changePara( 0.3f, 0.75f, destino );
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

	gluQuadricDrawStyle(q, GLU_FILL );

	glDisable(GL_LIGHTING);

	glColor3f(1.0f, 1.0f, 1.0f);

	glPushMatrix();
		glColor3f( 1.0f, 1.0f, 0.0f );
		glTranslatef( bala.Pos.x, bala.Pos.y, bala.Pos.z);
		glRotatef(90.0f,1.0f,0.0f,0.0f);
		gluSphere(q, bala.radio, 16, 8);
	glPopMatrix();

	glEnable( GL_LIGHTING );

}

void Bullet::moveToTarget()
{
	if( ( bala.Pos.x <= sTarget.x + bala.radio && bala.Pos.x >= sTarget.x - bala.radio ) &&
		( bala.Pos.x <= sTarget.y + bala.radio && bala.Pos.y >= sTarget.y - bala.radio ) &&
		( bala.Pos.x <= sTarget.z + bala.radio && bala.Pos.z >= sTarget.z - bala.radio ) )
	{
		//speed = 0.0f;
		// shit I dunno why but it never reaches this point lol, so much for that, huh?
	}
	else
	{
		// Get a vector from the current point to the next point
		CVector temp = sTarget - bala.Pos;
		// Normalize it
		temp = Normaliza( temp );
		// Move towards the next point according to our speed
		bala.Pos = bala.Pos + temp * speed;
		drawBullet();
	}
}

boundingsphere Bullet::getSphere()
{
	return bala;
}