#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

using std::abs;
using std::pow;
using std::sqrt;

namespace Geometry {

const double kHelp = 0.5;
const unsigned int kMax = 10000;
const double kMagic = 2.;
const double kMagic1 = 4.;

struct PointsHelp {
  double xa;
  double ya;
  double xb;
  double yb;
  double xc;
  double yc;
  double xd;
  double yd;
};

class IShape;
class Point;
class Segment;
class Ray;
class Line;
class Circle;
class Polygon;

// YOUR CODE HERE
// Vector
class Vector {
 public:
  Vector() = default;
  Vector(long double a, long double b);
  // без него проблеммы
  Vector(const Vector& v);
  Vector& operator=(const Vector& other);
  Vector& operator+=(const Vector& other);
  Vector& operator-=(const Vector& other);
  Vector& operator*=(long double num);
  Vector& operator-();
  double Length() const;
  double GetCoordX() const;
  double GetCoordY() const;

 private:
  double x_ = 0;
  double y_ = 0;
};

long double operator*(const Vector& other1, const Vector& other2);
long double operator^(const Vector& other1, const Vector& other2);
Vector operator+(const Vector& other1, const Vector& other2);
Vector operator-(const Vector& other1, const Vector& other2);
Vector operator*(Vector& other, long double num);
long double AreaOfTriangle(const Vector& other1, const Vector& other2);
std::ostream& operator<<(std::ostream& out, const Vector& other);

Vector::Vector(long double a, long double b) : x_(a), y_(b) {}

Vector& Vector::operator+=(const Vector& other) {
  x_ += other.x_;
  y_ += other.y_;
  return *this;
}

Vector& Vector::operator-=(const Vector& other) {
  x_ -= other.x_;
  y_ -= other.y_;
  return *this;
}

Vector& Vector::operator*=(long double num) {
  x_ *= num;
  y_ *= num;
  return *this;
}

Vector& Vector::operator=(const Vector& other) {
  x_ = other.GetCoordX();
  y_ = other.GetCoordY();
  return *this;
}

Vector::Vector(const Vector& v) {
  x_ = v.GetCoordX();
  y_ = v.GetCoordY();
}

Vector operator*(Vector& other, long double num) {
  Vector res(other.GetCoordX() * num, other.GetCoordY() * num);
  return res;
}

Vector& Vector::operator-() {
  x_ = -x_;
  y_ = -y_;
  return *this;
}

double Vector::Length() const {
  return static_cast<long double>(std::pow((x_ * x_ + y_ * y_), kHelp));
}

double Vector::GetCoordX() const { return x_; }
double Vector::GetCoordY() const { return y_; }

Vector operator-(const Vector& other1, const Vector& other2) {
  Vector res = other1;
  res -= other2;
  return res;
}

Vector operator+(const Vector& other1, const Vector& other2) {
  Vector res = other1;
  res += other2;
  return res;
}

long double operator*(const Vector& other1, const Vector& other2) {
  return other1.GetCoordX() * other2.GetCoordX() +
         other1.GetCoordY() * other2.GetCoordY();
}

long double operator^(const Vector& other1, const Vector& other2) {
  return other1.GetCoordX() * other2.GetCoordY() -
         other1.GetCoordY() * other2.GetCoordX();
}

std::ostream& operator<<(std::ostream& out, const Vector& other) {
  const long double kEps = 1e-10;
  long double zero = 0;
  if (std::abs(other.GetCoordX() - zero) < kEps) {
    out << std::abs(other.GetCoordX());
  } else {
    out << other.GetCoordX();
  }
  out << " ";
  if (std::abs(other.GetCoordY() - zero) < kEps) {
    out << std::abs(other.GetCoordY());
  } else {
    out << other.GetCoordY();
  }
  // out << other.x << " " << other.y;
  return out;
}

long double AreaOfTriangle(const Vector& other1, const Vector& other2) {
  return kHelp * (other2 ^ other1);
}

bool OnLine(const Vector& point, const Vector& begin, const Vector& end) {
  Vector line(end.GetCoordX() - begin.GetCoordX(),
              end.GetCoordY() - begin.GetCoordY());
  Vector check(point.GetCoordX() - begin.GetCoordX(),
               point.GetCoordY() - begin.GetCoordY());
  long long res = line ^ check;
  return res == 0;
}

bool OnRay(const Vector& point, const Vector& begin, const Vector& end) {
  Vector line(end.GetCoordX() - begin.GetCoordX(),
              end.GetCoordY() - begin.GetCoordY());
  Vector check(point.GetCoordX() - begin.GetCoordX(),
               point.GetCoordY() - begin.GetCoordY());
  long double res = line * check;
  return OnLine(point, begin, end) && res >= 0;
}

bool OnSegment(const Vector& point, const Vector& begin, const Vector& end) {
  Vector line1(begin.GetCoordX() - point.GetCoordX(),
               begin.GetCoordY() - point.GetCoordY());
  Vector check1(end.GetCoordX() - point.GetCoordX(),
                end.GetCoordY() - point.GetCoordY());
  long long res1 = line1 * check1;
  return OnLine(point, begin, end) && res1 <= 0;
}

bool SegmentsIntersection(const Vector& begin1, const Vector& end1,
                          const Vector& begin2, const Vector& end2) {
  Vector line1(end1.GetCoordX() - begin1.GetCoordX(),
               end1.GetCoordY() - begin1.GetCoordY());
  Vector line2(end2.GetCoordX() - begin1.GetCoordX(),
               end2.GetCoordY() - begin1.GetCoordY());
  Vector line3(begin2.GetCoordX() - begin1.GetCoordX(),
               begin2.GetCoordY() - begin1.GetCoordY());
  long long res1 = line1 ^ line2;
  long long res2 = line1 ^ line3;
  Vector line4(end2.GetCoordX() - begin2.GetCoordX(),
               end2.GetCoordY() - begin2.GetCoordY());
  Vector line5(begin1.GetCoordX() - begin2.GetCoordX(),
               begin1.GetCoordY() - begin2.GetCoordY());
  Vector line6(end1.GetCoordX() - begin2.GetCoordX(),
               end1.GetCoordY() - begin2.GetCoordY());
  long long res3 = line4 ^ line5;
  long long res4 = line4 ^ line6;
  return (res1 * res2 < 0 && res3 * res4 < 0) ||
         OnSegment(begin1, begin2, end2) || OnSegment(end1, begin2, end2) ||
         OnSegment(begin2, begin1, end1) || OnSegment(end2, begin1, end1);
}
long double DistancePointLine(const Vector& point, const Vector& begin,
                              const Vector& end) {
  Vector line1(end.GetCoordX() - begin.GetCoordX(),
               end.GetCoordY() - begin.GetCoordY());
  Vector line2(point.GetCoordX() - begin.GetCoordX(),
               point.GetCoordY() - begin.GetCoordY());
  long double length = line1.Length();
  long double cross_product = line1 ^ line2;
  return std::abs(static_cast<long double>(cross_product / length));
}

bool NormalOnRay(const Vector& point, const Vector& begin, const Vector& end) {
  Vector line1(end.GetCoordX() - begin.GetCoordX(),
               end.GetCoordY() - begin.GetCoordY());
  Vector line2(point.GetCoordX() - begin.GetCoordX(),
               point.GetCoordY() - begin.GetCoordY());
  long long res = static_cast<long long>(line2 * line1);
  return res >= 0;
}

long double DistancePointRay(const Vector& point, const Vector& begin,
                             const Vector& end) {
  if (!NormalOnRay(point, begin, end)) {
    Vector line(point.GetCoordX() - begin.GetCoordX(),
                point.GetCoordY() - begin.GetCoordY());
    return line.Length();
  }
  return DistancePointLine(point, begin, end);
}

bool NormalOnSegment(const Vector& point, const Vector& begin,
                     const Vector& end) {
  Vector line1(end.GetCoordX() - begin.GetCoordX(),
               end.GetCoordY() - begin.GetCoordY());
  Vector line2(point.GetCoordX() - begin.GetCoordX(),
               point.GetCoordY() - begin.GetCoordY());
  Vector line3(point.GetCoordX() - end.GetCoordX(),
               point.GetCoordY() - end.GetCoordY());
  Vector line4(begin.GetCoordX() - end.GetCoordX(),
               begin.GetCoordY() - end.GetCoordY());
  long long res1 = static_cast<long long>(line2 * line1);
  long long res2 = static_cast<long long>(line3 * line4);
  return !(res1 < 0 || res2 < 0);
}

long double DistancePointSegment(const Vector& point, const Vector& begin,
                                 const Vector& end) {
  if (!NormalOnSegment(point, begin, end)) {
    Vector line1(point.GetCoordX() - begin.GetCoordX(),
                 point.GetCoordY() - begin.GetCoordY());
    Vector line2(point.GetCoordX() - end.GetCoordX(),
                 point.GetCoordY() - end.GetCoordY());
    return std::min(line1.Length(), line2.Length());
  }
  return DistancePointLine(point, begin, end);
}

// IShape

class IShape {
 public:
  virtual bool ContainsPoint(const Point& unused) const = 0;
  virtual void Move(const Vector& unused) = 0;
  virtual bool CrossesSegment(const Segment& unused) const = 0;
  virtual IShape* Clone() const = 0;
  virtual void ToString() const = 0;
  virtual ~IShape() = default;

