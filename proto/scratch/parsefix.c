
const char *const fixmsg = "8=FIX4.2;35=D;38=100.1;44=20;49=TRADER1;56=DEST1;10=000";
const int size = 54;
const char delim = ';'

Addr	Act	Digit	Offset	Fn
0	1	1	10	X
1	2	2	16	X
2	3	3
3	4	4
4	5	5
5	6	6
6	7	7
7	8	8
8	9	9
9		X		X
10	11	1	14		
11	12	2		X
12	15	5	15	X
13		X
14	113	3
		X
15	154     4
		X
16	21	1	
17	23	3	20
18	25	5
19	27	7
		X
20	236	6	21
		X
21	2364	4

union
{
  char     digit;
  uint16_t parser;
  uint16_t offset;
  uint64_t raw;
}
node_t node[65536];
node_t cnode;

int main()
{
  char *p = fixmsg;
  unsigned int idx = *p - 49;
  if( idx > 8 ) return 0; // Bad digit
  cnode = node[idx];

  while( *++p )
  {
    if( *p == '=' )
    { 
      cnode.raw >> 24;
      return cnode.parser; // tag found
    }

    code.raw = cnode.raw >> 8;
    cnode = node[cnode.offset];

    while( *p != cnode.digit )
    {
      if( !cnode.digit ) return 0; // tag not handled
      code.raw = cnode.raw >> 8;
      cnode = node[cnode.offset + 1];
    }
  }
}
