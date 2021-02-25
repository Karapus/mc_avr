;Send START condition
ldi
r16, (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)
out TWCR, r16

;Wait for the START condition to be transmitted.
wait1:
in r16, TWCR
sbrs r16, TWINT
rjmp wait1

;Check status. If status different from START go to ERROR.
in r16, TWSR
andi r16, 0xF8
cpi r16, START
brne ERROR

;Load SLA_W into TWDR Register. Clear TWINT bit in TWCR to start transmission of address.
ldi r16, SLA_W
out TWDR, r16
ldi r16, (1<<TWINT) | (1<<TWEN)
out TWCR, r16

;Wait for SLA+W to be transmitted, and ACK/NACK to be received.
wait2:
in
r16,TWCR
sbrs r16,TWINT
rjmp wait2

;Check status, if different from MT_SLA_ACK go to ERROR.
in r16, TWSR
andi r16, 0xF8
cpi r16, MT_SLA_ACK
brne ERROR

;Load DATA. And start transmission of address.
ldi r16, DATA
out TWDR, r16
ldi r16, (1<<TWINT) | (1<<TWEN)
out TWCR, r16

;Wait for DATA to be transmitted.
wait3:
in r16,TWCR
sbrs r16,TWINT
rjmp wait3
;Check status. If different from MT_DATA_ACK go to ERROR.
in r16, TWSR
andi r16, 0xF8
cpi r16, MT_DATA_ACK
brne ERROR

;Transmit STOP.
ldi r16, (1<<TWINT)|(1<<TWEN)|(1<<TWSTO)
out TWCR, r16
