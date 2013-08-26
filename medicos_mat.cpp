/*
  Name: medicos_mat
  Copyright: -
  Author: Almar Suárez
  Date: 19/02/08 13:35
  Description: sistema matemático, tipos básicos (vértices, vectores, etc)
  Última revisión: 19/02/08 13:35
  Comentarios: [19/02/08 13:35] inicio
               [19/02/08 17:49] fin revisiones
			   [26/01/09 20:04] incluído en ir-alz-optitrack
*/

#include "stdafx.h"

#include <stdio.h>
#include "medicos_mat.h"

bool enRango(float valor, float min, float max){
    if((valor >= min) && (valor <= max)) return true;
    return false;
}



void n_err_msg(char *box_name, char *fmt,...){ 
     
  va_list arg_ptr;		      	  // puntero a la lista de argumentos	        
  char    str[500];			      // buffer para construir la cadena
  int     cnt;			          // contador para almacenar el resultado de sprintf

  va_start(arg_ptr, fmt);		  // inicializa va_ functions	     
  vsprintf(str, fmt, arg_ptr);
  va_end(arg_ptr);			          // cierra las funciones va
  
  //sprintf(str, 
  MessageBoxA(NULL, str, box_name, MB_OK); 
}

/*
void n_err_msg(char *box_name, char *fmt,...){ 
     
  va_list arg_ptr;		      	  // puntero a la lista de argumentos	        
  char    str[500];			      // buffer para construir la cadena
  int     cnt;			          // contador para almacenar el resultado de sprintf

  va_start(arg_ptr, fmt);		  // inicializa va_ functions	     
  vsprintf(str, fmt, arg_ptr);
  va_end(arg_ptr);			          // cierra las funciones va
  
  //sprintf(str, 
  MessageBoxA(NULL, str, box_name, MB_OK); 
}*/


////////////////////////////////////////////////////////////////////////////////////////////////////
 
tVector4 mult44_14_2(matrix44_t p_matrix2, tVector4 vec){
    tVector4 res;
    int j,k;
    float l_sum;
    float p_matrix1[4], rress[4];

    p_matrix1[0] = vec.x;
    p_matrix1[1] = vec.y;
    p_matrix1[2] = vec.z;
    p_matrix1[3] = vec.w;

    for(j=0;j<4;j++){
        l_sum = 0;
        for(k=0;k<4;k++)
            l_sum += p_matrix1[k] * p_matrix2.fm[k][j];
//        p_matrix_res[j] = l_sum;
        rress[j] = l_sum;
    }
    
    res = tVector4(rress[0], rress[1], rress[2], rress[3]);
    return res;
}

tVector4 mult44_14(matrix44_t matriz, tVector4 vec){
/*
   tVector4 res;
   int arr_res[4];
   int i;
   float sum;

   for(i=0; i<4; i++){
       sum = 0;
       sum += vec.x * matriz.fm[0][i];
       sum += vec.y * matriz.fm[1][i];
       sum += vec.z * matriz.fm[2][i];
       sum += vec.w * matriz.fm[3][i];
       arr_res[i] = sum;
   }   
   
   res = tVector4(arr_res[0], arr_res[1], arr_res[2], arr_res[3]);
   return res; 
   */
   
   tVector4 res = mult44_14_2(matriz, vec);
   return res;
}


/*
void MatrMul_1x4_4x4 (matrix_1x4_type p_matrix1, matrix_4x4_type p_matrix2, matrix_1x4_type p_matrix_res)
{
    int j,k;
    float l_sum;

    for (j=0;j<4;j++)
    {
        l_sum=0;
        for(k=0;k<4;k++)
            l_sum+=p_matrix1[k]*p_matrix2[k][j];
        p_matrix_res[j]=l_sum;
    }
}*/
 

float f_min(float a, float b){ return (a < b ? a : b); }
float f_max(float a, float b){ return (a > b ? a : b); }



////////////////////////////////////////////////////////////////////////////////////////////////////



tVector2* mirrorVector2(tVector2 *v, int n){
    tVector2 f, temp;
    tVector2 *ret;
    
    for(int i=0;i<n;i++){
       temp     = tVector2(v[i]);
       v[i]     = tVector2(v[n-1]);
       v[n - 1] = temp;
    }
    
    ret = new tVector2[n];
    for(int j=0;j<n;j++) ret[j] = tVector2(v[j]);
    
    return ret;
}

tVector3* mirrorVector2(tVector3 *v, int n){
    tVector3 f, temp;
    tVector3 *ret;
    
    for(int i=0;i<n;i++){
       temp     = tVector3(v[i]);
       v[i]     = tVector3(v[n-1]);
       v[n - 1] = temp;
    }
    
    ret = new tVector3[n];
    for(int j=0;j<n;j++) ret[j] = tVector3(v[j]);
    
    return ret;
}

tVector4* mirrorVector2(tVector4 *v, int n){
    tVector4 f, temp;
    tVector4 *ret;
    
    for(int i=0;i<n;i++){
       temp     = tVector4(v[i]);
       v[i]     = tVector4(v[n-1]);
       v[n - 1] = temp;
    }
    
    ret = new tVector4[n];
    for(int j=0;j<n;j++) ret[j] = tVector4(v[j]);
    
    return ret;
}


