# Born2beRoot — guia de configuração e defesa

Este documento registra o que foi configurado na máquina virtual, por que cada
configuração existe, onde ela fica e como comprová-la durante uma avaliação.
Ele descreve a nossa VM Debian; não é uma lista genérica de comandos.

> Estado atual: a parte obrigatória está configurada em grande parte, mas ainda
> faremos a auditoria final e a simulação de defesa. O bônus foi iniciado:
> Lighttpd, MariaDB e PHP estão instalados, mas o WordPress ainda não foi
> concluído.

## 1. Visão geral da máquina

- Sistema: Debian 13 (Trixie), sem interface gráfica.
- Hipervisor: Oracle VirtualBox.
- Hostname: `guilamar42`.
- Usuário principal: `guilamar`.
- Grupos exigidos: `sudo` e `user42`.
- Disco: aproximadamente 30 GB, com LUKS, LVM e partições separadas.
- SSH: porta 4242, autenticação de `root` proibida.
- Firewall: UFW.
- Controle de acesso obrigatório: AppArmor.
- Monitoramento: script Bash executado pelo `cron` e exibido com `wall`.

Comandos de visão geral:

```bash
hostnamectl --static
uname -a
id guilamar
lsblk
```

## 2. Máquina virtual e rede

### Por que usamos uma VM

O VirtualBox simula hardware: processador, memória, disco e placa de rede. O
Debian convidado fica isolado do sistema hospedeiro, permitindo configurar um
servidor sem alterar diretamente o computador físico.

### NAT e redirecionamento de porta

A placa da VM usa NAT. Nesse modo, a VM consegue sair para a Internet, mas uma
conexão iniciada pelo hospedeiro precisa de uma regra de redirecionamento:

- IP do hospedeiro: `127.0.0.1`;
- porta do hospedeiro: `4242`;
- porta do convidado: `4242`.

Assim, no terminal do computador hospedeiro usamos:

```bash
ssh -p 4242 guilamar@127.0.0.1
```

`127.0.0.1` é o loopback do computador onde o comando é executado. Essa regra
serve para acesso local; ela não torna o servidor acessível automaticamente por
outros computadores. Na 42, pode ser necessário recriar a VM no VirtualBox ou
importá-la e refazer apenas as configurações externas do VirtualBox. As
configurações dentro do Debian permanecem no VDI.

Para o site do bônus, pode-se criar outra regra NAT, por exemplo porta `8080` do
hospedeiro para porta `80` do convidado. Isso permite abrir
`http://127.0.0.1:8080` no hospedeiro sem alterar a porta usada internamente pelo
Lighttpd.

Essa regra foi criada e validada no hospedeiro com:

```bash
curl -I http://127.0.0.1:8080
```

O teste retornou `HTTP/1.1 200 OK` e `Server: lighttpd/1.4.79`, comprovando que o
redirecionamento NAT e o servidor HTTP estão funcionando. A opção `-I` solicita
somente os cabeçalhos da resposta, sem baixar ou mostrar o corpo da página.

## 3. Disco, criptografia e LVM

### Construção de baixo para cima

```text
disco virtual /dev/sda
├── /dev/sda1              sistema de arquivos ext4 montado em /boot
└── /dev/sda5              contêiner criptografado LUKS
    └── /dev/sda5_crypt    dispositivo aberto pelo device mapper
        └── LVMGroup       grupo de volumes LVM
            ├── root       /
            ├── swap       [SWAP]
            ├── home       /home
            ├── var        /var
            ├── srv        /srv
            ├── tmp        /tmp
            └── var-log    /var/log
```

O `/boot` fica fora do contêiner criptografado porque o firmware e o carregador
de inicialização precisam ler o kernel antes que o sistema peça a senha do LUKS.
Depois da senha, o LUKS expõe `/dev/sda5_crypt`; dentro dele, o LVM organiza o
espaço em volumes lógicos.

### O que o LVM acrescenta

- PV (Physical Volume): armazenamento entregue ao LVM, aqui o dispositivo LUKS.
- VG (Volume Group): conjunto de espaço, chamado `LVMGroup`.
- LV (Logical Volume): unidades criadas dentro do VG, semelhantes a partições,
  mas mais flexíveis para redimensionamento.

Separar diretórios reduz o impacto de esgotamento de espaço. Por exemplo, logs
que lotem `/var/log` não ocupam diretamente todo o volume raiz.

### Função de cada volume

- `/`: arquivos essenciais do sistema e programas.
- `swap`: espaço em disco usado para páginas de memória menos ativas. É muito
  mais lento que RAM; ajuda sob pressão de memória e pode ser usado para
  hibernação, mas não substitui RAM.
- `/home`: arquivos e configurações pessoais dos usuários.
- `/var`: dados variáveis, como cache, filas, bancos e estado de serviços.
- `/var/log`: registros do sistema e dos serviços.
- `/srv`: dados servidos por serviços; é uma separação exigida pelo esquema do
  bônus, embora atualmente quase vazio.
