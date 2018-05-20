#include <string.h>
#include <stdlib.h>

typedef struct {
  unsigned char type;
  unsigned char data[42];
  unsigned int len;
} ssl_buffer;

typedef struct {
  ssl_buffer buffer;
} SSL;

/* function stubs - we don't need the implementation */
void RAND_pseudo_bytes (unsigned char*, unsigned int);
int ssl3_write_bytes (SSL*, unsigned, void*, unsigned);
unsigned int nondet_uint();
unsigned char nondet_uchar();

#define n2s(c,s)	((s=(((unsigned int)(c[0]))<< 8)| \
			    (((unsigned int)(c[1]))    )),c+=2)
#define s2n(s,c)	((c[0]=(unsigned char)(((s)>> 8)&0xff), \
			  c[1]=(unsigned char)(((s)    )&0xff)),c+=2)

#define TLS1_HB_REQUEST		1
#define TLS1_HB_RESPONSE	2
#define TLS1_RT_HEARTBEAT 24

int tls1_process_heartbeat(SSL *s) {
  unsigned char *p = s->buffer.data, *pl;
  
  unsigned char hbtype;
  unsigned int payload;
  unsigned int padding = 16;

  hbtype = s->buffer.type;
  n2s(p, payload);
  pl = p;
  
  if (hbtype == TLS1_HB_REQUEST) {
    unsigned char *buffer, *bp;
    int r;

    /* Allocate memory for the response, size is 1 bytes
     * message type, plus 2 bytes payload length, plus
     * payload, plus padding
     */
    buffer = malloc(1 + 2 + payload + padding);
    bp = buffer;
		
    /* Enter response type, length and copy payload */
    *bp++ = TLS1_HB_RESPONSE;
    s2n(payload, bp);
    memcpy(bp, pl, payload);
    bp += payload;
    /* Random padding */
    RAND_pseudo_bytes(bp, padding);

    r = ssl3_write_bytes(s, TLS1_RT_HEARTBEAT, buffer, 3 + payload + padding);
    
    if (r < 0)
      return r;
  }
  else if (hbtype == TLS1_HB_RESPONSE) {
    // ...
  }

  return 0;
}

int main() {
  SSL obj;

  obj.buffer.type = TLS1_HB_REQUEST;
  // non-deterministically assign a length of the buffer
  obj.buffer.len = nondet_uint(); 
  return tls1_process_heartbeat(&obj);
}
