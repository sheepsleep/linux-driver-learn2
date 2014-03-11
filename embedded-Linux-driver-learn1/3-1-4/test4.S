			AREA	Example4,CODE,READONLY	; ���������Example4
			ENTRY				; ��ʶ�������
			CODE32				; ����32λARMָ��
			
START		; if(x>y) z=100;
			;  else  z=50; 
			; ��xΪR0��yΪR1��zΪR2 (x��y��z��Ϊ�޷�������)
			MOV		R0,#76		; ��ʼ��x��ֵ
			MOV		R1,#243		; ��ʼ��y��ֵ
			CMP		R0,R1		; �ж�x>y?
			MOVHI	R2,#100		; x>y������ȷ��z=100
			MOVLS	R2,#50		; ����ʧ�ܣ�z=50
			
			; for(i=0; i<10; i++)
			; {  x++;
			; }
			; ��xΪR0��iΪR2 (i��x��Ϊ�޷�������)
			MOV		R0,#0		; ��ʼ��x��ֵ
			MOV		R2,#0		; ����i=0
FOR_L1		CMP		R2,#10		; �ж�i<10?
			BHS		FOR_END		; R2>=10,�˳�ѭ��
			
			ADD		R0,R0,#1	; ѭ���壬x++
			
			ADD		R2,R2,#1	; i++
			B		FOR_L1		
FOR_END		NOP


			; while(x<=y)	
			; {  x *= 2;
			; }
			; ��xΪR0��yΪR1 (x��y��Ϊ�޷�������)
			MOV		R0,#1		; ��ʼ��x��ֵ
			MOV		R1,#20		; ��ʼ��y��ֵ
			B		WHILE_L2	; ����Ҫ�ж�����
			
WHILE_L1	MOV		R0,R0,LSL #1	; ѭ���壬x *= 2

WHILE_L2	CMP		R0,R1		; �ж�x��y?			
			BLS		WHILE_L1	; ��������ȷ������ѭ��
WHILE_END	NOP


			; do
			; {  x--;
			; }	while(x>0);
			; ��xΪR0 (xΪ�޷�������)
			MOV		R0,#5		; ��ʼ��x��ֵ
			
DOWHILE_L1	ADD		R0,R0,#-1	; ѭ���壬x--
			
DOWHILE_L2	MOVS	R0,R0		; R0 <= R0����Ӱ���������־		
			BNE		DOWHILE_L1	; ��R0��Ϊ0(��x��Ϊ0)�������ѭ��
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
			; ��xΪR0��yΪR1��keyΪR2 (x��y��key��Ϊ�޷�������)
			MOV		R1,#3		; ��ʼ��y��ֵ
			MOV		R2,#2		; ��ʼ��key��ֵ

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