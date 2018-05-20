#define MIN(x, y) ((x)<(y))?(x):(y)
#define MAX(x, y) ((x)<(y))?(y):(x)
#define IS_CD(r, x, y) (((x)%(r)==0)&&((y)%(r)==0))

unsigned gcd (unsigned x, unsigned y)
{
  for (unsigned t = MIN (x,y); t>0; t--) {
    if (IS_CD(t, x, y))
      return t;
  }
  return MAX(x, y);
}