- `/tmp`: arquivos temporários; isolá-lo impede que temporários encham `/`.

Verificação:

```bash
lsblk -f
sudo pvs
sudo vgs
sudo lvs
df -h
free -h
```

## 4. Usuário, grupos e hostname

O hostname foi definido como `guilamar42`, seguindo o formato `login42`:

```bash
hostnamectl --static
```

O usuário `guilamar` pertence a `user42` e `sudo`:

```bash
id guilamar
getent group user42
getent group sudo
```

O grupo `user42` atende ao requisito do projeto. O grupo `sudo` permite solicitar
elevação por comando, mantendo a sessão normal sem privilégios permanentes.
Criar e verificar usuários está documentado também em `COMMANDS.md`.

### Teste de criação de usuário e grupo

Como a avaliação pode solicitar essa operação, criamos `evaluser`, criamos
`evalgroup` e associamos ambos com `adduser`. `id` e `getent` confirmaram o grupo,
e `chage -l` mostrou que a conta nova herdou mínimo 2, máximo 30 e aviso 7 de
`/etc/login.defs`.

Depois do teste, removemos a conta temporária com seu diretório pessoal e o grupo
de teste. A listagem final de UIDs administrativos/humanos voltou a mostrar
somente `root` (UID 0) e `guilamar` (UID 1000). Isso evita deixar uma conta de
avaliação desnecessária no servidor.

### Teste de alteração do hostname

O subject informa que o hostname será alterado durante a avaliação. Praticamos a
mudança de `guilamar42` para `evaluation42`:

```bash
sudo hostnamectl set-hostname evaluation42
sudo nano /etc/hosts
```

Mantivemos consistentes o estado do systemd, `/etc/hostname` e a entrada
`127.0.1.1` de `/etc/hosts`. Os três passaram a mostrar `evaluation42`. O prompt
antigo permaneceu até `exec bash`, pois o shell havia calculado seu prompt antes
da alteração.

Depois do teste, repetimos o procedimento para restaurar `guilamar42`. A
verificação final confirmou o nome em `hostnamectl`, `/etc/hostname` e
`/etc/hosts`, e `exec bash` restaurou o prompt `guilamar@guilamar42`. A VM está,
portanto, novamente no estado exigido para entrega.

## 5. SSH

### Objetivo

O SSH fornece um terminal remoto criptografado. O servidor escuta na porta 4242
e não aceita login direto como `root`. Isso reduz a exposição da porta padrão e,
principalmente, exige que ações administrativas sejam associadas a um usuário
normal por meio do `sudo`.

Arquivo criado:

```text
/etc/ssh/sshd_config.d/born2beroot.conf
```

Conteúdo relevante:

```text
Port 4242
PermitRootLogin no
PasswordAuthentication yes
```

Após editar, validamos a sintaxe antes de reiniciar:

```bash
sudo sshd -t
sudo systemctl restart ssh
```

Verificação da configuração efetiva e da porta:

```bash
sudo sshd -T | grep -E '^(port|permitrootlogin|passwordauthentication)'
sudo ss -ltnp
systemctl is-active ssh
systemctl is-enabled ssh
```

`ss -ltnp` significa sockets, TCP, listening, numérico e processos. As linhas
IPv4 e IPv6 na porta 4242 são dois sockets do mesmo serviço, não duas conexões.
Para contar conexões realmente estabelecidas usamos o estado `established`, não
`LISTEN`.

### Teste de login remoto como root

Testamos no hospedeiro com autenticação por senha, sem chave e com somente uma
tentativa:

```bash
ssh -o PreferredAuthentications=password -o PubkeyAuthentication=no \
    -o NumberOfPasswordPrompts=1 -p 4242 root@127.0.0.1
```

Mesmo com a senha correta, o servidor respondeu `Permission denied`, enquanto
`ssh -p 4242 guilamar@127.0.0.1` entrou normalmente. Isso comprova na prática a
diretiva `PermitRootLogin no`. O prompt ainda solicita uma senha antes de negar
porque o SSH evita revelar antecipadamente detalhes que ajudariam a enumerar
contas e políticas do servidor.

`PermitRootLogin no` bloqueia especificamente autenticação da conta `root` pelo
servidor SSH. Não desativa a conta no sistema. Há três caminhos distintos:

- login como `root` no console virtual da própria VM, se a política PAM permitir;
- `su -` a partir de uma sessão existente, autenticando com a senha de `root`;
- `sudo comando`, autenticando como usuário autorizado e elevando apenas o
  comando solicitado.

No uso normal preferimos entrar como `guilamar` e usar `sudo`. Isso mantém
rastreabilidade no log, reduz o tempo com privilégio total e evita expor por rede
uma conta cujo nome é conhecido em todo sistema Linux.

