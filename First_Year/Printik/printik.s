;_______________________________________________________________________________
;Custom version of default printf() in C/C++ written in ASM
;
; Purpose:
;    Writes the string by format to the standard output (in console)
;
; Call format:
;     _printik(const char*, ...);
;
;Entry:
;   rdi, rsi, rdx, rcx, r8, r9 - arguments of printf() 
;
; Destroy list:
; r14 r15 r12 r10 r9 r8 rax cx rdx rsi rdi rbp 
;_______________________________________________________________________________

global _printik
section .text
_printik:
  pop r14       ;Saving return address
  mov r15, rbp  ;Saving default base pointer

  push r9       ;Pushing arguments to work with them in stack
  push r8
  push rcx
  push rdx
  push rsi
  push rdi
  mov rbp, rsp

  mov r10, rdi  
  mov r12, 8

  repeat:
  mov rax, '%'
  call strchr

  cmp r10, 0        ;if there is no more specificators, write the string
  je simple_string

who_is_who:

  mov r9, r10     ;writting everything before specificator
  sub r9, rdi

  push rdi
  push rsi
  mov rax, 1
  mov rsi, rdi
  mov rdi, 1
  mov rdx , r9
  syscall
  pop rsi
  pop rdi

  inc r10

  mov rbx, [r10]

  mov al, 'c'   ;Determining the required qualifier
  cmp al, bl
  je chr

  mov al, 's'
  cmp al, bl
  je str

  mov al, 'd'
  cmp al, bl
  je dec

  mov al, 'b'
  cmp al, bl
  je bin

  mov al, 'h'
  cmp al, bl
  je hex

  mov al, '%'
  cmp al, bl
  je proc
  
;_______________________________________________________________________________
; %c 
;Convert number to letter
; Destroy list:
;   rax rbx rsi rdi rdx r12 r10 r9
;_______________________________________________________________________________
chr:

  mov rax, [rbp + r12]
  add r12, 8
  mov r9, buf
  mov [r9], rax
  mov rax, 1
  mov rsi, buf
  mov rdi, 1
  mov rdx, 1
  syscall

  inc r10
  mov rdi, r10

  jmp repeat

;_______________________________________________________________________________
;%%
;Printing "%"
; Destroy list:
;   rax rsi rdi r9 r10
;_______________________________________________________________________________
proc:

  mov rax,'%'
  mov r9, buf
  mov [r9], rax
  mov rax, 1
  mov rsi, buf
  mov rdi, 1
  mov rdx, 1
  syscall

  inc r10
  mov rdi, r10

  jmp repeat

;_______________________________________________________________________________
; %d
;Convert number to string in decimal look
; Destroy list:
;   rax rbx rcx rsi rdi rdx r12 r10
;_______________________________________________________________________________
dec:
    xor rcx, rcx
    mov rax, [rbp + r12]
    add r12, 8


    mov rbx, 10
  rep_dec:            ;convering number to string by pushing to buffer ASCI of digits
    xor rdx, rdx
    div rbx
    cmp rax, rdx
    je check_dec
    back_dec:
    add rdx, '0'

    mov r9, buf
    add r9, rcx
    mov [r9], rdx
    inc rcx
    jmp rep_dec

    check_dec:
    cmp rax, 0
    jne back_dec

    end_dec:

    rep_2_dec:    ;writting to standart output from buffer
    mov rax, 1
    mov rsi, buf
    add rsi, rcx
    dec rsi
    mov rdi, 1
    mov rdx, 1
    push rcx
    syscall
    pop rcx
    dec rcx
    cmp rcx, 0
    jne rep_2_dec

    inc r10
    mov rdi, r10

    jmp repeat

