#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

#define PROB_HEAD 0.4
#define PROB_TAIL (1 - PROB_HEAD)
#define THETA 0.000000000000001

int min (int a, int b)
{
  return a > b ? b : a;
}

int main ()
{
  double states[101];
  int actions[101];
  int s, a;
  int count = 0;
  double max_delta = 1;

  for (s = 0; s <= 99; s++)
    states[s] = 0;
  states[100] = 1;

  while (max_delta > THETA) {
    max_delta = 0;
    for (s = 1; s <= 99; s++) {
      double max_v = 0;
      for (a = 0; a <= min (s, 100 - s); a++) {
        double v = states[s + a] * PROB_HEAD + states[s - a] * PROB_TAIL;
        if (v - max_v > THETA) {
          max_v = v;
        }
      }
      if (fabs (states[s] - max_v) - max_delta > THETA)
        max_delta = fabs (states[s] - max_v);
      states[s] = max_v;
    }

    printf ("%d\n", count);
    for (s = 1; s <= 99; s++)
      printf ("%5.3f ", states[s]);
    printf ("\n");

    count++;
  }

  for (s = 1; s <= 99; s++) {
    double max_v = 0;
    int best_action;
    for (a = 1; a <= min (s, 100 - s); a++) {
      double v = states[s + a] * PROB_HEAD + states[s - a] * PROB_TAIL;
      if (v - max_v > THETA) {
        best_action = a;
        max_v = v;
      }
    }
    actions[s] = best_action;
  }

  printf ("\n");
  for (s = 1; s <= 99; s++)
    printf ("%d ", actions[s]);
  printf ("\n");

  printf ("\nState: Best Actions\n");
  for (s = 1; s <= 99; s++) {
    printf ("%5d:", s);
    double max_v = states[s + actions[s]] * PROB_HEAD + states[s - actions[s]] * PROB_TAIL;
    for (a = 1; a <= min (s, 100 - s); a++) {
      double v = states[s + a] * PROB_HEAD + states[s - a] * PROB_TAIL;
      if (fabs (v - max_v) <= THETA)
        printf (" %d", a);
    }
    printf ("\n");
  }

  return 0;
}

