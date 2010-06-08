#ifndef _FRUSTUM_H
#define _FRUSTUM_H

#include "main.h"

class CFrustum {

public:

	//Cada vez que se mueve la cámara hay que llamar a esta funcion
	void CalculateFrustum();

	bool PointInFrustum(float x, float y, float z);
	bool SphereInFrustum(float x, float y, float z, float radius);
	bool CubeInFrustum( float x, float y, float z, float size );

private:

	//Guarda los valores A, B, C y D para cada plano que forma un lado del frustrum.
	float m_Frustum[6][4];
};


#endif
