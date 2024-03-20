/**
*   Autor: Cesar Tadeu Pozzer
*   Modificado por: Nikolas Machado Correa
*   UFSM - 15/05/2020
*
*   pozzer@inf.ufsm.br
*   pozzer3@gmail.com
*
**/
#define DIM 20
#define N_PONTOS_CONT 10

#include "gl_canvas2d.h"
#include "Vector3.h"
#include <math.h>
#include <time.h>
#include <string>
#include <iostream>

using namespace std;

class Ponto
{
public:
    float x, y, z;

    Ponto() {}

    Ponto(float x_, float y_, float z_)
    {
        x = x_;
        y = y_;
        z = z_;
    }

    Ponto operator + (const Ponto& v)
    {
        return(Ponto(x + v.x, y + v.y, z + v.z));
    }
    Ponto operator - (const Ponto& v)
    {
        return(Ponto(x - v.x, y - v.y, z - v.z));
    }
    Ponto operator * (const float& escalar)
    {
        return(Ponto(escalar * x, escalar*y, escalar*z));
    }
    Ponto operator += (const Ponto& v)
    {
        return(Ponto(x+=v.x,y+=v.y,z+=v.z));
    }
};

//matriz quadrada de pontosControleBezier que define uma superficie. Cada ponto tem uma elevacao randomica.
class Superficie
{
    Ponto mat[DIM][DIM]; //COORDENADAS originais centradas na origem

    Ponto transf[DIM][DIM]; //coordenadas transformadas para exibicao na tela.

    float d = -40;
    float zoom = -40;
    string eixo_rotacao = "y";

    Ponto angulo;
    Ponto centroTela;
    bool mostraInstrucoes;

public:
    Ponto coordMouse;
    Ponto pontosControleBezier[N_PONTOS_CONT];
    Ponto bezierProj[N_PONTOS_CONT];
    Ponto raios[N_PONTOS_CONT];

    void set_d(double inc)
    {
        zoom += inc;
    }
    void setCentroTela(float incX, float incY)
    {
        centroTela += Ponto(incX, incY,0);
    }
    void setEixoRotacao(string eixo)
    {
        eixo_rotacao = eixo;
    }
    string getEixoRotacao()
    {
        return eixo_rotacao;
    }
    void setAng(float incX, float incY, float incZ)
    {
        angulo += Ponto(incX,incY,incZ);
    }
    Ponto getAngulo()
    {
        return angulo;
    }
    void setInstrucoes()
    {
        mostraInstrucoes = !mostraInstrucoes;
    }
    bool getInstrucoes()
    {
        return mostraInstrucoes;
    }
    void atualizaPontosBezier(int index, Ponto coord)
    {
        pontosControleBezier[index] = coord;
    }
    Ponto getCoordMouse()
    {
        return Ponto(coordMouse.x,coordMouse.y,0.0);
    }
    void verificaLimitesLayout(int index)
    {
        if(pontosControleBezier[index].x>20)
            pontosControleBezier[index].x = 20;
        if(pontosControleBezier[index].y>40)
            pontosControleBezier[index].y = 40;
        if(pontosControleBezier[index].x<0)
            pontosControleBezier[index].x = 0;
        if(pontosControleBezier[index].y<0)
            pontosControleBezier[index].y = 0;
    }

    Superficie()
    {
        srand(time(NULL));

        centroTela = Ponto(15,10,0);
        mostraInstrucoes = true;

        // Inicializacao zerando toda a matriz
        for(int i=0; i<DIM; i++)
            for(int j=0; j<DIM; j++)
                mat[i][j] = Ponto(0,0,0);

        // Curva Bezier com valores de x aleatorios
        for(int i=0; i<N_PONTOS_CONT; i++)
            pontosControleBezier[i] = Ponto(rand() % 18, i*(float)(40/DIM), 0);
    }
// rotacao de um ponto no eixo 3D
    Ponto rotaciona(Ponto p )
    {
        Ponto resp;
        float angX, angY, angZ;
        string eixo = getEixoRotacao();

        if(eixo == "x")
        {
            angX = getAngulo().x;
            resp = Ponto(p.x, cos(angX)*p.y - sin(angX)*p.z, sin(angX)*p.y + cos(angX)*p.z);
        }
        else if(eixo == "y")
        {
            angY = getAngulo().y;
            resp = Ponto(cos(angY)*p.x + sin(angY)*p.z, p.y, -sin(angY)*p.x + cos(angY)*p.z);
        }
        else if(eixo == "z")
        {
            angZ = getAngulo().z;
            resp = Ponto(cos(angZ)*p.x - sin(angZ)*p.y, sin(angZ)*p.x + cos(angZ)*p.y, p.z);
        }
        return resp;
    }

    //translacao de um unico ponto
    Ponto translada(Ponto p)
    {
        return Ponto(p.x, p.y, p.z + d*2); // d = distancia da camera do eixo x
    }
    //projecao de um ponto com zoom aplicado
    Ponto projetaZoom (Ponto p)
    {
        return Ponto((p.x*zoom) / p.z + centroTela.x, (p.y*zoom) / p.z + centroTela.y, 0);
    }
    //projecao em perspectiva, assumindo camera na origem olhando para z negativo
    Ponto projeta( Ponto p)
    {
        return Ponto((p.x*d) / p.z, (p.y*d) / p.z, 0);
    }

