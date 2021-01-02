/********************* Cantos posibles ***************************/

const int MENU = -1;
const int TRUCO = 0;
const int RETRUCO = 1;
const int VALE4 = 2;
const int QUIERO = 3;
const int ENVIDO = 4;
const int REAL_ENVIDO = 5;
const int FALTA_ENVIDO = 6;
const int NO_QUIERO = 7;
const int AL_MAZO = 8;

/********************** Naipes Valores Jerárquicos ********************/

const int ANCHO_ESPADA = 20;
const int ANCHO_BASTO = 0;
const int SIETE_ESPADA = 26;
const int SIETE_ORO = 36;

const int TRES_BASTO = 2;
const int TRES_COPA = 12;
const int TRES_ESPADA = 22;
const int TRES_ORO = 32;

const int DOS_BASTO = 1;
const int DOS_COPA = 11;
const int DOS_ESPADA = 21;
const int DOS_ORO = 31;

const int AS_COPA = 10;
const int AS_ORO = 30;

const int REY_BASTO = 9;
const int REY_COPA = 19;
const int REY_ESPADA = 29;
const int REY_ORO = 39;

const int CABALLO_BASTO = 8;
const int CABALLO_COPA = 18;
const int CABALLO_ESPADA = 28;
const int CABALLO_ORO = 38;

const int SOTA_BASTO = 7;
const int SOTA_COPA = 17;
const int SOTA_ESPADA = 27;
const int SOTA_ORO = 37;

const int SIETE_BASTO = 6;
const int SIETE_COPA = 16;

const int SEIS_BASTO = 5;
const int SEIS_COPA = 15;
const int SEIS_ESPADA = 25;
const int SEIS_ORO = 35;

const int CINCO_BASTO = 4;
const int CINCO_COPA = 14;
const int CINCO_ESPADA = 24;
const int CINCO_ORO = 34;

const int CUATRO_BASTO = 3;
const int CUATRO_COPA = 13;
const int CUATRO_ESPADA = 23;
const int CUATRO_ORO = 33;

/************** Naipes Valores de los tantos del Envido **************/

const int TANTO_BASE = 20;
const int TANTO_AS = 1;
const int TANTO_DOS = 2;
const int TANTO_TRES = 3;
const int TANTO_CUATRO = 4;
const int TANTO_CINCO = 5;
const int TANTO_SEIS = 6;
const int TANTO_SIETE = 7;
const int TANTO_SOTA = 0;
const int TANTO_CABALLO = 0;
const int TANTO_REY = 0;

/*************************** Palos Naipes ***************************/

const int BASTO = 0;
const int COPA = 1;
const int ESPADA = 2;
const int ORO = 3;

/******************** Estado de los botones *************************/

const int ON = 1;
const int OFF = 0;
const int TRUCO_ON = ON;
const int TRUCO_OFF = OFF;
const int RETRUCO_ON = ON;
const int RETRUCO_OFF = OFF;
const int VALE4_ON = ON;
const int VALE4_OFF = OFF;
const int QUIERO_ON = ON;
const int QUIERO_OFF = OFF;
const int ENVIDO_ON = ON;
const int ENVIDO_OFF = OFF;
const int REAL_ENVIDO_ON = ON;
const int REAL_ENVIDO_OFF = OFF;
const int FALTA_ENVIDO_ON = ON;
const int FALTA_ENVIDO_OFF = OFF;
const int NO_QUIERO_ON = ON;
const int NO_QUIERO_OFF = OFF;
const int AL_MAZO_ON = ON;
const int AL_MAZO_OFF = OFF;

/*************************** Actores del juego ************************/

const int HUMANO = 0;
const int PC = 1;

/*************************** Acciones del humano *************************/

const int SOLO_PRESIONAR_BOTON = 2;
const int JUGAR_NAIPE_PRESIONAR_BOTON = 0; //Jugar y/o Presionar

/*************************** Acciones de la Pc *************************/

const int JUGAR_SIN_VERIFICAR_NAIPE = 0;
const int JUGAR_VERIFICANDO_NAIPE = 1;

/*************************** Esperas y Respuestas *************************/

const int RTA_TRUCO = 9;
const int RTA_ENVIDO = 10;
const int RTA_REAL_ENVIDO = 11;
const int RTA_FALTA_ENVIDO = 12;
const int RTA_RETRUCO = 13;
const int RTA_VALE4 = 14;
const int RTA_OFF = -1;

/***************************** Nivel de Naipes ****************************/

const int EXCELENTES = 0;
const int MUY_BUENAS = 1;
const int BUENAS = 2;
const int MALAS = 3;
const int MUY_MALAS = 4;

/******************************* Opciones *********************************/

const int MANO_PC = 0;
const int MANO_HUMANO = 1;
const int UNA_FALTA = 1;
const int DOS_FALTAS = 2;

/***************************** Menú de Opciones ****************************/

const int JUEGO_NUEVO = 1;
const int OPCIONES = 2;
const int REGLAMENTO = 3;
const int CREDITOS = 4;
const int SALIR = 5;

/******************* Jugadas (Primera, Segunda, Tercera) ******************/

const int DE_HUMANO = 0;
const int DE_PC = 1;
const int PARDA = 2;

/******************************** Misceláneas *****************************/

const int RECHAZADO = 0;
const int ACEPTADO = 1;
const int CANTADO = 1;
const int NO_CANTADO = 0;
