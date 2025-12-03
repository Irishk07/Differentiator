#ifndef TEX_H_
#define TEX_H_


const char* const CONNECTING_PHRASES[] = {
                                "Этот дифференциал такой же упругий, как усы кота при виде сметаны",
                                "Как говорил кот Шрёдингер: производная одновременно и существует, и не существует",
                                "Интеграл пока написан без пределов, но этот беспредел мы скоро устраним, как кот устраняет мышей",
                                "Данная производная имеет такую же степень свободы, как кот на столе в три часа ночи",
                                "Ноль всегда сходится к нулю. Это очень глубокая мысль, достойная кота, созерцающего птиц за окном",
                                "Производная от рыбки равна котлетке - это знает каждый усатый аналитик",
                                "Как доказал кот Декарт: 'Я мурчу, значит, существую'",
                                "Эта формула такая же элегантная, как изгиб кошачьего хвоста",
                                "Данный предел стремится к банке с кормом - это аксиома кошачьей математики",
                                "Производная кошачьего терпения при приближении времени кормёжки стремительно растёт",
                                "Этот дифференциал разбежался, как кот от купания",
                                "Асимптота выходит из преисподней и идёт дальше в рай, прямо как кот, укравший сосиску",
                                "Расмотрим все 128 подмножеств нашего множество, включая пустое, как миска кота утром",
                                "Этот интеграл такой же бесконечный, как кошачья наглость",
                                "По 3 теореме Вейрштрасса: любой непрерывный кот на отрезке равномерно мурчит",
                                "Гомотопические группы данного выражения изоморфны группе мурчания",
                                "Этот дифференциал инвариантен относительно преобразований кошачьей системы координат",
                                "Квадрат - это не треугольник на стероидах, это отдельная фигура, как и кот в коробке",
                                "Формулу разности кубов я не помню, потому что в 7 классе болел, а на моих конспектах спал кот",
                                "Это выражение такое же простое, как кот, но только на первый взгляд",
                                "иуцарифдаьщфяащигшдяыпро... Данная производная очевидна, как и то, что кот снова спит на клавиатуре"
                                };

const size_t CNT_OF_PHRASES = sizeof(CONNECTING_PHRASES) / sizeof(CONNECTING_PHRASES[0]);

#ifdef OPERATORS_TO_TEX
#define PRINT_OPERATOR_TO_TEX(sign)                                                                          \
{                                                                                                            \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet, depth + 1); \
    fprintf(tex_dump_file, sign);                                                                            \
    PrintExpressionToTex(differentiator, tree_node->right_node, tex_dump_file, current_prioritet, depth + 1);\
    break;                                                                                                   \
}      

#define PRINT_ADD_OPERATOR_TO_TX                                                                            \
{                                                                                                           \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet, depth + 1);\
    if (depth > 2) {                                                                                        \
        fprintf(tex_dump_file, " + \\\\\n");                                                                \
        depth = 0;                                                                                          \
    }                                                                                                       \
    else {                                                                                                  \
        fprintf(tex_dump_file, " + ");                                                                      \
    }                                                                                                       \
    PrintExpressionToTex(differentiator, tree_node->right_node, tex_dump_file, current_prioritet, depth + 1);\
    break;                                                                                                  \
}

#define PRINT_DIV_OPERATOR_TO_TEX                                                                            \
{                                                                                                            \
    fprintf(tex_dump_file, "\\frac{");                                                                       \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet, depth + 1); \
    fprintf(tex_dump_file, "}{");                                                                            \
    PrintExpressionToTex(differentiator, tree_node->right_node, tex_dump_file, current_prioritet, depth + 1);\
    fprintf(tex_dump_file, "}");                                                                             \
    break;                                                                                                   \
}                                                                                       

#define PRINT_POW_OPERATOR_TO_TEX                                                                            \
{                                                                                                            \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet, depth + 1); \
    fprintf(tex_dump_file, "^{");                                                                            \
    PrintExpressionToTex(differentiator, tree_node->right_node, tex_dump_file, current_prioritet, depth + 1);\
    fprintf(tex_dump_file, "}");                                                                             \
    break;                                                                                                   \
}         

