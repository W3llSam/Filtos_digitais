/* ====================================================================================================

   Programa: Demonstração da diferença entre media e media movel.
   
   Autor: William_sam.
   Data: 26/08/2024.
   Atualizado em: xx/xx/xxxx.
   Versão: 0.0.1
      
==================================================================================================== */

/* ==================================================================================================== */
/* ----- Constantes ----- */

 #define Pontos_filtro 10 // Modificando os pontos de captura pode deixar o filtro mais ou menos eficiente (se aumentar os pontos ficará mais eficiente porem o processamento será maior).
 #define Pontos_auxiliar 100 // Pontos de captura para auxiliar na estabilização. 
 #define Pontos_movel 10 // Pontos de captura da media movel aqui tem que tomar cuidado com o consumo de memoria dinamica do microcontrolador.

/* ==================================================================================================== */
/* ----- Variáveis globais ----- */

 short Escolha = 0; // Variavel global para fazer a escolha de qual filtro vai ser usado.

/* ==================================================================================================== */
/* ----- Protótipo das funções ----- */

 short Filtro(); // Função para o filtro de media.
 short Auxiliar(); // Função auxiliar o filtro de media e deixa la mais estavel.
 short Media_movel(); // Função para o filtro de media movel.

 // Lembrando que o tipo de dado retornado tem que ser trocado de acordo com o tipo de dado que está sendo trabalhado no sensor.

/* ==================================================================================================== */
/* ----- Função principal ----- */

void setup() 
{
  /* ==============================
         Escolha de filtros.

    1: Filtro media.
    2: Filtro media movel.
    3: Comparar ambos os filtros com base no sinal original.

  ==============================*/

  Escolha = 1; // Escola entre 1 e 3 para tester os filtros.

  Serial.begin(9600); // Inicia a comunicação serial.

}
/* Fim da função setup */

/* ==================================================================================================== */
/* ----- Função para repetição do codigo principal ----- */

void loop() 
{

        static unsigned long Inicial = 0, Final = 0, Resultado_Media = 0, Resultado_Media_movel = 0; // variavel local que irá armazenar os valores finais dos resultados dos filtros.

  Inicial = millis(); // Inicia a contagem de tempo com a função millis.
  
  if(Inicial - Final >= 10) // Se a subitração entre a variavel inicial e a final for maior que o tempo de 1000 ms executa o codigo abaxo do if.
  {

    if(Escolha == 1) // Se escolheu 1 vai executar o filtro de media.
    {
      Resultado_Media = Filtro(); // A variavel Resultado_media vai receber o valor retornado pela função Filtro.

      Serial.print(analogRead(A0)); // Mostra os dados na serial plotter da entrada analogica sem o filtro.
      Serial.print(" "); // Vai mostrar um caractere vazio para o serial plotter diferenciar um dado do outro.
      Serial.println(Resultado_Media + 10); // Mostra os dados recebidos pela variavel Resultado_media e para diferenciar um dado do outro se soma mais 10 para mostrar 
                                            // ambas as linhas na serial plotter.
      
    }
    
    else if(Escolha == 2) // Se escolheu 2 vai executar o filtro de media movel.
    {
      Resultado_Media_movel = Media_movel(); // Variavel para receber os dados retornados pela função media_movel.

      Serial.print(analogRead(A0)); // Mostra os dados na serial plotter da entrada analogica sem o filtro.
      Serial.print(" "); // Vai mostrar um caractere vazio para o serial plotter diferenciar um dado do outro.
      Serial.println(Resultado_Media_movel + 10); // Mostra os dados recebidos pela variavel Resultado_media_movel e para diferenciar um dado do outro se soma mais 10 para mostrar 
                                                  // ambas as linhas na serial plotter, é necessario colocar a função Serial.println() para que o serial plotter entenda
                                                  // que é o ultimo dado a ser mostrado.

    }
    
    else if(Escolha == 3) // Se escolheu 3 vai comparar ambos os filtros alem do sinal puro.
    {
      Resultado_Media = Filtro(); // A variavel Resultado_media vai receber o valor retornado pela função Filtro.
      Resultado_Media_movel = Media_movel(); // Variavel para receber os dados retornados pela função media_movel.
      
      Serial.print((analogRead(A0))); // Mostra os dados na serial plotter da entrada analogica sem o filtro.
      Serial.print(" "); // Vai mostrar um caractere vazio para o serial plotter diferenciar um dado do outro.
      Serial.print((Resultado_Media + 5)); // Mostra os dados recebidos na variavel resultado_media e soma + 5 para diferenciar a linha do filtro da linha dos dados originais.
      Serial.print(" "); // Vai mostrar um caractere vazio para o serial plotter diferenciar um dado do outro.
      Serial.println((Resultado_Media_movel + 10)); // Mostra os dados recebidos na variavel resultado_media_movel e soma + 10 para diferenciar a linha do filtro_media_movel da linha dos dados anteriores.
      
    }

    Final = Inicial; // A variavel Final recebe os dados da variavel inicial para que se possa fazer a subitração no inicio do codigo.
  }

}
/* Fim da função loop */

