#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct stu{
	int rollno;
	char name[15],grade[5];
	int marks[5],total;
	float per,att;
	char G;
	struct stu *link;
}*first=NULL,*last=NULL,*cur=NULL,*hp[4]={NULL,NULL,NULL,NULL},*stack=NULL;//hashing table,linked list,stack,queue
struct rnk{
	int rollno,m;
	char name[15];
	struct rnk *rchild,*lchild;
}*root;//bst only for ranking use
struct req{
	char a[15],b[15];
	int roll;
	char z[13];
	struct req *link;
}*queue=NULL;
void enque(struct req *lead);
void deque();void requests();
FILE *r,*w;
int i,f,l,h;
int S,T;
char pwd[15],str[15],user[15],temp[15],sub[11][13],s[15],k[10];//these global variables are used for internal applications
char file[2][16]={"$stulogin.txt","$stafflogin.txt"},sec[4][13]={"2021-22.txt","2020-21.txt","2019-20.txt","2018-19.txt"};
//lodin info and sec A,B,C,D records
void forgetchange();
void adminmode();void studentmode();int checklog(int ST);void change();void forget();//modules and login access
void append(char a[15],int r,int app[5],float p);int check(int r);char gradefind(float p);//add student from backup and mannual
void readbackup();void appendbackup();void editbackup();void read1();//file operations
void dispall();void disp1();void search();void dispperformance();void reportcard();//display functions
void delall();void del();void modify();//changing data in memory not in files
void lowest(struct stu *temp);void ttol(struct stu *temp/*,struct stu *lead*/);//ranking method 1
void push(char a[15],int r,int t,float p,struct stu *lead);void pop();//not req
struct rnk *get(struct stu *m);void subrank(struct rnk *t);struct rnk *insert(struct rnk *temp);struct rnk *bstdel(struct rnk *m);
//ranking method 2(easy)	for bst creaction and deletion every node
int year(){
	if(!strcmp(k,"2021-22"))return(0);
	else if(!strcmp(k,"2020-21"))return(1);
	else if(!strcmp(k,"2019-20"))return(2);
	else if(!strcmp(k,"2018-19"))return(3);
}
int checklog(int ST){
	int c;
	printf("ENTER YOUR USER ID:");
	scanf("%s",user);
	r=fopen(file[ST-1],"r");
	if(r==NULL){
		printf("UNABLE TO LOGIN.\n");
		return(0);
	}
	while(!feof(r))
	{
		fscanf(r,"%s %s",temp,str);
		if(ST==1){
			fscanf(r,"	%s	%d",k,&f);
		}
		if(c=(!strcmp(user,temp)))break;
	}
	if(c==0){
		printf("INVALID USER ID.\n");
	}else{
		printf("ENTER PASSWORD:");
		scanf("%s",pwd);
		if(c=(!strcmp(pwd,str))){
			printf("YOU HAVE LOGED IN.\n");
		}else{
			printf("INCORRECT PASSWORD.\n");
		}
	}
	fclose(r);
	return(c);
}
void studentmode(){
	read1();
	while(1){
	
		printf("\n1.YOUR REPORT CARD\n2.PASS FAIL LIST\n3.CHANGE PASSWORD\n4.LOGOUT\n\tENTER YOUR CHOICE:");
		scanf("%d",&S);
		switch(S){
		case 1:
		if(cur==NULL){
			printf("NO DETAILS OF YOU ARE FOUND.\n");
			break;
		}
		reportcard();
		break;
		case 2:
		if(cur==NULL){
			printf("NO DETAILS OF YOU ARE FOUND.\n");
			break;
		}
		printf("PASSED: ");
		for(i=0;i<5;i++){
			if(cur->marks[i]>=35){
				printf("%s ,",sub[i+3]);
			}
		}
		printf("\nFAILED: ");
		for(i=0;i<5;i++){
			if(cur->marks[i]<35){
				printf("%s ,",sub[i+3]);
			}
		}
		break;
		case 3:change();
		break;
		case 4:delall();
		return;
		default:printf("WRONG ENTRY OF OPTION.\n");
		}
		printf("\n\t------------------------------------------------\n");
	}
}
void change(){
	r=fopen("$stulogin.txt","r");
	while(!feof(r)){
		fscanf(r,"%s	%s	%s	%d",user,pwd,k,&f);
		if(!strcmp(user,temp)){
			printf("ENTER PREVIOUS PASSWORD:");
			scanf("%s",str);
			if(!strcmp(pwd,str)){
				printf("ENTER NEW PASSWORD:");
				scanf("%s",pwd);
			}else printf("INCORRECT PASSWORD");
		}
		enque(queue);
	}
	fclose(r);
	w=fopen("$stulogin.txt","w");
	while(queue!=NULL){
		fprintf(w,"\n%s\t%s\t%s\t%d",queue->a,queue->b,queue->z,queue->roll);
		deque();
	}
	fclose(w);
}
void adminmode(){
	float att;
	int mark[5],roll;
	char a[15];
	for(h=0;h<4;h++){
		first=hp[h];
		readbackup();
		hp[h]=first;
	}
	while(1){
		printf("\n1.2021-22\t2.2020-21\t3.2019-20\t4.2018-19\t5.REQUESTS\t6.FORGOT REQUESTS\t7.LOGOUT\nENTER SECTION:");
		scanf("%d",&h);
		if(h<5 && h>0){
			first=hp[h-1];
			if(first!=NULL) {
				for(last=first;last->link!=NULL;last=last->link);
			}
		}
		while(h<5 && h>0){
			printf("\n1.NEW STUDENT RECORD\n2.DISPLAY ALL STUDENTS REPORT\n3.SEARCH A STUDENT REPORT\n");
			printf("4.PERFORMANCE OF ALL STUDENTS\n5.MODIFY A REPORT CARD\n6.DELETE A REPORT CARD\n");
			printf("7.RANKING\n8.SUBJECT WISE RANKING\n");
			printf("9.BACK TO SECTIONS\n10.LOGOUT\n\tENTER YOUR OPTION:");
			scanf("%d",&T);
			switch(T){
				case 1:printf("ENTER ROLL NO. & NAME OF STUDENT:\n");
				scanf("%s%d",a,&roll);
				if(check(roll)){
					printf("%d ALREADY EXIST.\n",roll);
					break;
				}
				for(i=0;i<5;i++){
					printf("ENTER MARKS OF %s:",sub[i+3]);
					scanf("%d",&mark[i]);
					if(mark[i]<0 || mark[i]>100){
						printf("WRONG ENTRY OF MARKS RANGE SHOULD BE 0 TO 100\n");
						i--;
					}
				}
				printf("ENTER ATTENDENCE PERCENTAGE OF %d:",roll);
				scanf("%f",&att);
				append(a,roll,mark,att);
				appendbackup();
				break;
				case 2:dispall();
				break;
				case 3:search();
				break;
				case 4:dispperformance();
				break;
				case 5:modify();
				break;
				case 6:del();
				break;
				case 7:
				if(first!=NULL){
					lowest(first);
					l=500;f=1;
					printf("-------------------------------------------------------\n");
					printf("RANK\tROLLNO.\tNAME    \t\tMARKS\tPER\n");
					printf("-------------------------------------------------------\n");
					ttol(first);
					break;
				}
				printf("SOMTHING WENT WRONG.\n");
				break;
				case 8:
				printf("1.PHYSICS\t2.CHEMISTRY\t3.MATHEMATICS\t4.ENGLISH\t5.CMP SCIENCE\nENTER SUBJECT:");
				scanf("%d",&i);
				i--;
				for(cur=first;cur!=NULL;cur=cur->link){
					root=insert(root);
				}
				f=1;
				printf("------------------------------------------------\n");
				printf("RANK\tROLLNO.\tNAME    \t\tMARKS\n");
				printf("------------------------------------------------\n");
				if(root!=NULL){
					subrank(root);
					root=bstdel(root);
					break;
				}
				printf("SOMETHING WENT WRONG.\n");
				break;
				case 9:h=0;break;
				case 10:h=7;break;
				default:printf("WRONG ENTRY OF OPTION.\n");
			}
			printf("\n\t------------------------------------------------\n");
		}
		if(h==5){
			requests();
		}
		if(h==6){
			forgetchange();
		}
		if(h==7){
			for(h=0;h<4;h++){
				first=hp[h];
				delall();
				hp[h]=NULL;
			}
			return;
		}
	}
}
void read1(){
	int app[5];
	int roll;
	float att;
	r=fopen(sec[year()],"r");
	first=hp[year()];
	if(r==NULL){
		printf("UNABLE TO LOAD DATA.\n");
		return;
	}
	for(i=0;i<11;i++){
		fscanf(r,"%s",sub[i]);
	}
	while(fgetc(r)!='\n'){
	}
	while(r->_cnt!=0){
		fscanf(r,"%d",&roll);
		if(roll==f){
			fscanf(r,"%s			%f",s,&att);
			for(i=0;i<5;i++){
				fscanf(r,"%d",&app[i]);
			}
			append(s,roll,app,att);break;
		}
		while(fgetc(r)!='\n'){
		}
	}
	fclose(r);
}
void reportcard(){
	printf("------------------------------------------------------------------------------------------------------\n");
	printf("\n\n\t\t\t\tSTUDENT\tREPORT\tCARD\n");
	printf("\n\t\tNAME OF THE STUDENT:%s\n",cur->name);
	printf("\n\t\tROLLNO. OF THE STUDENT:%d\n",cur->rollno);
	printf("\n\t\tACTIVE YEAR OF THE STUDENT:%s\n",k);
	printf("\n\t----------------------------------------------------------------------------------");
	printf("\n\tSNO.\tSUBJECT\t\tOBT.MARKS\tMAX.MARKS\tOBT.GRADE\tMAX.GRADE\n");
	printf("\t----------------------------------------------------------------------------------\n");
	printf("\n\t1.  \tPHYSICS\t\t%d\t\t100\t\t%c\t\tO\n",cur->marks[0],cur->grade[0]);
	printf("\n\t2.  \tCHEMISTRY\t%d\t\t100\t\t%c\t\tO\n",cur->marks[1],cur->grade[1]);
	printf("\n\t3.  \tMATHEMATICS\t%d\t\t100\t\t%c\t\tO\n",cur->marks[2],cur->grade[2]);
	printf("\n\t4.  \tENGLISH\t\t%d\t\t100\t\t%c\t\tO\n",cur->marks[3],cur->grade[3]);
	printf("\n\t5.  \tCMP SCIENCE\t%d\t\t100\t\t%c\t\tO\n",cur->marks[4],cur->grade[4]);
	printf("\n\t----------------------------------------------------------------------------------\n");
	printf("\t\tTOTAL:\t\t%d\t\t500\t\t%c\t\tO\n",cur->total,cur->G);
	printf("\n\t----------------------------------------------------------------------------------\n");
	printf("\n\tPERCENTAGE:\t%0.2f\n\n\tATTENDANCE:\t%0.2f\n",cur->per,cur->att);
	printf("\n------------------------------------------------------------------------------------------------------\n");
}
void delall(){
	for(;first!=NULL;first=first->link){
		free(first);
	}
	cur=NULL;
	last=NULL;
}
void readbackup(){
	int app[5];
	int roll;
	float att;
	r=fopen(sec[h],"r");
	if(r==NULL){
		printf("UNABLE TO LOAD DATA.\n");
		return;
	}
	for(i=0;i<11;i++){
		fscanf(r,"%s",sub[i]);
	}
	while(fgetc(r)!='\n'){
	}
	while(r->_cnt!=0){
		fscanf(r,"%d%s			%f",&roll,s,&att);
		for(i=0;i<5;i++){
			fscanf(r,"%d",&app[i]);
		}
		append(s,roll,app,att);
		while(fgetc(r)!='\n'){
		}
	}
	fclose(r);
}
int check(int r){
	for(cur=first;cur!=NULL;cur=cur->link){
		if(r==cur->rollno)return(1);
	}
	return(0);
}
void append(char a[15],int r,int app[5],float p){
	cur=(struct stu*)malloc(sizeof(struct stu));
	strcpy(cur->name,a);
	cur->rollno=r;
	cur->total=0;
	cur->att=p;
	for(i=0;i<5;i++){
		cur->marks[i]=app[i];
		cur->grade[i]=gradefind(cur->marks[i]);
		cur->total+=app[i];
	}
	cur->per=cur->total*0.2;
	cur->G=gradefind(cur->per);
	cur->link=NULL;
	if(first==NULL){
		first=cur;
		last=first;
		return;
	}
	last->link=cur;
	last=cur;
	/*if(first==NULL)first=cur;return;
	while(1){
	}
	*/
}
char gradefind(float p){
	if(p>=91){
		return('O');
	}else if(p>=81){
		return('A');
	}else if(p>=71){
		return('B');		
	}else if(p>=61){
		return('C');
	}else if(p>=51){
		return('D');
	}else if(p>=35){
		return('E');
	}else{
		return('F');
	}
}
void dispall(){
	if(first==NULL){
		printf("NO STUDENT RECORDS PRESENT.\n");
		return;
	}
	printf("--------------------------------------------------------------------------------------------------------------\n");
	printf("ROLLNO.\tNAME     \t\tATTENDENCE\tPHY\tCHE\tMATH\tENG\tCS\tTOTAL\tPER()\tGRADE\n");
	printf("--------------------------------------------------------------------------------------------------------------\n");
	for(cur=first;cur!=NULL;cur=cur->link){
		disp1();
	}
}
void disp1(){
	printf("%d\t%s\t\t%0.2f\t\t",cur->rollno,cur->name,cur->att);
	for(i=0;i<5;i++){
		printf("%d-%c\t",cur->marks[i],cur->grade[i]);
	}
	printf("%d\t%0.2f\t%c\n",cur->total,cur->per,cur->G);
}
void search(){
	printf("ENTER ROLLNO. :");
	scanf("%d",&i);
	if(first==NULL){
		printf("NO STUDENT RECORDS PRESENT.");
		return;
	}
	if(check(i)){
		printf("--------------------------------------------------------------------------------------------------------------\n");
		printf("ROLLNO.\tNAME     \t\tATTENDENCE\tPHY\tCHE\tMATH\tENG\tCS\tTOTAL\tPER()\tGRADE\n");
		printf("--------------------------------------------------------------------------------------------------------------\n");
		disp1();
		return;
	}
	printf("%d IS NOT FOUND IN STUDENTS REPORT RECORD.",i);
}
void dispperformance(){
	printf("-------------------------------------------------------\n");
	printf("ROLLNO.\tNAME    \t\tPER()\t\tGRADE\n");
	printf("-------------------------------------------------------\n");
	for(cur=first;cur!=NULL;cur=cur->link){
		printf("%d\t%s\t\t%0.2f\t\t%c\n",cur->rollno,cur->name,cur->per,cur->G);
	}
}
void modify(){
	float p;
	int m[5],o;
	search();
	if(cur!=NULL){
		for(i=0;i<5;i++){
			m[i]=cur->marks[i];
		}
		p=cur->att;
		printf("1.PHYSICS\n2.CHEMISTRY\n3.MATHEMATICS\n4.ENGLISH\n5.CMP SCIENCE\n6.ATTENDANCE\n7.DONE\n8.RETURN WITHOUT MODIFYING\n");
		while(1){
			printf("ENTER:");
			scanf("%d",&o);
			switch(o){
				case 1:case 2:case 3:case 4:case 5:i=o-1;
				printf("ENTER MODIFYING MARKS:");
				scanf("%d",&m[i]);
				break;
				case 6:
				printf("ENTER MODIFYING ATTENDENCE:");
				scanf("%f",&p);
				break;
				case 7:
					printf("ENTER PASSWORD TO CONTINUE:");
					scanf("%s",pwd);
					if(!strcmp(pwd,str)){
						cur->total=0;
						cur->att=p;
						for(i=0;i<5;i++){
							cur->marks[i]=m[i];
							cur->grade[i]=gradefind(cur->marks[i]);
							cur->total+=m[i];
						}
						cur->per=cur->total*0.2;
						cur->G=gradefind(cur->per);
						editbackup();
					}else{
						printf("INCORRECT PASSWORD.\n");
						break;
					}
				case 8:return;
			}
		}
	}
}
void del(){
	printf("ENTER ROLL NO. TO DELETE STUDENT REPORT CARD:");
	scanf("%d",&i);
	struct stu *t=first;
	for(cur=first;cur!=NULL;cur=cur->link){
		if(cur->rollno==i)break;
		t=cur;
	}
	if(cur!=NULL){
		disp1();
		printf("ENTER PASSWORD TO CONTINUE:");
		scanf("%s",pwd);
		if(!strcmp(str,pwd)){
			if(cur==first){
				first=first->link;
			}else if(cur==last){
				t->link=NULL;
				last=t;
			}else{
				t->link=cur->link;
			}
			cur->link=NULL;
			free(cur);
			cur=NULL;
			editbackup();
			return;
		}
		printf("INCORRECT PASSWORD.\n");
		return;
	}
	printf("%d IS NOT FOUND IN STUDENT REPORT RECORDS.",i);
}
void lowest(struct stu *temp){//323,453,456,323,451
	for(cur=temp->link;cur!=NULL;cur=cur->link){
		if(temp->total>=cur->total)temp=cur;
	}
	cur=temp;
	push(cur->name,cur->rollno,cur->total,cur->per,NULL);
}
void ttol(struct stu *temp/*,struct stu *lead*/){
	for(;temp!=NULL;temp=temp->link){
		if(temp!=cur && l>=temp->total){
			if(/*lead->total*/stack->total<temp->total){
				push(temp->name,temp->rollno,temp->total,temp->per,stack);//not req
				ttol(temp->link/*,temp*/);
			}
			else if(/*cur==lead*/stack->link==NULL && /*cur->total*/stack->total==temp->total){
				push(temp->name,temp->rollno,temp->total,temp->per,stack);//not req
				ttol(temp->link/*,temp*/);
			}
		}
	}
	l=stack->total;//l=lead->total
	pop();//printf("%d\t%d\t%s\t\t%d\t%0.2f\n",f++,lead->rollno,lead->name,lead->total,lead->per);
}
void push(char a[15],int r,int t,float p,struct stu *lead){
	stack=(struct stu*)malloc(sizeof(struct stu));
	strcpy(stack->name,a);
	stack->rollno=r;
	stack->per=p;
	stack->total=t;
	stack->link=lead;
}
void pop(){
	printf("%d\t%d\t%s\t\t%d\t%0.2f\n",f++,stack->rollno,stack->name,stack->total,stack->per);
	free(stack);
	stack=stack->link;
}
struct rnk *insert(struct rnk *temp){
	if(root==NULL){
		root=get(cur);
		return(root);
	}
	while(1){
		if(temp->m>cur->marks[i]){
			if(temp->lchild==NULL){
                temp->lchild=get(cur);
                break;
            }else
            temp=temp->lchild;
		}else{
			 if(temp->rchild==NULL){
                temp->rchild=get(cur);
                break;
            }else
            temp=temp->rchild;
		}
	}
	return(root);
}
struct rnk *get(struct stu *m){
	struct rnk *t=(struct rnk*)malloc(sizeof(struct rnk));
	t->rollno=m->rollno;
	strcpy(t->name,m->name);
	t->m=m->marks[i];
	t->lchild=NULL;
	t->rchild=NULL;
	return(t);
}
void subrank(struct rnk *t){
	if(t->rchild!=NULL){
        subrank(t->rchild);
    }
    printf("%d\t%d\t%s\t\t%d\n",f++,t->rollno,t->name,t->m);
    if(t->lchild!=NULL){
        subrank(t->lchild);
    }
}
struct rnk *bstdel(struct rnk *m){
    if(m->lchild!=NULL){
        bstdel(m->lchild);
    }
    if(m->rchild!=NULL){
        bstdel(m->rchild);
    }
	free(m);
	return(NULL);
}
void appendbackup(){
	w=fopen(sec[h-1],"a");
	fprintf(w,"%d\t\t%s\t\t\t%0.2f\t\t",cur->rollno,cur->name,cur->att);
	for(i=0;i<5;i++){
		fprintf(w,"%d\t\t",cur->marks[i]);
	}
	fprintf(w,"%d\t\t%0.2f\t\t%c\n",cur->total,cur->per,cur->G);
	fclose(w);
}
void editbackup(){
	w=fopen(sec[h-1],"w");
	for(i=0;i<11;i++){
		fprintf(w,"%s\t\t",sub[i]);
	}
	fprintf(w,"\n");
	for(cur=first;cur!=NULL;cur=cur->link){
		fprintf(w,"%d\t\t%s\t\t\t%0.2f\t\t",cur->rollno,cur->name,cur->att);
		for(i=0;i<5;i++){
			fprintf(w,"%d\t\t",cur->marks[i]);
		}
		fprintf(w,"%d\t\t%0.2f\t\t%c\n",cur->total,cur->per,cur->G);
	}
	fclose(w);
}///////////////////////////////////////
void requests(){
	r=fopen("$request.txt","r");
	f=0;
	while(!feof(r)){
		fscanf(r,"%d	%s	%s	%s",&f,k,user,pwd);
		if(f!=0)enque(queue);
	}
	fclose(r);
	if(queue==NULL){
		printf("NO REQUESTS FOUND.\n");
		return;
	}
	w=fopen("$request.txt","w");fclose(w);
	printf("REQUEST ARE...\n");
	while(queue!=NULL){
		printf("%d\t%s\n1.ACCEPT\t2.DENEY\t3.NOT YET\t4.BACK\nENTER:",queue->roll,queue->z);
		scanf("%d",&i);
		switch(i){
			case 1:w=fopen("$stulogin.txt","a");
			fprintf(w,"\n%s\t%s\t%s\t%d",queue->a,queue->b,queue->z,queue->roll);
			fclose(w);
			case 2:deque();
			break;
			case 3:w=fopen("$request.txt","a");
			fprintf(w,"\n%d %s %s %s",queue->roll,queue->z,queue->a,queue->b);
			fclose(w);
			deque();
			break;
			case 4:w=fopen("$request.txt","a");
			for(;queue!=NULL;deque()){
				fprintf(w,"\n%d %s %s %s",queue->roll,queue->z,queue->a,queue->b);
			}
			fclose(w);
			break;
		}
	}
}
void signup(){
	printf("ENTER USERID:");
	scanf("%s",user);
	r=fopen("$stulogin.txt","r");
	while(!feof(r)){
		fscanf(r,"%s",temp);
		if(!strcmp(user,temp)){
			printf("USERID ALREADY EXISTS.\n");
			fclose(r);
			return;
		}
	}
	fclose(r);
	printf("ENTER YEAR:");
	scanf("%s",k);
	printf("ENTER YOUR PIN:");
	scanf("%d",&f);
	printf("ENTER PASSWORD:");
	scanf("%s",pwd);
	w=fopen("$request.txt","a");
	fprintf(r,"\n%d	%s	%s	%s",f,k,user,pwd);
	fclose(w);
	printf("YOU HAVE REQUESTED FOR USER ACCOUNT.\nWAIT UNTILL ADMIN ACCEPT IT.\n");
}
void enque(struct req *lead){
	struct req *q=(struct req*)malloc(sizeof(struct req));
	strcpy(q->a,user);
	strcpy(q->b,pwd);
	q->roll=f;
	strcpy(q->z,k);
	q->link=NULL;
	if(lead!=NULL){
		for(;lead->link!=NULL;lead=lead->link);
		lead->link=q;
		return;
	}
	queue=q;
}
void deque(){
	free(queue);
	queue=queue->link;
}
void forget(){
	printf("ENTER YOUR NAME AS IN RECORDS:");
	scanf("%s",s);
	printf("ENTER YOUR ACTIVE YEAR:");
	scanf("%s",k);
	printf("ENTER YOUR ROLLNO.:");
	scanf("%d",&f);
	printf("ENTER YOUR USERID:");
	scanf("%s",user);
	w=fopen("$forget.txt","a");
	fprintf(w,"\n%d\t%s\t%s\t%s",f,s,user,k);
	fclose(w);
	printf("REQUESTED HAVE DONE.\nAFTER ADMIN ACCEPT IT USE YOUR NAME IN RECORDS AS PASSWORD.");
}
void forgetchange(){
	r=fopen("$forget.txt","r");
	f=0;
	while(!feof(r)){
		fscanf(r,"%d	%s	%s	%s",&f,pwd,user,k);
		printf("%d\t%s\t%s\t%s\n1.ACCEPT\t2.DENEY\t3.BACK\nENTER:",f,pwd,user,k);
		scanf("%d",&i);
		if(i==1)enque(queue);
		if(i==3)break;
	}
	fclose(r);
	w=fopen("$forget.txt","w");fclose(w);
	r=fopen("$stulogin.txt","r");
	while(!feof(r)){
		fscanf(r,"%s	%s	%s	%d",user,pwd,k,&f);
		enque(queue);
	}
	fclose(r);
	w=fopen("$stulogin.txt","w");
	while(queue!=NULL){
		fprintf(w,"\n%s\t%s\t%s\t%d",queue->a,queue->b,queue->z,queue->roll);
		deque();
	}
	fclose(w);
}
