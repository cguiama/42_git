# Born2beRoot — roteiro completo da avaliação

Este roteiro segue a ordem da régua fornecida em 7 de setembro de 2026. Use-o
para ensaiar a defesa até conseguir repetir os procedimentos e explicações sem
consultá-lo.

> Atenção: blocos marcados como **explicação** não são comandos. Execute somente
> os blocos `bash`, `sql` ou os procedimentos numerados. Nunca digite palavras
> como `novo_usuario`, `novo_hostname` ou caminhos de exemplo literalmente:
> substitua pelo valor solicitado pelo avaliador.

## Convenções deste roteiro

Há três lugares diferentes onde os comandos podem ser executados:

- **Hospedeiro**: terminal do computador físico da 42;
- **Console da VM**: tela preta aberta diretamente no VirtualBox;
- **Sessão SSH**: terminal remoto conectado à VM.

No ensaio deste roteiro usamos:

```text
usuário temporário: evaluser
grupo pedido pela régua: avaliando
hostname temporário: evaluation42
hostname final: guilamar42
```

Durante a defesa, use o nome de usuário e o hostname pedidos pelo avaliador.
Nunca escreva senhas neste roteiro, no README, no Git ou em capturas de tela.

---

## Etapa 0 — preparação em casa

Antes de ir à 42:

1. conclua todas as configurações e a personalização opcional;
2. desligue a VM completamente;
3. confirme que ela não tem snapshots;
4. transporte a pasta da VM ou o VDI conforme `TRANSPORT.md`;
5. na 42, recrie as regras NAT e teste tudo;
6. desligue novamente;
7. calcule o SHA-1 final;
8. crie `signature.txt`;
9. envie somente `README.md` e `signature.txt` no repositório avaliado.

Os arquivos `COMMANDS.md`, `STUDY_GUIDE.md`, `TRANSPORT.md`,
`EVALUATION_CHECKLIST.md`, `EVALUATION_ROADMAP.md` e a cópia local do script são
materiais de estudo; não devem entrar no commit de entrega.

---

## Etapa 1 — preliminares, antes de iniciar a VM

### 1.1 Clonar e conferir o repositório

No **hospedeiro**:

```bash
git clone URL_DO_REPOSITORIO
cd born2beroot
git ls-files
```

Resultado esperado: somente `README.md` e `signature.txt` entre os arquivos
entregues.

### 1.2 Comparar a assinatura

Ainda no **hospedeiro**, localize o VDI correto pelo VirtualBox. Em Linux, a
pasta padrão costuma ser `~/VirtualBox VMs/`.

```bash
cat signature.txt
sha1sum "/caminho/real/Born2Reboot.vdi"
```

Os dois hashes devem ser idênticos. Para usar `diff`:

```bash
sha1sum "/caminho/real/Born2Reboot.vdi" | awk '{print $1}' > /tmp/vdi-signature.txt
diff signature.txt /tmp/vdi-signature.txt
```

Resultado esperado: `diff` sem saída.

Não digite `/caminho/real/...` literalmente. O hash muda assim que o disco da VM
é modificado.

### 1.3 Conferir VM e snapshots

No VirtualBox:

1. confirme que a VM está **Desligada**, não em estado salvo;
2. abra a seção de snapshots;
3. confirme que não existe snapshot anterior ao início da avaliação.

A régua permite criar um snapshot frio dedicado depois dessa verificação e
apagá-lo ao final. Outra opção é duplicar o VDI e iniciar a cópia, preservando o
arquivo assinado.

Não aceite iniciar uma VM que já estava executando antes da defesa.

---

## Etapa 2 — README

No **hospedeiro**:

```bash
sed -n '1,240p' README.md
```

Mostre:

- primeira linha em itálico com `guilamar`;
- descrição e objetivo;
- instruções;
- escolha do Debian, vantagens e desvantagens;
- particionamento, segurança, usuários e serviços;
- Debian versus Rocky;
- AppArmor versus SELinux;
- UFW versus firewalld;
- VirtualBox versus UTM;
- recursos e uso de IA.

Se qualquer item estiver ausente na entrega, a régua encerra a avaliação.

