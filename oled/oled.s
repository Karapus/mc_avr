TWCR = 0x36
TWINT = 7
TWSTA = 5
TWSTO = 4
TWEN = 2
TWDR = 0x03
TWSR = 0x01
TWBR = 0x00
CPU_F = 4000000
SCL_F = 20000
SLA_W = 0x78
PORTC = 0x15
START = 0x08
MT_SLA_ACK = 0x18
MT_DATA_ACK = 0x28
DDRC  = 0x14

.global x_re
        .data
        .type   x_re, @object
        .size   x_re, 128
x_re:
        .word	3 
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0
        .word   0
        .word   17
        .word   18
        .word   19
        .word   20
        .word   21
        .word   22
        .word   23
        .word   24
        .word   25
        .zero   78

.section .text
ERROR:
;in r16, TWSR
;out PORTA, r16 ;A = FF
rjmp ERROR

.global main
main:
;-----------------------TWI_Init---------------------------
ldi r16, (((CPU_F)/(SCL_F)-16)/2) 
out TWBR, r16
ldi r16, 0
out TWSR, r16

;-----------------------TWI_Start--------------------------
;Send START condition
ldi r16, (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)
out TWCR, r16

;Wait for the START condition to be transmitted.
1:
in r16, TWCR
sbrs r16, TWINT
rjmp 1b

;Check status. If status different from START go to ERROR.
in r16, TWSR
andi r16, 0xF8
cpi r16, START
brne ERROR

;-----------------------TWI_SendAdress---------------------
;Load SLA_W into TWDR Register. Clear TWINT bit in TWCR to start transmission of address.
ldi r16, SLA_W
out TWDR, r16
ldi r16, (1<<TWINT) | (1<<TWEN)
out TWCR, r16

;Wait for SLA+W to be transmitted, and ACK/NACK to be received.
1:
in r16,TWCR
sbrs r16,TWINT
rjmp 1b

;Check status, if different from MT_SLA_ACK go to ERROR.
in r16, TWSR
andi r16, 0xF8
cpi r16, MT_SLA_ACK
brne ERROR

;-----------------------OLED_SendByte-----------------------
.macro sendByte b
	ldi r16, \b
	rcall TWI_SendByte
.endm

;-----------------------OLED_SendCmd-----------------------
.macro sendCmd b
	sendByte 0x80
	sendByte \b
.endm

;-----------------------OLED_Init--------------------------
sendCmd 0xA8	;Set Multiplex Ratio
sendCmd 63

sendCmd 0xD3	;Set Display Offset
sendCmd 0

sendCmd 0x40	;Set Display Start Line to 0

sendCmd 0xA1	;Set Set Segment Re-map ON

sendCmd 0xC8	;Set COM Output Scan Direction
		;to remapped mod

sendCmd 0xDA	;Set COM Pins Hardware Configuration
sendCmd 0x12	;alternative, no remap

sendCmd 0x81	;Set Contrast Control
sendCmd 255

sendCmd 0xA4	;Entire Display ON, show RAM

sendCmd 0xA6	;Set Normal/Inverse Display to Normal

sendCmd 0xD5	;Set Display Clock
sendCmd 0x80

sendCmd 0x8D	;Charge Pump Setting
sendCmd 0x14	;enable
sendCmd 0xAF	;Set Display ON
;-----------------------OLED_Draw--------------------------
sendCmd 0x20	;Set Memory Addressing Mode
sendCmd 0x00
sendCmd 0x21	;Set Column Addres
sendCmd 0	;start
sendCmd 127	;end
sendCmd 0x22	;Set Page Address
sendCmd 0	;start
sendCmd 7	;end
sendByte 0x40	;Next is data

ldi r17, 8
l10:
ldi r18, 128
	l20:
	clr r16
	rcall TWI_SendByte
	dec r18
	brne l20
dec r17
brne l10

;sendByte 0xFF
ldi r30, lo8(x_re)
ldi r31, hi8(x_re)
ld r16, Z
rcall TWI_SendByte
adiw Z, 1
ld r16, Z
rcall TWI_SendByte
ldi r19, 1
ldi r18, 0
l3:
ld r1, Y
adiw Y, 2
cp r1, r18
brne post
or r16, r19
post:
lsl r19
brne l3
;rcall TWI_SendByte
;ldi r17, 8
;l1:
;ldi r18, 128
;	l2:
;	clr r16
;	ldi r19, 1
;		l3:
;		ld r1, Y+
;		cp r1, r18
;		brne post
;		or r16, r19
;		post:
;		lsl r19
;		brne l3
;	sbiw Y, 8
;	rcall TWI_SendByte
;	dec r18
;	brne l2
;dec r17
;brne l1


;-----------------------TWI_Stop---------------------------
;Transmit STOP.
ldi r16, (1<<TWINT)|(1<<TWEN)|(1<<TWSTO)
out TWCR, r16
l:
rjmp l

TWI_SendByte:
;Load DATA. And start transmission.
out TWDR, r16
ldi r16, (1<<TWINT)|(1<<TWEN)
out TWCR, r16

;Wait for DATA to be transmitted.
1:
in r16,TWCR
sbrs r16,TWINT
rjmp 1b

;Check status. If different from MT_DATA_ACK go to ERROR.
in r16, TWSR
andi r16, 0xF8
;cpi r16, MT_DATA_ACK
;brne ERROR
ret    

;.global __vector_default
;__vector_default:
;reti
.end
