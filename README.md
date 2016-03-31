# Uso de libnova 0.14

### Castellano: ###

Ejemplos de códigos para utilizar con la librería libnova 0.14.1 en linux (raspbian)

Se incluye:
 - Directorio "libnova-0.14.1", que consiste en una pequeña modificación de la versión 0.14.0 que permite un uso fácil y rápido.
 - Ejemplos de uso de las opciones más importantes de esta libería, como:
    - Hora de amanecer y atardecer del sol a partir de una posición geográfica y una fecha.
    - Posición de la luna a partir de una posición geográfica y una fecha.
    - Posición de los planetas mayores (excepto la Tierra) a partir de una posición geográfica y una fecha.
    - Posición de Plutón a partir de una posición geográfica y una fecha.
    - Una conversión de coordenadas RaDec a coordenadas AltAz.

Los cambios añadidos son los siguientes:
 - Muchas variables y constantes globales han sido renombradas para evitar duplicación de nombres en la compilación.
 - Todos los includes han sido comentados en todos los archivos, excepto el "libnova.h".
 - Ahora, es necesario especificar los includes (libnova.h y los *.c) en los archivos fuente.

Estos cambios se han hecho porque he tenido muchos problemas con las 2 instalaciones disponibles (vía apt-get y vía make) y por no encontrar ningún manual en la web oficial.

Se puede ver un ejemplo de instalación, compilación y ejecución en mi blog (bajo Raspbian Jessie), URL: http://lascosasdealfredo.blogspot.com.es/2016/03/libreria-libnova.html


### English: ###

In this repository, you can see a fork of libnova 0.14.0, with a few changes and examples.

The most important changes are:
 - Many global constants and variables are renamed because it make a error compilation while the compilation.
 - All includes are been deleted putting it in a comment.
 - Now, you must to specify the source files (libnova.h and *.c) in the file that will be compiled.

This changes are been included because I had many errors for this code, now I don't have any error.

You can see a example to install, compilation and execution in my blog (under Raspbian Jessie), URL: http://lascosasdealfredo.blogspot.com.es/2016/03/libreria-libnova.html
