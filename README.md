# Uso de libnova 0.14.0

### Castellano: ###

Ejemplos de códigos para utilizar con la librería libnova 0.14.0 en linux (Raspbian Jessie)

Entre ellos busco responder a los siguientes apartados:
    - Hora de amanecer y atardecer del sol a partir de una posición geográfica y una fecha.
    - Posición de la luna a partir de una posición geográfica y una fecha.
    - Posición de los planetas mayores (excepto la Tierra) a partir de una posición geográfica y una fecha.
    - Posición de Plutón a partir de una posición geográfica y una fecha.
    - Una conversión de coordenadas RaDec a coordenadas AltAz.
	- Posiciones de cometas y planetas pequeños utilizando la base de datos de MPC (por ahora no).

Para realizar la compilación de cada archivo es necesario ejecutar este comando:
	gcc miLibnova.c /opt/libnova-0.14.0/src/*.o -lm -o miLibnova

Se puede ver un ejemplo de instalación, compilación y ejecución en mi blog (bajo Raspbian Jessie), URL: http://lascosasdealfredo.blogspot.com.es/2016/03/libreria-libnova.html


### English: ###

In this repository, you can see examples of libnova 0.14.0 on linux (Raspbian Jessie).

With this examples you can see this hits:
	- Sunrise and Sunset time in a geographical position and date.
	- Moon's position in a geographical position and date.
	- Mayors planets' position (except Earth) in a geographical position and date.
	- Pluto's position in a geographical position and date.
	- Convert RaDec coordinates to AltAz coordinates.
	- Comet's position and minors planets' position using MPC data base (not now).

You can do a compilation of earch file with this command:
	gcc miLibnova.c /opt/libnova-0.14.0/src/*.o -lm -o miLibnova

You can see a example to install, compilation and execution in my blog (under Raspbian Jessie), URL: http://lascosasdealfredo.blogspot.com.es/2016/03/libreria-libnova.html