## 6. UFW

O UFW é uma interface de administração do firewall do Linux. Configuramos negar
entrada por padrão, permitir saída e abrir somente portas necessárias:

```bash
sudo ufw default deny incoming
sudo ufw default allow outgoing
sudo ufw allow 4242/tcp
sudo ufw enable
```

Com o início do bônus, também abrimos HTTP:

```bash
sudo ufw allow 80/tcp
```

Estado atual esperado: portas 4242 e 80, para IPv4 e IPv6.

```bash
sudo ufw status numbered
systemctl is-active ufw
systemctl is-enabled ufw
```

Uma regra do UFW e uma regra NAT do VirtualBox têm funções distintas: o UFW
decide se o Debian aceita o tráfego; o NAT decide como o tráfego do hospedeiro
chega à VM.

## 7. AppArmor

O AppArmor aplica políticas por programa. Mesmo que um processo seja explorado,
seu perfil pode limitar quais arquivos e recursos ele acessa. Ele complementa
permissões Unix, firewall e `sudo`; não substitui nenhum deles.

Verificação:

```bash
systemctl is-active apparmor
systemctl is-enabled apparmor
sudo aa-status | head
```

`active` comprova o serviço, enquanto `aa-status` mostra o módulo, os perfis
carregados e quantos estão em modo de imposição (`enforce`).

## 8. Política de senhas

Existem duas partes diferentes: validade temporal e qualidade da nova senha.

### Validade temporal

Arquivo alterado:

```text
/etc/login.defs
```

Valores:

```text
PASS_MAX_DAYS 30
PASS_MIN_DAYS 2
PASS_WARN_AGE 7
```

- `PASS_MAX_DAYS`: senha válida por no máximo 30 dias.
- `PASS_MIN_DAYS`: aguarda 2 dias antes de permitir outra troca comum.
- `PASS_WARN_AGE`: avisa 7 dias antes de expirar.

Esses padrões afetam principalmente contas criadas depois da alteração. Para as
contas existentes aplicamos explicitamente:

```bash
sudo chage -M 30 -m 2 -W 7 guilamar
sudo chage -M 30 -m 2 -W 7 root
```

Verificação:

```bash
sudo chage -l guilamar
sudo chage -l root
```

Na auditoria final, ambas as contas mostraram última alteração em 31 de agosto de
2026, expiração em 30 de setembro de 2026, mínimo de 2 dias, máximo de 30 dias e
aviso de 7 dias. As datas e os três parâmetros confirmam que a política temporal
foi aplicada às contas existentes, e não apenas definida como padrão para contas
futuras.

### Qualidade da senha

Pacote usado: `libpam-pwquality`.

Arquivo alterado:

```text
/etc/security/pwquality.conf
```

Configuração:

```text
difok = 7
minlen = 10
dcredit = -1
ucredit = -1
lcredit = -1
maxrepeat = 3
usercheck = 1
enforce_for_root
```

- `difok=7`: ao menos sete caracteres da nova senha devem diferir da antiga.
- `minlen=10`: comprimento mínimo de dez caracteres.
- créditos negativos significam requisitos mínimos; `dcredit=-1`,
  `ucredit=-1` e `lcredit=-1` exigem ao menos um dígito, uma maiúscula e uma
  minúscula.
- `maxrepeat=3`: limita repetições consecutivas do mesmo caractere.
- `usercheck=1`: rejeita senha que contenha o nome do usuário.
- `enforce_for_root`: a política também é obrigatória ao alterar a senha de
  `root`.

Integração PAM verificada em:

```text
/etc/pam.d/common-password
```

Comando de inspeção:

```bash
grep pam_pwquality /etc/pam.d/common-password
```

O PAM é a camada pela qual programas como `passwd` aplicam módulos de
autenticação e política.

## 9. Sudo

### Por que usar sudo

O `sudo` eleva somente um comando após autenticação. Isso reduz o tempo com
privilégios, registra quem pediu a operação e permite políticas específicas.

Arquivo criado:

```text
/etc/sudoers.d/born2beroot
```

Diretivas configuradas:

- `passwd_tries=3`: três tentativas de senha.
- `badpass_message=...`: mensagem personalizada para senha incorreta.
- `logfile=/var/log/sudo/sudo.log`: log textual das execuções.
- `log_input,log_output`: grava entrada e saída da sessão de I/O.
- `iolog_dir=/var/log/sudo`: diretório dos registros de I/O.
- `requiretty`: exige execução a partir de um terminal.
- `secure_path=...`: define os diretórios confiáveis onde o `sudo` procura
  executáveis.

O `secure_path` evita que a elevação use o `PATH` arbitrário do usuário. Um
diretório gravável como `/tmp` não deve aparecer nele: alguém poderia criar ali
um programa falso chamado, por exemplo, `reboot`, e tentar fazê-lo ser encontrado
antes do binário legítimo. O risco principal é execução de código como `root`,
que pode também resultar em roubo de dados e senhas.

