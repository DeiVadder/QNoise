#ifndef QNOISE_H
#define QNOISE_H

#include <cstdint>
#include <vector>

class QNoise
{
public:
    QNoise(int64_t seed = 0);
    QNoise(const std::vector<short> &permutation);

    void setSeed(int64_t seed);

    //Return value between -1 and 1
    double noise(double x, double y);
    double noise(double x, double y, double z);
    double noise(double x, double y, double z, double w);

private:
    double extrapolate(int xsb, int ysb, double dx, double dy);
    double extrapolate(int xsb, int ysb, int zsb, double dx, double dy, double dz);
    double extrapolate(int xsb, int ysb, int zsb, int wsb, double dx, double dy, double dz, double dw);

private:

    std::vector<short> m_permutation;
    std::vector<short> m_permGradIndex3D;
};

#endif // QNOISE_H