#define PRINT_LN_OPERATOR_TO_TEX                                                                            \
{                                                                                                           \
    fprintf(tex_dump_file, "\\ln");                                                                        \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet, depth + 1);\
    break;                                                                                                  \
}    

#define PRINTF_LOG_OPERATOR_TO_TEX                                                                          \
{                                                                                                           \
    fprintf(tex_dump_file, "\\log");                                                                       \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet, depth + 1);\
    break;                                                                                                  \
}

#define PRINTF_SIN_OPERATOR_TO_TEX                                                                          \
{                                                                                                           \
    fprintf(tex_dump_file, "\\sin");                                                                       \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet, depth + 1);\
    break;                                                                                                  \
}                                                                                                           \

#define PRINTF_COS_OPERATOR_TO_TEX                                                                          \
{                                                                                                           \
    fprintf(tex_dump_file, "\\cos");                                                                       \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet, depth + 1);\
    break;                                                                                                  \
}

#define PRINTF_TG_OPERATOR_TO_TEX                                                                           \
{                                                                                                           \
    fprintf(tex_dump_file, "\\tan");                                                                       \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet, depth + 1);\
    break;                                                                                                  \
}

#define PRINTF_CTG_OPERATOR_TO_TEX                                                                          \
{                                                                                                           \
    fprintf(tex_dump_file, "\\cot");                                                                       \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet, depth + 1);\
    break;                                                                                                  \
}

#define PRINTF_ARCSIN_OPERATOR_TO_TEX                                                                       \
{                                                                                                           \
    fprintf(tex_dump_file, "\\arcsin(");                                                                    \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet, depth + 1);\
    fprintf(tex_dump_file, ")");                                                                            \
    break;                                                                                                  \
}

#define PRINTF_ARCCOS_OPERATOR_TO_TEX                                                                       \
{                                                                                                           \
    fprintf(tex_dump_file, "\\arccos(");                                                                    \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet, depth + 1);\
    fprintf(tex_dump_file, ")");                                                                            \
    break;                                                                                                  \
}

#define PRINTF_ARCTG_OPERATOR_TO_TEX                                                                        \
{                                                                                                           \
    fprintf(tex_dump_file, "\\arctan(");                                                                    \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet, depth + 1);\
    fprintf(tex_dump_file, ")");                                                                            \
    break;                                                                                                  \
}

#define PRINTF_ARCCTG_OPERATOR_TO_TEX                                                                       \
{                                                                                                           \
    fprintf(tex_dump_file, "\\arccot(");                                                                    \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet, depth + 1);\
    fprintf(tex_dump_file, ")");                                                                            \
    break;                                                                                                  \
}

#define PRINTF_SH_OPERATOR_TO_TEX                                                                           \
{                                                                                                           \
    fprintf(tex_dump_file, "\\sinh(");                                                                      \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet, depth + 1);\
    fprintf(tex_dump_file, ")");                                                                            \
    break;                                                                                                  \
}

#define PRINTF_CH_OPERATOR_TO_TEX                                                                           \
{                                                                                                           \
    fprintf(tex_dump_file, "\\cosh(");                                                                      \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet, depth + 1);\
    fprintf(tex_dump_file, ")");                                                                            \
    break;                                                                                                  \
}

#define PRINTF_TH_OPERATOR_TO_TEX                                                                           \
{                                                                                                           \
    fprintf(tex_dump_file, "\\tanh(");                                                                      \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet, depth + 1);\
    fprintf(tex_dump_file, ")");                                                                            \
    break;                                                                                                  \
}

#define PRINTF_CTH_OPERATOR_TO_TEX                                                                          \
{                                                                                                           \
    fprintf(tex_dump_file, "\\arccot(");                                                                    \
    PrintExpressionToTex(differentiator, tree_node->left_node, tex_dump_file, current_prioritet, depth + 1);\
    fprintf(tex_dump_file, ")");                                                                            \
    break;                                                                                                  \
}
#endif // OPERATORS_TO_TEX


#ifdef DIFFERENTIANTIONS_TO_TEX

