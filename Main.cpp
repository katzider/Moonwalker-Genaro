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
float CamPos[4][6] = { 
	{ 190.0f, 90.0f, 10.0f, 180.0f, 6.5f, -60.0f }, 
	{ 45.0f, 90.0f, 10.0f, 55.0f, 6.5f, -60.0f }, 
	{ 15.0f, 90.0f, -56.0f, -50.0f, 6.5f, -56.0f }, 
	{ 35.0f, 90.0f, 75.0f, -60.0f, 6.5f, 80.0f }, 
};

int pisoId = 0;

// Variable de acceso a la estructura de parametros
parametros player1;
parametros miku;

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

#define FILE_NAME1miku "Modelos/miku/miku_torso.3ds"
#define FILE_NAME2miku "Modelos/miku/miku_pder.3ds"
#define FILE_NAME3miku "Modelos/miku/miku_pizq.3ds"
#define FILE_NAME4miku "Modelos/miku/miku_bder.3ds"
#define FILE_NAME5miku "Modelos/miku/miku_bizq.3ds"

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

// Para el escenario 
#define FILE_NAME1e  "Modelos/escenario1.3ds"

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
#define FILE_NAME3k	 "Modelos/Ene2pers.3ds"

//nombre y ubicación de modelo tarantula
#define FILE_NAME1t	 "Modelos/tarantula.3ds"

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

CTga textureModel1miku[20]; //miku.tga
CTga textureModel2miku[20];
CTga textureModel3miku[20];
CTga textureModel4miku[20];
CTga textureModel5miku[20];

// Contenedores de texturas del escenario
CTga textureModel1e[20];
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

//Contenedor de texturas de tarantula
CTga textureModel1t[20];

CLoad3DS g_Load3ds;
CShader cel_Shader;

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

//Acceso a la estructura que almacena los datos de los modelos (MJ y miku)
t3DModel g_3DModel1aru;
t3DModel g_3DModel2aru;
t3DModel g_3DModel3aru;
t3DModel g_3DModel4aru;
t3DModel g_3DModel5aru;
t3DModel g_3DModel6aru;
t3DModel g_3DModel7aru;
t3DModel g_3DModel8aru;
t3DModel g_3DModel9aru;

t3DModel g_3DModel1miku;
t3DModel g_3DModel2miku;
t3DModel g_3DModel3miku;
t3DModel g_3DModel4miku;
t3DModel g_3DModel5miku;

// Acceso a la estructura que almacena los datos del escenario
t3DModel g_3DModel1e;

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


//Acceso a la estructura que almacena a tarantula
t3DModel g_3DModel1t;

//Contenedor de texturas adicionales
CTga textura[30];

jerarquiaModelo player1modelo;	//Acceso a la estructura con las variables de cada pieza del modelo
jerarquiaModelo enem3amodelo;
jerarquiaModelo enem3bmodelo;
jerarquiaModelo changmodelo;
jerarquiaModelo enem1modelo;
jerarquiaModelo enem2modelo;
jerarquiaModelo MJ6modelo;

/* Declaracion de keyframes, aqui puse todos los keyframes que chance usaran
 * ya le puse un comentario a quien le pertenece cada frame, si requieren mas,
 * agreguenlas despues de las que ya estan aqui 
 */

// Animacion de caminar de MJ
const int maxKF1=3;				//Num. total de KeyFrames para la secuencia 1 (caminar)
FRAME KeyFrame1[maxKF1];		//Contenedor para almacenar cada keyframe de la secuencia 1

// Animacion Miku
const int maxKF2 = 3;
FRAME KeyFrame2[maxKF2];

// Animacion enem1
const int maxKF3 = 3;
FRAME KeyFrame3[maxKF3];

// Animacion enem2
const int maxKF4 = 3;
FRAME KeyFrame4[maxKF4];

// Animacion MJ6modelo
const int maxKF5 = 3;
FRAME KeyFrame5[maxKF5];

// Animacion enem3A
const int maxKF6 = 3;
FRAME KeyFrame6[maxKF6];

// Animacion enem3B
const int maxKF7 = 3;
FRAME KeyFrame7[maxKF7];

// Animacion changmodelo
const int maxKF8 = 3;
FRAME KeyFrame8[maxKF8];

// Miku
jerarquiaModelo mikumodelo;

bool play=false;//Bandera para iniciar la animación
int playIndex = 0;//Auxiliar para leer la información del contenedor de keyframes
int tipoAnim = 1; //Indicador del tipo de animación

// Banderas para iniciar las demas animaciones
const int otros = 8;

bool playOtros[ otros ] = { false };
int playIndexOtros[ otros ] = { 0 };
int tipoAnimOtros[ otros ] = { 1 };

CMultitexturas Multitext;

// Modelos enemigos y chango
GLuint modelo1;
GLuint modelo1out;
GLuint ene1;
GLuint ene1out;
GLuint noMJ6;
GLuint noMJ6out;
GLuint ene2;
GLuint ene2out;
GLuint ene3a;
GLuint ene3aout;
GLuint ene3b;
GLuint ene3bout;
GLuint cha;
GLuint chaout; 
GLuint taran;
GLuint taranout; 

// las listas savage
GLuint enemigo8L;
GLuint enemigo8Lout;

// Declara enteros para los modelos de Aru
GLuint modelo1aru;
GLuint modelo1aruout;
GLuint modelo1miku;
GLuint modelo1mikuout;

//Constantes de iluminación y materiales
GLfloat LightPos[] = { 200.0f, 20.0f, 0.0f, 1.0f};		// Posición de la luz
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

//Variables del spline
int running;
int trayectoria;
int esperar;

float idxtp, dtidx;
int ptsXtramo;
int aw, ah;

int AuxT;

typedef GLfloat TPoint[3];

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

struct spline {
	TPoint *ctrlpoints;
	int	    tpc;
	int	    drawp;
	int	    prec;
};

spline helspline;
spline camspline;
TPoint target;

TPoint helsplinepoints[] = {
	{ 181.5f, 20.0f, -67.0f},
	{ 181.5f, 20.0f, -67.0f},
	{ 150.0f, 20.0f, -41.0f},
	{ 130.0f, 20.0f, -53.0f},
	{ 130.0f, 20.0f, -81.0f},
	{ 150.0f, 20.0f, -93.0f},
	{ 181.5f, 20.0f, -67.0f},
	{ 211.0f, 20.0f, -93.0f},
	{ 231.0f, 20.0f, -81.0f},
	{ 231.0f, 20.0f, -53.0f},
	{ 211.0f, 20.0f, -41.0f},
	{ 181.5f, 20.0f, -67.0f},
	{ 181.5f, 20.0f, -67.0f},
};

TPoint camsplinepoints[] = {
	{ 245.0f, 90.0f, -64.0f},
	{ 240.5f, 90.0f, -64.0f},
	{ 200.0f, 90.0f, -64.0f},
	{ 150.4f, 90.0f, -64.0f},
	{ 125.0f, 90.0f, -64.0f},
	{ 100.0f, 90.0f, -64.0f},
	{  60.5f, 90.0f, -64.0f},
	{  20.0f, 90.0f, -64.0f},
	{   5.0f, 90.0f, -64.0f},
	{   0.5f, 90.0f, -64.0f},
	{ -10.5f, 90.0f, -64.0f},
	{ -57.5f, 90.0f, -64.0f},
	{ -57.5f, 90.0f,  50.4f},
	{ -57.5f, 90.0f, 100.5f},
	{ -57.5f, 90.0f, 150.5f},
	{ -57.5f, 90.0f, 200.0f},
};

#define totalCP  (sizeof(helsplinepoints)/sizeof(TPoint))
#define totalCP1 (sizeof(camsplinepoints)/sizeof(TPoint))

void spline_init( spline &sp, TPoint * ctrl, int tot, int res )
{
	sp.ctrlpoints = ctrl;
	sp.prec = res;
	sp.tpc = tot;
	sp.drawp = (tot-3)*res;
}

void spline_point( spline &sp, int indice,  TPoint P)
{
    int i, j;
	GLfloat t, t3, t2, c1, c2, c3, c4, _1_t;
	GLfloat * Pj3, * Pj2, * Pj1, * Pj0;

    indice = indice % sp.drawp;
	j = indice / sp.prec + 3;
	i = indice % sp.prec;
	Pj3=sp.ctrlpoints[j-3];
	Pj2=sp.ctrlpoints[j-2];
	Pj1=sp.ctrlpoints[j-1];
	Pj0=sp.ctrlpoints[j-0];
	t = i/(double)sp.prec;
	t2 = t*t; 
	t3 = t2*t;
	_1_t = 1-t;
	c1 = (_1_t*_1_t*_1_t) / 6;
	c2 = (3*t3-6*t2+4) / 6;
	c3 = (-3*t3+3*t2+3*t+1) / 6;
	c4 = t3 / 6;
	P[0] = c1* Pj3[0] + c2*Pj2[0] + c3*Pj1[0] + c4*Pj0[0];
	P[1] = c1* Pj3[1] + c2*Pj2[1] + c3*Pj1[1] + c4*Pj0[1];
	P[2] = c1* Pj3[2] + c2*Pj2[2] + c3*Pj1[2] + c4*Pj0[2];
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
	textura[1].LoadTGA("Texturas/MJi.tga");
}

void DescargaTexturas()
{
	textura[0].Elimina();
	textura[1].Elimina();
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
	if(!g_Load3ds.Load3DSFile(FILE_NAME1e, &g_3DModel1e, textureModel1e))
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

	// Miku
	if(!g_Load3ds.Load3DSFile(FILE_NAME1miku, &g_3DModel1miku, textureModel1miku ))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME2miku, &g_3DModel2miku, textureModel2miku ))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME3miku, &g_3DModel3miku, textureModel3miku ))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME4miku, &g_3DModel4miku, textureModel4miku ))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME5miku, &g_3DModel5miku, textureModel5miku ))
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
	if(!g_Load3ds.Load3DSFile(FILE_NAME3k, &g_3DModel3k, textureModel1k))
		return 0;

	//tarantula
	if(!g_Load3ds.Load3DSFile(FILE_NAME1t, &g_3DModel1t, textureModel1t))
		return 0;

	return TRUE;
}

void DescargaModelos()
{
	
	// descargamodelos savage
			
	g_Load3ds.UnLoad3DSFile(&g_3DModel1g, textureModel1g);
	g_Load3ds.UnLoad3DSFile(&g_3DModel2g, textureModel2g);
	g_Load3ds.UnLoad3DSFile(&g_3DModel3g, textureModel3g);

	// Escenario
	g_Load3ds.UnLoad3DSFile(&g_3DModel1e, textureModel1e);

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
	// Miku
	g_Load3ds.UnLoad3DSFile( &g_3DModel1miku, textureModel1miku );
	g_Load3ds.UnLoad3DSFile( &g_3DModel2miku, textureModel2miku );
	g_Load3ds.UnLoad3DSFile( &g_3DModel3miku, textureModel3miku );
	g_Load3ds.UnLoad3DSFile( &g_3DModel4miku, textureModel4miku );
	g_Load3ds.UnLoad3DSFile( &g_3DModel5miku, textureModel5miku );

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
	g_Load3ds.UnLoad3DSFile(&g_3DModel1k, textureModel1k);
	g_Load3ds.UnLoad3DSFile(&g_3DModel2k, textureModel2k);
	g_Load3ds.UnLoad3DSFile(&g_3DModel3k, textureModel3k);

	//tarantula
	g_Load3ds.UnLoad3DSFile(&g_3DModel1t, textureModel1t);
}

