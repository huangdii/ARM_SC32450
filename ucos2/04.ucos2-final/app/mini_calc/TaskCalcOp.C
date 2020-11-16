/*******************************************************************************
**	File:
**
**		TaskCalcOp.c
**
**  Contents:
**		uC/OS-II programming
**
**	History:
**		Date        Name              Description
**		------------------------------------------------------------------------
**
*******************************************************************************/
#include "includes.h"
#include "calc.h"
#include "ui.h"

void VLcd_Draw_BUTTON(int button);

// 
// VIRTUAL 좌표 계산
// : VLcd_printf() 에서만 사용함
#ifdef _LCD_480x272
#define VAXIS_X		(850-(LCD_XSIZE-480)/2)
#define VAXIS_Y		(60-(LCD_YSIZE-272)/2)
#else
#define VAXIS_X		(600-(LCD_XSIZE-320)/2)
#define VAXIS_Y		(43-(LCD_YSIZE-240)/2)
#endif

// 메시지 큐 'LCD'
extern OS_EVENT *qLCD_id;	/* Message Queue Pointer */

/* Stack methods.  */
int     push_value  (value_stack_t* stack, value_t value);
value_t pop_value   (value_stack_t* stack);
int     operation   (value_stack_t* stack, enum op_type op);
void    print_value (value_stack_t* stack, print_mode mode, int which);
int     calc_loop   (value_stack_t* stack);

/* Implementation of some libc methods. */
static char* hex_convert(char* buf, value_t value);
static char* dec_convert(char* buf, value_t value);
static int get_value(const char* buf, value_t* v);
static op_type calc_wait_command(value_t* v);
static void print_help(void);

/* List of commands with a description string. */
static const command commands[] = {
  {
    "add",
    OP_ADD,
    "Pop two values and push their sum"
  } , {
    "sub",
    OP_SUB,
    "Pop two values and push their subtraction"
  } , {
    "mul",
    OP_MUL,
    "Pop two values and push their mul"
  } , {    
    "div",
    OP_DIV,
    "Divide the two values"
  } , {
    "mod",
    OP_MOD
  } , {
    "and",
    OP_AND,
    "Logical and between the two values"
  } , {
    "or",
    OP_OR,
    "Logical or between the two values"
  } , {
    "xor",
    OP_XOR
  } , {
    "not",
    OP_NOT
  } , {
    "neg",
    OP_NEG
  } , {
    "sqrt",
    OP_SQRT,
    "Square root of the value"
  } , {
    "quit",
    OP_QUIT,
    "Quit the calculator"
  } , {
    "list",
    OP_LIST,
    "List the stack"
  } , {
    "help",
    OP_HELP,
    "This help"
  } , {
    "dec",
    OP_DEC,
    "Switch in decimal mode"
  } , {
    "hex",
    OP_HEX,
    "Switch in hexadecimal mode"
  } , {
    0,
    0,
    0
  }
};

// 작동 코드
int opcode;

// 광역 키코드 버퍼
extern int KeyCode;

// 세마포어
extern OS_EVENT *TOUCH_SEM;
extern OS_EVENT *LCD_SEM;
extern OS_EVENT *INPUT_SEM;