---

## Etapa 3 — iniciar e explicar a VM

Inicie a VM no VirtualBox e digite a senha do LUKS no **console da VM**. Faça
login como `guilamar`, nunca como `root`.

### Explicação curta: máquina virtual

Uma VM é um computador implementado em software. O VirtualBox fornece CPU,
memória, disco, firmware e rede virtuais; o Debian convidado usa esses recursos
como se fossem hardware. Isso permite isolamento, reprodução e testes sem
instalar o servidor diretamente no hospedeiro.

### Explicação curta: por que Debian

Escolhemos Debian por sua versão stable conservadora, documentação, APT e
integração padrão com AppArmor e UFW. Rocky segue o ecossistema RHEL, usando
normalmente DNF, SELinux e firewalld; é forte no ambiente empresarial, mas tem
uma curva inicial maior para esta instalação.

### Explicação curta: apt versus aptitude

`apt` é a interface de linha de comando padrão para operações comuns com pacotes
e usa as bibliotecas do APT. `aptitude` é uma interface separada, textual e
interativa, com outro resolvedor de dependências e comandos de busca. Os dois
trabalham sobre o sistema de pacotes `dpkg`; instalar `aptitude` não substitui
`apt` nem muda o formato dos pacotes.

### Explicação curta: AppArmor

AppArmor é controle de acesso obrigatório baseado principalmente em caminhos.
Se um processo for comprometido, seu perfil ainda pode limitar arquivos,
capacidades e ações disponíveis. Ele complementa permissões Unix, sudo e UFW.

Mostre:

```bash
cat /etc/os-release
uname -a
systemctl is-active apparmor
systemctl is-enabled apparmor
sudo aa-status | head
```

---

## Etapa 4 — configuração simples

### 4.1 Provar que não há interface gráfica

```bash
dpkg -l |
awk '$1 == "ii" && $2 ~ /^(xserver-xorg|gnome-shell|plasma-desktop|wayland)/ {
    print $2
}'
```

Resultado esperado: nenhuma saída.

### 4.2 Mostrar sistema, firewall e SSH

```bash
systemctl is-active ufw ssh
systemctl is-enabled ufw ssh
sudo ufw status numbered
sudo ss -ltnp
```

Explique que `active` é o estado atual e `enabled` significa inicialização
automática. Na nossa VM, TCP 4242 é SSH e TCP 80 é Lighttpd do bônus.

---

## Etapa 5 — usuário e política de senhas

### 5.1 Mostrar o usuário principal

```bash
id guilamar
getent group sudo
getent group user42
```

`guilamar` deve aparecer em `sudo` e `user42`.

### 5.2 Criar o usuário temporário

No nosso ensaio, o nome será `evaluser`. Durante a avaliação, substitua-o pelo
nome solicitado.

```bash
sudo adduser evaluser
```

Escolha uma senha que satisfaça a política. Os campos de nome e telefone podem
ser deixados vazios pressionando `Enter`.

### 5.3 Criar exatamente o grupo da régua

```bash
sudo addgroup avaliando
sudo adduser evaluser avaliando
sudo adduser evaluser sudo
```

Verifique:

```bash
id evaluser
getent group avaliando
getent group sudo
sudo chage -l evaluser
```

O usuário deve aparecer em `avaliando` e `sudo`, e a conta nova deve mostrar
mínimo 2, máximo 30 e aviso 7.

### 5.4 Explicar onde a política foi configurada

```bash
grep -E '^[[:space:]]*PASS_(MAX_DAYS|MIN_DAYS|WARN_AGE)' /etc/login.defs
grep -vE '^[[:space:]]*(#|$)' /etc/security/pwquality.conf
grep -n pam_pwquality /etc/pam.d/common-password
```

- `/etc/login.defs`: padrões temporais para contas novas;
- `chage`: aplica ou consulta esses valores por conta;
- `/etc/security/pwquality.conf`: comprimento e composição da nova senha;
- `/etc/pam.d/common-password`: conecta `pam_pwquality` ao fluxo de senha.

