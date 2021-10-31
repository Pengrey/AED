# include <stdio.h>
# include <stdlib.h>
# include <math.h>

/*Problem 1:
* Write a program that finds all integer n that have the following characteristics:
*  -the integer has exactly 10 base-10 digits
*  -the integer cannot have repeated digits (all digits must appear exactly once)
*  -for k = 1,2,..,10 the integer formed by the first k most significant base-10 digits of n must be divisible by k
*/

/* Note:
*  -O numero é composto por 10 digitos mas o primeiro poderá ser qlqr um pois qlqr digito de (0-9) é divisivel por 1
*  -O ultimo digito terá k ser obrigatóriamente 0 pois só assim o numero formado pelos 10 digitos será divisivel por 10    
*/

int does_it_work(long long int n){
  for (int i=0;i<=9;i++){
    if(((long long)(n/(pow(10,(9-i)))) % (i+1))==0){ //uso de 10^i para obter os numeros k vêem em primeiro i+1 elementos. Pois têm-se k-> (int)(12301/10^2) = 123 dando assim para verificar a sua divisão. 
      if(i==9){return 1;}                            //se o if chegar aqui sem return ent é divisivel segundo os critérios do ex
      continue;                                      //salto para continuar a verificar divisibilidade
    }else{                                            
      return 0;                                      //caso n seja divisivel dá return de 0 (n há booleans é o k se têm) 
    }
  }
  return 3;                                          //para o compilador ficar contente pk ele quer ter a certeza k tem um return, worried boi
}

void get_number(){
 long long int i,n;
 int flag;
 int count[10]={0};
 for(i =123456789; i<=987654321; i++){    //como terá de ser divisivel por 10 os numeros possiveis terão sempre um 0 no fim
    n=i*10;
    while (n>0){
    ++count[n%10];
    n/=10;
    }
    for(n=0; n < 10; n++){
      if(count[n]!=1){
        flag=0;
        break;
      }else{
        flag=1;
      }
    }
    if ((flag == 1)&& does_it_work((i*10))==1){
      printf("-->%lli\n",i*10);
    }
  }
}

int main(void){
  get_number(); 
  return 0; 
}
