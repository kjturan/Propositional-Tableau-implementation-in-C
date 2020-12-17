#include <stdio.h> 
#include <string.h>   /* for all the new-fangled string functions */
#include <stdlib.h>     /* malloc, free, rand */

int Fsize=50; /*maximum formula length*/
int inputs =20;/* number of formulas expected in input.txt*/
int i;/* in case you need it */
int ThSize=100;/* maximum size of set of formulas*/
int TabSize=500; /*maximum length of tableau queue*/

void printString(char* string) {
    while(*string) {
        printf("%c",*string);
        string++;
    }
    printf("\n");
}

void prepend(char* s, const char* t) {
    size_t len = strlen(t);
    size_t i;
    memmove(s + len, s, strlen(s) + 1);
    for (i = 0; i < len; ++i)
    {
        s[i] = t[i];
    }
}

char* getSubstring(char *source, int start, int end) {
    char* s = (char*)malloc(strlen(source)*sizeof(char));
    s = source;
    char* p = (char*)malloc(strlen(s)*sizeof(char) + 1);
    char* d = (char*)malloc(strlen(s)*sizeof(char) + 1);
    p = d;
    if(end == -1) {end = strlen(s);}
    if(start == -1) {start = 0;}
    int j;
    int i;
    for(j = 0; j < start; j++) { s++; }
    for(i = 0; i < end - start; i++)
    {
        *d = *s;
        d++;
        s++;
    }
    *d = '\0';
    return p;
}

char* copyString(char* srcString) {
    char* destString = (char*)malloc(strlen(srcString)*sizeof(char) + 1);
    memcpy(destString,srcString,strlen(srcString)+1);
    return destString;
}

int findConnectiveIndex(char* g) {
    int index = 0;
    int pCount = 0;
    while(*g) {
        if(*g == '(') { 
            pCount++; 
        }
        else if(*g == ')') { 
            pCount--;
        }
        else if(pCount == 1) {
            if(*g == 'v' || *g == '>' || *g == '^'){
                return index;
            }
        }
        g++;
        index++;
    }
    return -1;
}

int checkFormula(char* g) {
    //printString(g);
    if(strlen(g) == 1) {
        if(*g == 'p' || *g == 'q' || *g == 'r') {
            return 1;
        }
    }
    else if(*g == '-') {
        g++;
        if(checkFormula(g) == 1) {return 1;}
    }
    else if(*g == '(') {
        int connectiveIndex = findConnectiveIndex(g);
        if(connectiveIndex != -1)
        {
            char* fHalf = (char*)malloc(sizeof(char)*connectiveIndex);
            char* sHalf = (char*)malloc(sizeof(char)*(strlen(g)-connectiveIndex));
            fHalf = getSubstring(g,1,connectiveIndex);
            sHalf = getSubstring(g,connectiveIndex+1,strlen(g)-1);

            if(checkFormula(fHalf) == 1 && checkFormula(sHalf) == 1) {
                return 1;
            }
        }
    }
    return 0;
}

char *partone(char *g) {
    int connectiveIndex = findConnectiveIndex(g);
    char* fHalf = (char*)malloc(sizeof(char)*connectiveIndex);
    fHalf = copyString(getSubstring(g,1,connectiveIndex));
    return fHalf;
}

char *parttwo(char *g) {
    int connectiveIndex = findConnectiveIndex(g);
    char* sHalf = (char*)malloc(sizeof(char)*(strlen(g)-connectiveIndex));
    sHalf = copyString(getSubstring(g,connectiveIndex+1,strlen(g)-1));
    return sHalf;
}

int parse(char *g) {
    if(checkFormula(g) != 1) {return 0;}
    if(*g == '-') {return 2;} 
    if(*g == 'p' | *g == 'q' | *g == 'r') {return 1;} //checks if it's a proposition
    if(*g == '(') { return 3;}
    return 0;
}

struct set {
    char* item;/*first word of non-empty set*/
    struct set* tail;/*remaining words in the set*/
};

struct tableau {
  struct set* S; /* pointer to first set in non-empty list */
  struct tableau* rest; /*list of pointers to other sets*/
};

