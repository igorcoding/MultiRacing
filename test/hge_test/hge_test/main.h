#ifndef MAIN_H
#define MAIN_H

struct Section
{
    Section(float xlb, float ylb, float xrt, float yrt)
        : x_lb(xlb),
          y_lb(ylb),
          x_rt(xrt),
          y_rt(yrt)
    { }

    float width() const
    { return x_rt - x_lb; }

    float height() const
    { return y_rt - y_lb; }

    float x_lb;
    float y_lb;
    float x_rt;
    float y_rt;


};

#endif // MAIN_H
