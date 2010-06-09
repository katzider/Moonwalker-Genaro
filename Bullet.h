#ifndef BULLET_H
#define BULLET_H

#include "Main.h"

class Bullet
{

public:

	// Constructor
	Bullet( CVector origen );

	void changePara( float vel, float radius, CVector destino );
	void drawBullet();
	bool moveToTarget();
	void destroy();

private:
	boundingsphere bala;
	float speed;
	CVector sTarget;

};

#endif // BULLET_H