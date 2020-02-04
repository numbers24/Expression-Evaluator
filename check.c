#include <stdio.h>
#include <stdlib.h>

void printExpression(int colCount, int expCount, char* expr)
{
    int i=0;
    int x=0;
    int count=0;
    if(expCount-colCount==1)
    {
        while(expr[i]!='\0')
        {
            if(expr[i]==';')
                x=i+1;
            
                i++;
        }
        while(x<=i)
        {
            printf("%c",expr[x]);
            x++;
        }
        printf("\n");
        return;
        
    }
    if(!colCount)
    {
        while(expr[i]!='\0')
        {
            printf("%c",expr[i]);
            i++;
        }
        printf("\n");
    }
    while(expr[i]!='\0')
    {
        if(expr[i]==';')
        {
            count++;
            
            if(count==colCount)
            {
                while(x<=i)
                {
                    printf("%c",expr[x]);
                    x++;
                }
                printf("\n");
                return;
            }
            else
            {
                x=i+1;
            }
        }
        i++;
    }
}
char initialNext(int i, char* token)
{
    while(token[i]!=';'&&token[i]!='\0')
    {
        if(token[i]==' ')
        {
            i++;
            continue;
        }
        
        if(token[i]=='!')
        return '!';
        if(token[i]=='t'||token[i]=='v')
        return 't';
        if(token[i]=='n'||token[i]=='+')
        return 'n';
        i++;
    }
    return '\0';
}
char* tokenize(char expr[])
{
    char* token=(char*)malloc(250*sizeof(char));
    int i=0;
    int j=0;
    /*
    character symbol list:
    
    n: integer of any size digit
    +: arithmatic operator
    t: boolean value
    v: logical operator
    !: negation operator (which is a logical operator, but since it starts first instead of in the middle, it gets its own symbol)
    #: junk
    */
    while(expr[i]!='\0')
    {
        if(expr[i]=='1'||expr[i]=='2'||expr[i]=='3'||expr[i]=='4'||expr[i]=='5'||expr[i]=='6'||expr[i]=='7'||expr[i]=='8'||expr[i]=='9'||expr[i]=='0')
        {
            while(expr[i+1]!=' '&&expr[i+1]!='\0'&&expr[i+1]!=';')//multi=Digit
            {
                i++;
                if(expr[i]!='1'&&expr[i]!='2'&&expr[i]!='3'&&expr[i]!='4'&&expr[i]!='5'&&expr[i]!='6'&&expr[i]!='7'&&expr[i]!='8'&&expr[i]!='9'&&expr[i]!='0')
                token[j]='#';
            }
            if(token[j]!='#')
            token[j]='n';        
        }
        else if((expr[i]=='+'||expr[i]=='-'||expr[i]=='*'||expr[i]=='/')&&(expr[i+1]==' '||expr[i+1]=='\0'||expr[i+1]==';'))
        {
            token[j]='+';
        }
        else if(expr[i]=='t'&&expr[i+1]=='r'&&expr[i+2]=='u'&&expr[i+3]=='e'&&(expr[i+4]==' '||expr[i+4]=='\0'||expr[i+4]==';'))
        {
            i+=3;
            token[j]='t';
        }
        else if(expr[i]=='f'&&expr[i+1]=='a'&&expr[i+2]=='l'&&expr[i+3]=='s'&&expr[i+4]=='e'&&(expr[i+5]==' '||expr[i+5]=='\0'||expr[i+5]==';'))
        {
            i+=4;
            token[j]='t';
        }
        else if(expr[i]=='A'&&expr[i+1]=='N'&&expr[i+2]=='D'&&(expr[i+3]==' '||expr[i+3]=='\0'||expr[i+3]==';'))
        {
            i+=2;
            token[j]='v';
        }
        else if(expr[i]=='N'&&expr[i+1]=='O'&&expr[i+2]=='T'&&(expr[i+3]==' '||expr[i+3]=='\0'||expr[i+3]==';'))
        {
            i+=2;
            token[j]='!';
        }
        else if(expr[i]=='O'&&expr[i+1]=='R'&&(expr[i+2]==' '||expr[i+2]=='\0'||expr[i+2]==';'))
        {
            i+=1;
            token[j]='v';
        }
        else if(expr[i]==' ')
        token[j]='_';
        else if(expr[i]==';')
        token[j]=';';
        else
        {
            token[j]='#';
            while(expr[i+1]!=' '&&expr[i+1]!='\0'&&expr[i+1]!=';') //multiworded
            i++;
        }
        i++;
        j++;
    }
    return token;
}
int checkExpression(int start, int end, int expCount, int colCount, char* token, char* expr)
{
    int pos=0;
    int i=start;
    
    int n=0;
    int op=0;
    int t=0;
    int v=0;
    int neg=0;
    int junk=0;
    int junkOp=0;
    
    //"a_b_c;_a_b_c;..._a_b_c"
    int a=0;
    int b=0;
    int c=0;
    
    int x=0; //if true, current object is an operator
    int y=0; //if ture current object is an operand
    
    int type=0; //holds the type of this expression 0:no type, 1:logical, 2:numerical
    int error=0; //counts errors
    int incomplete=0;
    
    char prev='\0'; //holds the last token
    char next=initialNext(i,token); //what the next expected token will be
    int p=0; //holds the previous position
    
    if(start==end)
    {
        error++;
        printf("Error in expression %d: incomplete expression\n",expCount);
        type = 3;
    }
    if(expCount==0) //for the first expression
    {
    
        if(token[i]=='_')
        {
             error++;
             printf("Error in expression %d: unknown identifier\n",expCount);
             i++;
        }
    }
    else
    {
        if(token[i]!='_')
        {
           if(token[i]=='n'||token[i]=='t')
	   {error++;
            printf("Error in expression %d: unexpected operand\n",expCount);
	   }	
         }
    }
    while(i<=end)
        {
            //printf("%c",token[i]);
            x=0;
            y=0;
            if(token[i]==';')
            {
                if(token[i+1]=='\0')
                {
                    error++;
                    printf("Error in expression %d: incomplete expression\n",expCount+1);        
                }
                if(token[i-1]==' ')
                {
                    error++;
                    printf("Error in expression %d: unended expression\n",expCount);
                }
		break;
            }
            if(token[i]=='_')
            {
                i++;
                if(token[i]=='_')
                {
                    error++;
		    if(prev=='n'||prev=='t')
			{
                    		printf("Error in expression %d: unknown operator\n",expCount);
				if(prev=='t')
				{   if(b)
				    next=';';
				    else
				    next='t';
				}
				if(prev=='n')
				{
					if(c)
		            		next=';';
		            		else
		            		next='n';
				}
			}
			else
			printf("Error in expression %d: unknown identifier\n",expCount);
			
			prev='_';
                }
                continue;
            }
            
            pos++;
            if(token[i]!=next)
            {
                if(token[i]=='n')
                {
                    n++;
                    y=1;
                    if(c)
                    next=';';
                    else
                    next='+';
                }
                if(token[i]=='+')
                {
                    op++;
                    x=1;
                    if(c)
                    next=';';
                    else
                    next='n';
                }
                if(token[i]=='t')
                {
                    t++;
                    y=1;
                    if(b)
                    next=';';
                    else
                    next='v';
                }
                if(token[i]=='v')
                {
                    v++;
                    x=1;
                    if(c)
                    next=';';
                    else
                    next='t';
                }
                if(token[i]=='!')
                {
                    neg++;
                    x=1;
                    if(b)
                    next=';';
                    else
                    next='t';
                }
                if(token[i]=='#')
                {
                    error++;
                    junk++;
                    if(prev=='t' || prev=='n')
                    {
                        junkOp++;
			
			if(prev=='t')
			{   if(b)
		            next=';';
		            else
		            next='t';
			}
			if(prev=='n')
			{
				if(c)
                    		next=';';
                    		else
                    		next='n';
			}
			
			
                        printf("Error in expression %d: unknown operator at token %d\n",expCount,pos);       
                    }
		    else if(prev == '\0' || pos>3 || prev=='#')
                    printf("Error in expression %d: unknown identifier at token %d\n",expCount,pos);
		    else
                    printf("Error in expression %d: unknown operand at token %d\n",expCount,pos);
                }
		
                if(x)
                {
                    if((token[i]=='v'&&prev=='n')||(token[i]=='+'&&prev=='t'))
                    {
                        error++;
                        printf("Error in expression %d: type mismatch between operator and operand at tokens %d and %d\n",expCount,p,pos);
                    }
                    if(token[i]=='!' && (prev=='t' || prev=='n'))
                    {
                        error++;
                        printf("Error in expression %d: unexpected operator at token %d\n",expCount,pos);
                    }
			char temp = token[i];
                    if(prev=='+'||prev=='v'||prev=='!'||prev=='#'||prev=='\0'||prev=='_'||c||(temp='!'&&b))
                    {
                        error++;
                        if(c||(temp=='!'&&b))
                        incomplete=1;
                        printf("Error in expression %d: unexpected operator at token %d\n",expCount,pos);
                    }//for some reason there is a data leak when the algorithm enters this condtional
                }
                if(y)
                {
                    if(prev=='t'||prev=='n'||(prev=='#' && a)||c||(neg&&b))
                    {
                        error++;
                        if(c||(neg&&b))
                        incomplete=1;
                        printf("Error in expression %d: unexpected operand at token %d\n",expCount,pos);
                    }
                    if((token[i]=='n'&&prev=='v')||(token[i]=='t'&&prev=='+'))
                    {
                        error++;
                        printf("Error in expression %d: type mismatch between operator and operand at tokens %d and %d\n",expCount,p,pos);
                    }
                }
            }
            else
                {
                    if(a)
                    {
                        if(b)
                        {
                            if(c)
                            {
                              continue;  
                            }
                            else
                            {
                                c=1;
                                if(token[i]=='n')
                                {
                                    n++;
                                    next=';';
                                }
                                if(token[i]=='t')
                                {
                                    t++;
                                    next=';';
                                }
                            }
                        }
                        else
                        {
                            b=1;
                            if(token[i]=='+')
                            {
                                op++;
                                next='n';
                            }
                            if(token[i]=='v')
                            {
                                v++;
                                next='t';
                            }
                            if(token[i]=='t')
                            {
                                t++;
                                next=';';
                            }
                        }
                    }
                    else
                    {
                        a=1;
                        if(token[i]=='n')
                        {
                            n++;
                            next='+';
                        }
                        if(token[i]=='t')
                        {
                            t++;
                            next='v';
                        }
                        if(token[i]=='!')
                        {
                            neg++;
                            next='t';
                        }
                    }
                }
            p=pos;
            prev=token[i];
            i++;
        }
    if(!incomplete&&(type!=end)&&(prev=='!'||prev=='+'||prev=='v'||(junkOp && prev=='#'&&(next=='v'||next=='+'))))
    {
        error++;
        printf("Error in expression %d: missing operand after token %d\n",expCount,p);
	type=3;
    }
    if(!op&&!v&&!neg&&!junkOp&&pos>0)
    {
        error++;
        printf("Error in expression %d: missing operator\n",expCount);
	type=3;
    }
    if(incomplete)
    {
        error++;
        printf("Error in expression %d: unended expression\n",expCount);
    }
    if(junk&&!op&&!v&&!t&&!n&&!neg)
    {
        error++;
        printf("Error in expression %d: incomplete expression\n",expCount);
	type==3;
    }
    
    if(error)
    {
        printExpression(colCount,expCount,expr);
        printf("Number of errors: %d\n",error);
    }
	
    
    if(type==3)
    return 0;
    if(!n&&!op&&t&&(v||neg)&&!junk)
    return 1;
    if(n&&op&&!t&&!v&&!neg&&!junk)
    return 2;
    
    return 0;
}