Validação obrigatória após editar:

```bash
sudo visudo -cf /etc/sudoers
sudo -l
```

Nunca se valida um arquivo `sudoers` apenas visualmente; um erro de sintaxe pode
impedir o uso posterior do `sudo`.

### Logs e replay

O arquivo textual mostra usuário, terminal, diretório, usuário-alvo, TSID e
comando:

```bash
sudo cat /var/log/sudo/sudo.log
```

Cada `TSID`, como `000004`, corresponde a uma árvore de I/O no diretório
configurado. Como escolhemos um diretório diferente do padrão, o replay precisa
recebê-lo:

```bash
sudo sudoreplay -d /var/log/sudo 000004
```

O arquivo `seq` guarda o próximo identificador. As pastas `00/00/04` são uma
distribuição hierárquica do ID para evitar milhares de entradas em um único
diretório.

## 10. Script de monitoramento

Local instalado na VM:

```text
/usr/local/bin/monitoring.sh
```

Escolhemos `/usr/local/bin` porque ele é destinado a programas instalados pelo
administrador local, separados dos arquivos gerenciados pelos pacotes Debian em
`/usr/bin`.

Propriedade e permissões esperadas:

```bash
sudo chown root:root /usr/local/bin/monitoring.sh
sudo chmod 755 /usr/local/bin/monitoring.sh
ls -l /usr/local/bin/monitoring.sh
```

O proprietário `root` pode ler, escrever e executar; grupo e outros podem ler e
executar, mas não alterar. Isso impede que um usuário comum modifique um script
executado automaticamente como `root`.

Uma cópia de estudo está em `monitoring.sh` neste repositório.

### Como cada linha é obtida

- Arquitetura: `uname -a`.
- CPUs físicas: IDs físicos únicos em `/proc/cpuinfo`.
- vCPUs: quantidade de entradas `processor` em `/proc/cpuinfo`.
- Memória: segunda linha de `free --mega`.
- Disco: soma dos sistemas de arquivos cujo dispositivo começa com `/dev/`.
- Carga de CPU: `100 - idle` da segunda amostra de `top`.
- Último boot: `uptime -s`.
- LVM: procura uma linha exatamente `lvm` na coluna `TYPE` do `lsblk`.
- TCP: conta sockets em estado `established`.
- Usuários: conta sessões retornadas por `who`.
- Rede: obtém interface da rota padrão e então seu IPv4 e MAC.
- Sudo: conta registros contendo `COMMAND=` no log textual.
- Exibição: o bloco inteiro é enviado para `wall`.

O `export LC_ALL=C` padroniza a linguagem e o separador decimal da saída dos
comandos. Sem isso, o `awk` poderia procurar campos que mudam conforme o idioma.

Validação:

```bash
bash -n /usr/local/bin/monitoring.sh
sudo /usr/local/bin/monitoring.sh
```

`bash -n` verifica sintaxe sem executar. Nenhuma saída e código `0` significam
que a sintaxe foi aceita:

```bash
echo $?
```

### Ferramentas de texto usadas

`grep` seleciona linhas que correspondem a um padrão. Em `^processor`, `^`
significa início da linha; isso evita casar a palavra em outra posição. A opção
`-c` conta linhas e `-q` apenas informa sucesso ou falha.

No `lsblk -nr -o TYPE`:

- `-n`: omite cabeçalho;
- `-r`: saída simples, adequada para scripts;
- `-o TYPE`: exibe somente a coluna de tipo;
- `grep -q '^lvm$'`: procura uma linha que seja exatamente `lvm`; `$` marca o
  fim da linha.

O `awk` divide cada linha em campos (`$1`, `$2` etc.), permite selecionar linhas,
somar valores e formatar resultados com `printf`.

## 11. Cron e mensagens de broadcast

O `cron` executa comandos automaticamente conforme um calendário. Usamos a
crontab de `root` porque o script lê o log protegido do `sudo` e chama `wall`:

```bash
sudo crontab -l
```

Conteúdo:

```cron
@reboot /usr/local/bin/monitoring.sh
*/10 * * * * /usr/local/bin/monitoring.sh
```

- `@reboot`: uma execução após o boot.
- `*/10` no campo de minutos: a cada múltiplo de dez no relógio.
- os outros quatro `*`: qualquer hora, dia do mês, mês e dia da semana.

O script não contém um laço infinito. É o `cron` que inicia uma nova execução a
cada dez minutos; o `wall` envia o resultado aos terminais conectados.

```bash
systemctl is-enabled cron
systemctl is-active cron
sudo journalctl -u cron --since "30 minutes ago" --no-pager
```

Parar o serviço interrompe todos os trabalhos do cron, não apenas o monitor:

```bash
sudo systemctl stop cron
sudo systemctl start cron
```