//
// 태스크 'TaskCalcOp'
//
void TaskCalcOp(void *pdata)
{
	value_t val_table[10];
	value_stack_t stack;
	op_type op;
	int result;
	value_t value;
	int i;
  
	pdata = pdata;				/* 컴파일러 경고를 막기 위함 */

	// stack 구조체 초기화
	stack.top    = 0;
	stack.values = val_table;
	stack.max    = 10;
	stack.mode   = PRINT_DEC;

	print_help ();
	while (1)
	{
        print_value (&stack, stack.mode, -1);

		// 계산기 스택 전체 디스플레이
//		for (i = 0; i < stack.top; i++)
//			print_value (&stack, stack.mode, i);
	  
		op = calc_wait_command (&value);
		switch (op)
		{
			case OP_QUIT:
				Uart_Printf("[OP_QUIT]\n");
				break;

			case OP_NUMBER:
				Uart_Printf("[OP_NUMBER]\n");
				result = push_value (&stack, value);
				if (result != 0)
				{
					Uart_Printf("The stack is full.\n");
				}
				break;

			case OP_DEC:
				Uart_Printf("[OP_DEC]\n");
				stack.mode = PRINT_DEC;
				break;

			case OP_HEX:
				Uart_Printf("[OP_HEX]\n");
				stack.mode = PRINT_HEX;
				break;

			case OP_LIST:
				Uart_Printf("[OP_LIST]\n");
				for (i = 0; i < stack.top; i++)
				print_value (&stack, stack.mode, i);
				break;
	  
			case OP_HELP:
				Uart_Printf("[OP_HELP]\n");
				print_help ();
				break;
	  
			default:
				if(op & OP_NUMBER)
				{
					Uart_Printf("[op & OP_NUMBER]\n");
					result = push_value (&stack, value);
					if (result != 0)
					{
					  Uart_Printf("The stack is full.\n");
					}
					result = operation (&stack, op-OP_NUMBER);
				}
				else
				{
					Uart_Printf("[OP_CODE]\n");
					result = operation (&stack, op);
				}
				break;
		}
   }
}

/* Implementation of some libc methods. */
int
strcmp (const char* s1, const char* s2)
{
  while (*s1 && (*s1 == *s2))
    s1++, s2++;

  return *s1 - *s2;
}

static char*
hex_convert(char* buf, value_t value)
{
  char num[32];
  int pos;
  
  *buf++ = '0';
  *buf++ = 'x';

  pos = 0;
  while (value != 0)
    {
      char c = value & 0x0F;
      num[pos++] = "0123456789ABCDEF"[(unsigned) c];
      value = (value >> 4) & HEX_CVT_MASK;
    }
  if (pos == 0)
    num[pos++] = '0';

  while (--pos >= 0)
    *buf++ = num[pos];
  
  *buf = 0;
  return buf;
}

static char*
dec_convert(char* buf, value_t value)
{
  char num[20];
  int pos;

  pos = 0;
  if (value < 0)
    {
      *buf++ = '-';
      value = -value;
    }
  while (value != 0)
    {
      char c = value % 10;
      value = value / 10;
      num[pos++] = c + '0';
    }
  if (pos == 0)
    num[pos++] = '0';

  while (--pos >= 0)
    {
      *buf = num[pos];
      buf++;
    }
  *buf = 0;
  return buf;
}

/* A very simple sprintf. It only recognizes %d and %x.
   The parameter MUST be of type 'value_t'. Otherwise, you will not get
   the expected result! */
int
sprintf (char* buf, const char* pattern, ...)
{
  va_list argp;
  char* p = buf;
  char c;
  
  va_start (argp, pattern);
  while ((c = *pattern++) != 0)
    {
      if (c != '%')
	{
	  *p++ = c;
	}
      else
	{
	  value_t v;

	  c = *pattern++;
          if (c == 'l')
            c = *pattern++;
          
	  switch (c)
	    {
	    case 'b':
	    case 'o':
	    case 'x':
	      v = va_arg (argp, vavalue_t);
	      p = hex_convert (p, v);
	      break;

	    case 'd':
	      v = va_arg (argp, vavalue_t);
	      p = dec_convert (p, v);
	      break;

	    default:
	      *p++ = '%';
	      *p++ = c;
	      break;
	    }
	}
    }
  va_end (argp);
  *p++ = 0;
  return (int) (p - buf);
}

/* Push a new value on the stack. Returns 0 if this succeeded and -1 if
   the stack is full. */
int
push_value(value_stack_t* stack, value_t v)
{
  if (stack->top >= stack->max)
    return -1;

  stack->values[stack->top++] = v;
  return 0;
}

