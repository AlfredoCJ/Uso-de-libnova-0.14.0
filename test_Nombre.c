#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <libnova/rise_set.h>
#include <libnova/transform.h>
#include <libnova/julian_day.h>
#include <libnova/ln_types.h>
#include <libnova/solar.h>
#include <libnova/earth.h>
#include <libnova/lunar.h>
#include <libnova/mercury.h>
#include <libnova/venus.h>
#include <libnova/mars.h>
#include <libnova/jupiter.h>
#include <libnova/saturn.h>
#include <libnova/uranus.h>
#include <libnova/neptune.h>
#include <libnova/pluto.h>

struct ln_dms convierteGdGms(double coordDecimal) {
	struct ln_dms resultado;
	// Asigno el positivo o negativo y los grados son la parte entera
	if (coordDecimal < 0) {
		resultado.neg   = 1;
		resultado.degrees = -1 * (int) coordDecimal;
	}else{
		resultado.neg   = 0;
		resultado.degrees = (int) coordDecimal;
	}
	// Para los minutos: quitamos la parte entera, multiplicamos por 60 y cogemos la parte entera de ese resultado
	double min_seg = (coordDecimal - resultado.degrees) * 60;
	resultado.minutes = (int) min_seg;
	// Para los segundos: quitamos la parte entera, multiplicamos por 60 y esos son los segundos
	resultado.seconds = (min_seg - resultado.minutes) * 60;
	return resultado;
}

struct ln_hms convierteGdHms(double coordDecimal) {
	struct ln_hms resultado;
	// Las horas son la parte entera
	resultado.hours = -1 * (int) coordDecimal;
	// Para los minutos: quitamos la parte entera, multiplicamos por 60 y cogemos la parte entera de ese resultado
	double min_seg = (coordDecimal - resultado.hours) * 60;
	resultado.minutes = (int) min_seg;
	// Para los segundos: quitamos la parte entera, multiplicamos por 60 y esos son los segundos
	resultado.seconds = (min_seg - resultado.minutes) * 60;
	return resultado;
}

char* traduceCoordenadasString(struct ln_hrz_posn objAltAz) {
    char* strResp = malloc(sizeof(char) * 100);
    sprintf(strResp, "{'resp':'ok','alt':'%f','az':'%f'}", objAltAz.alt, objAltAz.az);
    return strResp;
}

struct ln_hrz_posn getAltazFromRadecInDate(struct ln_equ_posn objRaDec, double JD) {
	// Datos de la localización
	struct ln_lnlat_posn localizacion;
	localizacion.lat = 36.7123456;
	localizacion.lng = -4.4123456;

	// Cogemos los datos de la Ascención Recta (1ª ln_dms del array)
	struct lnh_equ_posn objRaDec2;
	objRaDec2.ra = convierteGdHms(objRaDec.ra);
	// Cogemos los datos de la Declinación (2ª ln_dms del array)
	objRaDec2.dec = convierteGdGms(objRaDec.dec);

    // Se realizan las conversiones de posición de objeto y observador
	struct ln_equ_posn object;
    ln_hequ_to_equ(&objRaDec2, &object);
    struct ln_lnlat_posn observer;
    ln_hlnlat_to_lnlat(&localizacion, &observer);
    // Se traducen a AltAz
    struct ln_hrz_posn resultado;
    ln_get_hrz_from_equ(&object, &observer, JD, &resultado);

    // Si es necesario se imprimen por pantalla
    fprintf(stdout, "Alt: %f\n",   resultado.alt);
    fprintf(stdout, "Az:  %f\n\n", resultado.az);
	
	// Creo el JSON de las ln_dmss y lo devuelvo
	return resultado;
}

