#include "Main.h"
#include "3ds.h"

// Probando esta mamada XD
// lol hcker
// Agregando comentario de prueba
//Y viendo si este funciona... FAHL


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

parametros player1;  //Variable con la que tenemos acceso a la estructura de parámetros

// Variable de acceso a la estructura de parametros
parametros player1aru;

parametros enem1;	 //Variable con la que tenemos acceso a la estructura de parámetros de ene1
parametros MJ6;		 //Variable con la que tenemos acceso a la estructura de parámetros de MJ6

CMateriales Material;

//Nombre y ubicación de los modelos
#define FILE_NAME1c  "Modelos/bob_torso.3ds"
#define FILE_NAME2c  "Modelos/bob_piernader_a.3ds"
#define FILE_NAME3c  "Modelos/bob_piernader_b.3ds"
#define FILE_NAME4c  "Modelos/bob_piernaizq_a.3ds"
#define FILE_NAME5c  "Modelos/bob_piernaizq_b.3ds"
#define FILE_NAME6c  "Modelos/bob_brazoder_a.3ds"
#define FILE_NAME7c  "Modelos/bob_brazoizq_a.3ds"
#define FILE_NAME8c  "Modelos/bob_brazoder_b.3ds"
#define FILE_NAME9c  "Modelos/bob_brazoizq_b.3ds"

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

CTga textureModel1c[20];
CTga textureModel2c[20];
CTga textureModel3c[20];
CTga textureModel4c[20];
CTga textureModel5c[20];
CTga textureModel6c[20];
CTga textureModel7c[20];
CTga textureModel8c[20];
CTga textureModel9c[20];
//Contenedor de texturas de enemigo1
CTga textureModel1d[20];
//Contenedor de texturas de MJ6 (MJ Robot)
CTga textureModel1f[20];


// Contenedores de texturas de los modelos de Aru(MJ y Miku)
CTga textureModel1aru[20]; //MJ.tga

// Contenedores de texturas del escenario
CTga textureModel2e[20];

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
//Escenario

//Contenedor de texturas adicionales
CTga textura[30];

jerarquiaModelo player1modelo;	//Acceso a la estructura con las variables de cada pieza del modelo
const int maxKF1=3;				//Num. total de KeyFrames para la secuencia 1 (caminar)
FRAME KeyFrame1[maxKF1];		//Contenedor para almacenar cada keyframe de la secuencia 1

jerarquiaModelo player1modeloaru;

jerarquiaModelo enem1modelo;
jerarquiaModelo MJ6modelo;

bool play=false;//Bandera para iniciar la animación
int playIndex=0;//Auxiliar para leer la información del contenedor de keyframes
int tipoAnim=1; //Indicador del tipo de animación

CMultitexturas Multitext;

// Modelo de Bob
GLuint modelo1;
GLuint ene1;
GLuint noMJ6;

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
	
	if(!g_Load3ds.Load3DSFile(FILE_NAME1c, &g_3DModel1c, textureModel1c))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME2c, &g_3DModel2c, textureModel2c))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME3c, &g_3DModel3c, textureModel3c))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME4c, &g_3DModel4c, textureModel4c))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME5c, &g_3DModel5c, textureModel5c))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME6c, &g_3DModel6c, textureModel6c))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME7c, &g_3DModel7c, textureModel7c))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME8c, &g_3DModel8c, textureModel8c))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME9c, &g_3DModel9c, textureModel9c))
		return 0;
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
		
	return TRUE;
}

