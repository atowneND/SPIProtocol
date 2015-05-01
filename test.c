#include <stdio.h>
#include <stdlib.h>

int main(){
    unsigned int foo = 0xFF47;
    int i = 0;
    unsigned int bar = 0x0000;
    unsigned int temp = 0x0000;

    for (i=0;i<8;i++){
        temp = (foo>>i)&1;
//        printf("temp = %i\n",temp);
        bar = bar|(temp << i);
        printf("i: %i\tibar: %i\ttemp: %i\tbar: %i\n",i,31-i,temp,bar);
    }
    
    return 0;
}
