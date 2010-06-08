#include "Frustum.h"

#define FIRST_OBJECT_ID  3								

enum FrustumSide
{
	RIGHT	= 0,		// lado derecho del frustum
	LEFT	= 1,		// lado izquierdo del frustum
	BOTTOM	= 2,		// lado inferior del frustum
	TOP		= 3,		// lado superior del frustum
	BACK	= 4,		// lado trasero del frustum
	FRONT	= 5			// lado frontal del frustum
}; 

enum PlaneData
{
	A = 0,				// Comp. X de la normal del plano
	B = 1,				// Comp. Y de la normal del plano
	C = 2,				// Comp. Z de la normal del plano
	D = 3				// Distancia del plano al origen
};

void NormalizePlane(float frustum[6][4], int side)
{
	//Se calcula la magnitud de la normal al plano (x, y, z) que es igual que (A, B, C)
	float magnitude = (float)sqrt( frustum[side][A] * frustum[side][A] + 
								   frustum[side][B] * frustum[side][B] + 
								   frustum[side][C] * frustum[side][C] );

	//Ahora se normalizan los valores del plano
	frustum[side][A] /= magnitude;
	frustum[side][B] /= magnitude;
	frustum[side][C] /= magnitude;
	frustum[side][D] /= magnitude; 
}


//Se obtinen los datos del frustrum desde la matriz de modelo de vista
void CFrustum::CalculateFrustum()
{    
	float   proj[16];								// Guarda una copia de la matriz de proyección
	float   modl[16];								// Guarda una copia de la matriz de modelo de vista
	float   clip[16];								// Guarda una copia de los planos de recorte

	//Obtenemos la copia de la matriz de proyeccion (4x4 pero almacenada en un arreglo lineal 16x1)
	glGetFloatv( GL_PROJECTION_MATRIX, proj );

	//Obtenemos la copia de la matriz de modelo de vista (4x4 pero almacenada en un arreglo lineal 16x1)
	glGetFloatv( GL_MODELVIEW_MATRIX, modl );

	//Multiplicando ambas matrices se obtienen los planos de recorte
	
	clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
	clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
	clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
	clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

	clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
	clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
	clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
	clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

	clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
	clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
	clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
	clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

	clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
	clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
	clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
	clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];
	
	//A partir de los planos de recorte se pueden calcular los valores entre cada par de planos y obtener los lados del frustrum
	
	//Lado derecho del frustrum
	m_Frustum[RIGHT][A] = clip[ 3] - clip[ 0];
	m_Frustum[RIGHT][B] = clip[ 7] - clip[ 4];
	m_Frustum[RIGHT][C] = clip[11] - clip[ 8];
	m_Frustum[RIGHT][D] = clip[15] - clip[12];

	//El plano para el lado derecho se representa por su normal (A,B,C) y la distancia (D) del plano al origen

	//Se normaliza el lado derecho (normal y distancia del plano)
	NormalizePlane(m_Frustum, RIGHT);

	//Lado izquierdo del frustrum
	m_Frustum[LEFT][A] = clip[ 3] + clip[ 0];
	m_Frustum[LEFT][B] = clip[ 7] + clip[ 4];
	m_Frustum[LEFT][C] = clip[11] + clip[ 8];
	m_Frustum[LEFT][D] = clip[15] + clip[12];

	//Se normaliza el lado izquierdo (normal y distancia del plano)
	NormalizePlane(m_Frustum, LEFT);

	//Lado inferior del frustrum
	m_Frustum[BOTTOM][A] = clip[ 3] + clip[ 1];
	m_Frustum[BOTTOM][B] = clip[ 7] + clip[ 5];
	m_Frustum[BOTTOM][C] = clip[11] + clip[ 9];
	m_Frustum[BOTTOM][D] = clip[15] + clip[13];

	//Se normaliza el lado inferior (normal y distancia del plano)
	NormalizePlane(m_Frustum, BOTTOM);

	//Lado superior del frustrum
	m_Frustum[TOP][A] = clip[ 3] - clip[ 1];
	m_Frustum[TOP][B] = clip[ 7] - clip[ 5];
	m_Frustum[TOP][C] = clip[11] - clip[ 9];
	m_Frustum[TOP][D] = clip[15] - clip[13];

	//Se normaliza el lado superior (normal y distancia del plano)
	NormalizePlane(m_Frustum, TOP);

	//Lado trasero del frustrum
	m_Frustum[BACK][A] = clip[ 3] - clip[ 2];
	m_Frustum[BACK][B] = clip[ 7] - clip[ 6];
	m_Frustum[BACK][C] = clip[11] - clip[10];
	m_Frustum[BACK][D] = clip[15] - clip[14];

	//Se normaliza el lado trasero (normal y distancia del plano)
	NormalizePlane(m_Frustum, BACK);

	//Lado frontal del frustrum
	m_Frustum[FRONT][A] = clip[ 3] + clip[ 2];
	m_Frustum[FRONT][B] = clip[ 7] + clip[ 6];
	m_Frustum[FRONT][C] = clip[11] + clip[10];
	m_Frustum[FRONT][D] = clip[15] + clip[14];

	//Se normaliza el lado frontal (normal y distancia del plano)
	NormalizePlane(m_Frustum, FRONT);
}

