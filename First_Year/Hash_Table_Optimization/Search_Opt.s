global SearchByValue
extern strcmp

section .text

SearchByValue:
       push r15
       push rbx
       mov  QWORD r15, [rdi]
       xor     ebx, ebx
      .Inside:
             cmp     rbx, rdx
             ja     .Not_Found
             mov     rdi, QWORD  [r15]
             test    rdi, rdi
             je      .InsideNext
             call    strcmp
             test    eax, eax
             je      .Found

      .InsideNext:
             inc     rbx
             add     r15, 16
             jmp .Inside
  .Not_Found:
         xor     rax, rax
.EXX:
         pop rbx
         pop r15
         ret
         
.Found:
       mov     rax, 1
       pop rbx
       pop r15
       ret
