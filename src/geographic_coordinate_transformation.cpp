#include <geographic_coordinate_transformation/geographic_coordinate_transformation.hpp>

bool GCT::bd09togcj02(
  const double& bd_lon, const double& bd_lat,
  double& gg_lng, double& gg_lat)
{
  auto x = bd_lon - 0.0065;
  auto y = bd_lat - 0.006;
  auto z = std::sqrt(x * x + y * y) - 0.00002 * std::sin(y * x_PI);
  auto theta = std::atan2(y, x) - 0.000003 * std::cos(x * x_PI);
  gg_lng = z * std::cos(theta);
  gg_lat = z * std::sin(theta);
  return true;
}

bool GCT::gcj02tobd09(
  const double& lng, const double& lat,
  double& bd_lng, double& bd_lat)
{
  auto z = std::sqrt(lng * lng + lat * lat) + 0.00002 * std::sin(lat * x_PI);
  auto theta = std::atan2(lat, lng) + 0.000003 * std::cos(lng * x_PI);
  bd_lng = z * std::cos(theta) + 0.0065;
  bd_lat = z * std::sin(theta) + 0.006;
  return true;
}

bool GCT::wgs84togcj02(
    const double& lng, const double& lat,
    double& mglng, double& mglat)
{
  if (outOfChina(lng, lat))
  {
    return false;
  }
  auto dlat = transformlat(lng - 105.0, lat - 35.0);
  auto dlng = transformlng(lng - 105.0, lat - 35.0);
  auto radlat = lat / 180.0 * M_PI;
  auto magic = std::sin(radlat);
  magic = 1 - ee * magic * magic;
  auto sqrtmagic = std::sqrt(magic);
  dlat = (dlat * 180.0) / ((a * (1 - ee)) / (magic * sqrtmagic) * M_PI);
  dlng = (dlng * 180.0) / (a / sqrtmagic * std::cos(radlat) * M_PI);
  mglat = lat + dlat;
  mglng = lng + dlng;
  return true;
}

bool GCT::gcj02towgs84(
  const double& lng, const double& lat,
  double& wgs84lng, double& wgs84lat)
{
  if (outOfChina(lng, lat))
  {
      return false;
  }
  auto dlat = transformlat(lng - 105.0, lat - 35.0);
  auto dlng = transformlng(lng - 105.0, lat - 35.0);
  auto radlat = lat / 180.0 * M_PI;
  auto magic = std::sin(radlat);
  magic = 1 - ee * magic * magic;
  auto sqrtmagic = std::sqrt(magic);
  dlat = (dlat * 180.0) / ((a * (1 - ee)) / (magic * sqrtmagic) * M_PI);
  dlng = (dlng * 180.0) / (a / sqrtmagic * std::cos(radlat) * M_PI);
  auto mglat = lat + dlat;
  auto mglng = lng + dlng;
  wgs84lng = lng * 2 - mglng;
  wgs84lat = lat * 2 - mglat;
  return true;
}

bool GCT::bd09towgs84(
  const double& lng, const double& lat,
  double& wgs84lng, double& wgs84lat)
{
  double gg_lng, gg_lat;
  bool result = false;
  if (bd09togcj02(lng, lat, gg_lng, gg_lat))
  {
    result = gcj02towgs84(gg_lng, gg_lat, wgs84lng, wgs84lat);
  }
  return result;
}

bool GCT::wgs84tobd09(
    const double& lng, const double& lat,
    double& bd_lng, double& bd_lat)
{
  double mglng, mglat;
  bool result = false;
  if (wgs84togcj02(lng, lat, mglng, mglat))
  {
    result = gcj02tobd09(mglng, mglat, bd_lng, bd_lat);
  }
  return result;
}

double GCT::transformlat(const double& lng, const double& lat)
{
    auto ret = -100.0 + 2.0 * lng + 3.0 * lat + 0.2 * lat * lat
      + 0.1 * lng * lat + 0.2 * std::sqrt(std::abs(lng));
    ret += (20.0 * std::sin(6.0 * lng * M_PI) + 20.0 * std::sin(2.0 * lng * M_PI)) * 2.0 / 3.0;
    ret += (20.0 * std::sin(lat * M_PI) + 40.0 * std::sin(lat / 3.0 * M_PI)) * 2.0 / 3.0;
    ret += (160.0 * std::sin(lat / 12.0 * M_PI) + 320 * std::sin(lat * M_PI / 30.0)) * 2.0 / 3.0;
    return ret;
}

double GCT::transformlng(const double& lng, const double& lat)
{
    auto ret = 300.0 + lng + 2.0 * lat + 0.1 * lng * lng
      + 0.1 * lng * lat + 0.1 * std::sqrt(std::abs(lng));
    ret += (20.0 * std::sin(6.0 * lng * M_PI) + 20.0 * std::sin(2.0 * lng * M_PI)) * 2.0 / 3.0;
    ret += (20.0 * std::sin(lng * M_PI) + 40.0 * std::sin(lng / 3.0 * M_PI)) * 2.0 / 3.0;
    ret += (150.0 * std::sin(lng / 12.0 * M_PI) + 300.0 * std::sin(lng / 30.0 * M_PI)) * 2.0 / 3.0;
    return ret;
}

bool GCT::outOfChina(const double& lng, const double& lat)
{
  return lng < 72.004 || lng > 137.8347 || lat < 0.8293 || lat > 55.8271;
}