char* getAltazFromName(char* objStr) {
	char* auxStr = malloc(sizeof(char) * 10);
	struct ln_equ_posn equ;
	int encontrado = 0;
	double ra, dec;

	// Extraigo la fecha en formato JD
	double JD = ln_get_julian_from_sys();

	// Probamos con el sol
	sprintf(auxStr, "sol");
	if (strcmp(objStr, auxStr) == 0) {
		ln_get_solar_equ_coords(JD, &equ);
		fprintf(stdout, "sol  RA %f  Dec %f\n", equ.ra, equ.dec);
		encontrado = 1;
	}

	// Probamos con la luna
	sprintf(auxStr, "luna");
	if (strcmp(objStr, auxStr) == 0) {
		ln_get_lunar_equ_coords(JD, &equ);
		fprintf(stdout, "luna  RA %f  Dec %f\n", equ.ra, equ.dec);
		encontrado = 1;
	}

	// Probamos con mercurio
	sprintf(auxStr, "mercurio");
	if (strcmp(objStr, auxStr) == 0) {
		ln_get_mercury_equ_coords(JD, &equ);
		fprintf(stdout, "mercurio  RA %f  Dec %f\n", equ.ra, equ.dec);
		encontrado = 1;
	}

	// Probamos con venus
	sprintf(auxStr, "venus");
	if (strcmp(objStr, auxStr) == 0) {
		ln_get_venus_equ_coords(JD, &equ);
		fprintf(stdout, "venus  RA %f  Dec %f\n", equ.ra, equ.dec);
		encontrado = 1;
	}

	// Probamos con marte
	sprintf(auxStr, "marte");
	if (strcmp(objStr, auxStr) == 0) {
		ln_get_mars_equ_coords(JD, &equ);
		fprintf(stdout, "marte  RA %f  Dec %f\n", equ.ra, equ.dec);
		encontrado = 1;
	}

	// Probamos con jupiter
	sprintf(auxStr, "jupiter");
	if (strcmp(objStr, auxStr) == 0) {
		ln_get_jupiter_equ_coords(JD, &equ);
		fprintf(stdout, "jupiter  RA %f  Dec %f\n", equ.ra, equ.dec);
		encontrado = 1;
	}

	// Probamos con saturno
	sprintf(auxStr, "saturno");
	if (strcmp(objStr, auxStr) == 0) {
		ln_get_saturn_equ_coords(JD, &equ);
		fprintf(stdout, "saturno  RA %f  Dec %f\n", equ.ra, equ.dec);
		encontrado = 1;
	}

	// Probamos con urano
	sprintf(auxStr, "urano");
	if (strcmp(objStr, auxStr) == 0) {
		ln_get_uranus_equ_coords(JD, &equ);
		fprintf(stdout, "urano  RA %f  Dec %f\n", equ.ra, equ.dec);
		encontrado = 1;
	}

	// Probamos con neptuno
	sprintf(auxStr, "neptuno");
	if (strcmp(objStr, auxStr) == 0) {
		ln_get_neptune_equ_coords(JD, &equ);
		fprintf(stdout, "neptuno  RA %f  Dec %f\n", equ.ra, equ.dec);
		encontrado = 1;
	}

	// Probamos con pluton
	sprintf(auxStr, "pluton");
	if (strcmp(objStr, auxStr) == 0) {
		ln_get_pluto_equ_coords(JD, &equ);
		fprintf(stdout, "pluton  RA %f  Dec %f\n", equ.ra, equ.dec);
		encontrado = 1;
	}

	// Si encontré algo
	if (encontrado == 1) {
		// Creo la cadena y la devuelvo
	    struct ln_hrz_posn objAltAz = getAltazFromRadecInDate(equ, JD);
	    char* strResp = traduceCoordenadasString(objAltAz);
	    return strResp;
	}else{
		char* strResp = malloc(sizeof(char) * 40);
		sprintf(strResp, "{'resp':'ko','msg':'Objeto desconocido'}");
	    return strResp;
	}
}

int main(int argc, char *argv[]) {
	// Creo el string con el objeto
	char* objeto = malloc(sizeof(char) * 25);
	// Lo muestro
	sprintf(objeto, "luna");
	// Lo convierto a AltAz
	char* strResp = getAltazFromName(objeto);
	// Y muestro el resultado
	fprintf(stdout, "Respuesta: %s\n\n", strResp);
	return 0;
}



