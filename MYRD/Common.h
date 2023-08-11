#pragma once

#include <math.h>
#include <functional>

typedef unsigned int BYTECOLOR;
#define PI 3.141592654
#define TWO_PI (2.0* MY_PI)

struct Handness
{
    enum Enum
    {
        Left,
        Right,
    };
};
struct Vector2f
{
    // Default Constructor
    Vector2f()
    {
        X = 0.0f;
        Y = 0.0f;
    }
    // Variable Set Constructor
    Vector2f(float X_, float Y_)
    {
        X = X_;
        Y = Y_;
    }
    // Bool Equals Operator Overload
    bool operator==(const Vector2f& other) const
    {
        return (this->X == other.X && this->Y == other.Y);
    }
    // Bool Not Equals Operator Overload
    bool operator!=(const Vector2f& other) const
    {
        return !(this->X == other.X && this->Y == other.Y);
    }
    // Addition Operator Overload
    Vector2f operator+(const Vector2f& right) const
    {
        return Vector2f(this->X + right.X, this->Y + right.Y);
    }
    // Subtraction Operator Overload
    Vector2f operator-(const Vector2f& right) const
    {
        return Vector2f(this->X - right.X, this->Y - right.Y);
    }
    // Float Multiplication Operator Overload
    Vector2f operator*(const float& other) const
    {
        return Vector2f(this->X * other, this->Y * other);
    }

    // Positional Variables
    float X;
    float Y;
};

struct Vector4f
{
    // Default Constructor
    Vector4f()
    {
        X = 0.0f;
        Y = 0.0f;
        Z = 0.0f;
        W = 1.0f;
    }
    // Variable Set Constructor

    Vector4f(float value)
    {
        X = value;
        Y = value;
        Z = value;
        W = 1.0f;
    }

    Vector4f(float X_, float Y_, float Z_)
    {
        X = X_;
        Y = Y_;
        Z = Z_;
        W = 1.0f;
    }

    // Addition Operator Overload
    Vector4f operator+(const Vector4f& right) const
    {
        return Vector4f(this->X + right.X, this->Y + right.Y, this->Z + right.Z);
    }

    // Subtraction Operator Overload
    Vector4f operator-(const Vector4f& right) const
    {
        return Vector4f(this->X - right.X, this->Y - right.Y, this->Z - right.Z);
    }

    // Float Multiplication Operator Overload
    Vector4f operator*(const float& other) const
    {
        return Vector4f(this->X * other, this->Y * other, this->Z * other);
    }

    // Float Division Operator Overload
    Vector4f operator/(const float& other) const
    {
        return Vector4f(this->X / other, this->Y / other, this->Z / other);
    }

    //Homogeneous division
    void Homogeneous()
    {
        if (W != 0.0)
        {
            X = X / W;
            Y = Y / W;
            Z = Z / W;
            W = 1.0;
        }
    }

    // Positional Variables
    float X;
    float Y;
    float Z;
    float W;
};

//––æÿ’Û
struct Matrix4f
{
    // Default Constructor
    Matrix4f() = default;


    // Variable Set Constructor
    Matrix4f(const float value)
    {
        matrix[0][0] = matrix[0][1] = matrix[0][2] = matrix[0][3] = value;
        matrix[1][0] = matrix[1][1] = matrix[1][2] = matrix[1][3] = value;
        matrix[2][0] = matrix[2][1] = matrix[2][2] = matrix[2][3] = value;
        matrix[3][0] = matrix[3][1] = matrix[3][2] = matrix[3][3] = value;
    }

    Matrix4f(const float values[])
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
            {
                matrix[i][j] = values[i * 4 + j];
            }
    }

    // Addition Operator Overload
    Matrix4f operator+(const Matrix4f& right) const
    {
        Matrix4f result;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
            {
                result.matrix[i][j] = matrix[i][j] + right.matrix[i][j];
            }
        return result;
    }

    // Subtraction Operator Overload
    Matrix4f operator-(const Matrix4f& right) const
    {
        Matrix4f result;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
            {
                result.matrix[i][j] = matrix[i][j] - right.matrix[i][j];
            }
        return result;
    }

    // Float Multiplication Operator Overload
    Matrix4f operator*(const float& other) const
    {
        Matrix4f result;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                result.matrix[i][j] = matrix[i][j] * other;
            }
        return result;
    }

    // Matrix Multiplication Operator Overload
    Matrix4f operator*(const Matrix4f& other) const
    {
        Matrix4f result;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
            {
                result.matrix[i][j] = matrix[i][0] * other.matrix[0][j] +
                    matrix[i][1] * other.matrix[1][j] +
                    matrix[i][2] * other.matrix[2][j] +
                    matrix[i][3] * other.matrix[3][j];
            }
        return result;
    }

    // Variables
    float matrix[4][4] = { 0 };
};

