#include "main.hpp"

int main(int argc, char *argv[])
{
    vector<double> x = {
        35.13, 35.56, 34.18, 34.38, 34.33, 35.04, 33.99, 32.65
    };
    vector<double> y = {
        36.6, 36.7, 36.3, 36.4, 36.4, 36.6, 36.3, 36.0
    };

    int power = 3;

    Regressions::Polynomial<double> polynomial(x, y, power);

    vector<double> coeffs = polynomial.get_coeffs();
    for (auto coeff : coeffs)
        cout << "Coeff : " << coeff << endl;
    
    double value = 34.5;

    cout 
        << "Calculated" << endl
        << "Value: " << value << endl
        << "New value: " << polynomial.calculate(value) << endl
    << endl;
}