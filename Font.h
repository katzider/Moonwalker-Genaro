#ifndef __FONT_H__
#define __FONT_H__

#include "main.h"

class CFont
{
	public:
		CFont();
		~CFont(); 
		
		void BuildFont(GLvoid);
		void glPrint(float escala, GLint x, GLint y, const char *string, ...);
		void DestroyFont();

	private:
		
};

#endif