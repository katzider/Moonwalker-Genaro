#include "Main.h"
#include "3ds.h"

/* Quitados los comentarios de prueba, deberiamos hacer un listado en el README.txt de las cosas que cambian y donde hallarlas, 
aunque tambien sale en el historial de git, a veces me da flojera ver xp */

//Libreria que usamos para el timer
#pragma comment( lib, "winmm.lib" )	

HDC			hDC=NULL;		// Dispositivo de contexto GDI
HGLRC		hRC=NULL;		// Contexto de renderizado
HWND		hWnd=NULL;		// Manejador de ventana
HINSTANCE	hInstance;		// Instancia de la aplicacion

bool	keys[256];			// Arreglo para el manejo de teclado
bool	active=TRUE;		// Bandera de ventana activa

int glWidth;
int glHeight;

GLUquadricObj	*e;

//Variables para fmod (cambiar el tamaño de los arreglos según el número de archivos de audio a usar)
FMOD_SOUND       *sound[2] = { 0, 0 };
FMOD_CHANNEL     *channel[2] = { 0, 0 };

// Variables para la detección de colisiones
#define maxPlanos 30
#define maxPersonajes 30

boundingplane plano[maxPlanos];
boundingsphere esfera[maxPersonajes];

// variables que usaremos para la camara
int CamPos[6][6] = { 
	{ 190.0f, 90.0f, 10.0f, 180.0f, 6.5f, -60.0f }, 
	{ 45.0f, 90.0f, 10.0f, 55.0f, 6.5f, -60.0f }, 
	{ 15.0f, 90.0f, -56.0f, -50.0f, 6.5f, -56.0f }, 
	{ 35.0f, 90.0f, 75.0f, -60.0f, 6.5f, 80.0f }, 
	{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, 
	{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f } 
};
int pisoId = 0;

// Variable de acceso a la estructura de parametros
parametros player1;

parametros enem1;	 //Variable con la que tenemos acceso a la estructura de parámetros de ene1
parametros MJ6;		 //Variable con la que tenemos acceso a la estructura de parámetros de MJ6
parametros enem2;	 //Variable con la que tenemos acceso a la estructura de parámetros de ene2
parametros enem3a;	 //Variable con la que tenemos acceso a la estructura de parámetros de ene3a
parametros enem3b;	 //Variable con la que tenemos acceso a la estructura de parámetros ene3b
parametros chang;	 //Variable con la que tenemos acceso a la estructura de parámetros de chango

parametros enemigo8; // parametros savage

CMateriales Material;

//Nombre y ubicación de los modelos

// Modelos Savage
#define FILE_NAME1g  "Modelos/enemigo8_cuerpo.3ds"
#define FILE_NAME2g  "Modelos/enemigo8_heliceabajo.3ds"
#define FILE_NAME3g  "Modelos/enemigo8_heliceatras.3ds"

//Nombre y ubicacion de los modelos de Aru (MJ y Miku)
#define FILE_NAME1aru  "Modelos/MJ_torso.3ds"
#define FILE_NAME2aru  "Modelos/MJ_piernader_a.3ds"
#define FILE_NAME3aru  "Modelos/MJ_piernader_b.3ds"
#define FILE_NAME4aru  "Modelos/MJ_piernaizq_a.3ds"
#define FILE_NAME5aru  "Modelos/MJ_piernaizq_b.3ds"
#define FILE_NAME6aru  "Modelos/MJ_brazoder_a.3ds"
#define FILE_NAME7aru  "Modelos/MJ_brazoizq_a.3ds"
#define FILE_NAME8aru  "Modelos/MJ_brazoder_b.3ds"
#define FILE_NAME9aru  "Modelos/MJ_brazoizq_b.3ds"

//nombre y ubicación de modelo Enemigo1
#define FILE_NAME1d	 "Modelos/Ene1torso.3ds"
#define FILE_NAME2d	 "Modelos/Ene1Cabeza.3ds"
#define FILE_NAME3d	 "Modelos/Ene1BraDer.3ds"
#define FILE_NAME4d	 "Modelos/Ene1BraIzq.3ds"
#define FILE_NAME5d	 "Modelos/Ene1PierDerA.3ds"
#define FILE_NAME6d	 "Modelos/Ene1PierDerB.3ds"
#define FILE_NAME7d	 "Modelos/Ene1PieDer.3ds"
#define FILE_NAME8d	 "Modelos/Ene1PierIzqA.3ds"
#define FILE_NAME9d	 "Modelos/Ene1PierIzqB.3ds"
#define FILE_NAME10d "Modelos/Ene1PieIzq.3ds"

// Para el escenario de mayra
#define FILE_NAME2e  "Modelos/escenario1.3ds"

//nombre y ubicación de modelo MJ6 (MJ Robot)
#define FILE_NAME1f	 "Modelos/MJ6Torzo.3ds"
#define FILE_NAME2f	 "Modelos/MJ6Cabeza.3ds"
#define FILE_NAME3f	 "Modelos/MJ6BraDerA.3ds"
#define FILE_NAME4f	 "Modelos/MJ6BraDerB.3ds"
#define FILE_NAME5f	 "Modelos/MJ6BraIzqA.3ds"
#define FILE_NAME6f	 "Modelos/MJ6BraIzqB.3ds"
#define FILE_NAME7f	 "Modelos/MJ6PierDerA.3ds"
#define FILE_NAME8f	 "Modelos/MJ6PierDerB.3ds"
#define FILE_NAME9f	 "Modelos/MJ6PierIzqA.3ds"
#define FILE_NAME10f "Modelos/MJ6PierIzqB.3ds"

//nombre y ubicación de modelo Ene3a
#define FILE_NAME1h	 "Modelos/torsoe3a.3ds"
#define FILE_NAME2h	 "Modelos/bder3a.3ds"
#define FILE_NAME3h	 "Modelos/bizq3a.3ds"
#define FILE_NAME4h	 "Modelos/pder3a.3ds"
#define FILE_NAME5h	 "Modelos/pizq3a.3ds"

//nombre y ubicación de modelo ene3b
#define FILE_NAME1i	 "Modelos/torsoe3b.3ds"
#define FILE_NAME2i	 "Modelos/bder3b.3ds"
#define FILE_NAME3i	 "Modelos/bizq3b.3ds"
#define FILE_NAME4i	 "Modelos/pder3b.3ds"
#define FILE_NAME5i	 "Modelos/pizq3b.3ds"

//nombre y ubicación de modelo chango
#define FILE_NAME1j	 "Modelos/torsoch.3ds"
#define FILE_NAME2j	 "Modelos/bderch.3ds"
#define FILE_NAME3j	 "Modelos/bizqch.3ds"
#define FILE_NAME4j	 "Modelos/pderch.3ds"
#define FILE_NAME5j	 "Modelos/pizqch.3ds"

//nombre y ubicación de modelo Enemigo2
#define FILE_NAME1k	 "Modelos/Ene2nave.3ds"
#define FILE_NAME2k	 "Modelos/Ene2bomb.3ds"
//#define FILE_NAME3k	 "Modelos/Ene2pers.3ds"

//Contenedor de texturas de enemigo1
CTga textureModel1d[20];
//Contenedor de texturas de MJ6 (MJ Robot)
CTga textureModel1f[20];

// Contenedor de texturas para Robot8
CTga textureModel1g[20];
CTga textureModel2g[20];
CTga textureModel3g[20];

// Contenedores de texturas de los modelos de Aru(MJ y Miku)
CTga textureModel1aru[20]; //MJ.tga

// Contenedores de texturas del escenario
CTga textureModel2e[20];
//Contenedor de texturas de enemigo3a
CTga textureModel1h[20];
CTga textureModel2h[20];
CTga textureModel3h[20];
CTga textureModel4h[20];
CTga textureModel5h[20];
//Contenedor de texturas de enemigo3b
CTga textureModel1i[20];
CTga textureModel2i[20];
CTga textureModel3i[20];
CTga textureModel4i[20];
CTga textureModel5i[20];
//Contenedor de texturas de chango
CTga textureModel1j[20];
CTga textureModel2j[20];
CTga textureModel3j[20];
CTga textureModel4j[20];
CTga textureModel5j[20];

//Contenedor de texturas de enemigo2
CTga textureModel1k[20];
CTga textureModel2k[20];
CTga textureModel3k[20];

CLoad3DS g_Load3ds;

//Acceso a la estructura que almacena los datos de los modelos
t3DModel g_3DModel1c;
t3DModel g_3DModel2c;
t3DModel g_3DModel3c;
t3DModel g_3DModel4c;
t3DModel g_3DModel5c;
t3DModel g_3DModel6c;
t3DModel g_3DModel7c;
t3DModel g_3DModel8c;
t3DModel g_3DModel9c;

//Acceso a la estructura que almacena los datos de los modelos
t3DModel g_3DModel1aru;
t3DModel g_3DModel2aru;
t3DModel g_3DModel3aru;
t3DModel g_3DModel4aru;
t3DModel g_3DModel5aru;
t3DModel g_3DModel6aru;
t3DModel g_3DModel7aru;
t3DModel g_3DModel8aru;
t3DModel g_3DModel9aru;

// Acceso a la estructura que almacena los datos del escenario
t3DModel g_3DModel2e;

//Acceso a la estructura que almacena a Ene1
t3DModel g_3DModel1d;
t3DModel g_3DModel2d;
t3DModel g_3DModel3d;
t3DModel g_3DModel4d;
t3DModel g_3DModel5d;
t3DModel g_3DModel6d;
t3DModel g_3DModel7d;
t3DModel g_3DModel8d;
t3DModel g_3DModel9d;
t3DModel g_3DModel10d;

//Acceso a la estructura que almacena a MJ6
t3DModel g_3DModel1f;
t3DModel g_3DModel2f;
t3DModel g_3DModel3f;
t3DModel g_3DModel4f;
t3DModel g_3DModel5f;
t3DModel g_3DModel6f;
t3DModel g_3DModel7f;
t3DModel g_3DModel8f;
t3DModel g_3DModel9f;
t3DModel g_3DModel10f;

// los modelos savage

t3DModel g_3DModel1g;
t3DModel g_3DModel2g;
t3DModel g_3DModel3g;


//Acceso a la estructura que almacena a Ene3a
t3DModel g_3DModel1h;
t3DModel g_3DModel2h;
t3DModel g_3DModel3h;
t3DModel g_3DModel4h;
t3DModel g_3DModel5h;


//Acceso a la estructura que almacena a Ene3b
t3DModel g_3DModel1i;
t3DModel g_3DModel2i;
t3DModel g_3DModel3i;
t3DModel g_3DModel4i;
t3DModel g_3DModel5i;


//Acceso a la estructura que almacena a chango
t3DModel g_3DModel1j;
t3DModel g_3DModel2j;
t3DModel g_3DModel3j;
t3DModel g_3DModel4j;
t3DModel g_3DModel5j;

//Acceso a la estructura que almacena a Ene1
t3DModel g_3DModel1k;
t3DModel g_3DModel2k;
t3DModel g_3DModel3k;

//Contenedor de texturas adicionales
CTga textura[30];

jerarquiaModelo player1modelo;	//Acceso a la estructura con las variables de cada pieza del modelo
const int maxKF1=3;				//Num. total de KeyFrames para la secuencia 1 (caminar)
FRAME KeyFrame1[maxKF1];		//Contenedor para almacenar cada keyframe de la secuencia 1

jerarquiaModelo enem3amodelo;
jerarquiaModelo enem3bmodelo;
jerarquiaModelo changmodelo;
jerarquiaModelo enem1modelo;
jerarquiaModelo enem2modelo;
jerarquiaModelo MJ6modelo;

bool play=false;//Bandera para iniciar la animación
int playIndex=0;//Auxiliar para leer la información del contenedor de keyframes
int tipoAnim=1; //Indicador del tipo de animación

CMultitexturas Multitext;

// Modelo de Bob
GLuint modelo1;
GLuint ene1;
GLuint noMJ6;
GLuint ene2;
GLuint ene3a;
GLuint ene3b;
GLuint cha;

// las listas savage
GLuint enemigo8L;

// Declara enteros para los modelos de Aru
GLuint modelo1aru;

//Constantes de iluminación y materiales
GLfloat LightPos[] = { 0.0f, 20.0f, 25.0f, 1.0f};		// Posición de la luz
GLfloat LightAmb[] = { 0.8f,  0.8f, 0.8f, 1.0f};			// Valores de la componente ambiente
GLfloat LightDif[] = { 0.9f,  0.9f, 0.9f, 1.0f};			// Valores de la componente difusa
GLfloat LightSpc[] = { 0.5f,  0.5f, 0.5f, 1.0f};			// Valores de la componente especular

CVector lightPosition;

//Variables auxiliares para animación de texturas
float aText1;
float aText2;

//Acceso a la clase CFont
CFont Font;

bool displayVolume = false;

//Acceso a la clase CShadow3DS
CShadow3DS objSh;

//Posición de la luz para el espacio de objeto del modelo1
CVector objectSpaceLightPosition1;
CVector objectSpaceLightPosition2;
CVector objectSpaceLightPosition3;
CVector objectSpaceLightPosition4;
CVector objectSpaceLightPosition5;
CVector objectSpaceLightPosition6;
CVector objectSpaceLightPosition7;
CVector objectSpaceLightPosition8;
CVector objectSpaceLightPosition9;

//Posición de la luz para el espacio de objeto de los modelos de Aru
CVector objectSpaceLightPosition1aru;
CVector objectSpaceLightPosition2aru;
CVector objectSpaceLightPosition3aru;
CVector objectSpaceLightPosition4aru;
CVector objectSpaceLightPosition5aru;
CVector objectSpaceLightPosition6aru;
CVector objectSpaceLightPosition7aru;
CVector objectSpaceLightPosition8aru;
CVector objectSpaceLightPosition9aru;

//Variables para el cálculo de transformaciones inversas para las sombras
typedef float GLvector4f[4];
typedef float GLmatrix16f[16];

//Variables para el control de fps
GLfloat Time1;
GLfloat Time2;
GLfloat DiffTime;
static int FPS = 0;

bool  framerate_limit = true;

int	  adjust=2;											// Speed Adjustment For Really Slow Video Cards

struct			 										// Create A Structure For The Timer Information
{
  __int64       frequency;								// Timer Frequency
  float         resolution;								// Timer Resolution
  unsigned long mm_timer_start;							// Multimedia Timer Start Value
  unsigned long mm_timer_elapsed;						// Multimedia Timer Elapsed Time
  bool			performance_timer;						// Using The Performance Timer?
  __int64       performance_timer_start;				// Performance Timer Start Value
  __int64       performance_timer_elapsed;				// Performance Timer Elapsed Time
} timer;												// Structure Is Named timer

int		steps[6]={ 1, 2, 4, 5, 10, 20 };				// Stepping Values For Slow Video Adjustment

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaracion de WndProc (Procedimiento de ventana)

void TimerInit(void)
{
	memset(&timer, 0, sizeof(timer));

	if (!QueryPerformanceFrequency((LARGE_INTEGER *) &timer.frequency))
	{
		timer.performance_timer	= FALSE;
		timer.mm_timer_start	= timeGetTime();
		timer.resolution		= 1.0f/1000.0f;
		timer.frequency			= 1000;
		timer.mm_timer_elapsed	= timer.mm_timer_start;
	}
	else
	{
		QueryPerformanceCounter((LARGE_INTEGER *) &timer.performance_timer_start);
		timer.performance_timer			= TRUE;
		timer.resolution				= (float) (((double)1.0f)/((double)timer.frequency));
		timer.performance_timer_elapsed	= timer.performance_timer_start;
	}
}

float TimerGetTime()
{
	__int64 time;

	if (timer.performance_timer)
	{
		QueryPerformanceCounter((LARGE_INTEGER *) &time);
		return ( (float) ( time - timer.performance_timer_start) * timer.resolution)*1000.0f;
	}
	else
	{
		return( (float) ( timeGetTime() - timer.mm_timer_start) * timer.resolution)*1000.0f;
	}
}

void CalculateFrameRate()
{
	static float framesPerSecond    = 0.0f;
    static float lastTime			= 0.0f;
	static char strFrameRate[50] = {0};

	float currentTime = GetTickCount() * 0.001f;				

	++framesPerSecond;

	if( currentTime - lastTime > 1.0f )
    {
	    lastTime = currentTime;
		
		FPS=int(framesPerSecond);
		
	    framesPerSecond = 0;
    }
}

void VMatMult(GLmatrix16f M, GLvector4f v)
{
	GLfloat res[4];
	res[0]=M[ 0]*v[0]+M[ 4]*v[1]+M[ 8]*v[2]+M[12]*v[3];
	res[1]=M[ 1]*v[0]+M[ 5]*v[1]+M[ 9]*v[2]+M[13]*v[3];
	res[2]=M[ 2]*v[0]+M[ 6]*v[1]+M[10]*v[2]+M[14]*v[3];
	res[3]=M[ 3]*v[0]+M[ 7]*v[1]+M[11]*v[2]+M[15]*v[3];
	v[0]=res[0];
	v[1]=res[1];
	v[2]=res[2];
	v[3]=res[3];
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Redimensiona e inicializa la ventana
{
	if (height==0)							// Para que no se presente una division por cero
	{
		height=1;							// la altura se iguala a 1
	}

	glViewport(0,0,width,height);					// Resetea el puerto de vista

	glMatrixMode(GL_PROJECTION);					// Selecciona la Matriz de Proyeccion
	glLoadIdentity();								// Resetea la Matriz de Proyeccion

	// Calcula el radio de aspecto o proporcion de medidas de la ventana
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,2000.0f);
	
	glMatrixMode(GL_MODELVIEW);							// Selecciona la Matriz de Vista de Modelo
	glLoadIdentity();									// Resetea la Matriz de Vista de Modelo

	glWidth=width;
	glHeight=height;
}

void CargaTexturas()
{
	textura[0].LoadTGA("Texturas/t1.tga");
}

void DescargaTexturas()
{
	textura[0].Elimina();
}

int CargaModelos()
{
	//Ene1
	if(!g_Load3ds.Load3DSFile(FILE_NAME1d, &g_3DModel1d, textureModel1d))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME2d, &g_3DModel2d, textureModel1d))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME3d, &g_3DModel3d, textureModel1d))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME4d, &g_3DModel4d, textureModel1d))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME5d, &g_3DModel5d, textureModel1d))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME6d, &g_3DModel6d, textureModel1d))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME7d, &g_3DModel7d, textureModel1d))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME8d, &g_3DModel8d, textureModel1d))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME9d, &g_3DModel9d, textureModel1d))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME10d, &g_3DModel10d, textureModel1d))
		return 0;

	// Mayra lol
	if(!g_Load3ds.Load3DSFile(FILE_NAME2e, &g_3DModel2e, textureModel2e))
		return 0;

	//agregar en CargaModelos() savage meh te la volaste XD
	if(!g_Load3ds.Load3DSFile(FILE_NAME1g, &g_3DModel1g, textureModel1g))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME2g, &g_3DModel2g, textureModel2g))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME3g, &g_3DModel3g, textureModel3g))
		return 0;

	// Carga modelos de Aru
	if(!g_Load3ds.Load3DSFile(FILE_NAME1aru, &g_3DModel1aru, textureModel1aru))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME2aru, &g_3DModel2aru, textureModel1aru))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME3aru, &g_3DModel3aru, textureModel1aru))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME4aru, &g_3DModel4aru, textureModel1aru))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME5aru, &g_3DModel5aru, textureModel1aru))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME6aru, &g_3DModel6aru, textureModel1aru))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME7aru, &g_3DModel7aru, textureModel1aru))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME8aru, &g_3DModel8aru, textureModel1aru))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME9aru, &g_3DModel9aru, textureModel1aru))
		return 0;

	//MJ6
	if(!g_Load3ds.Load3DSFile(FILE_NAME1f, &g_3DModel1f, textureModel1f))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME2f, &g_3DModel2f, textureModel1f))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME3f, &g_3DModel3f, textureModel1f))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME4f, &g_3DModel4f, textureModel1f))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME5f, &g_3DModel5f, textureModel1f))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME6f, &g_3DModel6f, textureModel1f))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME7f, &g_3DModel7f, textureModel1f))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME8f, &g_3DModel8f, textureModel1f))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME9f, &g_3DModel9f, textureModel1f))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME10f, &g_3DModel10f, textureModel1f))
		return 0;

	//Ene3a
	if(!g_Load3ds.Load3DSFile(FILE_NAME1h, &g_3DModel1h, textureModel1h))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME2h, &g_3DModel2h, textureModel2h))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME3h, &g_3DModel3h, textureModel3h))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME4h, &g_3DModel4h, textureModel4h))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME5h, &g_3DModel5h, textureModel5h))
		return 0;

	//Ene3b
	if(!g_Load3ds.Load3DSFile(FILE_NAME1i, &g_3DModel1i, textureModel1i))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME2i, &g_3DModel2i, textureModel2i))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME3i, &g_3DModel3i, textureModel3i))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME4i, &g_3DModel4i, textureModel4i))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME5i, &g_3DModel5i, textureModel5i))
		return 0;

	//chango
	if(!g_Load3ds.Load3DSFile(FILE_NAME1j, &g_3DModel1j, textureModel1j))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME2j, &g_3DModel2j, textureModel2j))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME3j, &g_3DModel3j, textureModel3j))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME4j, &g_3DModel4j, textureModel4j))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME5j, &g_3DModel5j, textureModel5j))
		return 0;
		
	//Ene2
	if(!g_Load3ds.Load3DSFile(FILE_NAME1k, &g_3DModel1k, textureModel1k))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME2k, &g_3DModel2k, textureModel1k))
		return 0;