Vantagens: reduz senhas triviais, repetição e permanência longa de credenciais
vazadas. Desvantagens: regras rígidas podem incentivar padrões previsíveis,
anotações inseguras e trocas mínimas; política de senha não substitui MFA,
proteção contra força bruta e treinamento do usuário.

---

## Etapa 6 — hostname e partições

### 6.1 Mostrar hostname final

```bash
hostnamectl --static
cat /etc/hostname
grep -n '127.0.1.1' /etc/hosts
```

Resultado inicial esperado: `guilamar42` em todos.

### 6.2 Alterar para o nome pedido e reiniciar

No ensaio usamos `evaluation42`. Durante a defesa, use o login do avaliador
seguido de `42`, se esse for o nome solicitado.

```bash
sudo hostnamectl set-hostname evaluation42
sudo nano /etc/hosts
```

No Nano, troque somente:

```text
127.0.1.1    guilamar42
```

por:

```text
127.0.1.1    evaluation42
```

Reinicie:

```bash
sudo reboot
```

Desbloqueie o LUKS no console, faça login e confirme:

```bash
hostnamectl --static
cat /etc/hostname
grep -n '127.0.1.1' /etc/hosts
```

### 6.3 Restaurar no ensaio sem snapshot

Se estiver ensaiando na VM real, restaure antes de continuar:

```bash
sudo hostnamectl set-hostname guilamar42
sudo nano /etc/hosts
```

Restaure a linha `127.0.1.1 guilamar42`, reinicie novamente e confira. Durante a
defesa, o avaliador pode preferir restaurar o snapshot frio.

### 6.4 Mostrar partições

```bash
lsblk -f
sudo pvs
sudo vgs
sudo lvs
df -h
free -h
```

Explique de baixo para cima:

```text
/dev/sda → partições → LUKS → dispositivo descriptografado → PV → VG LVMGroup
→ volumes lógicos → filesystems → pontos de montagem
```

LUKS protege os dados em repouso. LVM organiza o espaço desbloqueado em volumes
flexíveis. `/boot` precisa estar legível antes de o sistema abrir o LUKS. Os
volumes separados limitam o impacto de um diretório encher o disco. Swap recebe
páginas de memória menos ativas e é muito mais lenta que RAM.

---

## Etapa 7 — sudo

### 7.1 Mostrar instalação e autorização

```bash
dpkg -l sudo
id evaluser
sudo -l
sudo visudo -cf /etc/sudoers
```

O usuário recém-criado deve estar no grupo `sudo`.

### 7.2 Explicar sudo

`sudo` executa um comando com outra identidade, normalmente root, após aplicar
política e autenticação. É preferível a permanecer em uma shell root porque reduz
o tempo elevado, associa a ação ao usuário original e permite auditoria.

Mostre as regras:

```bash
sudo cat /etc/sudoers.d/born2beroot
```

Explique:

- três tentativas;
- mensagem personalizada;
- `log_input` e `log_output`;
- `requiretty`;
- `secure_path` sem diretórios graváveis como `/tmp`;
- logs em `/var/log/sudo/`.

### 7.3 Provar que o log é atualizado

```bash
sudo ls -la /var/log/sudo
sudo tail -n 20 /var/log/sudo/sudo.log
sudo whoami
sudo tail -n 20 /var/log/sudo/sudo.log
```

O novo `sudo whoami` deve gerar uma entrada com `COMMAND=/usr/bin/whoami` e um
TSID. Para reproduzir uma sessão gravada:

```bash
sudo sudoreplay -d /var/log/sudo NUMERO_TSID
```

Substitua `NUMERO_TSID` por um TSID realmente presente no log.

---

## Etapa 8 — UFW

### 8.1 Mostrar estado e regras

```bash
dpkg -l ufw
systemctl is-active ufw
systemctl is-enabled ufw
sudo ufw status numbered
```

UFW é uma interface para administrar o firewall. Ele reduz a superfície de
ataque permitindo somente tráfego necessário. Na configuração com bônus, 4242 é
SSH e 80 é HTTP.

### 8.2 Adicionar e remover TCP 8080

```bash
sudo ufw allow 8080/tcp
sudo ufw status numbered
```

