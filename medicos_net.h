/*
  Name: k_net
  Copyright: almar zeeraus
  Author: almar
  Date: 06/08/09 03:46
  Description: funcionalidades varias para enviar y recibir información (conexión vía sockets)
  Última revisión: 09/08/09 02:24
  Comentarios: [06/08/09 04:42] Empiezo a hacer algunas cosas
               [08/08/09 02:24] Arreglados net_emisor_t y net_receptor_t
  
*/


#pragma once

#include <winsock2.h>
#include <WS2TCPIP.h>
//#include "stdafx.h"
#include "medicos_mat.h"

#define _msg_null          0  //"..."
#define _msg_error         1  // "<!> --- Error en socket ---"
#define _msg_err_send      2  // "<!> Fallo en send"
#define _msg_escuchando    3  // "< > Escribiendo en el puerto definido"
#define _msg_enviando      4  // "<+> Enviando bytes "
#define _msg_datagrama     5  // "<!> No se puede enviar el datagrama "

#define _msg_err_rcv      12  // "<!> Fallo en recv from"
//#define _msg_escuchando  13  // "< > Escuchando en el puerto definido"
#define _msg_recibiendo   14  // "<+> Recibiendo bytes "
#define _msg_recibiendo2  15  // "<-> Recibiendo bytes (no hay más por ahora) "

#define LOCAL_HOST      "127.0.0.1"
#define PORT_DEFAULT          2020
#define FLOAT_SIZE               4

/*
void n_err_msg234(char *box_name, char *fmt,...){ 
     
  va_list arg_ptr;		      	  // puntero a la lista de argumentos	        
  char    str[500];			      // buffer para construir la cadena
  int     cnt;			          // contador para almacenar el resultado de sprintf

  va_start(arg_ptr, fmt);		  // inicializa va_ functions	     
  vsprintf(str, fmt, arg_ptr);
  va_end(arg_ptr);			          // cierra las funciones va
  
  //sprintf(str, 
  MessageBoxA(NULL, str, box_name, MB_OK); 
}*/ 

//==========================================================
//    net_emisor_t: tipo para crear conexión vía sockets 
//                  y enviar datos
//==========================================================
typedef struct net_emisor_t {
    SOCKET  m_socket;
    struct  sockaddr_in name;
    char   *ip;     // ip a conectarse
    int     puerto; // puerto
    float  *data;   // datos a pasarle
    float  *mtemp;
    int     msg_actual, n_data, n_actual; // mensaje de estado, número de datos, dato actual agregado

    net_emisor_t() : ip(LOCAL_HOST), n_data(0), puerto(PORT_DEFAULT), msg_actual(_msg_null), n_actual(0) {}
    net_emisor_t(char *nip, int npuerto, int nd){  // IP, PUERTO, NÚMERO DE DATOS   
                 ip = nip; puerto = npuerto; msg_actual = _msg_null; n_data = nd; n_actual = 0; }

    void init_emisor(){
        int i; 
         
        data  = (float *)malloc(sizeof(float *) * n_data);
        mtemp = (float *)malloc(sizeof(float *) * n_data);
        for(i=0;i<n_data;i++){ data[i] = 0.0f; mtemp[i] = 0.0f; }
        
	    m_socket = socket(AF_INET, SOCK_DGRAM, 0);
	    WORD wVersionRequerida;
	    WSADATA wsaDatos;
	    wVersionRequerida = MAKEWORD(1,1);
	    int error = WSAStartup(wVersionRequerida, &wsaDatos);
	    if(error !=0){
	       n_err_msg("Error [net_emisor_t]", "No se pudo inicializar la API Winsock.");
	       WSACleanup();
	       msg_actual = _msg_error;
	       return;
	    }
	    m_socket = socket(AF_INET, SOCK_DGRAM, 0);
	    if(m_socket == INVALID_SOCKET){
	       n_err_msg("Error [net_emisor_t]", "No se ha podido crear el socket.");
	       msg_actual = _msg_error;
	       return;
	    }
	    u_long nobloqueante  = 1;
	    ioctlsocket(m_socket, FIONBIO, &nobloqueante);
	    name.sin_family      = AF_INET;
	    name.sin_addr.s_addr = inet_addr(ip);
	    name.sin_port        = htons(puerto);  
        msg_actual = _msg_escuchando; 
    }

    
    void add_float(float f){    if(n_actual<=n_data){  data[n_actual]=f;  n_actual++;} 
                                else n_err_msg("Error [net_emisor]", "Error en add_float: imposible agregar nuevo elemento."); }
    void add_vec2f(tVector2 v){if(n_actual<=n_data-2){data[n_actual]=v.x;data[n_actual+1]=v.y;n_actual += 2;}
                                else n_err_msg("Error [net_emisor]", "Error en add_vec2f: imposible agregar nuevos elementos.");}
    void add_vec3f(tVector3 v){if(n_actual<=n_data-3){data[n_actual]=v.x;data[n_actual+1]=v.y;data[n_actual+2]=v.z;n_actual += 3;}
                                else n_err_msg("Error [net_emisor]", "Error en add_vec3f: imposible agregar nuevos elementos.");}
    void add_vec4f(tVector4 v){if(n_actual<=n_data-4){data[n_actual]=v.x;data[n_actual+1]=v.y;data[n_actual+2]=v.z;data[n_actual+3]=v.w;n_actual+=4;}
                                else n_err_msg("Error [net_emisor]", "Error en add_vec4f: imposible agregar nuevos elementos.");}
    
    void send_data(){
	    int i;
	    bool envia = false;
	
	    for(i=0;i<n_data;i++) if(fabs(mtemp[i] - data[i]) > 0){ envia = true; break; }
        
        if(envia)
                     // HAY QUE TENER MUUUCHO CUIDADO CON ESTO, PORQUE ASUMO QUE UN FLOAT VALE 4 BYTES
           if(sendto(m_socket, (char *)data, n_data * FLOAT_SIZE, 0, (struct sockaddr *)&name, sizeof name) == SOCKET_ERROR)//{	
              msg_actual = _msg_datagrama;	
//              n_err_msg("err", "ERROR AL MANDAR EL PUTO SOCKET");
//           }
	    else{
           for(i=0;i<n_data;i++) mtemp[i] = data[i];
           msg_actual = _msg_enviando;
        } 
//        n_err_msg("err", "SEND\nenvia: %d\ndata: %f %f %f %f\ntemp: %f %f %f %f", envia, data[0],data[1],data[2],data[3],mtemp[0],mtemp[1],mtemp[2],mtemp[3]);        
    }    
    
    // hay que llamarlo justo al final del todo
    void flush(){
        n_actual = 0;
    }
    
    void get_msg_state(char *ret){
        switch(msg_actual){
           case _msg_null:        strcpy(ret, "..."); break;
           case _msg_error:       strcpy(ret, "<!> --- Error en socket ---"); break;
           case _msg_err_send:    strcpy(ret, "<!> Fallo en send"); break;
           case _msg_escuchando:  strcpy(ret, "< > Escribiendo en el puerto definido"); break;
           case _msg_enviando:    strcpy(ret, "<+> Enviando bytes "); break;
           case _msg_datagrama:   strcpy(ret, "<!> No se puede enviar el datagrama "); break;        
           default:               strcpy(ret, "ERROR: estado no reconocido.");
        }
    }        
    
} net_emisor_t;


