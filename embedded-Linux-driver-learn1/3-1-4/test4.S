			AREA	Example4,CODE,READONLY	; 声明代码段Example4
			ENTRY				; 标识程序入口
			CODE32				; 声明32位ARM指令
			
START		; if(x>y) z=100;
			;  else  z=50; 
			; 设x为R0，y为R1，z为R2 (x、y、z均为无符号整数)
			MOV		R0,#76		; 初始化x的值
			MOV		R1,#243		; 初始化y的值
			CMP		R0,R1		; 判断x>y?
			MOVHI	R2,#100		; x>y条件正确，z=100
			MOVLS	R2,#50		; 条件失败，z=50
			
			; for(i=0; i<10; i++)
			; {  x++;
			; }
			; 设x为R0，i为R2 (i、x均为无符号整数)
			MOV		R0,#0		; 初始化x的值
			MOV		R2,#0		; 设置i=0
FOR_L1		CMP		R2,#10		; 判断i<10?
			BHS		FOR_END		; R2>=10,退出循环
			
			ADD		R0,R0,#1	; 循环体，x++
			
			ADD		R2,R2,#1	; i++
			B		FOR_L1		
FOR_END		NOP


			; while(x<=y)	
			; {  x *= 2;
			; }
			; 设x为R0，y为R1 (x、y均为无符号整数)
			MOV		R0,#1		; 初始化x的值
			MOV		R1,#20		; 初始化y的值
			B		WHILE_L2	; 首先要判断条件
			
WHILE_L1	MOV		R0,R0,LSL #1	; 循环体，x *= 2

WHILE_L2	CMP		R0,R1		; 判断x≤y?			
			BLS		WHILE_L1	; 若条件正确，继续循环
WHILE_END	NOP


			; do
			; {  x--;
			; }	while(x>0);
			; 设x为R0 (x为无符号整数)
			MOV		R0,#5		; 初始化x的值
			
DOWHILE_L1	ADD		R0,R0,#-1	; 循环体，x--
			
DOWHILE_L2	MOVS	R0,R0		; R0 <= R0，并影响条件码标志		
			BNE		DOWHILE_L1	; 若R0不为0(即x不为0)，则继续循环
DOWHILE_END	NOP			
			
			
			; switch(key&0x0F)
			; {  case  0:
			;    case  2:
			;    case  3:  x = key + y;
			;              break;
			;    case  5:  x = key - y;
			;              break;
			;    case  7:  x = key * y;
			;              break;
			;    default:  x = 168;
			;              break;       
			; }
			; 设x为R0，y为R1，key为R2 (x、y、key均为无符号整数)
			MOV		R1,#3		; 初始化y的值
			MOV		R2,#2		; 初始化key的值

SWITCH		AND		R2,R2,#0x0F	; switch(key&0x0F)
		
CASE_0		CMP		R2,#0		; case	0:
CASE_2		CMPNE	R2,#2		; case	2:
CASE_3		CMPNE	R2,#3		; case	3:
			BNE		CASE_5	
			ADD		R0,R2,R1	; 		x = key + y
			B		SWITCH_END	; 		break
			
CASE_5		CMP		R2,#5		; case  5:
			BNE		CASE_7
			SUB		R0,R2,R1	; 		x = key - y
			B		SWITCH_END	; 		break
			
CASE_7		CMP		R2,#7		; case	7:
			BNE		DEFAULT		
			MUL		R0,R2,R1	;		x = key * y
			B		SWITCH_END  ; 		break
			
DEFAULT		MOV		R0,#168		; default: x = 168
			
SWITCH_END	NOP
			

HALT		B		HALT		
	
			END	
