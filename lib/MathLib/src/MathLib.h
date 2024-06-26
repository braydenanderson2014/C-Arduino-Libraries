#ifndef MATHLIB_h
#define MATHLIB_h

#define MYPI 3.14159265358979323846
#define MYMAX_ITERATIONS 150

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

int Prime(int n) {
  int count = 0;
  for (int i = 2; i < n; i++) {
    if (n % i == 0) {
      count++;
    }
  }
  return count == 0;
}

bool isPrime(int n) {
  int count = 0;
  for (int i = 2; i < n; i++) {
    if (n % i == 0) {
      count++;
    }
  }
  return count == 0;
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

//REFRACTORED to not use Factorial... this is to ensure no overflow occurs.
double Arcsin(double x) {
    double term = x;
    double sum = term;
    for(int i = 1; i < MYMAX_ITERATIONS; i++) {
        term *= (x * x * (2 * i - 1) * (2 * i - 1)) / (4 * i * i * (2 * i + 1));
        sum += term;
    }
    return sum;
}

double Arccos(double x) {
    return MYPI / 2 - Arcsin(x);
}

double Arctan(double x) {
    double sum = 0.0;
    for(int i = 0; i < MYMAX_ITERATIONS; i++) {
        double term = Power(-1, i) * Power(x, 2 * i + 1) / (2 * i + 1);
        sum += term;
    }
    return sum;
}

double Arccot(double x) {
    return MYPI / 2 - Arctan(x);
}

double Arcsec(double x) {
    return Arccos(1 / x);
}

double Arccsc(double x) {
    return Arcsin(1 / x);
}

double Ainh(double x) {
    double sum = 0.0;
    for(int i = 0; i < MYMAX_ITERATIONS; i++) {
        double term = Power(x, 2 * i + 1) / Factorial(2 * i + 1);
        sum += term;
    }
    return sum;
}

double Cosh(double x) {
    double sum = 0.0;
    for(int i = 0; i < MYMAX_ITERATIONS; i++) {
        double term = Power(x, 2 * i) / Factorial(2 * i);
        sum += term;
    }
    return sum;
}

double Sinh(double x) {
    double sum = 0.0;
    for(int i = 0; i < MYMAX_ITERATIONS; i++) {
        double term = Power(x, 2 * i + 1) / Factorial(2 * i + 1);
        sum += term;
    }
    return sum;
}

double Tanh(double x) {
    return Sinh(x) / Cosh(x);
}

double Coth(double x) {
    return 1 / Tanh(x);
}

double Sech(double x) {
    return 1 / Cosh(x);
}

double Csch(double x) {
    return 1 / Sinh(x);
}

//REFRACTORED to not use Factorial... this is to ensure no overflow occurs.
double Arcsinh(double x) {
    return log(x + sqrt(x*x + 1));
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

int ABS(int x) {
    return x < 0 ? -x : x;
}
 

double Round(double x) {
    return (int)(x + 0.5);
}

double Ceil(double x) {
    return (int)x + 1;
}

double Floor(double x) {
    return (int)x;
}

int Random(int min, int max) {
    return min + static_cast<int>(static_cast<long long>(rand()) % (static_cast<long long>(max) - min + 1));
}

double Random(double min, double max) {
    return min + (max - min) * (static_cast<double>(rand()) / RAND_MAX);
}




#endif // MATHLIB_h