/* ==================================================================================================== */
/* ----- Função para o filtro de sinais ----- */

short Filtro()
{

        register short i = 0; // Variavel colocada na memoria cache do microcontrolador para que se possa fazer a contagem mais rapido possivel.
        long Dados = 0, Soma = 0, Media = 0; // Variaveis locais para armazenar temporariamente os valores recebidos da entrada analogica ou de outro sensor, caso queira
                                             // filtrar valores float ou double é só mudar de long para a variavel correspondente, só lembrando que tem de ser trocada o 
                                             // tipo de dado retornado pela função, troque de short para a variavel correspondente.

  for(i = 0; i < Pontos_filtro; i++) // Laço "for" para segurar o processamento no filtro ate que os pontos definidos sejam coletados.
  {
    Dados = Auxiliar(); // Variavel para receber os dados retornados pela função auxiliar que irá ajudar a estabilizar o filtro.
    Soma += Dados; // Soma os dados.

  }

  Media = Soma / Pontos_filtro; // após a soma dos dados é feita a divisão para obter a media de todos os dados somados.

  return Media; // retorna o valor obtido da divisão.
}

/* Fim da função filtro */

/* ==================================================================================================== */
/* ----- Função para auxiliar na estabilização dos dados ----- */

short Auxiliar()
{
        register short i = 0; // Variavel colocada na memoria cache do microcontrolador para que se possa fazer a contagem mais rapido possivel.
        long Dados = 0, Soma = 0, Media = 0; // Variaveis locais para armazenar temporariamente os valores recebidos da entrada analogica ou de outro sensor, caso queira
                                             // filtrar valores float ou double é só mudar de long para a variavel correspondente, só lembrando que tem de ser trocada o 
                                             // tipo de dado retornado pela função, troque de short para a variavel correspondente.

  for(i = 0; i < Pontos_auxiliar; i++)// Laço "for" para segurar o processamento no filtro ate que os pontos definidos sejam coletados.
  {
    Dados = analogRead(A0); //Recebe os dados do sensor e os salva temporariamente na variavel dados.
    Soma += Dados; // Soma os dados.

  }

  Media = Soma / Pontos_auxiliar; // Divide a soma dos dados com base nos pontos coletados.

  return Media; // Retorna o valor da divisão.

  // Faz o mesmo da função anterior anterior só que aqui os pontos de captura serão maiores.
  
}
/* Fim da função auxiliar */


/* ==================================================================================================== */
/* ----- Função media movel ----- */

short Media_movel()
{

         register short i = 0; // Variavel colocada na memoria cache do microcontrolador para que se possa fazer a contagem mais rapido possivel.
         long Dados = 0; // Quarda os dados somados.
         static long Quarda_dados[Pontos_movel]; // Vetor para guardar os dados recebidos.

  for(i = Pontos_movel - 1; i >= 0; i--) Quarda_dados[i] = Quarda_dados[i - 1];

  // Na linha anterior no laço for vai fazendo o deslocamentos dos pontos de captura fazendo com que o valor de i inicial receba o valor de i - 1, ou seja se caso i
  // estiver com o valor de 5 irá receber o valor que estiver no vetor 4 e assim por diante, fazendo com que o vetor 19 da veriavel seja perdido. 

  Quarda_dados[0] = analogRead(A0); // Guarda o valor do sensor no vetor 0 da variavel.

  for(i = 0; i < Pontos_movel; i++) Dados += Quarda_dados[i]; // Aqui é feita a media de todos os dados recebidos.

  return Dados / Pontos_movel; //Retorna o valor da divisão.
}

/* Fim da função Media movel*/