### Correção necessária no Debian 13

O `wall` inicialmente retornava sucesso, mas não mostrava nada porque não havia
`/run/utmp`, arquivo que relaciona sessões aos terminais. Criamos:

```text
/etc/tmpfiles.d/utmp.conf
```

Com este conteúdo:

```text
f /run/utmp 0664 root utmp -
```

O `systemd-tmpfiles` passa a criar `/run/utmp` no boot, com proprietário `root`,
grupo `utmp` e modo `0664`. Após reiniciar, `who` e `wall` reconheceram as
sessões. No Debian 13, essa compatibilidade pode fazer a mensagem aparecer duas
vezes devido à transição entre o registro de sessões do systemd-logind e o utmp;
não são duas execuções do script necessariamente.

Verificação:

```bash
ls -l /etc/tmpfiles.d/utmp.conf /run/utmp
cat /etc/tmpfiles.d/utmp.conf
who
echo "TESTE WALL" | sudo wall
```

## 12. Bônus: estado atual

Foram instalados Lighttpd, MariaDB e PHP, e ativados os módulos FastCGI:

```bash
sudo lighty-enable-mod fastcgi
sudo lighty-enable-mod fastcgi-php
sudo systemctl restart lighttpd
sudo systemctl enable mariadb lighttpd
sudo ufw allow 80/tcp
```

Verificações já aprovadas:

```bash
systemctl is-active lighttpd mariadb
systemctl is-enabled lighttpd mariadb
php --version
```

Além do PHP no terminal, a integração FastCGI foi validada criando
temporariamente `/var/www/html/test.php` e acessando-o pelo redirecionamento NAT:

```bash
curl http://127.0.0.1:8080/test.php
```

A resposta `PHP OK - versão 8.4.24` comprovou que o Lighttpd encaminhou o arquivo
ao interpretador PHP. Se o navegador tivesse recebido o texto `<?php ... ?>`, o
código-fonte estaria sendo exposto e o FastCGI não estaria funcionando. O `%`
mostrado depois da resposta pelo Zsh apenas indicou ausência de uma quebra de
linha final; não veio do servidor.

O teste temporário foi removido depois da validação. Isso reduz exposição
desnecessária da versão do PHP e evita deixar artefatos de diagnóstico públicos.

O fluxo configurado é: navegador solicita `.php`; Lighttpd reconhece a extensão;
FastCGI encaminha o arquivo ao interpretador PHP; PHP executa o código; Lighttpd
devolve somente o resultado. Sem essa associação, o código poderia ser servido
como texto. No WordPress isso seria grave porque `wp-config.php` contém as
credenciais do banco. `php --version` testa apenas o executável no terminal; o
arquivo temporário testou toda a cadeia HTTP.

Depois do WordPress e do serviço adicional, ainda precisamos:

1. confirmar a instalação pública e o painel administrativo;
2. revisar permissões do `wp-config.php` e dos arquivos do site;
3. fazer nova auditoria de serviços e portas.

Testes imediatos:

```bash
sudo lighttpd -tt -f /etc/lighttpd/lighttpd.conf
curl -I http://127.0.0.1
sudo ss -ltnp
```

### Banco de dados do WordPress

Entramos no MariaDB como administrador local com `sudo mariadb` e criamos uma
base e uma conta exclusivas para a aplicação:

```sql
CREATE DATABASE wordpress
CHARACTER SET utf8mb4
COLLATE utf8mb4_unicode_ci;

CREATE USER 'wpuser'@'localhost'
IDENTIFIED BY 'senha-secreta-definida-na-VM';

GRANT ALL PRIVILEGES ON wordpress.* TO 'wpuser'@'localhost';
FLUSH PRIVILEGES;
```

Não registramos a senha real neste repositório. `utf8mb4` suporta todo o conjunto
Unicode. `wpuser@localhost` restringe a conta a conexões originadas na própria
VM. `wordpress.*` concede privilégios somente às tabelas desse banco; usar `*.*`
daria acesso desnecessário a todos os bancos.

Verificamos a concessão e a autenticação com:

```sql
SHOW DATABASES;
SHOW GRANTS FOR 'wpuser'@'localhost';
```

```bash
mariadb -u wpuser -p wordpress
```

Inicialmente o marcador `SUA_SENHA_FORTE` foi usado literalmente. Antes de
configurar o WordPress, ele foi substituído por uma senha real com `ALTER USER`.
Isso foi simples porque nenhuma aplicação ainda dependia da credencial antiga.

### Instalação manual dos arquivos

O `curl` não existia inicialmente. A primeira tentativa de download falhou e o
`tar` falhou em seguida porque o arquivo nunca foi criado. Instalamos os meios de
download HTTPS:

```bash
sudo apt install curl ca-certificates
```

Então baixamos e extraímos o pacote oficial:

