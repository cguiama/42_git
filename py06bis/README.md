# Code Nexus — Python 06 bis

Projeto de programação orientada a objetos baseado no enunciado
**Code Nexus: Abstract Architecture, from Data Streams to Battle Strategies**,
versão 1.0.

Os cinco exercícios exploram classes abstratas, sobrescrita de métodos,
polimorfismo, tipagem estrutural, fábricas abstratas, capacidades independentes
e estratégias intercambiáveis.

O projeto tem duas partes: um pipeline que recebe dados heterogêneos e os
exporta como CSV ou JSON, e uma simulação de combates em que fábricas criam
criaturas e estratégias determinam suas ações. Os combates exibem mensagens;
não há sistema de vida, dano, pontuação ou vencedor.

## Requisitos e execução

- Python 3.10 ou superior.
- Nenhuma dependência externa para executar os exercícios.
- `flake8` e `mypy` para verificar estilo e tipagem, conforme o PDF.

Os únicos módulos da biblioteca padrão importados pela implementação são
`abc` e `typing`. Os demais imports conectam os próprios arquivos do projeto.
Não são usados `eval()` ou `exec()`.

Execute os comandos na raiz do projeto:

```bash
python3 data_processor.py
python3 data_stream.py
python3 data_pipeline.py
python3 battle.py
python3 capacitor.py
python3 tournament.py
```

Cada script possui uma função `main()` protegida por
`if __name__ == "__main__":`. Assim, importar suas classes não executa as
demonstrações.

## Organização dos arquivos

```text
.
├── README.md
├── data_processor.py       # Exercício 0: processadores e fila de saída
├── data_stream.py          # Exercício 1: roteamento e estatísticas
├── data_pipeline.py        # Exercício 2: protocolo e plugins de exportação
├── battle.py               # Demonstração das fábricas
├── capacitor.py            # Demonstração da transformação
├── ex3/
│   ├── __init__.py         # API pública das fábricas
│   ├── creature.py         # Classe abstrata Creature
│   ├── capabilities.py     # Capacidade abstrata de transformação
│   ├── entities.py         # Implementações concretas das criaturas
│   └── factories.py        # Fábrica abstrata e fábricas concretas
├── tournament.py           # Demonstração do torneio e do aborto por erro
└── ex4/
    ├── __init__.py         # API pública das estratégias
    └── strategies.py       # Estratégias e exceção dedicada
```

## Exercício 0 — Processadores de dados

`DataProcessor` herda de `ABC` e define dois métodos abstratos:

```python
validate(self, data: Any) -> bool
ingest(self, data: Any) -> None
```

`validate()` verifica o conteúdo sem alterar o estado. Cada implementação de
`ingest()` possui uma assinatura específica para os tipos aceitos e valida
novamente a entrada. Isso protege o processador mesmo quando o chamador não
executa `validate()` antes.

| Processador | Entradas aceitas | Representação armazenada |
| --- | --- | --- |
| `NumericProcessor` | `int`, `float` ou lista desses tipos | Cada número convertido para `str` |
| `TextProcessor` | `str` ou lista de strings | Cada string separadamente |
| `LogProcessor` | `dict[str, str]` ou lista desses dicionários | Uma string por dicionário |

O processador numérico usa uma verificação exata de tipo: aceita os tipos
`int` e `float`, mas rejeita `bool` e subclasses numéricas. Essa escolha evita
tratar `True` e `False` como números, já que `bool` herda de `int` em Python.

Os logs com as chaves `log_level` e `log_message` são formatados como
`INFO: mensagem`. Os demais dicionários de strings são formatados como pares
`chave: valor`, separados por vírgula. Um dicionário vazio é válido e produz
uma string vazia.

Uma lista vazia é válida para os três processadores e não adiciona itens.
Uma lista com qualquer elemento inválido é rejeitada por inteiro antes de
armazenar dados. Por exemplo, `[1, "erro"]` não adiciona nem mesmo o número `1`.
Entradas inválidas causam `ValueError`.

### Fila, ranks e estatísticas

O método concreto `output()` é compartilhado pelos processadores e retorna:

```python
tuple[int, str]  # (rank, valor)
```

A fila segue FIFO: o primeiro item armazenado é o primeiro a sair. O rank
começa em zero, pertence ao processador e continua crescendo mesmo quando
itens anteriores são consumidos. Não existe um rank global entre processadores.
Uma fila vazia causa `IndexError`.

```python
from data_processor import NumericProcessor

processor = NumericProcessor()
processor.ingest([10, 2.5])
print(processor.output())  # (0, '10')
processor.ingest(30)
print(processor.output())  # (1, '2.5')
print(processor.output())  # (2, '30')
```

`total_processed` conta os itens já armazenados durante a vida do objeto.
`remaining` informa quantos ainda estão na fila. Consumir um item reduz apenas
`remaining`.

A fila usa uma lista, mantendo a implementação simples e os imports dentro
das permissões do PDF. A remoção com `pop(0)` tem custo linear; esta solução
é voltada aos exemplos didáticos do projeto.

## Exercício 1 — Fluxo polimórfico