void printSet(struct set* set) {
    struct set* p = (struct set*)malloc(sizeof(struct set));
    p = set;

    if(p->item == NULL) {printf("no elements in set");}
    else
    {
        while(p->tail != NULL)
        {
            printf("{%s},",p->item);
            p = p->tail;
        }
        printf("{%s},",p->item);
    }
    printf("{}");
}

void printTab(struct tableau* tab) {
    struct tableau* p = (struct tableau*)malloc(sizeof(struct tableau));
    p = tab;

    if(p->S == NULL) {printf("no elements in set");}
    else
    {
        printf("");
        while(p->rest != NULL)
        {
            printf("");
            printf("[");
            printSet(p->S);
            printf("],");
            p = p->rest;
        }
        printf("[");
        printSet(p->S);
        printf("],");
    }
    printf("[]\n");
}

int sizeofSet(struct set set) {
    int count = 0;
    if(!set.item) {return count;}
    else
    {
        while(set.tail != NULL)
        {
            count++;
            set = *(set.tail);
        }
        count++;
    }
    return count;
}

struct set* copySet(struct set *dest, struct set *src) {  
    struct set* p = (struct set*)malloc(sizeof(struct set*));
    p = dest;
    if(src->item == NULL) {printf("copyset: empty set error \n");}
    else 
    {
        while(src->tail != NULL) //while the set has another set following
        {
            char *item = copyString(src->item);
            struct set* temp = (struct set*)malloc(sizeof(struct set*));
            //temp = src->tail;
            p->item = item;
            p->tail = temp;
            p = p->tail;
            src = src->tail;
        }
        char *item = copyString(src->item);
        p->item = item;
        p->tail = NULL;
    }
    return dest;
}

struct tableau* addTabToTab(struct tableau *tab, struct tableau *newTab) {
    struct tableau* p = (struct tableau*)malloc(sizeof(struct tableau));
    p = tab;
    struct tableau* n = (struct tableau*)malloc(sizeof(struct tableau));
    n = newTab;
    struct tableau* s = (struct tableau*)malloc(sizeof(struct tableau));
    s = p;
    if(p->S == NULL) {printf("ASTS: empty set error \n");}
    else 
    {
        while(p->rest != NULL) //while the set has another set following
        {
            p = p->rest; 
        }
        p->rest = n;
    }
    //s = s->rest;
    return s;
}

struct set* addSetToSet(struct set *existing, struct set *newSet) {
    struct set* s = (struct set*)malloc(sizeof(struct set));
    s = existing;
    if(s->item == NULL) {printf("ASTS: empty set error \n");}
    else 
    {
        while(s->tail != NULL) //while the set has another set following
        {
            s = s->tail; 
        }
        s->tail = newSet;
    }
    //s = s->tail;
    return existing;
}

struct tableau* push(struct set *set, struct tableau *tab) {
    struct tableau *p = (struct tableau*)malloc(sizeof(struct tableau));
    p = tab;
    struct set *s = (struct set*)malloc(sizeof(struct set));
    s = set;
    s = addSetToSet(p->S,s);
    p->S = s->tail;
    return p;
}

struct tableau* splitSet(struct tableau *tab, char *string) {
    struct tableau *p = tab;
    struct set *p1 = (struct set*)malloc(sizeof(struct set));
    struct set *p2 = (struct set*)malloc(sizeof(struct set));
    p2->item = parttwo(string);
    p2->tail = NULL;
    p1->item = partone(string);
    p1->tail = p2;
    tab = push(p1,tab);
    //tab->S = tab->S->tail;
    return p;
}

struct tableau* splitTableau(struct tableau *tab, char* string) {
    struct tableau *p = tab;

    struct set *p1 = (struct set*)malloc(sizeof(struct set));
    p1->item = copyString(partone(string));
    p1->tail = NULL;

    struct set *p2 = (struct set*)malloc(sizeof(struct set));
    p2->item = copyString(parttwo(string));
    p2->tail = NULL;

    struct set* s1 = (struct set*)malloc(sizeof(struct set));
    s1 = copySet(s1,p->S);
    struct set* s2 = (struct set*)malloc(sizeof(struct set));
    s2 = copySet(s2,p->S);

    s1 = addSetToSet(s1,p1);
    s2 = addSetToSet(s2,p2);

