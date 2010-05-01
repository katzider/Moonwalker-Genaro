#include "Materiales.h"

//Valores default del material en OpenGL
GLfloat gDefaultDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
GLfloat gDefaultAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};
GLfloat gDefaultEmission[]= {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat gDefaultSpecular[]= {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat gDefaultShininess = 0.0f;

//Total
GLfloat totalAmb[4] = {1.0f,1.0f,1.0f,1.0f};
GLfloat totalDif[4] = {1.0f,1.0f,1.0f,1.0f};
GLfloat totalSpe[4] = {1.0f,1.0f,1.0f,1.0f};
GLfloat totalShi = 128.0f;

//Nada
GLfloat nadaAmb[4] = {0.0f,0.0f,0.0f,1.0f};
GLfloat nadaDif[4] = {0.0f,0.0f,0.0f,1.0f};
GLfloat nadaSpe[4] = {0.0f,0.0f,0.0f,1.0f};
GLfloat nadaShi = 1.0f;

//Solo difuso
GLfloat difusionAmb[4] = {0.0f,0.0f,0.0f,1.0f};
GLfloat difusionDif[4] = {1.0f,1.0f,1.0f,1.0f};
GLfloat difusionSpe[4] = {0.0f,0.0f,0.0f,1.0f};
GLfloat difusionShi = 1.0f;

//Solo ambiente
GLfloat ambienteAmb[4] = {1.0f,1.0f,1.0f,1.0f};
GLfloat ambienteDif[4] = {0.0f,0.0f,0.0f,1.0f};
GLfloat ambienteSpe[4] = {0.0f,0.0f,0.0f,1.0f};
GLfloat ambienteShi = 1.0f;

//Solo especular
GLfloat especularAmb[4] = {0.0f,0.0f,0.0f,1.0f};
GLfloat especularDif[4] = {0.0f,0.0f,0.0f,1.0f};
GLfloat especularSpe[4] = {1.0f,1.0f,1.0f,1.0f};
GLfloat especularShi = 40.0f;

//Plastico Naranja
GLfloat plasticoNaranjaAmb[4] = {0.8f,0.25f,0.0f,1.0f};
GLfloat plasticoNaranjaDif[4] = {0.1f,0.035f,0.0f,1.0f};
GLfloat plasticoNaranjaSpe[4] = {0.6f,0.3f,0.0f,1.0f};
GLfloat plasticoNaranjaShi = 24.0f;

//Esmeralda
GLfloat esmeraldaAmb[4] = {0.0215f,0.1745f,0.0215f,0.55f};
GLfloat esmeraldaDif[4] = {0.07568f,0.61424f,0.07568f,0.55f};
GLfloat esmeraldaSpe[4] = {0.633f,0.727811f,0.633f,0.55};
GLfloat esmeraldaShi = 76.8f;

//Cromo
GLfloat cromoAmb[4] = {0.25f,0.25f,0.25f,1.0f};
GLfloat cromoDif[4] = {0.4f,0.4f,0.4f,1.0f};
GLfloat cromoSpe[4] = {0.774597f,0.774597f,0.774597f,1.0f};
GLfloat cromoShi = 76.8f;

//Bronce
GLfloat bronceAmb[4] = {0.2125f,0.1275f,0.054f,1.0f};
GLfloat bronceDif[4] = {0.714f,0.4284f,0.18144f,1.0f};
GLfloat bronceSpe[4] = {0.393548f,0.271906f,0.166721f,1.0f};
GLfloat bronceShi = 25.6f;

//Oro
GLfloat oroAmb[4] = {0.24725f,0.2245f,0.0645f,1.0f};
GLfloat oroDif[4] = {0.34615f,0.3143f,0.0903f,1.0f};
GLfloat oroSpe[4] = {0.797357f,0.723991f,0.208006f,1.0f};
GLfloat oroShi = 128.0f;

//Aluminio
GLfloat aluminioAmb[4] = {0.30f,0.30f,0.35f,1.0f};
GLfloat aluminioDif[4] = {0.30f,0.30f,0.50f,1.0f};
GLfloat aluminioSpe[4] = {0.70f,0.70f,0.80f,1.0f};
GLfloat aluminioShi = 9.0f;

//Cobre
GLfloat cobreAmb[4] = {0.33f,0.26f,0.23f,1.0f};
GLfloat cobreDif[4] = {0.50f,0.11f,0.0f,1.0f};
GLfloat cobreSpe[4] = {0.95f,0.73f,0.0f,1.0f};
GLfloat cobreShi = 93.0f;

//Plata
GLfloat plataAmb[4] = {0.19225f,0.19225f,0.19225f,1.0f};
GLfloat plataDif[4] = {0.50754f,0.50754f,0.50754f,1.0f};
GLfloat plataSpe[4] = {0.508273f,0.508273f,0.508273f,1.0f};
GLfloat plataShi = 51.2f;

//Plastico Negro
GLfloat plasticoNegroAmb[4] = {0.0f,0.0f,0.0f,1.0f};
GLfloat plasticoNegroDif[4] = {0.01f,0.01f,0.01f,1.0f};
GLfloat plasticoNegroSpe[4] = {0.5f,0.5f,0.5f,1.0f};
GLfloat plasticoNegroShi = 32.0f;

//Brillo metalico 1
GLfloat BrilloMetalAmb[4] = {0.25f,0.25f,0.25f,1.0f};
GLfloat BrilloMetalDif[4] = {0.6f,0.6f,0.6f,1.0f};
GLfloat BrilloMetalSpe[4] = {0.874597f,0.874597f,0.874597f,1.0f};
GLfloat BrilloMetalShi = 76.8f;

//Brillo metalico 2
GLfloat BrilloMetal2Amb[4] = {0.65f,0.15f,0.15f,1.0f};
GLfloat BrilloMetal2Dif[4] = {1.0f,0.2f,0.2f,1.0f};
GLfloat BrilloMetal2Spe[4] = {1.0f,0.674597f,0.674597f,1.0f};
GLfloat BrilloMetal2Shi = 76.8f;

CMateriales::CMateriales()
{
	
}

CMateriales::~CMateriales()
{
	
}

void CMateriales::SeleccionaMaterial(int tipo)
{
	if(tipo == 0)
	{
		//Material default de OpenGL
		glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, gDefaultDiffuse);
		glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, gDefaultAmbient);
		glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION,gDefaultEmission); 
		glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR,gDefaultSpecular);
		glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS,gDefaultShininess);
	}
	else if(tipo == 1)
	{
		//Total
		glMaterialfv(GL_FRONT, GL_AMBIENT,   totalAmb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE,   totalDif);
		glMaterialfv(GL_FRONT, GL_SPECULAR,  totalSpe);
		glMaterialf (GL_FRONT, GL_SHININESS, totalShi);
	}
	else if(tipo == 2)
	{
		//Nada
		glMaterialfv(GL_FRONT, GL_AMBIENT,   nadaAmb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE,   nadaDif);
		glMaterialfv(GL_FRONT, GL_SPECULAR,  nadaSpe);
		glMaterialf (GL_FRONT, GL_SHININESS, nadaShi);
	}
	else if(tipo == 3)
	{
		//Solo Difusion
		glMaterialfv(GL_FRONT, GL_AMBIENT,   difusionAmb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE,   difusionDif);
		glMaterialfv(GL_FRONT, GL_SPECULAR,  difusionSpe);
		glMaterialf (GL_FRONT, GL_SHININESS, difusionShi);
	}
	else if(tipo == 4)
	{
		//Solo Ambiente
		glMaterialfv(GL_FRONT, GL_AMBIENT,   ambienteAmb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE,   ambienteDif);
		glMaterialfv(GL_FRONT, GL_SPECULAR,  ambienteSpe);
		glMaterialf (GL_FRONT, GL_SHININESS, ambienteShi);
	}
	else if(tipo == 5)
	{
		//Solo Especular
		glMaterialfv(GL_FRONT, GL_AMBIENT, especularAmb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, especularDif);
		glMaterialfv(GL_FRONT, GL_SPECULAR, especularSpe);
		glMaterialf(GL_FRONT, GL_SHININESS, especularShi);
	}
	else if(tipo == 6)
	{
		//Plastico Naranja
		glMaterialfv(GL_FRONT, GL_AMBIENT, plasticoNaranjaAmb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, plasticoNaranjaDif);
		glMaterialfv(GL_FRONT, GL_SPECULAR, plasticoNaranjaSpe);
		glMaterialf(GL_FRONT, GL_SHININESS, plasticoNaranjaShi);
	}
	else if(tipo == 7)
	{
		//Esmeralda
	    glMaterialfv(GL_FRONT, GL_AMBIENT, esmeraldaAmb);
	    glMaterialfv(GL_FRONT, GL_DIFFUSE, esmeraldaDif);
	    glMaterialfv(GL_FRONT, GL_SPECULAR, esmeraldaSpe);
		glMaterialf(GL_FRONT, GL_SHININESS, esmeraldaShi);
	}
	else if(tipo == 8)
	{
		//Cromo
		glMaterialfv(GL_FRONT, GL_AMBIENT, cromoAmb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, cromoDif);
		glMaterialfv(GL_FRONT, GL_SPECULAR, cromoSpe);
		glMaterialf(GL_FRONT, GL_SHININESS, cromoShi);
	}
	else if(tipo == 9)
	{
		//Bronce
		glMaterialfv(GL_FRONT, GL_AMBIENT, bronceAmb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, bronceDif);
		glMaterialfv(GL_FRONT, GL_SPECULAR, bronceSpe);
		glMaterialf(GL_FRONT, GL_SHININESS, bronceShi);
	}
	else if(tipo == 10)
	{
		//Oro
		glMaterialfv(GL_FRONT, GL_AMBIENT, oroAmb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, oroDif);
		glMaterialfv(GL_FRONT, GL_SPECULAR, oroSpe);
		glMaterialf(GL_FRONT, GL_SHININESS, oroShi);
	}
	else if(tipo == 11)
	{
		//Aluminio
		glMaterialfv(GL_FRONT, GL_AMBIENT, aluminioAmb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, aluminioDif);
		glMaterialfv(GL_FRONT, GL_SPECULAR, aluminioSpe);
		glMaterialf(GL_FRONT, GL_SHININESS, aluminioShi);
	}
	else if(tipo == 12)
	{
		//Cobre
		glMaterialfv(GL_FRONT, GL_AMBIENT, cobreAmb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, cobreDif);
		glMaterialfv(GL_FRONT, GL_SPECULAR, cobreSpe);
		glMaterialf(GL_FRONT, GL_SHININESS, cobreShi);
	}
	else if(tipo == 13)
	{
		//Plata
		glMaterialfv(GL_FRONT, GL_AMBIENT, plataAmb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, plataDif);
		glMaterialfv(GL_FRONT, GL_SPECULAR, plataSpe);
		glMaterialf(GL_FRONT, GL_SHININESS, plataShi);
	}
	else if(tipo == 14)
	{
		//Plastico Negro
		glMaterialfv(GL_FRONT, GL_AMBIENT, plasticoNegroAmb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, plasticoNegroDif);
		glMaterialfv(GL_FRONT, GL_SPECULAR, plasticoNegroSpe);
		glMaterialf (GL_FRONT, GL_SHININESS, plasticoNegroShi);
	}
	else if(tipo == 15)
	{
		//Brillo Metalico 1
		glMaterialfv(GL_FRONT, GL_AMBIENT, BrilloMetalAmb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, BrilloMetalDif);
		glMaterialfv(GL_FRONT, GL_SPECULAR, BrilloMetalSpe);
		glMaterialf (GL_FRONT, GL_SHININESS, BrilloMetalShi);
	}
	else if(tipo == 16)
	{
		//Brillo Metalico 2
		glMaterialfv(GL_FRONT, GL_AMBIENT, BrilloMetal2Amb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, BrilloMetal2Dif);
		glMaterialfv(GL_FRONT, GL_SPECULAR, BrilloMetal2Spe);
		glMaterialf (GL_FRONT, GL_SHININESS, BrilloMetal2Shi);
	}

}