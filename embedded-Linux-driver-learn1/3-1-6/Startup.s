; 起动文件。初始化C程序的运行环境，然后进入C程序代码。

      	IMPORT	|Image$$RO$$Limit|  
      	IMPORT	|Image$$RW$$Base|   
      	IMPORT	|Image$$ZI$$Base|   
        IMPORT	|Image$$ZI$$Limit|  

		IMPORT	Main		; 声明C程序中的Main()函数

		AREA	Start,CODE,READONLY	; 声明代码段Start
		ENTRY				; 标识程序入口
		CODE32				; 声明32位ARM指令	
			
Reset	LDR		SP,=0x40003F00

        ; 初始化C程序的运行环境
    	LDR	  	R0,=|Image$$RO$$Limit|
    	LDR	    R1,=|Image$$RW$$Base|	
    	LDR	    R3,=|Image$$ZI$$Base|	
			
    	CMP	  	R0,R1
    	BEQ	    LOOP1
LOOP0	CMP	    R1,R3	  
    	LDRCC   R2,[R0],#4     
    	STRCC   R2,[R1],#4 
    	BCC	    LOOP0
    	
LOOP1	LDR	    R1,=|Image$$ZI$$Limit| 
    	MOV	    R2,#0
LOOP2	CMP	    R3,R1
    	STRCC   R2,[R3],#4
    	BCC	    LOOP2
    	
    	B		Main		; 跳转到C程序代码Main()函数
    	
    	END