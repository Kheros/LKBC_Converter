# LKBC_Converter
Conversor de modelos M2 de World of Warcraft M2 de la versión 08 (_Wrath of the Lich King_) a la 07-04 (_The Burning Crusade_).
Escrito en ANSI C, documentado con Doxygen, testeado con CuTest, hecho con amor y lágrimas.

##Qué queda por implementar :
* Efectos "cinta" (rastros como el dee Al'ar)
* Partículas (Spells, glowing effects..)
	
Todavía es un Work In Progress.
  
  
##Preguntas frecuentes :
###¿Se pueden convertir los modelos nuevos de personajes de Warlods of Draenor?
Si desde la versión 0.4. Pero lee esto cuidadosamente : 
* Hay geosets soportados por LK pero no por BC. Como resultado, si no los eliminas antes en Blender, siempre se verán. Ejemplos: "Catch-like" belt, los ojos brillantes de los DK.
* Recuerda que no tienes que diezmar las mallas de los modelos de los personajes si quieres convertir hacia BC por que hay muchos sets sin usar. Cortes de pelo (los que fueron añadidos en la peluquería de Wotlk) por ejemplo.
* Tendrás que redimensionar las texturas al tamaño estándar de los modelos de classic. Es una gran pérdida de calidad pero no podemos hacer nada. Las texturas de los personajes están muy codificadas dentro del ejecutable del cliente. 
* No te olvides de editar las DBC. No puedes usar las de LK.


###¡Mi M2 no funciona!
En la mayoría de los casos será por estar usando modelos con características no soportadas, como la iluminación o las partículas. 
De todas formas, pueden ocurrir más errores. Reporta tu problema en el repositorio original en inglés o en los foros de EspalZone ;) !
No te olvides mencionar el M2 con el que estás teniendo errores y la naturaleza del problema.
  
  
##Créditos :
Original idea and structures by Stan84.
Thanks to Stan84, PhilipTNG, Schlumpf and all the people who contributed to M2&M2/WotLK pages on the WoWDev wiki.
Thanks to OneSadCookie for its https://github.com/OneSadCookie/fcaseopen

##Ayuda al desarrollador :
* Si conoces C puro y tienes ideas para mejorar el código;