```bash
cd /tmp
curl -fLo wordpress.tar.gz https://wordpress.org/latest.tar.gz
tar -xzf wordpress.tar.gz
```

Em `curl`, `-f` trata erros HTTP como falha, `-L` segue redirecionamentos e `-o`
define o arquivo de saída. Em `tar -xzf`, `x` extrai, `z` descompacta gzip e `f`
indica que o próximo argumento é o arquivo.

Movemos a página padrão do Lighttpd de forma reversível e instalamos o conteúdo:

```bash
sudo mv /var/www/html/index.lighttpd.html /var/www/html/index.lighttpd.html.disabled
sudo cp -a /tmp/wordpress/. /var/www/html/
sudo chown -R www-data:www-data /var/www/html
```

`/var/www/html` é a raiz documental publicada pelo Lighttpd. `cp -a` preserva a
estrutura e os metadados; o `.` copia o conteúdo da pasta, não a pasta externa.
`www-data` é a identidade usada pelo servidor web e recebeu propriedade dos
arquivos para que o instalador conseguisse criar sua configuração.

### Contas e telas do WordPress

As três identidades não são intercambiáveis:

- `guilamar`: conta do sistema Debian, usada com SSH e `sudo`;
- `wpuser`: conta interna do MariaDB, limitada ao banco `wordpress`;
- conta administrativa do WordPress: usada somente no painel do site.

Cada uma deve ter senha diferente para evitar que o comprometimento de uma
camada conceda acesso às demais.

Durante o instalador web informamos banco `wordpress`, usuário `wpuser`, servidor
`localhost` e prefixo `wp_`. O WordPress criou `/var/www/html/wp-config.php`, que
contém a conexão com o banco, e tabelas como `wp_users`, `wp_posts` e
`wp_options`.

A existência do arquivo retornou status `0`, e `SHOW TABLES` mostrou as doze
tabelas iniciais do WordPress. Encontramos `wp-config.php` com modo `666`, que
permitia leitura e escrita a qualquer usuário local. Como o arquivo contém
credenciais, alteramos a propriedade e o modo:

```bash
sudo chown root:www-data /var/www/html/wp-config.php
sudo chmod 640 /var/www/html/wp-config.php
```

Com `640`, `root` pode ler e escrever, `www-data` pode somente ler para executar
o site, e os demais usuários não têm acesso.

Depois da correção, `curl -I http://127.0.0.1/` dentro da VM retornou `301 Moved
Permanently`, com `X-Redirect-By: WordPress` e destino
`http://127.0.0.1:8080/`. Isso não foi erro: o WordPress redirecionou para a URL
canônica que recebeu durante a instalação, incluindo a porta NAT vista pelo
navegador do hospedeiro. A resposta comprova que PHP e banco continuaram
funcionando.

A proteção foi confirmada como `root:www-data 640`. No hospedeiro,
`curl -I http://127.0.0.1:8080/` retornou `HTTP/1.1 200 OK`, cabeçalho da API do
WordPress e `Server: lighttpd/1.4.79`. Portanto, site público, NAT, servidor web,
PHP, banco e permissões foram validados em conjunto.

Uma tentativa no hospedeiro usando `http://127.0.0.1/` falhou porque, sem porta
explícita, o cliente tentou a porta 80 do próprio hospedeiro. A regra NAT escuta
na porta 8080 do hospedeiro e a encaminha à porta 80 do convidado; esses dois
endereços não são equivalentes.

As URLs têm funções diferentes:

- `/`: parte pública do site;
- `/wp-login.php`: formulário e processamento da autenticação;
- `/wp-admin/`: área administrativa protegida.

Quando alguém abre `/wp-admin/` sem sessão autenticada, o WordPress redireciona
para `/wp-login.php`. Após um login válido, retorna à área administrativa.

### Serviço adicional: Fail2ban

O subject não exige Fail2ban nominalmente; ele pede um serviço adicional útil,
sem usar Nginx ou Apache. Escolhemos Fail2ban porque o servidor expõe SSH. Ele
observa falhas repetidas de autenticação e aplica bloqueios temporários, reduzindo
a eficácia de tentativas automatizadas de senha.

UFW e Fail2ban têm funções complementares. O UFW decide quais portas podem ser
acessadas; a porta 4242 precisa permanecer aberta para o SSH. O Fail2ban reage ao
comportamento observado dentro dessa porta permitida e bloqueia origens que
excedem o limite.

Instalação verificada:

```bash
sudo apt install fail2ban
fail2ban-client --version
systemctl is-active fail2ban
systemctl is-enabled fail2ban
```

Foi encontrada a versão 1.1.0, com serviço ativo e habilitado no boot.

Criamos o arquivo:

```text
/etc/fail2ban/jail.d/sshd.local
```

Conteúdo:

```ini
[sshd]
enabled = true
port = 4242
backend = systemd
maxretry = 5
findtime = 10m
bantime = 10m
```

