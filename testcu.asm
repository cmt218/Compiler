  global main
  extern printf

  segment .bss
  a$1 resq 1
  a2$1 resq 1
  a1$1 resq 1
  sum$1 resq 1
  i$1 resq 1

  section .text
main:
  mov rax,1
  push rax
  pop  qword[i$1]
  mov rax,0
  push rax
  pop  qword[sum$1]
  mov   rdi,fmt1
  mov   rax,0
  push  rbp
  call  printf
  pop   rbp
L2:
  push  qword[i$1]
  mov rax,10
  push rax
  pop rbx
  pop rax
  cmp rax,rbx
  jl J4
  mov rax,0
  jmp J5
J4:
  mov rax,1
J5:
  push rax
  pop rax
  cmp rax, 0
  je L3
  push  qword[i$1]
  mov rax,1
  push rax
  call  add
  push  rax
  pop  qword[i$1]
  push  qword[sum$1]
  push  qword[i$1]
  call  add
  push  rax
  pop  qword[sum$1]
  push  qword[i$1]
  push  qword[sum$1]
  mov   rdi,fmt2
  pop   rdx
  pop   rsi
  mov   rax,0
  push  rbp
  call  printf
  pop   rbp
  jmp L2
L3:
  mov rax,0
  push rax
  pop   rax
  ret
add:
  pop   r15
  pop  qword[a1$1]
  pop  qword[a2$1]
  push  qword[a1$1]
  push  qword[a2$1]
  pop rbx
  pop rax
  add rax,rbx
  push rax
  pop  qword[a$1]
  push  qword[a$1]
  pop   rax
  push   r15
  ret

  section .data
  fmt1: db `  i   sum\n`, 0
  fmt2: db `  %ld   %ld\n`, 0
  fmt:  db '%ld  ', 0
  endl: db 10, 0
