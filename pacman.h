
//=========================================================
typedef struct TPacman Pacman;
typedef struct TPhantom Phantom;
typedef struct TCenario Cenario;

//=========================================================
void carregaTexturas();
void desenhaTela(int tipo);
void terminaJogo();

//=========================================================
Cenario* cenario_carrega();
void cenario_destroy(Cenario* cen);
void cenario_desenha(Cenario* cen);
void cai(Cenario* cen);
void troca(Cenario* cen, int m, int n);
void checa(Cenario* cen, Pacman* pac);
int sobe(Cenario *cen);

//=========================================================
Pacman* pacman_create(int x, int y);
void pacman_destroy(Pacman *pac);
int pacman_vivo(Pacman *pac);
void pacman_desenha(Pacman *pac);
void pacman_AlteraDirecao(Pacman *pac, int direcao, Cenario *cen);
void comanda_Cursor(Pacman *pac, int direcao, Cenario *cen);
void pacman_movimenta(Pacman *pac, Cenario *cen);
void desenha_ponto(Pacman *pac);

////=========================================================