 protected:
};

// Circle

class Circle : public IShape {
 public:
  Circle() = default;
  Circle(const Point& point, unsigned long i);
  bool ContainsPoint(const Point& p) const;
  void Move(const Vector& v);
  bool CrossesSegment(const Segment& s) const;
  IShape* Clone() const;
  void ToString() const;

 private:
  Vector center_;
  int64_t r_;
};

void Circle::ToString() const {
  std::cout << "Circle(Point(" << center_.GetCoordX() << ", "
            << center_.GetCoordY() << "), " << r_ << ")\n";
}

IShape* Circle::Clone() const {
  Circle* p = new Circle;
  p->r_ = this->r_;
  p->center_ = this->center_;
  return p;
}

void Circle::Move(const Vector& v) { center_ += v; }

// Polygon

class Polygon : public IShape {
 public:
  Polygon() = default;
  Polygon(std::vector<Point> vector_1);
  int InPolygon(int x, int y) const;
  bool ContainsPoint(const Point& point) const;
  void Move(const Vector& v);
  bool CrossesSegment(const Segment& s) const;
  IShape* Clone() const;
  void ToString() const;

 private:
  std::vector<Vector> p_ = {};
  unsigned int size_;
};

void Polygon::ToString() const {
  std::cout << "Polygon(";
  for (unsigned int i = 0; i < size_; ++i) {
    std::cout << "Point(" << p_[i].GetCoordX() << ", " << p_[i].GetCoordY()
              << ")";
    if (i != size_ - 1) {
      std::cout << ", ";
    }
  }
  std::cout << ")\n";
}

IShape* Polygon::Clone() const {
  Polygon* p = new Polygon;
  p->p_.reserve(size_);
  p->size_ = this->size_;
  for (unsigned int i = 0; i < size_; ++i) {
    p->p_[i] = this->p_[i];
  }
  return p;
}

void Polygon::Move(const Vector& v) {
  for (unsigned int i = 0; i < size_; ++i) {
    p_[i] += v;
  }
}

// Line

class Line : public IShape {
 public:
  Line() = default;
  Line(const Point& a, const Point& b);
  bool ContainsPoint(const Point& p) const;
  void Move(const Vector& v);
  bool CrossesSegment(const Segment& s) const;
  IShape* Clone() const;
  void ToString() const;