void IniSombraVolumen()
{
	//Establece la conectividad entre caras
	objSh.EstableceConectividad(&g_3DModel1c);
	objSh.calculaPlano(&g_3DModel1aru);

	objSh.EstableceConectividad(&g_3DModel2c);
	objSh.calculaPlano(&g_3DModel2aru);

	objSh.EstableceConectividad(&g_3DModel3c);
	objSh.calculaPlano(&g_3DModel3aru);

	objSh.EstableceConectividad(&g_3DModel4c);
	objSh.calculaPlano(&g_3DModel4aru);

	objSh.EstableceConectividad(&g_3DModel5c);
	objSh.calculaPlano(&g_3DModel5aru);

	objSh.EstableceConectividad(&g_3DModel6c);
	objSh.calculaPlano(&g_3DModel6aru);

	objSh.EstableceConectividad(&g_3DModel7c);
	objSh.calculaPlano(&g_3DModel7aru);

	objSh.EstableceConectividad(&g_3DModel8c);
	objSh.calculaPlano(&g_3DModel8aru);

	objSh.EstableceConectividad(&g_3DModel9c);
	objSh.calculaPlano(&g_3DModel9aru);
			
}

void CreaListas()
{
	//Ene1
	ene1=glGenLists(10);
	//Ene2
	ene2=glGenLists(3);
	//MJ6
	noMJ6=glGenLists(10);
	//Ene3a
	ene3a=glGenLists(5);
	//Ene3b
	ene3b=glGenLists(5);
	//chango
	cha=glGenLists(5);
	// en crea listas savage
	enemigo8L=glGenLists(3);
	//tarantula
	taran=glGenLists(1);

	//Ene1 contorno
	ene1out=glGenLists(10);
	//Ene2 contorno
	ene2out=glGenLists(3);
	//MJ6  contorno
	noMJ6out=glGenLists(10);
	//Ene3a  contorno
	ene3aout=glGenLists(5);
	//Ene3b contorno	
	ene3bout=glGenLists(5);
	//chango contorno
	chaout=glGenLists(5);
	// en crea listas savage contorno
	enemigo8Lout=glGenLists(3);
	//tarantula contorno
	taranout=glGenLists(1);

	glNewList(enemigo8L+0,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel1g, textureModel1g, 1);
	glEndList();

	glNewList(enemigo8L+1,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel2g, textureModel2g, 1);
	glEndList();

	glNewList(enemigo8L+2,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel3g, textureModel3g, 1);
	glEndList();


	glNewList(enemigo8Lout+0,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel1g);
	glEndList();

	glNewList(enemigo8Lout+1,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel2g);
	glEndList();

	glNewList(enemigo8Lout+2,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel3g);
	glEndList();


	// Crea listas para Aru
	modelo1aru = glGenLists(9);
	modelo1aruout = glGenLists(9);
	modelo1miku = glGenLists( 5 );
	modelo1mikuout = glGenLists( 5 );

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

	// Contorno
	glNewList(modelo1aruout+0,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel1aru);
	glEndList();

	glNewList(modelo1aruout+1,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel2aru);
	glEndList();

	glNewList(modelo1aruout+2,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel3aru);
	glEndList();

	glNewList(modelo1aruout+3,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel4aru);
	glEndList();

	glNewList(modelo1aruout+4,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel5aru);
	glEndList();

	glNewList(modelo1aruout+5,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel6aru);
	glEndList();

	glNewList(modelo1aruout+6,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel7aru);
	glEndList();

	glNewList(modelo1aruout+7,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel8aru);
	glEndList();

	glNewList(modelo1aruout+8,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel9aru);
	glEndList();

	// Miku
	glNewList( modelo1miku + 0, GL_COMPILE );
		g_Load3ds.Render3DSFile( &g_3DModel1miku, textureModel1miku, 1 );
	glEndList();

	glNewList( modelo1miku + 1, GL_COMPILE );
		g_Load3ds.Render3DSFile( &g_3DModel2miku, textureModel2miku, 1 );
	glEndList();
	
	glNewList( modelo1miku + 2, GL_COMPILE );
		g_Load3ds.Render3DSFile( &g_3DModel3miku, textureModel3miku, 1 );
	glEndList();
	
	glNewList( modelo1miku + 3, GL_COMPILE );
		g_Load3ds.Render3DSFile( &g_3DModel4miku, textureModel4miku, 1 );
	glEndList();
	
	glNewList( modelo1miku + 4, GL_COMPILE );
		g_Load3ds.Render3DSFile( &g_3DModel5miku, textureModel5miku, 1 );
	glEndList();

	// Miku contorno
	glNewList( modelo1mikuout + 0, GL_COMPILE );
		g_Load3ds.Render3DSContour( &g_3DModel1miku );
	glEndList();

	glNewList( modelo1mikuout + 1, GL_COMPILE );
		g_Load3ds.Render3DSContour( &g_3DModel2miku );
	glEndList();
	
	glNewList( modelo1mikuout + 2, GL_COMPILE );
		g_Load3ds.Render3DSContour( &g_3DModel3miku );
	glEndList();
	
	glNewList( modelo1mikuout + 3, GL_COMPILE );
		g_Load3ds.Render3DSContour( &g_3DModel4miku );
	glEndList();
	
	glNewList( modelo1mikuout + 4, GL_COMPILE );
		g_Load3ds.Render3DSContour( &g_3DModel5miku );
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


	glNewList(ene1out+0,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel1d);
	glEndList();

	glNewList(ene1out+1,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel2d);
	glEndList();

	glNewList(ene1out+2,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel3d);
	glEndList();

	glNewList(ene1out+3,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel4d);
	glEndList();

	glNewList(ene1out+4,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel5d);
	glEndList();

	glNewList(ene1out+5,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel6d);
	glEndList();

	glNewList(ene1out+6,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel7d);
	glEndList();

	glNewList(ene1out+7,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel8d);
	glEndList();

	glNewList(ene1out+8,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel9d);
	glEndList();

	glNewList(ene1out+9,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel10d);
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


	
	glNewList(noMJ6out+0,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel1f);
	glEndList();

	glNewList(noMJ6out+1,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel2f);
	glEndList();

	glNewList(noMJ6out+2,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel3f);
	glEndList();

	glNewList(noMJ6out+3,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel4f);
	glEndList();

	glNewList(noMJ6out+4,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel5f);
	glEndList();

	glNewList(noMJ6out+5,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel6f);
	glEndList();

	glNewList(noMJ6out+6,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel7f);
	glEndList();

	glNewList(noMJ6out+7,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel8f);
	glEndList();

	glNewList(noMJ6out+8,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel9f);
	glEndList();

	glNewList(noMJ6out+9,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel10f);
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


	glNewList(ene3aout+0,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel1h);
	glEndList();

	glNewList(ene3aout+1,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel2h);
	glEndList();

	glNewList(ene3aout+2,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel3h);
	glEndList();

	glNewList(ene3aout+3,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel4h);
	glEndList();

	glNewList(ene3aout+4,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel5h);
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


	glNewList(ene3bout+0,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel1i);
	glEndList();

	glNewList(ene3bout+1,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel2i);
	glEndList();

	glNewList(ene3bout+2,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel3i);
	glEndList();

	glNewList(ene3bout+3,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel4i);
	glEndList();

	glNewList(ene3bout+4,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel5i);
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


	glNewList(chaout+0,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel1j);
	glEndList();

	glNewList(chaout+1,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel2j);
	glEndList();

	glNewList(chaout+2,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel3j);
	glEndList();

	glNewList(chaout+3,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel4j);
	glEndList();

	glNewList(chaout+4,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel5j);
	glEndList();


	//Ene2
	glNewList(ene2+0,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel1k, textureModel1k, 1);
	glEndList();

	glNewList(ene2+1,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel2k, textureModel2k, 1);
	glEndList();

	glNewList(ene2+2,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel3k, textureModel3k, 1);
	glEndList();


	glNewList(ene2out+0,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel1k);
	glEndList();

	glNewList(ene2out+1,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel2k);
	glEndList();

	glNewList(ene2out+2,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel3k);
	glEndList();


	
	//tarantula
	glNewList(taran+0,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel1t, textureModel1t, 1);
	glEndList();



	glNewList(taranout+0,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel1t);
	glEndList();



}