//	if(!g_Load3ds.Load3DSFile(FILE_NAME3k, &g_3DModel3k, textureModel1k))
//		return 0;

	return TRUE;
}

void DescargaModelos()
{
	
	// descargamodelos savage
			
	g_Load3ds.UnLoad3DSFile(&g_3DModel1g, textureModel1g);
	g_Load3ds.UnLoad3DSFile(&g_3DModel2g, textureModel2g);
	g_Load3ds.UnLoad3DSFile(&g_3DModel3g, textureModel3g);

	// Mayra lol
	g_Load3ds.UnLoad3DSFile(&g_3DModel2e, textureModel2e);

	// Descarga los modelos de Aru
	g_Load3ds.UnLoad3DSFile(&g_3DModel1aru, textureModel1aru);
	g_Load3ds.UnLoad3DSFile(&g_3DModel2aru, textureModel1aru);
	g_Load3ds.UnLoad3DSFile(&g_3DModel3aru, textureModel1aru);
	g_Load3ds.UnLoad3DSFile(&g_3DModel4aru, textureModel1aru);
	g_Load3ds.UnLoad3DSFile(&g_3DModel5aru, textureModel1aru);
	g_Load3ds.UnLoad3DSFile(&g_3DModel6aru, textureModel1aru);
	g_Load3ds.UnLoad3DSFile(&g_3DModel7aru, textureModel1aru);
	g_Load3ds.UnLoad3DSFile(&g_3DModel8aru, textureModel1aru);
	g_Load3ds.UnLoad3DSFile(&g_3DModel9aru, textureModel1aru);
	//Ene1
	g_Load3ds.UnLoad3DSFile(&g_3DModel1d, textureModel1d);
	g_Load3ds.UnLoad3DSFile(&g_3DModel2d, textureModel1d);
	g_Load3ds.UnLoad3DSFile(&g_3DModel3d, textureModel1d);
	g_Load3ds.UnLoad3DSFile(&g_3DModel4d, textureModel1d);
	g_Load3ds.UnLoad3DSFile(&g_3DModel5d, textureModel1d);
	g_Load3ds.UnLoad3DSFile(&g_3DModel6d, textureModel1d);
	g_Load3ds.UnLoad3DSFile(&g_3DModel7d, textureModel1d);
	g_Load3ds.UnLoad3DSFile(&g_3DModel8d, textureModel1d);
	g_Load3ds.UnLoad3DSFile(&g_3DModel9d, textureModel1d);
	g_Load3ds.UnLoad3DSFile(&g_3DModel10d, textureModel1d);

	//MJ6
	g_Load3ds.UnLoad3DSFile(&g_3DModel1f, textureModel1f);
	g_Load3ds.UnLoad3DSFile(&g_3DModel2f, textureModel1f);
	g_Load3ds.UnLoad3DSFile(&g_3DModel3f, textureModel1f);
	g_Load3ds.UnLoad3DSFile(&g_3DModel4f, textureModel1f);
	g_Load3ds.UnLoad3DSFile(&g_3DModel5f, textureModel1f);
	g_Load3ds.UnLoad3DSFile(&g_3DModel6f, textureModel1f);
	g_Load3ds.UnLoad3DSFile(&g_3DModel7f, textureModel1f);
	g_Load3ds.UnLoad3DSFile(&g_3DModel8f, textureModel1f);
	g_Load3ds.UnLoad3DSFile(&g_3DModel9f, textureModel1f);
	g_Load3ds.UnLoad3DSFile(&g_3DModel10f, textureModel1f);

	//Ene3a
	g_Load3ds.UnLoad3DSFile(&g_3DModel1h, textureModel1h);
	g_Load3ds.UnLoad3DSFile(&g_3DModel2h, textureModel2h);
	g_Load3ds.UnLoad3DSFile(&g_3DModel3h, textureModel3h);
	g_Load3ds.UnLoad3DSFile(&g_3DModel4h, textureModel4h);
	g_Load3ds.UnLoad3DSFile(&g_3DModel5h, textureModel5h);

	//Ene3b
	g_Load3ds.UnLoad3DSFile(&g_3DModel1i, textureModel1i);
	g_Load3ds.UnLoad3DSFile(&g_3DModel2i, textureModel2i);
	g_Load3ds.UnLoad3DSFile(&g_3DModel3i, textureModel3i);
	g_Load3ds.UnLoad3DSFile(&g_3DModel4i, textureModel4i);
	g_Load3ds.UnLoad3DSFile(&g_3DModel5i, textureModel5i);

	//chango
	g_Load3ds.UnLoad3DSFile(&g_3DModel1j, textureModel1j);
	g_Load3ds.UnLoad3DSFile(&g_3DModel2j, textureModel2j);
	g_Load3ds.UnLoad3DSFile(&g_3DModel3j, textureModel3j);
	g_Load3ds.UnLoad3DSFile(&g_3DModel4j, textureModel4j);
	g_Load3ds.UnLoad3DSFile(&g_3DModel5j, textureModel5j);

	//ene2
	//g_Load3ds.UnLoad3DSFile(&g_3DModel1j, textureModel1j);
	//g_Load3ds.UnLoad3DSFile(&g_3DModel2j, textureModel2j);
	//g_Load3ds.UnLoad3DSFile(&g_3DModel3j, textureModel3j);
}

void IniSombraVolumen()
{
	//Establece la conectividad entre caras
	objSh.EstableceConectividad(&g_3DModel1c);
	objSh.calculaPlano(&g_3DModel1c);

	objSh.EstableceConectividad(&g_3DModel2c);
	objSh.calculaPlano(&g_3DModel2c);

	objSh.EstableceConectividad(&g_3DModel3c);
	objSh.calculaPlano(&g_3DModel3c);

	objSh.EstableceConectividad(&g_3DModel4c);
	objSh.calculaPlano(&g_3DModel4c);

	objSh.EstableceConectividad(&g_3DModel5c);
	objSh.calculaPlano(&g_3DModel5c);

	objSh.EstableceConectividad(&g_3DModel6c);
	objSh.calculaPlano(&g_3DModel6c);

	objSh.EstableceConectividad(&g_3DModel7c);
	objSh.calculaPlano(&g_3DModel7c);

	objSh.EstableceConectividad(&g_3DModel8c);
	objSh.calculaPlano(&g_3DModel8c);

	objSh.EstableceConectividad(&g_3DModel9c);
	objSh.calculaPlano(&g_3DModel9c);
			
}

void CreaListas()
{
	//Ene1
	ene1=glGenLists(10);
	//Ene2
	ene2=glGenLists(2);
	//MJ6
	noMJ6=glGenLists(10);
	//Ene3a
	ene3a=glGenLists(5);
	//Ene3a
	ene3b=glGenLists(5);
	//chango
	cha=glGenLists(5);
	// en crea listas savage
	enemigo8L=glGenLists(3);
	glNewList(enemigo8L+0,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel1g, textureModel1g, 1);
	glEndList();

	glNewList(enemigo8L+1,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel2g, textureModel2g, 1);
	glEndList();

	glNewList(enemigo8L+2,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel3g, textureModel3g, 1);
	glEndList();

	// Crea listas para Aru
	modelo1aru = glGenLists(9);

	glNewList(modelo1aru+0,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel1aru, textureModel1aru, 1);
	glEndList();

	glNewList(modelo1aru+1,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel2aru, textureModel1aru, 1);
	glEndList();

	glNewList(modelo1aru+2,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel3aru, textureModel1aru, 1);
	glEndList();

	glNewList(modelo1aru+3,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel4aru, textureModel1aru, 1);
	glEndList();

	glNewList(modelo1aru+4,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel5aru, textureModel1aru, 1);
	glEndList();

	glNewList(modelo1aru+5,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel6aru, textureModel1aru, 1);
	glEndList();

	glNewList(modelo1aru+6,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel7aru, textureModel1aru, 1);
	glEndList();

	glNewList(modelo1aru+7,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel8aru, textureModel1aru, 1);
	glEndList();

	glNewList(modelo1aru+8,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel9aru, textureModel1aru, 1);
	glEndList();
	//Ene1
	glNewList(ene1+0,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel1d, textureModel1d, 1);
	glEndList();

	glNewList(ene1+1,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel2d, textureModel1d, 1);
	glEndList();

	glNewList(ene1+2,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel3d, textureModel1d, 1);
	glEndList();

	glNewList(ene1+3,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel4d, textureModel1d, 1);
	glEndList();

	glNewList(ene1+4,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel5d, textureModel1d, 1);
	glEndList();

	glNewList(ene1+5,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel6d, textureModel1d, 1);
	glEndList();

	glNewList(ene1+6,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel7d, textureModel1d, 1);
	glEndList();

	glNewList(ene1+7,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel8d, textureModel1d, 1);
	glEndList();

	glNewList(ene1+8,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel9d, textureModel1d, 1);
	glEndList();

	glNewList(ene1+9,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel10d, textureModel1d, 1);
	glEndList();

	//MJ6
	glNewList(noMJ6+0,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel1f, textureModel1f, 1);
	glEndList();

	glNewList(noMJ6+1,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel2f, textureModel1f, 1);
	glEndList();

	glNewList(noMJ6+2,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel3f, textureModel1f, 1);
	glEndList();

	glNewList(noMJ6+3,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel4f, textureModel1f, 1);
	glEndList();

	glNewList(noMJ6+4,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel5f, textureModel1f, 1);
	glEndList();

	glNewList(noMJ6+5,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel6f, textureModel1f, 1);
	glEndList();

	glNewList(noMJ6+6,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel7f, textureModel1f, 1);
	glEndList();

	glNewList(noMJ6+7,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel8f, textureModel1f, 1);
	glEndList();

	glNewList(noMJ6+8,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel9f, textureModel1f, 1);
	glEndList();

	glNewList(noMJ6+9,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel10f, textureModel1f, 1);
	glEndList();

	//Ene3a
	glNewList(ene3a+0,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel1h, textureModel1h, 1);
	glEndList();

	glNewList(ene3a+1,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel2h, textureModel2h, 1);
	glEndList();

	glNewList(ene3a+2,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel3h, textureModel3h, 1);
	glEndList();

	glNewList(ene3a+3,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel4h, textureModel4h, 1);
	glEndList();

	glNewList(ene3a+4,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel5h, textureModel5h, 1);
	glEndList();

	//Ene3b
	glNewList(ene3b+0,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel1i, textureModel1i, 1);
	glEndList();

	glNewList(ene3b+1,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel2i, textureModel2i, 1);
	glEndList();

	glNewList(ene3b+2,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel3i, textureModel3i, 1);
	glEndList();

	glNewList(ene3b+3,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel4i, textureModel4i, 1);
	glEndList();

	glNewList(ene3b+4,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel5i, textureModel5i, 1);
	glEndList();

	//chango
	glNewList(cha+0,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel1j, textureModel1j, 1);
	glEndList();

	glNewList(cha+1,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel2j, textureModel2j, 1);
	glEndList();

	glNewList(cha+2,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel3j, textureModel3j, 1);
	glEndList();

	glNewList(cha+3,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel4j, textureModel4j, 1);
	glEndList();

	glNewList(cha+4,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel5j, textureModel5j, 1);
	glEndList();

	//Ene2
	glNewList(ene2+0,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel1k, textureModel1k, 1);
	glEndList();

	glNewList(ene2+1,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel2k, textureModel2k, 1);
	glEndList();

//	glNewList(ene2+2,GL_COMPILE);
//		g_Load3ds.Render3DSFile(&g_3DModel3k, textureModel3k, 1);
//	glEndList();

}

void DestruyeListas()
{

	// Borra listas de Aru
	glDeleteLists(modelo1aru,9);
	// Borra listas fahl
	glDeleteLists(ene1,10);
	glDeleteLists(ene2,2);
	glDeleteLists(noMJ6,10);

	glDeleteLists(ene3a,5);
	glDeleteLists(ene3b,5);
	glDeleteLists(cha,5);

}