int main(int argc, char** argv)
{
    //Check to make sure argc has only one argument
    if(argc!=2)
    return 0;
    
    char* expr= argv[1];
    
    int i=0;
    int x=0;
    int expCount=0; //counts the amount of expressions. starts at 1 bc having an ; implies that there are at least 2
    int colCount=0;//counts the amount of ;'s
    
    int lCount=0; //counts the number of logical expressions
    int aCount=0;//counts the number of arithmatic expressions
    
    //expCount should always be one greater than colCount
    
    char* token=tokenize(expr); //simplifies the tokens to be one char symbols
    if(token[i]=='\0')
    {
        printf("Error in expression 0: incomplete expression\n");
        return 0;
    }
    while(token[i]!='\0')
    {
        if(token[i]==';' || token[i+1]=='\0')
        {
            if(token[i]==';')
            colCount++;
           int temp = checkExpression(x,i,expCount,colCount,token,expr);
           {
               if(temp==1)
               lCount++;
               if(temp==2)
               aCount++;
           }
	   expCount++;

           x=i+1;
        }
        i++;
    }
   
    printf("Found %d expressions: %d logical and %d arithmatic\n", colCount+1, lCount, aCount);
	if(expCount==lCount+aCount) //if the the sum of logic and arithmatic expressions isn't equal to total count, then there has to be one expression with an error
	printf("Okay");
    return 0;
}




