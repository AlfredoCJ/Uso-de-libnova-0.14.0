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
	// Las horas son la parte entera dividido entre 15 (1h=15º)
	resultado.hours = (int) (coordDecimal / 15);
	// Para los minutos: quitamos las horas, multiplicamos por 60 y cogemos la parte entera de ese resultado dividido por 15
	double min_seg = (coordDecimal - (resultado.hours * 15)) * 60;
	resultado.minutes = (int) (min_seg / 15);
	// Para los segundos: quitamos los minutos, multiplicamos por 3.988... y esos son los segundos
	resultado.seconds = (min_seg - (resultado.minutes * 15)) * 3.98809523;
	return resultado;
}

char* traduceCoordenadasString(struct ln_hrz_posn objAltAz) {
	int altInt = (int) objAltAz.alt;
	int altDec = (int) ((objAltAz.alt - altInt) * 1000000);
	if (altDec < 0)
		altDec *= -1;
	int azInt = (int) objAltAz.az;
	int azDec = (int) ((objAltAz.az - azInt) * 1000000);
	
    char* strResp = malloc(sizeof(char) * 100);
    sprintf(strResp, "{\"resp\":\"ok\",\"alt\":%d.%.6d,\"az\":%d.%.6d}", altInt, altDec, azInt, azDec);
    return strResp;
}

struct ln_hrz_posn getAltazFromRadecInDate(struct ln_equ_posn objRaDec, double JD, double latitud, double longitud) {
	// Datos de la localización
	struct ln_lnlat_posn localizacion;
	localizacion.lat = latitud;
	localizacion.lng = longitud;

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

char* getAltazFromRadec(struct ln_equ_posn objRaDec, double latitud, double longitud) {
	// Extraigo la fecha en formato JD
	double JD = ln_get_julian_from_sys();
	
	// Datos de la localización
	struct ln_lnlat_posn localizacion;
	localizacion.lat = latitud;
	localizacion.lng = longitud;

	// Cogemos los datos de la Ascención Recta (1ª coordenada del array)
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
    struct ln_hrz_posn objAltAz;
    ln_get_hrz_from_equ(&object, &observer, JD, &objAltAz);

    // Si es necesario se imprimen por pantalla
    fprintf(stdout, "Alt: %f\n",   objAltAz.alt);
    fprintf(stdout, "Az:  %f\n\n", objAltAz.az);
	
	// Creo el JSON de las ln_dmss y lo devuelvo
    return traduceCoordenadasString(objAltAz);
}

char* getAltazFromName(char* objStr, double latitud, double longitud) {
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
	    struct ln_hrz_posn objAltAz = getAltazFromRadecInDate(equ, JD, latitud, longitud);
	    char* strResp = traduceCoordenadasString(objAltAz);
	    return strResp;
	}else{
		char* strResp = malloc(sizeof(char) * 40);
		sprintf(strResp, "{\"resp\":\"ko\",\"msg\":\"Objeto desconocido\"}");
	    return strResp;
	}
}

char* getSunriseSunset(int dia, int mes, int anio, double latitud, double longitud) {
	// Datos de la localización
	struct ln_lnlat_posn localizacion;
	localizacion.lat = latitud;
	localizacion.lng = longitud;
	
	// Creo la fecha
	struct ln_date fecha;
	fecha.years   = anio;
	fecha.months  = mes;
	fecha.days    = dia;
	fecha.hours   = 0;
	fecha.minutes = 0;
	fecha.seconds = 0;
	
	// Se convierte la fecha a formato interno
	double JD = ln_get_julian_day(&fecha);

	struct ln_rst_time rst;
	// Si no es visible...
	if (ln_get_solar_rst(JD, &localizacion, &rst) != 0) {
		char* strKo = malloc(sizeof(char) * 60);
		sprintf(strKo, "{\"resp\":\"ko\";\"msg\":\"Estas por encima del circulo polar\"}");
		return strKo;
	}else{  // si es visible...
		// Extraemos los datos
		struct ln_zonedate rise, set;
		ln_get_local_date(rst.rise, &rise);
		ln_get_local_date(rst.set, &set);
		
		// Los muestro por pantalla
		fprintf(stdout, "Rise %d/%d/%d %2d:%2d:%f\n",  rise.days, rise.months, rise.years, rise.hours, rise.minutes, rise.seconds);
		fprintf(stdout, "Set  %d/%d/%d %2d:%2d:%f\n\n", set.days,  set.months,  set.years,  set.hours,  set.minutes,  set.seconds);

		// Lo convierto a un número de segundos
		int amanecer = rise.hours*3600 + rise.minutes*60 + ((int) rise.seconds);
		int atardecer = set.hours*3600 +  set.minutes*60 + ((int)  set.seconds);

		// Creo la respuesta y la envío
	    char* strResp = (char *) malloc(sizeof(char) * 60);
		sprintf(strResp, "{\"resp\":\"ok\",\"amanecer\":%d,\"atardecer\":%d,\"amanecer-horas\":%d,\"amanecer-minutos\":%d,\"amanecer-segundos\":%f,\"atardecer-horas\":%d,\"atardecer-minutos\":%d,\"atardecer-segundos\":%f}", amanecer, atardecer, rise.hours, rise.minutes, rise.seconds, set.hours, set.minutes, set.seconds);

		return strResp;
	}
}

int main(int argc, char *argv[]) {
	// La localización que usaré en todos los ejemplos
	double latitud  = 36.7123456;
	double longitud = -4.4123456;
	// La muestro
	fprintf(stdout, "Latitud %f   Longitud %f\n\n", latitud, longitud);
	
	// Muestro la fecha que usaré
	fprintf(stdout, "Fecha a pasar %d/%d/%d %2d:%2d:%f\n", 12, 7, 2017, 0, 0, 0);
	// Busco el amanecer y atardecer
	char* strResp = getSunriseSunset(12, 7, 2017, latitud, longitud);

	// Y muestro el resultado
	fprintf(stdout, "Respuesta: %s\n\n", strResp);
	
	// Creo el RaDec
	struct ln_equ_posn objRaDec;
	objRaDec.ra  =  25.5;
	objRaDec.dec = 120.0;
	// Lo muestro
	fprintf(stdout, "Ra:  %f\n",   objRaDec.ra);
	fprintf(stdout, "Dec: %f\n\n", objRaDec.dec);
	// Lo convierto a AltAz
	strResp = getAltazFromRadec(objRaDec, latitud, longitud);
	// Y muestro el resultado
	fprintf(stdout, "Respuesta: %s\n\n", strResp);
	
	// Creo el string con el objeto
	char* objeto = malloc(sizeof(char) * 25);
	// Lo muestro
	sprintf(objeto, "luna");
	// Lo convierto a AltAz
	strResp = getAltazFromName(objeto, latitud, longitud);
	// Y muestro el resultado
	fprintf(stdout, "Respuesta: %s\n\n", strResp);
	
	return 0;
}












