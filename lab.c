#include <stdio.h>
#include <string.h>

typedef enum
{
	true = 1,
	false = 0,
} bool;

void propriedades(char *buff)
{
  *buff = '\0';
  for (int i = 0; i < 8; i++)
  {
    strcat(buff, "[#]");
  }
}

void propriedades2(char *buff)
{
  *buff = '\0';

  for (int i = 0; i < 8; i++)
  {
    strcat(buff, "[-]");
  }
}

int main()
{
  /*
  char buff[25] = "";
  
  propriedades(buff);
  printf("1-%s\n", buff);

  propriedades2(buff);
  printf("2-%s\n", buff);
  printf("Len: %d\n", strlen("[C][C][C][C][C][C][C][C]"));
  
  int x = 512;
  int y = 2048;

  double result = (double) x / y; 
  printf("%lf\n", result);

  x = 1024;
  result = (double) x / y;
  printf("%lf\n", result);

  x = 513;
  result = x / y;
  printf("%lf\n", result);
  return 0;

  */
 if(NULL == false)
  puts("Sim e");


 return 0;
}