/* Pop a value from the stack. If the stack is empty, returns 0. */
value_t
pop_value(value_stack_t* stack)
{
  if (stack->top == 0)
    return 0;

  return stack->values[--stack->top];
}

/* Do some logical or arithmetic operation with top-most values of the
   stack. */
int
operation(value_stack_t* stack, enum op_type op)
{
  int result;

  switch (op)
    {
    case POS_RESULT: // '='
      calc(stack);
            Uart_Printf("POS_RESULT operation.\n");
	  opcode = 0; // opcode reset to '0'
      break;

    case POS_C: // 'C'
    case POS_CE: // 'CE'
		while(pop_value(stack));
		opcode = 0; // opcode reset to '0'
		VLcd_Printf(VAXIS_X-(32*8),VAXIS_Y, BLACK, WHITE, 1,1,"                                0");
		break;

    case POS_PLUS: // OP_ADD
		if(opcode)
	      calc(stack);
		opcode = POS_PLUS;
      break;

    case POS_MINUS:	// OP_SUB
		if(opcode)
	      calc(stack);
      opcode = POS_MINUS;
      break;

    case POS_MUL:	// OP_MUL
		if(opcode)
	      calc(stack);
      opcode = POS_MUL;
      break;

    case POS_DIV:	// OP_DIV
		if(opcode)
	      calc(stack);
      opcode = POS_DIV;
      break;

    case POS_MODULA:	// OP_MOD
		if(opcode)
	      calc(stack);
      opcode = POS_MODULA;
      break;

    case OP_AND:
      opcode = OP_AND;
      break;

    case OP_OR:
      opcode = OP_OR;
      break;

    case OP_XOR:
      opcode = OP_XOR;
      break;

    case OP_NOT:
		if(opcode)
	      calc(stack);
      opcode = OP_NOT;
      break;

    case POS_PLUSMINUS:	// OP_NEG
		if(opcode)
	      calc(stack);
      opcode = POS_PLUSMINUS;
      break;

    case POS_SQRT:	// OP_SQRT
		if(opcode)
	      calc(stack);
      opcode = POS_SQRT;
      break;
      
    default:
      result = 0;
      break;
    }
  return result;
}

//
// 계산기 연산처리
//
int
calc(value_stack_t* stack)
{
	int result;
	value_t value, tvalue;
	char buf[32];
	
  switch (opcode)
    {
    case POS_PLUS: // OP_ADD
      result = push_value (stack, value = pop_value (stack) + pop_value (stack));
      break;

    case POS_MINUS:	// OP_SUB
		tvalue = pop_value (stack);
      result = push_value (stack, value = pop_value (stack) - tvalue);
      break;

    case POS_MUL:	// OP_MUL
      result = push_value (stack, value = pop_value (stack) * pop_value (stack));
      break;

    case POS_DIV:	// OP_DIV
		tvalue = pop_value (stack);
		if(tvalue == 0)
		{
			result = push_value (stack, value = pop_value (stack) / 1);
			Uart_Printf("Warning! Divide by zero\n");
		}
		else
			result = push_value (stack, value = pop_value (stack) / tvalue);
      break;

    case POS_MODULA:	// OP_MOD
		tvalue = pop_value (stack);
      result = push_value (stack, value = pop_value (stack) % tvalue);
      break;

    case OP_AND:
      result = push_value (stack, value = pop_value (stack) & pop_value (stack));
      break;

    case OP_OR:
      result = push_value (stack, value = pop_value (stack) | pop_value (stack));
      break;

    case OP_XOR:
      result = push_value (stack, value = pop_value (stack) ^ pop_value (stack));
      break;

    case OP_NOT:
      result = push_value (stack, value = ~pop_value (stack));
      break;

    case POS_PLUSMINUS:	// OP_NEG
      result = push_value (stack, value = -pop_value (stack));
      break;

    case POS_SQRT:	// OP_SQRT
      result = push_value (stack, value = calc_sqrt (pop_value (stack)));
      break;
      
    default:
      result = 0;
	  return result;
      break;
    }

	VLcd_Printf(VAXIS_X-(32*8),VAXIS_Y, BLACK, WHITE, 1,1,"                                ");
	sprintf (buf, "%ld", value);
	VLcd_Printf(VAXIS_X-((strlen(buf)-1)*8),VAXIS_Y, BLACK, WHITE, 1,1,buf);

  return result;
}

