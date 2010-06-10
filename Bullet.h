#ifndef BULLET_H
#define BULLET_H

#include "Main.h"

class Bullet
{

public:

	// Constructor
	Bullet( CVector origen, CVector destino );

	void changePara( float vel, float radius, CVector destino );
	void drawBullet();
	void moveToTarget();
	boundingsphere getSphere();

private:
	float speed;
	boundingsphere bala;
	CVector sTarget;

};

#endif // BULLET_H