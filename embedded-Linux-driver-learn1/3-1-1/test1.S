COUNT		EQU		0x30003100	; ����һ����������ַΪ0x30003100	
			
			AREA	Example2,CODE,READONLY	; ���������Example2 
			ENTRY				; ��ʶ�������
			CODE32				; ����32λARMָ��
START		LDR		R1,=COUNT	; R1 <= COUNT
			MOV		R0,#0		; R0 <= 0 
			STR		R0,[R1]		; [R1] <= R0��������COUNTΪ0
						
LOOP   		LDR		R1,=COUNT	
			LDR		R0,[R1]		; R0 <= [R1] 
			ADD		R0,R0,#1	; R0 <= R0 + 1
			STR		R0,[R1]		; [R1] <= R0��������COUNT
			
			B		LOOP
			
			END	