- `[sshd]`: jail que protege o SSH;
- `port=4242`: porta real configurada no `sshd`;
- `backend=systemd`: lê eventos pelo journal, em vez de depender de
  `/var/log/auth.log`;
- `maxretry=5`: limite de falhas;
- `findtime=10m`: janela na qual as cinco falhas são contadas;
- `bantime=10m`: duração do bloqueio.

Uma jail reúne filtro, limites e ação. O estado confirmou que o filtro usa
`_SYSTEMD_UNIT=ssh.service` e `_COMM=sshd`, isto é, eventos do serviço e processo
corretos.

Validação:

```bash
sudo fail2ban-client -t
sudo fail2ban-client status
sudo fail2ban-client status sshd
```

O teste retornou configuração válida e exatamente uma jail, `sshd`. Logo após
um `systemctl restart`, uma consulta encontrou temporariamente o socket ausente;
a consulta seguinte funcionou. Foi apenas uma corrida de inicialização: o
systemd já considerava o processo ativo antes de o socket de controle estar
pronto.

Para testar a ação sem bloquear o acesso NAT, usamos `192.0.2.1`, endereço de uma
faixa reservada para documentação:

```bash
sudo fail2ban-client set sshd banip 192.0.2.1
sudo fail2ban-client status sshd
sudo fail2ban-client set sshd unbanip 192.0.2.1
sudo fail2ban-client status sshd
```

O estado mudou para um IP banido e voltou a zero após o desbloqueio. `Total
banned` permaneceu em 1 porque é um contador histórico desde que a jail iniciou;
`Currently banned` e `Banned IP list` representam o estado presente.

Se um endereço legítimo for bloqueado, use pelo console direto da VM:

```bash
sudo fail2ban-client set sshd unbanip ENDERECO_IP
```

Não provocamos cinco senhas erradas pelo SSH porque o tráfego NAT do hospedeiro
aparece para a VM por um endereço compartilhado, normalmente `10.0.2.2`. Bani-lo
poderia interromper o acesso SSH usado na própria configuração.

### Por que não adicionamos Chrony

Chrony seria um serviço adicional útil para sincronização NTP: horário correto é
importante para logs, cron, validade de senhas, certificados e análise de
incidentes. Antes de instalá-lo, verificamos o mecanismo que o Debian já usa:

```bash
timedatectl
systemctl is-active systemd-timesyncd
```

O resultado mostrou `System clock synchronized: yes`, `NTP service: active` e
`systemd-timesyncd` ativo. A zona é `America/Sao_Paulo`, o horário local está em
UTC-3 e o RTC permanece em UTC (`RTC in local TZ: no`), configuração recomendada
para evitar ambiguidades de fuso e horário de verão.

Por isso não instalamos Chrony: ele duplicaria uma função que já está saudável e
introduziria outro serviço para configurar, auditar e explicar. Mantivemos
Fail2ban como o serviço adicional oficial do bônus.

## 13. Erros encontrados e o que ensinaram

- `sudo: comando não encontrado`: o pacote ainda não estava instalado; ser membro
  de um grupo não cria o executável.
- `su: usuário não existe`: `su` espera um nome de usuário; `su -` abre uma shell
  de login como root.
- `passwd: Erro de manipulação de token`: ao simular uma senha recém-alterada com
  `chage`, `PASS_MIN_DAYS=2` bloqueou nova troca imediata. Após ajustar o teste,
  a troca funcionou quando permitido.
- `Broadcast: comando não encontrado`: texto exibido pelo `wall` foi copiado para
  o shell como se fosse comando. Somente linhas explicitamente marcadas como
  comandos devem ser executadas.
- `@reboot: comando não encontrado`: linhas de crontab não são comandos para o
  shell interativo; devem ficar dentro de `sudo crontab -e`.
- `sudoreplay ... arquivo inexistente`: o replay procurou o diretório padrão;
  nossa configuração exige `-d /var/log/sudo`.
- `wall` sem mensagem: faltava `/run/utmp`, corrigido com systemd-tmpfiles.

## 14. Auditoria antes da entrega

Parte obrigatória:

```bash
hostnamectl --static
id guilamar
getent group user42
lsblk
systemctl is-enabled ssh apparmor ufw cron
systemctl is-active ssh apparmor ufw cron
sudo ss -ltnp
sudo ufw status numbered
sudo sshd -T | grep -E '^(port|permitrootlogin|passwordauthentication)'
sudo visudo -cf /etc/sudoers
sudo chage -l guilamar
sudo chage -l root
sudo crontab -l
bash -n /usr/local/bin/monitoring.sh
```

Também devemos confirmar que não existe ambiente gráfico instalado e testar que
o login SSH de `root` é negado. Depois do bônus, a lista de portas deverá incluir
somente as que conseguimos justificar.

### Resultado da auditoria de serviços e portas

