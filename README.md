# mjpeg-input-control
Plugin para el servidor de video streaming MJPEG Streamer. Cuya finalidad es lograr el control de un hardware conectado a un ordenador de placa reducida, como raspberry pi, a través de sus puertos GPIO. Con ello se consigue simplificar el desarrollo de una página web de control de dicho dispositivo de manera que se logra su control a partir de este servidor de video streaming.

Modo de prueba:
  
  $ make fix-dependency
  
  $ make
  
  $ ./test.sh

  Abrir navegador en http://localhost:8080, observar en la web si al pulsar el botón la bombilla parpadea, lo que significa que a través de la red se envía un mensaje, repetitivo y AJAX, hacia el servidor “mjpeg-streamer” el plugin lo reconoce y responde una respuesta a la web. También es posible ver un mensaje de respuesta en la consola.

* Si se prueba en un ordenador incompatible con WiringPi, sin GPIO por ejemplo, para probarlo es posible editar el fichero "control.c" y borrar o comentar la línea "#define WIRINGPI" con lo que se elimina las lineas que llaman a esa libreria y podrá probarse.
