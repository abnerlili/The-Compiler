#ifndef _INTERPRETER_H
#define _INTERPRETER_H

#include "error.h"
#include "parser/parser.h"
#include <opencv2/core/core.hpp>    /* Use OpenCV for drawing */
#include <opencv2/highgui/highgui.hpp>

class Interpreter : public Parser {
public:
    void Run(const std::string& source_file);

private:
    static double origin_x, origin_y;
    static double scale_x, scale_y;
    static double rot_angle;
    static unsigned pixel_size;

    static cv::Mat cv_img;
    static cv::Vec3b color;
    static unsigned width, height;

    /* Helper functions */
    void SetWindow();
    void ShowWindow();
    void CalcCoordinate(ExprNode* x_ptr, ExprNode* y_ptr, double& x, double& y);
    void DrawPixel(unsigned x, unsigned y);
    void DrawLoop(double start, double end, double step, ExprNode* x_ptr, ExprNode* y_ptr);

    /* Override functions */
    void OriginStatement();
    void ScaleStatement();
    void RotStatement();
    void ForStatement();
    void ColorStatement();
    void PixelSizeStatement();
    void WindowSizeStatement();
};

#endif /* _INTERPRETER_H */