namespace math
{
    // Vector Cross Product
    static Vector4f Cross(const Vector4f& a, const Vector4f& b)
    {
        return Vector4f(a.Y * b.Z - a.Z * b.Y,
            a.Z * b.X - a.X * b.Z,
            a.X * b.Y - a.Y * b.X);
    }

    // Vector Dot Product
    static float Dot(const Vector4f& a, const Vector4f& b)
    {
        return (a.X * b.X) + (a.Y * b.Y) + (a.Z * b.Z);
    }

    // Vector4 Magnitude Calculation
    static float Magnitude(const Vector4f& a)
    {
        return(sqrtf(powf(a.X, 2) + powf(a.Y, 2) + powf(a.Z, 2)));
    }

    // Normalize the Vector4
    static Vector4f Normalize(const Vector4f& in)
    {
        float length = math::Magnitude(in);
        return in / length;
    }

    // Degree to Rad
    static inline float toRad(float deg)
    {
        return deg * PI / 180.0f;
    }

    // Clamp function
    static inline float clamp(float x, float min, float max)
    {
        return (x < min) ? min : ((x > max) ? max : x);
    }


    // Get the BitColor from a (0,1) rgb color
    static BYTECOLOR getBitColor(Vector4f rgba)
    {
        int R = (int)(rgba.X * 255.0f);
        int G = (int)(rgba.Y * 255.0f);
        int B = (int)(rgba.Z * 255.0f);
        R = clamp(R, 0, 255);
        G = clamp(G, 0, 255);
        B = clamp(B, 0, 255);

        return (R << 16) | (G << 8) | (B);
    }
}

struct Vertex
{
    Vertex() = default;

    // Position Vector
    Vector4f position;

    // Normal Vector
    Vector4f normal;

    // Color Vector
    Vector4f vertexColor;

    // Texture Coordinate Vector
    Vector2f textureCoordinate;
};

struct Triangle
{
    Triangle() = default;

    Triangle(const Vertex& a, const Vertex& b, const Vertex& c)
    {
        v[0] = a;
        v[1] = b;
        v[2] = c;
    }

    void setVertex(int index, Vertex ver)
    {
        v[index] = ver;
    }
    //Vertex Array
    Vertex v[3];

};

// construct Triangle from given vertexbuffer
static Triangle GetTriangle(const Vertex* vertex, int a, int b, int c)
{
    return Triangle(vertex[a], vertex[b], vertex[c]);
}


struct Light
{
    Light() = default;
    Light(const Vector4f& pos, const Vector4f& inten) :
        position(pos), intensity(inten) {};

    Vector4f position;
    Vector4f intensity;
};


// Vector2 Multiplication Opertor Overload
static Vector2f operator*(const float& left, const Vector2f& right)
{
    return Vector2f(right.X * left, right.Y * left);
}

// Vector4 Multiplication Opertor Overload
static Vector4f operator*(const float& left, const Vector4f& right)
{
    return Vector4f(right.X * left, right.Y * left, right.Z * left);
}

// Matrix Vector4 Multiplication Opertor Overload
static Vector4f operator*(const Matrix4f& m, const Vector4f& v)
{
    Vector4f result;
    result.X = m.matrix[0][0] * v.X + m.matrix[0][1] * v.Y + m.matrix[0][2] * v.Z + m.matrix[0][3] * v.W;
    result.Y = m.matrix[1][0] * v.X + m.matrix[1][1] * v.Y + m.matrix[1][2] * v.Z + m.matrix[1][3] * v.W;
    result.Z = m.matrix[2][0] * v.X + m.matrix[2][1] * v.Y + m.matrix[2][2] * v.Z + m.matrix[2][3] * v.W;
    result.W = m.matrix[3][0] * v.X + m.matrix[3][1] * v.Y + m.matrix[3][2] * v.Z + m.matrix[3][3] * v.W;
    return result;
}



namespace algorithm
{

    // A test to see if P1 is on the same side as P2 of a line segment ab
    static bool SameSide(Vector2f p1, Vector2f p2, Vector2f a, Vector2f b)
    {
        float cp1 = (b - a).X * (p1 - a).Y - (b - a).Y * (p1 - a).X;
        float cp2 = (b - a).X * (p2 - a).Y - (b - a).Y * (p2 - a).X;
        if (cp1 * cp2 >= 0)
            return true;
        else
            return false;
    }

    // is the point on the right side of each edge(clockwize)
    static bool insideTriangle(float x, float y, const Vertex* v)
    {
        Vector2f P = Vector2f(x, y);
        Vector2f A = Vector2f(v[0].position.X, v[0].position.Y);
        Vector2f B = Vector2f(v[1].position.X, v[1].position.Y);
        Vector2f C = Vector2f(v[2].position.X, v[2].position.Y);

        return SameSide(P, A, B, C)
            && SameSide(P, B, C, A)
            && SameSide(P, C, A, B);
    }

    static Vector4f getTriNormal(Vector4f a, Vector4f b, Vector4f c)
    {
        Vector4f AB = b - a;
        Vector4f BC = c - b;
        Vector4f normal = math::Normalize(math::Cross(AB, BC));
        return normal;
    }