bool CFrustum::PointInFrustum( float x, float y, float z )
{
	//Para determinar si un punto está adentro del frustrum se considera que todas las normales de los lados del frustrum apuntan
	//hacia adentro, así para cada cara se cumple que el punto está enfrente de ella, entonces está adentro del frustrum

	for(int i = 0; i < 6; i++ )
	{
		//Para cada lado del frustrum se calcula su ecuacion usando el punto y se revisa si está detrás del plano
		if(m_Frustum[i][A] * x + m_Frustum[i][B] * y + m_Frustum[i][C] * z + m_Frustum[i][D] <= 0)
		{
			//Si se cumple esto para un solo lado entonces no está adentro del frustrum
			return false;
		}
	}

	//De lo contrario el punto está adentro del frustrum
	return true;
}


bool CFrustum::SphereInFrustum( float x, float y, float z, float radius )
{
	for(int i = 0; i < 6; i++ )	
	{
		if( m_Frustum[i][A] * x + m_Frustum[i][B] * y + m_Frustum[i][C] * z + m_Frustum[i][D] <= -radius )
		{
			// Si la distancia del centro de la esfera al plano es mas grande que el radio entonces la esfera está afuera del frustrum
			return false;
		}
	}
	
	//De lo contrario la esfera está adentro del frustrum
	return true;
}

bool CFrustum::CubeInFrustum( float x, float y, float z, float size )
{
	//La comprobación para este caso es similar a la esfera usando el centro del cubo y la mitad del ancho de sus caras como radio

	for(int i = 0; i < 6; i++ )
	{
		if(m_Frustum[i][A] * (x - size) + m_Frustum[i][B] * (y - size) + m_Frustum[i][C] * (z - size) + m_Frustum[i][D] > 0)
		   continue;
		if(m_Frustum[i][A] * (x + size) + m_Frustum[i][B] * (y - size) + m_Frustum[i][C] * (z - size) + m_Frustum[i][D] > 0)
		   continue;
		if(m_Frustum[i][A] * (x - size) + m_Frustum[i][B] * (y + size) + m_Frustum[i][C] * (z - size) + m_Frustum[i][D] > 0)
		   continue;
		if(m_Frustum[i][A] * (x + size) + m_Frustum[i][B] * (y + size) + m_Frustum[i][C] * (z - size) + m_Frustum[i][D] > 0)
		   continue;
		if(m_Frustum[i][A] * (x - size) + m_Frustum[i][B] * (y - size) + m_Frustum[i][C] * (z + size) + m_Frustum[i][D] > 0)
		   continue;
		if(m_Frustum[i][A] * (x + size) + m_Frustum[i][B] * (y - size) + m_Frustum[i][C] * (z + size) + m_Frustum[i][D] > 0)
		   continue;
		if(m_Frustum[i][A] * (x - size) + m_Frustum[i][B] * (y + size) + m_Frustum[i][C] * (z + size) + m_Frustum[i][D] > 0)
		   continue;
		if(m_Frustum[i][A] * (x + size) + m_Frustum[i][B] * (y + size) + m_Frustum[i][C] * (z + size) + m_Frustum[i][D] > 0)
		   continue;

		return false;
	}

	return true;
}
