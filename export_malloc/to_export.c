#include <stdio.h>
#include <emscripten/emscripten.h>

void EMSCRIPTEN_KEEPALIVE fillData(int32_t *buffer,size_t size){
  printf("Got pointer is %p\n",buffer);

  for(int i = 0;i<size;i++){
    buffer[i] = i*2;
  }
}

int main(int argc, char *argv[])
{
  return 0;
}