    // get the BaryCentric weight of each vertex
    static std::tuple<float, float, float>computeBaryCentric(float x, float y, const Vector4f* v)
    {
        float alpha = (x * (v[1].Y - v[2].Y) + (v[2].X - v[1].X) * y + v[1].X * v[2].Y - v[2].X * v[1].Y) / (v[0].X * (v[1].Y - v[2].Y) + (v[2].X - v[1].X) * v[0].Y + v[1].X * v[2].Y - v[2].X * v[1].Y);
        float beta = (x * (v[2].Y - v[0].Y) + (v[0].X - v[2].X) * y + v[2].X * v[0].Y - v[0].X * v[2].Y) / (v[1].X * (v[2].Y - v[0].Y) + (v[0].X - v[2].X) * v[1].Y + v[2].X * v[0].Y - v[0].X * v[2].Y);
        float gamma = (x * (v[0].Y - v[1].Y) + (v[1].X - v[0].X) * y + v[0].X * v[1].Y - v[1].X * v[0].Y) / (v[2].X * (v[0].Y - v[1].Y) + (v[1].X - v[0].X) * v[2].Y + v[0].X * v[1].Y - v[1].X * v[0].Y);
        return { alpha, beta, gamma };
    }

    // Get an identity Matrix4x4
    static Matrix4f GetIdentity()
    {
        Matrix4f result;
        result.matrix[0][0] = result.matrix[1][1] = result.matrix[2][2] = result.matrix[3][3] = 1.0f;
        return result;
    }

    // Generate a cross produect normal for a triangle
    static Vector4f GenTriNormal(const Vector4f& t1, const Vector4f& t2, const Vector4f& t3)
    {
        Vector4f u = t2 - t1;
        Vector4f v = t3 - t1;
        Vector4f normal = math::Cross(u, v);
        return normal;
    }

    // Generate the rotation matrix from Rodrigues' rotation formula
    static Matrix4f GetRotation(const Vector4f& axis, const float angle)
    {
        float radian = angle * PI / 180.0;
        float temp[16] = { 0,-axis.Z, axis.Y,0,
            axis.Z,0,-axis.X,0,
            -axis.Y,axis.X,0,0
            ,0,0,0,1 };
        Matrix4f Rk(temp);
        Matrix4f I = GetIdentity();
        Matrix4f Rk2 = Rk * Rk;
        Matrix4f rotation = I + Rk2 * (1 - cos(radian)) + Rk * sin(radian);

        return rotation;
    }

    // Generate the translation matrix
    static Matrix4f GetTranslation(const float x, const float y, const float z)
    {
        Matrix4f translation = GetIdentity();
        translation.matrix[0][3] = x;
        translation.matrix[1][3] = y;
        translation.matrix[2][3] = z;
        return translation;
    }

    // Generate the View matrix
    static Matrix4f GetView(const Vector4f& eye, const Vector4f& target, const Vector4f& up, Handness::Enum handness)
    {
        Vector4f xAxis, yAxis, zAxis;
        zAxis = math::Normalize(Handness::Right == handness ? (eye - target) : (target - eye));
        xAxis = math::Normalize(math::Cross(up, zAxis));
        yAxis = math::Cross(zAxis, xAxis);

        Matrix4f view;
        view.matrix[0][0] = xAxis.X;
        view.matrix[0][1] = xAxis.Y;
        view.matrix[0][2] = xAxis.Z;
        view.matrix[0][3] = -math::Dot(xAxis, eye);

        view.matrix[1][0] = yAxis.X;
        view.matrix[1][1] = yAxis.Y;
        view.matrix[1][2] = yAxis.Z;
        view.matrix[1][3] = -math::Dot(yAxis, eye);

        view.matrix[2][0] = zAxis.X;
        view.matrix[2][1] = zAxis.Y;
        view.matrix[2][2] = zAxis.Z;
        view.matrix[2][3] = -math::Dot(zAxis, eye);

        view.matrix[3][0] = 0.0f;
        view.matrix[3][1] = 0.0f;
        view.matrix[3][2] = 0.0f;
        view.matrix[3][3] = 1.0f;

        return view;
    }

    // Generate the Projection matrix Z[0,1] version
    static Matrix4f GetProjection(float fovy, float aspect, float near, float far, Handness::Enum handness) {
        const float a = 1.0f / tan(math::toRad(fovy) * 0.5f);
        const float b = a / aspect;
        const float diff = far - near;

        float fax = 1.0f / (float)tan(fovy * 0.5f);
        Matrix4f projection;

        projection.matrix[0][0] = b;
        projection.matrix[1][1] = a;
        projection.matrix[2][2] = (Handness::Right == handness) ? -far / diff : far / diff;
        projection.matrix[2][3] = -near * far / diff;
        projection.matrix[3][2] = (Handness::Right == handness) ? -1.0f : 1.0f;

        return projection;
    }
}