`DataStream` registra objetos pelo contrato `DataProcessor`. Para cada elemento
da entrada, percorre os processadores na ordem de registro e chama
`validate()`. O primeiro que aceitar recebe a entrada por `ingest()`.

```text
lista heterogênea
    → validate() dos processadores registrados
    → ingest() do primeiro que aceita
    → fila de strings com ranks
```

O roteamento não precisa conhecer os tipos concretos dos processadores.
Uma nova implementação pode ser registrada sem alterar `process_stream()`.
Esse é o uso de polimorfismo neste exercício.

Decisões de funcionamento:

- Registrar novamente a mesma instância não duplica seu registro.
- Instâncias diferentes da mesma classe podem ser registradas.
- A ordem importa quando mais de um processador aceita a mesma entrada.
- Uma lista vazia é encaminhada ao primeiro processador, sem gerar itens.
- Elementos sem processador compatível geram uma mensagem de erro.
- `ValueError` e `TypeError` durante o processamento são reportados, e o fluxo
  continua no próximo elemento. Outras exceções se propagam.

`print_processors_stats()` mostra o total processado e os itens restantes de
cada processador, inclusive após o consumo por `output()`.

## Exercício 2 — Pipeline e plugins

O `DataStream` de `data_pipeline.py` herda do `DataStream` do exercício 1.
Assim, reutiliza o registro, o roteamento e as estatísticas, acrescentando:

```python
output_pipeline(self, nb: int, plugin: ExportPlugin) -> None
```

`nb` é o limite de itens **por processador**. Se um processador tiver menos
itens, exporta somente os disponíveis. Filas vazias são ignoradas. Zero não
faz nada; valores negativos, booleanos ou valores que não sejam inteiros
causam `ValueError`.

### Por que usar Protocol

`ExportPlugin` herda de `typing.Protocol` e exige este método:

```python
process_output(self, data: list[tuple[int, str]]) -> None
```

Os plugins concretos não herdam de `ExportPlugin`: o método compatível é
suficiente para satisfazer o contrato por tipagem estrutural.

Uma `ABC` exige participação explícita na hierarquia e impede instanciar
subclasses que não implementaram os métodos abstratos. Um `Protocol` permite
que classes independentes satisfaçam uma interface pela sua estrutura, com
verificação estática pelo mypy. Aqui, não há checagem de protocolo em runtime.

### Formatos e tratamento de falhas

`CSVExportPlugin` imprime os valores em uma linha CSV, sem os ranks. Campos
com vírgula, aspas ou quebras de linha recebem aspas; aspas internas são
duplicadas.

`JSONExportPlugin` imprime um objeto com chaves `item_<rank>` e valores sempre
strings. A função `json_string()` escapa aspas, barras invertidas, caracteres
de controle e surrogates. A serialização é manual porque o PDF restringe
os imports; não são usados `csv` ou `json` na implementação.

Os plugins exibem um título seguido do conteúdo no terminal. A demonstração
não grava arquivos de exportação.

Se o plugin lançar uma exceção, o lote do processador atual é recolocado no
início da fila, com os ranks originais, e a exceção é propagada. O plugin
recebe uma cópia da lista para que alterações nessa lista não destruam os
dados usados na restauração.

Essa proteção é local ao lote: exportações anteriores não são desfeitas,
e uma saída que o plugin já tenha produzido também não pode ser retirada.
A restauração acessa a fila interna `_items`, criando um acoplamento entre
o pipeline e a implementação do processador.

## Exercício 3 — Fábricas e capacidades

`Creature` contém `name`, `type`, o método abstrato `attack()` e o método
concreto `describe()`. Os métodos das criaturas retornam mensagens; os scripts
e as estratégias ficam responsáveis por imprimi-las.

`CreatureFactory` define `create_base()` e `create_evolved()`. Cada chamada
cria uma instância nova e retorna o contrato abstrato `Creature`.

| Fábrica | Criatura base | Criatura evoluída |
| --- | --- | --- |
| `FlameFactory` | `Flameling` | `Pyrodon` |
| `TransformCreatureFactory` | `Shiftling` | `Morphagon` |

A fábrica abstrata permite trocar uma família inteira sem mudar o código que
usa suas criaturas. `battle.py` demonstra isso com uma função que recebe
qualquer fábrica e outra que recebe duas fábricas para um combate.

O `ex3/__init__.py` publica apenas as fábricas por meio de imports explícitos
e de `__all__`. As classes concretas ficam em `entities.py`. Isso define a API
pública; não impede tecnicamente um import direto do módulo interno em Python.

### Transformação como capacidade independente

`TransformCapability` é uma `ABC` independente de `Creature`. Define
`transform()`, `revert()` e o atributo de estado `transformed`.

`Shiftling` e `Morphagon` herdam das duas abstrações. Seus ataques mudam conforme
o estado de transformação. Os construtores inicializam explicitamente as duas
bases, garantindo tanto os atributos da criatura quanto o estado da capacidade.

Manter a capacidade separada evita obrigar `Flameling` e `Pyrodon` a implementar
uma transformação que não possuem. Também permite reutilizar essa capacidade
em outra hierarquia.

`capacitor.py` cria as duas criaturas da família de transformação e demonstra:

```text
descrever → atacar → transformar → atacar transformado → reverter
```

A reversão fica em um bloco `finally`, para acontecer mesmo se o ataque
transformado lançar uma exceção.

## Exercício 4 — Estratégias e torneio

`BattleStrategy` define dois métodos abstratos:

- `is_valid(creature)`: informa se a criatura é adequada à estratégia.
- `act(creature)`: executa a ação e rejeita combinações incompatíveis.

| Estratégia | Compatibilidade | Ação |
| --- | --- | --- |
| `NormalStrategy` | Qualquer `Creature` | Atacar |
| `AggressiveStrategy` | Criatura com `TransformCapability` | Transformar, atacar e reverter |

A estratégia agressiva verifica a capacidade com `isinstance`, sem verificar
nomes ou famílias concretas. Uma criatura incompatível causa
`InvalidStrategyError`, com uma mensagem explicando o problema.

O bloco `finally` garante a chamada de `revert()` quando o ataque falha após
a transformação. A decisão sobre como agir fica nas estratégias, permitindo
alterar o comportamento sem reescrever o torneio.

### Organização dos confrontos

`tournament.battle()` recebe uma lista de pares:

```python
list[tuple[CreatureFactory, BattleStrategy]]
```

Cada posição enfrenta as posições seguintes uma única vez. Para `n` oponentes,
isso resulta em `n * (n - 1) / 2` confrontos. Listas com zero ou um oponente
não geram combates.

Cada confronto cria novas criaturas base pelas fábricas e executa as duas
estratégias em sequência. Ao encontrar `InvalidStrategyError`, imprime o erro
e encerra o torneio. A validação ocorre durante `act()`: a primeira criatura
pode agir antes de a combinação da segunda ser rejeitada, como no PDF.

O script demonstra um torneio válido e outro com `Flameling` associado à
estratégia agressiva, que é interrompido propositalmente.

## Como testar

### Demonstrações funcionais

Execute os seis scripts apresentados no início ou use:

```bash
for script in data_processor.py data_stream.py data_pipeline.py battle.py capacitor.py tournament.py
do
    python3 "$script" || break
done
```

Observe estes resultados:

| Script | O que verificar |
| --- | --- |
| `data_processor.py` | Validações, exceção capturada e saída em ordem FIFO |
| `data_stream.py` | Erros antes do registro completo e atualização das estatísticas |
| `data_pipeline.py` | CSV, JSON, ranks contínuos e consumo limitado por processador |
| `battle.py` | Mesmas funções trabalhando com famílias diferentes |
| `capacitor.py` | Ataque normal, transformação, ataque alterado e reversão |
| `tournament.py` | Torneio válido e interrupção por estratégia incompatível |

Ao final de `data_stream.py`, os itens restantes devem ser: numérico `5`,
texto `1` e log `1`. Ao final de `data_pipeline.py`, devem ser: numérico `3`,
texto `0` e log `0`.

### Estilo e tipagem

Com as ferramentas instaladas no ambiente:

```bash
flake8 .
mypy .
mypy --strict .
```

O `flake8` deve terminar sem mensagens. O mypy deve reportar **um erro
intencional** na chamada `numeric.ingest("foo")`, em `data_processor.py`.
O exercício 0 pede explicitamente uma chamada com tipo inválido, sem validação
prévia, que gere esse diagnóstico. Em execução, o `ValueError` correspondente
é capturado pela demonstração. Por isso, o mypy termina com código de erro
mesmo quando esse é o único diagnóstico esperado.

Durante a implementação, os seis scripts foram executados e uma verificação
temporária realizou 172 checagens de comportamento e estrutura, incluindo
rejeição de lotes inválidos, FIFO, escape de CSV/JSON, restauração de lote,
reversão após falha no ataque e contagem dos confrontos. Esse verificador
auxiliar não está incluído no repositório; os testes reproduzíveis entregues
são as demonstrações e os comandos acima.

## Como estender e explicar o projeto

Para adicionar um tipo de dado, implemente uma subclasse de `DataProcessor`
e registre-a no fluxo. Para adicionar um exportador, forneça um método
`process_output()` com a assinatura do protocolo. Para adicionar uma família,
implemente uma fábrica e suas criaturas. Para adicionar um comportamento de
combate, implemente uma nova `BattleStrategy` e verifique as capacidades
necessárias.

Na revisão e na defesa, vale conseguir explicar:

- Por que `ingest()` valida novamente e como evita uma ingestão parcial.
- A diferença entre o rank histórico e a quantidade restante na fila.
- Como o fluxo usa polimorfismo sem conhecer cada processador concreto.
- A diferença entre herança de uma `ABC` e conformidade com um `Protocol`.
- Como a fábrica esconde a escolha das classes concretas.
- Por que a capacidade de transformação é independente de `Creature`.
- Como as estratégias retiram decisões de comportamento do torneio.
- O que `finally` garante e quais são os limites da restauração de exportação.

O PDF recomenda revisar, testar e compreender qualquer conteúdo produzido
com auxílio de IA, além de buscar revisão com colegas. Este README serve como
guia de estudo e consulta do código.
