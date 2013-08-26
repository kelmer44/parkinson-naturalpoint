/*
  Name: medicos_mat
  Copyright: -
  Author: Almar Suárez
  Date: 18/11/07 03:20
  Description: sistema matemático, tipos básicos (vértices, vectores, etc)
  Última revisión: 20/07/08 18:30
  Comentarios: [27/12/07 19:11] inicio de _mat
               [20/07/08 18:30] última versión conocida
			   [26/01/09 20:01] Usada en proyecto médicos
*/

#pragma once

#include "stdafx.h"

#include <math.h>
#include <float.h> // para FLT_MAX
//#include <values.h> // para maxfloat


extern void n_err_msg(char *box_name, char *fmt,...);
//extern void n_err_msg234(char *box_name, char *fmt,...);

#define MIFABS(n) (n<0 ? (n * (-1)) : n)

#define RANDOM_FLOAT          (((float)rand())/RAND_MAX)
#define RAND_MINMAX(min, max) (min + ((max - min) * RANDOM_FLOAT))

extern bool  enRango(float valor, float min, float max);
extern float f_min(float a, float b);
extern float f_max(float a, float b);


//==========================================================
//    vector2_t: vector de 2 dimensiones con operadores para
//               modificarlo.
//    miembros:  x,y                   
//==========================================================
typedef struct tVector2{
   float x, y;

   tVector2() {}	// constructores   
   tVector2(float mi_x, float mi_y){x = mi_x; y = mi_y;}
   tVector2(const tVector2& v): x(v.x), y(v.y) {}
	
   // operadores a cascoporro. sólo falta == y algún otro, pero paso de tanta cosa	
   tVector2 operator+(tVector2 v) {return tVector2(v.x+x,  v.y+y);}
   tVector2 operator-(tVector2 v) {return tVector2(x-v.x,  y-v.y);}
   tVector2 operator*(float num)   {return tVector2(x*num,  y*num);}
   tVector2 operator/(float num)   {return tVector2(x/num,  y/num);}	   					
   tVector2 operator=(tVector2 v) {return tVector2(x=v.x,  y=v.y);}
   tVector2 operator+=(tVector2 v){return tVector2(x+=v.x, y+=v.y);}
   tVector2 operator-=(tVector2 v){return tVector2(x-=v.x, y-=v.y);}
   tVector2 operator*=(float num)  {return tVector2(x*=num, y*=num);}
   tVector2 operator/=(float num)  {return tVector2(x/=num, y/=num);}
   tVector2 operator-() {return tVector2(-x, -y);}

   // tamaño de nuestro vector
   float length() {return sqrtf(x*x + y*y); }   

   // convierte el vector a un vector unidad (modifica sus valores)
   void unitize(){
        float len = this->length();
		if (len == 0) return;
		x /= len; y /= len; 
   }
   
   // devuelve un vector unidad en base a éste vector
   tVector2 unit(){
        float len = this->length();
		if (len == 0) return tVector2(x,y);	
		return tVector2(x/len, y/len);
   }
   
   // distancia desde este vector a otro
   float distancia(tVector2 v){ return (sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y))); }
     
} tVector2; 

