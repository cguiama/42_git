# py05bis — The Codex

Projeto de estudo de ambientes virtuais, dependências, configuração por variáveis
de ambiente e organização de código em módulos e pacotes Python. A metáfora é um
laboratório de alquimia: preparar o ambiente, carregar reagentes, consultar uma
configuração e combinar elementos em poções e feitiços.

As funções de alquimia retornam textos. Não há conexão real com banco de dados,
autenticação em serviços ou transformação de dados com pandas e NumPy. O foco é
entender como o ambiente e os imports funcionam.

## Requisitos e estrutura

- Python 3.10 ou superior, respeitando a compatibilidade das dependências.
- pip para instalar as bibliotecas; Poetry para testar a segunda forma de gestão.
- flake8 e mypy para verificar estilo e tipos.

```text
.
├── ex0/
│   ├── construct.py            # Diagnóstico do ambiente Python
│   ├── loading.py              # Disponibilidade e versões das bibliotecas
│   ├── requirements.txt        # Dependências para pip
│   └── pyproject.toml          # Dependências e configuração do Poetry
├── ex1/
│   ├── oracle.py               # Configuração por ambiente e .env
│   ├── requirements.txt
│   ├── .env.example            # Modelo sem credenciais reais
│   └── .gitignore              # Ignora o arquivo .env local
├── elements.py                # Fogo e água
├── alchemy/
│   ├── __init__.py             # Expõe create_air na interface do pacote
│   ├── elements.py            # Terra e ar
│   ├── potions.py             # Combina elementos em poções
│   ├── transmutation/
│   │   ├── __init__.py
│   │   └── recipes.py         # Combina imports absolutos e relativos
│   └── grimoire/
│       ├── __init__.py         # Expõe light_spell_record
│       ├── light_spellbook.py
│       ├── light_validator.py
│       ├── dark_spellbook.py
│       └── dark_validator.py
├── ft_alembic_0.py
├── ft_alembic_1.py
├── ft_distillation_0.py
├── ft_transmutation_0.py
├── ft_kaboom_0.py
└── ft_kaboom_1.py
```

Execute os comandos abaixo a partir da raiz deste projeto. Os exemplos de shell
usam Linux/macOS.

## Preparação com pip

Para observar primeiro o comportamento sem um ambiente virtual, execute em um
terminal no qual nenhum venv esteja ativo:

```sh
python3 ex0/construct.py
```

Crie um ambiente fora do repositório para não incluí-lo na entrega:

```sh
python3 -m venv /tmp/py05bis-lab
source /tmp/py05bis-lab/bin/activate
python ex0/construct.py
python ex0/loading.py
python ex1/oracle.py
```

Antes da instalação, `loading.py` deve indicar bibliotecas ausentes e sugerir
comandos de instalação. O Oracle também informa quando falta `python-dotenv`.
Ambos tratam essa situação sem traceback.

```sh
python -m pip install -r ex0/requirements.txt -r ex1/requirements.txt
python -m pip install flake8 mypy
python ex0/loading.py
```

As dependências declaradas são pandas 2.2.3, NumPy 2.2.6 e python-dotenv 1.0.1.
Após instalar, `loading.py` deve mostrar `[OK]` e a versão de pandas e NumPy.

Para sair do ambiente:

```sh
deactivate
```

## Exercício 0: ambiente e dependências

### Como `construct.py` funciona

A detecção compara `sys.prefix` com `sys.base_prefix`:

- `sys.prefix` identifica o ambiente do interpretador em execução.
- `sys.base_prefix` identifica a instalação Python que originou o ambiente.
- Valores diferentes indicam execução em um venv.

O programa mostra `sys.executable`, o estado do ambiente e os caminhos de
instalação globais, do usuário e, quando existe, do venv. Os caminhos são obtidos
com `site.getsitepackages()` e `site.getusersitepackages()`, sem assumir uma
versão ou uma pasta fixa do Python.

Esses caminhos representam locais de instalação; sua exibição não significa que
todos estejam presentes ou habilitados na busca de imports daquele processo.
Um venv comum não habilita os pacotes globais ou do usuário.

A ativação modifica o `PATH` do shell para selecionar o Python do ambiente.
Também é possível usá-lo diretamente, sem ativar:

```sh
/tmp/py05bis-lab/bin/python ex0/construct.py
```

O venv separa dependências Python. Ele não é um isolamento de segurança: o programa
continua tendo acesso aos arquivos, à rede e às permissões do usuário.

### Como `loading.py` funciona

O script tenta importar cada biblioteca com `importlib.import_module()` e lê seu
atributo `__version__`. Cada import tem seu próprio tratamento de `ImportError`,
permitindo verificar a outra biblioteca mesmo quando uma está ausente.

As mesmas versões diretas foram declaradas em `requirements.txt` e
`pyproject.toml` para manter as duas formas de instalação coerentes.

### Testar com Poetry

Com Poetry instalado e sem outro venv ativo:

```sh
poetry --version
poetry -C ex0 install
poetry -C ex0 run python loading.py
poetry -C ex0 run python construct.py
```

`-C ex0` seleciona o diretório que contém `pyproject.toml`. Por isso, os nomes dos
scripts nesses comandos são relativos a `ex0/`.

Foi escolhido `package-mode = false` porque o exercício usa Poetry para gerir
dependências, sem instalar `ex0` como uma biblioteca distribuível.

O pip resolve as dependências a partir de `requirements.txt`. As versões diretas
estão fixadas, mas as transitivas não estão todas fixadas nesse arquivo. O Poetry
resolve as dependências e gera `ex0/poetry.lock`, que registra as versões
resolvidas. Esse arquivo não acompanha esta versão do projeto: a primeira
instalação precisa gerar a resolução. Preservar o lock permite reutilizá-la em
instalações posteriores.

## Exercício 1: o Oracle

`oracle.py` carrega `ex1/.env` com `python-dotenv`. O caminho é calculado a partir
de `__file__`, então a leitura não depende do diretório de onde o script foi
chamado. A opção `override=False` preserva valores já definidos no ambiente.

| Variável | Finalidade | Quando ausente |
| --- | --- | --- |
| `MATRIX_MODE` | Selecionar development ou production | Usa development |
| `DATABASE_URL` | Configurar o banco | Informa que não foi configurado |
| `API_KEY` | Configurar acesso à API | Informa que não foi configurado |
| `LOG_LEVEL` | Selecionar verbosidade | DEBUG em development; INFO em production |
| `ZION_ENDPOINT` | Configurar o endereço remoto | Informa que não foi configurado |

Toda variável vazia ou ausente gera um aviso. Um modo desconhecido também gera
aviso e volta para development. A chave de exemplo
`replace_with_your_api_key` é tratada como não configurada.

### Testar sem configuração

Antes de criar `.env`, e sem essas variáveis exportadas no shell:

```sh
python3 ex1/oracle.py
```

São esperados avisos de configuração ausente, modo development e log DEBUG.

### Testar com `.env` e sobrescrita

```sh
cp ex1/.env.example ex1/.env
python3 ex1/oracle.py
MATRIX_MODE=production API_KEY=example_for_local_test python3 ex1/oracle.py
```

O primeiro teste usa development e os valores do arquivo. O segundo deve mostrar
production e acesso à API configurado, preservando os outros valores do `.env`.
O `LOG_LEVEL` continua DEBUG porque está explicitamente definido no arquivo;
INFO é apenas o padrão de produção quando essa variável está ausente ou vazia.

```sh
MATRIX_MODE=production LOG_LEVEL= API_KEY=example_for_local_test python3 ex1/oracle.py
MATRIX_MODE=unknown python3 ex1/oracle.py
```

O primeiro comando demonstra o padrão INFO. O segundo demonstra o tratamento de
modo inválido. Use apenas valores fictícios nesses testes.

### Decisões e limites da demonstração

- O código não imprime o conteúdo da chave nem da URL do banco.
- `.env.example` documenta a configuração; `.env` guarda valores locais e está
  listado em `ex1/.gitignore`. Nunca inclua credenciais reais na entrega.
- Variáveis de ambiente separam configuração e código, mas ainda podem aparecer
  em logs, diagnósticos ou processos com acesso suficiente.
- As mensagens `Connected`, `Authenticated` e `Online` indicam a presença de
  configuração; nenhuma conexão ou autenticação real é executada.
- A mensagem sobre segredos é informativa, sem varredura automática do código.
  A checagem de `.gitignore` procura a linha `.env`; não inspeciona o histórico
  nem confirma se um arquivo já foi rastreado pelo Git.

Para conferir a regra do Git:

```sh
git check-ignore ex1/.env
git ls-files ex1/.env
```

O primeiro comando deve mostrar `ex1/.env`; o segundo deve produzir saída vazia.

## Exercício 2: módulos e interface do pacote

Existem dois módulos chamados `elements`, em caminhos distintos:

- `elements.py` define `create_fire()` e `create_water()`.
- `alchemy/elements.py` define `create_earth()` e `create_air()`.

`ft_alembic_0.py` usa `import elements` e acessa `elements.create_fire()`.
`ft_alembic_1.py` usa `import alchemy` e acessa `alchemy.create_air()`.

O arquivo `alchemy/__init__.py` reexporta somente `create_air` com um alias
explícito e define `__all__`. Isso torna a intenção de reexportação clara também
para ferramentas de análise estática.

`create_earth` não se torna atributo de `alchemy` porque não foi reexportada.
Ela continua acessível pelo módulo `alchemy.elements`. Não existe privacidade
imposta aqui. `__all__` controla principalmente `from pacote import *`; ele não
é uma barreira de acesso.

```sh
python3 ft_alembic_0.py
python3 ft_alembic_1.py
```

O primeiro comando deve criar fogo. O segundo deve criar ar e então terminar com
`AttributeError` ao tentar `alchemy.create_earth()`. Essa falha é intencional.

