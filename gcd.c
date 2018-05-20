unsigned gcd (unsigned x, unsigned y)
{
  unsigned m, k;
  if (x > y) {
    k = x;
    m = y;
  }
  else {
    k = y;
    m = x;
  }

  while (m != 0) {
    unsigned r = k % m;
    k = m; m = r;
  }
  return k;
}
