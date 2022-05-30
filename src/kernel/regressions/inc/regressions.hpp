/**
 * @file regressions.hpp
 * @author GerrFrog (ghaghal93@gmail.com)
 * @brief File contains all regression functions
 * @version 1.0
 * @date 2022-05-30
 * 
 * @copyright Copyright (c) 2022
 */

#pragma once

#ifndef KERNEL_REGRESSIONS_HEADER
#define KERNEL_REGRESSIONS_HEADER

#include "../../common.hpp"

/**
 * @brief All regression objects
 */
namespace Regressions
{
    /**
     * @brief Polynomial regression
     * 
     * @note https://gist.github.com/chrisengelsma/108f7ab0a746323beaaf7d6634cf4add
     * @note https://arachnoid.com/polysolve/
     * 
     * @tparam T T of coefficients
     */
    template <typename T>
    class Polynomial
    {
        private:
            /**
             * @brief Container for coefficients
             * 
             * @note i - number of element in array:
             * m_coeffs[i] * x ^ i
             */
            vector<T> m_coeffs { 0 };

            /**
             * @brief Is coefficients calculated?
             */
            bool m_calculated { false };

            /**
             * @brief Calculate coefficients
             * 
             * @param x X value (measured)
             * @param y Y value (real)
             * @param power The maximum power for calculation
             * @return true 
             * @return false 
             */
            bool resolve(
                vector<T>& x,
                vector<T>& y,
                const int power
            )
            {
                if (x.size() != y.size())
                    return false;

                if (x.size() == 0 || y.size() == 0)
                    return false;

                size_t N = x.size();

                int n = power;
                int np1 = n + 1;
                int np2 = n + 2;
                int tnp1 = 2 * n + 1;

                T tmp;

                vector<T> X(tnp1);
                for (int i = 0; i < tnp1; ++i)
                {
                    X[i] = 0;
                    for (size_t j = 0; j < N; ++j)
                        X[i] += (T)pow(x[j], i);
                }

                vector<T> a(np1);

                vector<vector<T>> B(np1, vector<T>(np2, 0));

                for (int i = 0; i <= n; ++i)
                    for (int j = 0; j <= n; ++j)
                        B[i][j] = X[i + j];

                vector<T> Y(np1);
                for (int i = 0; i < np1; ++i)
                {
                    Y[i] = (T)0;
                    for (size_t j = 0; j < N; ++j)
                    {
                        Y[i] += (T)pow(x[j], i) * y[j];
                    }
                }

                for (int i = 0; i <= n; ++i)
                    B[i][np1] = Y[i];

                n += 1;
                int nm1 = n - 1;

                for (int i = 0; i < n; ++i)
                    for (int k = i + 1; k < n; ++k)
                        if (B[i][i] < B[k][i])
                            for (int j = 0; j <= n; ++j)
                            {
                                tmp = B[i][j];
                                B[i][j] = B[k][j];
                                B[k][j] = tmp;
                            }

                for (int i = 0; i < nm1; ++i)
                    for (int k = i + 1; k < n; ++k)
                    {
                        T t = B[k][i] / B[i][i];
                        for (int j = 0; j <= n; ++j)
                            B[k][j] -= t * B[i][j];
                    }

                for (int i = nm1; i >= 0; --i)
                {
                    a[i] = B[i][n];
                    for (int j = 0; j < n; ++j)
                        if (j != i)
                            a[i] -= B[i][j] * a[j]; 
                    a[i] /= B[i][i];
                }

                m_coeffs.resize(a.size());
                for (size_t i = 0; i < a.size(); ++i)
                    m_coeffs[i] = a[i];

                return true;
            }

        public:
            /**
             * @brief Construct a new Polynomial object
             * 
             * @param x X value (measured)
             * @param y Y value (real)
             * @param power The maximum power for calculation
             */
            Polynomial(
                vector<T>& x,
                vector<T>& y,
                const int power
            ) : m_calculated(
                    resolve(x, y, power)
                )
            { }

            /**
             * @brief Calculate the fitted value using coefficients
             * 
             * @param value Measured value
             * @return T 
             */
            T calculate(T value)
            {
                double new_value = 0;
                for (size_t i = 0; i < m_coeffs.size(); i++)
                {
                    new_value += m_coeffs[i] * pow(value, i);
                }
                return new_value;
            }

            /**
             * @brief Get the coeffs object
             * 
             * @return vector<T> 
             */
            vector<T> get_coeffs() { return m_coeffs; }
    };
}

#endif