//==========================================================
//    vector3_t: vector de 3 dimensiones con operadores para
//               modificarlo.
//    miembros:  x,y,z                   
//==========================================================
typedef struct tVector3{			
   float x, y, z;     

   tVector3() {}	// constructores   
   tVector3(float mi_x, float mi_y, float mi_z){x = mi_x; y = mi_y; z = mi_z;}
   tVector3(tVector2 v, float mi_z) : x(v.x), y(v.y), z(mi_z) {}
   tVector3(const tVector3& v): x(v.x), y(v.y), z(v.z) {}

   // operadores a cascoporro. sólo falta == y algún otro, pero paso de tanta cosa	
   tVector3 operator+(tVector3 v) {return tVector3(v.x+x,  v.y+y,  v.z+z);}   
   tVector3 operator-(tVector3 v) {return tVector3(x-v.x,  y-v.y,  z-v.z);}
   tVector3 operator*(float num)	{return tVector3(x*num,  y*num,  z*num);}
   tVector3 operator/(float num)	{return tVector3(x/num,  y/num,  z/num);}	
   tVector3 operator^(tVector3 v) {return tVector3((y * v.z - z * v.y), (z * v.x - x * v.z), (x * v.y - y * v.x));}
   tVector3 operator=(tVector3 v) {return tVector3(x=v.x,  y=v.y,  z=v.z);}
   tVector3 operator+=(tVector3 v){return tVector3(x+=v.x, y+=v.y, z+=v.z);}
   tVector3 operator-=(tVector3 v){return tVector3(x-=v.x, y-=v.y, z-=v.z);}
   tVector3 operator*=(float num)  {return tVector3(x*=num, y*=num, z*=num);}
   tVector3 operator/=(float num)  {return tVector3(x/=num, y/=num, z/=num);}
   tVector3 operator-() {return tVector3(-x, -y, -z);}  

   // tamaño de nuestro vector
   float length() {return sqrtf(x*x + y*y + z*z); }
   
   // convierte el vector a un vector unidad (modifica sus valores)
   void unitize(){
        float len = this->length();
		if (len == 0) return;
		x /= len; y /= len; z /= len; 		 
   }
      
   // devuelve un vector unidad en base a éste vector
   tVector3 unit(){
        float len = this->length();
		if (len == 0) return tVector3(x,y,z);	
		return tVector3(x/len, y/len, z/len);
   }
   					
   // distancia desde este vector a otro
   float distancia(tVector3 v){ return (sqrt((x-v.x)*(x-v.x) + (y-v.y)*(y-v.y) + (z-v.z)*(z-v.z))); }

   float distancia2D(tVector3 v){ return (sqrt((x-v.x)*(x-v.x) + (y-v.y)*(y-v.y))); }

}tVector3;

