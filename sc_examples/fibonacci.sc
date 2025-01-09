int fib(int h)
{
   if(h<=1)
   return h;
   return fib(h-1)+fib(h-2);
}

__ASM__ uint32_t cycles_count()
{
   "entry a1,56"
   "rsr a2,234"
   "retw.n"
}

void main()
{
int num=40;
 int op=45;
   uint32_t t1=millis();
   int k=fib(num);
   uint32_t t2=millis();
   float t3=(t2-t1);
   printfln("fib %d=%d duration %u %dms",num,k,t2-t1,(int)(t3));
}