void InicializaParametrosdeControl()
{

	// Inicializa parametros de control para Aru
	//Esta función establece los parámetros como velocidad del objeto y distancia de la cámara así como la posición y dirección iniciales
	player1.visible = true;
	player1.VelocidadObj = 0.2f;
	player1.DistanciaCam = 20.0f;

	player1.CamaraPosAlt = 10.0f;	//Posición en y de la cámara (altura a la que se situa la cámara)
	player1.CamaraObjAlt = 6.4f;	//Posición en y del objetivo de la cámara (altura a la que ve la cámara)
	player1.AngDir = 90.0f;		//Este ángulo inicial hace que la dirección inicial sea paralela al eje Z y con sentido negativo
	player1.AngObj = 0.0f;		//Este valor se elige dependiendo de la orientación con la que aparece el modelo en la escena al dibujarlo
								//sin aplicarle ninguna transformación (hacia adonde está volteando). Se elige un ángulo tal que al aplicarle
								//una rotación inicial con respecto al eje Y esté viendo hacia la misma dirección que la definida por AngDir
	
	player1.PosicionObj = CVector( 220.0f, 4.0f, -45.0f); //Esta es la posición inicial del objeto en la escena
	player1.Direccion.x = cosf( player1.AngDir * PI / 180.0f); //Dirección inicial definida por el ángulo inicial AngDir (x=cos(AngDir), y=0.0, z=sen(AngDir))
	player1.Direccion.y = 0.0f;
	player1.Direccion.z = sinf( player1.AngDir * PI / 180.0f);   
	player1.PosicionCam = CVector( player1.PosicionObj.x, player1.PosicionObj.y + player1.CamaraPosAlt, player1.PosicionObj.z +player1.DistanciaCam ); //Posición inicial de la cámara a [DistanciaCam] unidades detrás del objeto
	player1.ObjetivoCam = player1.PosicionObj;		//La cámara ve siempre al objeto
	player1.ObjetivoCam.y = player1.CamaraObjAlt;		//Para que no vea a los "pies" del objeto (personaje)

	player1.Dir=0;
	player1.DirAnt=0;

	player1.escalaX=0.4f;
	player1.escalaY=0.4f;
	player1.escalaZ=0.4f;
	
	player1.CamaraObjAltE=0.0f;

	// en inicializa parametro de control savage
	enemigo8.visible=true;
	enemigo8.VelocidadObj=0.4f;
	enemigo8.DistanciaCam=10.0f;

	enemigo8.CamaraPosAlt=0.0f;	//Posición en y de la cámara (altura a la que se situa la cámara)
	enemigo8.CamaraObjAlt=4.0f;	//Posición en y del objetivo de la cámara (altura a la que ve la cámara)
	enemigo8.AngDir=-PI;		//Este ángulo inicial hace que la dirección inicial sea paralela al eje Z y con sentido negativo
	enemigo8.AngObj=PI;		//Este valor se elige dependiendo de la orientación con la que aparece el modelo en la escena al dibujarlo
								//sin aplicarle ninguna transformación (hacia adonde está volteando). Se elige un ángulo tal que al aplicarle
								//una rotación inicial con respecto al eje Y esté viendo hacia la misma dirección que la definida por AngDir
	
	enemigo8.PosicionObj=CVector(-45.0f, 0.0f, 0.0f); //Esta es la posición inicial del objeto en la escena
	enemigo8.Direccion.x=(float)cos(enemigo8.AngDir); //Dirección inicial definida por el ángulo inicial AngDir (x=cos(AngDir), y=0.0, z=sen(AngDir))
	enemigo8.Direccion.y=0.0f;
	enemigo8.Direccion.z=(float)sin(enemigo8.AngDir);   
	enemigo8.PosicionCam=CVector(0.0f, enemigo8.CamaraPosAlt, 10.0f); //Posición inicial de la cámara a [DistanciaCam] unidades detrás del objeto
	enemigo8.ObjetivoCam=enemigo8.PosicionObj;		//La cámara ve siempre al objeto
	enemigo8.ObjetivoCam.y=enemigo8.CamaraObjAlt;		//Para que no vea a los "pies" del objeto (personaje)

	enemigo8.Dir=0;
	enemigo8.DirAnt=0;
	enemigo8.escalaX=0.1f;
	enemigo8.escalaY=0.1f;
	enemigo8.escalaZ=0.1f;
	
	// Enem1
	enem1.visible=true;
	enem1.VelocidadObj=0.2f;
	enem1.DistanciaCam=10.0f;

	enem1.CamaraPosAlt=5.0f;	//Posición en y de la cámara (altura a la que se situa la cámara)
	enem1.CamaraObjAlt=4.0f;	//Posición en y del objetivo de la cámara (altura a la que ve la cámara)
	enem1.AngDir=90.0f;		//Este ángulo inicial hace que la dirección inicial sea paralela al eje Z y con sentido negativo
	enem1.AngObj=0.0f;		//Este valor se elige dependiendo de la orientación con la que aparece el modelo en la escena al dibujarlo
								//sin aplicarle ninguna transformación (hacia adonde está volteando). Se elige un ángulo tal que al aplicarle
								//una rotación inicial con respecto al eje Y esté viendo hacia la misma dirección que la definida por AngDir
	
	enem1.PosicionObj=CVector(5.0f, 0.0f, 0.0f); //Esta es la posición inicial del objeto en la escena
	enem1.Direccion.x=(float)cos(player1.AngDir*PI/180.0f); //Dirección inicial definida por el ángulo inicial AngDir (x=cos(AngDir), y=0.0, z=sen(AngDir))
	enem1.Direccion.y=0.0f;
	enem1.Direccion.z=(float)sin(player1.AngDir*PI/180.0f);   
	enem1.PosicionCam=CVector(0.0f, player1.CamaraPosAlt, 10.0f); //Posición inicial de la cámara a [DistanciaCam] unidades detrás del objeto
	enem1.ObjetivoCam=player1.PosicionObj;		//La cámara ve siempre al objeto
	enem1.ObjetivoCam.y=player1.CamaraObjAlt;		//Para que no vea a los "pies" del objeto (personaje)

	enem1.Dir=0;
	enem1.DirAnt=0;

	enem1.escalaX=0.4f;
	enem1.escalaY=0.4f;
	enem1.escalaZ=0.4f;

	enem1.CamaraObjAltE=0.0f;
	
	// Enem2
	enem2.visible=true;
	enem2.VelocidadObj=0.2f;
	enem2.DistanciaCam=10.0f;

	enem2.CamaraPosAlt=5.0f;	//Posición en y de la cámara (altura a la que se situa la cámara)
	enem2.CamaraObjAlt=4.0f;	//Posición en y del objetivo de la cámara (altura a la que ve la cámara)
	enem2.AngDir=90.0f;		//Este ángulo inicial hace que la dirección inicial sea paralela al eje Z y con sentido negativo
	enem2.AngObj=0.0f;		//Este valor se elige dependiendo de la orientación con la que aparece el modelo en la escena al dibujarlo
								//sin aplicarle ninguna transformación (hacia adonde está volteando). Se elige un ángulo tal que al aplicarle
								//una rotación inicial con respecto al eje Y esté viendo hacia la misma dirección que la definida por AngDir
	
	enem2.PosicionObj=CVector(10.0f, 0.0f, 0.0f); //Esta es la posición inicial del objeto en la escena
	enem2.Direccion.x=(float)cos(player1.AngDir*PI/180.0f); //Dirección inicial definida por el ángulo inicial AngDir (x=cos(AngDir), y=0.0, z=sen(AngDir))
	enem2.Direccion.y=0.0f;
	enem2.Direccion.z=(float)sin(player1.AngDir*PI/180.0f);   
	enem2.PosicionCam=CVector(0.0f, player1.CamaraPosAlt, 10.0f); //Posición inicial de la cámara a [DistanciaCam] unidades detrás del objeto
	enem2.ObjetivoCam=player1.PosicionObj;		//La cámara ve siempre al objeto
	enem2.ObjetivoCam.y=player1.CamaraObjAlt;		//Para que no vea a los "pies" del objeto (personaje)

	enem2.Dir=0;
	enem2.DirAnt=0;

	enem2.escalaX=0.2f;
	enem2.escalaY=0.2f;
	enem2.escalaZ=0.2f;

	enem2.CamaraObjAltE=0.0f;
	
	//MJ6

	MJ6.visible=true;

	MJ6.VelocidadObj=0.2f;
	MJ6.DistanciaCam=10.0f;

	MJ6.CamaraPosAlt=5.0f;	//Posición en y de la cámara (altura a la que se situa la cámara)
	MJ6.CamaraObjAlt=4.0f;	//Posición en y del objetivo de la cámara (altura a la que ve la cámara)
	MJ6.AngDir=90.0f;		//Este ángulo inicial hace que la dirección inicial sea paralela al eje Z y con sentido negativo
	MJ6.AngObj=0.0f;		//Este valor se elige dependiendo de la orientación con la que aparece el modelo en la escena al dibujarlo
								//sin aplicarle ninguna transformación (hacia adonde está volteando). Se elige un ángulo tal que al aplicarle
								//una rotación inicial con respecto al eje Y esté viendo hacia la misma dirección que la definida por AngDir
	
	MJ6.PosicionObj=CVector(-5.0f, 5.0f, 0.0f); //Esta es la posición inicial del objeto en la escena
	MJ6.Direccion.x=(float)cos(player1.AngDir*PI/180.0f); //Dirección inicial definida por el ángulo inicial AngDir (x=cos(AngDir), y=0.0, z=sen(AngDir))
	MJ6.Direccion.y=0.0f;
	MJ6.Direccion.z=(float)sin(player1.AngDir*PI/180.0f);   
	MJ6.PosicionCam=CVector(0.0f, player1.CamaraPosAlt, 10.0f); //Posición inicial de la cámara a [DistanciaCam] unidades detrás del objeto
	MJ6.ObjetivoCam=player1.PosicionObj;		//La cámara ve siempre al objeto
	MJ6.ObjetivoCam.y=player1.CamaraObjAlt;		//Para que no vea a los "pies" del objeto (personaje)

	MJ6.Dir=0;
	MJ6.DirAnt=0;

	MJ6.escalaX=0.4f;
	MJ6.escalaY=0.4f;
	MJ6.escalaZ=0.4f;

	MJ6.CamaraObjAltE=0.0f;

	// enem3a
	enem3a.visible=true;
	enem3a.VelocidadObj=0.2f;
	enem3a.DistanciaCam=10.0f;

	enem3a.CamaraPosAlt=5.0f;	//Posición en y de la cámara (altura a la que se situa la cámara)
	enem3a.CamaraObjAlt=4.0f;	//Posición en y del objetivo de la cámara (altura a la que ve la cámara)
	enem3a.AngDir=90.0f;		//Este ángulo inicial hace que la dirección inicial sea paralela al eje Z y con sentido negativo
	enem3a.AngObj=0.0f;		//Este valor se elige dependiendo de la orientación con la que aparece el modelo en la escena al dibujarlo
								//sin aplicarle ninguna transformación (hacia adonde está volteando). Se elige un ángulo tal que al aplicarle
								//una rotación inicial con respecto al eje Y esté viendo hacia la misma dirección que la definida por AngDir
	
	enem3a.PosicionObj=CVector(-15.0f, 0.0f, 10.0f); //Esta es la posición inicial del objeto en la escena
	enem3a.Direccion.x=(float)cos(player1.AngDir*PI/180.0f); //Dirección inicial definida por el ángulo inicial AngDir (x=cos(AngDir), y=0.0, z=sen(AngDir))
	enem3a.Direccion.y=0.0f;
	enem3a.Direccion.z=(float)sin(player1.AngDir*PI/180.0f);   
	enem3a.PosicionCam=CVector(0.0f, player1.CamaraPosAlt, 10.0f); //Posición inicial de la cámara a [DistanciaCam] unidades detrás del objeto
	enem3a.ObjetivoCam=player1.PosicionObj;		//La cámara ve siempre al objeto
	enem3a.ObjetivoCam.y=player1.CamaraObjAlt;		//Para que no vea a los "pies" del objeto (personaje)

	enem3a.Dir=0;
	enem3a.DirAnt=0;

	enem3a.escalaX=0.4f;
	enem3a.escalaY=0.4f;
	enem3a.escalaZ=0.4f;

	enem3a.CamaraObjAltE=0.0f;

	// enem3b
	enem3b.visible=true;
	enem3b.VelocidadObj=0.2f;
	enem3b.DistanciaCam=10.0f;

	enem3b.CamaraPosAlt=5.0f;	//Posición en y de la cámara (altura a la que se situa la cámara)
	enem3b.CamaraObjAlt=4.0f;	//Posición en y del objetivo de la cámara (altura a la que ve la cámara)
	enem3b.AngDir=90.0f;		//Este ángulo inicial hace que la dirección inicial sea paralela al eje Z y con sentido negativo
	enem3b.AngObj=270.0f;		//Este valor se elige dependiendo de la orientación con la que aparece el modelo en la escena al dibujarlo
								//sin aplicarle ninguna transformación (hacia adonde está volteando). Se elige un ángulo tal que al aplicarle
								//una rotación inicial con respecto al eje Y esté viendo hacia la misma dirección que la definida por AngDir
	
	enem3b.PosicionObj=CVector(-19.0f, 0.0f, 10.0f); //Esta es la posición inicial del objeto en la escena
	enem3b.Direccion.x=(float)cos(player1.AngDir*PI/180.0f); //Dirección inicial definida por el ángulo inicial AngDir (x=cos(AngDir), y=0.0, z=sen(AngDir))
	enem3b.Direccion.y=0.0f;
	enem3b.Direccion.z=(float)sin(player1.AngDir*PI/180.0f);   
	enem3b.PosicionCam=CVector(0.0f, player1.CamaraPosAlt, 10.0f); //Posición inicial de la cámara a [DistanciaCam] unidades detrás del objeto
	enem3b.ObjetivoCam=player1.PosicionObj;		//La cámara ve siempre al objeto
	enem3b.ObjetivoCam.y=player1.CamaraObjAlt;		//Para que no vea a los "pies" del objeto (personaje)

	enem3b.Dir=0;
	enem3b.DirAnt=0;

	enem3b.escalaX=0.4f;
	enem3b.escalaY=0.4f;
	enem3b.escalaZ=0.4f;

	enem3b.CamaraObjAltE=0.0f;


	// chang
	chang.visible=true;
	chang.VelocidadObj=0.2f;
	chang.DistanciaCam=10.0f;

	chang.CamaraPosAlt=5.0f;	//Posición en y de la cámara (altura a la que se situa la cámara)
	chang.CamaraObjAlt=4.0f;	//Posición en y del objetivo de la cámara (altura a la que ve la cámara)
	chang.AngDir=90.0f;		//Este ángulo inicial hace que la dirección inicial sea paralela al eje Z y con sentido negativo
	chang.AngObj=0.0f;		//Este valor se elige dependiendo de la orientación con la que aparece el modelo en la escena al dibujarlo
								//sin aplicarle ninguna transformación (hacia adonde está volteando). Se elige un ángulo tal que al aplicarle
								//una rotación inicial con respecto al eje Y esté viendo hacia la misma dirección que la definida por AngDir
	
	chang.PosicionObj=CVector(-30.0f, 0.0f, 5.0f); //Esta es la posición inicial del objeto en la escena
	chang.Direccion.x=(float)cos(player1.AngDir*PI/180.0f); //Dirección inicial definida por el ángulo inicial AngDir (x=cos(AngDir), y=0.0, z=sen(AngDir))
	chang.Direccion.y=0.0f;
	chang.Direccion.z=(float)sin(player1.AngDir*PI/180.0f);   
	chang.PosicionCam=CVector(0.0f, player1.CamaraPosAlt, 10.0f); //Posición inicial de la cámara a [DistanciaCam] unidades detrás del objeto
	chang.ObjetivoCam=player1.PosicionObj;		//La cámara ve siempre al objeto
	chang.ObjetivoCam.y=player1.CamaraObjAlt;		//Para que no vea a los "pies" del objeto (personaje)

	chang.Dir=0;
	chang.DirAnt=0;

	chang.escalaX=0.15f;
	chang.escalaY=0.15f;
	chang.escalaZ=0.15f;

	chang.CamaraObjAltE=0.0f;
}

