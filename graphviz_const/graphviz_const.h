#ifndef GRAPHVIZ_DUMP_H
#define GRAPHVIZ_DUMP_H

#include <stdlib.h>

static const char *GRAPHVIZ_COLOR_NAMES[]   =
{
    "lightblue"     ,
    "lightgreen"    ,
    "lightgrey"     ,
    "darkorange1"   ,
    "deeppink"      ,

    "darkblue"      ,
    "darkred"       ,
    "darkgreen"     ,
    "darkorange3"   ,
    "deeppink3"     ,

    "white"         ,
    "black"         ,
};

enum GRAPHVIZ_COLOR
{
    GRAPHVIZ_COLOR_LIGHT_BLUE   ,
    GRAPHVIZ_COLOR_LIGHT_GREEN  ,
    GRAPHVIZ_COLOR_LIGHT_GREY   ,
    GRAPHVIZ_COLOR_LIGHT_ORANGE ,
    GRAPHVIZ_COLOR_LIGHT_PINK   ,

    GRAPHVIZ_COLOR_DARK_BLUE    ,
    GRAPHVIZ_COLOR_DARK_RED     ,
    GRAPHVIZ_COLOR_DARK_GREEN   ,
    GRAPHVIZ_COLOR_DARK_ORANGE  ,
    GRAPHVIZ_COLOR_DARK_PINK    ,

    GRAPHVIZ_COLOR_WHITE        ,
    GRAPHVIZ_COLOR_BLACK        ,
};

#endif //GRAPH_DUMP_H
