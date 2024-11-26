# Deadlocks (Impasses)

Problema comum em SO, em que ocorre disputa circular de recursos.

### Exemplo: Cruzamento.

Em um cruzamento, ocorre que um veículo que espera por outro que espera por outro, e nunca destrava o entrave.

## Como ocorre deadlocks

O deadlock acontece quando ocorre disputa de recursos. Recursos como impressoras, tabelas de bds, unidades de fita. Isso resulta em um laço infinito de espera entre essa disputa de recursos.

Quando um processo tem um acesso exclusivo a um recurso (dispositivo) e outros tentam acessar circularmente (ou seja, eventualmente o mesmo processo atual vai precisar novamente do recurso), pode ocorrer deadlock.

Existem dois tipos de recursos:

- Preemptivo: podem ser retirados para que outro o utilize. Ex: processos.
- Não-preemptivo: se retirar durante o uso, o recurso pode falhar. Ex: HDs, SSDs

A sequência de eventos necessários ao uso de recurso são: *solicitar o recurso -> usar o recurso -> liberar o recurso*.

**Exemplo:**

Processo P1:

```cpp
solicita(r1);
solicita(r2);
computa();
libera(r2);
libera(r1);
```

Processo P2:

```cpp
solicita(r1);
solicita(r2);
computa();
libera(r2);
libera(r1);
```

Esses dois processos não rodam em paralelo. São forçados a rodar em sequencial. P2 espera P1 terminar. Agora imagina que essa fosse a ordem:

Processo P1:

```cpp
solicita(r1);
solicita(r2);
computa();
libera(r2);
libera(r1);
```

Processo P2:

```cpp
solicita(r2);
solicita(r1);
computa();
libera(r1);
libera(r2);
```

Aqui ocorre deadlock. P1 solicita r1, P2 solicita r2, P1 solicita r1 porém bloqueado por P2 portanto espera, P2 solicita r2 mas está bloqueado por P1 portanto espera. Deadlock.

## Outras condições de deadlock

- **Condição de exclusão mútua**: eles não podem acessar os recursos ao mesmo tempo. Um sistema sem essa condição é um sistema sem deadlock.
- **Condição de posse e espera**: se tem um processo e esse processo retém um recurso pq esse recurso precisa ser retido pra garantir a exclusão mutua, e esse recurso é capaz de solicitar novos recursos, isso pode gerar a condição de posse e espera.
- **Condição de não-preempção:** se os recursos não podem ser trocados, não tem deadlock.
- **Condição de espera circular.**

## Tratamento de deadlocks

### Ignorar o deadlock

Ele finge que o problema não existe. UNIX e Windows seguem essa abordagem. É razoável se os deadlocks são raros e o custo de prevenir é alto. Ex: algoritmo do avestruz.

### Detecção e recuperação

- Recuperação através de preempção
    - Retirar um recurso de algum outro processo. DEpende da natureza do recurso.

- Recuperação através da eliminação de processos

### Evitação dinâmica

É a alocação cuidadosa de recursos

### Prevenção

Negação de uma das quatro condições do tópico acima.

## Representação em grafos

```
(B) ---> [C]
 ^        |
 |        |
 |        V
[A] <--- (D)
```

Onde [A] e [C] são recursos que estão sendo utilizados pelos processos (D) e (B), respectivamente, e (D) e (B) são processos que estão requisitando os recursos [C] e [A], respectivamente.

Uma forma de também verificar se tem deadlock sem utilizar grafos é verificar o uso baixo de CPU em um suposto laço infinito de espera. Se a espera da resolução não termina e o uso da CPU está baixo, significa que muito provavelmente está tudo bloqueado, ou seja, os processos parados e esperando, pois nada está sendo processado.