void InicializaAnim()
{
	//Se inicializan las variables de la secuencia 1
	for(int i=0; i<maxKF1; i++)
	{
		KeyFrame1[i].Angt1=0.0f;
		KeyFrame1[i].Angt2=0.0f;
		KeyFrame1[i].Angc1=0.0f;
		KeyFrame1[i].Angc2=0.0f;
		KeyFrame1[i].Angbi1=0.0f;
		KeyFrame1[i].Angbi2=0.0f;
		KeyFrame1[i].Angbib=0.0f;
		KeyFrame1[i].Angbd1=0.0f;
		KeyFrame1[i].Angbd2=0.0f;
		KeyFrame1[i].Angbdb=0.0f;
		KeyFrame1[i].Angpizq=0.0f;
		KeyFrame1[i].Angpizqb=0.0f;
		KeyFrame1[i].Angpder=0.0f;
		KeyFrame1[i].Angpderb=0.0f;
		KeyFrame1[i].Angpi=0.0f;
		KeyFrame1[i].Angpd=0.0f;
		KeyFrame1[i].Xtor=0.0f;
		KeyFrame1[i].Ytor=0.0f;
		KeyFrame1[i].Ztor=0.0f;

		KeyFrame1[i].incAngt1=false;
		KeyFrame1[i].incAngt1=false;
		KeyFrame1[i].incAngc1=false;
		KeyFrame1[i].incAngc2=false;
		KeyFrame1[i].incAngbi1=false;
		KeyFrame1[i].incAngbi2=false;
		KeyFrame1[i].incAngbib=false;
		KeyFrame1[i].incAngbd1=false;
		KeyFrame1[i].incAngbd2=false;
		KeyFrame1[i].incAngbdb=false;
		KeyFrame1[i].incAngpizq=false;
		KeyFrame1[i].incAngpizqb=false;
		KeyFrame1[i].incAngpder=false;
		KeyFrame1[i].incAngpderb=false;
		KeyFrame1[i].incAngpi=false;
		KeyFrame1[i].incAngpd=false;
		KeyFrame1[i].incXtor=false;
		KeyFrame1[i].incYtor=false;
		KeyFrame1[i].incZtor=false;
	}

	player1modelo.Angt1=0.0f;
	player1modelo.Angt2=0.0f;
	player1modelo.Angc1=0.0f;
	player1modelo.Angc2=0.0f;
	player1modelo.Angbi1=0.0f;
	player1modelo.Angbi2=0.0f;
	player1modelo.Angbib=0.0f;
	player1modelo.Angbd1=0.0f;
	player1modelo.Angbd2=0.0f;
	player1modelo.Angbdb=0.0f;
	player1modelo.Angpizq=0.0f;
	player1modelo.Angpizqb=0.0f;
	player1modelo.Angpder=0.0f;
	player1modelo.Angpderb=0.0f;
	player1modelo.Angpi=0.0f;
	player1modelo.Angpd=0.0f;
	player1modelo.Xtor=0.0f;
	player1modelo.Ytor=0.0f;
	player1modelo.Ztor=0.0f;
	
	enem1modelo.Angt1=0.0f;
	enem1modelo.Angt2=0.0f;
	enem1modelo.Angc1=0.0f;
	enem1modelo.Angc2=0.0f;
	enem1modelo.Angbi1=0.0f;
	enem1modelo.Angbi2=0.0f;
	enem1modelo.Angbib=0.0f;
	enem1modelo.Angbd1=0.0f;
	enem1modelo.Angbd2=0.0f;
	enem1modelo.Angbdb=0.0f;
	enem1modelo.Angpizq=0.0f;
	enem1modelo.Angpizqb=0.0f;
	enem1modelo.Angpder=0.0f;
	enem1modelo.Angpderb=0.0f;
	enem1modelo.Angpi=0.0f;
	enem1modelo.Angpd=0.0f;
	enem1modelo.Xtor=0.0f;
	enem1modelo.Ytor=0.0f;
	enem1modelo.Ztor=0.0f;

	enem2modelo.Angt1=0.0f;
	enem2modelo.Angt2=0.0f;
	enem2modelo.Angc1=0.0f;
	enem2modelo.Angc2=0.0f;
	enem2modelo.Angbi1=0.0f;
	enem2modelo.Angbi2=0.0f;
	enem2modelo.Angbib=0.0f;
	enem2modelo.Angbd1=0.0f;
	enem2modelo.Angbd2=0.0f;
	enem2modelo.Angbdb=0.0f;
	enem2modelo.Xtor=0.0f;
	enem2modelo.Ytor=0.0f;
	enem2modelo.Ztor=0.0f;

	MJ6modelo.Angt1=0.0f;
	MJ6modelo.Angt2=0.0f;
	MJ6modelo.Angc1=0.0f;
	MJ6modelo.Angc2=0.0f;
	MJ6modelo.Angbi1=0.0f;
	MJ6modelo.Angbi2=0.0f;
	MJ6modelo.Angbib=0.0f;
	MJ6modelo.Angbd1=0.0f;
	MJ6modelo.Angbd2=0.0f;
	MJ6modelo.Angbdb=0.0f;
	MJ6modelo.Angpizq=0.0f;
	MJ6modelo.Angpizqb=0.0f;
	MJ6modelo.Angpder=0.0f;
	MJ6modelo.Angpderb=0.0f;
	MJ6modelo.Angpi=0.0f;
	MJ6modelo.Angpd=0.0f;
	MJ6modelo.Xtor=0.0f;
	MJ6modelo.Ytor=0.0f;
	MJ6modelo.Ztor=0.0f;

	enem3amodelo.Angt1=0.0f;
	enem3amodelo.Angt2=0.0f;
	enem3amodelo.Angc1=0.0f;
	enem3amodelo.Angc2=0.0f;
	enem3amodelo.Angbi1=0.0f;
	enem3amodelo.Angbi2=0.0f;
	enem3amodelo.Angbib=0.0f;
	enem3amodelo.Angbd1=0.0f;
	enem3amodelo.Angbd2=0.0f;
	enem3amodelo.Angbdb=0.0f;
	enem3amodelo.Angpizq=0.0f;
	enem3amodelo.Angpizqb=0.0f;
	enem3amodelo.Angpder=0.0f;
	enem3amodelo.Angpderb=0.0f;
	enem3amodelo.Angpi=0.0f;
	enem3amodelo.Angpd=0.0f;
	enem3amodelo.Xtor=0.0f;
	enem3amodelo.Ytor=0.0f;
	enem3amodelo.Ztor=0.0f;

	enem3bmodelo.Angt1=0.0f;
	enem3bmodelo.Angt2=0.0f;
	enem3bmodelo.Angc1=0.0f;
	enem3bmodelo.Angc2=0.0f;
	enem3bmodelo.Angbi1=0.0f;
	enem3bmodelo.Angbi2=0.0f;
	enem3bmodelo.Angbib=0.0f;
	enem3bmodelo.Angbd1=0.0f;
	enem3bmodelo.Angbd2=0.0f;
	enem3bmodelo.Angbdb=0.0f;
	enem3bmodelo.Angpizq=0.0f;
	enem3bmodelo.Angpizqb=0.0f;
	enem3bmodelo.Angpder=0.0f;
	enem3bmodelo.Angpderb=0.0f;
	enem3bmodelo.Angpi=0.0f;
	enem3bmodelo.Angpd=0.0f;
	enem3bmodelo.Xtor=0.0f;
	enem3bmodelo.Ytor=0.0f;
	enem3bmodelo.Ztor=0.0f;

	changmodelo.Angt1=0.0f;
	changmodelo.Angt2=0.0f;
	changmodelo.Angc1=0.0f;
	changmodelo.Angc2=0.0f;
	changmodelo.Angbi1=0.0f;
	changmodelo.Angbi2=0.0f;
	changmodelo.Angbib=0.0f;
	changmodelo.Angbd1=0.0f;
	changmodelo.Angbd2=0.0f;
	changmodelo.Angbdb=0.0f;
	changmodelo.Angpizq=0.0f;
	changmodelo.Angpizqb=0.0f;
	changmodelo.Angpder=0.0f;
	changmodelo.Angpderb=0.0f;
	changmodelo.Angpi=0.0f;
	changmodelo.Angpd=0.0f;
	changmodelo.Xtor=0.0f;
	changmodelo.Ytor=0.0f;
	changmodelo.Ztor=0.0f;
}

void DatosAnimacion()
{
	//Secuencia 1
	KeyFrame1[0].Angt1=-5.0f;
	KeyFrame1[0].Angt2=0.0f;
	KeyFrame1[0].Angc1=-25.0f;
	KeyFrame1[0].Angc2=0.0f;
	KeyFrame1[0].Angbi1=-5.0f;
	KeyFrame1[0].Angbi2=0.0f;
	KeyFrame1[0].Angbib=-15.0f;
	KeyFrame1[0].Angbd1=5.0f;
	KeyFrame1[0].Angbd2=0.0f;
	KeyFrame1[0].Angbdb=-40.0f;
	KeyFrame1[0].Angpizq=20.0f;
	KeyFrame1[0].Angpder=-30.0f;
	KeyFrame1[0].Angpizqb=15.0f;
	KeyFrame1[0].Angpderb=10.0f;
	KeyFrame1[0].Angpi=0.0f;
	KeyFrame1[0].Angpd=0.0f;
	KeyFrame1[0].Xtor=0.0f;
	KeyFrame1[0].Ytor=0.0f;
	KeyFrame1[0].Ztor=0.0f;

	KeyFrame1[1].Angt1=-5.0f;
	KeyFrame1[1].Angt2=0.0f;
	KeyFrame1[1].Angc1=-25.0f;
	KeyFrame1[1].Angc2=0.0f;
	KeyFrame1[1].Angbi1=10.0f;
	KeyFrame1[1].Angbi2=0.0f;
	KeyFrame1[1].Angbib=-40.0f;
	KeyFrame1[1].Angbd1=-10.0f;
	KeyFrame1[1].Angbd2=0.0f;
	KeyFrame1[1].Angbdb=-15.0f;
	KeyFrame1[1].Angpizq=-30.0f;
	KeyFrame1[1].Angpder=20.0f;
	KeyFrame1[1].Angpizqb=20.0f;
	KeyFrame1[1].Angpderb=15.0f;
	KeyFrame1[1].Angpi=0.0f;
	KeyFrame1[1].Angpd=0.0f;
	KeyFrame1[1].Xtor=0.0f;
	KeyFrame1[1].Ytor=0.0f;
	KeyFrame1[1].Ztor=0.0f;

	KeyFrame1[2].Angt1=-5.0f;
	KeyFrame1[2].Angt2=0.0f;
	KeyFrame1[2].Angc1=-25.0f;
	KeyFrame1[2].Angc2=0.0f;
	KeyFrame1[2].Angbi1=-5.0f;
	KeyFrame1[2].Angbi2=0.0f;
	KeyFrame1[2].Angbib=-15.0f;
	KeyFrame1[2].Angbd1=5.0f;
	KeyFrame1[2].Angbd2=0.0f;
	KeyFrame1[2].Angbdb=-40.0f;
	KeyFrame1[2].Angpizq=20.0f;
	KeyFrame1[2].Angpder=-20.0f;
	KeyFrame1[2].Angpizqb=15.0f;
	KeyFrame1[2].Angpderb=10.0f;
	KeyFrame1[2].Angpi=0.0f;
	KeyFrame1[2].Angpd=0.0f;
	KeyFrame1[2].Xtor=0.0f;
	KeyFrame1[2].Ytor=0.0f;
	KeyFrame1[2].Ztor=0.0f;
			
}

// Colisiones, todo lo referente a colisiones ira aqui
void InicializaObjetosdeColision()
{
	CVector v1, v2, a, b, c, d, va, vb, vd, Normal;
	
	//Inicia planos de colisión del piso
	plano[0].A=CVector(238.6f, 6.5f,-32.0f);
	plano[0].B=CVector(238.6f, 6.5f,-96.7f);
	plano[0].C=CVector(125.0f, 6.5f,-96.7f);
	plano[0].D=CVector(125.0f, 6.5f,-32.0f);
	plano[0].Normal=CVector(0.0f,1.0f,0.0f);
	plano[0].b1=plano[0].B-plano[0].A;
	plano[0].b2=plano[0].C-plano[0].B;
	plano[0].b3=plano[0].D-plano[0].C;
	plano[0].b4=plano[0].A-plano[0].D;
	plano[0].b1Normal=Cruz(plano[0].b1, plano[0].Normal);
	plano[0].b2Normal=Cruz(plano[0].b2, plano[0].Normal);
	plano[0].b3Normal=Cruz(plano[0].b3, plano[0].Normal);
	plano[0].b4Normal=Cruz(plano[0].b4, plano[0].Normal);
	plano[0].tipo=1;

	plano[1].A=CVector(125.0f, 6.5f,-32.0f);
	plano[1].B=CVector(125.0f, 6.5f,-96.7f);
	plano[1].C=CVector(100.0f, 6.5f,-96.7f);
	plano[1].D=CVector(100.0f, 6.5f,-32.0f);
	plano[1].Normal=CVector(0.0f,1.0f,0.0f);
	plano[1].b1=plano[1].B-plano[1].A;
	plano[1].b2=plano[1].C-plano[1].B;
	plano[1].b3=plano[1].D-plano[1].C;
	plano[1].b4=plano[1].A-plano[1].D;
	plano[1].b1Normal=Cruz(plano[1].b1, plano[1].Normal);
	plano[1].b2Normal=Cruz(plano[1].b2, plano[1].Normal);
	plano[1].b3Normal=Cruz(plano[1].b3, plano[1].Normal);
	plano[1].b4Normal=Cruz(plano[1].b4, plano[1].Normal);
	plano[1].tipo=1;

	plano[2].A=CVector(100.0f, 6.5f,-32.0f);
	plano[2].B=CVector(100.0f, 6.5f,-96.7f);
	plano[2].C=CVector(5.0f, 6.5f,-96.7f);
	plano[2].D=CVector(5.0f, 6.5f,-32.0f);
	plano[2].Normal=CVector(0.0f,1.0f,0.0f);
	plano[2].b1=plano[2].B-plano[2].A;
	plano[2].b2=plano[2].C-plano[2].B;
	plano[2].b3=plano[2].D-plano[2].C;
	plano[2].b4=plano[2].A-plano[2].D;
	plano[2].b1Normal=Cruz(plano[2].b1, plano[2].Normal);
	plano[2].b2Normal=Cruz(plano[2].b2, plano[2].Normal);
	plano[2].b3Normal=Cruz(plano[2].b3, plano[2].Normal);
	plano[2].b4Normal=Cruz(plano[2].b4, plano[2].Normal);
	plano[2].tipo=1;

	plano[3].A=CVector(5.0f, 6.5f,-32.0f);
	plano[3].B=CVector(5.0f, 6.5f,-96.7f);
	plano[3].C=CVector(-10.5f, 6.5f,-96.7f);
	plano[3].D=CVector(-10.5f, 6.5f,-32.0f);
	plano[3].Normal=CVector(0.0f,1.0f,0.0f);
	plano[3].b1=plano[3].B-plano[3].A;
	plano[3].b2=plano[3].C-plano[3].B;
	plano[3].b3=plano[3].D-plano[3].C;
	plano[3].b4=plano[3].A-plano[3].D;
	plano[3].b1Normal=Cruz(plano[3].b1, plano[3].Normal);
	plano[3].b2Normal=Cruz(plano[3].b2, plano[3].Normal);
	plano[3].b3Normal=Cruz(plano[3].b3, plano[3].Normal);
	plano[3].b4Normal=Cruz(plano[3].b4, plano[3].Normal);
	plano[3].tipo=1;

	//Inicia planos de colisión de los muros
	plano[4].A=CVector(237.0f,  0.0f,-99.0f);
	plano[4].B=CVector(237.0f,  50.0f, -99.0f);
	plano[4].C=CVector(-97.0f, 50.0f, -99.0f);
	plano[4].D=CVector(-97.0f, 0.0f,-99.0f);
	plano[4].Normal=CVector(0.0f,0.0f,1.0f);
	plano[4].b1=plano[4].B-plano[4].A;
	plano[4].b2=plano[4].C-plano[4].B;
	plano[4].b3=plano[4].D-plano[4].C;
	plano[4].b4=plano[4].A-plano[4].D;
	plano[4].b1Normal=Cruz(plano[4].b1, plano[4].Normal);
	plano[4].b2Normal=Cruz(plano[4].b2, plano[4].Normal);
	plano[4].b3Normal=Cruz(plano[4].b3, plano[4].Normal);
	plano[4].b4Normal=Cruz(plano[4].b4, plano[4].Normal);
	plano[4].tipo=2;
	
	plano[5].A=CVector(-97.0f,  0.0f, -99.0f);
	plano[5].B=CVector(-97.0f,  50.0f,-99.0f);
	plano[5].C=CVector(-97.0f, 50.0f,153.0f);
	plano[5].D=CVector(-97.0f, 0.0f, 153.0f);
	plano[5].Normal=CVector(1.0f,0.0f,0.0f);
	plano[5].b1=plano[5].B-plano[5].A;
	plano[5].b2=plano[5].C-plano[5].B;
	plano[5].b3=plano[5].D-plano[5].C;
	plano[5].b4=plano[5].A-plano[5].D;
	plano[5].b1Normal=Cruz(plano[5].b1, plano[5].Normal);
	plano[5].b2Normal=Cruz(plano[5].b2, plano[5].Normal);
	plano[5].b3Normal=Cruz(plano[5].b3, plano[5].Normal);
	plano[5].b4Normal=Cruz(plano[5].b4, plano[5].Normal);
	plano[5].tipo=2;

	plano[6].A=CVector(-10.5f, 6.5f,-96.7f);
	plano[6].B=CVector(-95.0f, 6.5f,-96.7f);
	plano[6].C=CVector(-95.0f, 6.5f,5.0f);
	plano[6].D=CVector(-10.5f, 6.5f,5.0f);
	plano[6].Normal=CVector(0.0f,1.0f,0.0f);
	plano[6].b1=plano[6].B-plano[6].A;
	plano[6].b2=plano[6].C-plano[6].B;
	plano[6].b3=plano[6].D-plano[6].C;
	plano[6].b4=plano[6].A-plano[6].D;
	plano[6].b1Normal=Cruz(plano[6].b1, plano[6].Normal);
	plano[6].b2Normal=Cruz(plano[6].b2, plano[6].Normal);
	plano[6].b3Normal=Cruz(plano[6].b3, plano[6].Normal);
	plano[6].b4Normal=Cruz(plano[6].b4, plano[6].Normal);
	plano[6].tipo=1;

	plano[7].A=CVector(-10.5f, 6.5f,5.0f);
	plano[7].B=CVector(-95.0f, 6.5f,5.0f);
	plano[7].C=CVector(-95.0f, 6.5f,50.0f);
	plano[7].D=CVector(-10.5f, 6.5f,50.0f);
	plano[7].Normal=CVector(0.0f,1.0f,0.0f);
	plano[7].b1=plano[7].B-plano[7].A;
	plano[7].b2=plano[7].C-plano[7].B;
	plano[7].b3=plano[7].D-plano[7].C;
	plano[7].b4=plano[7].A-plano[7].D;
	plano[7].b1Normal=Cruz(plano[7].b1, plano[7].Normal);
	plano[7].b2Normal=Cruz(plano[7].b2, plano[7].Normal);
	plano[7].b3Normal=Cruz(plano[7].b3, plano[7].Normal);
	plano[7].b4Normal=Cruz(plano[7].b4, plano[7].Normal);
	plano[7].tipo=1;



	plano[8].A=CVector(-10.5f, 6.5f,50.0f);
	plano[8].B=CVector(-95.0f, 6.5f,50.0f);
	plano[8].C=CVector(-95.0f, 6.5f,150.0f);
	plano[8].D=CVector(-10.5f, 6.5f,150.0f);
	plano[8].Normal=CVector(0.0f,1.0f,0.0f);
	plano[8].b1=plano[7].B-plano[7].A;
	plano[8].b2=plano[7].C-plano[7].B;
	plano[8].b3=plano[7].D-plano[7].C;
	plano[8].b4=plano[7].A-plano[7].D;
	plano[8].b1Normal=Cruz(plano[7].b1, plano[7].Normal);
	plano[8].b2Normal=Cruz(plano[7].b2, plano[7].Normal);
	plano[8].b3Normal=Cruz(plano[7].b3, plano[7].Normal);
	plano[8].b4Normal=Cruz(plano[7].b4, plano[7].Normal);
	plano[8].tipo=1;	


	for(int i=0; i<maxPlanos; i++)
	{
		plano[i].PM=plano[i].A+(plano[i].C-plano[i].A)*0.5f;
	}

	//Esfera de colision del personaje
	esfera[0].radio=1.8f;
	esfera[0].Pos=CVector(player1.PosicionObj.x, player1.PosicionObj.y+2.5f, player1.PosicionObj.z);
	esfera[0].colision=false;

	esfera[20].radio=4.0;
	esfera[20].Pos=CVector(210.91f, 6.0f, -93.0f);
	esfera[20].colision=false;

	esfera[21].radio=4.0;
	esfera[21].Pos=CVector(203.0f, 6.0f, -96.0f);
	esfera[21].colision=false;

	esfera[22].radio=4.0;
	esfera[22].Pos=CVector(43.0f, 6.0f, -92.0f);
	esfera[22].colision=false;

	esfera[23].radio=4.0;
	esfera[23].Pos=CVector(47.0f, 6.0f, -92.0f);
	esfera[23].colision=false;

	esfera[24].radio=8.0;
	esfera[24].Pos=CVector(-5.0f, 6.0f, -35.0f);
	esfera[24].colision=false;

	esfera[25].radio=4.0;
	esfera[25].Pos=CVector(-47.0f, 6.0f, -93.0f);
	esfera[25].colision=false;

	esfera[26].radio=4.0;
	esfera[26].Pos=CVector(-53.0f, 6.0f, -91.0f);
	esfera[26].colision=false;

	esfera[27].radio=4.0;
	esfera[27].Pos=CVector(-56.0f, 6.0f, -93.0f);
	esfera[27].colision=false;

	esfera[28].radio=8.0;
	esfera[28].Pos=CVector(-82.0f, 6.0f, 17.0f);
	esfera[28].colision=false;

	esfera[29].radio=8.0;
	esfera[29].Pos=CVector(-22.0f, 6.0f, 120.0f);
	esfera[29].colision=false;

}