#define TEX_CONNECTING_PHRASES \
fprintf(differentiator->dump_info.tex_dump_file, "\\end{math}"); \
PrintfConnectingPhrases(differentiator); \
fprintf(differentiator->dump_info.tex_dump_file, "\\begin{math}");

#define TEX_CONST \
fprintf(differentiator->dump_info.tex_dump_file, "0\\\\\n"); 

#define TEX_VARIABLE \
fprintf(differentiator->dump_info.tex_dump_file, "1 \\\\\n");

#define TEX_ADD                                                                                                         \
fprintf(differentiator->dump_info.tex_dump_file, "\\frac{d}{d%s} ( ", variable);                                        \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") + \\frac{d}{d%s} ( ", variable);                                    \
PrintExpressionToTex(differentiator, tree_node->right_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);  \
fprintf(differentiator->dump_info.tex_dump_file, ")\\\\\n");

#define TEX_SUB                                                                                                         \
fprintf(differentiator->dump_info.tex_dump_file, "\\frac{d}{d%s} ( ", variable);                                        \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") - \\frac{d}{d%s} ( ", variable);                                    \
PrintExpressionToTex(differentiator, tree_node->right_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);  \
fprintf(differentiator->dump_info.tex_dump_file, ")\\\\\n");

#define TEX_MUL                                                                                                         \
fprintf(differentiator->dump_info.tex_dump_file, "\\frac{d}{d%s}(", variable);                                          \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\cdot (");                                                         \
PrintExpressionToTex(differentiator, tree_node->right_node,  differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0); \
fprintf(differentiator->dump_info.tex_dump_file, ") + (");                                                              \
PrintExpressionToTex(differentiator, tree_node->left_node,  differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);  \
fprintf(differentiator->dump_info.tex_dump_file, ") \\cdot \\frac{d}{d%s}(", variable);                                 \
PrintExpressionToTex(differentiator, tree_node->right_node,  differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0); \
fprintf(differentiator->dump_info.tex_dump_file, ")\\\\\n");

#define TEX_DIV                                                                                                         \
fprintf(differentiator->dump_info.tex_dump_file, "\\frac{ \\frac{d}{d%s}(", variable);                                  \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\cdot (");                                                         \
PrintExpressionToTex(differentiator, tree_node->right_node,  differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0); \
fprintf(differentiator->dump_info.tex_dump_file, ") - (");                                                              \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\cdot \\frac{d}{d%s}(", variable);                                 \
PrintExpressionToTex(differentiator, tree_node->right_node,  differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0); \
fprintf(differentiator->dump_info.tex_dump_file, ") }{ (");                                                             \
PrintExpressionToTex(differentiator, tree_node->right_node,  differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0); \
fprintf(differentiator->dump_info.tex_dump_file, ")^2 }\\\\\n");                                                        

#define TEX_POW_X_N                                                                                                                                 \
fprintf(differentiator->dump_info.tex_dump_file, "%g \\cdot (", Calculating(differentiator, tree_node->right_node));                                \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);                               \
fprintf(differentiator->dump_info.tex_dump_file, ")^{%g} \\cdot \\frac{d}{d%s}(", Calculating(differentiator, tree_node->right_node) - 1, variable);\
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);                               \
fprintf(differentiator->dump_info.tex_dump_file, ")\\\\\n");

#define TEX_POW_X_X                                                                                                     \
fprintf(differentiator->dump_info.tex_dump_file, "(");                                                                  \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ")^{");                                                                \
PrintExpressionToTex(differentiator, tree_node->right_node,  differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0); \
fprintf(differentiator->dump_info.tex_dump_file, "} \\cdot \\frac{d}{d%s}(", variable);                                 \
PrintExpressionToTex(differentiator, tree_node->right_node,  differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0); \
fprintf(differentiator->dump_info.tex_dump_file, ") \\cdot \\ln(");                                                     \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") + \\frac{");                                                        \
PrintExpressionToTex(differentiator, tree_node->right_node,  differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0); \
fprintf(differentiator->dump_info.tex_dump_file, " \\cdot \\frac{d}{d%s}(", variable);                                  \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ")}{");                                                                \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, "}\\\\\n");  

