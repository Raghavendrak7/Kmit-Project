#include"$sturec.h"
int ST;
void forgetchange();
int main(){
	printf("\t\t\tWELCOME\t\tTO\n\t\tSTUDENT REPORT CARD CONSOLE APPLICATION.\n");
	while(1)
	{
		printf("\n\t1.STUDENT MODULE\t2.ADMIN MODULE\t\t3.SIGN UP\t\t4.FORGET\t5.EXIT\nENTER YOUR CHOICE:");
		scanf("%d",&ST);
		switch(ST){
			case 1:
			if(checklog(ST)){
				studentmode();
				printf("YOU HAVE LOGED OUT OF THIS APPLICATION.\n");
			}
			break;
			case 2:
			if(checklog(ST)){
				adminmode();
				printf("YOU HAVE LOGED OUT OF THIS APPLICATION.\n");
			}
			break;
			case 3:signup();
			break;
			case 4:forget();break;
			case 5:exit(0);
			default:printf("WRONG ENTRY OF OPTIONS.\n");
		}
	}
	return(0);
}