void ActualizaObjetosDinamicosColision()
{
	esfera[0].Pos=CVector(player1.PosicionObj.x, player1.PosicionObj.y+2.5f, player1.PosicionObj.z);
}

void DibujaObjetosdeColision()
{
	glDisable(GL_LIGHTING);

	glColor3f(1.0f,0.0f,0.0f);
	glLineWidth(4.0f);

	for(int i=0; i<maxPlanos; i++)
	{
		glBegin(GL_LINE_LOOP);
			glVertex3f(plano[i].A.x,plano[i].A.y,plano[i].A.z);
			glVertex3f(plano[i].B.x,plano[i].B.y,plano[i].B.z);
			glVertex3f(plano[i].C.x,plano[i].C.y,plano[i].C.z);
			glVertex3f(plano[i].D.x,plano[i].D.y,plano[i].D.z);
		glEnd();
	}

	glColor3f(1.0f,1.0f,1.0f);
	glLineWidth(1.0f);

	glEnable(GL_LIGHTING);
}

void DibujaEsferasColision()
{
	GLUquadricObj	*q;

	q=gluNewQuadric();

	gluQuadricDrawStyle(q, GLU_LINE);

	glDisable(GL_LIGHTING);

	glColor3f(1.0f, 1.0f, 1.0f);

	for( int i = 0; i < 30; i++)
	{
		glPushMatrix();
			glTranslatef(esfera[i].Pos.x, esfera[i].Pos.y, esfera[i].Pos.z);
			glRotatef(90.0f,1.0f,0.0f,0.0f);
			gluSphere(q, esfera[i].radio, 16, 8);
		glPopMatrix();
	}

	glEnable(GL_LIGHTING);

	gluDeleteQuadric(q);
}

void ColisionEsferaPlano(int id, int dir, parametros& player )
{
	CVector TestPoint;
	CVector PMVect;

	CVector bordeNormal;
	CVector dp;

	CVector PosAux;

	float SaveAltObj, SaveAltCam;

	float Determinante;

	SaveAltObj=player.PosicionObj.y;
	SaveAltCam=player.PosicionCam.y;

	//Se comprueba para cada plano
	for(int j=0; j<maxPlanos; j++)
	{
		if(plano[j].tipo == 2)
		{
			//TestPoint=esfera[id].Pos-plano[j].Normal*esfera[id].radio;
			TestPoint=player.PosicionObj-plano[j].Normal*esfera[id].radio;
			PMVect=TestPoint-plano[j].PM;

			float param=Punto(plano[j].Normal,PMVect);
			if(param <= 0.0001f && param > -0.4f)
			{
				//Ahora se comprueba que el punto de prueba se encuentre dentro de los límites del plano
				dp=TestPoint-plano[j].A;
				Determinante=Punto(plano[j].b1Normal, dp);
				if(Determinante > 0.001f) continue;

				dp=TestPoint-plano[j].B;
				Determinante=Punto(plano[j].b2Normal,dp);	
				if(Determinante > 0.001f) continue;

				dp=TestPoint-plano[j].C;
				Determinante=Punto(plano[j].b3Normal,dp);	
				if(Determinante > 0.001f) continue;

				dp=TestPoint-plano[j].D;
				Determinante=Punto(plano[j].b4Normal,dp);	
				if(Determinante > 0.001f) continue;

				//esfera[0].colision=true;

				if(Punto(plano[j].Normal,PMVect) < 0.0f)
				{
					float deltaV=player.VelocidadObj/10.0f;

					for(int k=0; k<10; k++)
					{
						float vel=k*deltaV;
						if(dir == 1)
							PosAux=player.PosAntObj+player.Direccion*vel;
						else if(dir == 2)
							PosAux=player.PosAntObj-player.Direccion*vel;

						CVector TestPointA=PosAux-plano[j].Normal*esfera[id].radio;
						CVector PMVectA=TestPointA-plano[j].PM;
						
						if(Punto(plano[j].Normal,PMVectA) <= 0.0001f)
						{
							//float velRet=(k+1)*deltaV;
							float velRet=k*deltaV;
							//float velRet=k*deltaV+0.001f;

							player.PosicionObj=PosAux;
							//player.Posicion=player.Posicion+plano[j].Normal*velRet;

							if(dir == 1)
								player.PosicionObj=player.PosicionObj-player.Direccion*velRet;
							else if(dir == 2)
								player.PosicionObj=player.PosicionObj+player.Direccion*velRet;
								
							player.PosicionCam=player.PosicionObj-player.Direccion*player.DistanciaCam;
							player.PosicionCam.y=player.CamaraPosAlt;

							player.ObjetivoCam=player.PosicionObj;
							player.ObjetivoCam.y=player.CamaraObjAlt;

							break;
						}
					}
				}
				
				break;
								
			}

		}
	}
	
}
bool InterseccionPlanoPiso(CVector P, CVector Dir, boundingplane *plano, float *Altura, CVector *Qp)
{
	float Determinante;
	CVector Normal, vA, vB, vC, vD;
	CVector dp;

	Normal=plano->Normal;
	vA=plano->A;
	vB=plano->B;
	vC=plano->C;
	vD=plano->D;

	float D=Punto(Normal*-1, vA);
	
	float denominador=Punto(Normal,Dir);

	if(fabs(denominador) < 0.0001f)
		return false;

	float numerador=-(Punto(Normal, P)+D);

	float t=numerador/denominador;

	CVector Q = P + Dir*t;

	CVector bordeNormal1=plano->b1Normal;
	CVector bordeNormal2=plano->b2Normal;
	CVector bordeNormal3=plano->b3Normal;
	CVector bordeNormal4=plano->b4Normal;

	dp=Q-vA;
	Determinante=Punto(bordeNormal1, dp);
	if(Determinante > 0.001f) return false;

	dp=Q-vB;
	Determinante=Punto(bordeNormal2,dp);	
	if(Determinante > 0.001f) return false;

	dp=Q-vC;
	Determinante=Punto(bordeNormal3,dp);	
	if(Determinante > 0.001f) return false;

	dp=Q-vD;
	Determinante=Punto(bordeNormal4,dp);	
	if(Determinante > 0.001f) return false;

	*Altura=Q.y;
	*Qp=Q;

	return true;
}

void ColisionesPiso()
{
	float Altura;
	int index;

	CVector Normal;
	CVector Qpoint;

	CVector RayStart = CVector(player1.PosicionObj.x, player1.PosicionObj.y, player1.PosicionObj.z);
	CVector RayDir = CVector(0.0f,-1.0f,0.0f);

	for(int i=0; i<maxPlanos; i++)
	{
		if(plano[i].tipo == 1)
		{
			if(InterseccionPlanoPiso(RayStart, RayDir, &plano[i], &Altura, &Qpoint))
			{
				//player1.PosicionObj.y=Altura;

				player1.PosicionObj.y=Altura;
				player1.CamaraPosAlt=Altura+5.0f;
				player1.CamaraObjAlt=Altura+4.0f+player1.CamaraObjAltE;

				player1.PosicionCam.y=player1.CamaraPosAlt;
				player1.ObjetivoCam=player1.PosicionObj;
				player1.ObjetivoCam.y=player1.CamaraObjAlt;
				pisoId = i;
			}
		}
	}

}

void ColisionEsferaEsfera( boundingsphere& a, boundingsphere& b, int dir, parametros& player )
{
	// Calcula la distancia cuadrada entre los centros
	CVector d = a.Pos - b.Pos;
	float dist2 = Punto(d, d);
	// Las esferas se intersectan si la distancia es menor a la suma cuadrada de sus radios
	float radiusSum = a.radio + b.radio;
	float colision = dist2 - radiusSum * radiusSum;
	// Robado de arriba yeah XD
	CVector PosAux;
	float deltaV = player.VelocidadObj/10.0f;

	if( dist2 <= radiusSum * radiusSum ) // si hay una colision
	{
		if(dir == 1)
			PosAux = player.PosAntObj + player.Direccion * ( deltaV + 0.0001f );
		else if(dir == 2)
			PosAux = player.PosAntObj - player.Direccion * ( deltaV + 0.0001f );

		player.PosicionObj = PosAux;

		if( colision < -2.0f )
		{
		if(dir == 1)
			player.PosicionObj = player.PosicionObj - player.Direccion * ( deltaV + 0.3f );
		else if(dir == 2)
			player.PosicionObj = player.PosicionObj + player.Direccion * ( deltaV + 0.3f );
		}
	}
}

int InitGL(GLvoid)										// Aqui se configuran los parametros iniciales de OpenGL
{
	Multitext.InitMultitext(hWnd);

	glShadeModel(GL_SMOOTH);							// Activa Smooth Shading
	glClearColor(0.4f, 0.6f, 0.8f, 0.5f);				// Fondo azul
	glClearDepth(1.0f);									// Valor para el Depth Buffer
	glClearStencil(0);
	glEnable(GL_DEPTH_TEST);							// Activa Depth Testing
	glDepthFunc(GL_LEQUAL);								// Tipo de Depth Testing a usar
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Correccion de cálculos de perspectiva

	glCullFace(GL_BACK);								// Set Culling Face To Back Face
	glEnable(GL_CULL_FACE);								// Enable Culling

	//Iluminación
	//Inicializa la luz
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);		// Posicion de la luz1
	glLightfv(GL_LIGHT0, GL_AMBIENT,  LightAmb);		// Componente ambiente
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  LightDif);		// Componente difusa
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpc);		// Componente especular

	/*
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.05f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05f);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);
	*/
	
	glEnable(GL_LIGHT0);								// Activa luz0
	glEnable(GL_LIGHTING);								// Habilita la iluminación

	//CargaTexturas();

	CargaModelos();
	CreaListas();
	IniSombraVolumen();

	Font.BuildFont();

	InitGLSL();
	
	e=gluNewQuadric();

	InicializaParametrosdeControl();
	InicializaAnim();
	DatosAnimacion();

	// Colisiones
	InicializaObjetosdeColision();

	return TRUE;										
}

void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }
}

void LiberaSonido(FMOD_SYSTEM *system, FMOD_RESULT result)
{
	result = FMOD_Sound_Release(sound[0]);
    ERRCHECK(result);
    result = FMOD_Sound_Release(sound[1]);
    ERRCHECK(result);
    result = FMOD_System_Close(system);
    ERRCHECK(result);
    result = FMOD_System_Release(system);
    ERRCHECK(result);
}

void ControlPersonaje(int funcion)
{
	if( funcion == 1 ) //Giro a la derecha
	{
		player1.AngDir+=1.0f;
		if(player1.AngDir > 360.0f)
			player1.AngDir -= 360.0f;

		player1.AngObj-=1.0f;
		if(player1.AngObj < 0.0f)
			player1.AngObj += 360.0f;

		player1.Direccion.x = cosf( player1.AngDir * PI/180.0f);
		player1.Direccion.y = 0.0f;
		player1.Direccion.z = sinf( player1.AngDir * PI/180.0f);

		player1.PosicionCam = player1.PosicionObj - player1.Direccion * player1.DistanciaCam;
		player1.PosicionCam.y = player1.CamaraPosAlt;
		player1.ObjetivoCam = player1.PosicionObj;
		player1.ObjetivoCam.y = player1.CamaraObjAlt;
	}
	else if(funcion == 2) //Giro a la izquierda
	{
		player1.AngDir-=1.0f;
		if(player1.AngDir < 0.0f)
			player1.AngDir+=360.0f;

		player1.AngObj+=1.0f;
		if(player1.AngObj > 360.0f)
			player1.AngObj-=360.0f;

		player1.Direccion.x = cosf(player1.AngDir*PI/180.0f);
		player1.Direccion.y = 0.0f;
		player1.Direccion.z = sinf(player1.AngDir*PI/180.0f);

		player1.PosicionCam = player1.PosicionObj - player1.Direccion * player1.DistanciaCam;
		player1.PosicionCam.y = player1.CamaraPosAlt;
		player1.ObjetivoCam = player1.PosicionObj;
		player1.ObjetivoCam.y = player1.CamaraObjAlt;
	}
	else if(funcion == 3) //Avanza hacia adelante
	{
		player1.PosicionObj = player1.PosicionObj + player1.Direccion * player1.VelocidadObj;
		player1.PosicionCam = player1.PosicionObj - player1.Direccion * player1.DistanciaCam;
		player1.PosicionCam.y = player1.CamaraPosAlt;
		player1.ObjetivoCam = player1.PosicionObj;
		player1.ObjetivoCam.y = player1.CamaraObjAlt;

		// Colisiones
		for(int i = 1; i <= 30; i++)
		{
			ColisionEsferaEsfera(esfera[0], esfera[i], 1, player1 );
		}
		ColisionEsferaPlano(0, 1, player1 );

		player1.PosAntObj = player1.PosicionObj;

	}
	else if(funcion == 4) //Avanza hacia atrás
	{
		player1.PosicionObj = player1.PosicionObj - player1.Direccion * player1.VelocidadObj;
		player1.PosicionCam = player1.PosicionObj - player1.Direccion * player1.DistanciaCam;
		player1.PosicionCam.y = player1.CamaraPosAlt;
		player1.ObjetivoCam = player1.PosicionObj;
		player1.ObjetivoCam.y = player1.CamaraObjAlt;

		// Colisiones
		for(int i = 1; i <= 30; i++)
		{
			ColisionEsferaEsfera(esfera[0], esfera[i], 2, player1 );
		}
		ColisionEsferaPlano(0, 2, player1 );

		player1.PosAntObj = player1.PosicionObj;

	}
	else if(funcion == 5) //Sube objetivo de la cámara
	{
		player1.ObjetivoCam.y += 1.0f;
		player1.CamaraObjAlt += 1.0f;
		player1.CamaraObjAltE += 1.0f;
		
	}
	else if(funcion == 6) //Baja objetivo de la cámara
	{
		player1.ObjetivoCam.y -= 1.0f;
		player1.CamaraObjAlt -= 1.0f;
		player1.CamaraObjAltE -= 1.0f;
		
	}
}

