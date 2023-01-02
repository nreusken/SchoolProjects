#include <getopt.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define PI 3.1415926535
#define EPSILON .000000001

//function for calculating sine(x) using Pades approximation
double Sin(double x) {
  //defines y to x^2
  double y = x * x;
  //calculates numerator
  double top = 479249 * y;
  top = 52785432 - top;
  top = y * top;
  top = top - 1640635920;
  top = top * y;
  top = top + 11511339840;
  top = x * top;
  //calcualtes denominator
  double bottom = 18361 * y;
  bottom = bottom + 3177720;
  bottom = bottom * y;
  bottom = bottom + 277920720;
  bottom = bottom * y;
  bottom = bottom + 11511339840;

  //return the approximation of sin(x)
  return top / bottom;
}

//function for calculating cosine(x) using Pades approximation
double Cos(double x) {
  //defines y to x^2
  double y = x * x;
  //calculates numerator
  double top = 14615 * y;
  top = 1075032 - top;
  top = y * top;
  top = top - 18471600;
  top = top * y;
  top = top + 39251520;
  //calculates denominator
  double bottom = 127 * y;
  bottom = bottom + 16632;
  bottom = bottom * y;
  bottom = bottom + 1154160;
  bottom = bottom * y;
  bottom = bottom + 39251520;

  //returns approximation of cos(x)
  return top / bottom;
}

//function for calculating tangent(x) using Pades approximation
double Tan(double x) {
  //calculates numerator
  double top
      = x
        * ((x * x * x * x * x * x * x * x) - (990 * (x * x * x * x * x * x))
              + (135135 * (x * x * x * x)) - (4729725 * (x * x)) + 34459425);
  //calculates denominator
  double bottom
      = 45
        * ((x * x * x * x * x * x * x * x) - (308 * (x * x * x * x * x * x))
              + (21021 * (x * x * x * x)) - (360360 * (x * x)) + 765765);
  //returns approximation of tan(x)
  return top / bottom;
}

//function calculates e^x using taylor series
double Exp(double x) {
  //sum = total summation
  double sum = 1;
  //n = n term in the series
  double n = 1;
  //y = x is used to raise the power of x without editing x it iself
  double y = x;
  //fact = the factorial for the denominator for the taylor series
  double fact = 1;
  //add = what is added to the summation for each term
  double add = x;
  //while loop till what is added is smaller than EPSILON which is defined in the beginning of the code
  while (add > EPSILON) {
    //adds the term to the summations
    sum += add;
    //increases the term
    n += 1;
    //finds the factorial for n
    fact *= n;
    //sets y to x^n
    y = y * x;
    //solves what needs to be added next loop for while
    add = y / fact;
  }
  //returns the approximation of e^x
  return sum;
}

int main(int argc, char **argv) {
  //command line code
  int c = 0;
  //sets bools for the possible command line inputs
  bool sine = false;
  bool cosine = false;
  bool tangent = false;
  bool exponential = false;
  //check is to count how many are equal to true
  int check = 0;
  //this sets all the functions that are inputted to true
  while ((c = getopt(argc, argv, "sceat")) != -1) {
    switch (c) {

    //if s is inputted as argument than sine is set to true
    case 's':
      sine = true;
      check += 1;
      break;
    //if c is inputted as argument than cosine is set to true
    case 'c':
      cosine = true;
      check += 1;
      break;
    //if t is inputted as argument than tangent is set to true
    case 't':
      tangent = true;
      check += 1;
      break;
    //if e is inputted as argument than exponetial is set to true
    case 'e':
      exponential = true;
      check += 1;
      break;
    //if a is inputted as argument than all will be set to true
    case 'a':
      sine = true;
      cosine = true;
      tangent = true;
      exponential = true;
      check += 1;
      break;
    }
  }
  //if no argments are inputted it gives this error
  if (argc == 1) {
    puts("Error: no  arguments  supplied!");
    return -1;
  }
  //if there are more than one arguments given check would be bigger than 1 giving an error
  //since program is mutually exclusive
  if (check > 1) {
    puts("Error: To many arguemnts, only one allowed!");
    return -1;
  }
  //if sin test is requested
  if (sine) {
    printf("x        Sin             Library         Difference\n");
    printf("-        ---             -------         ----------\n");
    double x = -(2 * PI);
    //goes through range -2pi to 2pi with increments of pi/16
    while (x <= (2 * PI)) {
      double difference = Sin(x) - sin(x);
      printf("% 6.4lf\t% 10.8lf\t% 10.8lf\t% 12.10lf\n", x, Sin(x), sin(x),
          difference);
      x += PI / 16;
    }
  }
  //if cos test is requested
  if (cosine) {
    printf("x        Cos             Library         Difference\n");
    printf("-        ---             -------         ----------\n");
    double x = -(2 * PI);
    //goes through range -2pi to 2pi with increments of pi/16
    while (x <= (2 * PI)) {
      double difference = Cos(x) - cos(x);
      printf("% 6.4lf\t% 10.8lf\t% 10.8lf\t% 12.10lf\n", x, Cos(x), cos(x),
          difference);
      x += PI / 16;
    }
  }
  //if tan test is requested
  if (tangent) {
    printf("x        Tan             Library         Difference\n");
    printf("-        ---             -------         ----------\n");
    double x = -(PI / 3);
    //goes through range -pi/3 to pi/3 with increments of pi/16
    while (x <= (PI / 3)) {
      double difference = Tan(x) - tan(x);
      printf("% 6.4lf\t% 10.8lf\t% 10.8lf\t% 12.10lf\n", x, Tan(x), tan(x),
          difference);
      x += PI / 16;
    }
  }
  //if exp test is requeseted
  if (exponential) {
    printf("x        Exp             Library         Difference\n");
    printf("-        ---             -------         ----------\n");
    double x = 0;
    //goes through range 0 - 9 with increments .1
    while (x <= 9) {
      double difference = Exp(x) - exp(x);
      printf("% 6.4lf\t% 10.8lf\t% 10.8lf\t% 12.10lf\n", x, Exp(x), exp(x),
          difference);
      x += 0.1;
    }
  }

  return 0;
}
