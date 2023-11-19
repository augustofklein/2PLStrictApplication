# 2PLStrictApplication
Application to develop a 2PL Strict mechanism.

## Entrada de Dados
A entrada de dados está definida no código fonte, prevendo dois cenários possíveis: <br>
<ul>    
    <li>Irá gerar delay para uma transação</li>
    <li>Irá gerar deadlock</li>
</ul>
Para alterar as operações iniciais poderá ser modificada a função processaPopulaDadosTransacoes().

## Saída de dados
A saída irá demonstrar na linha de comando a História Final obtida após o processamento, indicando os bloqueios, oprações nos dados, transações em delay e deadlock caso ocorram.
<br>

## Estrutura de Dados
Para gerenciar a história inicial, final e delay foi utilizada a estrutura de lista de alocação dinâmica criada a partir da struct tHistoria e tDelay.<br>

## Características Implementadas
Após o preenchimento da História Inicial já entrelaçada será realizado o envio de cada operação para o escalonador, que por sua vez irá gerenciar os locks concedidos para cada dado e caso haja concorrência adiciona toda a transação em delay. <br>
Após a realização do commit de alguma transação que estava bloqueando outra, serão reavaliadas as operações da transação que estava em delay para serem executadas novamente. <br>

## Critérios de abort para deadlock
Quando uma transação é adicionada em delay controlamos através de um contador a quantidade de operações que requisitam execução, sendo o limite controlado pela constante CONTAGEM_DEADLOCK.