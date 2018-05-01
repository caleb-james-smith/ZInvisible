// QuickPlot.h
#ifndef QuickPlot_H
#define QuickPlot_H

#include "TH1.h"

#include <string>
#include <vector>
#include <map>


class QuickPlot
{

private:
    // private things
    std::vector<int> m_colors;

public:
    // public things
    QuickPlot();
    void plot(std::map<std::string, TH1*> histos, std::string p_title, std::string x_title, std::string y_title);

};

#endif
