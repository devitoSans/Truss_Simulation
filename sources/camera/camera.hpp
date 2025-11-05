#ifndef __CAMERA__
#define __CAMERA__

#include <splashkit.h>

#define CAMERA_UP_INPUT key_down(UP_KEY)
#define CAMERA_DOWN_INPUT key_down(DOWN_KEY)
#define CAMERA_LEFT_INPUT key_down(LEFT_KEY)
#define CAMERA_RIGHT_INPUT key_down(RIGHT_KEY)

#define CAMERA_ZOOM_IN key_down(PLUS_KEY)
#define CAMERA_ZOOM_OUT key_down(MINUS_KEY)

inline const int CAMERA_SPEED = 10;
inline const int CAMERA_ZOOM_SPEED = 1.1;

class Camera
{
    private:

    public:

        vector_2d zoom(const vector_2d& to, const vector_2d& vertex)
        {
            double multiplier = 1.0;

            if(CAMERA_ZOOM_IN)
            {
                multiplier = CAMERA_ZOOM_SPEED;
            }
            else if(CAMERA_ZOOM_OUT)
            {
                multiplier = 1.0 / CAMERA_ZOOM_SPEED;
            }
            else { return vertex; }

            vector_2d diff = vector_subtract(vertex, to);

            diff = matrix_multiply(scale_matrix(multiplier), diff);

            return vector_add(vertex, diff);
        }

        void update()
        {
            vector_2d direction = {0.0, 0.0};

            if(CAMERA_UP_INPUT)
            {
                direction = vector_add(direction, {0.0, -1.0});
            }
            if(CAMERA_DOWN_INPUT)
            {
                direction = vector_add(direction, {0.0, 1.0});
            }
            if(CAMERA_LEFT_INPUT)
            {
                direction = vector_add(direction, {-1.0, 0.0});
            }
            if(CAMERA_RIGHT_INPUT)
            {
                direction = vector_add(direction, {1.0, 0.0});
            }
            if(abs(vector_magnitude(direction) - 0.0) < 1e-6)
            {
                return;
            }
            direction = vector_from_angle(vector_angle(direction), CAMERA_SPEED);

            move_camera_by(direction);
        }
};

inline Camera camera = Camera();

#endif