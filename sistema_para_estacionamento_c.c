/******************************************************************************

Parte IV - Final - Sistema para estacionamento

Alteracao no listarCarros para que enumere os carros estacionados;
implementacao das funcoes editarPlaca, editarHorario e calcularEMostrarTaxa;
criacao da funcao removerCarroFile, que cria um filetemp com o intuito de gerar
um arquivo temporario que verifica alteracao na lista e reescreve sem o arquivo 
removido substituindo, por fim, o arquivo anterior pelo novo, renomeando;
criacao da funcao converterMaiusculas;
complementacao das validacoes nos inputs main.

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_PLACA_LENGTH 10
#define TAXA_POR_HORA 5.0
#define TAXA_FIXA 10.0
#define NOME_ARQUIVO "registro_estacionamento.txt"

typedef struct {
    char placa[MAX_PLACA_LENGTH + 1];
    int entrada_hora;
    int entrada_minuto;
} Carro;


void converterMaiuscula(char *convertido) {
    while (*convertido){
        *convertido = toupper(*convertido);
        convertido++;
    }
}

bool verificarLista(char *placa) {
    FILE *file = fopen(NOME_ARQUIVO, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    Carro carro;
    while (fscanf(file, "%s", carro.placa) == 1) {
        if (strcmp(carro.placa, placa) == 0) {
            fclose(file);
            return true;
        }
    }
    
    fclose(file);
    return false;
}

void salvarCarro(Carro carro) {
    
    FILE *file = fopen(NOME_ARQUIVO, "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }
     if (verificarLista(carro.placa)) {
        printf("Veiculo com a placa %s ja esta na estacionado.\n", carro.placa);
        return;
    }
    
    fprintf(file, "%s %02d:%02d\n", carro.placa, carro.entrada_hora, carro.entrada_minuto);
    fclose(file);
    printf("Veiculo com placa %s registrado com sucesso!\n", carro.placa);
}

void removerCarroFile(char *placa) {
    FILE *tempFile = fopen("tempfile.txt", "w");
    if (tempFile == NULL) {
        printf("Erro ao criar arquivo temporario");
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen(NOME_ARQUIVO, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    Carro carro;
    while (fscanf(file, "%s %02d:%02d", carro.placa, &carro.entrada_hora, &carro.entrada_minuto) == 3) {
        if (strcmp(placa, carro.placa) != 0) {
            fprintf(tempFile, "%s %02d:%02d\n", carro.placa, carro.entrada_hora, carro.entrada_minuto);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(NOME_ARQUIVO);
    rename("tempfile.txt", NOME_ARQUIVO); //renomeando o arquivo original
}

void calcularEMostrarTaxa(Carro carro) {
   
     FILE *file = fopen(NOME_ARQUIVO, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo\n");
        exit(EXIT_FAILURE);
    }

    Carro carroEstacionado;

    int carroEncontrado = 0;
    while (fscanf(file, "%s %02d:%02d", carroEstacionado.placa, &carroEstacionado.entrada_hora, &carroEstacionado.entrada_minuto) == 3) {
        if (strcmp(carro.placa, carroEstacionado.placa) == 0) {
            carroEncontrado = 1;
            break;
        }
    }

    fclose(file);

    if (carroEncontrado) {
        int saida_hora, saida_minuto;
        
        do {
            printf("Digite a hora de saida (HH mm): \n");
            scanf("%d %d", &saida_hora, &saida_minuto);
        
            if((saida_hora < 0 || saida_hora > 23) || (saida_minuto < 0 || saida_minuto > 59)) {
                printf("Horario invalido\n");
            } else {
                break;
            }
        
        } while(1);

        int duracao_horas = saida_hora - carroEstacionado.entrada_hora;
        int duracao_minutos = saida_minuto - carroEstacionado.entrada_minuto;

        if (duracao_horas < 0 || (duracao_horas == 0 && duracao_minutos < 0)) {
            duracao_horas += 24;
        }

        if (duracao_minutos < 0) {
            duracao_horas--;
            duracao_minutos += 60;
        }

        double taxa = TAXA_FIXA + TAXA_POR_HORA * (duracao_horas + (duracao_minutos / 60.0));

        printf("Veiculo com placa %s ficou estacionado por %02d:%02d.\n", carroEstacionado.placa,
               duracao_horas, duracao_minutos);
        printf("Taxa a pagar: R$ %.2f\n", taxa);

        removerCarroFile(carroEstacionado.placa);

        printf("VEICULO DE PLACA %s SAINDO. VOLTE SEMPRE! \n", carro.placa);
    }
}

void listarCarros() {
    FILE *file = fopen(NOME_ARQUIVO, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo: nao ha veículos cadastrados\n");
        exit(EXIT_FAILURE);
    }

    Carro carro;
    int qtdCarros = 0;

    printf("Placa\t\t\tHora de Entrada\n");
    
    while (fscanf(file, "%s %02d:%02d", carro.placa, &carro.entrada_hora, &carro.entrada_minuto) == 3) {
        printf("%d. %s\t\t\t%02d:%02d\n", qtdCarros+1, carro.placa, carro.entrada_hora, carro.entrada_minuto);
        qtdCarros++;
    }

    fclose(file);
    
    if (qtdCarros == 0) {
        printf("Nenhum veículo cadastrado.\n");
        
    }
    
}

void editarPlaca(char *placa, char*placaNova) {
    FILE *tempFile = fopen("tempfile.txt", "w");
    if (tempFile == NULL) {
        printf("Erro ao criar arquivo temporario");
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen(NOME_ARQUIVO, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }
    
    Carro carro;
    while (fscanf(file, "%s %02d:%02d", carro.placa, &carro.entrada_hora, &carro.entrada_minuto) == 3) {
        if (strcmp(placa, carro.placa) == 0) {
            fprintf(tempFile, "%s %02d:%02d\n", placaNova, carro.entrada_hora, carro.entrada_minuto);
            printf("\nPlaca atualizada com sucesso!\n");
        } else {
           fprintf(tempFile, "%s %02d:%02d\n", carro.placa, carro.entrada_hora, carro.entrada_minuto); 
        }
    }
    
    fclose(file);
    fclose(tempFile);

    remove(NOME_ARQUIVO);
    rename("tempfile.txt", NOME_ARQUIVO);
}

void editarHorario(char *placa, int horaNova, int minutoNovo) {
    FILE *tempFile = fopen("tempfile.txt", "w");
    if (tempFile == NULL) {
        printf("Erro ao criar arquivo temporario");
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen(NOME_ARQUIVO, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }
    
    Carro carro;
    while (fscanf(file, "%s %02d:%02d", carro.placa, &carro.entrada_hora, &carro.entrada_minuto) == 3) {
        if (strcmp(placa, carro.placa) == 0) {
            fprintf(tempFile, "%s %02d:%02d\n", carro.placa, horaNova, minutoNovo);
            printf("\nHorário atualizado com sucesso!");
        } else {
           fprintf(tempFile, "%s %02d:%02d\n", carro.placa, carro.entrada_hora, carro.entrada_minuto); 
        }
    }
    
    fclose(file);
    fclose(tempFile);

    remove(NOME_ARQUIVO);
    rename("tempfile.txt", NOME_ARQUIVO);

}


int main() {
    Carro carro;
    int opcaoMenu;
    int opcaoEditar;
    char placaNova[MAX_PLACA_LENGTH + 1];
    int horaNova;
    int minutoNovo;
    

        printf("\n_________SISTEMA PARA ESTACIONAMENTO_________");
        printf("\n___________________VALORES___________________\n");
        printf("\n\t R$ %.2f", TAXA_FIXA);
        printf("\n\t R$ %.2f (adicional por hora)\n", TAXA_POR_HORA);
        printf("_____________________________________________\n");

    do {
        printf("\n======== SISTEMA PARA ESTACIONAMENTO ========\n\n");
        printf("1 - Registrar entrada de veiculo\n");
        printf("2 - Registrar saida de veiculo\n");
        printf("3 - Listar veiculos estacionados\n");
        printf("4 - Editar registro\n");
        printf("5 - Sair\n");
        printf("\nEscolha uma opcao:\n");
        scanf("%d", &opcaoMenu);

        switch (opcaoMenu) {
            case 1:
                getchar();
                printf("\n============ Entrada de Veiculo =============\n\n");
                printf("Digite a placa do veiculo (ate %d caracteres):\n", MAX_PLACA_LENGTH);
                scanf("%40s", carro.placa);
                
                converterMaiuscula(carro.placa);
                
                if (strlen(carro.placa) > MAX_PLACA_LENGTH) {
                    printf("Opcao invalida! A placa deve ter no maximo %d caracteres.\n", MAX_PLACA_LENGTH);
                    continue;
                }
                
                 do {
                        printf("Digite a hora de entrada (HH mm):\n*OBS.: Atencao ao espaco*\n");
                        scanf("%d %d", &carro.entrada_hora, &carro.entrada_minuto);
                    
                        if ((carro.entrada_hora < 0 || carro.entrada_hora > 23) || (carro.entrada_minuto < 0 || carro.entrada_minuto > 59)) {
                            printf("Horario invalido\n");
                        } else {
                            break;
                        }
                        
                    } while(1);
                    
                salvarCarro(carro);

                
                break;

            case 2:
                printf("\n============== Saida de Veiculo ==============\n\n");
                printf("Digite a placa do veiculo que esta saindo:\n");
                scanf("%s", carro.placa);
                
                converterMaiuscula(carro.placa);

                if(verificarLista(carro.placa)) {
                    calcularEMostrarTaxa(carro);
                } else {
                printf("O veiculo com placa %s nao pode ser removido pois nao esta cadastrado", carro.placa);
                }
                break;

            case 3:
                printf("\n=========== Veiculos estacionados ============\n\n");
                listarCarros();
                break;
                
            case 4:
                printf("\n=========== Alteracao de cadastro ============\n\n");
                printf("Escolha a opcao a ser alterada:\n");
                printf("Digite 1 para alterar a placa\n");
                printf("Digite 2 para alterar o horario de entrada\n");
                scanf("%d", &opcaoEditar);
                
                if (opcaoEditar != 1 && opcaoEditar != 2) {
                    printf("Opcao invalida!\n");
                    
                } else if (opcaoEditar == 1) {
                    printf("Digite a placa que sera alterada:\n");
                    scanf("%40s", carro.placa);
                    
                    converterMaiuscula(carro.placa);
                    
                    if (verificarLista(carro.placa)){
                        printf("Digite a nova placa:\n");
                        scanf("%40s", placaNova);
                        
                        converterMaiuscula(placaNova);
                        
                        editarPlaca(carro.placa, placaNova);
                    } else {
                        printf("Veiculo de placa %s nao esta cadastrado\n", carro.placa);
                        continue;
                    }
                    
                } else if (opcaoEditar == 2) {
                    printf("Digite a placa do veiculo:\n");
                    scanf("%40s", carro.placa);
                    
                    converterMaiuscula(carro.placa);
                    
                    if (verificarLista(carro.placa)){
                        do {
                            printf("Digite o novo horario de entrada (HH mm):\n");
                            scanf("%d %d", &horaNova, &minutoNovo);
                    
                                if((horaNova < 0 || horaNova > 23) || (minutoNovo < 0 || minutoNovo > 59)) {
                                    printf("Horario invalido\n");
                                    } else {
                                        break;
                                    }
                                    
                            } while(1);
                            editarHorario(carro.placa, horaNova, minutoNovo);
                    

                        
                    } else {
                        printf("Veiculo de placa %s nao esta cadastrado\n", carro.placa);
                        continue;
                    }
                }   
                break;

            case 5:
                printf("Encerrando o programa. Obrigado!\n");
                break;

            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcaoMenu != 5);

   return 0;
 }