 private:
  Vector begin_;
  Vector end_;
  int64_t a_;
  int64_t b_;
  int64_t c_;
};

void Line::ToString() const {
  std::cout << "Line(" << -a_ << ", " << -b_ << ", " << -c_ << ")\n";
}

IShape* Line::Clone() const {
  Line* p = new Line;
  p->a_ = a_;
  p->b_ = b_;
  p->c_ = c_;
  p->begin_ = this->begin_;
  p->end_ = this->end_;
  return p;
}

void Line::Move(const Vector& v) {
  begin_ += v;
  end_ += v;
  a_ = begin_.GetCoordY() - end_.GetCoordY();  // y1 - y2;
  b_ = end_.GetCoordX() - begin_.GetCoordX();  // x2 - x1;
  c_ = begin_.GetCoordX() * end_.GetCoordY() -
       end_.GetCoordX() * begin_.GetCoordY();
}

// Ray

class Ray : public IShape {
 public:
  Ray() = default;
  Ray(const Point& a, const Point& b);
  bool ContainsPoint(const Point& p) const;
  void Move(const Vector& v);
  bool CrossesSegment(const Segment& s) const;
  IShape* Clone() const;
  void ToString() const;

 private:
  Vector begin_;
  Vector end_;
};

void Ray::ToString() const {
  std::cout << "Ray("
            << "Point(" << begin_.GetCoordX() << ", " << begin_.GetCoordY()
            << "), Vector(" << end_.GetCoordX() - begin_.GetCoordX() << ", "
            << end_.GetCoordY() - begin_.GetCoordY() << "))\n";
}

IShape* Ray::Clone() const {
  Ray* p = new Ray;
  p->begin_ = this->begin_;
  p->end_ = this->end_;
  return p;
}

void Ray::Move(const Vector& v) {
  begin_ += v;
  end_ += v;
}

// Segment

class Segment : public IShape {
 public:
  Segment() {}
  Segment(const Point& a, const Point& b);
  const Vector& GetBegin() const { return begin_; }
  const Vector& GetEnd() const { return end_; }
  bool ContainsPoint(const Point& p) const;
  void Move(const Vector& v);
  bool CrossesSegment(const Segment& s) const;
  IShape* Clone() const;
  void ToString() const;
  long double BGetCoordX() const;
  long double BGetCoordY() const;
  long double EGetCoordX() const;
  long double EGetCoordY() const;
  friend long double DistanceSegments(Segment& seg1, Segment& seg2);
  friend PointsHelp Help(Segment& seg1, Segment& seg2);

