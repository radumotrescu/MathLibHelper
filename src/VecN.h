#ifndef Vec3_h_include
#define Vec3_h_include

#include <array>
#include <type_traits>
#include <numeric>
#include <cmath>

namespace MathLib
{
    template <typename IntegralType>
    typename std::enable_if<std::is_integral<IntegralType>::value, bool>::type
        equal(const IntegralType& t1, const IntegralType& t2)
    {
        return t1 == t2;
    }


    template <typename FloatingType>
    typename std::enable_if<std::is_floating_point<FloatingType>::value, bool>::type
        equal(const FloatingType& f1, const FloatingType& f2) {
        return std::fabs(f1 - f2) < std::numeric_limits<FloatingType>::epsilon();
    }


    template <typename T, uint64_t size>
    struct VecN
    {
        VecN()
        {
        }

        VecN(std::initializer_list<T> data)
        {
            int i = 0;
            for (const auto& x : data)
            {
                m_data[i++] = x;
            }
        }


        VecN(const std::array<T, size>& data)
            :m_data(data)
        {
        }

        VecN(VecN&& other)
        {
            m_data = other.m_data;
        }

        VecN& operator=(VecN&& other)
        {
            m_data = other.m_data;
            return *this;
        }

        VecN(const VecN& other)
        {
            m_data = other.m_data;
        }

        VecN& operator=(const VecN& other)
        {
            m_data = other.m_data;
            return *this;
        }

        VecN operator+(const VecN& rhs) const
        {
            auto data = std::array<double, size>();
            for (auto i = 0; i < size; i++)
                data[i] = m_data[i] + rhs.m_data[i];
            return { data };
        };

        VecN operator-(const VecN& rhs) const
        {
            auto data = std::array<double, size>();
            for (auto i = 0; i < size; i++)
                data[i] = m_data[i] - rhs.m_data[i];
            return { data };
        };

        VecN operator*(double scalar) const
        {
            auto data = std::array<double, size>();
            for (auto i = 0; i < size; i++)
                data[i] = m_data[i] * scalar;
            return { data };
        };

        VecN operator*(const VecN& rhs) const
        {
            auto data = std::array<double, size>();
            for (auto i = 0; i < size; i++)
                data[i] = m_data[i] * rhs.m_data[i];
            return { data };
        }

        VecN operator/(double scalar) const
        {
            auto data = std::array<double, size>();
            for (auto i = 0; i < size; i++)
                data[i] = m_data[i] / scalar;
            return { data };
        }

        VecN operator/(const VecN& rhs) const
        {
            auto data = std::array<double, size>();
            for (auto i = 0; i < size; i++)
                data[i] = m_data[i] / rhs.m_data[i];
            return { data };
        }

        bool operator==(const VecN& rhs) const
        {
            auto data = std::array<double, size>();
            for (auto i = 0; i < size; i++)
                if (!equal(rhs.m_data[i], m_data[i]))
                    return false;
            return true;
        };


        double Dot(const VecN& rhs) const
        {
            auto dotProduct = 0.;
            for (auto i = 0; i < size; i++)
                dotProduct += m_data[i] * rhs.m_data[i];
            return dotProduct;
        }

        // this version will only perform with Clang
        /*
        template <typename = std::enable_if<size == 3>>
        inline VecN Cross(const VecN& rhs)
        {
            return { Y() * rhs[2] - Z() * rhs[1],
                    -(X() * rhs[2] - Z() * rhs[0]),
                    X()* rhs[1] - Y() * rhs[0] };
        }
        */


        template <typename = std::enable_if<size == 3>>
        VecN Cross(const VecN& rhs)
        {
            return { m_data[1]* rhs[2] - m_data[2] * rhs[1],
                    -(m_data[0] * rhs[2] - m_data[2] * rhs[0]),
                    m_data[0]* rhs[1] - m_data[1] * rhs[0] };
        }

        void Normalize()
        {
            auto sumOfSquares = 0.;
            for (const auto& data : m_data)
                sumOfSquares += data * data;
            const auto norm = sqrt(sumOfSquares);
            for (auto& data : m_data)
                data /= norm;
        }

        template <typename = std::enable_if<size >= 1 >>
        inline const T& X() const
        {
            return m_data[0];
        }

        template <typename = std::enable_if<size >= 2 >>
        inline const T& Y() const
        {
            return m_data[1];
        }

        template <typename = std::enable_if<size >= 3 >>
        inline const T& Z() const
        {
            return m_data[2];
        }

        template <typename = std::enable_if<size >= 4 >>
        inline const T& W() const
        {
            return m_data[3];
        }

        const T& operator[](int index) const
        {
            return m_data[index];
        }

        std::array<T, size> m_data;
    };

    using Vec2f = VecN<double, 2>;
    using Vec3f = VecN<double, 3>;
    using Vec4f = VecN<double, 4>;

    using Vec2i = VecN<int, 2>;
    using Vec3i = VecN<int, 3>;
    using Vec4i = VecN<int, 4>;
}

#endif