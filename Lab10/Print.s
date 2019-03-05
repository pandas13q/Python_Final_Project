; Print.s
; Student names: Summer Gregurek and Zachary Susskind
; Last modification date: 4/13/16
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix

    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB

  

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutDec
	PUSH {R0, LR}
	MOV R12, #1
	;handles the number of output digits needed
	;also handles the number of zeros needed to avoid null character error
outSet
	LDR R1, =1000000000
	CMP R0, R1
	BHS bOut
	LDR R1, =100000000
	CMP R0, R1
	BHS hmOut
	LDR R1, =10000000
	CMP R0, R1
	BHS tmOut
	LDR R1, =1000000
	CMP R0, R1
	BHS mOut
	LDR R1, =100000
	CMP R0, R1
	BHS htOut
	LDR R1, =10000
	CMP R0, R1
	BHS ttOut
	LDR R1, =1000
	CMP R0, R1
	BHS thOut
	LDR R1, =100
	CMP R0, R1
	BHS hOut
	LDR R1, =10
	CMP R0, R1
	BHS tOut
	LDR R1, =1
	CMP R0, R1
	BHS oOut
	B finOut
bOut
	MOV R12, #9
	B nOut
hmOut
	MOV R12, #8
	B nOut
tmOut
	MOV R12, #7
	B nOut
mOut
	MOV R12, #6
	B nOut
htOut
	MOV R12, #5
	B nOut
ttOut
	MOV R12, #4
	B nOut
thOut
	MOV R12, #3
	B nOut
hOut
	MOV R12, #2
	B nOut
tOut
	MOV R12, #1
	B nOut
oOut
	MOV R12, #0
	B nOut
nOut
	MOV R2, R0
	UDIV R2, R1
	MUL R3, R2, R1
	SUB R0, R3
	LDR R1, =0x30
	PUSH {R0, R1}
	ADD R0, R1, R2
	BL ST7735_OutChar
	POP {R0, R1}
	B outSet
finOut
	CMP R12, #0
	BEQ finOutEnd
	MOV R0, #0x30
	BL ST7735_OutChar
	SUB R12, #1
	B finOut
finOutEnd
	POP {R0, PC}		
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000 "
;       R0=3,    then output "0.003 "
;       R0=89,   then output "0.089 "
;       R0=123,  then output "0.123 "
;       R0=9999, then output "9.999 "
;       R0>9999, then output "*.*** "
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix
	PUSH {R0, LR}
	LDR R1, =9999
	CMP R0, R1
	BHI errorOut
	MOV R3, #1000
	UDIV R1, R0, R3
	MUL R2, R1, R3
	SUB R0, R2
	PUSH {R0, R1}
	MOV R0, R1
	ADD R0, #0x30
	BL ST7735_OutChar
	MOV R0, #0x2E ;the period
	BL ST7735_OutChar
	POP {R0, R1}
	MOV R3, #100
	UDIV R1, R0, R3
	MUL R2, R1, R3
	SUB R0, R2
	PUSH {R0, R1}
	MOV R0, R1
	ADD R0, #0x30
	BL ST7735_OutChar
	POP {R0, R1}
	MOV R3, #10
	UDIV R1, R0, R3
	MUL R2, R1, R3
	SUB R0, R2
	PUSH {R0, R1}
	MOV R0, R1
	ADD R0, #0x30
	BL ST7735_OutChar
	POP {R0, R1}
	ADD R0, #0x30
	BL ST7735_OutChar
	POP {R0, PC}
errorOut
;prints asterisk, followed by period and then 3 more asterisks
	MOV R0, #0x2A
	BL ST7735_OutChar
	MOV R0, #0x2E
	BL ST7735_OutChar
	MOV R0, #0x2A
	BL ST7735_OutChar
	MOV R0, #0x2A
	BL ST7735_OutChar
	MOV R0, #0x2A
	BL ST7735_OutChar
	POP {R0, PC}
    ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
