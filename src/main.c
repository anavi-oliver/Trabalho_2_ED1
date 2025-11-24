#include "processaGeo.h"
#include "svg.h"
#include "anteparo.h"
#include "bomba.h"
#include "gerador.h"
#include "ordenacao.h"

int main(int argc, char *argv[]) {
    // 1. Parsear argumentos (-e, -f, -o, -q, -to, -i)
    char *dirEntrada = ".";
    char *arquivoGeo = NULL;
    char *dirSaida = NULL;
    char *arquivoQry = NULL;
    TipoOrdenacao tipoOrd = TIPO_QSORT;
    int limiarInsert = 10;
    
    // Parse dos argumentos...
    
    // 2. Processar arquivo .geo
    char caminhoGeo[500];
    sprintf(caminhoGeo, "%s/%s", dirEntrada, arquivoGeo);
    Lista formas = processaArquivoGeo(caminhoGeo);
    
    // 3. Gerar SVG do .geo
    char caminhoSvgGeo[500];
    sprintf(caminhoSvgGeo, "%s/%s.svg", dirSaida, nomeBaseGeo);
    geraSVGCompleto(caminhoSvgGeo, formas, 800, 600);
    
    // 4. Criar gerador de IDs
    int maiorId = calculaMaiorId(formas);
    Gerador gerador = criaGerador(maiorId + 1);
    
    // 5. Processar arquivo .qry (se existir)
    if (arquivoQry != NULL) {
        FILE *txt = fopen(caminhoTxt, "w");
        
        // Ler linha por linha do .qry
        while (fgets(linha, MAX, arqQry)) {
            char cmd;
            sscanf(linha, " %c", &cmd);
            
            if (cmd == 'a') {
                // Comando anteparo
                int i, j;
                char orient;
                sscanf(linha, "a %d %d %c", &i, &j, &orient);
                processaComandoAnteparo(formas, i, j, orient, gerador, txt);
            }
            else if (cmd == 'd') {
                // Bomba destruição
                double x, y;
                char sufixo[100];
                sscanf(linha, "d %lf %lf %s", &x, &y, sufixo);
                processaBombaDestruicao(x, y, formas, txt, sufixo, dirSaida, nomeBase);
            }
            else if (cmd == 'p') {
                // Bomba pintura
                double x, y;
                char cor[50], sufixo[100];
                sscanf(linha, "p %lf %lf %s %s", &x, &y, cor, sufixo);
                processaBombaPintura(x, y, cor, formas, txt, sufixo, dirSaida, nomeBase);
            }
            else if (strncmp(linha, "cln", 3) == 0) {
                // Bomba clonagem
                double x, y, dx, dy;
                char sufixo[100];
                sscanf(linha, "cln %lf %lf %lf %lf %s", &x, &y, &dx, &dy, sufixo);
                processaBombaClonagem(x, y, dx, dy, formas, gerador, txt, sufixo, dirSaida, nomeBase);
            }
        }
        
        fclose(txt);
        
        // Gerar SVG final
        sprintf(caminhoSvgFinal, "%s/%s-%s.svg", dirSaida, nomeBaseGeo, nomeBaseQry);
        geraSVGCompleto(caminhoSvgFinal, formas, 800, 600);
    }
    
    // Limpar memória
    destroiListaCompleta(formas, (void (*)(void*))destroiForma);
    destroiGerador(gerador);
    
    return 0;
}