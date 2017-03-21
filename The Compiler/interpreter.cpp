#include "interpreter.h"

double Interpreter::origin_x, Interpreter::origin_y;
double Interpreter::scale_x, Interpreter::scale_y;
double Interpreter::rot_angle;
unsigned Interpreter::pixel_size = 1;

cv::Mat Interpreter::cv_img;
cv::Vec3b Interpreter::color = cv::Vec3b(0, 0, 0);
unsigned Interpreter::width = 1024;
unsigned Interpreter::height = 640;

void Interpreter::Run(const std::string& source_file)
{
    SetWindow();
    Parser::Parse(source_file);
    ShowWindow();
}

void Interpreter::SetWindow()
{
    cv_img = cv::Mat(cv::Size(width, height), CV_8UC3);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            cv_img.at<cv::Vec3b>(j, i) = cv::Vec3b(255, 255, 255);
        }
    }
    cv::namedWindow("pic", cv::WINDOW_AUTOSIZE);
}

void Interpreter::ShowWindow()
{
    cv::imshow("pic", cv_img);
    while (cv::waitKey(1) != 27);
}

void Interpreter::CalcCoordinate(ExprNode* x_ptr, ExprNode* y_ptr, double& x, double& y)
{
    x = x_ptr->GetValue();
    y = y_ptr->GetValue();
    x *= scale_x;                                           /* scale */
    y *= scale_y;
    double temp = x * cos(rot_angle) + y * sin(rot_angle);  /* rotation */
    y = y * cos(rot_angle) - x * sin(rot_angle);
    x = temp;
    x += origin_x;                                          /* translation */
    y += origin_y;
}

void Interpreter::DrawPixel(unsigned x, unsigned y)
{
    for (int i = 0; i < pixel_size; i++) {
        for (int j = 0; j < pixel_size; j++) {
            if (i * i + j* j <= pixel_size * pixel_size) {
                cv_img.at<cv::Vec3b>(x+i, y+j) = color;
                cv_img.at<cv::Vec3b>(x+i, y-j) = color;
                cv_img.at<cv::Vec3b>(x-i, y+j) = color;
                cv_img.at<cv::Vec3b>(x-i, y-j) = color;
            }
        }
    }
}

void Interpreter::DrawLoop(double start, double end, double step, ExprNode* x_ptr, ExprNode* y_ptr)
{
    double x, y;
    for (param_T = start; param_T < end; param_T += step) {
        CalcCoordinate(x_ptr, y_ptr, x, y);
        DrawPixel(static_cast<unsigned>(y), static_cast<unsigned>(x));
    }
}

void Interpreter::OriginStatement()
{
    ExprNode* x;
    ExprNode* y;

    MatchToken(Lexer::ORIGIN);
    MatchToken(Lexer::IS);
    MatchToken(Lexer::L_BRACKET);
    x = Expression();
    origin_x = x->GetValue();
    MatchToken(Lexer::COMMA);
    y = Expression();
    origin_y = y->GetValue();
    MatchToken(Lexer::R_BRACKET);
}

void Interpreter::ScaleStatement()
{
    ExprNode* x;
    ExprNode* y;

    MatchToken(Lexer::SCALE);
    MatchToken(Lexer::IS);
    MatchToken(Lexer::L_BRACKET);
    x = Expression();
    scale_x = x->GetValue();
    MatchToken(Lexer::COMMA);
    y = Expression();
    scale_y = y->GetValue();
    MatchToken(Lexer::R_BRACKET);
}

void Interpreter::RotStatement()
{
    ExprNode* rot;

    MatchToken(Lexer::ROT);
    MatchToken(Lexer::IS);
    rot = Expression();
    rot_angle = rot->GetValue();
}

void Interpreter::ForStatement()
{
    ExprNode* start;
    ExprNode* end;
    ExprNode* step;
    ExprNode* x;
    ExprNode* y;

    MatchToken(Lexer::FOR);
    MatchToken(Lexer::T);
    MatchToken(Lexer::FROM);
    start = Expression();
    MatchToken(Lexer::TO);
    end = Expression();
    MatchToken(Lexer::STEP);
    step = Expression();
    MatchToken(Lexer::DRAW);
    MatchToken(Lexer::L_BRACKET);
    x = Expression();
    MatchToken(Lexer::COMMA);
    y = Expression();
    MatchToken(Lexer::R_BRACKET);

    DrawLoop(start->GetValue(), end->GetValue(), step->GetValue(), x, y);
}

void Interpreter::ColorStatement()
{
    ExprNode* bgr_r;
    ExprNode* bgr_g;
    ExprNode* bgr_b;
    unsigned color_r, color_g, color_b;

    MatchToken(Lexer::COLOR);
    MatchToken(Lexer::IS);
    MatchToken(Lexer::L_BRACKET);
    bgr_r = Expression();
    color_r = static_cast<unsigned>(bgr_r->GetValue());
    MatchToken(Lexer::COMMA);
    bgr_g = Expression();
    color_g = static_cast<unsigned>(bgr_g->GetValue());
    MatchToken(Lexer::COMMA);
    bgr_b = Expression();
    color_b = static_cast<unsigned>(bgr_b->GetValue());
    MatchToken(Lexer::R_BRACKET);
    color = cv::Vec3b(color_b, color_g, color_r); /* The channels in OpenCV is BGR, not RGB */
}

void Interpreter::PixelSizeStatement()
{
    ExprNode* pixsize;

    MatchToken(Lexer::PIXSIZE);
    MatchToken(Lexer::IS);
    pixsize = Expression();
    pixel_size = pixsize->GetValue();
}

void Interpreter::WindowSizeStatement()
{
    ExprNode* x;
    ExprNode* y;

    MatchToken(Lexer::WINDOWSIZE);
    MatchToken(Lexer::IS);
    MatchToken(Lexer::L_BRACKET);
    x = Expression();
    width = x->GetValue();
    MatchToken(Lexer::COMMA);
    y = Expression();
    height = y->GetValue();
    MatchToken(Lexer::R_BRACKET);
    
    SetWindow();  /* Reset window since windowsize changed */
}