void DescargaModelos()
{
	g_Load3ds.UnLoad3DSFile(&g_3DModel1c, textureModel1c);
	g_Load3ds.UnLoad3DSFile(&g_3DModel2c, textureModel2c);
	g_Load3ds.UnLoad3DSFile(&g_3DModel3c, textureModel3c);
	g_Load3ds.UnLoad3DSFile(&g_3DModel4c, textureModel4c);
	g_Load3ds.UnLoad3DSFile(&g_3DModel5c, textureModel5c);
	g_Load3ds.UnLoad3DSFile(&g_3DModel6c, textureModel6c);
	g_Load3ds.UnLoad3DSFile(&g_3DModel7c, textureModel7c);
	g_Load3ds.UnLoad3DSFile(&g_3DModel8c, textureModel8c);
	g_Load3ds.UnLoad3DSFile(&g_3DModel9c, textureModel9c);
	
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

	//Escenario
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
	modelo1=glGenLists(9);
	//Ene1
	ene1=glGenLists(10);
	//MJ6
	noMJ6=glGenLists(10);

	glNewList(modelo1+0,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel1c, textureModel1c, 1);
	glEndList();

	glNewList(modelo1+1,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel2c, textureModel2c, 1);
	glEndList();

	glNewList(modelo1+2,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel3c, textureModel3c, 1);
	glEndList();

	glNewList(modelo1+3,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel4c, textureModel4c, 1);
	glEndList();

	glNewList(modelo1+4,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel5c, textureModel5c, 1);
	glEndList();

	glNewList(modelo1+5,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel6c, textureModel6c, 1);
	glEndList();

	glNewList(modelo1+6,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel7c, textureModel7c, 1);
	glEndList();

	glNewList(modelo1+7,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel8c, textureModel8c, 1);
	glEndList();

	glNewList(modelo1+8,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel9c, textureModel9c, 1);
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
}

void DestruyeListas()
{
	glDeleteLists(modelo1,9);

	// Borra listas de Aru
	glDeleteLists(modelo1aru,9);
	// Borra listas fahl
	glDeleteLists(ene1,10);
	glDeleteLists(noMJ6,10);

}

