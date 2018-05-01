// QuickPlot.h
#ifndef QuickPlot_H
#define QuickPlot_H

#include "TH1.h"

#include <string>
#include <vector>


class QuickPlot
{

private:
    // private things
    std::vector<int> m_colors;

public:
    // public things
    QuickPlot();
    void plot(std::vector<TH1*> histos, std::string title);

};

#endif
