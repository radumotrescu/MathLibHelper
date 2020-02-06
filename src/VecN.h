#ifndef Vec3_h_include
#define Vec3_h_include

#include <array>
#include <type_traits>

namespace MathLib
{
    template <class T, uint64_t size>
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

        VecN operator*(VecN rhs) const
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

        VecN operator/(VecN rhs) const
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
                if (rhs.m_data[i] != m_data[i])
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

        template <typename = std::enable_if_t<size == 3>>
        VecN Cross(const VecN& rhs)
        {
            return { Y()* rhs.Z() - Z() * rhs.Y(),
                    -(X() * rhs.Z() - Z() * rhs.X()),
                    X()* rhs.Y() - Y() * rhs.X() };
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

        template <typename = std::enable_if_t<size >= 1>>
        T X() const
        {
            return m_data[0];
        }

        template <typename = std::enable_if_t<size >= 2>>
        T Y() const
        {
            return m_data[1];
        }

        template <typename = std::enable_if_t<size >= 3>>
        T Z() const
        {
            return m_data[2];
        }

        template <typename = std::enable_if_t<size >= 4>>
        T W() const
        {
            return m_data[3];
        }

        T operator[](int index) const
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