;_______________________________________________________________________________
; %h
;Convert number to string in hexadecimal look
; Destroy list:
;   rax rbx rcx rsi rdi rdx r12 r10 r9
;_______________________________________________________________________________
hex:
  xor rcx, rcx
  mov rax, [rbp + r12]
  add r12, 8


  rep_hex:        ;converting digit to hexadecimal look
  mov rbx, 16     ;and transfering ASCI to buffer
  xor rdx, rdx
  div rbx
  cmp rax, rdx
  je check_hex
  back_hex:
  add rdx, num16

  mov r9, buf
  add r9, rcx
  mov rbx, [rdx]
  mov [r9], bl
  inc rcx
  jmp rep_hex

  check_hex:
  cmp rax, 0
  jne back_hex

  end_hex:

  rep_2_hex:      ;printing to standart output
  mov rax, 1
  mov rsi, buf
  add rsi, rcx
  dec rsi
  mov rdi, 1
  mov rdx, 1
  push rcx
  syscall
  pop rcx
  dec rcx
  cmp rcx, 0
  jne rep_2_hex

  inc r10
  mov rdi, r10

  jmp repeat

;_______________________________________________________________________________
; %b
;Convert number to string in binary look
; Destroy list:
;   rax rbx rcx rsi rdi rdx r12 r11 r10 r9
;_______________________________________________________________________________

bin:
      mov rax, [rbp + r12]
      add r12, 8
      mov rbx, rax
      mov r9, 63
    rep_bin:            ;digit determination
      mov rbx, rax
      mov rcx, 64
      sub rcx, r9
      cmp r9, 0
      je end_bin
      rol rbx, cl
      and rbx, 0x0000001
      cmp rbx, 0
      je zero
      jmp one

    zero:               ;printing zero
      push rax
      push rcx
      mov rax, 1
      mov rsi, num16
      mov rdi, 1
      mov rdx, 1
      syscall
      pop rcx
      pop rax
      dec r9
      jmp rep_bin

    one:                ;printing one
      push rax
      push rcx
      mov rax, 1
      mov rsi, num16
      inc rsi
      mov rdi, 1
      mov rdx, 1
      syscall
      pop rcx
      pop rax
      dec r11
      jmp rep_bin

    end_bin:
      inc r10
      mov rdi, r10

      jmp repeat

;_______________________________________________________________________________
; %s
;Convert number to string
; Destroy list:
;   rax rbx rsi rdi rdx r12 r10
;_______________________________________________________________________________

str:
    mov rsi, [rbp + r12]
    add r12, 8
    mov rax, 1
    mov rdi, 1
    mov rdx, 1

  rep_str:
    syscall
    inc rsi
    mov rbx, [rsi]
    cmp bl, 0
    jne rep_str

    inc r10
    mov rdi, r10

    jmp repeat

;_______________________________________________________________________________
; Printing string wothout specificators
; Destroy list:
;   rax rsi rdi rdx
;_______________________________________________________________________________

simple_string:        
  mov rax, 1
  mov rsi, rdi
  mov rdi, 1
  mov rdx, 1
rep_sim_str:
  syscall
  inc rsi
  cmp byte [rsi], 0
  jne rep_sim_str
  jmp ex

;_______________________________________________________________________________
; Custom version of strchr() in ASM
;
; Entry:
;       rax - ASCI of symbol  
;       r10 - address of string
; Return:
;       In case of finding
;         r10 - location address of specificator
;       else
;         r10 = 0 mark of absense of specificators in string
;
; Destroy list:
;   r10 rbx
;_______________________________________________________________________________

global strchr
strchr:
  again:
  mov bl, [r10]

  cmp bl, al
  je found

  cmp bl, 0
  je not_found

  inc r10
  jmp again

  found:
  ret

  not_found:
  xor r10,r10
  ret

;_______________________________________________________________________________
; Exit  from _printik
;Entry:
; r14 - return address
; r15 - default rbp
;_______________________________________________________________________________
ex:
  pop rax
  pop rax
  pop rax
  pop rax
  pop rax
  pop rax
  mov rbp, r15
  push r14

  ret
;_______________________________________________________________________________
section .data

num16: db '0123456789ABCDEF'
;_______________________________________________________________________________
section .bss

buf: resb 64
bufSize: equ $ - buf