void DestruyeListas()
{

	// Borra listas de Aru
	glDeleteLists(modelo1aru,9);
	glDeleteLists(modelo1miku,5);
	// Borra listas fahl
	glDeleteLists(ene1,10);
	glDeleteLists(ene2,3);
	glDeleteLists(noMJ6,10);

	glDeleteLists(ene3a,5);
	glDeleteLists(ene3b,5);
	glDeleteLists(cha,5);
	glDeleteLists(taran,5);

	// Borra listas de Aru
	glDeleteLists(modelo1aruout,9);
	glDeleteLists(modelo1mikuout,9);
	// Borra listas fahl
	glDeleteLists(ene1out,10);
	glDeleteLists(ene2out,3);
	glDeleteLists(noMJ6out,10);

	glDeleteLists(ene3aout,5);
	glDeleteLists(ene3bout,5);
	glDeleteLists(chaout,5);
	glDeleteLists(taranout,5);


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

	// Miku
	//Esta función establece los parámetros como velocidad del objeto y distancia de la cámara así como la posición y dirección iniciales
	miku.visible = true;
	miku.VelocidadObj = 0.2f;
	miku.DistanciaCam = 20.0f;

	miku.CamaraPosAlt = 10.0f;	//Posición en y de la cámara (altura a la que se situa la cámara)
	miku.CamaraObjAlt = 6.4f;	//Posición en y del objetivo de la cámara (altura a la que ve la cámara)
	miku.AngDir = 90.0f;		//Este ángulo inicial hace que la dirección inicial sea paralela al eje Z y con sentido negativo
	miku.AngObj = 0.0f;		//Este valor se elige dependiendo de la orientación con la que aparece el modelo en la escena al dibujarlo
								//sin aplicarle ninguna transformación (hacia adonde está volteando). Se elige un ángulo tal que al aplicarle
								//una rotación inicial con respecto al eje Y esté viendo hacia la misma dirección que la definida por AngDir
	
	miku.PosicionObj = CVector( 35.0f, 7.7f, -90.0f); //Esta es la posición inicial del objeto en la escena
	miku.Direccion.x = cosf( miku.AngDir * PI / 180.0f); //Dirección inicial definida por el ángulo inicial AngDir (x=cos(AngDir), y=0.0, z=sen(AngDir))
	miku.Direccion.y = 0.0f;
	miku.Direccion.z = sinf( miku.AngDir * PI / 180.0f);   
	miku.PosicionCam = CVector( miku.PosicionObj.x, miku.PosicionObj.y + miku.CamaraPosAlt, miku.PosicionObj.z +miku.DistanciaCam ); //Posición inicial de la cámara a [DistanciaCam] unidades detrás del objeto
	miku.ObjetivoCam = miku.PosicionObj;		//La cámara ve siempre al objeto
	miku.ObjetivoCam.y = miku.CamaraObjAlt;		//Para que no vea a los "pies" del objeto (personaje)

	miku.Dir=0;
	miku.DirAnt=0;

	miku.escalaX=0.8f;
	miku.escalaY=0.8f;
	miku.escalaZ=0.8f;
	
	miku.CamaraObjAltE=0.0f;

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
	
	enem1.PosicionObj=CVector(180.0f, 9.0f, -60.0f); //Esta es la posición inicial del objeto en la escena
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
	
	enem2.PosicionObj=CVector(120.0f, 10.0f, -80.0f); //Esta es la posición inicial del objeto en la escena
	enem2.Direccion.x=(float)cos(player1.AngDir*PI/180.0f); //Dirección inicial definida por el ángulo inicial AngDir (x=cos(AngDir), y=0.0, z=sen(AngDir))
	enem2.Direccion.y=0.0f;
	enem2.Direccion.z=(float)sin(player1.AngDir*PI/180.0f);   
	enem2.PosicionCam=CVector(0.0f, player1.CamaraPosAlt, 10.0f); //Posición inicial de la cámara a [DistanciaCam] unidades detrás del objeto
	enem2.ObjetivoCam=player1.PosicionObj;		//La cámara ve siempre al objeto
	enem2.ObjetivoCam.y=player1.CamaraObjAlt;		//Para que no vea a los "pies" del objeto (personaje)

	enem2.Dir=0;
	enem2.DirAnt=0;

	enem2.escalaX=0.5f;
	enem2.escalaY=0.5f;
	enem2.escalaZ=0.5f;

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
	
	MJ6.PosicionObj=CVector(-15.0f, 8.5f, -65.0f); //Esta es la posición inicial del objeto en la escena
	MJ6.Direccion.x=(float)cos(player1.AngDir*PI/180.0f); //Dirección inicial definida por el ángulo inicial AngDir (x=cos(AngDir), y=0.0, z=sen(AngDir))
	MJ6.Direccion.y=0.0f;
	MJ6.Direccion.z=(float)sin(player1.AngDir*PI/180.0f);   
	MJ6.PosicionCam=CVector(0.0f, player1.CamaraPosAlt, 10.0f); //Posición inicial de la cámara a [DistanciaCam] unidades detrás del objeto
	MJ6.ObjetivoCam=player1.PosicionObj;		//La cámara ve siempre al objeto
	MJ6.ObjetivoCam.y=player1.CamaraObjAlt;		//Para que no vea a los "pies" del objeto (personaje)

	MJ6.Dir=0;
	MJ6.DirAnt=0;

	MJ6.escalaX=0.7f;
	MJ6.escalaY=0.7f;
	MJ6.escalaZ=0.7f;

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
	
	enem3a.PosicionObj=CVector(-15.0f, 8.0f, -5.0f); //Esta es la posición inicial del objeto en la escena
	enem3a.Direccion.x=(float)cos(player1.AngDir*PI/180.0f); //Dirección inicial definida por el ángulo inicial AngDir (x=cos(AngDir), y=0.0, z=sen(AngDir))
	enem3a.Direccion.y=0.0f;
	enem3a.Direccion.z=(float)sin(player1.AngDir*PI/180.0f);   
	enem3a.PosicionCam=CVector(0.0f, player1.CamaraPosAlt, 10.0f); //Posición inicial de la cámara a [DistanciaCam] unidades detrás del objeto
	enem3a.ObjetivoCam=player1.PosicionObj;		//La cámara ve siempre al objeto
	enem3a.ObjetivoCam.y=player1.CamaraObjAlt;		//Para que no vea a los "pies" del objeto (personaje)

	enem3a.Dir=0;
	enem3a.DirAnt=0;

	enem3a.escalaX=0.3f;
	enem3a.escalaY=0.3f;
	enem3a.escalaZ=0.3f;

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
	
	enem3b.PosicionObj=CVector(-19.0f, 8.0f, -10.0f); //Esta es la posición inicial del objeto en la escena
	enem3b.Direccion.x=(float)cos(player1.AngDir*PI/180.0f); //Dirección inicial definida por el ángulo inicial AngDir (x=cos(AngDir), y=0.0, z=sen(AngDir))
	enem3b.Direccion.y=0.0f;
	enem3b.Direccion.z=(float)sin(player1.AngDir*PI/180.0f);   
	enem3b.PosicionCam=CVector(0.0f, player1.CamaraPosAlt, 10.0f); //Posición inicial de la cámara a [DistanciaCam] unidades detrás del objeto
	enem3b.ObjetivoCam=player1.PosicionObj;		//La cámara ve siempre al objeto
	enem3b.ObjetivoCam.y=player1.CamaraObjAlt;		//Para que no vea a los "pies" del objeto (personaje)

	enem3b.Dir=0;
	enem3b.DirAnt=0;

	enem3b.escalaX=0.3f;
	enem3b.escalaY=0.3f;
	enem3b.escalaZ=0.3f;

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
	
	chang.PosicionObj=CVector(-30.0f, 7.0f, -5.0f); //Esta es la posición inicial del objeto en la escena
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

void InicializaAnim( FRAME *KeyFrame, int maxKF, jerarquiaModelo* modelo )
{
	//Se inicializan las variables de la secuencia 1
	for(int i = 0; i < maxKF; i++)
	{
		KeyFrame[i].Angt1=0.0f;
		KeyFrame[i].Angt2=0.0f;
		KeyFrame[i].Angc1=0.0f;
		KeyFrame[i].Angc2=0.0f;
		KeyFrame[i].Angbi1=0.0f;
		KeyFrame[i].Angbi2=0.0f;
		KeyFrame[i].Angbib=0.0f;
		KeyFrame[i].Angbd1=0.0f;
		KeyFrame[i].Angbd2=0.0f;
		KeyFrame[i].Angbdb=0.0f;
		KeyFrame[i].Angpizq=0.0f;
		KeyFrame[i].Angpizqb=0.0f;
		KeyFrame[i].Angpder=0.0f;
		KeyFrame[i].Angpderb=0.0f;
		KeyFrame[i].Angpi=0.0f;
		KeyFrame[i].Angpd=0.0f;
		KeyFrame[i].Xtor=0.0f;
		KeyFrame[i].Ytor=0.0f;
		KeyFrame[i].Ztor=0.0f;

		KeyFrame[i].incAngt1=false;
		KeyFrame[i].incAngt1=false;
		KeyFrame[i].incAngc1=false;
		KeyFrame[i].incAngc2=false;
		KeyFrame[i].incAngbi1=false;
		KeyFrame[i].incAngbi2=false;
		KeyFrame[i].incAngbib=false;
		KeyFrame[i].incAngbd1=false;
		KeyFrame[i].incAngbd2=false;
		KeyFrame[i].incAngbdb=false;
		KeyFrame[i].incAngpizq=false;
		KeyFrame[i].incAngpizqb=false;
		KeyFrame[i].incAngpder=false;
		KeyFrame[i].incAngpderb=false;
		KeyFrame[i].incAngpi=false;
		KeyFrame[i].incAngpd=false;
		KeyFrame[i].incXtor=false;
		KeyFrame[i].incYtor=false;
		KeyFrame[i].incZtor=false;
	}

	modelo->Angt1=0.0f;
	modelo->Angt2=0.0f;
	modelo->Angc1=0.0f;
	modelo->Angc2=0.0f;
	modelo->Angbi1=0.0f;
	modelo->Angbi2=0.0f;
	modelo->Angbib=0.0f;
	modelo->Angbd1=0.0f;
	modelo->Angbd2=0.0f;
	modelo->Angbdb=0.0f;
	modelo->Angpizq=0.0f;
	modelo->Angpizqb=0.0f;
	modelo->Angpder=0.0f;
	modelo->Angpderb=0.0f;
	modelo->Angpi=0.0f;
	modelo->Angpd=0.0f;
	modelo->Xtor=0.0f;
	modelo->Ytor=0.0f;
	modelo->Ztor=0.0f;
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

	//Continuan los planos de colisión de piso
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
	plano[8].b1=plano[8].B-plano[8].A;
	plano[8].b2=plano[8].C-plano[8].B;
	plano[8].b3=plano[8].D-plano[8].C;
	plano[8].b4=plano[8].A-plano[8].D;
	plano[8].b1Normal=Cruz(plano[8].b1, plano[8].Normal);
	plano[8].b2Normal=Cruz(plano[8].b2, plano[8].Normal);
	plano[8].b3Normal=Cruz(plano[8].b3, plano[8].Normal);
	plano[8].b4Normal=Cruz(plano[8].b4, plano[8].Normal);
	plano[8].tipo=1;	

	//Muros invisibles

	plano[9].A=CVector(238.6f, 6.5f,-32.0f);
	plano[9].B=CVector(238.6f, 50.0f,-32.0f);
	plano[9].C=CVector(238.6f, 50.0f,-96.7f);
	plano[9].D=CVector(238.6f, 6.5f,-96.7f);
	plano[9].Normal=CVector(-1.0f,0.0f,0.0f);
	plano[9].b1=plano[9].B-plano[9].A;
	plano[9].b2=plano[9].C-plano[9].B;
	plano[9].b3=plano[9].D-plano[9].C;
	plano[9].b4=plano[9].A-plano[9].D;
	plano[9].b1Normal=Cruz(plano[9].b1, plano[9].Normal);
	plano[9].b2Normal=Cruz(plano[9].b2, plano[9].Normal);
	plano[9].b3Normal=Cruz(plano[9].b3, plano[9].Normal);
	plano[9].b4Normal=Cruz(plano[9].b4, plano[9].Normal);
	plano[9].tipo=2;	

	plano[10].A=CVector(238.6f, 6.5f,-32.0f);
	plano[10].B=CVector(5.0f, 6.5f,-32.0f);
	plano[10].C=CVector(5.0f, 50.0f,-32.0f);
	plano[10].D=CVector(238.6f, 50.0f,-32.0f);
	plano[10].Normal=CVector(0.0f,0.0f,-1.0f);
	plano[10].b1=plano[10].B-plano[10].A;
	plano[10].b2=plano[10].C-plano[10].B;
	plano[10].b3=plano[10].D-plano[10].C;
	plano[10].b4=plano[10].A-plano[10].D;
	plano[10].b1Normal=Cruz(plano[10].b1, plano[10].Normal);
	plano[10].b2Normal=Cruz(plano[10].b2, plano[10].Normal);
	plano[10].b3Normal=Cruz(plano[10].b3, plano[10].Normal);
	plano[10].b4Normal=Cruz(plano[10].b4, plano[10].Normal);
	plano[10].tipo=2;	

	plano[11].A=CVector(-10.5f, 6.5f,-32.0f);
	plano[11].B=CVector(-10.5f, 6.5f,150.0f);
	plano[11].C=CVector(-10.5f, 50.0f,150.0f);
	plano[11].D=CVector(-10.5f, 50.0f,-32.0f);
	plano[11].Normal=CVector(-1.0f,0.0f,0.0f);
	plano[11].b1=plano[11].B-plano[11].A;
	plano[11].b2=plano[11].C-plano[11].B;
	plano[11].b3=plano[11].D-plano[11].C;
	plano[11].b4=plano[11].A-plano[11].D;
	plano[11].b1Normal=Cruz(plano[11].b1, plano[11].Normal);
	plano[11].b2Normal=Cruz(plano[11].b2, plano[11].Normal);
	plano[11].b3Normal=Cruz(plano[11].b3, plano[11].Normal);
	plano[11].b4Normal=Cruz(plano[11].b4, plano[11].Normal);
	plano[11].tipo=2;	

	plano[12].A=CVector(-10.5f, 6.5f,150.0f);
	plano[12].B=CVector(-97.0f, 6.5f,150.0f);
	plano[12].C=CVector(-97.0f, 50.0f,150.0f);
	plano[12].D=CVector(-10.5f, 50.0f,150.0f);
	plano[12].Normal=CVector(0.0f,0.0f,-1.0f);
	plano[12].b1=plano[12].B-plano[12].A;
	plano[12].b2=plano[12].C-plano[12].B;
	plano[12].b3=plano[12].D-plano[12].C;
	plano[12].b4=plano[12].A-plano[12].D;
	plano[12].b1Normal=Cruz(plano[12].b1, plano[12].Normal);
	plano[12].b2Normal=Cruz(plano[12].b2, plano[12].Normal);
	plano[12].b3Normal=Cruz(plano[12].b3, plano[12].Normal);
	plano[12].b4Normal=Cruz(plano[12].b4, plano[12].Normal);
	plano[12].tipo=2;	

	for(int i=0; i<maxPlanos; i++)
	{
		plano[i].PM=plano[i].A+(plano[i].C-plano[i].A)*0.5f;
	}

	//Esfera de colision del personaje
	esfera[0].radio=1.8f;
	esfera[0].Pos=CVector(player1.PosicionObj.x, player1.PosicionObj.y+2.5f, player1.PosicionObj.z);
	esfera[0].colision=false;
	
	//Esfera de colision del Enem1
	esfera[1].radio=2.2f;
	esfera[1].Pos=CVector(enem1.PosicionObj.x, enem1.PosicionObj.y-2.0f, enem1.PosicionObj.z);
	esfera[1].colision=false;

	//Esfera de colision del Enem2
	esfera[2].radio=5.5f;
	esfera[2].Pos=CVector(enem2.PosicionObj.x, enem2.PosicionObj.y+2.5f, enem2.PosicionObj.z);
	esfera[2].colision=false;

	//Esfera de colision de MJ6
	esfera[6].radio=2.0f;
	esfera[6].Pos=CVector(MJ6.PosicionObj.x, MJ6.PosicionObj.y+3.5f, MJ6.PosicionObj.z);
	esfera[6].colision=false;

	//Esfera de colision del Enem3a
	esfera[7].radio=3.0f;
	esfera[7].Pos=CVector(enem3a.PosicionObj.x, enem3a.PosicionObj.y+2.0f, enem3a.PosicionObj.z);
	esfera[7].colision=false;

	//Esfera de colision del Enem3b
	esfera[8].radio=3.0f;
	esfera[8].Pos=CVector(enem3b.PosicionObj.x, enem3b.PosicionObj.y+2.0f, enem3b.PosicionObj.z);
	esfera[8].colision=false;

	//Esfera de colision del chango
	esfera[9].radio=2.5f;
	esfera[9].Pos=CVector(chang.PosicionObj.x, chang.PosicionObj.y+2.0f, chang.PosicionObj.z);
	esfera[9].colision=false;

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


	esfera[1].Pos=CVector(enem1.PosicionObj.x, enem1.PosicionObj.y+2.5f, enem1.PosicionObj.z);
	esfera[2].Pos=CVector(target[0], target[1] + 2.5, target[2]);
	esfera[6].Pos=CVector(MJ6.PosicionObj.x, MJ6.PosicionObj.y+2.5f, MJ6.PosicionObj.z);
	esfera[9].Pos=CVector(chang.PosicionObj.x, chang.PosicionObj.y+2.0f, chang.PosicionObj.z);
	esfera[8].Pos=CVector(enem3b.PosicionObj.x, enem3b.PosicionObj.y+2.0f, enem3b.PosicionObj.z);
	esfera[7].Pos=CVector(enem3a.PosicionObj.x, enem3a.PosicionObj.y+2.0f, enem3a.PosicionObj.z);


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

	glPushMatrix();
		glTranslatef(esfera[1].Pos.x, esfera[1].Pos.y, esfera[1].Pos.z);
		glRotatef(90.0f,1.0f,0.0f,0.0f);
		gluSphere(q, esfera[1].radio, 16, 8);
	glPopMatrix();

		glPushMatrix();
		glTranslatef(esfera[2].Pos.x, esfera[2].Pos.y, esfera[2].Pos.z);
		glRotatef(90.0f,1.0f,0.0f,0.0f);
		gluSphere(q, esfera[2].radio, 16, 8);
	glPopMatrix();

		glPushMatrix();
		glTranslatef(esfera[6].Pos.x, esfera[6].Pos.y, esfera[6].Pos.z);
		glRotatef(90.0f,1.0f,0.0f,0.0f);
		gluSphere(q, esfera[6].radio, 16, 8);
	glPopMatrix();

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

	CargaTexturas();

	// para la Spline de la trayectoria automática
	ptsXtramo = 20;
	running = 1;
	trayectoria = 1;
	esperar=0;

	spline_init(helspline, helsplinepoints, totalCP, ptsXtramo );
	spline_init(camspline, camsplinepoints, totalCP1, ptsXtramo );

	idxtp = 2.0f;
	dtidx = 0.35f;

	CargaModelos();
	CreaListas();
	IniSombraVolumen();

	Font.BuildFont();

	InitGLSL();
	
	e=gluNewQuadric();

	InicializaParametrosdeControl();
	InicializaAnim( KeyFrame1, maxKF1, &player1modelo );
	InicializaAnim( KeyFrame2, maxKF2, &mikumodelo );
	InicializaAnim( KeyFrame3, maxKF3, &enem1modelo );
	InicializaAnim( KeyFrame4, maxKF4, &enem2modelo );
	InicializaAnim( KeyFrame5, maxKF5, &MJ6modelo );
	InicializaAnim( KeyFrame6, maxKF6, &enem3amodelo );
	InicializaAnim( KeyFrame7, maxKF7, &enem3bmodelo );
	InicializaAnim( KeyFrame8, maxKF8, &changmodelo );
	DatosAnimacion();

	// Colisiones
	InicializaObjetosdeColision();

	if(InitGLSL())
	{
		cel_Shader.InitShaders("Shaders/celshader.vert","Shaders/celshader.frag");
	}

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

/* animacion() ahora recibe un apuntador hacia el modelo que quieran usar, ej. &mikumodelo
 * tambien recibe el playIndex de su personaje a animar, ese nada mas lo pasan como argumento
 */
void animacion(FRAME *KeyFrame, int maxKF , int frames, jerarquiaModelo* modelo, int& pIndex)
{
	if(play)
	{		
		if((abs(KeyFrame[ pIndex+1].Angt1-modelo->Angt1))<0.1 &&
		   (abs(KeyFrame[ pIndex+1].Angt2-modelo->Angt2))<0.1 &&
		   (abs(KeyFrame[ pIndex+1].Angc1-modelo->Angc1))<0.1 &&
		   (abs(KeyFrame[ pIndex+1].Angc2-modelo->Angc2))<0.1 &&
		   (abs(KeyFrame[ pIndex+1].Angbi1-modelo->Angbi1))<0.1 &&
		   (abs(KeyFrame[ pIndex+1].Angbi2-modelo->Angbi2))<0.1 &&
		   (abs(KeyFrame[ pIndex+1].Angbib-modelo->Angbib))<0.1 &&
		   (abs(KeyFrame[ pIndex+1].Angbd1-modelo->Angbd1))<0.1 &&
		   (abs(KeyFrame[ pIndex+1].Angbd2-modelo->Angbd2))<0.1 &&
		   (abs(KeyFrame[ pIndex+1].Angbdb-modelo->Angbdb))<0.1 &&
		   (abs(KeyFrame[ pIndex+1].Angpizq-modelo->Angpizq))<0.1 &&
		   (abs(KeyFrame[ pIndex+1].Angpizqb-modelo->Angpizqb))<0.1 &&
		   (abs(KeyFrame[ pIndex+1].Angpder-modelo->Angpder))<0.1 &&
		   (abs(KeyFrame[ pIndex+1].Angpderb-modelo->Angpderb))<0.1 &&
		   (abs(KeyFrame[ pIndex+1].Angpi-modelo->Angpi))<0.1 &&
		   (abs(KeyFrame[ pIndex+1].Angpd-modelo->Angpd))<0.1 &&
		   (abs(KeyFrame[ pIndex+1].Xtor-modelo->Xtor))<0.1 &&
		   (abs(KeyFrame[pIndex+1].Ytor-modelo->Ytor))<0.1 &&
		   (abs(KeyFrame[pIndex+1].Ztor-modelo->Ztor))<0.1)
		{			
			pIndex++;			
			if(pIndex>maxKF-2)
			{
				pIndex=0;
				play=false;
								
			}
		}
		else
		{
			KeyFrame[pIndex].incAngt1    = (KeyFrame[pIndex+1].Angt1-KeyFrame[pIndex].Angt1)/frames;
			KeyFrame[pIndex].incAngt2    = (KeyFrame[pIndex+1].Angt2-KeyFrame[pIndex].Angt2)/frames;
			KeyFrame[pIndex].incAngc1    = (KeyFrame[pIndex+1].Angc1-KeyFrame[pIndex].Angc1)/frames;
			KeyFrame[pIndex].incAngc2    = (KeyFrame[pIndex+1].Angc2-KeyFrame[pIndex].Angc2)/frames;
			KeyFrame[pIndex].incAngbi1   = (KeyFrame[pIndex+1].Angbi1-KeyFrame[pIndex].Angbi1)/frames;
			KeyFrame[pIndex].incAngbi2   = (KeyFrame[pIndex+1].Angbi2-KeyFrame[pIndex].Angbi2)/frames;
			KeyFrame[pIndex].incAngbib   = (KeyFrame[pIndex+1].Angbib-KeyFrame[pIndex].Angbib)/frames;
			KeyFrame[pIndex].incAngbd1   = (KeyFrame[pIndex+1].Angbd1-KeyFrame[pIndex].Angbd1)/frames;
			KeyFrame[pIndex].incAngbd2   = (KeyFrame[pIndex+1].Angbd2-KeyFrame[pIndex].Angbd2)/frames;
			KeyFrame[pIndex].incAngbdb   = (KeyFrame[pIndex+1].Angbdb-KeyFrame[pIndex].Angbdb)/frames;
			KeyFrame[pIndex].incAngpizq  = (KeyFrame[pIndex+1].Angpizq-KeyFrame[pIndex].Angpizq)/frames;
			KeyFrame[pIndex].incAngpizqb = (KeyFrame[pIndex+1].Angpizqb-KeyFrame[pIndex].Angpizqb)/frames;
			KeyFrame[pIndex].incAngpder  = (KeyFrame[pIndex+1].Angpder-KeyFrame[pIndex].Angpder)/frames;
			KeyFrame[pIndex].incAngpderb = (KeyFrame[pIndex+1].Angpderb-KeyFrame[pIndex].Angpderb)/frames;
			KeyFrame[pIndex].incAngpi    = (KeyFrame[pIndex+1].Angpi-KeyFrame[pIndex].Angpi)/frames;
			KeyFrame[pIndex].incAngpd    = (KeyFrame[pIndex+1].Angpd-KeyFrame[pIndex].Angpd)/frames;
			KeyFrame[pIndex].incXtor     = (KeyFrame[pIndex+1].Xtor-KeyFrame[pIndex].Xtor)/frames;
			KeyFrame[pIndex].incYtor     = (KeyFrame[pIndex+1].Ytor-KeyFrame[pIndex].Ytor)/frames;
			KeyFrame[pIndex].incZtor     = (KeyFrame[pIndex+1].Ztor-KeyFrame[pIndex].Ztor)/frames;
			
			modelo->Angt1    += KeyFrame[pIndex].incAngt1;
			modelo->Angt2    += KeyFrame[pIndex].incAngt2;
			modelo->Angc1    += KeyFrame[pIndex].incAngc1;
			modelo->Angc2    += KeyFrame[pIndex].incAngc2;
			modelo->Angbi1   += KeyFrame[pIndex].incAngbi1;
			modelo->Angbi2   += KeyFrame[pIndex].incAngbi2;
			modelo->Angbib   += KeyFrame[pIndex].incAngbib;
			modelo->Angbd1   += KeyFrame[pIndex].incAngbd1;
			modelo->Angbd2   += KeyFrame[pIndex].incAngbd2;
			modelo->Angbdb   += KeyFrame[pIndex].incAngbdb;
			modelo->Angpizq  += KeyFrame[pIndex].incAngpizq;
			modelo->Angpizqb += KeyFrame[pIndex].incAngpizqb;
			modelo->Angpder  += KeyFrame[pIndex].incAngpder;
			modelo->Angpderb += KeyFrame[pIndex].incAngpderb;
			modelo->Angpi    += KeyFrame[pIndex].incAngpi;
			modelo->Angpd    += KeyFrame[pIndex].incAngpd;
			modelo->Xtor     += KeyFrame[pIndex].incXtor;
			modelo->Ytor     += KeyFrame[pIndex].incYtor;
			modelo->Ztor     += KeyFrame[pIndex].incZtor;
			
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

void dibujaEnemigo8out()
{
	static float anglef=0.0f;
	anglef+=4.0f;

	glCallList(enemigo8Lout);

	glPushMatrix();	
		glRotatef(anglef, 0.0f, 1.0f, 0.0f);
		glCallList(enemigo8Lout+1);
	glPopMatrix();

	glPushMatrix();
		glRotatef(anglef, 1.0f, 0.0f, 0.0f);
		glCallList(enemigo8Lout+2);
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

void DibujaEnemigo1out()
{
	glTranslatef(enem1modelo.Xtor, enem1modelo.Ytor, enem1modelo.Ztor);
	glRotatef(enem1modelo.Angt2, 0.0f, 1.0f, 0.0f);
	glRotatef(enem1modelo.Angt1, 1.0f, 0.0f, 0.0f);
			
	//Torso
	glCallList(ene1out+0);
	
	//Cabeza
	glPushMatrix();
		glRotatef(enem1modelo.Angc1, 0.0f, 1.0f, 0.0f);
		glCallList(ene1out+1);
	glPopMatrix();

	//Brazo derecho
	glPushMatrix();
		//glTranslatef(-2.8f, 1.1f, 0.0f);
		glRotatef(enem1modelo.Angbd2, 0.0f, 1.0f, 0.0f);
		glRotatef(enem1modelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glCallList(ene1out+2);
	glPopMatrix();

	//Brazo izquierdo
	glPushMatrix();
		//glTranslatef(-2.8f, 1.1f, 0.0f);
		glRotatef(enem1modelo.Angbd2, 0.0f, 1.0f, 0.0f);
		glRotatef(enem1modelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glCallList(ene1out+3);
	glPopMatrix();
	
	//Pierna derecha
	glPushMatrix();
		//glTranslatef(-1.2f, -1.3f ,0.0f);
		glRotatef(enem1modelo.Angpder, 1.0f, 0.0f, 0.0f);
		glCallList(ene1out+4);
		
		//Pierna derecha_b
		glPushMatrix();
			//glTranslatef(0.0f, -1.25f , 0.0f);
			glRotatef(enem1modelo.Angpderb, 1.0f, 0.0f, 0.0f);
			glCallList(ene1out+5);
			
			//Pie derecho
			glPushMatrix();
				glRotatef(enem1modelo.Angpd,1.0f, 0.0f, 0.0f);
				glCallList(ene1out+6);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();

	//Pierna izquierda
	glPushMatrix();
		//glTranslatef(1.2f, -1.3f ,0.0f);
		glRotatef(enem1modelo.Angpizq, 1.0f, 0.0f, 0.0f);
		glCallList(ene1out+7);

		//Pierna izquierda_b
		glPushMatrix();
			//glTranslatef(0.0f, -1.25f , 0.0f);
			glRotatef(enem1modelo.Angpizqb, 1.0f, 0.0f, 0.0f);
			glCallList(ene1out+8);
			//Pie derecho
			glPushMatrix();
				glRotatef(enem1modelo.Angpd,1.0f, 0.0f, 0.0f);
				glCallList(ene1out+9);
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
	glCallList(ene2+2);
}

void DibujaEnemigo2out()
{
	glTranslatef(enem2modelo.Xtor, enem2modelo.Ytor, enem2modelo.Ztor);
	glRotatef(enem2modelo.Angt2, 0.0f, 1.0f, 0.0f);
	glRotatef(enem2modelo.Angt1, 1.0f, 0.0f, 0.0f);

	//Nave
	glCallList(ene2out+0);
	//Bomba
	glCallList(ene2out+1);
	//Piloto
	glCallList(ene2out+2);
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

void DibujaMJ6out()
{
	glTranslatef(MJ6modelo.Xtor, MJ6modelo.Ytor, MJ6modelo.Ztor);
	glRotatef(MJ6modelo.Angt2, 0.0f, 1.0f, 0.0f);
	glRotatef(MJ6modelo.Angt1, 1.0f, 0.0f, 0.0f);
			
	//Torso
	glCallList(noMJ6out+0);
	
	//Cabeza
	glPushMatrix();
		glRotatef(MJ6modelo.Angc1, 0.0f, 1.0f, 0.0f);
		glCallList(noMJ6out+1);
	glPopMatrix();

	//Brazo derecho
	glPushMatrix();
		//glTranslatef(-2.8f, 1.1f, 0.0f);
		glRotatef(MJ6modelo.Angbd2, 0.0f, 1.0f, 0.0f);
		glRotatef(MJ6modelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glCallList(noMJ6out+2);

		//Brazo derecho_b
		glPushMatrix();
			//glTranslatef(0.0f, -1.25f , 0.0f);
			glRotatef(MJ6modelo.Angpderb, 1.0f, 0.0f, 0.0f);
			glCallList(noMJ6out+3);
		glPopMatrix();
	glPopMatrix();

	//Brazo izquierdo
	glPushMatrix();
		//glTranslatef(-2.8f, 1.1f, 0.0f);
		glRotatef(MJ6modelo.Angbd2, 0.0f, 1.0f, 0.0f);
		glRotatef(MJ6modelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glCallList(noMJ6out+4);

		//Brazo izquierdo_b
		glPushMatrix();
			//glTranslatef(0.0f, -1.25f , 0.0f);
			glRotatef(MJ6modelo.Angpderb, 1.0f, 0.0f, 0.0f);
			glCallList(noMJ6out+5);
		glPopMatrix();
	glPopMatrix();
	
	//Pierna derecha
	glPushMatrix();
		//glTranslatef(-1.2f, -1.3f ,0.0f);
		glRotatef(MJ6modelo.Angpder, 1.0f, 0.0f, 0.0f);
		glCallList(noMJ6out+6);
		
		//Pierna derecha_b
		glPushMatrix();
			//glTranslatef(0.0f, -1.25f , 0.0f);
			glRotatef(MJ6modelo.Angpderb, 1.0f, 0.0f, 0.0f);
			glCallList(noMJ6out+7);
		glPopMatrix();
	glPopMatrix();

	//Pierna izquierda
	glPushMatrix();
		//glTranslatef(1.2f, -1.3f ,0.0f);
		glRotatef(MJ6modelo.Angpizq, 1.0f, 0.0f, 0.0f);
		glCallList(noMJ6out+8);

		//Pierna izquierda_b
		glPushMatrix();
			//glTranslatef(0.0f, -1.25f , 0.0f);
			glRotatef(MJ6modelo.Angpizqb, 1.0f, 0.0f, 0.0f);
			glCallList(noMJ6out+9);
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

void DibujaEnemigo3aout()
{
	glTranslatef(enem3amodelo.Xtor, enem3amodelo.Ytor, enem3amodelo.Ztor);
	glRotatef(enem3amodelo.Angt2, 0.0f, 1.0f, 0.0f);
	glRotatef(enem3amodelo.Angt1, 1.0f, 0.0f, 0.0f);
			
	//Torso
	glCallList(ene3aout+0);
	
	//Brazo derecho
	glPushMatrix();
		//glTranslatef(-2.8f, 1.1f, 0.0f);
		glRotatef(enem3amodelo.Angbd2, 0.0f, 1.0f, 0.0f);
		glRotatef(enem3amodelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glCallList(ene3aout+1);
	glPopMatrix();

	//Brazo izquierdo
	glPushMatrix();
		//glTranslatef(-2.8f, 1.1f, 0.0f);
		glRotatef(enem3amodelo.Angbd2, 0.0f, 1.0f, 0.0f);
		glRotatef(enem3amodelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glCallList(ene3aout+2);
	glPopMatrix();
	
	//Pierna derecha
	glPushMatrix();
		//glTranslatef(-1.2f, -1.3f ,0.0f);
		glRotatef(enem3amodelo.Angpder, 1.0f, 0.0f, 0.0f);
		glCallList(ene3aout+3);
	glPopMatrix();

	//Pierna izquierda
	glPushMatrix();
		//glTranslatef(1.2f, -1.3f ,0.0f);
		glRotatef(enem3amodelo.Angpizq, 1.0f, 0.0f, 0.0f);
		glCallList(ene3aout+4);
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

void DibujaEnemigo3bout()
{
	glTranslatef(enem3bmodelo.Xtor, enem3bmodelo.Ytor, enem3bmodelo.Ztor);
	glRotatef(enem3bmodelo.Angt2, 0.0f, 1.0f, 0.0f);
	glRotatef(enem3bmodelo.Angt1, 1.0f, 0.0f, 0.0f);
			
	//Torso
	glCallList(ene3bout+0);
	
	//Brazo derecho
	glPushMatrix();
		//glTranslatef(-2.8f, 1.1f, 0.0f);
		glRotatef(enem3bmodelo.Angbd2, 0.0f, 1.0f, 0.0f);
		glRotatef(enem3bmodelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glCallList(ene3bout+1);
	glPopMatrix();

	//Brazo izquierdo
	glPushMatrix();
		//glTranslatef(-2.8f, 1.1f, 0.0f);
		glRotatef(enem3bmodelo.Angbd2, 0.0f, 1.0f, 0.0f);
		glRotatef(enem3bmodelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glCallList(ene3bout+2);
	glPopMatrix();
	
	//Pierna derecha
	glPushMatrix();
		//glTranslatef(-1.2f, -1.3f ,0.0f);
		glRotatef(enem3bmodelo.Angpder, 1.0f, 0.0f, 0.0f);
		glCallList(ene3bout+3);
	glPopMatrix();

	//Pierna izquierda
	glPushMatrix();
		//glTranslatef(1.2f, -1.3f ,0.0f);
		glRotatef(enem3bmodelo.Angpizq, 1.0f, 0.0f, 0.0f);
		glCallList(ene3bout+4);
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


void DibujaChangoout()
{
	glTranslatef(changmodelo.Xtor, changmodelo.Ytor, changmodelo.Ztor);
	glRotatef(changmodelo.Angt2, 0.0f, 1.0f, 0.0f);
	glRotatef(changmodelo.Angt1, 1.0f, 0.0f, 0.0f);
			
	//Torso
	glCallList(chaout+0);
	
	//Brazo derecho
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, 2.8f);
		glRotatef(changmodelo.Angbd2, 0.0f, 1.0f, 0.0f);
		glRotatef(changmodelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glCallList(chaout+1);
	glPopMatrix();

	//Brazo izquierdo
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, 2.8f);
		glRotatef(changmodelo.Angbd2, 0.0f, 1.0f, 0.0f);
		glRotatef(changmodelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glCallList(chaout+2);
	glPopMatrix();
	
	//Pierna derecha
	glPushMatrix();
		//glTranslatef(-1.2f, -1.3f ,0.0f);
		glRotatef(changmodelo.Angpder, 1.0f, 0.0f, 0.0f);
		glCallList(chaout+3);
	glPopMatrix();

	//Pierna izquierda
	glPushMatrix();
		//glTranslatef(1.2f, -1.3f ,0.0f);
		glRotatef(changmodelo.Angpizq, 1.0f, 0.0f, 0.0f);
		glCallList(chaout+4);
	glPopMatrix();
}


void DibujaTarantula()
{
	//Torso
	glCallList(taran+0);
}


void DibujaTarantulaout()
{
	//Torso
	glCallList(taranout+0);
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

void DibujaMiku()
{
	glTranslatef( mikumodelo.Xtor, mikumodelo.Ytor, mikumodelo.Ztor);
	glRotatef( mikumodelo.Angt2, 0.0f, 1.0f, 0.0f);
	glRotatef( mikumodelo.Angt1, 1.0f, 0.0f, 0.0f);
			
	//Torso
	glCallList( modelo1miku + 0 );

	//Pierna derecha
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, 0.0f);
		glRotatef(mikumodelo.Angpder, 1.0f, 0.0f, 0.0f);
		glCallList( modelo1miku + 1 );

	glPopMatrix();

	//Pierna izquierda
	glPushMatrix();
		glTranslatef(0.0f, 0.0f ,0.0f);
		glRotatef( mikumodelo.Angpizq, 1.0f, 0.0f, 0.0f);
		glCallList( modelo1miku + 2 );

	glPopMatrix();

	//Brazo derecho_a
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, 0.0f);
		glRotatef(mikumodelo.Angbd2, 0.0f, 1.0f, 0.0f);
		glRotatef(mikumodelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glCallList( modelo1miku + 3 );

	glPopMatrix();

	//Brazo izquierdo
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, 0.0f);
		glRotatef(mikumodelo.Angbi2, 0.0f, 1.0f, 0.0f);
		glRotatef(mikumodelo.Angbi1, 1.0f, 0.0f, 0.0f);
		glCallList(modelo1miku + 4);

	glPopMatrix();
}
void DibujaMikuout()
{
	glTranslatef( mikumodelo.Xtor, mikumodelo.Ytor, mikumodelo.Ztor);
	glRotatef( mikumodelo.Angt2, 0.0f, 1.0f, 0.0f);
	glRotatef( mikumodelo.Angt1, 1.0f, 0.0f, 0.0f);
			
	//Torso
	glCallList( modelo1mikuout + 0 );

	//Pierna derecha
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, 0.0f);
		glRotatef(mikumodelo.Angpder, 1.0f, 0.0f, 0.0f);
		glCallList( modelo1mikuout + 1 );

	glPopMatrix();

	//Pierna izquierda
	glPushMatrix();
		glTranslatef(0.0f, 0.0f ,0.0f);
		glRotatef( mikumodelo.Angpizq, 1.0f, 0.0f, 0.0f);
		glCallList( modelo1mikuout + 2 );

	glPopMatrix();

	//Brazo derecho_a
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, 0.0f);
		glRotatef(mikumodelo.Angbd2, 0.0f, 1.0f, 0.0f);
		glRotatef(mikumodelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glCallList( modelo1mikuout + 3 );

	glPopMatrix();

	//Brazo izquierdo
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, 0.0f);
		glRotatef(mikumodelo.Angbi2, 0.0f, 1.0f, 0.0f);
		glRotatef(mikumodelo.Angbi1, 1.0f, 0.0f, 0.0f);
		glCallList(modelo1mikuout + 4);

	glPopMatrix();
}

void DibujaSombraMJ()
{
	glPushMatrix();

		glTranslatef( player1.PosicionObj.x, player1.PosicionObj.y + 2.4f, player1.PosicionObj.z + 0.0f);
		glRotatef(player1.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(player1.escalaX,player1.escalaY,player1.escalaZ);
		glTranslatef(player1modelo.Xtor, player1modelo.Ytor, player1modelo.Ztor);
		glRotatef(player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
				
		//Torso
		objSh.calculaSombraDepthPass(&g_3DModel1aru, objectSpaceLightPosition1);
		
		//Pierna derecha
		glPushMatrix();
			glTranslatef(1.0f, 0.0f, 0.0f);
			glRotatef(player1modelo.Angpder, 1.0f, 0.0f, 0.0f);
			objSh.calculaSombraDepthPass(&g_3DModel2aru, objectSpaceLightPosition2);

			//Pierna derecha_b
			glPushMatrix();
				glTranslatef(-5.2f, 0.7f , 0.0f);
				glRotatef(player1modelo.Angpderb, 1.0f, 0.0f, 0.0f);
				objSh.calculaSombraDepthPass(&g_3DModel3aru, objectSpaceLightPosition3);
			glPopMatrix();

		glPopMatrix();

		//Pierna izquierda
		glPushMatrix();
			glTranslatef(0.0f, 0.0f ,0.0f);
			glRotatef(player1modelo.Angpizq, 1.0f, 0.0f, 0.0f);
			objSh.calculaSombraDepthPass(&g_3DModel4aru, objectSpaceLightPosition4);

			//Pierna izquierda_b
			glPushMatrix();
				glTranslatef(-6.5f, 0.8f , 0.0f);
				glRotatef(player1modelo.Angpizqb, 1.0f, 0.0f, 0.0f);
				objSh.calculaSombraDepthPass(&g_3DModel5aru, objectSpaceLightPosition5);
			glPopMatrix();

		glPopMatrix();

		//Brazo derecho_a
		glPushMatrix();
			glTranslatef(5.65f, 0.2f, 0.2f);
			glRotatef(player1modelo.Angbd2, 0.0f, 1.0f, 0.0f);
			glRotatef(player1modelo.Angbd1, 1.0f, 0.0f, 0.0f);
			objSh.calculaSombraDepthPass(&g_3DModel6aru, objectSpaceLightPosition6);

			//Brazo derecho_b
			glPushMatrix();
				glTranslatef(-4.6f, 2.0f, 0.0f);
				glRotatef(player1modelo.Angbdb, 1.0f, 0.0f, 0.0f);
				objSh.calculaSombraDepthPass(&g_3DModel8aru, objectSpaceLightPosition7);
			glPopMatrix();

		glPopMatrix();

		//Brazo izquierdo
		glPushMatrix();
			glTranslatef(-1.0f, 1.5f, 0.0f);
			glRotatef(player1modelo.Angbi2, 0.0f, 1.0f, 0.0f);
			glRotatef(player1modelo.Angbi1, 1.0f, 0.0f, 0.0f);
			objSh.calculaSombraDepthPass(&g_3DModel7aru, objectSpaceLightPosition8);

			//Brazo izquierdo_b
			glPushMatrix();
				glTranslatef(-0.7f, 0.5f, 0.0f);
				glRotatef(player1modelo.Angbib, 1.0f, 0.0f, 0.0f);
				objSh.calculaSombraDepthPass(&g_3DModel9aru, objectSpaceLightPosition9);
			glPopMatrix();
		glPopMatrix();

	glPopMatrix();
		
}
void DibujaPersonajeAruout()
{
	glTranslatef(player1modelo.Xtor, player1modelo.Ytor, player1modelo.Ztor);
	glRotatef(player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
	glRotatef(player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
			
	//Torso
	glCallList(modelo1aruout+0);
	
	//Pierna derecha
	glPushMatrix();
		glTranslatef(1.0f, 0.0f, 0.0f);
		glRotatef(player1modelo.Angpder, 1.0f, 0.0f, 0.0f);
		glCallList(modelo1aruout+1);
		
		//Pierna derecha_b
		glPushMatrix();
			glTranslatef(-5.2f, 0.7f , 0.0f);
			glRotatef(player1modelo.Angpderb, 1.0f, 0.0f, 0.0f);
			glCallList(modelo1aruout+2);
		glPopMatrix();

	glPopMatrix();

	//Pierna izquierda
	glPushMatrix();
		glTranslatef(0.0f, 0.0f ,0.0f);
		glRotatef(player1modelo.Angpizq, 1.0f, 0.0f, 0.0f);
		glCallList(modelo1aruout+3);

		//Pierna izquierda_b
		glPushMatrix();
			glTranslatef(-6.5f, 0.8f , 0.0f);
			glRotatef(player1modelo.Angpizqb, 1.0f, 0.0f, 0.0f);
			glCallList(modelo1aruout+4);
		glPopMatrix();

	glPopMatrix();

	//Brazo derecho_a
	glPushMatrix();
		glTranslatef(5.65f, 0.2f, 0.2f);
		glRotatef(player1modelo.Angbd2, 0.0f, 1.0f, 0.0f);
		glRotatef(player1modelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glCallList(modelo1aruout+5);

		//Brazo derecho_b
		glPushMatrix();
			glTranslatef(-4.6f, 2.0f, 0.0f);
			glRotatef(player1modelo.Angbdb, 1.0f, 0.0f, 0.0f);
			glCallList(modelo1aruout+7);
		glPopMatrix();

	glPopMatrix();

	//Brazo izquierdo
	glPushMatrix();
		glTranslatef(-1.0f, 1.5f, 0.0f);
		glRotatef(player1modelo.Angbi2, 0.0f, 1.0f, 0.0f);
		glRotatef(player1modelo.Angbi1, 1.0f, 0.0f, 0.0f);
		glCallList(modelo1aruout+6);

		//Brazo izquierdo_b
		glPushMatrix();
			glTranslatef(-0.7f, 0.5f, 0.0f);
			glRotatef(player1modelo.Angbib, 1.0f, 0.0f, 0.0f);
			glCallList(modelo1aruout+8);
		glPopMatrix();

	glPopMatrix();
}


void DibujaVolumendeSombra()
{
	glPushMatrix();

		glTranslatef( player1.PosicionObj.x, player1.PosicionObj.y + 2.4f, player1.PosicionObj.z + 0.0f);
		glRotatef(player1.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(player1.escalaX,player1.escalaY,player1.escalaZ);
		glTranslatef(player1modelo.Xtor, player1modelo.Ytor, player1modelo.Ztor);
		glRotatef(player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
				
		//Torso
		objSh.DrawShadowVolume(&g_3DModel1aru, objectSpaceLightPosition1);
		
		//Pierna derecha
		glPushMatrix();
			glTranslatef(1.0f, 0.0f, 0.0f);
			glRotatef(player1modelo.Angpder, 1.0f, 0.0f, 0.0f);
			objSh.DrawShadowVolume(&g_3DModel2aru, objectSpaceLightPosition2);

			//Pierna derecha_b
			glPushMatrix();
				glTranslatef(-5.2f, 0.7f , 0.0f);
				glRotatef(player1modelo.Angpderb, 1.0f, 0.0f, 0.0f);
				objSh.DrawShadowVolume(&g_3DModel3aru, objectSpaceLightPosition3);
			glPopMatrix();

		glPopMatrix();

		//Pierna izquierda
		glPushMatrix();
			glTranslatef(0.0f, 0.0f ,0.0f);
			glRotatef(player1modelo.Angpizq, 1.0f, 0.0f, 0.0f);
			objSh.DrawShadowVolume(&g_3DModel4aru, objectSpaceLightPosition4);

			//Pierna izquierda_b
			glPushMatrix();
				glTranslatef(-6.5f, 0.8f , 0.0f);
				glRotatef(player1modelo.Angpizqb, 1.0f, 0.0f, 0.0f);
				objSh.DrawShadowVolume(&g_3DModel5aru, objectSpaceLightPosition5);
			glPopMatrix();

		glPopMatrix();

		//Brazo derecho_a
		glPushMatrix();
			glTranslatef(5.65f, 0.2f, 0.2f);
			glRotatef(player1modelo.Angbd2, 0.0f, 1.0f, 0.0f);
			glRotatef(player1modelo.Angbd1, 1.0f, 0.0f, 0.0f);
			objSh.DrawShadowVolume(&g_3DModel6aru, objectSpaceLightPosition6);

			//Brazo derecho_b
			glPushMatrix();
				glTranslatef(-4.6f, 2.0f, 0.0f);
				glRotatef(player1modelo.Angbdb, 1.0f, 0.0f, 0.0f);
				objSh.DrawShadowVolume(&g_3DModel8aru, objectSpaceLightPosition7);
			glPopMatrix();

		glPopMatrix();

		//Brazo izquierdo
		glPushMatrix();
			glTranslatef(-1.0f, 1.5f, 0.0f);
			glRotatef(player1modelo.Angbi2, 0.0f, 1.0f, 0.0f);
			glRotatef(player1modelo.Angbi1, 1.0f, 0.0f, 0.0f);
			objSh.DrawShadowVolume(&g_3DModel7aru, objectSpaceLightPosition8);

			//Brazo izquierdo_b
			glPushMatrix();
				glTranslatef(-0.7f, 0.5f, 0.0f);
				glRotatef(player1modelo.Angbib, 1.0f, 0.0f, 0.0f);
				objSh.DrawShadowVolume(&g_3DModel9aru, objectSpaceLightPosition9);
			glPopMatrix();

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

		//Dibujar el indicador de personaje

		glColor3f( 1.0f, 1.0f, 1.0f );

		glBindTexture(GL_TEXTURE_2D, textura[1].texID);

		glPushMatrix();
			glTranslatef( glWidth * 0.03f, glHeight * 0.05, 0.0f );
			glBegin(GL_QUADS);
				glTexCoord2f( 0.0f, 0.0f ); glVertex2i( 0, 0 );
				glTexCoord2f( 1.0f, 0.0f ); glVertex2i( 50, 0 );
				glTexCoord2f( 1.0f, 1.0f ); glVertex2i( 50, 50 );
				glTexCoord2f( 0.0f, 1.0f ); glVertex2i( 0, 50 );
			glEnd();
		glPopMatrix();

		// Texto a mostrar en pantalla
		Font.glPrint((1.0f/640.0f)*glWidth, glWidth*0.05f,glHeight*0.9f,"pisoId: %d", pisoId );
		/*Font.glPrint((1.0f/640.0f)*glWidth, glWidth*0.05f,glHeight*0.85f,"PosCam %f", player1.PosicionCam.x );
		Font.glPrint((1.0f/640.0f)*glWidth, glWidth*0.05f,glHeight*0.80f,"PosObj %f", player1.PosicionObj.x);*/
		Font.glPrint( (1.0f/640.0f)*glWidth, glWidth * 0.45f, glHeight * 0.95f, "High 50000" );
		Font.glPrint( (1.0f/640.0f)*glWidth, glWidth * 0.42f, glHeight * 0.90f, "Round 1 Stage 1" );
		Font.glPrint( (1.0f/640.0f)*glWidth, glWidth * 0.08f, glHeight * 0.15f, "1P" );
		Font.glPrint( (1.0f/640.0f)*glWidth, glWidth * 0.20f, glHeight * 0.15f, "0" );
		Font.glPrint( (1.0f/640.0f)*glWidth, glWidth * 0.1f, glHeight * 0.11f, "3" );
									
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_TEXTURE_2D);

		//Barra de energia
		glPushMatrix();
			glTranslatef( glWidth * 0.11f, glHeight * 0.05, 0.0f );
			glColor3ub( 250, 197, 0 );
			glBegin(GL_QUADS);
				glVertex2i( 0, 0 );
				glVertex2i( 70, 0 );
				glVertex2i( 70, 25 );
				glVertex2i( 0, 25 );
			glEnd();
		glPopMatrix();

		glColor3f(1.0f,1.0f,1.0f);

		glEnable(GL_LIGHTING);
		glMatrixMode(GL_PROJECTION);						// Selecciona la matriz de proyección
		glPopMatrix();										// Recupera la anterior matriz de proyección
		glMatrixMode(GL_MODELVIEW);							// Selecciona la matriz de modelo de vista
	glPopMatrix();										// Recupera la anterior matriz de modelo de vista
	glEnable(GL_DEPTH_TEST);							// Activa la prueba de profundidad
		
}

void DibujaMJ()
{
	cel_Shader.TurnOn();
	// Aru
	glPushMatrix();
		glTranslatef( player1.PosicionObj.x, player1.PosicionObj.y + 2.4f, player1.PosicionObj.z + 0.0f);
		glRotatef(player1.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(player1.escalaX,player1.escalaY,player1.escalaZ);
		DibujaPersonajeAru();
	glPopMatrix();

	// Miku
	glPushMatrix();
		glTranslatef( miku.PosicionObj.x, miku.PosicionObj.y + 2.4f, miku.PosicionObj.z + 0.0f);
		glRotatef(miku.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(miku.escalaX,miku.escalaY,miku.escalaZ);
		DibujaMiku();
	glPopMatrix();

	//MJ6
	glPushMatrix();
		glTranslatef(MJ6.PosicionObj.x, MJ6.PosicionObj.y+2.4f, MJ6.PosicionObj.z);
		glRotatef(MJ6.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(MJ6.escalaX,MJ6.escalaY,MJ6.escalaZ);
		DibujaMJ6();
	glPopMatrix();
	cel_Shader.TurnOff();

	//contornos
	glDisable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(2.5f);

	// Aru
	glPushMatrix();
		glTranslatef( player1.PosicionObj.x, player1.PosicionObj.y + 2.4f, player1.PosicionObj.z + 0.0f);
		glRotatef(player1.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(player1.escalaX,player1.escalaY,player1.escalaZ);
		DibujaPersonajeAruout();
	glPopMatrix();

	//MJ6
	glPushMatrix();
		glTranslatef(MJ6.PosicionObj.x, MJ6.PosicionObj.y+2.4f, MJ6.PosicionObj.z);
		glRotatef(MJ6.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(MJ6.escalaX,MJ6.escalaY,MJ6.escalaZ);
		DibujaMJ6out();
	glPopMatrix();

	glLineWidth(1.0f);
	// Miku
	glPushMatrix();
		glTranslatef( miku.PosicionObj.x, miku.PosicionObj.y + 2.4f, miku.PosicionObj.z + 0.0f);
		glRotatef(miku.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(miku.escalaX,miku.escalaY,miku.escalaZ);
		DibujaMikuout();
	glPopMatrix();


	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
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

	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition2[0]=lightPosition.x;
	lightposition2[1]=lightPosition.y;
	lightposition2[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 2 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef(-player1modelo.Angpder, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player1.AngObj, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix2);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 2
	VMatMult(inverseModelMatrix2, lightposition2);
	
	objectSpaceLightPosition2.x=lightposition2[0];
	objectSpaceLightPosition2.y=lightposition2[1];
	objectSpaceLightPosition2.z=lightposition2[2];
	

	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition3[0]=lightPosition.x;
	lightposition3[1]=lightPosition.y;
	lightposition3[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 3 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef(-player1modelo.Angpderb, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angpder, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player1.AngObj, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix3);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 3
	VMatMult(inverseModelMatrix3, lightposition3);
	
	objectSpaceLightPosition3.x=lightposition3[0];
	objectSpaceLightPosition3.y=lightposition3[1];
	objectSpaceLightPosition3.z=lightposition3[2];
	

	//Modelo 4 (Pierna izquierda)

	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition4[0]=lightPosition.x;
	lightposition4[1]=lightPosition.y;
	lightposition4[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 4 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef(-player1modelo.Angpizq, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player1.AngObj, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix4);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 4
	VMatMult(inverseModelMatrix4, lightposition4);
	
	objectSpaceLightPosition4.x=lightposition4[0];
	objectSpaceLightPosition4.y=lightposition4[1];
	objectSpaceLightPosition4.z=lightposition4[2];

	//Modelo 5 (Pierna izquierda_b)

	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition5[0]=lightPosition.x;
	lightposition5[1]=lightPosition.y;
	lightposition5[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 5 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef(-player1modelo.Angpizqb, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angpizq, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player1.AngObj, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix5);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 5
	VMatMult(inverseModelMatrix5, lightposition5);
	
	objectSpaceLightPosition5.x=lightposition5[0];
	objectSpaceLightPosition5.y=lightposition5[1];
	objectSpaceLightPosition5.z=lightposition5[2];

	//Modelo 6 (Brazo derecho_a)

	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition6[0]=lightPosition.x;
	lightposition6[1]=lightPosition.y;
	lightposition6[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 6 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef(-player1modelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angbd2, 1.0f, 1.0f, 0.0f);
		glRotatef(-player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player1.AngObj, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix6);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 6
	VMatMult(inverseModelMatrix6, lightposition6);
	
	objectSpaceLightPosition6.x=lightposition6[0];
	objectSpaceLightPosition6.y=lightposition6[1];
	objectSpaceLightPosition6.z=lightposition6[2];

	//Modelo 7 (Brazo derecho_b)

	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition7[0]=lightPosition.x;
	lightposition7[1]=lightPosition.y;
	lightposition7[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 7 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef(-player1modelo.Angbdb, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angbd2, 1.0f, 1.0f, 0.0f);
		glRotatef(-player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player1.AngObj, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix7);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 7
	VMatMult(inverseModelMatrix7, lightposition7);
	
	objectSpaceLightPosition7.x=lightposition7[0];
	objectSpaceLightPosition7.y=lightposition7[1];
	objectSpaceLightPosition7.z=lightposition7[2];

	//Modelo 8 (Brazo izquierdo_a)

	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition8[0]=lightPosition.x;
	lightposition8[1]=lightPosition.y;
	lightposition8[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 8 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef(-player1modelo.Angbi1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angbi2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player1.AngObj, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix8);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 8
	VMatMult(inverseModelMatrix8, lightposition8);
	
	objectSpaceLightPosition8.x=lightposition8[0];
	objectSpaceLightPosition8.y=lightposition8[1];
	objectSpaceLightPosition8.z=lightposition8[2];
	
	//Modelo 9 (Brazo izquierdo_b)

	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition9[0]=lightPosition.x;
	lightposition9[1]=lightPosition.y;
	lightposition9[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 9 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef(-player1modelo.Angbib, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angbi1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angbi2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player1.AngObj, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix9);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 9
	VMatMult(inverseModelMatrix9, lightposition9);
	
	objectSpaceLightPosition9.x=lightposition9[0];
	objectSpaceLightPosition9.y=lightposition9[1];
	objectSpaceLightPosition9.z=lightposition9[2];

}

void DibujaEnemigos()
{
	cel_Shader.TurnOn();
	// savage
	glPushMatrix();
			glTranslatef(enemigo8.PosicionObj.x, enemigo8.PosicionObj.y+2.4f, enemigo8.PosicionObj.z);
			glRotatef(enemigo8.AngObj, 0.0f, 1.0f, 0.0f);
			glScalef(enemigo8.escalaX,enemigo8.escalaY,enemigo8.escalaZ);
			dibujaEnemigo8();
	glPopMatrix();

	//Ene1
	glPushMatrix();
		glTranslatef(enem1.PosicionObj.x, enem1.PosicionObj.y, enem1.PosicionObj.z);
		glRotatef(enem1.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(enem1.escalaX,enem1.escalaY,enem1.escalaZ);
		DibujaEnemigo1();
	glPopMatrix();

	//Ene2
	glPushMatrix();
		if(pisoId < 2)
			glTranslatef(target[0], target[1], target[2]);
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

	//chango
	glPushMatrix();
		glTranslatef(chang.PosicionObj.x, chang.PosicionObj.y+2.4f, chang.PosicionObj.z);
		glRotatef(chang.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(chang.escalaX,chang.escalaY,chang.escalaZ);
		DibujaChango();
	glPopMatrix();

	//tarantulas tipo pelicula Moonwalker
	glPushMatrix();   //1 (rocas inicio)
		glTranslatef(205.7f, 2.0f, -88.0f);
		glScalef(0.25,0.25,0.25);
		DibujaTarantula();
	glPopMatrix();
	glPushMatrix();   //2 (plataforma inicio grande)
		glTranslatef(230.0f, 2.5f, -35.0f);
		glRotatef(200.0f,0,1,0);
		glScalef(0.2,0.2,0.2);
		DibujaTarantula();
	glPopMatrix();
	glPushMatrix();   //3 (plataforma inicio chica)
		glTranslatef(229.0f, 5.5f, -32.0f);
		glRotatef(240.0f,0,1,0);
		glScalef(0.1,0.1,0.1);
		DibujaTarantula();
	glPopMatrix();
	glPushMatrix();   //4 (pared plano 2)
		glTranslatef(45.0f, 17.0f, -100.0f);
		glRotatef(100.0f,0,0,1);
		glRotatef(90.0f,1,0,0);
		glScalef(0.1,0.1,0.1);
		DibujaTarantula();
	glPopMatrix();
	glPopMatrix();
		glPushMatrix();   //5 (esquina interior)
		glTranslatef(-3.5f, 4.5f, -40.0f);
		glRotatef(160.0f,0,1,0);
		glScalef(0.11,0.11,0.11);
		DibujaTarantula();
	glPopMatrix();
	glPopMatrix();
		glPushMatrix();   //6 (rocas inicio plano 7)
		glTranslatef(-87.0f, 5.27f, 16.0f);
		glScalef(0.14,0.14,0.14);
		DibujaTarantula();
	glPopMatrix();
	glPopMatrix();
		glPushMatrix();   //7 (rocas final)
		glTranslatef(-16.0f, 5.3f, 115.0f);
		glRotatef(160.0f,0,1,0);
		glScalef(0.15,0.15,0.15);
		DibujaTarantula();
	glPopMatrix();


	cel_Shader.TurnOff();

	//contornos
	glDisable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(2.5f);

	// savage
	glPushMatrix();
			glTranslatef(enemigo8.PosicionObj.x, enemigo8.PosicionObj.y+2.4f, enemigo8.PosicionObj.z);
			glRotatef(enemigo8.AngObj, 0.0f, 1.0f, 0.0f);
			glScalef(enemigo8.escalaX,enemigo8.escalaY,enemigo8.escalaZ);
			dibujaEnemigo8out();
	glPopMatrix();

	//Ene1
	glPushMatrix();
		glTranslatef(enem1.PosicionObj.x, enem1.PosicionObj.y, enem1.PosicionObj.z);
		glRotatef(enem1.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(enem1.escalaX,enem1.escalaY,enem1.escalaZ);
		DibujaEnemigo1out();
	glPopMatrix();

	//Ene2
	glPushMatrix();
		glTranslatef(target[0], target[1], target[2]);
		glRotatef(enem2.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(enem2.escalaX,enem1.escalaY,enem1.escalaZ);
		DibujaEnemigo2out();
	glPopMatrix();

	//Ene3a
	glPushMatrix();
		glTranslatef(enem3a.PosicionObj.x, enem3a.PosicionObj.y+2.4f, enem3a.PosicionObj.z);
		glRotatef(enem3a.AngObj,0.0f, 1.0f, 0.0f);
		glScalef(enem3a.escalaX,enem3a.escalaY,enem3a.escalaZ);
		DibujaEnemigo3aout();
	glPopMatrix();

	//Ene3b
	glPushMatrix();
		glTranslatef(enem3b.PosicionObj.x, enem3b.PosicionObj.y+2.4f, enem3b.PosicionObj.z);
		glRotatef(enem3b.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(enem3b.escalaX,enem3b.escalaY,enem3b.escalaZ);
		DibujaEnemigo3bout();
	glPopMatrix();

	//chango
	glPushMatrix();
		glTranslatef(chang.PosicionObj.x, chang.PosicionObj.y+2.4f, chang.PosicionObj.z);
		glRotatef(chang.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(chang.escalaX,chang.escalaY,chang.escalaZ);
		DibujaChangoout();
	glPopMatrix();

	//tarantulas tipo pelicula Moonwalker
	glLineWidth(1.0f);
	glPushMatrix();   //1 (rocas inicio)
		glTranslatef(205.7f, 2.0f, -88.0f);
		glScalef(0.25,0.25,0.25);
		DibujaTarantulaout();
	glPopMatrix();
	glPushMatrix();   //2 (plataforma inicio grande)
		glTranslatef(230.0f, 2.5f, -35.0f);
		glRotatef(200.0f,0,1,0);
		glScalef(0.2,0.2,0.2);
		DibujaTarantulaout();
	glPopMatrix();
	glPushMatrix();   //3 (plataforma inicio chica)
		glTranslatef(229.0f, 5.5f, -32.0f);
		glRotatef(240.0f,0,1,0);
		glScalef(0.1,0.1,0.1);
		DibujaTarantulaout();
	glPopMatrix();
	glPushMatrix();   //4 (pared plano 2)
		glTranslatef(45.0f, 17.0f, -100.0f);
		glRotatef(100.0f,0,0,1);
		glRotatef(90.0f,1,0,0);
		glScalef(0.1,0.1,0.1);
		DibujaTarantulaout();
	glPopMatrix();
	glPopMatrix();
		glPushMatrix();   //5 (esquina interior)
		glTranslatef(-3.5f, 4.5f, -40.0f);
		glRotatef(160.0f,0,1,0);
		glScalef(0.11,0.11,0.11);
		DibujaTarantulaout();
	glPopMatrix();
	glPopMatrix();
		glPushMatrix();   //6 (rocas inicio plano 7)
		glTranslatef(-87.0f, 5.27f, 16.0f);
		glScalef(0.14,0.14,0.14);
		DibujaTarantulaout();
	glPopMatrix();
	glPopMatrix();
		glPushMatrix();   //7 (rocas final)
		glTranslatef(-16.0f, 5.3f, 115.0f);
		glRotatef(160.0f,0,1,0);
		glScalef(0.15,0.15,0.15);
		DibujaTarantulaout();
	glPopMatrix();

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
}

void DibujaEscena()
{
	// Escenario
	g_Load3ds.Render3DSFile(&g_3DModel1e, textureModel1e, 1);
	glDisable(GL_NORMALIZE);
}
void Camara()
{
	
	// Obten el valor de pisoID
	int c = pisoId;
	switch( pisoId )
	{
		case 0:
			gluLookAt( CamPos[ 0 ][ 0 ], CamPos[ 0 ][ 1 ],CamPos[ 0 ][ 2 ], CamPos[ 0 ][ 3 ], CamPos[ 0 ][ 4 ], CamPos[ 0 ][ 5 ], 0.0f, 1.0f, 0.0f);
		break;
		case 2:
			gluLookAt(CamPos[ 1 ][ 0 ], CamPos[ 1 ][ 1 ],CamPos[ 1 ][ 2 ], CamPos[ 1 ][ 3 ], CamPos[ 1 ][ 4 ], CamPos[ 1 ][ 5 ], 0.0f, 1.0f, 0.0f);
		break;
		case 6:
			gluLookAt(CamPos[ 2 ][ 0 ], CamPos[ 2 ][ 1 ],CamPos[ 2 ][ 2 ], CamPos[ 2 ][ 3 ], CamPos[ 2 ][ 4 ], CamPos[ 2 ][ 5 ], 0.0f, 1.0f, 0.0f);
		break;
		case 8:
			gluLookAt(CamPos[ 3 ][ 0 ], CamPos[ 3 ][ 1 ],CamPos[ 3 ][ 2 ], CamPos[ 3 ][ 3 ], CamPos[ 3 ][ 4 ], CamPos[ 3 ][ 5 ], 0.0f, 1.0f, 0.0f);
		break;
		case 1:
			gluLookAt( 190.0f - 5.8f * abs( player1.PosicionObj.x - 125.0f ), CamPos[ 0 ][ 1 ], CamPos[ 0 ][ 2 ], 
						180.0f - 5.0f * abs( player1.PosicionObj.x - 125.0f ), CamPos[ 0 ][ 4 ], CamPos[ 0 ][ 5 ], 
						0.0f, 1.0f, 0.0f );
		break;
		case 3: //15.5f
			gluLookAt( 45.0f - 1.93548f * abs( player1.PosicionObj.x - 5.5f ), CamPos[ 1 ][ 1 ], 10.0f - 4.25806f * abs( player1.PosicionObj.x - 5.5f ), 
						55.0f - 6.77419f * abs( player1.PosicionObj.x - 5.5f ), CamPos[ 1 ][ 4 ], -60.0f - 0.258065f * abs( player1.PosicionObj.x - 5.5f ), 
						0.0f, 1.0f, 0.0f );
		break;
		case 7: // 45.0f
			gluLookAt( 15.0f + 0.444f * abs( player1.PosicionObj.z - 5.0f ), CamPos[ 1 ][ 1 ], -56.0f + 2.9333f * abs( player1.PosicionObj.z - 5.0f ), 
						-50.0f - 0.222f * abs( player1.PosicionObj.z - 5.0f ), CamPos[ 1 ][ 4 ], -56.0f + 3.0222f * abs( player1.PosicionObj.z - 5.0f ), 
						0.0f, 1.0f, 0.0f );
		break;
		default:
			gluLookAt(player1.PosicionCam.x, player1.PosicionCam.y, player1.PosicionCam.z, 
				  player1.ObjetivoCam.x, player1.ObjetivoCam.y, player1.ObjetivoCam.z, 
					0.0f, 1.0f, 0.0f);
		break;
	}
}
int RenderizaEscena(GLvoid)								// Aqui se dibuja todo lo que aparecera en la ventana
{
	TPoint P;

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
	//DibujaSombraMJ();
			
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

	// Se desactiva la prueba de profundidad y del buffer stencil ya que no se utilizarán mas.
	glDisable(GL_STENCIL_TEST);

	// Dibujo de MJs
	glPushMatrix();
		DibujaMJ();
		//glTranslatef(40.0f, 10.0f,-35.0f);
		DibujaEnemigos();
	glPopMatrix();
		
	DibujaLuz(lightPosition);
	DibujaTextos();
	
	// Esta parte opcional muestra la silueta extruida que crea el volumen de sombra.
	if(displayVolume == true)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);     //Para que muestre el volumen en alambrado
		DibujaVolumendeSombra();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);     //Volvemos al modo sólido de nuevo
	}

	//splines
	if(running) 
	{	
		idxtp += dtidx;
		if( idxtp >= helspline.drawp - 5 || idxtp < 2 )
		{ 
			// ¿final o principio?
  			dtidx = -dtidx; // cambia el sentido de la camara
			//esperar = 50;
		}
		spline_point(helspline, idxtp, target);
	}

	//spline helicòptero
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
	for(int i=0; i < helspline.drawp; i++ ) 
	{
		spline_point( helspline, i, P );
		glVertex3fv( P );
	}
	glEnd();

	for (int i=0; i< helspline.tpc; i++ )
	{
		glColor3f(1.0f,1.0f,1.0f);
		glPushMatrix();
			glTranslatef(helspline.ctrlpoints[i][0],
						helspline.ctrlpoints[i][1],
						helspline.ctrlpoints[i][2]);
			glPointSize(10.0f);
			glColor3f(1.0f,0.0f,0.0f);
			glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
			glEnd();
						
		glPopMatrix();

		glColor3f(1.0f,1.0f,1.0f);
	}

	//spline camara
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	for(int i=0; i < camspline.drawp; i++ ) 
	{
		spline_point( camspline, i, P );
		glVertex3fv( P );
	}
	glEnd();

	for (int i=0; i< camspline.tpc; i++ )
	{
		glColor3f(1.0f,1.0f,1.0f);
		glPushMatrix();
			glTranslatef(camspline.ctrlpoints[i][0],
						camspline.ctrlpoints[i][1],
						camspline.ctrlpoints[i][2]);
			glPointSize(10.0f);
			glColor3f(0.0f,0.0f,1.0f);
			glBegin(GL_POINTS);
				glVertex3f(0.0f,0.0f,0.0f);
			glEnd();
						
		glPopMatrix();

		glColor3f(1.0f,1.0f,1.0f);
	}
	

	CalculateFrameRate();

	// Colisiones
	ActualizaObjetosDinamicosColision();
	DibujaObjetosdeColision();
	//DibujaEsferasColision();
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
							animacion(KeyFrame1, maxKF1 , 18, &player1modelo, playIndex );
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
	DescargaTexturas();
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