void InicializaParametrosdeControl()
{
	//Esta función establece los parámetros como velocidad del objeto y distancia de la cámara así como la posición y dirección iniciales
	player1.visible=true;
	player1.VelocidadObj=0.2f;
	player1.DistanciaCam=10.0f;

	player1.CamaraPosAlt=5.0f;	//Posición en y de la cámara (altura a la que se situa la cámara)
	player1.CamaraObjAlt=4.0f;	//Posición en y del objetivo de la cámara (altura a la que ve la cámara)
	player1.AngDir=90.0f;		//Este ángulo inicial hace que la dirección inicial sea paralela al eje Z y con sentido negativo
	player1.AngObj=0.0f;		//Este valor se elige dependiendo de la orientación con la que aparece el modelo en la escena al dibujarlo
								//sin aplicarle ninguna transformación (hacia adonde está volteando). Se elige un ángulo tal que al aplicarle
								//una rotación inicial con respecto al eje Y esté viendo hacia la misma dirección que la definida por AngDir
	
	player1.PosicionObj=CVector(0.0f, 0.0f, 0.0f); //Esta es la posición inicial del objeto en la escena
	player1.Direccion.x=(float)cos(player1.AngDir*PI/180.0f); //Dirección inicial definida por el ángulo inicial AngDir (x=cos(AngDir), y=0.0, z=sen(AngDir))
	player1.Direccion.y=0.0f;
	player1.Direccion.z=(float)sin(player1.AngDir*PI/180.0f);   
	player1.PosicionCam=CVector(0.0f, player1.CamaraPosAlt, 10.0f); //Posición inicial de la cámara a [DistanciaCam] unidades detrás del objeto
	player1.ObjetivoCam=player1.PosicionObj;		//La cámara ve siempre al objeto
	player1.ObjetivoCam.y=player1.CamaraObjAlt;		//Para que no vea a los "pies" del objeto (personaje)

	player1.Dir=0;
	player1.DirAnt=0;

	player1.escalaX=0.4f;
	player1.escalaY=0.4f;
	player1.escalaZ=0.4f;

	player1.CamaraObjAltE=0.0f;

	// Inicializa parametros de control para Aru
	//Esta función establece los parámetros como velocidad del objeto y distancia de la cámara así como la posición y dirección iniciales
	player1aru.visible=true;
	player1aru.VelocidadObj=0.2f;
	player1aru.DistanciaCam=10.0f;

	player1aru.CamaraPosAlt=5.0f;	//Posición en y de la cámara (altura a la que se situa la cámara)
	player1aru.CamaraObjAlt=4.0f;	//Posición en y del objetivo de la cámara (altura a la que ve la cámara)
	player1aru.AngDir=90.0f;		//Este ángulo inicial hace que la dirección inicial sea paralela al eje Z y con sentido negativo
	player1aru.AngObj=0.0f;		//Este valor se elige dependiendo de la orientación con la que aparece el modelo en la escena al dibujarlo
								//sin aplicarle ninguna transformación (hacia adonde está volteando). Se elige un ángulo tal que al aplicarle
								//una rotación inicial con respecto al eje Y esté viendo hacia la misma dirección que la definida por AngDir
	
	player1aru.PosicionObj=CVector(0.0f, 0.0f, 0.0f); //Esta es la posición inicial del objeto en la escena
	player1aru.Direccion.x=(float)cos(player1aru.AngDir*PI/180.0f); //Dirección inicial definida por el ángulo inicial AngDir (x=cos(AngDir), y=0.0, z=sen(AngDir))
	player1aru.Direccion.y=0.0f;
	player1aru.Direccion.z=(float)sin(player1aru.AngDir*PI/180.0f);   
	player1aru.PosicionCam=CVector(0.0f, player1aru.CamaraPosAlt, 10.0f); //Posición inicial de la cámara a [DistanciaCam] unidades detrás del objeto
	player1aru.ObjetivoCam=player1aru.PosicionObj;		//La cámara ve siempre al objeto
	player1aru.ObjetivoCam.y=player1aru.CamaraObjAlt;		//Para que no vea a los "pies" del objeto (personaje)

	player1aru.Dir=0;
	player1aru.DirAnt=0;

	player1aru.escalaX=0.4f;
	player1aru.escalaY=0.4f;
	player1aru.escalaZ=0.4f;
	
	player1aru.CamaraObjAltE=0.0f;
	
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
	
	MJ6.PosicionObj=CVector(-5.0f, 0.0f, 0.0f); //Esta es la posición inicial del objeto en la escena
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
}

