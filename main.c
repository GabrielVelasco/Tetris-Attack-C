/*
Grupo GAS
Integrantes:
Gabriel Araujo Velasco 
Amauri Pietropaolo Tavares Junior 
Savio Souza Farnese 
*/

#include "SOIL.h"
#include <windows.h>
#include <gl/gl.h>
#include <stdio.h>
#include <time.h>
#include "pacman.h"
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

// Define as vari�veis e fun��es que ir�o controlar o jogo
Pacman *pac;
Cenario *cen;
int iniciou_jogo = 0;

void desenhaJogo();
void iniciaJogo();
void terminaJogo();

//fun�ao que cria e configura a janela de desenho OpenGL
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;


    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          800,
                          800,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);
    // Chama a fun��o que inicializa o jogo
    iniciaJogo();

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glPushMatrix();
            // Esse la�o controla cada frame do jogo.
            // Desenhar o jogo aqui
            desenhaJogo();

            glPopMatrix();

            SwapBuffers(hDC);

            Sleep(50);
        }
    }
    // Saiu do la�o que desenha os frames?
    // Ent�o o jogo acabou.
    terminaJogo();

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}
// Fun��o que verifica se o teclado foi pressionado
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE: //Pressionou ESC?
                    PostQuitMessage(0);
                    terminaJogo();
                    break;
                case VK_RIGHT: //Pressionou seta direita?
                    comanda_Cursor(pac,0,cen);
                    break;
                case VK_DOWN: //Pressionou seta para baixo?
                    comanda_Cursor(pac,1,cen);
                    break;
                case VK_LEFT: //Pressionou seta esquerda?
                    comanda_Cursor(pac,2,cen);
                    break;
                case VK_UP: //Pressionou seta para cima?
                    comanda_Cursor(pac,3,cen);
                    break;
                case VK_SPACE: //Pressionou espa�o? troca de pa�as
                    comanda_Cursor(pac,4,cen);
                    break;
//                case VK_CONTROL:
//                    comanda_Cursor(pac,5,cen);    //Subir linha no ctrl, apenas para teste (desativada, descomentar para ativar)
                case 'P':
                    if(iniciou_jogo == 2){
                        terminaJogo();
                        iniciaJogo();
                    }
                    iniciou_jogo = 1;
                    break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}
// Fun��o que configura o OpenGL
void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);

    // Chama a fun��o que carrega as texturas do jogo
    carregaTexturas();
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering


}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

// ===================================================================
// Fun��es que controlam e desenham o jogo
// ===================================================================

// Fun��o que desenha cada componente do jogo
int contt = 0;
void desenhaJogo(){
    if(iniciou_jogo == 2){
        desenhaTela(1); //Chama tela game over
        return;
    }
    cenario_desenha(cen);
    if(iniciou_jogo == 0){
        desenhaTela(0); //Chama tela start
        return;
    }
    checa(cen, pac);    //checa e quebra
    if(contt == 100){
        int kek = sobe(cen);
        if(kek == 0){
            iniciou_jogo = 2;   //Perdeu
        }
        comanda_Cursor(pac,3,cen);
        contt = 0;
    }
    cai(cen);
    desenha_ponto(pac);
     if(pacman_vivo(pac)){
        //pacman_movimenta(pac, cen);
        pacman_desenha(pac);
    }else{
        iniciou_jogo = 2;   //Perdeu
    }
    contt++;
}
// Fun��o que inicia o mapa do jogo e as posi��es iniciais dos personagens
void iniciaJogo(){
    srand(time(NULL));
    cen = cenario_carrega();
    pac = pacman_create(5,11);
    iniciou_jogo = 0;
}

// Fun��o que libera os dados do jogo
void terminaJogo(){
    pacman_destroy(pac);
    cenario_destroy(cen);
}