void
print_value(value_stack_t* stack, print_mode mode, int which)
{
  char buf[40];
  value_t value;

  static const char* const print_formats[] = {
    " %d\r\n",
    " %x\r\n",
    " %o\r\n",
    " %b\r\n"
  };

  /* Print the top of the stack if no index is specified. */
  if (which < 0)
    {
      which = stack->top - 1;
      if (which < 0)
	{
	  printf ("Stack is empty\n");
	  return;
	}
      /* Note: we have to cast the value to 'vavalue_t' because the
	 basic sprintf implementation is hard coded with it. If we
	 are compiled with -mshort and using a long or long long,
	 we won't get the expected result... */
      sprintf (buf, "Top (%ld) = ", (vavalue_t) stack->top);
      printf (buf);
    }
  else
    {
      sprintf (buf, "[%ld] = ", (vavalue_t) which);
      printf (buf);
    }
  
  if (which >= 0 && which < stack->top)
    value = stack->values[which];
  else
    value = 0;

  sprintf (buf, print_formats[mode], value);
  Uart_Printf(buf);
}

/* Try to translate a string into a number. We look first for hexadecimal
   format, octal and then decimal. If the string could be converted, the
   value is returned in `v' and the function returns 0. Otherwise, it
   returns -1. */
static int
get_value(const char* buf, value_t* v)
{
  value_t value = 0;
  char c;
  
  if (!(*buf >= '0' && *buf <= '9'))
    return -1;

  /* Translate an hexadecimal value. */
  if (*buf == '0' && (buf[1] == 'x' || buf[1] == 'X'))
    {
      buf += 2;
      c = *buf++;
      while (c)
	{
	  if (c >= '0' && c <= '9')
	    c = c - '0';
	  else if (c >= 'a' && c <= 'f')
	    c = c - 'a' + 10;
	  else if (c >= 'A' && c <= 'F')
	    c = c - 'A' + 10;
	  else
	    return -1;

	  value = (value << 4) | (value_t) ((unsigned) c);
	}
      *v = value;
      return 0;
    }
  else
    {
      int sign = 0;

      if (buf[0] == '-')
	{
	  sign = 1;
	  buf++;
	}
      while ((c = *buf++) != 0)
	{
	  if (c >= '0' && c <= '9')
	    c = c - '0';
	  else
	    return -1;

	  value = (value * 10) + (value_t) c;
	}
      if (sign)
	value = -value;
      *v = value;
      return 0;
    }
  return -1;
}

