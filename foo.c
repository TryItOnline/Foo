/* foo.c
 * An interpreter for the Foo esoteric programming language
 * Written by Feky, 2008 */
 
 #include <ctype.h>
 #include <stdio.h>
 #include <string.h>
 #include <unistd.h>
 
 #define ARRAY_SZ 1024
 #define STACK_SZ 32
 #define MAX_NEST_LOOPS 8
 #define MAX_LINE 255
 
 #define ASCII_ZERO 48
  
 typedef unsigned short int USHORT;
 
 struct _stack {
	 USHORT data[STACK_SZ];
	 int top;
 };
 
 struct _loop {
	 USHORT val;
	 char *p;
 };

 USHORT getnum (char *); /* something like atoi(), but it's my own ^^ */
 int push (USHORT, struct _stack *);
 USHORT pop (struct _stack *);
	 
 USHORT getnum (char *p) {
	 USHORT y = 0;
	 
	 while(isdigit(*p)) {
		 y *= 10;
		 y += *p - ASCII_ZERO;
		 p++;
	 }
	 
	 return y;
 }
 
 int push (USHORT val, struct _stack *stack) {
	 
	if(stack->top == STACK_SZ) {
		fputs("Stack is full.\n", stderr);
		return -1;
	}
	
	stack->data[stack->top++] = val;
		 
	return 0;
}

 USHORT pop (struct _stack *stack) {
	 
	 if(stack->top <= 0) {
		 fputs("Stack is empty.\n", stderr);
		 return 0;
	 }
	 
	 stack->top--;
	 
	 return stack->data[stack->top];
 }
 
 int main (int argc, char **argv) {
	FILE *f;
	int ptr = 0;
	USHORT array[ARRAY_SZ];
	struct _stack stack;
	char line[MAX_LINE];
	char *p;
	USHORT div;
	int lc = 0;
	struct _loop loops[MAX_NEST_LOOPS];
	 
	 if(argc != 2) {
		 fprintf(stderr, "Usage: %s <file>\n", argv[0]);
		 return -1;
	 }
	 
	 f = fopen(argv[1], "r");
	 
	 if(f == NULL) {
		 perror("fopen");
		 return -1;
	 }
	 
	 memset(array, 0, sizeof array);
	 stack.top = 0;
	 
	 while(fgets(line, MAX_LINE, f) != NULL) {
		 p = line;
		 while(*p != '\0') {
			 switch(*p) {
				 case '"':
					 p++;
					 while(*p != '"' && *p != '\0') {
						 putchar(*p);
						 p++;
					 }
					 if(*p != '\0')
						 p++;
				 break;
					 
				 case '@':
					 p++;
				         if(!isdigit(*p)) {
						 if(push(array[ptr], &stack)){
							 fclose(f);
						         return -1;
						 }
					 }
					 else {
						 if(push(getnum(p), &stack)) {
							 fclose(f);
						         return -1;
						 }
					 }
				 break;
					 
				 case '<':
					 p++;
					 if(ptr == 0)
						 ptr = STACK_SZ;
					 else ptr--;
				 break;
				 
				 case '>':
					 p++;
					 if(ptr == STACK_SZ)
						 ptr = 0;
					 else ptr++;
				 break;
					 
				 case '&':
					 p++;
				         if(!isdigit(*p))
						 array[ptr] = pop(&stack);
					 else array[ptr] = getnum(p);
				break;
					 
				case '$':
					p++;
				        switch(*p) {
						case 'i':
							p++;
							if(!isdigit(*p))
								printf("%d", array[ptr]);
							else printf("%d", getnum(p));
						break;
							
						case 'h':
							p++;
							if(!isdigit(*p))
								printf("%x", array[ptr]);
							else printf("%x", getnum(p));
						break;
							
						case 'c':
							p++;
							if(!isdigit(*p))
								putchar(array[ptr]%256);
							else putchar(getnum(p)%256);
						break;
							
						default:
							fputs("Unknown mode for '$'\n", stderr);
					        break;
					}
				break;
					 
				case '+':
					p++;
				        if(!isdigit(*p))
						 array[ptr] += pop(&stack);
					 else array[ptr] += getnum(p);
				break;
					 
				case '-':
					p++;
				        if(!isdigit(*p))
						 array[ptr] -= pop(&stack);
					 else array[ptr] -= getnum(p);
				break;
					 
				case '*':
					p++;
				        if(!isdigit(*p))
						 array[ptr] *= pop(&stack);
					 else array[ptr] *= getnum(p);
				break;
					 
				case '/':
					p++;
				        if(!isdigit(*p))
						div = pop(&stack);
					 else div = getnum(p);
				        if(div != 0)
						array[ptr] /= div;
					else fputs("Only Chuck Norris can divide by zero.\n", stderr);
				break;
					
				case '%':
					p++;
				        if(!isdigit(*p))
						div = pop(&stack);
					 else div = getnum(p);
				        if(div != 0)
						array[ptr] %= div;
					else fputs("Only Chuck Norris can divide by zero.\n", stderr);
				break;
					
			        case '#':
					p++;
				        if(!isdigit(*p))
						 sleep(pop(&stack));
					 else sleep(getnum(p));
				break;        
					 
				case '(':
					p++;
				        if(lc == MAX_NEST_LOOPS) {
						fputs("Maximum number of nested loops reached.\n", stderr);
						fclose(f);
						return -1;
					}
					if(!isdigit(*p))
						loops[lc].val = pop(&stack);
					else loops[lc].val = getnum(p);
					loops[lc].p = p;
					lc++;
				break;
					
				case ')':
					p++;
					if(lc > 0) {
						if(array[ptr] != loops[lc - 1].val)
							p = loops[lc - 1].p;
						else lc--;
					}
				break;
					 
				default:
					p++;
				break;
			}			
		 }
	 }
	 
	 fclose(f);
	 return 0;
 }