void animacion(FRAME *KeyFrame, int maxKF , int frames)
{
	if(play)
	{		
		if((abs(KeyFrame[playIndex+1].Angt1-player1modelo.Angt1))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angt2-player1modelo.Angt2))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angc1-player1modelo.Angc1))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angc2-player1modelo.Angc2))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angbi1-player1modelo.Angbi1))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angbi2-player1modelo.Angbi2))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angbib-player1modelo.Angbib))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angbd1-player1modelo.Angbd1))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angbd2-player1modelo.Angbd2))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angbdb-player1modelo.Angbdb))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angpizq-player1modelo.Angpizq))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angpizqb-player1modelo.Angpizqb))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angpder-player1modelo.Angpder))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angpderb-player1modelo.Angpderb))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angpi-player1modelo.Angpi))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angpd-player1modelo.Angpd))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Xtor-player1modelo.Xtor))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Ytor-player1modelo.Ytor))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Ztor-player1modelo.Ztor))<0.1)
		{			
			playIndex++;			
			if(playIndex>maxKF-2)
			{
				playIndex=0;
				play=false;
								
			}
		}
		else
		{
			KeyFrame[playIndex].incAngt1    = (KeyFrame[playIndex+1].Angt1-KeyFrame[playIndex].Angt1)/frames;
			KeyFrame[playIndex].incAngt2    = (KeyFrame[playIndex+1].Angt2-KeyFrame[playIndex].Angt2)/frames;
			KeyFrame[playIndex].incAngc1    = (KeyFrame[playIndex+1].Angc1-KeyFrame[playIndex].Angc1)/frames;
			KeyFrame[playIndex].incAngc2    = (KeyFrame[playIndex+1].Angc2-KeyFrame[playIndex].Angc2)/frames;
			KeyFrame[playIndex].incAngbi1   = (KeyFrame[playIndex+1].Angbi1-KeyFrame[playIndex].Angbi1)/frames;
			KeyFrame[playIndex].incAngbi2   = (KeyFrame[playIndex+1].Angbi2-KeyFrame[playIndex].Angbi2)/frames;
			KeyFrame[playIndex].incAngbib   = (KeyFrame[playIndex+1].Angbib-KeyFrame[playIndex].Angbib)/frames;
			KeyFrame[playIndex].incAngbd1   = (KeyFrame[playIndex+1].Angbd1-KeyFrame[playIndex].Angbd1)/frames;
			KeyFrame[playIndex].incAngbd2   = (KeyFrame[playIndex+1].Angbd2-KeyFrame[playIndex].Angbd2)/frames;
			KeyFrame[playIndex].incAngbdb   = (KeyFrame[playIndex+1].Angbdb-KeyFrame[playIndex].Angbdb)/frames;
			KeyFrame[playIndex].incAngpizq  = (KeyFrame[playIndex+1].Angpizq-KeyFrame[playIndex].Angpizq)/frames;
			KeyFrame[playIndex].incAngpizqb = (KeyFrame[playIndex+1].Angpizqb-KeyFrame[playIndex].Angpizqb)/frames;
			KeyFrame[playIndex].incAngpder  = (KeyFrame[playIndex+1].Angpder-KeyFrame[playIndex].Angpder)/frames;
			KeyFrame[playIndex].incAngpderb = (KeyFrame[playIndex+1].Angpderb-KeyFrame[playIndex].Angpderb)/frames;
			KeyFrame[playIndex].incAngpi    = (KeyFrame[playIndex+1].Angpi-KeyFrame[playIndex].Angpi)/frames;
			KeyFrame[playIndex].incAngpd    = (KeyFrame[playIndex+1].Angpd-KeyFrame[playIndex].Angpd)/frames;
			KeyFrame[playIndex].incXtor     = (KeyFrame[playIndex+1].Xtor-KeyFrame[playIndex].Xtor)/frames;
			KeyFrame[playIndex].incYtor     = (KeyFrame[playIndex+1].Ytor-KeyFrame[playIndex].Ytor)/frames;
			KeyFrame[playIndex].incZtor     = (KeyFrame[playIndex+1].Ztor-KeyFrame[playIndex].Ztor)/frames;
			
			player1modelo.Angt1    += KeyFrame[playIndex].incAngt1;
			player1modelo.Angt2    += KeyFrame[playIndex].incAngt2;
			player1modelo.Angc1    += KeyFrame[playIndex].incAngc1;
			player1modelo.Angc2    += KeyFrame[playIndex].incAngc2;
			player1modelo.Angbi1   += KeyFrame[playIndex].incAngbi1;
			player1modelo.Angbi2   += KeyFrame[playIndex].incAngbi2;
			player1modelo.Angbib   += KeyFrame[playIndex].incAngbib;
			player1modelo.Angbd1   += KeyFrame[playIndex].incAngbd1;
			player1modelo.Angbd2   += KeyFrame[playIndex].incAngbd2;
			player1modelo.Angbdb   += KeyFrame[playIndex].incAngbdb;
			player1modelo.Angpizq  += KeyFrame[playIndex].incAngpizq;
			player1modelo.Angpizqb += KeyFrame[playIndex].incAngpizqb;
			player1modelo.Angpder  += KeyFrame[playIndex].incAngpder;
			player1modelo.Angpderb += KeyFrame[playIndex].incAngpderb;
			player1modelo.Angpi    += KeyFrame[playIndex].incAngpi;
			player1modelo.Angpd    += KeyFrame[playIndex].incAngpd;
			player1modelo.Xtor     += KeyFrame[playIndex].incXtor;
			player1modelo.Ytor     += KeyFrame[playIndex].incYtor;
			player1modelo.Ztor     += KeyFrame[playIndex].incZtor;
			
		}
	}
}

//savage
void dibujaEnemigo8()
{
	static float anglef=0.0f;
	anglef+=4.0f;

	glCallList(enemigo8L);

	glPushMatrix();	
		glRotatef(anglef, 0.0f, 1.0f, 0.0f);
		glCallList(enemigo8L+1);
	glPopMatrix();

	glPushMatrix();
		glRotatef(anglef, 1.0f, 0.0f, 0.0f);
		glCallList(enemigo8L+2);
	glPopMatrix();
}