/* Busy loop to wait for a command or a valid number. */
static op_type
calc_wait_command(value_t* v)
{
	INT8U err;
	char buf[64];
	int pos;
	int c;
	int code;

	while (1)
    {
		pos = 0;
		code = 0;
		while (1)
		{	// 세마포어 대기(from TOUCH_ISR)
			OSSemPend(INPUT_SEM, 0, &err);
			c = KeyCode;
			if( c >= 0 )
			{
				// 버튼 눌림 디스플레이
				VLcd_Draw_BUTTON(c);
				if (c == POS_BACKSPACE)
				{
					pos--;
					if (pos < 0)
					{
						pos = 0;
					}
					buf[pos] = 0;
					if(pos == 0)
					{
						buf[0]='0';
						buf[1]= 0;
					}
					VLcd_Printf(VAXIS_X-(32*8),VAXIS_Y, BLACK, WHITE, 1,1,"                                 ");

					if(pos == 0)
						VLcd_Printf(VAXIS_X,VAXIS_Y, BLACK, WHITE, 1,1,&buf[0]);						
					else
						VLcd_Printf(VAXIS_X-((pos-1)*8),VAXIS_Y, BLACK, WHITE, 1,1,&buf[0]);
				}
				else if(( c > POS_9 ) && (c < POS_MAX))
				{
					code = c;	// opcode
//					Touch_pressed = 0;
					break;
				}
				else if( c >= 0 )
				{
					if(pos <= 9)
					{
						buf[pos] = c+'0';
						buf[pos+1] = 0;
						VLcd_Printf(VAXIS_X-(32*8),VAXIS_Y, BLACK, WHITE, 1,1,"                                ");
						VLcd_Printf(VAXIS_X-(pos*8),VAXIS_Y, BLACK, WHITE, 1,1,&buf[0]);
						pos++;
					}
				}
			}
//			Touch_pressed = 0;
		}

		buf[pos] = 0;

		if(code)
		{
			if (get_value (buf, v) == 0)
				return (OP_NUMBER+code);
			else
				return code;
		}
		else
		{
			if (get_value (buf, v) == 0)
				return (OP_NUMBER);
		}
	}
}

static void
print_help()
{
  int i;
  
#ifdef VALUE_16
  Uart_Printf("16-bit Integer Calculator\n");
#elif defined(VALUE_32)
  Uart_Printf("32-bit Integer Calculator\n");
#else
  Uart_Printf("64-bit Integer Calculator\n");
#endif

  for (i = 0; commands[i].name; i++)
    {
      if (commands[i].help == 0)
	continue;

      Uart_Printf(commands[i].name);
      Uart_Printf(" \t");
      Uart_Printf(commands[i].help);
      Uart_Printf("\n");
    }
}

unsigned long
lsqrt (unsigned long x)
{
    if (x <= 1)
        return x;

    /* If 'x' is small enough, use 16-bit integers.  */
    if ((x & 0xFFFF0000L) == 0)
    {
        unsigned short v0, x0, q0, x1;

        v0 = (unsigned short)(x);
        x0 = v0 / 2;
        while (1) {
            q0 = v0 / x0;
            x1 = (x0 + q0) / 2;
            if (q0 >= x0)
                break;
            x0 = x1;
        }
        return (unsigned long) x1;
    }
    else
    {
        unsigned long v0, q0, x1;

        v0 = x;
        x  = x / 2;
        while (1) {
            q0 = v0 / x;
            x1 = (x + q0) / 2;
            if (q0 >= x)
                break;
            x = x1;
        }
        return x1;
    }
}

// virtual lcd printf
void VLcd_Printf(int x, int y, int color, int bkcolor, int zx, int zy, char *fmt,...)
{
	LCD_DISPLAY_MSG q;
	INT8U err;

	// 메지지 생성
	q.command = UI_LCDPRINTF;
	q.x = x;
	q.y = y;
	q.Color = color;
	q.bgColor = bkcolor;
	q.param = NULL;
	q.msg = fmt;

	// 메시지 큐 전달
    err = OSQPost(qLCD_id, (void *)&q);
    if (err) Uart_Printf("OSQ error found, code[0x%X]\n",err);
}

// virtual Lcd_Draw_BUTTON
void VLcd_Draw_BUTTON(int button)
{
	LCD_DISPLAY_MSG q;
	INT8U err;

	// 메지지 생성
	q.command = UI_BUTTON_PRESSED;
	q.param = button;

	// 메시지 큐 전달
    err = OSQPost(qLCD_id, (void *)&q);
    if (err) Uart_Printf("OSQ error found, code[0x%X]\n",err);
}

/*-----------------------------------------------------------------------------
 * Program : TaskCalcOp.C
-----------------------------------------------------------------------------*/
