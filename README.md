# Resumo do projeto

O projeto implementa numa matriz de leds valores de 0 a 9 que são incrementados ou decrementados a partir dos botões A e B respectivamente, 
tudo isso utilizando de interrupções, pull-up e debounce para garantir a leitura correta dos cliques dos botões, juntamente com um led vermelho individual que pisca 5 vezes por segundo.

# Instruções de uso

Para uso do software, siga os seguintes passos:

- **1°:** clone o repositório para o seu computador.

    - Ao abrir o projeto com o **VSCode**, a extensão do **CMake** irá criar a pasta ``build`` para você com os arquivos de compilação.

    - Caso não seja gerada a pasta, crie uma pasta com nome `build` e execute o seguinte comando dentro da pasta criada:
        
        ``cmake ..``

        O comando acima irá criar os arquivos de compilação.

- **2°:** execute a compilação do firmware usando a extensão do ***Raspberry Pi Pico*** do ***VSCode***.

A partir daqui, seu firmware já está compilado e pronto para uso, e só depende de onde será usado.

## Execução no ambiente de simulação Wokwi

Para utilizar o ***firmware*** no ambiente de simulação, basta ter a extensão do **Wokwi** instalada no seu ***VSCode***, além de ter configurado a sua licença.

Suprindo os requisitos citados acima, basta clicar no arquivo ``diagram.json``, e a simulação já abrirá com o esquemático pronto.

***Clique no botão de play, no canto superior esquerdo e simule!***

## Execução na *BitDogLab*

- **1°:** coloque o seu ***Raspberry*** em modo de ***bootsel***, clicando no botão branco na placa e reiniciando a mesma.

- **2°:** copie o arquivo `.uf2` que foi gerado na pasta `build` para o seu ***Raspberry*** (ele aparecerá como um armazenamento externo, um Pen-Drive com nome de RPI-RP2).

    - Após isso, o seu código já vai está rodando na sua plaquinha ***BitDogLab***.

- **3°:** Está pronto, dê os comandos para executar as funcionalidades clicando nos botões.



## Comandos e funcionalidades disponívels:

Botão `A`: incrementa o valor da matriz de leds;

Botão `B`: decrementa o valor da matriz de leds;

## Vídeo Ensaio mostrando execução na placa ***BitDogLab***:

Clique em ***[link do video](https://youtu.be/J4212Rxb-3Y?si=uVdvNZVevcSTIYHD)*** para visualizar o vídeo ensaio do projeto.