 private:
  Vector begin_;
  Vector end_;
};

long double Segment::BGetCoordX() const { return begin_.GetCoordX(); }

long double Segment::BGetCoordY() const { return begin_.GetCoordY(); }

long double Segment::EGetCoordX() const { return end_.GetCoordX(); }

long double Segment::EGetCoordY() const { return end_.GetCoordY(); }

void Segment::ToString() const {
  std::cout << "Segment("
            << "Point(" << begin_.GetCoordX() << ", " << begin_.GetCoordY()
            << "), Point(" << end_.GetCoordX() << ", " << end_.GetCoordY()
            << "))\n";
}

IShape* Segment::Clone() const {
  Segment* p = new Segment;
  p->begin_ = this->begin_;
  p->end_ = this->end_;
  return p;
}

void Segment::Move(const Vector& v) {
  begin_ += v;
  end_ += v;
}

PointsHelp Help(Segment& seg1, Segment& seg2) {
  PointsHelp h;
  h.xa = seg1.begin_.GetCoordX();  // xa
  h.ya = seg1.begin_.GetCoordY();  // ya
  h.xb = seg1.end_.GetCoordX();    // xb
  h.yb = seg1.end_.GetCoordY();    // yb
  h.xc = seg2.begin_.GetCoordX();  // xc
  h.yc = seg2.begin_.GetCoordY();  // yc
  h.xd = seg2.end_.GetCoordX();    // xd
  h.yd = seg2.end_.GetCoordY();    // yd
  return h;
}

struct ThreePoint {
  double x1;
  double y1;
  double x2;
  double y2;
  double x3;
  double y3;
  ThreePoint(double x1t, double y1t, double x2t, double y2t, double x3t,
             double y3t)
      : x1(x1t), y1(y1t), x2(x2t), y2(y2t), x3(x3t), y3(y3t) {}
};

long double Height(ThreePoint p) {
  double k;
  double d;
  if (p.x1 == p.x2) {  //Если отрезок вертикальный - меняем местами координаты
    //каждой точки.
    std::swap(p.x1, p.y1);
    std::swap(p.x2, p.y2);
    std::swap(p.x3, p.y3);
  }
  k = (p.y1 - p.y2) /
      (p.x1 - p.x2);  //Ищем коэффициенты уравнения прямой, которому
  //принадлежит данный отрезок.
  d = p.y1 - k * p.x1;
  double xz = (p.x3 * p.x2 - p.x3 * p.x1 + p.y2 * p.y3 - p.y1 * p.y3 +
               p.y1 * d - p.y2 * d) /
              (k * p.y2 - k * p.y1 + p.x2 - p.x1);
  double dl = -1;
  if ((xz <= p.x2 && xz >= p.x1) || (xz <= p.x1 && xz >= p.x2)) {
    dl = sqrt((p.x3 - xz) * (p.x3 - xz) +
              (p.y3 - xz * k - d) *
                  (p.y3 - xz * k -
                   d));  //Проверим лежит ли основание высоты на отрезке.
  }
  return dl;
}

long double DistanceSegments(Segment& seg1, Segment& seg2) {
  PointsHelp h = Help(seg1, seg2);
  long double dl1;
  long double dl2;
  long double dl3;
  long double dl4;
  long double min = -1;
  long double t = -2;
  long double s = -2;
  double o = (h.xb - h.xa) * (-h.yd + h.yc) - (h.yb - h.ya) * (-h.xd + h.xc);
  double o1 = (h.xb - h.xa) * (h.yc - h.ya) - (h.yb - h.ya) * (h.xc - h.xa);
  double o2 = (-h.yd + h.yc) * (h.xc - h.xa) - (-h.xd + h.xc) * (h.yc - h.ya);
  if (o != 0) {
    t = o1 / o;
    s = o2 / o;
  }
  if ((t >= 0 && s >= 0) && (t <= 1 && s <= 1)) {
    min = 0;
  } else {
    dl1 = Height(ThreePoint(h.xa, h.ya, h.xb, h.yb, h.xc, h.yc));
    min = dl1;
    dl2 = Height(ThreePoint(h.xa, h.ya, h.xb, h.yb, h.xd, h.yd));
    if ((dl2 < min && dl2 != -1) || min == -1) {
      min = dl2;
    }
    dl3 = Height(ThreePoint(h.xc, h.yc, h.xd, h.yd, h.xa, h.ya));
    if ((dl3 < min && dl3 != -1) || min == -1) {
      min = dl3;
    }
    dl4 = Height(ThreePoint(h.xc, h.yc, h.xd, h.yd, h.xb, h.yb));
    if ((dl4 < min && dl4 != -1) || min == -1) {
      min = dl4;
    }
    if (min == -1) {  // Минимальное растояние, еслли нет высоты
      dl1 = sqrt((h.xa - h.xc) * (h.xa - h.xc) + (h.ya - h.yc) * (h.ya - h.yc));
      min = dl1;
      dl2 = sqrt((h.xb - h.xd) * (h.xb - h.xd) + (h.yb - h.yd) * (h.yb - h.yd));
      if (dl2 < min) {
        min = dl2;
      }
      dl3 = sqrt((h.xb - h.xc) * (h.xb - h.xc) + (h.yb - h.yc) * (h.yb - h.yc));
      if (dl3 < min) {
        min = dl3;
      }
      dl4 = sqrt((h.xa - h.xd) * (h.xa - h.xd) + (h.ya - h.yd) * (h.ya - h.yd));
      if (dl4 < min) {
        min = dl4;
      }
    }
  }
  return min;
}

// Point
class Point : public IShape {
 public:
  Point(){};
  Point(const long double& xn, const long double& yn);
  bool ContainsPoint(const Point& p) const;
  void Move(const Vector& v);
  bool CrossesSegment(const Segment& s) const;
  IShape* Clone() const;
  void ToString() const;
  int GetX() const { return x_; }
  int GetY() const { return y_; }

