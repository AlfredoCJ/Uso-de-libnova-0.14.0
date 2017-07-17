#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <libnova/rise_set.h>
#include <libnova/transform.h>
#include <libnova/julian_day.h>
#include <libnova/solar.h>

char* getSunriseSunset(struct ln_date fecha) {
	// Datos de la localización
	struct ln_lnlat_posn localizacion;
	localizacion.lat = 36.7123456;
	localizacion.lng = -4.4123456;
	
	// Se convierte la fecha a formato interno
	double JD = ln_get_julian_day(&fecha);
	fprintf(stdout, "JD %f\n", JD);
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
		sprintf(strResp, "{\"resp\":\"ok\",\"amanecer\":%d,\"atardecer\":%d,\"amanecer-horas\":%d,\"amanecer-minutos\":%d,\"amanecer-segundos\":%d,\"atardecer-horas\":%d,\"atardecer-minutos\":%d,\"atardecer-segundos\":%d}", amanecer, atardecer, rise.hours, rise.minutes, rise.seconds, set.hours, set.minutes, set.seconds);
		return strResp;
	}
}

int main(int argc, char *argv[]) {
	// Creo la fecha
	struct ln_date fec;
	fec.years   = 2017;
	fec.months  = 7;
	fec.days    = 12;
	fec.hours   = 0;
	fec.minutes = 0;
	fec.seconds = 0;
	// La muestro
	fprintf(stdout, "Fecha pasada %d/%d/%d %2d:%2d:%f\n",
		fec.days, fec.months, fec.years, fec.hours, fec.minutes, fec.seconds);
	// Busco el amanecer y atardecer
	char* strResp = getSunriseSunset(fec);
	// Y muestro el resultado
	fprintf(stdout, "Respuesta: %s\n\n", strResp);
	return 0;
}



