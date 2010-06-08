#ifndef _MAIN_H
#define _MAIN_H

#include <windows.h>		// Archivo de cabecera para Windows
#include <math.h>			// Archivo de cabecera para Funciones Matemáticas
#include <stdio.h>			// Header File For Standard Input/Output
#include <stdlib.h>			// Header File For Standard Library
#include <string>			// Used for our STL string objects
#include <fstream>
#include <vector>
#include <crtdbg.h>
#include "gl.h"				// Archivo de cabecera para la libreria OpenGL32
#include "glu.h"			// Archivo de cabecera para la libreria GLu32
#include "glaux.h"			// Archivo de cabecera para la libreria Glaux
#include "glext.h"			// Archivo de cabecera para la libreria Glext
#include "Vector.h"
#include "cargadorTGA.h"
#include "Shaders.h"
#include "Multitexturas.h"
#include "Materiales.h"
#include "Font.h"
#include "fmod.h"
#include "fmod_errors.h"
#include "SombraVolumen.h" // Agregado segun las modificaciones del programa 2

extern HWND hWnd;

void Camara(int funcion);
int  ManejaTeclado();
void ManejaTeclado(FMOD_SYSTEM *system, FMOD_RESULT result);

struct parametros
{
	CVector PosicionObj;	//La posición (x,y,z) del objeto
	CVector PosicionObjA;	//La posición (x,y,z) del objeto
	CVector Direccion;		//La dirección en que se dirige el objeto en forma de vector=(x,y,z)
	CVector PosicionCam;    //La posición de la cámara que sigue al objeto
	CVector ObjetivoCam;	//El punto (x,y,z) que está viendo la cámara; en este caso al objeto
	CVector PosAntObj;	//La posición (x,y,z) del objeto

	float VelocidadObj;		//La velocidad a la que se mueve el objeto
	float DistanciaCam;		//La distancia que la cámara está separada del objeto
	float AngDir;			//Se usa para llevar control del angulo para el cálculo del vector de dirección
	float AngObj;			//Se usa para llevar control del ángulo de rotación para el modelo del objeto

	float CamaraPosAlt;		//Se usa para definir y cambiar si es necesario la altura de la cámara
	float CamaraObjAlt;		//Se usa para definir y cambiar si es necesario la altura del objetivo de la cámara

	float CamaraObjAltE;

	float escalaX;
	float escalaY;
	float escalaZ;

	int Dir;				//1: Hacia adelante; 2: hacia atras; 3: hacia la izquierda; 4: hacia la derecha
	int DirAnt;

	bool visible;
	bool caminando;
	bool saltando;
	bool disparando;
	
};

#include "Animator.h"

struct jerarquiaModelo
{
	float Angt1;
	float Angt2;
	float Angc1;
	float Angc2;
	float Angbi1;
	float Angbi2;
	float Angbib;
	float Angbd1;
	float Angbd2;
	float Angbdb;
	float Angpizq;
	float Angpizqb;
	float Angpder;
	float Angpderb;
	float Angpi;
	float Angpd;

	float Xtor;
	float Ytor;
	float Ztor;
};

struct FRAME
{
	float Angt1;
	float Angt2;
	float Angc1;
	float Angc2;
	float Angbi1;
	float Angbi2;
	float Angbib;
	float Angbd1;
	float Angbd2;
	float Angbdb;
	float Angpizq;
	float Angpizqb;
	float Angpder;
	float Angpderb;
	float Angpi;
	float Angpd;

	float Xtor;
	float Ytor;
	float Ztor;

	float incAngt1;
	float incAngt2;
	float incAngc1;
	float incAngc2;
	float incAngbi1;
	float incAngbi2;
	float incAngbib;
	float incAngbd1;
	float incAngbd2;
	float incAngbdb;
	float incAngpizq;
	float incAngpizqb;
	float incAngpder;
	float incAngpderb;
	float incAngpi;
	float incAngpd;

	float incXtor;
	float incYtor;
	float incZtor;

};

//Colisiones
struct boundingplane
{
	CVector Normal;
	CVector PM;
	CVector A, B, C, D;
	CVector b1, b2, b3, b4;
	CVector b1Normal, b2Normal, b3Normal, b4Normal;
	float ancho;
	float alto;
	int tipo; //1: plano de piso; 2: plano de pared
};

struct boundingsphere
{
	CVector Pos;
	float radio;
	bool colision;
	
};

struct ray
{
	CVector dir;
	CVector punto;
	bool colision;
};

#endif 