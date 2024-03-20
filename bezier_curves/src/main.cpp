/**
*   Autor: Cesar Tadeu Pozzer
*   Modificado por: Nikolas Machado Correa
*   UFSM - 15/05/2020
*
*   pozzer@inf.ufsm.br
*   pozzer3@gmail.com
*
*   Requisitos implementados:
*   - Modelagem do objeto com opcao de rotacao e translacao, para geracao de objetos
*   - Visualizacao ortografica e perspectiva (sob varios angulos) em wireframe, com uso de triangulos
*   - Parametrizacao do numero de pontos (variaveis DIM e N_PONTOS_CONT)
*
*   Forma de interacao com o programa explicado na interface grafica
*
*
**/

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "superficie.h"
#include "gl_canvas2d.h"

Superficie *s;

////////////////////////////////////////////////////////////////////////////////////////
void render(void)
{
    CV::color(1,0,0);

    s->render();

    Sleep(10);  //reduz a velocidade de renderizacao. Nao eh a forma correta de se fazer. Deve-se utilizar funcoes de tempo.
}

void keyboard(int key)
{
    switch(key)
    {
    case 97: // A
        s->setCentroTela(-0.5,0);
        break;

    case 115: // S
        s->setCentroTela(0,-0.5);
        break;

    case 119: // W
        s->setCentroTela(0,0.5);
        break;

    case 100: // D
        s->setCentroTela(0.5,0);
        break;

    case 201: // Zoom +
        s->set_d(1);
        break;

    case 203: // Zoom -
        s->set_d(-1);
        break;

    case 120:
        s->setEixoRotacao("x");
        s->setAng(0.05,0,0);
        break;

    case 121:
        s->setEixoRotacao("y");
        s->setAng(0,0.05,0);
        break;

    case 122:
        s->setEixoRotacao("z");
        s->setAng(0,0,0.05);
        break;

    case 113:
        s->setInstrucoes();
    }
}

void keyboardUp(int key)
{
    switch(key)
    {
    case 27:
        exit(0);
        break;
    }
}

void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    s->controlaMouse((float)x, (float)y);

    static int index = 0;
    static bool arrastando = false;
    if (state == 0)
    {
        for (int i = 0; i < N_PONTOS_CONT; i++)
            if (s->colidiu(s->coordMouse.x, s->coordMouse.y, i))
            {
                index = i;
                arrastando = true;
            }
    }
    if (arrastando && state == (-2))
    {
        s->atualizaPontosBezier(index, s->getCoordMouse()*2);
        s->verificaLimitesLayout(index);
    }
    if (arrastando && state == 1)
        arrastando = false;
    return;
}

int main(void)
{
    int w = 600, h = 600;

    s = new Superficie();

    CV::init(&w, &h, "Sweep Rotacional");
    CV::run();
}