void DibujaEnemigo1()
{
	glTranslatef(enem1modelo.Xtor, enem1modelo.Ytor, enem1modelo.Ztor);
	glRotatef(enem1modelo.Angt2, 0.0f, 1.0f, 0.0f);
	glRotatef(enem1modelo.Angt1, 1.0f, 0.0f, 0.0f);
			
	//Torso
	glCallList(ene1+0);
	
	//Cabeza
	glPushMatrix();
		glRotatef(enem1modelo.Angc1, 0.0f, 1.0f, 0.0f);
		glCallList(ene1+1);
	glPopMatrix();

	//Brazo derecho
	glPushMatrix();
		//glTranslatef(-2.8f, 1.1f, 0.0f);
		glRotatef(enem1modelo.Angbd2, 0.0f, 1.0f, 0.0f);
		glRotatef(enem1modelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glCallList(ene1+2);
	glPopMatrix();

	//Brazo izquierdo
	glPushMatrix();
		//glTranslatef(-2.8f, 1.1f, 0.0f);
		glRotatef(enem1modelo.Angbd2, 0.0f, 1.0f, 0.0f);
		glRotatef(enem1modelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glCallList(ene1+3);
	glPopMatrix();
	
	//Pierna derecha
	glPushMatrix();
		//glTranslatef(-1.2f, -1.3f ,0.0f);
		glRotatef(enem1modelo.Angpder, 1.0f, 0.0f, 0.0f);
		glCallList(ene1+4);
		
		//Pierna derecha_b
		glPushMatrix();
			//glTranslatef(0.0f, -1.25f , 0.0f);
			glRotatef(enem1modelo.Angpderb, 1.0f, 0.0f, 0.0f);
			glCallList(ene1+5);
			
			//Pie derecho
			glPushMatrix();
				glRotatef(enem1modelo.Angpd,1.0f, 0.0f, 0.0f);
				glCallList(ene1+6);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();

	//Pierna izquierda
	glPushMatrix();
		//glTranslatef(1.2f, -1.3f ,0.0f);
		glRotatef(enem1modelo.Angpizq, 1.0f, 0.0f, 0.0f);
		glCallList(ene1+7);

		//Pierna izquierda_b
		glPushMatrix();
			//glTranslatef(0.0f, -1.25f , 0.0f);
			glRotatef(enem1modelo.Angpizqb, 1.0f, 0.0f, 0.0f);
			glCallList(ene1+8);
			//Pie derecho
			glPushMatrix();
				glRotatef(enem1modelo.Angpd,1.0f, 0.0f, 0.0f);
				glCallList(ene1+9);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}

void DibujaEnemigo2()
{
	glTranslatef(enem2modelo.Xtor, enem2modelo.Ytor, enem2modelo.Ztor);
	glRotatef(enem2modelo.Angt2, 0.0f, 1.0f, 0.0f);
	glRotatef(enem2modelo.Angt1, 1.0f, 0.0f, 0.0f);

	//Nave
	glCallList(ene2+0);
	//Bomba
	glCallList(ene2+1);
	//Piloto
//	glCallList(ene2+2);
}

void DibujaMJ6()
{
	glTranslatef(MJ6modelo.Xtor, MJ6modelo.Ytor, MJ6modelo.Ztor);
	glRotatef(MJ6modelo.Angt2, 0.0f, 1.0f, 0.0f);
	glRotatef(MJ6modelo.Angt1, 1.0f, 0.0f, 0.0f);
			
	//Torso
	glCallList(noMJ6+0);
	
	//Cabeza
	glPushMatrix();
		glRotatef(MJ6modelo.Angc1, 0.0f, 1.0f, 0.0f);
		glCallList(noMJ6+1);
	glPopMatrix();

	//Brazo derecho
	glPushMatrix();
		//glTranslatef(-2.8f, 1.1f, 0.0f);
		glRotatef(MJ6modelo.Angbd2, 0.0f, 1.0f, 0.0f);
		glRotatef(MJ6modelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glCallList(noMJ6+2);

		//Brazo derecho_b
		glPushMatrix();
			//glTranslatef(0.0f, -1.25f , 0.0f);
			glRotatef(MJ6modelo.Angpderb, 1.0f, 0.0f, 0.0f);
			glCallList(noMJ6+3);
		glPopMatrix();
	glPopMatrix();

	//Brazo izquierdo
	glPushMatrix();
		//glTranslatef(-2.8f, 1.1f, 0.0f);
		glRotatef(MJ6modelo.Angbd2, 0.0f, 1.0f, 0.0f);
		glRotatef(MJ6modelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glCallList(noMJ6+4);

		//Brazo izquierdo_b
		glPushMatrix();
			//glTranslatef(0.0f, -1.25f , 0.0f);
			glRotatef(MJ6modelo.Angpderb, 1.0f, 0.0f, 0.0f);
			glCallList(noMJ6+5);
		glPopMatrix();
	glPopMatrix();
	
	//Pierna derecha
	glPushMatrix();
		//glTranslatef(-1.2f, -1.3f ,0.0f);
		glRotatef(MJ6modelo.Angpder, 1.0f, 0.0f, 0.0f);
		glCallList(noMJ6+6);
		
		//Pierna derecha_b
		glPushMatrix();
			//glTranslatef(0.0f, -1.25f , 0.0f);
			glRotatef(MJ6modelo.Angpderb, 1.0f, 0.0f, 0.0f);
			glCallList(noMJ6+7);
		glPopMatrix();
	glPopMatrix();

	//Pierna izquierda
	glPushMatrix();
		//glTranslatef(1.2f, -1.3f ,0.0f);
		glRotatef(MJ6modelo.Angpizq, 1.0f, 0.0f, 0.0f);
		glCallList(noMJ6+8);

		//Pierna izquierda_b
		glPushMatrix();
			//glTranslatef(0.0f, -1.25f , 0.0f);
			glRotatef(MJ6modelo.Angpizqb, 1.0f, 0.0f, 0.0f);
			glCallList(noMJ6+9);
		glPopMatrix();
	glPopMatrix();
}

void DibujaEnemigo3a()
{
	glTranslatef(enem3amodelo.Xtor, enem3amodelo.Ytor, enem3amodelo.Ztor);
	glRotatef(enem3amodelo.Angt2, 0.0f, 1.0f, 0.0f);
	glRotatef(enem3amodelo.Angt1, 1.0f, 0.0f, 0.0f);
			
	//Torso
	glCallList(ene3a+0);
	
	//Brazo derecho
	glPushMatrix();
		//glTranslatef(-2.8f, 1.1f, 0.0f);
		glRotatef(enem3amodelo.Angbd2, 0.0f, 1.0f, 0.0f);
		glRotatef(enem3amodelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glCallList(ene3a+1);
	glPopMatrix();

	//Brazo izquierdo
	glPushMatrix();
		//glTranslatef(-2.8f, 1.1f, 0.0f);
		glRotatef(enem3amodelo.Angbd2, 0.0f, 1.0f, 0.0f);
		glRotatef(enem3amodelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glCallList(ene3a+2);
	glPopMatrix();
	
	//Pierna derecha
	glPushMatrix();
		//glTranslatef(-1.2f, -1.3f ,0.0f);
		glRotatef(enem3amodelo.Angpder, 1.0f, 0.0f, 0.0f);
		glCallList(ene3a+3);
	glPopMatrix();

	//Pierna izquierda
	glPushMatrix();
		//glTranslatef(1.2f, -1.3f ,0.0f);
		glRotatef(enem3amodelo.Angpizq, 1.0f, 0.0f, 0.0f);
		glCallList(ene3a+4);
	glPopMatrix();

}

void DibujaEnemigo3b()
{
	glTranslatef(enem3bmodelo.Xtor, enem3bmodelo.Ytor, enem3bmodelo.Ztor);
	glRotatef(enem3bmodelo.Angt2, 0.0f, 1.0f, 0.0f);
	glRotatef(enem3bmodelo.Angt1, 1.0f, 0.0f, 0.0f);
			
	//Torso
	glCallList(ene3b+0);
	
	//Brazo derecho
	glPushMatrix();
		//glTranslatef(-2.8f, 1.1f, 0.0f);
		glRotatef(enem3bmodelo.Angbd2, 0.0f, 1.0f, 0.0f);
		glRotatef(enem3bmodelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glCallList(ene3b+1);
	glPopMatrix();

	//Brazo izquierdo
	glPushMatrix();
		//glTranslatef(-2.8f, 1.1f, 0.0f);
		glRotatef(enem3bmodelo.Angbd2, 0.0f, 1.0f, 0.0f);
		glRotatef(enem3bmodelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glCallList(ene3b+2);
	glPopMatrix();
	
	//Pierna derecha
	glPushMatrix();
		//glTranslatef(-1.2f, -1.3f ,0.0f);
		glRotatef(enem3bmodelo.Angpder, 1.0f, 0.0f, 0.0f);
		glCallList(ene3b+3);
	glPopMatrix();

	//Pierna izquierda
	glPushMatrix();
		//glTranslatef(1.2f, -1.3f ,0.0f);
		glRotatef(enem3bmodelo.Angpizq, 1.0f, 0.0f, 0.0f);
		glCallList(ene3b+4);
	glPopMatrix();
}


void DibujaChango()
{
	glTranslatef(changmodelo.Xtor, changmodelo.Ytor, changmodelo.Ztor);
	glRotatef(changmodelo.Angt2, 0.0f, 1.0f, 0.0f);
	glRotatef(changmodelo.Angt1, 1.0f, 0.0f, 0.0f);
			
	//Torso
	glCallList(cha+0);
	
	//Brazo derecho
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, 2.8f);
		glRotatef(changmodelo.Angbd2, 0.0f, 1.0f, 0.0f);
		glRotatef(changmodelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glCallList(cha+1);
	glPopMatrix();

	//Brazo izquierdo
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, 2.8f);
		glRotatef(changmodelo.Angbd2, 0.0f, 1.0f, 0.0f);
		glRotatef(changmodelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glCallList(cha+2);
	glPopMatrix();
	
	//Pierna derecha
	glPushMatrix();
		//glTranslatef(-1.2f, -1.3f ,0.0f);
		glRotatef(changmodelo.Angpder, 1.0f, 0.0f, 0.0f);
		glCallList(cha+3);
	glPopMatrix();

	//Pierna izquierda
	glPushMatrix();
		//glTranslatef(1.2f, -1.3f ,0.0f);
		glRotatef(changmodelo.Angpizq, 1.0f, 0.0f, 0.0f);
		glCallList(cha+4);
	glPopMatrix();
}



// Dibuja Personajes de Aru
void DibujaPersonajeAru()
{
	glTranslatef(player1modelo.Xtor, player1modelo.Ytor, player1modelo.Ztor);
	glRotatef(player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
	glRotatef(player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
			
	//Torso
	glCallList(modelo1aru+0);
	
	//Pierna derecha
	glPushMatrix();
		glTranslatef(1.0f, 0.0f, 0.0f);
		glRotatef(player1modelo.Angpder, 1.0f, 0.0f, 0.0f);
		glCallList(modelo1aru+1);
		
		//Pierna derecha_b
		glPushMatrix();
			glTranslatef(-5.2f, 0.7f , 0.0f);
			glRotatef(player1modelo.Angpderb, 1.0f, 0.0f, 0.0f);
			glCallList(modelo1aru+2);
		glPopMatrix();

	glPopMatrix();

	//Pierna izquierda
	glPushMatrix();
		glTranslatef(0.0f, 0.0f ,0.0f);
		glRotatef(player1modelo.Angpizq, 1.0f, 0.0f, 0.0f);
		glCallList(modelo1aru+3);

		//Pierna izquierda_b
		glPushMatrix();
			glTranslatef(-6.5f, 0.8f , 0.0f);
			glRotatef(player1modelo.Angpizqb, 1.0f, 0.0f, 0.0f);
			glCallList(modelo1aru+4);
		glPopMatrix();

	glPopMatrix();

	//Brazo derecho_a
	glPushMatrix();
		glTranslatef(5.65f, 0.2f, 0.2f);
		glRotatef(player1modelo.Angbd2, 0.0f, 1.0f, 0.0f);
		glRotatef(player1modelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glCallList(modelo1aru+5);

		//Brazo derecho_b
		glPushMatrix();
			glTranslatef(-4.6f, 2.0f, 0.0f);
			glRotatef(player1modelo.Angbdb, 1.0f, 0.0f, 0.0f);
			glCallList(modelo1aru+7);
		glPopMatrix();

	glPopMatrix();

	//Brazo izquierdo
	glPushMatrix();
		glTranslatef(-1.0f, 1.5f, 0.0f);
		glRotatef(player1modelo.Angbi2, 0.0f, 1.0f, 0.0f);
		glRotatef(player1modelo.Angbi1, 1.0f, 0.0f, 0.0f);
		glCallList(modelo1aru+6);

		//Brazo izquierdo_b
		glPushMatrix();
			glTranslatef(-0.7f, 0.5f, 0.0f);
			glRotatef(player1modelo.Angbib, 1.0f, 0.0f, 0.0f);
			glCallList(modelo1aru+8);
		glPopMatrix();

	glPopMatrix();
}

void DibujaEjes()
{
	glColor3f(1.0f,0.0f,0.0f);

	glBegin(GL_LINES);
		//Eje X
		glVertex3f(-100.0f, 0.0f, 0.0f);
		glVertex3f( 100.0f, 0.0f, 0.0f);

		//Eje Y
		glVertex3f(0.0f, -100.0f, 0.0f);
		glVertex3f(0.0f,  100.0f, 0.0f);

		//Eje Z
		glVertex3f(0.0f, 0.0f, -100.0f);
		glVertex3f(0.0f, 0.0f,  100.0f);
	glEnd();

	glColor3f(1.0f,1.0f,1.0f);
}

void DibujaLuz(CVector l)
{

	lightPosition.x=LightPos[0];
	lightPosition.y=LightPos[1];
	lightPosition.z=LightPos[2];

	//Dibuja una esfera que representa la fuente luminosa
	glDisable(GL_LIGHTING);									// Deshabilita iluminación
	glPushMatrix();
		glTranslatef(l.x, l.y, l.z);// Traslada a la posicion de la luz
		glColor4f(1.0f, 0.9f, 0.0f, 1.0f);					// Color amarillo
		gluSphere(e, 1.0f, 16, 8);							// Dibuja la esfera
	glPopMatrix();
	glEnable(GL_LIGHTING);									// Habilita Iluminación

	glColor3f(1.0f, 1.0f, 1.0f);
}

void ActualizaAnimText()
{
	if(aText1 < 1.0f)
		aText1+=0.01f;
	else
		aText1=0.0f;

	if(aText2 < 1.0f)
		aText2+=0.001f;
	else
		aText2=0.0f;
}

void DibujaTextos()
{
	glDisable(GL_DEPTH_TEST);							// Desactiva la prueba de profundidad
	glMatrixMode(GL_PROJECTION);						// Selecciona la matriz de proyección
	glPushMatrix();										// Guarda la matriz de proyección
	glLoadIdentity();									// Limpia la matriz de proyección
	glOrtho(0,glWidth,0,glHeight,-1,1);					// Crea una proyección ortogonal
	glMatrixMode(GL_MODELVIEW);							// Selecciona la matriz de modelo de vista
	glPushMatrix();										// Guarda la matriz de modelo de vista
	
	glDisable(GL_LIGHTING);
	glLoadIdentity();

	glColor3f(1.0f,1.0f,1.0f);
	
	glEnable(GL_TEXTURE_2D);

	glAlphaFunc(GL_GREATER, 0.8f);
	glEnable(GL_ALPHA_TEST);

	// Texto a mostrar en pantalla
	Font.glPrint((1.0f/640.0f)*glWidth, glWidth*0.05f,glHeight*0.9f,"Pos: %f,%f,%f",player1.PosicionObj.x,player1.PosicionObj.y,player1.PosicionObj.z);
	Font.glPrint((1.0f/640.0f)*glWidth, glWidth*0.05f,glHeight*0.85f,"Id %d", pisoId);
								
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);

	glColor3f(1.0f,1.0f,1.0f);

	glEnable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);						// Selecciona la matriz de proyección
	glPopMatrix();										// Recupera la anterior matriz de proyección
	glMatrixMode(GL_MODELVIEW);							// Selecciona la matriz de modelo de vista
	glPopMatrix();										// Recupera la anterior matriz de modelo de vista
	glEnable(GL_DEPTH_TEST);							// Activa la prueba de profundidad
		
}

void ActualizaLuz()
{
	GLmatrix16f inverseModelMatrix;
	GLmatrix16f inverseModelMatrix2;
	GLmatrix16f inverseModelMatrix3;
	GLmatrix16f inverseModelMatrix4;
	GLmatrix16f inverseModelMatrix5;
	GLmatrix16f inverseModelMatrix6;
	GLmatrix16f inverseModelMatrix7;
	GLmatrix16f inverseModelMatrix8;
	GLmatrix16f inverseModelMatrix9;
	GLvector4f  lightposition;
	GLvector4f  lightposition2;
	GLvector4f  lightposition3;
	GLvector4f  lightposition4;
	GLvector4f  lightposition5;
	GLvector4f  lightposition6;
	GLvector4f  lightposition7;
	GLvector4f  lightposition8;
	GLvector4f  lightposition9;

	lightPosition.x=LightPos[0];
	lightPosition.y=LightPos[1];
	lightPosition.z=LightPos[2];
	
	//Modelo 1 (Torso)

	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition[0]=lightPosition.x;
	lightposition[1]=lightPosition.y;
	lightposition[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 1 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef(-player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player1.AngObj, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 1
	VMatMult(inverseModelMatrix, lightposition);
	
	objectSpaceLightPosition1.x=lightposition[0];
	objectSpaceLightPosition1.y=lightposition[1];
	objectSpaceLightPosition1.z=lightposition[2];

	//Modelo 2 (Pierna derecha)

	

	//Modelo 3 (Pierna derecha_b)

	

	//Modelo 4 (Pierna izquierda)

	

	//Modelo 5 (Pierna izquierda_b)

	

	//Modelo 6 (Brazo derecho_a)

	

	//Modelo 7 (Brazo derecho_b)

	

	//Modelo 8 (Brazo izquierdo_a)

	
	
	//Modelo 9 (Brazo izquierdo_b)

	
}

void DibujaMJ()
{
	// Aru
	glPushMatrix();
		glTranslatef( player1.PosicionObj.x, player1.PosicionObj.y + 2.4f, player1.PosicionObj.z + 0.0f);
		glRotatef(player1.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(player1.escalaX,player1.escalaY,player1.escalaZ);
		DibujaPersonajeAru();
	glPopMatrix();

	//MJ6
	glPushMatrix();
		glTranslatef(MJ6.PosicionObj.x, MJ6.PosicionObj.y+2.4f, MJ6.PosicionObj.z);
		glRotatef(MJ6.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(MJ6.escalaX,MJ6.escalaY,MJ6.escalaZ);
		DibujaMJ6();
	glPopMatrix();
}

void DibujaEnemigos()
{
	// savage
	glPushMatrix();
			glTranslatef(enemigo8.PosicionObj.x, enemigo8.PosicionObj.y+2.4f, enemigo8.PosicionObj.z);
			glRotatef(enemigo8.AngObj, 0.0f, 1.0f, 0.0f);
			glScalef(enemigo8.escalaX,enemigo8.escalaY,enemigo8.escalaZ);
			dibujaEnemigo8();
	glPopMatrix();

	//Ene1
	glPushMatrix();
		glTranslatef(enem1.PosicionObj.x, enem1.PosicionObj.y+2.4f, enem1.PosicionObj.z);
		glRotatef(enem1.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(enem1.escalaX,enem1.escalaY,enem1.escalaZ);
		DibujaEnemigo1();
	glPopMatrix();

	//Ene2
	glPushMatrix();
		glTranslatef(enem2.PosicionObj.x, enem2.PosicionObj.y+2.4f, enem2.PosicionObj.z);
		glRotatef(enem2.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(enem2.escalaX,enem1.escalaY,enem1.escalaZ);
		DibujaEnemigo2();
	glPopMatrix();

	//Ene3a
	glPushMatrix();
		glTranslatef(enem3a.PosicionObj.x, enem3a.PosicionObj.y+2.4f, enem3a.PosicionObj.z);
		glRotatef(enem3a.AngObj,0.0f, 1.0f, 0.0f);
		glScalef(enem3a.escalaX,enem3a.escalaY,enem3a.escalaZ);
		DibujaEnemigo3a();
	glPopMatrix();

	//Ene3b
	glPushMatrix();
		glTranslatef(enem3b.PosicionObj.x, enem3b.PosicionObj.y+2.4f, enem3b.PosicionObj.z);
		glRotatef(enem3b.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(enem3b.escalaX,enem3b.escalaY,enem3b.escalaZ);
		DibujaEnemigo3b();
	glPopMatrix();

	//Ene3chango
	glPushMatrix();
		glTranslatef(chang.PosicionObj.x, chang.PosicionObj.y+2.4f, chang.PosicionObj.z);
		glRotatef(chang.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(chang.escalaX,chang.escalaY,chang.escalaZ);
		DibujaChango();
	glPopMatrix();
}

void DibujaEscena()
{
	// Mayralol
	g_Load3ds.Render3DSFile(&g_3DModel2e, textureModel2e, 1);
	glDisable(GL_NORMALIZE);
}
void CamDiff()
{
}
void Camara()
{
	if( pisoId == 0 )
	{
		gluLookAt( CamPos[ 0 ][ 0 ], CamPos[ 0 ][ 1 ],CamPos[ 0 ][ 2 ], CamPos[ 0 ][ 3 ], CamPos[ 0 ][ 4 ], CamPos[ 0 ][ 5 ], 0.0f, 1.0f, 0.0f);
	}
	else if ( pisoId == 2 )
	{
		gluLookAt(CamPos[ 1 ][ 0 ], CamPos[ 1 ][ 1 ],CamPos[ 1 ][ 2 ], CamPos[ 1 ][ 3 ], CamPos[ 1 ][ 4 ], CamPos[ 1 ][ 5 ], 0.0f, 1.0f, 0.0f);
	}
	else if ( pisoId == 6 )
	{
		gluLookAt(CamPos[ 2 ][ 0 ], CamPos[ 2 ][ 1 ],CamPos[ 2 ][ 2 ], CamPos[ 2 ][ 3 ], CamPos[ 2 ][ 4 ], CamPos[ 2 ][ 5 ], 0.0f, 1.0f, 0.0f);
	}
	else if ( pisoId == 8 )
	{
		gluLookAt(CamPos[ 3 ][ 0 ], CamPos[ 3 ][ 1 ],CamPos[ 3 ][ 2 ], CamPos[ 3 ][ 3 ], CamPos[ 3 ][ 4 ], CamPos[ 3 ][ 5 ], 0.0f, 1.0f, 0.0f);
	}
	else if ( pisoId == 10 )
	{
		gluLookAt( CamPos[ 0 ][ 0 ] - CamPos[ 1 ][ 0 ], CamPos[ 4 ][ 1 ],CamPos[ 4 ][ 2 ], CamPos[ 4 ][ 3 ], CamPos[ 4 ][ 4 ], CamPos[ 0 ][ 5 ], 0.0f, 1.0f, 0.0f );
	}
	else
		gluLookAt(player1.PosicionCam.x, player1.PosicionCam.y, player1.PosicionCam.z, 
			  player1.ObjetivoCam.x, player1.ObjetivoCam.y, player1.ObjetivoCam.z, 
			  0.0f, 1.0f, 0.0f);
}
int RenderizaEscena(GLvoid)								// Aqui se dibuja todo lo que aparecera en la ventana
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();
	
	Camara();
	
	/*
	gluLookAt(player1.PosicionCam.x, player1.PosicionCam.y, player1.PosicionCam.z, 
			  player1.ObjetivoCam.x, player1.ObjetivoCam.y, player1.ObjetivoCam.z, 
			  0.0f, 1.0f, 0.0f);
	*/

	//Se actualizan los parámetros de iluminación
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);		// Posicion de la luz1
	glLightfv(GL_LIGHT0, GL_AMBIENT,  LightAmb);		// Componente ambiente
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  LightDif);		// Componente difusa
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpc);		// Componente especular
	
	ActualizaLuz();
		
	// Se dibuja el modelo con la iluminación desactivada
	// Se renderizan todas las partes oscuras de la escena.
	glDisable(GL_LIGHT0);

	// Mayralol
	glPushMatrix();
		glTranslatef(40.0f, 10.0f,-35.0f);
		glScalef(1.4f,1.4f,1.4f);
		DibujaEscena();
	glPopMatrix();
	
	// Dibujo de MJs
	glPushMatrix();
		DibujaMJ();
		glTranslatef(40.0f, 10.0f,-35.0f);
		DibujaEnemigos();
	glPopMatrix();

	// Se desactiva la máscara de color para renderizar la escena en negro
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	//Se desactiva la escritura en el buffer de profundidad
	glDepthMask(GL_FALSE);
	// Se activa el face culling
	glEnable(GL_CULL_FACE);
	// Se habilita la prueba del buffer stencil
	glEnable(GL_STENCIL_TEST);
	
	//Se inicializa el buffer stencil con una máscara de ceros.
	glStencilFunc(GL_ALWAYS, 0, 0);

	//En este punto se aplica el depth pass (z-pass) o depth fail (z-fail)
	//DibujaSombraPersonaje();
			
	// Se habilitan de nuevo los buffers de profundidad y color.
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	//Se reestablecen los valores default de operaciones del stencil buffer y de face culling.
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);

	// Finalmente se dibuja todo lo que está en las áreas iluminadas de la escena. Para eso ahora
	// Los puntos que se dibujan son todos aquellos en donde el buffer stencil es cero, es decir, las
	// partes de la escena que no estan en el volumen de sombra.
	glEnable(GL_LIGHT0);
	glStencilFunc(GL_EQUAL, 0, ~0);

	// Mayralol
	glPushMatrix();
		glTranslatef(40.0f, 10.0f,-35.0f);
		glScalef(1.4f,1.4f,1.4f);
		DibujaEscena();
	glPopMatrix();

	// Dibujo de MJs
	glPushMatrix();
		DibujaMJ();
		glTranslatef(40.0f, 10.0f,-35.0f);
		DibujaEnemigos();
		DibujaEnemigos();
	glPopMatrix();

	// Se desactiva la prueba de profundidad y del buffer stencil ya que no se utilizarán mas.
	glDisable(GL_STENCIL_TEST);
		
	DibujaLuz(lightPosition);
	DibujaTextos();
	
	// Esta parte opcional muestra la silueta extruida que crea el volumen de sombra.
	if(displayVolume == true)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);     //Para que muestre el volumen en alambrado
		//DibujaVolumendeSombra();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);     //Volvemos al modo sólido de nuevo
	}

	CalculateFrameRate();

	// Colisiones
	ActualizaObjetosDinamicosColision();
	DibujaObjetosdeColision();
	DibujaEsferasColision();
	ColisionesPiso();

	return TRUE;
}

GLvoid KillGLWindow(GLvoid)								// Elimina la ventana apropiadamente
{
	if (hRC)											// Si existe un contexto de renderizado...
	{
		if (!wglMakeCurrent(NULL,NULL))					// Si no se pueden liberar los contextos DC y RC...
		{
			MessageBox(NULL,"Falla al liberar DC y RC.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Si no se puede eliminar el RC?
		{
			MessageBox(NULL,"Falla al liberar el contexto de renderizado.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Se pone RC en NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Si no se puede eliminar el DC
	{
		MessageBox(NULL,"Falla al liberar el contexto de renderizado.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Se pone DC en NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Si no se puede destruir la ventana
	{
		MessageBox(NULL,"No se pudo liberar hWnd.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Se pone hWnd en NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Si no se puede eliminar el registro de la clase
	{
		MessageBox(NULL,"No se pudo eliminar el registro de la clase.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Se pone hInstance en NULL
	}
}

//	Este código crea la ventana de OpenGL.  Parámetros:					
//	title			- Titulo en la parte superior de la ventana			
//	width			- Ancho de la ventana								
//	height			- Alto de la ventana								
//	bits			- Número de bits a usar para el color (8/16/24/32)	
  
BOOL CreateGLWindow(char* title, int width, int height, int bits)
{
	GLuint	PixelFormat;				// Guarda el resultado despues de determinar el formato a usar
	WNDCLASS	wc;						// Estructura de la clase ventana
	DWORD		dwExStyle;				// Estilo extendido de ventana
	DWORD		dwStyle;				// Estilo de ventana
	RECT		WindowRect;				// Guarda los valores Superior Izquierdo / Inferior Derecho del rectángulo
	WindowRect.left=(long)0;			// Inicia el valor Izquierdo a 0
	WindowRect.right=(long)width;		// Inicia el valor Derecho al ancho especificado
	WindowRect.top=(long)0;				// Inicia el valor Superior a 0
	WindowRect.bottom=(long)height;		// Inicia el valor Inferior al alto especificado

	hInstance			= GetModuleHandle(NULL);				// Guarda una instancia de la ventana
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redibuja el contenido de la ventana al redimensionarla
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// Maneja los mensajes para WndProc
	wc.cbClsExtra		= 0;									// Ningun dato extra para la clase
	wc.cbWndExtra		= 0;									// Ningun dato extra para la ventana
	wc.hInstance		= hInstance;							// Inicia la instancia
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Carga el ícono por defecto
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Carga el puntero de flecha
	wc.hbrBackground	= NULL;									// No se requiere ningun fondo
	wc.lpszMenuName		= NULL;									// No hay menú en la ventana
	wc.lpszClassName	= "OpenGL";								// Fija el nombre de la clase.

	if (!RegisterClass(&wc))									// Intenta registrar la clase de ventana
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											
	}
		
	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;					// Estilo extendido de ventana
	dwStyle=WS_OVERLAPPEDWINDOW;									// Estilo de ventana

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Ajusta la ventana al tamaño especificado

	// Crea la ventana
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Estilo extendido para la ventana
								"OpenGL",							// Nombre de la clase
								title,								// Título de la ventana
								dwStyle |							// Definición del estilo de la ventana
								WS_CLIPSIBLINGS |					// Estilo requerido de la ventana
								WS_CLIPCHILDREN,					// Estilo requerido de la ventana
								0, 0,								// Posición de la ventana
								WindowRect.right-WindowRect.left,	// Calcula el ancho de la ventana
								WindowRect.bottom-WindowRect.top,	// Calcula el alto de la ventana
								NULL,								// No hay ventana superior
								NULL,								// No hay menú
								hInstance,							// Instancia
								NULL)))								// No se pasa nada a WM_CREATE
	{
		KillGLWindow();												// Resetea el despliegue
		MessageBox(NULL,"Error al crear la ventana.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		1,											// 16Bit Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Si no se creo el contexto de dispositivo...
	{
		KillGLWindow();								// Resetea el despliegue
		MessageBox(NULL,"No se puede crear un contexto de dispositivo GL.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Si Windows no encontró un formato de pixel compatible
	{
		KillGLWindow();								// Resetea el despliegue
		MessageBox(NULL,"No se puede encontrar un formato de pixel compatible.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Si no se pudo habilitar el formato de pixel
	{
		KillGLWindow();								// Resetea el despliegue
		MessageBox(NULL,"No se puede usar el formato de pixel.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if (!(hRC=wglCreateContext(hDC)))				// Si no se creo el contexto de renderizado
	{
		KillGLWindow();								// Resetea el despliegue
		MessageBox(NULL,"No se puede crear un contexto de renderizado GL.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!wglMakeCurrent(hDC,hRC))					// Si no se puede activar el contexto de renderizado
	{
		KillGLWindow();								// Resetea el despliegue
		MessageBox(NULL,"No se puede usar el contexto de renderizado GL.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	ShowWindow(hWnd,SW_SHOW);				// Muestra la ventana
	SetForegroundWindow(hWnd);				// Le da la prioridad mas alta
	SetFocus(hWnd);							// Pasa el foco del teclado a la ventana
	ReSizeGLScene(width, height);			// Inicia la perspectiva para la ventana OGL

	if (!InitGL())							// Si no se inicializa la ventana creada
	{
		KillGLWindow();						// Resetea el despliegue
		MessageBox(NULL,"Falla en la inicialización.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	return TRUE;							// Todo correcto
}

LRESULT CALLBACK WndProc(	HWND	hWnd,	// Manejador para esta ventana
							UINT	uMsg,	// Mensaje para esta ventana
							WPARAM	wParam,	// Información adicional del mensaje
							LPARAM	lParam)	// Información adicional del mensaje
{
	switch (uMsg)							// Revisa los mensajes de la ventana
	{
		case WM_ACTIVATE:					// Revisa el mensaje de activación de ventana
		{
			if (!HIWORD(wParam))			// Revisa el estado de minimización
			{
				active=TRUE;				// El programa está activo
			}
			else
			{
				active=FALSE;				// El programa no está activo
			}

			return 0;						// Regresa al ciclo de mensajes
		}

		case WM_SYSCOMMAND:					// Intercepta comandos del sistema
		{
			switch (wParam)					// Revisa llamadas del sistema
			{
				case SC_SCREENSAVE:			// ¿Screensaver tratando de iniciar?
				case SC_MONITORPOWER:		// ¿Monitor tratando de entrar a modo de ahorro de energía?
				return 0;					// Evita que suceda
			}
			break;							// Sale del caso
		}

		case WM_CLOSE:						// Si se recibe un mensaje de cerrar...
		{
			PostQuitMessage(0);				// Se manda el mensaje de salida
			return 0;						// y se regresa al ciclo
		}

		case WM_KEYDOWN:					// Si se está presionando una tecla...
		{
			keys[wParam] = TRUE;			// Si es así, se marca como TRUE
			return 0;						// y se regresa al ciclo
		}

		case WM_KEYUP:						// ¿Se ha soltado una tecla?
		{
			keys[wParam] = FALSE;			// Si es así, se marca como FALSE
			return 0;						// y se regresa al ciclo
		}

		case WM_SIZE:						// Si se redimensiona la ventana...
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  	// LoWord=Width, HiWord=Height
			return 0;						// y se regresa al ciclo
		}
	}

	// Pasa todos los mensajes no considerados a DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

// Este es el punto de entrada al programa; la función principal 
int WINAPI WinMain(	HINSTANCE	hInstance,			// Instancia
					HINSTANCE	hPrevInstance,		// Instancia previa
					LPSTR		lpCmdLine,			// Parametros de la linea de comandos
					int			nCmdShow)			// Muestra el estado de la ventana
{
	MSG		msg;									// Estructura de mensajes de la ventana
	BOOL	done=FALSE;								// Variable booleana para salir del ciclo

	//variables de fmod
	FMOD_SYSTEM      *system;
	FMOD_RESULT       result;
    unsigned int      version;

	unsigned int ms = 0;
    unsigned int lenms = 0;
    int          playing = 0;
    int          paused = 0;
    int          channelsplaying = 0;

	// Crea la ventana OpenGL
	if (!CreateGLWindow("Computación Gráfica",640,480,32))
	{
		return 0;									// Salir del programa si la ventana no fue creada
	}

	//Se inicia el sistema de sonido de FMOD
	result = FMOD_System_Create(&system);
    ERRCHECK(result);

    result = FMOD_System_GetVersion(system, &version);
    ERRCHECK(result);

    if (version < FMOD_VERSION)
    {
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
        return 0;
    }

    result = FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, NULL);
    ERRCHECK(result);

	////Carga de los archivos de audio
 //   result = FMOD_System_CreateSound(system, "Audio/iteza.mp3", FMOD_HARDWARE, 0, &sound[0]);
 //   ERRCHECK(result);
 //   result = FMOD_Sound_SetMode(sound[0], FMOD_LOOP_NORMAL); //Se repetira en un loop
 //   ERRCHECK(result);										

 //   result = FMOD_System_CreateSound(system, "Audio/explode.wav", FMOD_SOFTWARE, 0, &sound[1]);
 //   ERRCHECK(result);

	////Asignación a canales y configuración
	////Música de fondo
	//result = FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound[0], 0, &channel[0]);
 //   ERRCHECK(result);
	//result = FMOD_Channel_SetPaused(channel[0], FALSE); //Inicialmente activo
	//ERRCHECK(result);
	//result = FMOD_Channel_SetVolume(channel[0], 0.3f);
	//ERRCHECK(result);

	//result = FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound[1], 0, &channel[1]);
 //   ERRCHECK(result);
	//result = FMOD_Channel_SetPaused(channel[1], TRUE); //Inicialmente pausado
	//ERRCHECK(result);

	TimerInit();

	while(!done)									// Mientras done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Revisa si hay mensajes en espera
		{
			if (msg.message==WM_QUIT)				// Si se ha recibido el mensje de salir...
			{
				done=TRUE;							// Entonces done=TRUE
			}
			else									// Si no, Procesa los mensajes de la ventana
			{
				TranslateMessage(&msg);				// Traduce el mensaje
				DispatchMessage(&msg);				// Envia el mensaje
			}
		}
		else										// Si no hay mensajes...
		{
			//FMOD_System_Update(system);

			// Dibuja la escena. 
			if (active)								// Si está activo el programa...
			{
				float start=TimerGetTime();

				if (keys[VK_ESCAPE])				// Si se ha presionado ESC
				{
					done=TRUE;						// ESC indica el termino del programa
				}
				else								// De lo contrario, actualiza la pantalla
				{
					RenderizaEscena();				// Dibuja la escena
					if(play)
					{
						if(tipoAnim == 1)
							animacion(KeyFrame1, maxKF1 , 18);
					}
					SwapBuffers(hDC);				// Intercambia los Buffers (Double Buffering)
				}

				if(!ManejaTeclado()) return 0;
				/*ManejaTeclado(system, result);*/

				while(TimerGetTime()<start+float(steps[adjust]*2.0f)) {}	// Desperdicia ciclos si es demasiado rápido
			}
			
		}
	}

	// Finalización del programa
	//DescargaTexturas();
	DescargaModelos();
	DestruyeListas();
	Font.DestroyFont();
	/*LiberaSonido(system, result);*/
	KillGLWindow();									// Destruye la ventana
	return (msg.wParam);							// Sale del programa
}

int ManejaTeclado()
{
	if(GetKeyState(VK_UP) & 0x80)
	{
		player1.Dir=1;

		ControlPersonaje(3);
		
		if(play==false)
		{
			player1modelo.Angt1    = KeyFrame1[0].Angt1;
			player1modelo.Angt2    = KeyFrame1[0].Angt2;
			player1modelo.Angc1    = KeyFrame1[0].Angc1;
			player1modelo.Angc2    = KeyFrame1[0].Angc2;
			player1modelo.Angbi1   = KeyFrame1[0].Angbi1;
			player1modelo.Angbi2   = KeyFrame1[0].Angbi2;
			player1modelo.Angbib   = KeyFrame1[0].Angbib;
			player1modelo.Angbd1   = KeyFrame1[0].Angbd1;
			player1modelo.Angbd2   = KeyFrame1[0].Angbd2;
			player1modelo.Angbdb   = KeyFrame1[0].Angbdb;
			player1modelo.Angpizq  = KeyFrame1[0].Angpizq;
			player1modelo.Angpder  = KeyFrame1[0].Angpder;
			player1modelo.Angpizqb = KeyFrame1[0].Angpizqb;
			player1modelo.Angpderb = KeyFrame1[0].Angpderb;
			player1modelo.Angpi    = KeyFrame1[0].Angpi;
			player1modelo.Angpd    = KeyFrame1[0].Angpd;
			player1modelo.Xtor     = KeyFrame1[0].Xtor;
			player1modelo.Ytor     = KeyFrame1[0].Ytor;
			player1modelo.Ztor     = KeyFrame1[0].Ztor;

			play=true;
			playIndex=0;
			tipoAnim=1;
			
		}
	}

	if(GetKeyState(VK_DOWN) & 0x80)
	{
		player1.Dir=2;

		ControlPersonaje(4);

		if(play==false)
		{
			player1modelo.Angt1    = KeyFrame1[0].Angt1;
			player1modelo.Angt2    = KeyFrame1[0].Angt2;
			player1modelo.Angc1    = KeyFrame1[0].Angc1;
			player1modelo.Angc2    = KeyFrame1[0].Angc2;
			player1modelo.Angbi1   = KeyFrame1[0].Angbi1;
			player1modelo.Angbi2   = KeyFrame1[0].Angbi2;
			player1modelo.Angbib   = KeyFrame1[0].Angbib;
			player1modelo.Angbd1   = KeyFrame1[0].Angbd1;
			player1modelo.Angbd2   = KeyFrame1[0].Angbd2;
			player1modelo.Angbdb   = KeyFrame1[0].Angbdb;
			player1modelo.Angpizq  = KeyFrame1[0].Angpizq;
			player1modelo.Angpder  = KeyFrame1[0].Angpder;
			player1modelo.Angpizqb = KeyFrame1[0].Angpizqb;
			player1modelo.Angpderb = KeyFrame1[0].Angpderb;
			player1modelo.Angpi    = KeyFrame1[0].Angpi;
			player1modelo.Angpd    = KeyFrame1[0].Angpd;
			player1modelo.Xtor     = KeyFrame1[0].Xtor;
			player1modelo.Ytor     = KeyFrame1[0].Ytor;
			player1modelo.Ztor     = KeyFrame1[0].Ztor;

			play=true;
			playIndex=0;
			tipoAnim=1;
			
		}
	}

	if(GetKeyState(VK_LEFT) & 0x80)
	{
		ControlPersonaje(2);

		if(play==false)
		{
			player1modelo.Angt1    = KeyFrame1[0].Angt1;
			player1modelo.Angt2    = KeyFrame1[0].Angt2;
			player1modelo.Angc1    = KeyFrame1[0].Angc1;
			player1modelo.Angc2    = KeyFrame1[0].Angc2;
			player1modelo.Angbi1   = KeyFrame1[0].Angbi1;
			player1modelo.Angbi2   = KeyFrame1[0].Angbi2;
			player1modelo.Angbib   = KeyFrame1[0].Angbib;
			player1modelo.Angbd1   = KeyFrame1[0].Angbd1;
			player1modelo.Angbd2   = KeyFrame1[0].Angbd2;
			player1modelo.Angbdb   = KeyFrame1[0].Angbdb;
			player1modelo.Angpizq  = KeyFrame1[0].Angpizq;
			player1modelo.Angpder  = KeyFrame1[0].Angpder;
			player1modelo.Angpizqb = KeyFrame1[0].Angpizqb;
			player1modelo.Angpderb = KeyFrame1[0].Angpderb;
			player1modelo.Angpi    = KeyFrame1[0].Angpi;
			player1modelo.Angpd    = KeyFrame1[0].Angpd;
			player1modelo.Xtor     = KeyFrame1[0].Xtor;
			player1modelo.Ytor     = KeyFrame1[0].Ytor;
			player1modelo.Ztor     = KeyFrame1[0].Ztor;

			play=true;
			playIndex=0;
			tipoAnim=1;
			
		}
	}

	if(GetKeyState(VK_RIGHT) & 0x80)
	{
		ControlPersonaje(1);

		if(play==false)
		{
			player1modelo.Angt1    = KeyFrame1[0].Angt1;
			player1modelo.Angt2    = KeyFrame1[0].Angt2;
			player1modelo.Angc1    = KeyFrame1[0].Angc1;
			player1modelo.Angc2    = KeyFrame1[0].Angc2;
			player1modelo.Angbi1   = KeyFrame1[0].Angbi1;
			player1modelo.Angbi2   = KeyFrame1[0].Angbi2;
			player1modelo.Angbib   = KeyFrame1[0].Angbib;
			player1modelo.Angbd1   = KeyFrame1[0].Angbd1;
			player1modelo.Angbd2   = KeyFrame1[0].Angbd2;
			player1modelo.Angbdb   = KeyFrame1[0].Angbdb;
			player1modelo.Angpizq  = KeyFrame1[0].Angpizq;
			player1modelo.Angpder  = KeyFrame1[0].Angpder;
			player1modelo.Angpizqb = KeyFrame1[0].Angpizqb;
			player1modelo.Angpderb = KeyFrame1[0].Angpderb;
			player1modelo.Angpi    = KeyFrame1[0].Angpi;
			player1modelo.Angpd    = KeyFrame1[0].Angpd;
			player1modelo.Xtor     = KeyFrame1[0].Xtor;
			player1modelo.Ytor     = KeyFrame1[0].Ytor;
			player1modelo.Ztor     = KeyFrame1[0].Ztor;

			play=true;
			playIndex=0;
			tipoAnim=1;
			
		}
	}

	if(!(GetKeyState(VK_UP) & 0x80 || GetKeyState(VK_DOWN) & 0x80
		 || GetKeyState(VK_LEFT) & 0x80  || GetKeyState(VK_RIGHT) & 0x80))
	{
		player1.Dir=0;

		play=false;

		player1modelo.Angt1    = 0.0f;
		player1modelo.Angt2    = 0.0f;
		player1modelo.Angc1    = 0.0f;
		player1modelo.Angc2    = 0.0f;
		player1modelo.Angbi1   = 0.0f;
		player1modelo.Angbi2   = 0.0f;
		player1modelo.Angbib   = 0.0f;
		player1modelo.Angbd1   = 0.0f;
		player1modelo.Angbd2   = 0.0f;
		player1modelo.Angbdb   = 0.0f;
		player1modelo.Angpizq  = 0.0f;
		player1modelo.Angpder  = 0.0f;
		player1modelo.Angpizqb = 0.0f;
		player1modelo.Angpderb = 0.0f;
		player1modelo.Angpi    = 0.0f;
		player1modelo.Angpd    = 0.0f;
		player1modelo.Xtor     = 0.0f;
		player1modelo.Ytor     = 0.0f;
		player1modelo.Ztor     = 0.0f;
		
	}
		

	if(keys[VK_PRIOR])
	{
		if(player1.PosicionObj.y < 200.0f)
		{
			player1.PosicionObj.y+=0.4f;
			player1.CamaraPosAlt+=0.4f;
			player1.CamaraObjAlt+=0.4f;

			player1.PosicionCam.y=player1.CamaraPosAlt;
			player1.ObjetivoCam=player1.PosicionObj;
			player1.ObjetivoCam.y=player1.CamaraObjAlt;
		}
	}
	if(keys[VK_NEXT])
	{
		if(player1.PosicionObj.y > 0.0f)
		{
			player1.PosicionObj.y -= 0.4f;
			player1.CamaraPosAlt -= 0.4f;
			player1.CamaraObjAlt -= 0.4f;

			player1.PosicionCam.y = player1.CamaraPosAlt;
			player1.ObjetivoCam = player1.PosicionObj;
			player1.ObjetivoCam.y = player1.CamaraObjAlt;
		}
	}

	if(keys[VK_HOME])
	{
		ControlPersonaje(5);
	}

	if(keys[VK_END])
	{
		ControlPersonaje(6);
	}

	//Controles de la iluminación
	if (keys['Z'])
		LightPos[0] += 1.0f; //Hacia la derecha

	if (keys['X'])
		LightPos[0] -= 1.0f; //Hacia la izquierda

	if (keys['C'])
		LightPos[1] += 1.0f; //Hacia arriba

	if (keys['V'])
		LightPos[1] -= 1.0f; //Hacia abajo

	if (keys['B'])
		LightPos[2] += 1.0f; //Hacia adelante

	if (keys['N'])
		LightPos[2] -= 1.0f; //Hacia atrás

	if((GetAsyncKeyState(VK_RETURN)&1) ==1)
	{
		if(displayVolume == false)
			displayVolume=true;
		else
			displayVolume=false;
	}

	return TRUE;
}

//Control de sonidos por teclado
void ManejaTeclado(FMOD_SYSTEM *system, FMOD_RESULT result)
{
	if((GetAsyncKeyState(VK_SPACE)&1) ==1) //Otra forma de usar el teclado: Detecta el evento de pulsar la tecla
	{
		result = FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound[1], 0, &channel[1]);
        ERRCHECK(result);
	}
	
}