#define TEX_LN                                                                                                          \
fprintf(differentiator->dump_info.tex_dump_file, "\\frac{1}{");                                                         \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, "} \\cdot \\frac{d}{d%s}(", variable);                                 \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ")\\\\\n");

#define TEX_LOG                                                                                                         \
fprintf(differentiator->dump_info.tex_dump_file, "\\frac{1}{");                                                         \
PrintExpressionToTex(differentiator, tree_node->right_node,  differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0); \
fprintf(differentiator->dump_info.tex_dump_file, " \\cdot \\ln(");                                                      \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") } \\cdot \\frac{d}{d%s}(", variable);                               \
PrintExpressionToTex(differentiator, tree_node->right_node,  differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0); \
fprintf(differentiator->dump_info.tex_dump_file, ") \\\\\n");

#define TEX_SIN                                                                                                         \
fprintf(differentiator->dump_info.tex_dump_file, "\\cos(");                                                             \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\cdot \\frac{d}{d%s}(", variable);                                 \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\\\\n");

#define TEX_COS                                                                                                         \
fprintf(differentiator->dump_info.tex_dump_file, "-\\sin(");                                                            \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\cdot \\frac{d}{d%s}(", variable);                                 \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\\\\n");

#define TEX_TG                                                                                                          \
fprintf(differentiator->dump_info.tex_dump_file, "\\frac{1}{\\cos^2(");                                                 \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ")} \\cdot \\frac{d}{d%s}(", variable);                                \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\\\\n");

#define TEX_CTG                                                                                                         \
fprintf(differentiator->dump_info.tex_dump_file, "-\\frac{1}{\\sin^2(");                                                \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ")} \\cdot \\frac{d}{d%s}(", variable);                                \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\\\\n");

#define TEX_ARCSIN                                                                                                      \
fprintf(differentiator->dump_info.tex_dump_file, "\\frac{1}{\\sqrt{1 - (");                                             \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ")^2}} \\cdot \\frac{d}{d%s}(", variable);                             \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\\\\n");

#define TEX_ARCCOS                                                                                                      \
fprintf(differentiator->dump_info.tex_dump_file, "-\\frac{1}{\\sqrt{1 - (");                                            \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ")^2}} \\cdot \\frac{d}{d%s}(", variable);                             \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\\\n");

#define TEX_ARCTG                                                                                                       \
fprintf(differentiator->dump_info.tex_dump_file, "\\frac{1}{1 + (");                                                    \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ")^2} \\cdot \\frac{d}{d%s}(", variable);                              \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\\\\n");

#define TEX_ARCCTG                                                                                                      \
fprintf(differentiator->dump_info.tex_dump_file, "-\\frac{1}{1 + (");                                                   \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ")^2} \\cdot \\frac{d}{d%s}(", variable);                              \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\\\\n");

#define TEX_SH                                                                                                          \
fprintf(differentiator->dump_info.tex_dump_file, "\\ch(");                                                              \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\cdot \\frac{d}{d%s}(", variable);                                 \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\\\\n");

#define TEX_CH                                                                                                          \
fprintf(differentiator->dump_info.tex_dump_file, "\\sh(");                                                              \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\cdot \\frac{d}{d%s}(", variable);                                 \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\\\\n");

#define TEX_TH                                                                                                          \
fprintf(differentiator->dump_info.tex_dump_file, "\\frac{1}{\\ch^2(");                                                  \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ")} \\cdot \\frac{d}{d%s}(", variable);                                \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\\\\n");

#define TEX_CTH                                                                                                         \
fprintf(differentiator->dump_info.tex_dump_file, "-\\frac{1}{\\sh^2(");                                                 \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ")} \\cdot \\frac{d}{d%s}(", variable);                                \
PrintExpressionToTex(differentiator, tree_node->left_node, differentiator->dump_info.tex_dump_file, NO_PRIORITET, 0);   \
fprintf(differentiator->dump_info.tex_dump_file, ") \\\\\n");

#endif //DIFFERENTIANTIONS_TO_TEX


#endif // TEX_H_