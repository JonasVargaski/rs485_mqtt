### Todo:

- Bolar estrutura de array para armazenar o endereço do escravo e seus HREG E COILS

```
 1 : {
  coils: [1,2,3],
  hreg: [2,3,4,5]
 },
 2 : {
  coils: [1,2,3],
  hreg: [2,3,4,5]
 },
```

- Iterar nessa estrutura e ler os registros do modbus

- montar JSON e enviar via mqtt a mesma estrutura via função de callback "onComplete"

- Remove condição while do modbus (travando timers);

- Pesquisar lib de timer ou thread que funciona por interrupção de timers