## Exercício 3: poções e imports aninhados

`alchemy/potions.py` combina elementos de dentro e de fora do pacote:

- `healing_potion()` combina terra e ar.
- `strength_potion()` combina fogo e água.

O import `from elements import ...` é absoluto e alcança o módulo da raiz.
`from .elements import ...` é relativo ao pacote `alchemy`.

Em `alchemy/transmutation/recipes.py`, `lead_to_gold()` combina ar, a poção de
força e fogo. O módulo usa um import absoluto para fogo e imports com `..` para
alcançar elementos e poções no pacote pai.

```sh
python3 ft_distillation_0.py
python3 ft_transmutation_0.py
```

Ambos devem terminar sem erro e imprimir os textos compostos pelas funções.
O primeiro script demonstra `from ... import ...`; o segundo acessa a receita
com `import alchemy.transmutation.recipes`.

Imports absolutos mostram o caminho a partir da raiz de busca. Imports relativos
expressam relações internas ao pacote e evitam repetir seu nome. Nenhum arquivo
modifica `sys.path`. Execute os scripts demonstradores da raiz; executar
`recipes.py` diretamente não fornece o contexto necessário para seus imports
relativos.

## Exercício 4: dependências circulares

O spellbook precisa do validator para decidir se registra um feitiço. O validator
precisa do spellbook para obter os ingredientes permitidos.

### Por que o grimoire claro funciona

`light_spellbook.py` define as funções sem importar o validator no topo. O import
acontece dentro de `light_spell_record()`, quando a função é chamada. Nesse
momento, o módulo já terminou sua inicialização e o validator pode importar
`light_spell_allowed_ingredients()`.

O Python reutiliza módulos já carregados por meio de `sys.modules`; o import
local não executa todo o módulo novamente a cada chamada.

O validator converte a entrada para minúsculas e procura pelo menos um dos termos
`earth`, `air`, `fire` ou `water`. A busca é por substring, sem separar palavras.
Assim, uma entrada que contenha um desses termos dentro de uma palavra também
é aceita. Sem nenhum termo permitido, inclusive com texto vazio, retorna INVALID.

A decisão usa `endswith(" - VALID")` para evitar confundir VALID com INVALID,
que também contém a sequência de letras `VALID`.

Outras soluções possíveis seriam extrair os ingredientes para um terceiro módulo
compartilhado ou importar o módulo inteiro e adiar o acesso aos seus atributos.
O import local foi escolhido para demonstrar como adiar a dependência resolve
este ciclo específico.

### Por que o grimoire escuro falha

Os dois módulos escuros importam funções um do outro no topo. Ao carregar
`dark_spellbook`, o Python começa a carregar `dark_validator`, que tenta obter
uma função do spellbook antes de sua definição ter sido executada. O resultado
é um `ImportError` de módulo parcialmente inicializado.

Os ingredientes escuros são `bats`, `frogs`, `arsenic` e `eyeball`. A implementação
de registro existe, mas a falha durante o import impede chegar até ela. Esse ciclo
foi mantido deliberadamente para demonstrar o problema.

```sh
python3 ft_kaboom_0.py
python3 ft_kaboom_1.py
```

O primeiro comando deve registrar o feitiço Fantasy. O segundo deve terminar com
`ImportError` causado pela dependência circular.

Para testar aceitação e rejeição no grimoire claro:

```sh
python3 - <<'PY'
from alchemy.grimoire import light_spell_record

print(light_spell_record("Accepted", "EARTH and water"))
print(light_spell_record("Rejected", "stone"))
print(light_spell_record("Empty", ""))
PY
```

## Verificação final

Com as ferramentas instaladas no Python em uso:

```sh
python3 -m flake8 .
python3 -m mypy .
```

O flake8 deve terminar sem mensagens. O mypy deve apontar somente o acesso a
`alchemy.create_earth` em `ft_alembic_1.py`: o erro de tipo faz parte da
demonstração exigida.

| Script | Resultado esperado |
| --- | --- |
| `ex0/construct.py` | Diagnóstico distinto dentro e fora do venv |
| `ex0/loading.py` | Versões ou instruções para instalar o que falta |
| `ex1/oracle.py` | Configuração e avisos, sem revelar os valores secretos |
| `ft_alembic_0.py` | Sucesso: fogo |
| `ft_alembic_1.py` | Falha intencional: AttributeError |
| `ft_distillation_0.py` | Sucesso: duas poções |
| `ft_transmutation_0.py` | Sucesso: receita de transmutação |
| `ft_kaboom_0.py` | Sucesso: feitiço claro |
| `ft_kaboom_1.py` | Falha intencional: ImportError circular |

Os dois scripts com falhas intencionais retornam status diferente de zero.
Execute-os separadamente; encadeá-los com `&&` interrompe os comandos seguintes.

Antes de entregar, confira `git status --short` e não inclua ambientes virtuais,
arquivos `.env`, caches ou credenciais no repositório.
