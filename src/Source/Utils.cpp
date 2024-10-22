#include "Utils.h"

namespace utils
{
    std::vector<Point> read_point_set(const std::string &filename, float x_scale, float y_scale, float z_scale)
    {
        std::ifstream file(std::string(DATA_DIR) + filename);
        if (!file.is_open())
        {
            error("[read_point_set] Couldn't open this file : ", std::string(DATA_DIR) + filename);
            exit(1);
        }

        int num_points; 
        file >> num_points; 

        std::vector<Point> points;
        points.reserve(num_points);
        for (int i = 0; i < num_points; ++i)
        {
            float x, y, z; 
            file >> x >> y >> z; 

            points.emplace_back(x * x_scale, y * y_scale, z * z_scale); 
        }

        return points;
    }

} // namespace utils