    void calculaPontosBezier()
    {
        int index = 0;
        double ang = 0;
        double incrParamT= 1/(double)(DIM-1);
        double incrRaio = 2.0*PI/(double)DIM;

        // define raios do objeto em funcao da curva
        for(double t=0; t<1; t+=incrParamT)
        {
            raios[index] = getBezierPoint(bezierProj, N_PONTOS_CONT, t);
            index++;
        }

        // atualiza primeira coluna da matriz (malha) com valores dos raios
        for(int x=0; x<DIM; x++)
            for(int z=0; z<DIM; z++)
            {
                mat[x][z] = Ponto(raios[x].x*cos(ang), raios[x].y, raios[x].x*sin(ang));
                ang += incrRaio;
            }
        ang=0;
    }

    // aplica sequencia de transformacoes na malha para fazer a visualizacao
    void transforma()
    {
        Ponto p;

        // Processa cada ponto da curva de bezier
        calculaPontosBezier();

        // Aplica transformacoes em cada ponto da curva
        for(int x=0; x<N_PONTOS_CONT; x++)
        {
            p = pontosControleBezier[x];
            p = translada(p);
            bezierProj[x] = projeta(p);
        }

        //Processa cada vertice do objeto individualmente.
        for(int x=0; x<DIM; x++)
            for(int z=0; z<DIM; z++)
            {
                p = mat[x][z]; //copia os valores originais
                p = rotaciona( p ); //rotacao no eixo
                p = translada( p ); //translacao no eixo z
                transf[x][z] = projetaZoom( p ); //projecao em perspectiva
            }
    }

// calcula curvatura de bezier
    Ponto getBezierPoint(Ponto * points, int numPoints, float t)
    {
        Ponto *aux = new Ponto[numPoints];
        memcpy(aux, points, numPoints * sizeof(Ponto));
        int i = numPoints - 1;
        while (i > 0)
        {
            for (int j = 0; j < i; j++)
                aux[j] = aux[j] + ((aux[j + 1] - aux[j]) * t);
            i--;
        }
        Ponto resp = aux[0];
        delete[] aux;
        return resp;
    }

    // verifica colisao do mouse com pontos de controle
    bool colidiu(int mx, int my, int index)
    {
        if ((pow(bezierProj[index].x - mx, 2) + pow(bezierProj[index].y - my, 2)) < pow(1, 2))
            return true;
        return false;
    }

    // atualiza posicao do mouse
    void controlaMouse(float x, float y)
    {
        coordMouse.x = x/15;
        coordMouse.y = y/15;
        coordMouse = translada(coordMouse);
        coordMouse = projeta(coordMouse);
    }

    void desenha()
    {
        // metade direita da tela
        CV::color(0,0,0);
        CV::rectFill(10,0,20,20);

        if(getInstrucoes())
        {
            CV::color(1,1,1);
            CV::text(11,19,"Up e Down - Zoom");
            CV::text(11,18,"A,S,D,W - Transladar");
            CV::text(11,17,"x, y, z - Rotacionar");
        }

        // vertices da malha
        CV::color(3);
        glPointSize(1);
        for(int x=0; x<DIM; x++)
            for(int z=0; z<DIM; z++)
                CV::point(transf[x][z].x, transf[x][z].y);

        //desenha as arestas
        for(int x=0; x<DIM-1; x++)
            for(int z=0; z<DIM-1; z++)
            {
                CV::line(transf[x][z].x, transf[x][z].y, transf[x+1][z].x, transf[x+1][z].y );
                CV::line(transf[x][z].x, transf[x][z].y, transf[x][z+1].x, transf[x][z+1].y );
                CV::line(transf[x][z].x, transf[x][z].y, transf[x+1][z+1].x, transf[x+1][z+1].y);
            }

        // uniao de arestas da ultima linha e coluna
        for(int z=0; z<DIM-1; z++)
        {
            CV::line(transf[DIM-1][z].x, transf[DIM-1][z].y, transf[DIM-1][z+1].x, transf[DIM-1][z+1].y );
            CV::line(transf[z][DIM-1].x, transf[z][DIM-1].y, transf[z+1][DIM-1].x, transf[z+1][DIM-1].y );
        }

        // uniao de arestas da ultima coluna com a primeira
        for(int z=0; z<DIM-1; z++)
        {
            CV::line(transf[z][DIM-1].x, transf[z][DIM-1].y, transf[z+1][0].x, transf[z+1][0].y );
            CV::line(transf[z][DIM-1].x, transf[z][DIM-1].y, transf[z][0].x, transf[z][0].y );
            CV::line(transf[DIM-1][DIM-1].x, transf[DIM-1][DIM-1].y, transf[DIM-1][0].x, transf[DIM-1][0].y );
        }

        // metade esquerda da tela
        CV::color(1,1,1);
        CV::rectFill(0,0,10,20);

        if(getInstrucoes())
        {
            CV::color(0);
            CV::text(0.5,19,"Curva de Bezier");
            CV::text(0.5,18,"Use o mouse para mover os pontos");
            CV::text(0.5,17,"Clique Q p/ abrir/fechar instrucoes");
        }

        // mouse
        CV::color(0,0,1);
        CV::circleFill(coordMouse.x, coordMouse.y, 0.3, 20);

        // pontos de controle da bezier
        CV::color(0,0,0);
        for(int z=0; z<N_PONTOS_CONT; z++)
            CV::circleFill(bezierProj[z].x, bezierProj[z].y, 0.3, 20);

        // curva de bezier
        CV::color(3);
        glPointSize(2);
        for (float i = 0; i < 1; i += 0.001)
            CV::point(getBezierPoint(bezierProj, N_PONTOS_CONT, i).x, getBezierPoint(bezierProj, N_PONTOS_CONT, i).y);
    }

    //a cada renderizacao aplica uma transformacao na superficie que faz a rotacao e projecao em perspectiva.
    void render()
    {
        glPointSize(4);
        transforma();
        desenha();
    }
};