Confirme as regras IPv4 e IPv6 para 8080. Depois remova:

```bash
sudo ufw delete allow 8080/tcp
sudo ufw status numbered
```

A porta 8080 deve desaparecer, mantendo 4242 e 80.

Essa porta 8080 é interna à VM e temporária. Não é a regra NAT do VirtualBox em
que a porta 8080 do hospedeiro encaminha para a porta 80 da VM.

---

## Etapa 9 — SSH

### 9.1 Mostrar configuração

```bash
dpkg -l openssh-server
systemctl is-active ssh
systemctl is-enabled ssh
sudo sshd -T | grep -E '^(port|permitrootlogin|passwordauthentication)'
sudo ss -ltnp
```

Esperado:

```text
port 4242
permitrootlogin no
passwordauthentication yes
```

SSH oferece shell remoto criptografado, autentica o servidor e o usuário e
protege comandos contra leitura e alteração durante o transporte.

### 9.2 Testar o usuário recém-criado

No **hospedeiro**:

```bash
ssh -p 4242 evaluser@127.0.0.1
```

Digite a senha temporária. Depois confirme:

```bash
whoami
hostname
id
exit
```

### 9.3 Provar que root é recusado

No **hospedeiro**:

```bash
ssh -o PreferredAuthentications=password -o PubkeyAuthentication=no -o NumberOfPasswordPrompts=1 -p 4242 root@127.0.0.1
```

Resultado esperado: `Permission denied`. Uma tentativa não atinge o limite de
cinco falhas do Fail2ban.

---

## Etapa 10 — monitoring e cron

### 10.1 Mostrar arquivo, permissões e conteúdo

Na VM:

```bash
sudo stat -c 'arquivo=%n dono=%U grupo=%G permissao=%a tamanho=%s' /usr/local/bin/monitoring.sh
sudo sha256sum /usr/local/bin/monitoring.sh
bash -n /usr/local/bin/monitoring.sh
sudo cat -n /usr/local/bin/monitoring.sh
```

Anote o SHA-256 para provar depois que o conteúdo não mudou. Explique cada uma
das doze métricas; a explicação completa está no `STUDY_GUIDE.md`.

### 10.2 Mostrar o cron atual

```bash
systemctl is-active cron
systemctl is-enabled cron
sudo crontab -l
```

Esperado:

```cron
@reboot /usr/local/bin/monitoring.sh
*/10 * * * * /usr/local/bin/monitoring.sh
```

Os cinco campos são minuto, hora, dia do mês, mês e dia da semana. `*/10` no
primeiro campo significa cada múltiplo de dez minutos.

### 10.3 Guardar a configuração antes do teste

Durante o ensaio, faça uma cópia recuperável:

```bash
sudo crontab -l > ~/root-crontab.backup
cat ~/root-crontab.backup
```

### 10.4 Alterar para cada minuto

```bash
sudo crontab -e
```

Mantenha `@reboot` e troque apenas a segunda linha para:

```cron
* * * * * /usr/local/bin/monitoring.sh
```

Salve e confira:

```bash
sudo crontab -l
```

Aguarde duas viradas de minuto. Devem aparecer broadcasts sucessivos; memória,
CPU, conexões e quantidade de comandos sudo podem mudar porque são valores
dinâmicos.

### 10.5 Impedir execução automática sem editar o script

Abra novamente:

```bash
sudo crontab -e
```

Comente as duas tarefas acrescentando `#` no início:

```cron
# @reboot /usr/local/bin/monitoring.sh
# * * * * * /usr/local/bin/monitoring.sh
```

O arquivo `/usr/local/bin/monitoring.sh` não foi editado; apenas o agendador foi
desabilitado para esse script.

Reinicie:

```bash
sudo reboot
```

Desbloqueie o LUKS e aguarde mais de um minuto. Não deve surgir broadcast do
monitoring. Confira que o arquivo permaneceu intacto:

```bash
sudo stat -c 'arquivo=%n dono=%U grupo=%G permissao=%a tamanho=%s' /usr/local/bin/monitoring.sh
sudo sha256sum /usr/local/bin/monitoring.sh
bash -n /usr/local/bin/monitoring.sh
sudo crontab -l
```