//==========================================================
//    net_receptor_t: tipo para crear conexión vía sockets 
//                    y recibir datos
//==========================================================
typedef struct net_receptor_t {              

    SOCKET  m_socket;
    char   *ip;
    int     puerto;
    float  *data, *buffer;	
    bool    flag;
    int     msg_actual, n_data, n_actual;

    net_receptor_t() : ip(LOCAL_HOST), n_data(0), puerto(PORT_DEFAULT), msg_actual(_msg_null), n_actual(0), flag(true) {}
    net_receptor_t(char *nip, int npuerto, int nd){  // IP, PUERTO, NÚMERO DE DATOS   
                   ip = nip; puerto = npuerto; msg_actual = _msg_null; n_data = nd; n_actual = 0; flag = true; }

    void init_receptor(){
        int i;
         
        data   = (float *)malloc(sizeof(float *) * (n_data));
        buffer = (float *)malloc(sizeof(float *) * (n_data + 1)); // +1
        for(i=0;i<n_data;i++){ data[i] = 0.0f; buffer[i] = 0.0f; }
        
    	WORD wVersionRequerida;
	    WSADATA wsaDatos;
	    wVersionRequerida = MAKEWORD(1,1);
	    int error = WSAStartup(wVersionRequerida, &wsaDatos);	
        if(error != 0){
	       n_err_msg("Error", "No se pudo inicializar la API Winsock\n");
	       WSACleanup();		
	       msg_actual = _msg_error;
	    }
	    m_socket = socket(AF_INET, SOCK_DGRAM, 0);	
	    if(m_socket == INVALID_SOCKET){
	       n_err_msg("Error", "No se ha podido crear el socket\n");		
	       msg_actual = _msg_error;
	    }
	    sockaddr_in name;
	    name.sin_family      = AF_INET;
	    name.sin_addr.s_addr = INADDR_ANY;
	    name.sin_port        = htons(puerto);

	    if(bind(m_socket, (LPSOCKADDR)&name, sizeof name) == SOCKET_ERROR){
	       n_err_msg("Error", "No se pudo hacer el bind\n");
	       closesocket(m_socket);
	       msg_actual = _msg_error;
	    }
	    ip_mreq mreq;
	    mreq.imr_multiaddr.s_addr = inet_addr(ip);
	    mreq.imr_interface.s_addr = INADDR_ANY;
	    if(setsockopt(m_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq))<0){
 	       n_err_msg("Error", "Error al añadir al grupo\n");
 	       closesocket(m_socket);
 	       msg_actual = _msg_error;
	    }
	    u_long nobloqueante = 1;
	    ioctlsocket(m_socket, FIONBIO, &nobloqueante);    
	    msg_actual = _msg_escuchando ; // recibiendo en puerto blablabla...
    }
    
    // ATENCIÓN: CUIDADO, los buffers son de tamaño FIJO, quizás sean demasiado chicos
    void recv_data(){
    	struct sockaddr_in Remitente;
	    int  tamRemitente = sizeof(Remitente);
	    int  bytesRecibidos = 0, bytesRecibidos_2 = 0;
	    char buf[1024], buf_2[1024];

//int i=0;
 	    do{
		   bytesRecibidos_2 = recvfrom(m_socket, (char *)buf_2, 1024, 0, (LPSOCKADDR)&Remitente, &tamRemitente);
//           bytesRecibidos_2 = recvfrom(m_socket, (char *)buf_2, (n_data*1024), 0, (LPSOCKADDR)&Remitente, &tamRemitente);
		   if(bytesRecibidos_2 == SOCKET_ERROR){
		      int error = WSAGetLastError();
		      if(error != WSAEWOULDBLOCK){
			     printf("Fallo en recv from\n");
//			     n_err_msg("err receptor", "fallo recv from");
			     closesocket(m_socket);
			     msg_actual = _msg_err_rcv;
		      }
		   } 
		   else{
		      bytesRecibidos = bytesRecibidos_2;		      
		      memcpy(buf, buf_2, sizeof(char)*1024);
//              memcpy(buf, buf_2, sizeof(char) * (n_data*1024)); //128); // antes n_data * 2
//              n_err_msg("err", "bytes (buf_2): %c %c %c %c %c %c %c %c", buf_2[0],buf_2[1],buf_2[2],buf_2[3],buf_2[4],buf_2[5],buf_2[6],buf_2[7]);
		      msg_actual = _msg_recibiendo;
		   }   
//		   n_err_msg("err", "bytes (buf_2): %c %c %c %c %c %c %c %c", buf_2[0],buf_2[1],buf_2[2],buf_2[3],buf_2[4],buf_2[5],buf_2[6],buf_2[7]);
//           n_err_msg("err", "EN WHILE [%d]\nrecibidos %d : %d", i, bytesRecibidos, bytesRecibidos_2);
//           i++;
	    } 
        while(bytesRecibidos_2 != SOCKET_ERROR);		  
	    if(bytesRecibidos > 0){
	       memcpy(buffer, (float *) buf, (n_data + 1) * sizeof(float));		// +1
//	       if(flag == true){ 
//              flag = false;
//           }
//	       else{
              memcpy(data, buffer, (n_data) * sizeof(float));		   
//	       }
        //n_err_msg("err", "RECV\ndata: %f %f %f %f\nbuff: %f %f %f %f", data[0],data[1],data[2],data[3],buffer[0],buffer[1],buffer[2],buffer[3]);        
//        n_err_msg("err", "RECV\ndata: %f %f %f\nbuff: %f %f %f\nbytes recibidos (1 2) %d %d", data[0],data[1],data[2],buffer[0],buffer[1],buffer[2], bytesRecibidos, bytesRecibidos_2);        
        }
    }
    
    float    get_elem1f(int i){ return data[i]; }
    tVector2 get_elem2f(int i){ return tVector2(data[i], data[i+1]); }
    tVector3 get_elem3f(int i){ return tVector3(data[i], data[i+1], data[i+2]); }
    tVector4 get_elem4f(int i){ return tVector4(data[i], data[i+1], data[i+2], data[i+3]); }
    float    get_elemf()      { if(n_actual < n_data)     n_actual++;  return data[n_actual-1]; }
    tVector2 get_elem2f()     { if(n_actual < n_data - 2) n_actual+=2; return tVector2(data[n_actual-2], data[n_actual-1]); }
    tVector3 get_elem3f()     { if(n_actual < n_data - 3) n_actual+=3; return tVector3(data[n_actual-3], data[n_actual-2],data[n_actual-1]); }
    tVector4 get_elem4f()     { if(n_actual < n_data - 4) n_actual+=4; return tVector4(data[n_actual-4], data[n_actual-3],data[n_actual-2],data[n_actual-1]); }
    
    // hay que llamarlo justo al final de todo
    void flush(){
        n_actual = 0;
    }
   
    void get_msg_state(char *ret){
        switch(msg_actual){
           case _msg_null:        strcpy(ret, "..."); break;
           case _msg_error:       strcpy(ret, "<!> --- Error en socket --- "); break;
           case _msg_err_rcv:     strcpy(ret, "<!> Fallo en recv from "); break;
           case _msg_escuchando:  strcpy(ret, "< > Escribiendo en el puerto definido "); break;
           case _msg_recibiendo:  strcpy(ret, "<+> Recibiendo bytes ");  break;
           case _msg_recibiendo2: strcpy(ret, "<-> Recibiendo bytes (no hay más por ahora) "); break;        
           default:               strcpy(ret, "ERROR: estado no reconocido.");
        }
    }   

} net_receptor_t;


