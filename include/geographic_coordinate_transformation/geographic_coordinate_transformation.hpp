#pragma once

#include <cmath>

class GeographicCoordinateTransformation
{
public:
  static bool bd09togcj02(
    const double& bd_lon, const double& bd_lat,
    double& gg_lng, double& gg_lat);
  static bool gcj02tobd09(
    const double& lng, const double& lat,
    double& bd_lng, double& bd_lat);
  static bool wgs84togcj02(
    const double& lng, const double& lat,
    double& mglng, double& mglat);
  static bool gcj02towgs84(
    const double& lng, const double& lat,
    double& wgs84lng, double& wgs84lat);
  static bool bd09towgs84(
    const double& lng, const double& lat,
    double& wgs84lng, double& wgs84lat);
  static bool wgs84tobd09(
    const double& lng, const double& lat,
    double& bd_lng, double& bd_lat);

private:
  static double transformlat(const double& lng, const double& lat);
  static double transformlng(const double& lng, const double& lat);
  static bool outOfChina(const double& lng, const double& lat);
  constexpr static double ee {0.00669342162296594323};
  constexpr static double a {6378245};
  constexpr static double x_PI {M_PI * 3000.0 / 180.0};
};

using GCT = GeographicCoordinateTransformation;