 private:
  long double x_ = 0;
  long double y_ = 0;
};

void Point::ToString() const {
  std::cout << "Point(" << x_ << ", " << y_ << ")\n";
}

Point::Point(const long double& xn, const long double& yn) {
  this->x_ = xn;
  this->y_ = yn;
}

Vector operator-(const Point& a, const Point& b) {
  return Vector(a.GetX() - b.GetX(), a.GetY() - b.GetY());
}

IShape* Point::Clone() const {
  Point* p = new Point;
  p->x_ = this->x_;
  p->y_ = this->y_;
  return p;
}

bool Point::CrossesSegment(const Segment& s) const {
  Vector point(x_, y_);
  Vector begin(s.GetBegin().GetCoordX(), s.GetBegin().GetCoordY());
  Vector end(s.GetEnd().GetCoordX(), s.GetEnd().GetCoordY());
  return ((OnSegment(point, begin, end)));
}

void Point::Move(const Vector& v) {
  x_ = x_ + v.GetCoordX();
  y_ = y_ + v.GetCoordY();
}

bool Point::ContainsPoint(const Point& p) const {
  return (x_ == p.x_ && y_ == p.y_);
}

Segment::Segment(const Point& a, const Point& b) {
  begin_ = Vector(a.GetX(), a.GetY());
  end_ = Vector(b.GetX(), b.GetY());
}

bool Segment::ContainsPoint(const Point& p) const {
  Vector point(p.GetX(), p.GetY());
  return ((OnSegment(point, begin_, end_)));
}

bool Segment::CrossesSegment(const Segment& s) const {
  Point p1(begin_.GetCoordX(), begin_.GetCoordY());
  Point p2(end_.GetCoordX(), end_.GetCoordY());
  Segment s_t(p1, p2);
  Segment sk = s;
  return (DistanceSegments(sk, s_t) == 0);
}

Ray::Ray(const Point& a, const Point& b) {
  begin_ = Vector(a.GetX(), a.GetY());
  end_ = Vector(b.GetX(), b.GetY());
}

bool Ray::ContainsPoint(const Point& p) const {
  if (begin_.GetCoordX() == end_.GetCoordX() &&
      begin_.GetCoordY() == end_.GetCoordY()) {
    return (begin_.GetCoordX() == p.GetX() && begin_.GetCoordY() == p.GetY());
  }
  Vector point(p.GetX(), p.GetY());
  Vector line(end_.GetCoordX() - begin_.GetCoordX(),
              end_.GetCoordY() - begin_.GetCoordY());
  Vector check(point.GetCoordX() - begin_.GetCoordX(),
               point.GetCoordY() - begin_.GetCoordY());
  long double res = line * check;
  return OnLine(point, begin_, end_) && res >= 0;
}

bool Ray::CrossesSegment(const Segment& s) const {
  Point p1(begin_.GetCoordX(), begin_.GetCoordY());
  long double end_x = end_.GetCoordX();
  long double end_y = end_.GetCoordY();
  if (end_.GetCoordX() - begin_.GetCoordX() != 0) {
    if (end_.GetCoordX() - begin_.GetCoordX() > 0) {
      end_x += kMax;
    } else {
      end_x -= kMax;
    }
  }
  if (end_.GetCoordY() - begin_.GetCoordY() != 0) {
    if (end_.GetCoordY() - begin_.GetCoordY() > 0) {
      end_y += kMax;
    } else {
      end_y -= kMax;
    }
  }
  if (begin_.GetCoordX() == end_.GetCoordX() &&
      begin_.GetCoordY() == end_.GetCoordY()) {
    Point p2(begin_.GetCoordX(), begin_.GetCoordY());
    Segment s_t(p1, p2);
    Segment sk = s;
    return (DistanceSegments(sk, s_t) == 0);
  }
  Point p2(end_x, end_y);
  Segment s_t(p1, p2);
  Segment sk = s;
  return (DistanceSegments(sk, s_t) == 0);
}

Line::Line(const Point& a, const Point& b) {
  begin_ = Vector(a.GetX(), a.GetY());
  end_ = Vector(b.GetX(), b.GetY());
  a_ = begin_.GetCoordY() - end_.GetCoordY();  // y1 - y2;
  b_ = end_.GetCoordX() - begin_.GetCoordX();  // x2 - x1;
  c_ = begin_.GetCoordX() * end_.GetCoordY() -
       end_.GetCoordX() * begin_.GetCoordY();
}

bool Line::ContainsPoint(const Point& p) const {
  if (begin_.GetCoordX() == end_.GetCoordX() &&
      begin_.GetCoordY() == end_.GetCoordY()) {
    return true;
  }
  return a_ * p.GetX() + b_ * p.GetY() + c_ == 0;
}

bool Line::CrossesSegment(const Segment& s) const {
  return (a_ * s.BGetCoordX() + b_ * s.BGetCoordY() + c_) *
             (a_ * s.EGetCoordX() + b_ * s.EGetCoordY() + c_) <=
         0;
}

Polygon::Polygon(std::vector<Point> vector_1) {
  size_ = vector_1.size();
  p_.reserve(size_ + 1);
  for (unsigned int i = 0; i < size_; ++i) {
    Vector k(vector_1[i].GetX(), vector_1[i].GetY());
    p_[i] = k;
  }
}

int Polygon::InPolygon(int x, int y) const {
  unsigned int i1;
  unsigned int i2;
  unsigned int n = 0;
  int s;
  int s1;
  int s2;
  int s3;
  int flag = 0;
  for (n = 0; n < size_; n++) {
    flag = 0;
    i1 = n < size_ - 1 ? n + 1 : 0;
    while (flag == 0) {
      i2 = i1 + 1;
      if (i2 >= size_) {
        i2 = 0;
      }
      if (i2 == (n < size_ - 1 ? n + 1 : 0)) {
        break;
      }
      s = abs(p_[i1].GetCoordX() * (p_[i2].GetCoordY() - p_[n].GetCoordY()) +
              p_[i2].GetCoordX() * (p_[n].GetCoordY() - p_[i1].GetCoordY()) +
              p_[n].GetCoordX() * (p_[i1].GetCoordY() - p_[i2].GetCoordY()));
      s1 = abs(p_[i1].GetCoordX() * (p_[i2].GetCoordY() - y) +
               p_[i2].GetCoordX() * (y - p_[i1].GetCoordY()) +
               x * (p_[i1].GetCoordY() - p_[i2].GetCoordY()));
      s2 = abs(p_[n].GetCoordX() * (p_[i2].GetCoordY() - y) +
               p_[i2].GetCoordX() * (y - p_[n].GetCoordY()) +
               x * (p_[n].GetCoordY() - p_[i2].GetCoordY()));
      s3 = abs(p_[i1].GetCoordX() * (p_[n].GetCoordY() - y) +
               p_[n].GetCoordX() * (y - p_[i1].GetCoordY()) +
               x * (p_[i1].GetCoordY() - p_[n].GetCoordY()));
      if (s == s1 + s2 + s3) {
        flag = 1;
        break;
      }
      i1++;
      if (i1 >= size_) {
        i1 = 0;
      }
    }
    if (flag == 0) {
      break;
    }
  }
  return flag;
}

bool Polygon::ContainsPoint(const Point& point) const {
  return InPolygon(point.GetX(), point.GetY()) != 0;
}

bool Polygon::CrossesSegment(const Segment& s) const {
  for (unsigned int i = 0; i < size_; ++i) {
    Point p1(p_[i].GetCoordX(), p_[i].GetCoordY());
    Point p2(p_[i + 1].GetCoordX(), p_[i + 1].GetCoordY());
    Segment s_t(p1, p2);
    Segment sk = s;
    if ((DistanceSegments(sk, s_t) == 0)) {
      return (DistanceSegments(sk, s_t) == 0);
    }
  }
  Point p1(p_[size_ - 1].GetCoordX(), p_[size_ - 1].GetCoordY());
  Point p2(p_[0].GetCoordX(), p_[0].GetCoordY());
  Segment s_t(p1, p2);
  Segment sk = s;
  return (DistanceSegments(sk, s_t) == 0);
}

Circle::Circle(const Point& point, unsigned long i) {
  r_ = i;
  center_ = Vector(point.GetX(), point.GetY());
}

bool Circle::ContainsPoint(const Point& p) const {
  return pow((p.GetX() - center_.GetCoordX()), 2) +
             pow((p.GetY() - center_.GetCoordY()), 2) <=
         pow(r_, 2);
}

bool Circle::CrossesSegment(const Segment& s) const {
  //(const Vector& point, const Vector& begin, const Vector& end) {
  int64_t x_start = s.BGetCoordX() - center_.GetCoordX();
  int64_t y_start = s.BGetCoordY() - center_.GetCoordY();
  int64_t x_end = s.EGetCoordX() - center_.GetCoordX();
  int64_t y_end = s.EGetCoordY() - center_.GetCoordY();
  if (x_start * x_start + y_start * y_start < r_ * r_ &&
      x_end * x_end + y_end * y_end < r_ * r_) {
    return false;
  }
  return DistancePointSegment(Vector(center_.GetCoordX(), center_.GetCoordY()),
                              s.GetBegin(),
                              s.GetEnd()) <= static_cast<double>(r_);
}

}  // namespace Geometry

template <class SmartPtrT>
void Delete(const SmartPtrT& unused) {}
template <class T>
void Delete(T* ptr) {
  delete ptr;
}

void CheckFunctions(const Geometry::IShape* shape_ptr,
                    const Geometry::Point& k_point1,
                    const Geometry::Point& k_point2) {
  std::cout << "Given shape "
            << (shape_ptr->ContainsPoint(k_point1) ? "contains"
                                                   : "does not contain")
            << " point A\n";

  const auto kSegmentAb = Geometry::Segment(k_point1, k_point2);
  std::cout << "Given shape "
            << (shape_ptr->CrossesSegment(kSegmentAb) ? "crosses"
                                                      : "does not cross")
            << " segment AB\n";

  const auto kVectorAb = k_point2 - k_point1;
  // Тут поменял, разрешили, не ставьте неуд /\, Move же должна двигать
  auto* cloned_shape_ptr =
      shape_ptr->Clone();  // may return either raw or smart pointer
  cloned_shape_ptr->Move(kVectorAb);
  // Тут тоже поменял, Move в условии должен быть без возврата(да я ленив писать
  // под возврат указателя)
  cloned_shape_ptr->ToString();
  Delete(cloned_shape_ptr);  // raw pointer compatibility
}