Após concluir o bônus, auditamos os sete serviços:

```bash
systemctl is-active ssh ufw apparmor cron lighttpd mariadb fail2ban
systemctl is-enabled ssh ufw apparmor cron lighttpd mariadb fail2ban
```

Todos retornaram `active` e `enabled`: estão executando agora e configurados para
iniciar com o sistema.

`ss -ltnup` mostrou:

- SSH em TCP 4242, IPv4 e IPv6;
- Lighttpd em TCP 80, IPv4 e IPv6;
- MariaDB em `127.0.0.1:3306`, acessível somente localmente;
- DHCP cliente em UDP 68 e 546.

Os sockets DHCP em estado `UNCONN` são usados pelo `dhcpcd` para obter e renovar
endereços; não são serviços adicionais publicados pelo UFW. O firewall permite
somente TCP 4242 e 80, em IPv4 e IPv6. A exposição do MariaDB apenas no loopback
permite acesso pelo WordPress sem publicar o banco na rede.

Também verificamos pacotes gráficos e servidores web proibidos:

```bash
dpkg -l |
awk '$1 == "ii" && $2 ~ /^(xserver-xorg|gnome-shell|plasma-desktop|apache2|nginx)/ {
    print $2
}'
```

O comando não retornou nada: não há X.Org, GNOME Shell, Plasma, Apache ou Nginx
instalado. A ausência de saída é o resultado positivo desse teste negativo.

### Teste de persistência após reboot

Antes de reiniciar, validamos `sudoers`, Fail2ban, sintaxe do monitoramento e a
crontab. Depois executamos `sudo reboot`, desbloqueamos o LUKS no console e
repetimos os testes.

Após o boot:

- SSH, UFW, AppArmor, cron, Lighttpd, MariaDB e Fail2ban retornaram `active`;
- Fail2ban carregou novamente a jail `sshd`;
- SSH voltou a escutar na porta 4242 em IPv4 e IPv6;
- Lighttpd voltou a escutar na porta 80 em IPv4 e IPv6;
- MariaDB permaneceu restrito a `127.0.0.1:3306`;
- `wp-config.php` permaneceu `root:www-data` com modo `640`;
- o WordPress retornou `HTTP/1.1 200 OK` pelo NAT em
  `http://127.0.0.1:8080/`.

Esse teste é diferente de verificar apenas `enabled`: ele demonstra que, após
uma inicialização real, unidades, arquivos, firewall, banco, aplicação e regras
externas voltaram a funcionar em conjunto.

### Repositórios Debian stable

Confirmamos as fontes do APT com:

```bash
grep -RhsE '^(Suites:|deb )' /etc/apt/sources.list /etc/apt/sources.list.d/
```

Foram encontradas somente as suítes `trixie`, `trixie-security` e
`trixie-updates`; não havia `testing`, `unstable` ou `forky`. Isso comprova que a
VM acompanha o Debian 13 stable, incluindo correções de segurança e atualizações
pontuais da mesma versão estável.

O componente `non-free-firmware` não transforma a distribuição em unstable. Ele
é uma seção do repositório para firmwares cuja licença não atende integralmente
às Debian Free Software Guidelines, útil para compatibilidade de hardware.

## 15. Entrega

O VDI não deve ser enviado ao Git. O repositório de entrega recebe o `README.md`
exigido pelo subject e `signature.txt`. A assinatura é calculada sobre o arquivo
de disco virtual final.

Fluxo seguro no fim:

1. concluir configurações e testes;
2. desligar completamente a VM;
3. localizar o VDI correto;
4. calcular seu SHA-1 conforme o sistema hospedeiro;
5. colocar somente o hash em `signature.txt`;
6. não iniciar nem modificar a VM depois disso sem recalcular a assinatura.

O VDI pode ser transportado por armazenamento externo ou transferência privada,
mas normalmente é grande demais e inadequado para Git/GitHub. O hash identifica
o estado exato do disco: qualquer alteração muda a assinatura.

## 16. Perguntas que você deve conseguir responder

1. Por que `/boot` não está dentro do LUKS?
2. Qual é a diferença entre LUKS e LVM?
3. Por que separar `/var/log`, `/tmp` e `/home`?
4. Por que usar `sudo` em vez de permanecer logado como `root`?
5. O que `secure_path` evita?
6. Qual a diferença entre uma porta `LISTEN` e uma conexão `ESTABLISHED`?
7. Por que abrir a porta tanto no UFW quanto no redirecionamento NAT?
8. Qual a diferença entre `PASS_MIN_DAYS` e `minlen`?
9. Como o cron interpreta `*/10 * * * *`?
10. Por que o script define `LC_ALL=C`?
11. Como provar que AppArmor, SSH, UFW e cron estão ativos e persistem no boot?
12. Por que `php --version` ainda não comprova a integração PHP/Lighttpd?