//==========================================================
//    tVector4: vector de 4 dimensiones con operadores para
//               modificarlo.
//    miembros:  x,y,z,w          
//==========================================================
typedef struct tVector4{
   float x, y, z, w;
   
   tVector4() {}	// constructores   
   tVector4(float mi_x, float mi_y, float mi_z, float mi_w){x = mi_x; y = mi_y; z = mi_z; w = mi_w;}
   tVector4(tVector2 v1, tVector2 v2) : x(v1.x), y(v1.y), z(v2.x), w(v2.y) {}
   tVector4(tVector3 v, float mi_w) : x(v.x), y(v.y), z(v.z), w(mi_w) {}
   tVector4(const tVector4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
	
   // operadores a cascoporro. sólo falta == y algún otro, pero paso de tanta cosa	
   tVector4 operator+(tVector4 v) {return tVector4(v.x+x,  v.y+y,  v.z+z,  v.w+w);}
   tVector4 operator-(tVector4 v) {return tVector4(x-v.x,  y-v.y,  z-v.z,  w-v.w);}
   tVector4 operator*(float num)	{return tVector4(x*num,  y*num,  z*num,  w*num);}
   tVector4 operator/(float num)	{return tVector4(x/num,  y/num,  z/num,  w/num);}	
   tVector4 operator=(tVector4 v) {return tVector4(x=v.x,  y=v.y,  z=v.z,  w=v.w);}
   tVector4 operator+=(tVector4 v){return tVector4(x+=v.x, y+=v.y, z+=v.z, w+=v.w);}
   tVector4 operator-=(tVector4 v){return tVector4(x-=v.x, y-=v.y, z-=v.z, w-=v.w);}
   tVector4 operator*=(float num)  {return tVector4(x*=num, y*=num, z*=num, w*=num);}
   tVector4 operator/=(float num)  {return tVector4(x/=num, y/=num, z/=num, w/=num);}
   tVector4 operator-() {return tVector4(-x, -y, -z, -w);}

   // tamaño de nuestro vector
   float length() {return sqrtf(x*x + y*y + z*z + w*w); }
   
   // convierte el vector a un vector unidad (modifica sus valores)
   void unitize(){
        float len = this->length();
		if (len == 0) return;
		x /= len; y /= len; z /= len; w /= len; 		 
   }
   
   // devuelve un vector unidad en base a éste vector
   tVector4 unit(){
        float len = this->length();
		if (len == 0) return tVector4(x,y,z,w);	
		return tVector4(x/len, y/len, z/len, w/len);
   }
     
   // distancia desde este vector a otro
   float distancia(tVector4 v){ 
         return (sqrt((x-v.x)*(x-v.x) + (y-v.y)*(y-v.y) + (z-v.z)*(z-v.z) + (w-v.w)*(w-v.w))); 
   } 
  
} tVector4;

#define NULL_VEC2 tVector2(.0f, .0f)
#define NULL_VEC3 tVector3(.0f, .0f, .0f)
#define NULL_VEC4 tVector4(.0f, .0f, .0f, .0f)

//==========================================================
//    matrix44_t: matríz de 4x4 (floats)
//==========================================================
typedef struct matrix44_t{
 public:
   float fm[4][4];

   matrix44_t() {}	
      
   float getElem(int i, int j){ return (fm[i][j]);}   
   void  setElem(float f, int i, int j) { fm[i][j] = f; }
      
   void init(float v){
      int i,j;
      
      for(i=0;i<4;i++)
          for(j=0;j<4;j++)
              fm[i][j] = v;
   }          
      
   void identidad(){
      int i,j;
      
      for(i=0;i<4;i++){
          for(j=0;j<4;j++){
              if(i == j) fm[i][j] = 1;
              else       fm[i][j] = 0;
          }
      }
   }
    
   void copia(matrix44_t m){
      int i,j;
      
      for(i=0;i<4;i++)
          for(j=0;j<4;j++)
              fm[i][j] = m.getElem(i,j);                
   }
   
   void copia33_traspuesta(matrix44_t copiame){
      int i,j;
      
      for(i=0;i<3;i++)
          for(j=0;j<3;j++)
              fm[i][j] = copiame.getElem(i,j);                
   }  
   
   void multiplica(matrix44_t m){
      int i,j,k;
      float suma;
      
      for(i=0;i<4;i++){
         for(j=0;j<4;j++){
            suma = 0;
            for(k=0;k<4;k++)
                suma += fm[i][k] * m.getElem(k,j);
            fm[i][j] = suma;
        }
      }
   }
         
   void mult(matrix44_t p_matrix1, matrix44_t p_matrix2, matrix44_t *p_matrix_res){
      
      int i,j,k;
      float l_sum;

      for(i=0;i<4;i++){
         for (j=0;j<4;j++){
            l_sum = 0;
            for(k=0;k<4;k++)
                l_sum += p_matrix1.fm[i][k] * p_matrix2.fm[k][j];
            p_matrix_res->fm[i][j] = l_sum;
         }
      }
   }
      
} matrix44_t;  

extern tVector4 mult44_14(matrix44_t matriz, tVector4 vec); 
extern tVector4 mult44_14_2(matrix44_t p_matrix2, tVector4 vec); 

//===============================================================================================

#include <windows.h>

////////////////////////////////// GRUPO DE VÉRTICES 3D - nvs, vs<[]> -
typedef struct vs_group3d_t {
     
   // constructores 
   vs_group3d_t(){}
   vs_group3d_t(int n) : nvs(n) {}   
   vs_group3d_t(const vs_group3d_t &c) : nvs(c.nvs) { 
                vertices = new tVector3[c.nvs];
                for(int i=0;i<c.nvs;i++) vertices[i] = c.vertices[i];
   }

   // atributos
   int        nvs;
   tVector3*  vertices;

   // métodos
   void init(){         
	    vertices = new tVector3[nvs];
        //if(vertices == NULL)        
//        if(!(vertices = new tVector2[nvs])) 
        //   MessageBox(NULL, "IMPOSIBLE RESERVAR MEMORIA", "error fatal", MB_OK);   
   }   
   float getVertexX(int n){ return (float)(vertices[n].x); }
   float getVertexY(int n){ return (float)(vertices[n].y); }
   float getVertexZ(int n){ return (float)(vertices[n].z); }
   void  addVertex(tVector3 v, int n) { vertices[n] = tVector3(v); }   
   void  addVertex(float x, float y, float z, int n) {  if(n<nvs){ vertices[n].x = x; vertices[n].y = y; vertices[n].z = z; }  }   
   void  uninit() { nvs = 0; delete [] vertices; } 
      
} vs_group3d_t;



typedef struct punto2D {
     
   double x;
   double y;
   double z;

   long		Rank;
   float	Score;
   float	Height;
   float	Mass;

         
} punto2D;

////////////////////////////////// GRUPO DE VÉRTICES 2D - nvs, vs<[]> -
typedef struct vs_group2d_t {
     
   // constructores 
   vs_group2d_t(){}
   vs_group2d_t(int n) : nvs(n) {}      
   vs_group2d_t(const vs_group2d_t &c) : nvs(c.nvs) { 
                vertices = new tVector2[c.nvs];
                for(int i=0;i<c.nvs;i++) vertices[i] = c.vertices[i];
   }

   // atributos
   int        nvs;
   tVector2*  vertices;

   // métodos
   void init(){ 
        vertices = new tVector2[nvs];
  //      if(vertices == NULL)        
//        if(!(vertices = new tVector2[nvs])) 
//           MessageBox(NULL, "IMPOSIBLE RESERVAR MEMORIA", "error fatal", MB_OK); 
   }   
   void addVertex(tVector2 v, int n) { vertices[n] = tVector2(v); }   
   void uninit() { nvs = 0; delete [] vertices; } 
      
} vs_group2d_t ;


/*
float areaTriangulo(tVector2 A1, tVector2 A2, tVector2 A3){
     return (A1.x - A3.x)*(A2.y - A3.y)-(A1.y - A3.y)*(A2.x - A3.x);
}

bool isInTriangulo(tVector2 A1, tVector2 A2, tVector2 A3, tVector2 P){  
     if(areaTriangulo(A1,A2,A3)>=0)
        return ((areaTriangulo(A1, A2, P) >= 0) && (areaTriangulo(A2, A3, P) >= 0) && (areaTriangulo(A3, A1, P) >= 0));
     else   
        return ((areaTriangulo(A1, A2, P) <= 0) && (areaTriangulo(A2, A3, P) <= 0) && (areaTriangulo(A3, A1, P) <= 0));
}
*/


typedef struct vline_t{
   
   // consctructores
   vline_t(){}
   vline_t(tVector2 v1, tVector2 v2){ p1 = tVector2(v1); p2 = tVector2(v2); }

   // los dos puntos que definen la recta
   tVector2 p1, p2;
   
   // devuelve la longitud de la recta
   float longitud(){  p1.distancia(p2);  }
  
   // devuelve el punto de intersección entre dos líneas
   tVector2 intersec(const vline_t& otra){ // Line2D r1, Line2D r2 ) {
        
        float m1 = pendiente();        //r1.getX1(),r1.getY1(),r1.getX2(),r1.getY2()) ;
        float b1 = _b(p1.x, p1.y, m1); //getB(r1.getX1(),r1.getY1(),m1) ;   
        float m2 = pendiente(otra.p1.x, otra.p1.y, otra.p2.x, otra.p2.y);  //r2.getX1(),r2.getY1(),r2.getX2(),r2.getY2()) ;
        float b2 = _b(otra.p1.x, otra.p1.y, m2);  //getB(r2.getX1(),r2.getY1(),m2) ;          
        tVector2 res;
                
        // Las rectas son paralelas, no tiene interseccion
        if((m1 - m2) == 0) return NULL_VEC2;
                        
        // Si la primera recta es vertical
        else if(m1 >= FLT_MAX){ //std::numeric_limits<float>::max()){            
            // y = m2 * x1 + b2
            float y = m2 * p1.x + b2 ;
            return (res = tVector2(p1.x, y)); 
        }
        
        // Si la segunda recta es vertical
        else if(m2 >= FLT_MAX){ //std::numeric_limits<float>::max()){            
            /*double y = m2 * r1.getX1() + b2 ;
            //System.out.println(new Point ((int)Math.round(r1.getX1()),(int)Math.round(y))) ;
            return new Point ((int)Math.round(r1.getX1()),(int)Math.round(y)) ;
            */ 
            // y = m2 * x1 + b2
            return (res = tVector2(p1.x, otra.p1.y));
        }        
                
        double x = (b2 - b1) / (m1 - m2);
        double y = (b2*m1 - b1*m2) / (m1 - m2);
  
//        if(x < 0 || y < 0) return NULL_VEC2;
                
        return (res = tVector2(x, y));                 
    }
       
    float pendiente(){
        return (pendiente(p1.x, p1.y, p2.x, p2.y));
    }
    
    float pendiente(float x1, float y1, float x2, float y2){
        double dy = y2 - y1 ; 
        double dx = x2 - x1 ;     
        double res;
        
        if(dx != 0 && dy != 0) res = dy/dx ;
        else res = 0;
               
        return res;
     }
     
     // y = m*x + b
     float _b (float x, float y, float m) {        
        float res = y- (m*x) ;
        return res;
     }    

     // para calcular si la recta contiene el punto (muy aproximado y cutre)
     bool contiene(int x, int y) {        
        /*double m = getPendiente(r.getX1(),r.getY1(),r.getX2(),r.getY2()) ;
        double b = getB(r.getX1(),r.getY1(),m) ; 
        if ( y == m * x + b ) {
            return true ;
        }
        return false ;
        */     
       if (x >= f_min(p1.x, p2.x) && x <= f_max(p1.x, p2.x) &&
           y >= f_min(p1.y, p2.y) && y <= f_max(p1.y, p2.y)) 
           return true;       
       return false;     
    }

} vline_t;