Compare SHA-256, proprietário, permissões e caminho com os valores anteriores.

### 10.6 Restaurar o estado final depois do ensaio

```bash
sudo crontab ~/root-crontab.backup
sudo crontab -l
rm ~/root-crontab.backup
```

O estado final deve voltar a `@reboot` e `*/10`. Durante a avaliação, o snapshot
frio pode restaurar o estado original em vez dessa etapa manual.

---

## Etapa 11 — bônus

O bônus só é avaliado se toda a parte obrigatória estiver perfeita.

### 11.1 Particionamento

```bash
lsblk -f
sudo pvs
sudo vgs
sudo lvs
```

Mostre `/boot`, LUKS, `LVMGroup` e os LVs para `/`, swap, `/home`, `/var`,
`/srv`, `/tmp` e `/var/log`.

### 11.2 WordPress

Na VM:

```bash
systemctl is-active lighttpd mariadb
systemctl is-enabled lighttpd mariadb
sudo ss -ltnp
sudo mariadb -e 'USE wordpress; SHOW TABLES;'
sudo stat -c 'arquivo=%n dono=%U grupo=%G permissao=%a' /var/www/html/wp-config.php
```

Esperado: Lighttpd e MariaDB ativos; MariaDB somente em `127.0.0.1:3306`;
WordPress com suas tabelas; `wp-config.php` como `root:www-data 640`.

No **hospedeiro**:

```bash
curl -I http://127.0.0.1:8080/
```

Abra também no navegador:

```text
http://127.0.0.1:8080/
http://127.0.0.1:8080/wp-admin/
```

Explique o fluxo: navegador → NAT 8080/80 → Lighttpd → FastCGI → PHP →
WordPress → MariaDB.

### 11.3 Serviço adicional: Fail2ban

```bash
systemctl is-active fail2ban
systemctl is-enabled fail2ban
sudo fail2ban-client -t
sudo fail2ban-client status
sudo fail2ban-client status sshd
sudo cat /etc/fail2ban/jail.d/sshd.local
```

Justificativa curta: Fail2ban observa falhas de autenticação SSH no journal e
bane temporariamente o IP que excede cinco falhas em dez minutos. Ele complementa
o UFW: o firewall permite a porta, enquanto Fail2ban reage ao comportamento de
quem a utiliza.

Não provoque falhas reais usando o IP NAT durante a avaliação. Se pedirem teste
controlado, use o endereço reservado:

```bash
sudo fail2ban-client set sshd banip 192.0.2.1
sudo fail2ban-client status sshd
sudo fail2ban-client set sshd unbanip 192.0.2.1
```

---

## Etapa 12 — limpeza do ensaio

Esta seção é apenas para a prática na VM sem snapshot. Não apague o usuário no
meio do teste SSH.

Confirme que saiu da sessão SSH de `evaluser`. Depois:

```bash
sudo deluser --remove-home evaluser
sudo delgroup avaliando
getent passwd evaluser
getent group avaliando
```

As duas últimas consultas devem ficar vazias.

Estado final:

```bash
hostnamectl --static
sudo crontab -l
sudo ufw status numbered
getent passwd |
awk -F: '$3 == 0 || ($3 >= 1000 && $3 < 65534) {print $1, $3, $7}'
```

Confirme:

- hostname `guilamar42`;
- monitoring no boot e a cada dez minutos;
- UFW com 4242 e 80, sem 8080 interno;
- somente `root` e `guilamar` como contas administrativas/humanas esperadas.

---

## Etapa 13 — resultado do ensaio

Considere o ensaio aprovado somente se você conseguir:

- escolher o comando correto sem copiar uma explicação para o shell;
- prever o resultado antes de executar;
- explicar por que o resultado comprova o requisito;
- desfazer com segurança todas as alterações temporárias;
- responder às perguntas sem ler frases prontas.

Depois deste ensaio, personalize o blog, faça uma última auditoria na 42,
desligue a VM e gere a assinatura definitiva.
