#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <libnova/transform.h>
#include <libnova/julian_day.h>
#include <libnova/ln_types.h>

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

char* getAltazFromRadec(struct ln_equ_posn objRaDec) {
	// Extraigo la fecha en formato JD
	double JD = ln_get_julian_from_sys();
	
	// Datos de la localización
	struct ln_lnlat_posn localizacion;
	localizacion.lat = 36.7123456;
	localizacion.lng = -4.4123456;

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

int main(int argc, char *argv[]) {
	// Creo el RaDec
	struct ln_equ_posn objRaDec;
	objRaDec.ra  =  25.5;
	objRaDec.dec = 120.0;
	// Lo muestro
	fprintf(stdout, "Ra:  %f\n",   objRaDec.ra);
	fprintf(stdout, "Dec: %f\n\n", objRaDec.dec);
	// Lo convierto a AltAz
	char* strResp = getAltazFromRadec(objRaDec);
	// Y muestro el resultado
	fprintf(stdout, "Respuesta: %s\n\n", strResp);
	return 0;
}



