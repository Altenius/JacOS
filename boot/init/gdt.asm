align 4

gdt_begin:
	dd 0 ;null descriptor
	dd 0
; all segments span 0 - 4GB
; Level 0 segments
; DPL = Descriptor privilege level
; C = Conforming (Code segment)
; E = Expansion Direction (Data segment; set = expand down)
; Ac = Accessed
; RW = Writable (Data segment)
; R = Readable (Code segment)
	gdt_code_0:
	;code segment
	dw 0xFFFF ;limit 0:15
	dw 0 ;base 0:15
	db 0 ;base 16:23
	db 10011010b ;Access Byte. Pr(Present)=1, DPL=00, S=1, Code=1, C=1, R=1, Ac=1
	db 11001111b ;Flags. Gr=1, Sz=1, 0, 0. Limit 16:19
	db 0 ;Base 24:31

	gdt_data_0:
	;data segment
	dw 0xFFFF ;limit 0:15
	dw 0 ;base 0:15
	db 0 ;base 16:23
	db 10010010b ;Access Byte. Pr=1, DPL=00, S=1, Code=0, E=0, RW=1, Ac=1
	db 11001111b ;Flags. Gr=1, Sz=1, 0, 0. Limit 16:19
	db 0 ;Base 24:31

; Level 3 segments
	gdt_code_3:
	;code segment
	dw 0xFFFF ;limit 0:15
	dw 0 ;base 0:15
	db 0 ;base 16:23
	db 11111010b ;Access Byte. Pr=1, DPL=03, S=1, Code=1, C=1, R=1, Ac=1
	db 11001111b ;Flags. Gr=1, Sz=1, 0, 0. Limit 16:19
	db 0 ;Base 24:31

	gdt_data_3:
	;data segment
	dw 0xFFFF ;limit 0:15
	dw 0 ;base 0:15
	db 0 ;base 16:23
	db 11110010b ;Access Byte. Pr=1, DPL=03, S=1, Code=0, E=0, RW=1, Ac=1
	db 11001111b ;Flags. Gr=1, Sz=1, 0, 0. Limit 16:19
	db 0 ;Base 24:31

	tss:
	dw 0x67
	dw 0
	db 0
	db 11101001b
	db 10000000b
	db 0
gdt_end:

nop
nop ; align gdt descriptor on an odd word address

gdt_descriptor:
	dw gdt_end-gdt_begin-1 ; gdt length - 1
	dd gdt_begin	; gdt address

; segment selector: index 3:15, Table Indicator 2:2, RPL (Requested Privilege Level) 0:1
CODE_SEG_0 equ 1 << 3
DATA_SEG_0 equ 2 << 3
CODE_SEG_3 equ (3 << 3) | 3
DATA_SEG_3 equ (4 << 3) | 3
TSS_SEG equ 5 << 3