    struct tableau *t2 = (struct tableau*)malloc(sizeof(struct tableau));
    t2->S = s2;
    t2->rest = NULL;
    //t2 = push(p2,t2);
    t2->S = t2->S->tail;
    struct tableau *t1 = (struct tableau*)malloc(sizeof(struct tableau));
    t1->S = s1;
    t1->rest = t2;
    //t1 = push(p1,t1);
    t1->S = t1->S->tail;

    p = addTabToTab(p,t1);
    p = p->rest;
    //printTab(p);
    return p;
}

char* getImplication(char* str)
{
    char* s = (char*)malloc(sizeof(char)*(strlen(str)+5));
    strcat(s,"(-");
    strcat(s,partone(str));
    strcat(s,"v");
    strcat(s,parttwo(str));
    strcat(s,")\0");
    return s;
}

struct tableau* enqueue(struct set *set, struct tableau *tab) {
    char *string = set->item;
    struct tableau *p = tab;
    char connective = *(string + findConnectiveIndex(string)); 
    if(connective == '^') //if the foruma is an alpha formula
    {
        p = splitSet(tab,string);
    }
    else if(connective == 'v') 
    {
        p = splitTableau(tab,string);
    }
    else if(connective == '>')
    {
        p = splitTableau(tab,getImplication(string));
    }
    return p;
}

int countNegs(char *string) {
    int negCount = 0;
    while(*string == '-') {
        negCount++;
        string++;
    }
    return negCount;
}

char* noNeg(char *string) {
    char *str = (char*)malloc(sizeof(char)*strlen(string)+1);
    str = copyString(getSubstring(string,countNegs(string),-1));
    return str;
}

char* oneNeg(char *string) {
    char *str = (char*)malloc(sizeof(char)*strlen(string)+1);
    str = copyString(getSubstring(string,countNegs(string)-1,-1));
    return str;
}

char* getNegatedFormula(char *s) {
    char* negatedString = (char*)malloc(sizeof(char)*(strlen(s)+5));
    //char *temp = (char*)malloc(sizeof(char)*(strlen(s)+5));
    //temp = negatedString;

    char *nns = (char*)malloc(sizeof(char)*(strlen(s)+1));
    nns = copyString(noNeg(s));

    char connective = *(nns + findConnectiveIndex(nns));

    if(connective == '>'){nns = copyString(getImplication(nns));}

    char* p1 = (char*)malloc(sizeof(char)*(strlen(s)+1));
    strcpy(p1,partone(nns));
    char* p2 = (char*)malloc(sizeof(char)*(strlen(s)+1));
    strcpy(p2,parttwo(nns));

    connective = *(nns + findConnectiveIndex(nns));

    char* con = (char*)malloc(sizeof(char)*2);
    if(connective == '^') {con = "v";}
    else if(connective == 'v') {con = "^";}

    strcpy(negatedString,"(-");
    strcat(negatedString,p1);
    strcat(negatedString,con);
    strcat(negatedString,"-");
    strcat(negatedString,p2);
    strcat(negatedString,")\0");

    return negatedString;
}

struct tableau* negate(struct set *set, struct tableau *tab) {   
    struct tableau *t = (struct tableau*)malloc(sizeof(struct tableau));
    t = tab;
    struct set *s = (struct set*)malloc(sizeof(struct set));
    s = set;

    char *n = (char*)malloc(sizeof(char)*strlen(s->item)+3);
    n = copyString(noNeg(s->item));

    char* nForm = (char*)malloc(sizeof(char)*(strlen(n)+1));
    strcpy(nForm,"");
    if(countNegs(s->item) % 2 == 0){nForm = copyString(noNeg(s->item));}
    else
    {
        if(parse(n) == 1)
        {
            strcat(nForm,"-");
            strcat(nForm,noNeg(s->item));
            strcat(nForm,"\0");
        } 
        else if(parse(n) == 3)
        {
            char *temp = (char*)malloc(sizeof(char)*strlen(s->item)+3);
            temp = getNegatedFormula(n);
            strcat(nForm,temp);
        }
    }
    struct set *new = (struct set*)malloc(sizeof(struct set));
    new->item = nForm;
    new->tail = NULL;
    t = push(new,t);
    return t;
}