void DatosAnimacion()
{
	//Secuencia 1
	KeyFrame1[0].Angt1=-5.0f;
	KeyFrame1[0].Angt2=0.0f;
	KeyFrame1[0].Angc1=-25.0f;
	KeyFrame1[0].Angc2=0.0f;
	KeyFrame1[0].Angbi1=-100.0f;
	KeyFrame1[0].Angbi2=0.0f;
	KeyFrame1[0].Angbib=-30.0f;
	KeyFrame1[0].Angbd1=120.0f;
	KeyFrame1[0].Angbd2=0.0f;
	KeyFrame1[0].Angbdb=-80.0f;
	KeyFrame1[0].Angpizq=40.0f;
	KeyFrame1[0].Angpder=-60.0f;
	KeyFrame1[0].Angpizqb=30.0f;
	KeyFrame1[0].Angpderb=40.0f;
	KeyFrame1[0].Angpi=0.0f;
	KeyFrame1[0].Angpd=0.0f;
	KeyFrame1[0].Xtor=0.0f;
	KeyFrame1[0].Ytor=0.0f;
	KeyFrame1[0].Ztor=0.0f;

	KeyFrame1[1].Angt1=-5.0f;
	KeyFrame1[1].Angt2=0.0f;
	KeyFrame1[1].Angc1=-25.0f;
	KeyFrame1[1].Angc2=0.0f;
	KeyFrame1[1].Angbi1=120.0f;
	KeyFrame1[1].Angbi2=0.0f;
	KeyFrame1[1].Angbib=-80.0f;
	KeyFrame1[1].Angbd1=-100.0f;
	KeyFrame1[1].Angbd2=0.0f;
	KeyFrame1[1].Angbdb=-30.0f;
	KeyFrame1[1].Angpizq=-60.0f;
	KeyFrame1[1].Angpder=40.0f;
	KeyFrame1[1].Angpizqb=40.0f;
	KeyFrame1[1].Angpderb=30.0f;
	KeyFrame1[1].Angpi=0.0f;
	KeyFrame1[1].Angpd=0.0f;
	KeyFrame1[1].Xtor=0.0f;
	KeyFrame1[1].Ytor=0.0f;
	KeyFrame1[1].Ztor=0.0f;

	KeyFrame1[2].Angt1=-5.0f;
	KeyFrame1[2].Angt2=0.0f;
	KeyFrame1[2].Angc1=-25.0f;
	KeyFrame1[2].Angc2=0.0f;
	KeyFrame1[2].Angbi1=-100.0f;
	KeyFrame1[2].Angbi2=0.0f;
	KeyFrame1[2].Angbib=-30.0f;
	KeyFrame1[2].Angbd1=120.0f;
	KeyFrame1[2].Angbd2=0.0f;
	KeyFrame1[2].Angbdb=-80.0f;
	KeyFrame1[2].Angpizq=40.0f;
	KeyFrame1[2].Angpder=-40.0f;
	KeyFrame1[2].Angpizqb=30.0f;
	KeyFrame1[2].Angpderb=20.0f;
	KeyFrame1[2].Angpi=0.0f;
	KeyFrame1[2].Angpd=0.0f;
	KeyFrame1[2].Xtor=0.0f;
	KeyFrame1[2].Ytor=0.0f;
	KeyFrame1[2].Ztor=0.0f;
			
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
	if(funcion == 1) //Giro a la derecha
	{
		player1.AngDir+=1.0f;
		if(player1.AngDir > 360.0f)
			player1.AngDir-=360.0f;

		player1.AngObj-=1.0f;
		if(player1.AngObj < 0.0f)
			player1.AngObj+=360.0f;

		player1.Direccion.x=(float)cos(player1.AngDir*PI/180.0f);
		player1.Direccion.y=0.0f;
		player1.Direccion.z=(float)sin(player1.AngDir*PI/180.0f);

		player1.PosicionCam=player1.PosicionObj-player1.Direccion*player1.DistanciaCam;
		player1.PosicionCam.y=player1.CamaraPosAlt;
		player1.ObjetivoCam=player1.PosicionObj;
		player1.ObjetivoCam.y=player1.CamaraObjAlt;
	}
	else if(funcion == 2) //Giro a la izquierda
	{
		player1.AngDir-=1.0f;
		if(player1.AngDir < 0.0f)
			player1.AngDir+=360.0f;

		player1.AngObj+=1.0f;
		if(player1.AngObj > 360.0f)
			player1.AngObj-=360.0f;

		player1.Direccion.x=(float)cos(player1.AngDir*PI/180.0f);
		player1.Direccion.y=0.0f;
		player1.Direccion.z=(float)sin(player1.AngDir*PI/180.0f);

		player1.PosicionCam=player1.PosicionObj-player1.Direccion*player1.DistanciaCam;
		player1.PosicionCam.y=player1.CamaraPosAlt;
		player1.ObjetivoCam=player1.PosicionObj;
		player1.ObjetivoCam.y=player1.CamaraObjAlt;
	}
	else if(funcion == 3) //Avanza hacia adelante
	{
		player1.PosicionObj=player1.PosicionObj+player1.Direccion*player1.VelocidadObj;
		player1.PosicionCam=player1.PosicionObj-player1.Direccion*player1.DistanciaCam;
		player1.PosicionCam.y=player1.CamaraPosAlt;
		player1.ObjetivoCam=player1.PosicionObj;
		player1.ObjetivoCam.y=player1.CamaraObjAlt;

		player1.PosAntObj=player1.PosicionObj;

	}
	else if(funcion == 4) //Avanza hacia atrás
	{
		player1.PosicionObj=player1.PosicionObj-player1.Direccion*player1.VelocidadObj;
		player1.PosicionCam=player1.PosicionObj-player1.Direccion*player1.DistanciaCam;
		player1.PosicionCam.y=player1.CamaraPosAlt;
		player1.ObjetivoCam=player1.PosicionObj;
		player1.ObjetivoCam.y=player1.CamaraObjAlt;

		player1.PosAntObj=player1.PosicionObj;

	}
	else if(funcion == 5) //Sube objetivo de la cámara
	{
		player1.ObjetivoCam.y+=1.0f;
		player1.CamaraObjAlt+=1.0f;
		player1.CamaraObjAltE+=1.0f;
		
	}
	else if(funcion == 6) //Baja objetivo de la cámara
	{
		player1.ObjetivoCam.y-=1.0f;
		player1.CamaraObjAlt-=1.0f;
		player1.CamaraObjAltE-=1.0f;
		
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

void DibujaPersonaje()
{
	glTranslatef(player1modelo.Xtor, player1modelo.Ytor, player1modelo.Ztor);
	glRotatef(player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
	glRotatef(player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
			
	//Torso
	glCallList(modelo1+0);
	
	//Pierna derecha
	glPushMatrix();
		glTranslatef(-1.2f, -1.3f ,0.0f);
		glRotatef(player1modelo.Angpder, 1.0f, 0.0f, 0.0f);
		glCallList(modelo1+1);
		
		//Pierna derecha_b
		glPushMatrix();
			glTranslatef(0.0f, -1.25f , 0.0f);
			glRotatef(player1modelo.Angpderb, 1.0f, 0.0f, 0.0f);
			glCallList(modelo1+2);
		glPopMatrix();

	glPopMatrix();

	//Pierna izquierda
	glPushMatrix();
		glTranslatef(1.2f, -1.3f ,0.0f);
		glRotatef(player1modelo.Angpizq, 1.0f, 0.0f, 0.0f);
		glCallList(modelo1+3);

		//Pierna izquierda_b
		glPushMatrix();
			glTranslatef(0.0f, -1.25f , 0.0f);
			glRotatef(player1modelo.Angpizqb, 1.0f, 0.0f, 0.0f);
			glCallList(modelo1+4);
		glPopMatrix();

	glPopMatrix();

	//Brazo derecho_a
	glPushMatrix();
		glTranslatef(-2.8f, 1.1f, 0.0f);
		glRotatef(player1modelo.Angbd2, 0.0f, 1.0f, 0.0f);
		glRotatef(player1modelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glCallList(modelo1+5);

		//Brazo derecho_b
		glPushMatrix();
			glTranslatef(-0.35f, -1.5f, 0.0f);
			glRotatef(player1modelo.Angbdb, 1.0f, 0.0f, 0.0f);
			glCallList(modelo1+7);
		glPopMatrix();

	glPopMatrix();

	//Brazo izquierdo
	glPushMatrix();
		glTranslatef(2.8f, 1.1f, 0.0f);
		glRotatef(player1modelo.Angbi2, 0.0f, 1.0f, 0.0f);
		glRotatef(player1modelo.Angbi1, 1.0f, 0.0f, 0.0f);
		glCallList(modelo1+6);

		//Brazo izquierdo_b
		glPushMatrix();
			glTranslatef(0.35f, -1.5f, 0.0f);
			glRotatef(player1modelo.Angbib, 1.0f, 0.0f, 0.0f);
			glCallList(modelo1+8);
		glPopMatrix();

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

void DibujaSombraPersonaje()
{
	glPushMatrix();

		glTranslatef(player1.PosicionObj.x, player1.PosicionObj.y+2.4f, player1.PosicionObj.z);
		glRotatef(player1.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(player1.escalaX,player1.escalaY,player1.escalaZ);
		glTranslatef(player1modelo.Xtor, player1modelo.Ytor, player1modelo.Ztor);
		glRotatef(player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
				
		//Torso
		objSh.calculaSombraDepthPass(&g_3DModel1c, objectSpaceLightPosition1);
		
		//Pierna derecha
		glPushMatrix();
			glTranslatef(-1.2f, -1.3f ,0.0f);
			glRotatef(player1modelo.Angpder, 1.0f, 0.0f, 0.0f);
			objSh.calculaSombraDepthPass(&g_3DModel2c, objectSpaceLightPosition2);
			
			//Pierna derecha_b
			

		glPopMatrix();

		//Pierna izquierda
		

			//Pierna izquierda_b
			

		//Brazo derecho_a
		

			//Brazo derecho_b
			

		//Brazo izquierdo
		

			//Brazo izquierdo_b
		

	glPopMatrix();
		
}

void DibujaVolumendeSombra()
{
	glPushMatrix();

		glTranslatef(player1.PosicionObj.x, player1.PosicionObj.y+2.4f, player1.PosicionObj.z);
		glRotatef(player1.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(player1.escalaX,player1.escalaY,player1.escalaZ);
		glTranslatef(player1modelo.Xtor, player1modelo.Ytor, player1modelo.Ztor);
		glRotatef(player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
				
		//Torso
		objSh.DrawShadowVolume(&g_3DModel1c, objectSpaceLightPosition1);
		
		//Pierna derecha
		glPushMatrix();
			glTranslatef(-1.2f, -1.3f ,0.0f);
			glRotatef(player1modelo.Angpder, 1.0f, 0.0f, 0.0f);
			objSh.DrawShadowVolume(&g_3DModel2c, objectSpaceLightPosition2);
			
			//Pierna derecha_b
			

		glPopMatrix();

		//Pierna izquierda
		

		//Brazo derecho_a
		

			//Brazo derecho_b
			

		//Brazo izquierdo
		

			//Brazo izquierdo_b
			

	glPopMatrix();
}


// Dibuja Personajes de Aru
void DibujaPersonajeAru()
{
	glTranslatef(player1modeloaru.Xtor, player1modeloaru.Ytor, player1modeloaru.Ztor);
	glRotatef(player1modeloaru.Angt2, 0.0f, 1.0f, 0.0f);
	glRotatef(player1modeloaru.Angt1, 1.0f, 0.0f, 0.0f);
			
	//Torso
	glCallList(modelo1aru+0);
	
	//Pierna derecha
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, 0.0f);
		glRotatef(player1modeloaru.Angpder, 1.0f, 0.0f, 0.0f);
		glCallList(modelo1aru+1);
		
		//Pierna derecha_b
		glPushMatrix();
			glTranslatef(-4.5f, 0.7f , 0.0f);
			glRotatef(player1modeloaru.Angpderb, 1.0f, 0.0f, 0.0f);
			glCallList(modelo1aru+2);
		glPopMatrix();

	glPopMatrix();

	//Pierna izquierda
	glPushMatrix();
		glTranslatef(0.0f, 0.0f ,0.0f);
		glRotatef(player1modeloaru.Angpizq, 1.0f, 0.0f, 0.0f);
		glCallList(modelo1aru+3);

		//Pierna izquierda_b
		glPushMatrix();
			glTranslatef(-6.5f, 0.8f , 0.0f);
			glRotatef(player1modeloaru.Angpizqb, 1.0f, 0.0f, 0.0f);
			glCallList(modelo1aru+4);
		glPopMatrix();

	glPopMatrix();

	//Brazo derecho_a
	glPushMatrix();
		glTranslatef(5.0f, 0.0f, 0.0f);
		glRotatef(player1modeloaru.Angbd2, 0.0f, 1.0f, 0.0f);
		glRotatef(player1modeloaru.Angbd1, 1.0f, 0.0f, 0.0f);
		glCallList(modelo1aru+5);

		//Brazo derecho_b
		glPushMatrix();
			glTranslatef(-4.6f, -0.0f, 0.0f);
			glRotatef(player1modeloaru.Angbdb, 1.0f, 0.0f, 0.0f);
			glCallList(modelo1aru+7);
		glPopMatrix();

	glPopMatrix();

	//Brazo izquierdo
	glPushMatrix();
		glTranslatef(2.8f, 1.1f, 0.0f);
		glRotatef(player1modeloaru.Angbi2, 0.0f, 1.0f, 0.0f);
		glRotatef(player1modeloaru.Angbi1, 1.0f, 0.0f, 0.0f);
		glCallList(modelo1aru+6);

		//Brazo izquierdo_b
		glPushMatrix();
			glTranslatef(0.35f, -1.5f, 0.0f);
			glRotatef(player1modeloaru.Angbib, 1.0f, 0.0f, 0.0f);
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
	// Font.glPrint((2.0f/640.0f)*glWidth, glWidth*0.05f,glHeight*0.9f,"Ejemplo texto");
	// Font.glPrint((1.2f/640.0f)*glWidth, glWidth*0.05f,glHeight*0.85f,"FPS %d",FPS);
								
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

void DibujaEscena()
{
	// Mayralol
	g_Load3ds.Render3DSFile(&g_3DModel2e, textureModel2e, 1);

	glEnable(GL_NORMALIZE);
	glPushMatrix();
		glTranslatef(player1.PosicionObj.x, player1.PosicionObj.y+2.4f, player1.PosicionObj.z);
		glRotatef(player1.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(player1.escalaX,player1.escalaY,player1.escalaZ);
		DibujaPersonaje();
	glPopMatrix();

	// Aru
	glPushMatrix();
		glTranslatef(player1aru.PosicionObj.x-40.0f, player1aru.PosicionObj.y+2.4f, player1aru.PosicionObj.z+40.0f);
		glRotatef(player1aru.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(player1aru.escalaX,player1aru.escalaY,player1aru.escalaZ);
		DibujaPersonajeAru();
	glPopMatrix();

	//Ene1
	glPushMatrix();
		glTranslatef(enem1.PosicionObj.x, enem1.PosicionObj.y+2.4f, enem1.PosicionObj.z);
		glRotatef(enem1.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(enem1.escalaX,enem1.escalaY,enem1.escalaZ);
		DibujaEnemigo1();
	glPopMatrix();

	//MJ6
	glPushMatrix();
		glTranslatef(MJ6.PosicionObj.x, MJ6.PosicionObj.y+2.4f, MJ6.PosicionObj.z);
		glRotatef(MJ6.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(MJ6.escalaX,MJ6.escalaY,MJ6.escalaZ);
		DibujaMJ6();
	glPopMatrix();

	glDisable(GL_NORMALIZE);
}

int RenderizaEscena(GLvoid)								// Aqui se dibuja todo lo que aparecera en la ventana
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();
	
	gluLookAt(10.0f, 45.0f, 45.0f, 10.0f, 25.0f, 0.0f, 0, 1, 0);
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
		glScalef(0.7f,0.7f,0.7f);
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
		glScalef(0.7f,0.7f,0.7f);
		DibujaEscena();
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
		if(player1.PosicionObj.y < 20.0f)
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
			player1.PosicionObj.y-=0.4f;
			player1.CamaraPosAlt-=0.4f;
			player1.CamaraObjAlt-=0.4f;

			player1.PosicionCam.y=player1.CamaraPosAlt;
			player1.ObjetivoCam=player1.PosicionObj;
			player1.ObjetivoCam.y=player1.CamaraObjAlt;
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