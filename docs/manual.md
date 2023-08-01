## Led indicador de status:

`AMARELO `Indica que está no modo de configuração, habilita o servidor web e uma rede wifi com ip 102.168.4.1.

`VERMELHO  `Indica que não conseguiu se conectar a rede wifi configurada.

`ROSA `Indica que está conectado a rede wifi mas não consegue se conectar ao servidor na nuvem. Erro por falta de internet, ou mal configuração do servidor mqtt.

`AZUL `Indica que não está conseguindo se comunicar com a rede RS485. Verificar configuração da rede no outro dispositivo, ou conexão.

`VERDE `Indica que está conectado e operando normalmente.

## Configuração dos parâmetros:

Para entrar no modo de configuração, manter pressionado o botão "BOT" na placa até o led indicador ficar na cor `AMARELA`;
Após, com o smartphone, procurar a rede wifi com nome 485Gateway-XXXXX;
Conectar a está rede, a senha da rede é o código do dispositivo, encontrado no nome da rede após o prefixo 485Gateway-; EX: `485Gateway-BC02F3D1`, a senha será `BC02F3D1`;

Alterar somente os campos:
`wifi.ssid` nome da rede a qual deseja se conectar.
`wifi.pass` senha da rede a qual deseja se conectar. (deixar em branco caso nao possua).

Após os ajustes das configurações a placa será reiniciada automaticamente, retornando para o modo de operação.
