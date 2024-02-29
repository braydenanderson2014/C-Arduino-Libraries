#ifndef MATH_h
#define MATH_h

#define MYPI 3.14159265358979323846
#define MYMAX_ITERATIONS 10

double Factorial(int n) {
  double fact = 1.0;
    for (int i = 1; i <= n; i++) {
        fact *= i;
    }
    return fact;
}

double Power(double base, int exp) {
  double result = 1.0;
  for (int i = 0; i < exp; i++) {
    result *= base;
  }
  return result;
}

double Sin(double x) {
    x = x * (MYPI / 180.0); // Convert to radians
    double sum = 0.0;
    for(int i = 0; i < MYMAX_ITERATIONS; i++) {
        double term = Power(-1, i) * Power(x, 2 * i + 1) / Factorial(2 * i + 1);
        sum += term;
    }
    return sum;
}

double Cos(double x) {
    x = x * (MYPI / 180.0); // Convert to radians
    double sum = 0.0;
    for(int i = 0; i < MYMAX_ITERATIONS; i++) {
        double term = Power(-1, i) * Power(x, 2 * i) / Factorial(2 * i);
        sum += term;
    }
    return sum;
}

double Tan(double x) {
    return Sin(x) / Cos(x);
}

double Cot(double x) {
    return 1 / Tan(x);
}

double Sec(double x) {
    return 1 / Cos(x);
}

double CSC(double x) {
    return 1 / Sin(x);
}

double arcsin(double x) {
    double sum = 0.0;
    for(int i = 0; i < MYMAX_ITERATIONS; i++) {
        double term = Factorial(2 * i) * Power(x, 2 * i + 1) / (Power(4, i) * Power(Factorial(i), 2) * (2 * i + 1));
        sum += term;
    }
    return sum;
}

double arccos(double x) {
    return MYPI / 2 - arcsin(x);
}

double arctan(double x) {
    double sum = 0.0;
    for(int i = 0; i < MYMAX_ITERATIONS; i++) {
        double term = Power(-1, i) * Power(x, 2 * i + 1) / (2 * i + 1);
        sum += term;
    }
    return sum;
}

double arccot(double x) {
    return MYPI / 2 - arctan(x);
}

double arcsec(double x) {
    return arccos(1 / x);
}

double arccsc(double x) {
    return arcsin(1 / x);
}

double sinh(double x) {
    double sum = 0.0;
    for(int i = 0; i < MYMAX_ITERATIONS; i++) {
        double term = Power(x, 2 * i + 1) / Factorial(2 * i + 1);
        sum += term;
    }
    return sum;
}

double cosh(double x) {
    double sum = 0.0;
    for(int i = 0; i < MYMAX_ITERATIONS; i++) {
        double term = Power(x, 2 * i) / Factorial(2 * i);
        sum += term;
    }
    return sum;
}

double tanh(double x) {
    return sinh(x) / cosh(x);
}

double coth(double x) {
    return 1 / tanh(x);
}

double sech(double x) {
    return 1 / cosh(x);
}

double csch(double x) {
    return 1 / sinh(x);
}

double arcsinh(double x) {
    double sum = 0.0;
    for(int i = 0; i < MYMAX_ITERATIONS; i++) {
        double term = Factorial(2 * i) * Power(x, 2 * i + 1) / (Power(4, i) * Power(Factorial(i), 2) * (2 * i + 1));
        sum += term;
    }
    return sum;
}

double Log(double x) {
    double sum = 0.0;
    for(int i = 1; i < MYMAX_ITERATIONS; i++) {
        double term = Power(-1, i + 1) * Power(x - 1, i) / i;
        sum += term;
    }
    return sum;
}

double EXP(double x) {
    double sum = 0.0;
    for(int i = 0; i < MYMAX_ITERATIONS; i++) {
        double term = Power(x, i) / Factorial(i);
        sum += term;
    }
    return sum;
}

double Log10(double x) {
    return Log(x) / Log(10);
}

double Sqrt(double x) {
    double guess = x / 2;
    for(int i = 0; i < MYMAX_ITERATIONS; i++) {
        guess = (guess + x / guess) / 2;
    }
    return guess;
}

double Cbrt(double x) {
    double guess = x / 2;
    for(int i = 0; i < MYMAX_ITERATIONS; i++) {
        guess = (2 * guess + x / (guess * guess)) / 3;
    }
    return guess;
}

double Pow(double base, double exp) {
    return exp * Log(base);
}

double Log2(double x) {
    return Log(x) / Log(2);
}

double Logn(double x, double n) {
    return Log(x) / Log(n);
}

double ABS(double x) {
    return x < 0 ? -x : x;
}

double Ceil(double x) {
    return (int)x + 1;
}

double Floor(double x) {
    return (int)x;
}






#endif // MATH_h