int expanded(struct set* set) {
    struct set *s = (struct set*)malloc(sizeof(struct set));
    s = set;
    while(s->item != NULL)
    {
        char *str = (char*)malloc(sizeof(char)*strlen(s->item));
        str = getSubstring(s->item,countNegs(s->item),-1);
        if(parse(str) != 1) {return -1;}
        if(s->tail == NULL) {break;}
        s = s->tail;
    }
    return 1;
}

int contains(struct set *set, char *str) {
    struct set *s = (struct set*)malloc(sizeof(struct set));
    s = set;
    while(s != NULL)
    {
        char* negated = oneNeg(s->item);
        if(strcmp(negated,str) == 0) {return 1;}
        if(s->tail == NULL) {break;}
        s = s->tail;
    }
    return -1;
}

int contradictory(struct set *set) {
    struct set *s = (struct set*)malloc(sizeof(struct set));
    s = set;
    
    if(contains(s,"p") == 1 && contains(s,"-p") == 1) {return 1;}
    else if(contains(s,"q") == 1 && contains(s,"-q") == 1) {return 1;}
    else if(contains(s,"r") == 1 && contains(s,"-r") == 1) {return 1;}
    else {return -1;}
}

struct tableau* pushProp(char *str,struct tableau* tab)
{
    struct tableau *t = (struct tableau*)malloc(sizeof(struct tableau));
    t = tab;

    struct set *propSet = (struct set*)malloc(sizeof(struct set));
    propSet->item = copyString(str);
    propSet->tail = NULL;
    t->S = addSetToSet(t->S,propSet);
    //t->S = t->S->tail;
    return t;
}

int closed(struct tableau* tab) {
    struct tableau *t = (struct tableau*)malloc(sizeof(struct tableau));
    t = tab;

    while(t->S != NULL)
    {
        struct set *s = (struct set*)malloc(sizeof(struct set));
        s = t->S;
        while(s->item != NULL)
        {
            //printf("current tab: ");
            printTab(t);
            if(contradictory(s) == 1)
            {
                //printf("contradiction!\n");
                if(t->rest != NULL) {t = t->rest; s = t->S;}
                else {return 0;}
            }
            else if(expanded(s) == 1)
            {
                return 1;
            }
            else
            {
                switch(parse(s->item))
                {
                    case(0): return -1; //not a formula
                    case(1): t = pushProp(s->item,t);//pushes to the back of the queue
                    case(2): t = negate(s,t); s = t->S; continue;
                    case(3): t = enqueue(s,t); //a and b rules
                }
                s = t->S; 
            }
        }
    }
    return 0;
}

/*You should not need to alter the program below.*/
int main()

{ /*input 10 strings from "input.txt" */


  char *name = malloc(Fsize);
  FILE *fp, *fpout;
 
  /* reads from input.txt, writes to output.txt*/
  if ((  fp=fopen("input.txt","r"))==NULL){printf("Error opening file");exit(1);}
  if ((  fpout=fopen("output.txt","w"))==NULL){printf("Error opening file");exit(1);}

  int j;
  for(j=0;j<inputs;j++)
  {
    fscanf(fp, "%s",name);/*read formula*/
    switch (parse(name))
    {
        case(0): fprintf(fpout, "%s is not a formula.  \n", name);break;
        case(1): fprintf(fpout, "%s is a proposition. \n ", name);break;
        case(2): fprintf(fpout, "%s is a negation.  \n", name);break;
        case(3):fprintf(fpout, "%s is a binary. The first part is %s and the second part is %s  \n", name, partone(name), parttwo(name));break;
        default:fprintf(fpout, "What the f***!  ");     
    }

    struct set S={name, NULL};
    struct tableau t={&S, NULL};

    if (parse(name) != 0) 
    {   
      //complete(&t);
	    if (closed(&t) == 0)  fprintf(fpout, "%s is not satisfiable.\n", name);
	    else fprintf(fpout, "%s is satisfiable.\n", name);
	}
    else  fprintf(fpout, "I told you, %s is not a formula.\n", name);
  }

 
  fclose(fp);
  fclose(fpout);
  free(name);

  return(0);
}




