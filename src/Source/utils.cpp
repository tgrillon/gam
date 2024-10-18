#include "utils.h"

namespace utils
{
    std::vector<Point> read_point_set(const std::string &filename, float x_scale, float y_scale)
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
            float x, y; 
            file >> x >> y; 

            points.emplace_back(x * x_scale, y * y_scale, 0.f); 
        }

        return points;
    }

} // namespace utils