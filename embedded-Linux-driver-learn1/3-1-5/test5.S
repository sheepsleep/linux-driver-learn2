;定义堆栈的大小
USR_STACK_LEGTH     EQU         64
SVC_STACK_LEGTH     EQU         0
FIQ_STACK_LEGTH     EQU         16
IRQ_STACK_LEGTH     EQU         64
ABT_STACK_LEGTH     EQU         0
UND_STACK_LEGTH     EQU         0

			AREA	Example5,CODE,READONLY	; 声明代码段Example5
			ENTRY				; 标识程序入口
			CODE32				; 声明32位ARM指令
START		MOV		R0,#0
			MOV		R1,#1
			MOV		R2,#2
			MOV		R3,#3
			MOV		R4,#4
			MOV		R5,#5
			MOV		R6,#6
			MOV		R7,#7
			MOV		R8,#8
			MOV		R9,#9
			MOV		R10,#10
			MOV		R11,#11
			MOV		R12,#12
			
			BL		InitStack	; 初始化各模式下的堆栈指针
			
			; 打开IRQ中断 (将CPSR寄存器的I位清零)
			MRS		R0,CPSR			; R0 <= CPSR
			BIC		R0,R0,#0x80 
			MSR		CPSR_cxsf,R0	; CPSR <= R0
						
			; 切换到用户模式
        	MSR     CPSR_c, #0xd0
        	MRS		R0,CPSR
        	
        	; 切换到管理模式
        	MSR     CPSR_c, #0xdf
        	MRS		R0,CPSR		

HALT		B		HALT
			
; 名称：InitStack
; 功能：堆栈初始化，即初始化各模式下的堆栈指针。
; 入口参数：无
; 出口参数：无
; 说明：在特权模式下调用此子程序，比如复位后的管理模式
InitStack    
        	MOV     R0, LR		; R0 <= LR，因为各种模式下R0是相同的

;设置管理模式堆栈
        	MSR     CPSR_c, #0xd3
        	LDR     SP, StackSvc
;设置中断模式堆栈
        	MSR     CPSR_c, #0xd2
        	LDR     SP, StackIrq
;设置快速中断模式堆栈
        	MSR     CPSR_c, #0xd1
        	LDR     SP, StackFiq
;设置中止模式堆栈
        	MSR     CPSR_c, #0xd7
      		LDR     SP, StackAbt
;设置未定义模式堆栈
        	MSR     CPSR_c, #0xdb
        	LDR     SP, StackUnd
;设置系统模式堆栈
        	MSR     CPSR_c, #0xdf
        	LDR     SP, StackUsr

        	MOV     PC, R0

StackUsr   	DCD     UsrStackSpace + (USR_STACK_LEGTH - 1)*4
StackSvc    DCD     SvcStackSpace + (SVC_STACK_LEGTH - 1)*4
StackIrq    DCD     IrqStackSpace + (IRQ_STACK_LEGTH - 1)*4
StackFiq    DCD     FiqStackSpace + (FIQ_STACK_LEGTH - 1)*4
StackAbt    DCD     AbtStackSpace + (ABT_STACK_LEGTH - 1)*4
StackUnd    DCD     UndtStackSpace + (UND_STACK_LEGTH - 1)*4


; 分配堆栈空间 
        	AREA    MyStacks, DATA, NOINIT, ALIGN=2
UsrStackSpace     	SPACE  	USR_STACK_LEGTH * 4 	; 用户（系统）模式堆栈空间
SvcStackSpace      	SPACE  	SVC_STACK_LEGTH * 4  	; 管理模式堆栈空间
IrqStackSpace      	SPACE  	IRQ_STACK_LEGTH * 4  	; 中断模式堆栈空间
FiqStackSpace      	SPACE  	FIQ_STACK_LEGTH * 4  	; 快速中断模式堆栈空间
AbtStackSpace      	SPACE  	ABT_STACK_LEGTH * 4  	; 中止义模式堆栈空间
UndtStackSpace     	SPACE  	UND_STACK_LEGTH * 4  	; 未定义模式堆栈			
			
			
			END	