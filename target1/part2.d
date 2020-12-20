
part2.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <.text>:
   0:	48 bf 3a 3f 37 39 39 	movabs $0x35393b3939373f3a,%rdi
   7:	3b 39 35 
   a:	48 89 7c 24 08       	mov    %rdi,0x8(%rsp)
   f:	c7 44 24 10 00 00 00 	movl   $0x0,0x10(%rsp)
  16:	00 
  17:	48 8d 7c 24 08       	lea    0x8(%rsp),%rdi
  1c:	48 c7 04 24 ec 17 40 	movq   $0x4017ec,(%rsp)
  23:	00 
  24:	c3                   	retq   

0x59b997fa

3a 3f 37 39 39 3b /* */ 39 35