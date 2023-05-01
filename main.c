#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define NoRType 5
#define NoIType 8
#define NoJType 2

#define Rtype 0
#define Itype 1
#define Jtype 2

struct symblTable{
    int value;
    char *symbol;
    int amount;
};
struct instruction{
    size_t instType;
    size_t intinst;
    char   *nameOfInstruction;
    char   instructHex[8];
    int rs;
    int rt;
    int rd;
    int imm;
    int PC;
    int opcode;
};
int decToHexa(int n,char offsethex[])
{
    int i = 0;
    while (n != 0) {
        int temp = 0;
        temp = n % 16;
        if (temp < 10) {
            offsethex[i] = temp + 48;
            i++;
        }
        else {
            offsethex[i] = temp + 55;
            i++;
        }
        n = n / 16;
    }
    int len=i;
    return len;
}
int findSymTableLen(FILE *);
int fillSymTab(struct symblTable *,FILE *);
void formInst(struct instruction *,FILE *);
int hexToInt(char*,int);
void intToHex16(struct instruction *);
int heximaltoint(char *);
void main(int argc,char **argv){
    FILE *assp,*machp,*fopen();
    struct symblTable *psymTab;
    int symbolTableLen;
    int i,j,found,noInst;
    struct instruction *currentInst;
    size_t lineSize=72;
    char  *line;
    char *token;
    char *instructions[]={"add","sub","slt","or","nand","addi","slti","ori","lui","lw","sw","beq","jalr","j","halt"};
    int instructionCount=0;
    char *hexTable[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    char Lower[5];
    i=0;
    j=0;
    line=(char *) malloc(72);
    currentInst=(struct instruction *) malloc(sizeof (struct instruction));

    if(argc < 3){
        printf("***** Please run this program as follows:\n");
        printf("***** %s assprog.as machprog.m\n",argv[0]);
        printf("***** where assprog.as is your assembly program\n");
        printf("***** and machprog.m will be your machine code.\n");
        exit(1);
    }
    if((assp=fopen(argv[1],"r")) == NULL){
        printf("%s cannot be openned\n",argv[1]);
        exit(1);
    }
    if((machp= fopen(argv[2],"w+"))==NULL){
        printf("%s cannot be oppened\n",argv[2]);
        exit(1);
    }
    symbolTableLen=findSymTableLen(assp);
   // printf("%d\n",symbolTableLen);
    psymTab=(struct symblTable *) malloc(symbolTableLen* sizeof(struct symblTable));
    for(i=0;i<symbolTableLen;i++){
        psymTab[i].symbol=(char *) malloc(symbolTableLen*sizeof (struct symblTable));
    }
    noInst= fillSymTab(psymTab,assp);

   /* for(int(i)=0;(i)<symbolTableLen;i++){
        printf("%s %d\n",psymTab[i].symbol,psymTab[i].value);
    }*/
    while (fgets(line, lineSize, assp) != NULL){
        instructionCount++;
        found=0;
        token= strtok(line,"\t ,\n");
        for(i=0;i<symbolTableLen;i++) {
            if (strcmp(psymTab[i].symbol, token) == 0) {
                token = strtok(NULL, "\t ,\n");
            }
        }
        bool isfill=false;
        char * fill=".fill ";
        char * fill2=".fill";
        if((strcmp(token,fill)==0)||(strcmp(token,fill2)==0)){
                token = strtok(NULL, "\t ,\n");
                bool isnum=true;
                isfill=true;
                for(j=0;j<symbolTableLen;j++){
                    if(strcmp(psymTab[j].symbol,token)==0){
                        currentInst->intinst=psymTab[j].value;
                        isnum=false;
                    }
                }
                if(isnum){
                    currentInst->intinst=atoi(token);
                }
            printf("%d\n",currentInst->intinst);
        }
        //strcpy(currentInst->nameOfInstruction,token);
        currentInst->instType=-1;
        currentInst->nameOfInstruction=NULL; //bepak
        if(strcmp(/*currentInst->nameOfInstruction*/token,".fill")==0) {
            token = strtok(NULL, "\t,\n");
            bool isnumber = true;
            for (i = 0; i < symbolTableLen; i++) {
                if (strcmp(psymTab[i].symbol, token) == 0) {
                    currentInst->intinst = psymTab->value;
                    isnumber = false;
                }
            }
            if (isnumber) {
                currentInst->intinst = atoi(token);
            }
        }
        bool valid_opcode=false;
        for(i=0;i<=14;i++){
            if(strcmp(instructions[i],token)==0){
               // strcpy(currentInst->nameOfInstruction,token);
                currentInst->opcode=i;
                if(i>=0&&i<=4){
                    valid_opcode=true;
                    currentInst->instType=0;
                    currentInst->rd=atoi( strtok(NULL,"\t ,"));
                    currentInst->rs=atoi(strtok(NULL,"\t ,"));
                    currentInst->rt=atoi(strtok(NULL,"\t ,")) ;
                    intToHex16(currentInst);
                    currentInst->intinst= heximaltoint(currentInst->instructHex);
                    printf("%d\n",currentInst->intinst);
                    break;
                } else if(i>=5&&i<=12){
                    valid_opcode=true;
                    currentInst->instType=1;
                    if(i>=5&&i<=7){
                        currentInst->rt= atoi(strtok(NULL,"\t ,"));
                        currentInst->rs=atoi( strtok(NULL,"\t ,"));
                        currentInst->imm= atoi(strtok(NULL,"\t ,"));
                        intToHex16(currentInst);
                        currentInst->intinst= heximaltoint(currentInst->instructHex);
                        printf("%d\n",currentInst->intinst);
                        break;
                    } else if(i>=9&&i<=11){
                        currentInst->rt= atoi(strtok(NULL,"\t ,"));
                        currentInst->rs=atoi( strtok(NULL,"\t ,"));
                        char* offset= strtok(NULL,"\t ,\n");
                        bool num=true;
                        bool validL=false;
                        for(j=0;j<symbolTableLen;j++){
                            if(strcmp(psymTab[j].symbol,offset)==0){
                                currentInst->imm=psymTab[j].value;
                                num=false;
                                validL=true;
                                break;
                                //printf("%d\n",currentInst->imm);
                            }
                        }
                        if(i==11){
                            int num=currentInst->imm-instructionCount;
                            if(num<0){
                                currentInst->imm=num+65536;
                            } else{
                                currentInst->imm=num;
                            }
                        }
                        if(num){
                            currentInst->imm= atoi(offset);
                            validL=true;
                        }
                        if(validL==false){
                            printf("Label is not valid");
                            exit(1);
                        }
                        intToHex16(currentInst);
                        currentInst->intinst= heximaltoint(currentInst->instructHex);
                        printf("%d\n",currentInst->intinst);
                        break;
                    }else if(i==8){
                        currentInst->rt= atoi(strtok(NULL,"\t ,"));
                        currentInst->imm= atoi(strtok(NULL,"\t ,"));
                        currentInst->rs=0;
                        intToHex16(currentInst);
                        currentInst->intinst= heximaltoint(currentInst->instructHex);
                        printf("%d\n",currentInst->intinst);
                        break;
                    } else if(i==12){
                        currentInst->rt= atoi(strtok(NULL,"\t ,"));
                        currentInst->rs= atoi(strtok(NULL,"\t ,"));
                        currentInst->imm=0;
                        intToHex16(currentInst);
                        currentInst->intinst= heximaltoint(currentInst->instructHex);
                        printf("%d\n",currentInst->intinst);
                        break;
                    }
                }else if(i>=13&&i<=14){
                    valid_opcode=true;
                    currentInst->instType=2;
                    if(i==13){
                        char* offset=strtok(NULL,"\t ,\n");
                        bool validL=false;
                        for(j=0;j<symbolTableLen;j++){
                            if(strcmp(psymTab[j].symbol,offset)==0){
                                currentInst->imm=psymTab->value;
                                validL=true;
                                break;
                            }
                        }
                        if(validL==false){
                            printf("Label is not valid");
                            exit(1);
                        }
                        intToHex16(currentInst);
                        currentInst->intinst= heximaltoint(currentInst->instructHex);
                        printf("%d\n",currentInst->intinst);
                        break;
                    }else if(i==14){
                        currentInst->imm=0;
                        intToHex16(currentInst);
                        currentInst->intinst= heximaltoint(currentInst->instructHex);
                        printf("%d\n",currentInst->intinst);
                        break;
                    }
                }
            }
        }
        if(valid_opcode==false && isfill==false){
            printf("%s\n","opcode is not valid");
            exit(1);
        }
        fprintf(machp,"%d\n" , currentInst->intinst);
    }
    fclose(assp);
    fclose(machp);
}
int findSymTableLen(FILE *file){
    int count=0;
    size_t lineSize=72;
    char *line;
    line=(char *) malloc(72);
    while(fgets(line, lineSize , file) != NULL){
        //printf("%s\n",line);
        if((line[0]==' ')||(line[0]=='\t'));
        else
            count++;
    }
    rewind(file);
    free(line);
    return count;
}
int fillSymTab(struct symblTable* symblTable,FILE* file){
    int lineNum=0;
    size_t lineSize=72;
    char *line;
    int i=0;
    char *token;
    line=(char *) malloc(72);
    while(fgets(line, lineSize,file) != NULL){
        if(line[0]==' '||line[0]=='\t');
        else{
            token= strtok(line,"\t ,");
            if(isdigit(token[0])){
                printf("%s\n","lable is not valid");
                exit(1);
            } else{
                for(int j=0;j<i;j++){
                    if(strcmp(symblTable[j].symbol,token)==0){
                        printf("%s\n","Repetitive Label!");
                        exit(1);
                    }
                }
                strcpy(symblTable[i].symbol,token);
                symblTable[i].value=lineNum;
                i++;
            }
        }
        lineNum++;
    }
    rewind(file);
    free(line);
    return lineNum;
}
void intToHex16(struct instruction * currInstruc){
    char hexTable[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    for(int i=0;i<8;i++){
        currInstruc->instructHex[i]='0';
    }
    if(currInstruc->instType==0){
        currInstruc->instructHex[1]=hexTable[currInstruc->opcode];
        currInstruc->instructHex[2]=hexTable[currInstruc->rs];
        currInstruc->instructHex[3]=hexTable[currInstruc->rt];
        currInstruc->instructHex[4]=hexTable[currInstruc->rd];
    } else if(currInstruc->instType==1){
        if(currInstruc->opcode==11){
            currInstruc->instructHex[3]=hexTable[currInstruc->rs];
            currInstruc->instructHex[2]=hexTable[currInstruc->rt];
        }else{
            currInstruc->instructHex[2]=hexTable[currInstruc->rs];
            currInstruc->instructHex[3]=hexTable[currInstruc->rt];
        }
        currInstruc->instructHex[1]=hexTable[currInstruc->opcode];
        char  offsetHeximal[10];
        int len=decToHexa(currInstruc->imm,offsetHeximal);
        if(len>4){
            printf("%s\n","offset more than 16 bit !");
            exit(1);
        }else{
            int count=7;
            for(int i=0;i<len;i++){
                currInstruc->instructHex[count]=offsetHeximal[i];
                count--;
            }
        }
    }else if(currInstruc->instType==2){
        currInstruc->instructHex[1]=hexTable[currInstruc->opcode];
        if(currInstruc->opcode==13){
            char *offsetHex=NULL;
            int len=decToHexa(currInstruc->imm,&offsetHex);
            int count=7;
            for(int i=0;i<len;i++){
                currInstruc->instructHex[count]=&offsetHex[i];
                count--;
            }
        }
    }
}
int heximaltoint(char * numHex){
    int res=0;
    while(*numHex!='\0'){
        if(((*numHex)>='0') &&((*numHex)<='9')){
            res=res*16+(*numHex)-'0';
        }else if(((*numHex)>='A')&&((*numHex)<='F')){
            res=res*16+(*numHex)-'A'+10;
        }
        numHex++;
    }
    return (res);
}
