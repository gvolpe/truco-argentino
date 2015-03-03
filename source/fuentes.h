#ifndef FUENTES_H
#define FUENTES_H

class Fuentes
{
public:		
	TTF_Font * init(const char * fname, int tam);	
	void free();
};

/* Inicialización */
TTF_Font * Fuentes::init(const char * fname, int tam)
{
	TTF_Font * font;
	
	if (TTF_Init() == -1) 
		exit(1);
					
	font = TTF_OpenFont(fname, tam);
	if(font==NULL) exit(1);		
		
	return font;
}

#endif
