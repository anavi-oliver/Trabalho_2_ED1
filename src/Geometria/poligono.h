   #include "lista.c"

   typedef void * Poligono;

   Poligono criaPoligono();
   void insertVertice(Poligono p, Ponto v);
   void insertSegmento(Poligono p, Segmento s);
   bool pontoNoPoligono(Poligono p, Ponto pt);
   Lista getVertices(Poligono p);
   Lista getSegmentos(Poligono p);
   BoundingBox getBoundingBox(Poligono p);
   void destroiPoligono(Poligono p);