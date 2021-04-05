# Practica 2 videojuegos

Autores:
- Néstor Monzón (735418): 8h
- Andrés Otero (757755): 8h


Decisiones de diseño:
Al principio observamos que al acumular las rotaciones del ratón en la matriz view se acumulaba una rotación no deseada en el eje Z de la cámara. Para solucionarlo, mantenemos los dos ángulos de rotación en variables (junto con la matriz de traslación), y transformamos la matriz de view cuando se modifican cualquiera de estos (función updateView()).
Con esto, los vectores del movimiento correspondientes a cada flecha también se rotan de acuerdo a estos ángulos al añadir transformaciones a la matriz de traslación (para que el movimiento sea en los ejes locales de la cámara).
