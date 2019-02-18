#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>


// table for ln(1), ln(2), ln(3), ..., ln(20)
float ln[21];
// table for ln(1!), ln(2!), ln(3!), ..., ln(20!)
float ln_ex[21];
// table for poisson(lambda, 1), poisson(lambda, 2), ..., poisson(lambda, 20)
float poisson_tbl[5][21];
// table for poisson(lambda, >= 1), poisson(lambda, >= 2), ..., poisson(lambda, >= 20)
float poisson_ge_tbl[5][21];

int min3 (int a, int b, int c) 
{
  int t = a < b ? a : b;
  return t < c ? t : c;
}

void init_ln () {
  int i;
  ln_ex[0] = 0;
  for (i = 1; i <= 20; i++) {
    ln[i] = logf (i);
    ln_ex[i] = ln[i] + ln_ex[i - 1];
  }
}

float poisson_internal (int lambda, int n)
{
  float l = n * ln[lambda] - lambda - ln_ex[n];
  float p = expf (l);
  return p;
}

void init_poisson ()
{
  int i, j;
  for (i = 2; i <= 4; i++)
    for (j = 0; j <= 20; j++) {
      poisson_tbl[i][j] = poisson_internal (i, j);
      if (j == 0)
        poisson_ge_tbl[i][j] = 1;
      else
        poisson_ge_tbl[i][j] = poisson_ge_tbl[i][j - 1] - poisson_tbl[i][j - 1];
    }
}

float poisson (int lambda, int k)
{
  return poisson_tbl[lambda][k];
}

float poisson_ge (int lambda, int k)
{
  return poisson_ge_tbl[lambda][k];
}

float evaluate (int num1, int num2, int num_move, float v[][21])
{
  int next_1 = num1;
  int next_2 = num2;
  int reqs1, reqs2;
  int returns1, returns2;
  float new_v = 0;

  next_1 -= num_move;
  next_2 += num_move;
  new_v -= abs (num_move) * 2;

  for (reqs1 = 0; reqs1 <= next_1; reqs1++)
    for (reqs2 = 0; reqs2 <= next_2; reqs2++) {
      float q1, q2;
      if (reqs1 == next_1)
        q1 = poisson_ge (3, reqs1);
      else
        q1 = poisson (3, reqs1);
      if (reqs2 == next_2)
        q2 = poisson_ge (4, reqs2);
      else
        q2 = poisson (4, reqs2);
      new_v += (reqs1 + reqs2) * q1 * q2 * 10;

      for (returns1 = 0; returns1 <= 20 - next_1 + reqs1; returns1++)
        for (returns2 = 0; returns2 <= 20 - next_2 + reqs2; returns2++) {
          int new_1 = next_1 - reqs1 + returns1;
          int new_2 = next_2 - reqs2 + returns2;
          float p1, p2;
          if (new_1 == 20)
            p1 = poisson_ge (3, returns1);
          else
            p1 = poisson (3, returns1);
          if (new_2 == 20)
            p2 = poisson_ge (2, returns2);
          else
            p2 = poisson (2, returns2);
          new_v += 0.9 * v[new_1][new_2] * q1 * q2 * p1 * p2;
        }
    }

  return new_v;
}

int main ()
{
  int i, j, z;
  float v[21][21] = {0};
  float v2[21][21] = {0};
  int m[21][21] = {0};
  bool value_stable;
  bool policy_stable;
  int count = 0;

  init_ln ();
  init_poisson ();

  do {
    policy_stable = true;

    do {
      value_stable = true;

      for (i = 0; i <= 20; i++)
        for (j = 0; j <= 20; j++) {
          v2[i][j] = evaluate (i, j, m[i][j], v);
          if (fabs(v2[i][j] - v[i][j]) > .0000001 )
            value_stable = false;
        }
  
      if (!value_stable)
        for (i = 0; i <= 20; i++)
          for (j = 0; j <= 20; j++) {
            v[i][j] = v2[i][j];
            v2[i][j] = 0;
          }
    } while (!value_stable);

    printf ("%d\n", count);
    for (i = 0; i <= 20; i++) {
      for (j = 0; j <= 20; j++)
        printf ("%7.2f", v[i][j]);
      printf ("\n");
    }
  
    for (i = 0; i <= 20; i++)
      for (j = 0; j <= 20; j++) {
        float max = 0;
        int max_m = 0;
        int t;
        for (t = - min3 (j, 20 - i, 5); t <= min3 (i, 20 - j, 5); t++) {
          float new_v = evaluate (i, j, t, v);
  
          if (new_v > max) {
            max = new_v;
            max_m = t;
          }
        }
        if (m[i][j] != max_m) {
          m[i][j] = max_m;
          policy_stable = false;
        }
      }
  
    printf ("\n");
    for (i = 0; i <= 20; i++) {
      for (j = 0; j <= 20; j++)
        printf ("%2d ", m[i][j]);
      printf ("\n");
    }
    count++;
  } while (!policy_stable);

  return 0;
}

