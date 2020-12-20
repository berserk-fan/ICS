phase_3:
int phase_3(char * mystr) {
  long i, i2;
  int result;
  char * template;

  //i = rsp + 12;
  //i2 = rsp + 8;
  result = 0;
  if (sscanf(mystr, template, i2, i) <= 1)
    explode_bomb();
  if (i2 > 7)
    explode_bomb();
  switch (i2) {
  case 0:
    result = 0xcf;
    break;
  case 1:
    result = 0x137;
    break;
  case 2:
    result = 0x2c3;
    break;
  case 3:
    result = 0x100;
    break;
  case 4:
    result = 0x185;
    break;
  case 5:
    result = 0xc3;
    break;
  case 6:
    result = 0x2aa;
    break;
  case 7:
    result = 0x147;
    break;
  default:
    explode_bomb();
    result = 0;
  }
  //400fbe   
  if (i != result)
    explode_bomb();
  return result;
}
read_six_numbers:
//читает 6 чисел (по 4 байта) в введенной строке и последовательно записывает их на стек вызвавшей его функции
phase_2:
int phase_2(char *str) {
  read_six_numbers();
  if ( * rsp != 1)
    explode_bomb();
  int result;
  for (i = rsp + 4, j = rsp + 24, i != j, i += 4) {
    result = * (i - 4);
    result *= 2;
    if ( * i != result)
      explode_bomb();
  }
  2n d = 2 * 1 st;
}
//so idea is every number should double previous;
//so idea is every number should double previous;
phase_4:
int phase_4(char * str) {
  //allocate 18 bytes
  //rcx = rsp + 12
  //rdx = rsp + 8
  int i, i2;
  int result;
  char * template;
  smth = 0x4025cf;
  result = 0;
  if (sscanf(str, template, i, i2) != 2)
    explode_bomb();
  if (i > 0xe)
    explode_bomb();
  int result = func4(i, 0x0, 0xe);
  if (result != 0)
    explode_bomb();
  if (0x0 != i2)
    explode_bomb();
}
func4:
func4(int a,unsigned int b, unsigned int c) {
  //alloc 8 bytes
  int result, ret;
  //rcx
  unsigned int smth;
  result = (c - b) >> 31 + (c - b);
  result >>= 1; //arithmetical shift
  smth = result + b;
  //smth here equal to 7 on first call
  //so by setting a to 7 fucntion will return 0. Other stafff is not important  
  if (smth <= a) {
    result = 0;
    if (smth >= a)
      return;
    else :
      ret = func4(a, smth + 1, c);
    result = 2 * ret + 1;
  } else {
    ret = func4(a, b, smth - 1);
    result = 2 * ret;
    return;
  }
}

phase_5:
int phase_5(char *str){
  //alloc 32 bytes
  long i;
  char * our_string;
  char tmp_char;
  if(string_length(str) != 6)
    explode_bomb;

  /* rbx + result to ecx - ecx is first symbol in my word
   * (%rsp) = first byte;
   * idea is by using our characters fill 0x10 to 0x16 with characters
   * that start at 0x4024b0;
   *after analysis char codes must end as 9 15 14 5 6 7;
   * take randomly : characters : ionefg
   */
  for(i = 0; i != 6; ++i){
    tmp_char = str[i] & 0xf; 
    our_string[i] = *(0x4024b0 + cur_char);
  }

  /* it is end of line as 7th symbol of string */
  *c3 &= 0xFFFFFFFFFFFF00FF;
  int arg2 = 0x40245e;
  str = c3;
  if(string_not_equal(str,arg2))
    explode_bomb();
  return;
}

phase_6:
int phase_6(char * str){
  /* alloc 0x50 bytes */
  int numbers[6];
  read_six_numbers(str, numbers);
  int some = 0;
  
  for(long i = numbers; i < 6; ++i){
    int test = number[i] - 1;
    if(test > 5)
      explode_bomb();
    for(int k = i; k <= 5; ++k){
      if(number[0] == number[k])
	explode_bomb();
    }
  }
  
  int tmp, tmp2;		/* ecx, edx */
  tmp = 7;
  for(int ii = 0; ii < 6; ++ii){
    tmp2 = tmp;
    tmp2 -= number[ii];
    number[ii] = tmp2;
  }
  /* here out new numbers are just 7 - prev number*/
  /*
    Probably it is linked list 
    0x6032d0 <node1>:       0x000000010000014c      0x00000000006032e0
    0x6032e0 <node2>:       0x00000002000000a8      0x00000000006032f0
    0x6032f0 <node3>:       0x000000030000039c      0x0000000000603300
    0x603300 <node4>:       0x00000004000002b3      0x0000000000603310
    0x603310 <node5>:       0x00000005000001dd      0x0000000000603320
    0x603320 <node6>:       0x00000006000001bb      0x0000000000000000
  */
  struct linked_list{
    long val;
    linked_list * next;
  } arr[6];
  
  rdx = 0x6032d0;
  linked_list* some_numbrs[6];
  for(int rsi = 0; rsi != 6; ++rsi){
    ecx = number[rsi];
    for(int eax = 1; eax != ecx; rsi++){
      rdx = rdx->next;
    }
  }
  some_numbers[rsi] = rdx;
  /* is desc order nodes: 3 4 5 6 1 2 */
  /* maybe all our numbers are from 1 to 6 so we can decide "adsad "which nodes to include in some_number; */

  rbx = some_numbrs;
  rcx = rbx;
  
  for(int i = 1; i != 6; ++i){
    rcx->next = some_numbrs[i];
    rcx = some_numbrs[i];
  }

  rbx = arr[0];
  (some_numbers[5])->next = 0;	/* end of list is zero */
  for(int jj = 5; jj > 0; --j){
  rax = rbx->next;
  rax = rax->val;
  if(arr[0]->val < rax)
    explode_bomb();
  }
  rbx = rbx->next;
  /* walk throught list and view it is sorted in desc order */
}





























